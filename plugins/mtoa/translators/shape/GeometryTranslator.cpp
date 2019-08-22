#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "GeometryTranslator.h"

#include <maya/MNodeMessage.h>
#include <maya/MBoundingBox.h>
#include <maya/MUintArray.h>
#include <maya/MItMeshEdge.h>
#include "utils/MtoaLog.h"
#include <algorithm>

#include "utils/time.h"

namespace
{
   
   void SetKeyData(AtArray* arr, unsigned int key, const float* data)
   {
      if (AiArrayGetType(arr) != AI_TYPE_VECTOR)
         return;

      AtVector *vectorList = (AtVector*)AiArrayMapKey(arr, key);
      memcpy(vectorList, data, AiArrayGetKeySize(arr));
      AiArrayUnmap(arr);      
   }
}

bool CPolygonGeometryTranslator::GetVertices(const MObject& geometry,
                                      const float*& vertices)
{
   MStatus status;
   MFnMesh fnMesh(geometry);
   int nverts = fnMesh.numVertices();
   if (nverts > 0)
   {
      vertices = fnMesh.getRawPoints(&status);
      if (status)
         return true;
      else
         return false;
   }
   return false;
}

bool CPolygonGeometryTranslator::GetPerVertexNormals(const MObject &geometry,
                                              AtArray*& normals,
                                              MSpace::Space space,
                                              bool force)
{
   MStatus status;
   MFnMesh fnMesh(geometry);

   int nnorms = fnMesh.numNormals();
   if (nnorms > 0 && (force || (FindMayaPlug("smoothShading").asBool() && !FindMayaPlug("aiSubdivType").asBool())))
   {
      int numVerts = fnMesh.numVertices();
      normals = AiArrayAllocate(numVerts, 1, AI_TYPE_VECTOR);

      MFloatVectorArray normalArray;
      if (space == MSpace::kWorld)
      {
         // A mesh has no transform, we must trace it back in the DAG
         // FIXME : won't work for smoothed mesh or tessellated nurbs
         MDagPath dp;
         status = fnMesh.getPath(dp);
         CHECK_MSTATUS(status);
         MFnMesh normFn(dp);
         normFn.getVertexNormals(false, normalArray, MSpace::kWorld);
      }
      else
      {
         fnMesh.getVertexNormals(false, normalArray, space);
      }

      for (int J = 0; J < numVerts; ++J)
      {
         AtVector atv;
         atv.x = normalArray[J].x;
         atv.y = normalArray[J].y;
         atv.z = normalArray[J].z;
         AiArraySetVec(normals, J, atv);
      }
      return true;
   }
   return false;
}

bool CPolygonGeometryTranslator::GetNormals(const MObject& geometry,
                                     const float*& normals)
{
   MFnMesh fnMesh(geometry);
   int nnorms = fnMesh.numNormals();
   // TODO this should be checked outside this function!
   if (FindMayaPlug("smoothShading").asBool() &&
         !FindMayaPlug("aiSubdivType").asBool() &&
         nnorms > 0)
   {
      MStatus status;
      normals = fnMesh.getRawNormals(&status);
      if (status)
         return true;
      else
         return false;
   }
   return false;
}

bool CPolygonGeometryTranslator::GetTangents(const MObject &geometry,
                                      AtArray*& tangents,
                                      AtArray*& bitangents,
                                      AtArray*& tangentidxs,
                                      AtArray*& bitangentidxs,
                                      MSpace::Space space,
                                      bool force)
{
   MStatus status;
   MFnMesh fnMesh(geometry);

   bool doExport;
   if (force)
      doExport = true;
   else
   {
      MPlug pExportTangents = fnMesh.findPlug("aiExportTangents", false, &status);
      doExport = (status == MStatus::kSuccess && pExportTangents.asBool());
   }

   if (!doExport)
      return false;

   MObject meshObject = fnMesh.object();
   
   MFnMesh tangentFn;
   if (space == MSpace::kWorld)
   {
      // A mesh has no transform, we must trace it back in the DAG
      MDagPath dp;
      status = fnMesh.getPath(dp);
      CHECK_MSTATUS(status);
      tangentFn.setObject(dp);
   }
   else
      tangentFn.setObject(fnMesh.object());  
   
   MFloatVectorArray mayaTangents;
   MFloatVectorArray mayaBitangents;
   
   status = tangentFn.getTangents(mayaTangents, space);
   if (!status)
      return false;
   status = tangentFn.getBinormals(mayaBitangents, space);
   if (!status)
      return false;

   int nFaceVerts = fnMesh.numFaceVertices();
   int nVerts = fnMesh.numVertices();

   std::vector<AtVector> perVertexTangent;
   std::vector<AtVector> perVertexBitangent;

   perVertexTangent.assign(nVerts, AtVector(0.f,0.f,0.f));
   perVertexBitangent.assign(nVerts, AtVector(0.f,0.f,0.f));
   std::vector<bool> foundVtx(nVerts, false);

   std::vector<AtVector> indexedTangent(mayaTangents.length());
   std::vector<AtVector> indexedBitangent(mayaBitangents.length());
   std::vector<unsigned int> indexedTangentIdxs(nFaceVerts, 0);
   
   // FIXME is there another way to copy the vectors from Maya to Arnold ? can we assume they store the data the same way ?
   for (unsigned int i = 0, tangentsLength = mayaTangents.length(); i < tangentsLength; ++i)
      indexedTangent[i] = AtVector(mayaTangents[i].x, mayaTangents[i].y, mayaTangents[i].z);
   for (unsigned int i = 0, bitangentsLength = mayaBitangents.length(); i < bitangentsLength; ++i)
      indexedBitangent[i] = AtVector(mayaBitangents[i].x, mayaBitangents[i].y, mayaBitangents[i].z);

   // We will try to set per-vertex tangents / bitangents. If we find a difference for a same vertex on different faces, 
   // we'll switch to indexed data
   MItMeshPolygon iter(fnMesh.object());
   MIntArray vids;

   bool needFaceVertexData = false;

   int fvidx = 0;
   for (;!iter.isDone(); iter.next())
   {
      iter.getVertices(vids);
      for (unsigned int i = 0; i < vids.length(); ++i, ++fvidx)
      {
         unsigned int tid = iter.tangentIndex(i);

         if (!needFaceVertexData)
         {
            // so far we didn't spot any difference for vertices per face, so let's try to make it work this way
            unsigned int vtxId = vids[i];
            if (!foundVtx[vtxId])
            {
               // first time we see this vertex, let's fill the arrays
               perVertexTangent[vtxId] = indexedTangent[tid];
               perVertexBitangent[vtxId] = indexedBitangent[tid];
               foundVtx[vtxId] = true;
            } else
            {
               // we already found this vertex, let's see if the current tangent is different from the one stored previously
               AtVector diff = perVertexTangent[vtxId] - indexedTangent[tid];
               if (std::abs(diff.x) > AI_EPSILON || std::abs(diff.y) > AI_EPSILON || std::abs(diff.z) > AI_EPSILON)
                  needFaceVertexData = true; // we can't continue with per-vertex data, need to go with indexed user data
            }
         }
         indexedTangentIdxs[fvidx] = tid;
      }
   }
   if (needFaceVertexData)
   {
      // we need to store per-face-vertex data
      tangents = AiArrayConvert(indexedTangent.size(), 1, AI_TYPE_VECTOR, &indexedTangent[0]);
      bitangents = AiArrayConvert(indexedBitangent.size(), 1, AI_TYPE_VECTOR, &indexedBitangent[0]);

      tangentidxs = AiArrayConvert(nFaceVerts, 1, AI_TYPE_UINT, &indexedTangentIdxs[0]);
      bitangentidxs = AiArrayConvert(nFaceVerts, 1, AI_TYPE_UINT, &indexedTangentIdxs[0]);
   } else
   {
      // Awesome, we can store a per-vertex (varying) user data. The results for the difference faces were identical
      // We create tangents / bitangent arrays per-vertex and set the idxs arrays to null.
      tangents = AiArrayConvert(nVerts, 1, AI_TYPE_VECTOR, &perVertexTangent[0]);
      bitangents = AiArrayConvert(nVerts, 1, AI_TYPE_VECTOR, &perVertexBitangent[0]);      
      tangentidxs = bitangentidxs = NULL;
   }

   return true;
}

MDagPath CPolygonGeometryTranslator::GetMeshRefObj()
{
   MFnMesh fnMesh(m_dagPath);

   // If it has already been set (by the smooth reference fix)
   if (!m_dagPathRef.isValid())
   {
      MStatus stat;
      MObject referenceObj;
      // Check if there is a referenceObject plug
      MPlug pReferenceObject = fnMesh.findPlug("referenceObject", false, &stat);
      if (stat != MStatus::kSuccess)
      {
         return m_dagPathRef;
      }
      MPlugArray connectedPlugs;
      // Check if anything is connected to .referenceObject plug
      pReferenceObject.connectedTo(connectedPlugs, true, true, &stat);
      if (stat != MStatus::kSuccess || 1 != connectedPlugs.length())
      {
         return m_dagPathRef;
      }
      else
      {
         // Checking if a script is not going rogue : the object must be a mesh !
         referenceObj = connectedPlugs[0].node();
         if (referenceObj.hasFn(MFn::kMesh) != 1)
         {
            return m_dagPathRef;
         }
      }

      MFnMesh fnMeshRef(referenceObj);
      // Check if the numbers of vertices is the same as the current object
      if (fnMeshRef.numVertices() != fnMesh.numVertices())
      {
         return m_dagPathRef;
      }

      MFnDagNode dag_node(referenceObj);
      dag_node.getPath(m_dagPathRef);
   }

   return m_dagPathRef;
}

bool CPolygonGeometryTranslator::GetRefObj(const float*& refVertices,
                                    AtArray*& refNormals,
                                    AtArray*& rnidxs,
                                    AtArray*& refTangents,
                                    AtArray*& refBitangents,
                                    AtArray*& reftangentidxs,
                                    AtArray*& refBitangentidxs,
                                    const std::vector<unsigned int> &polyVtxRemap)
{
   MFnMesh fnMesh(m_dagPath);
   MDagPath dagPathRef = GetMeshRefObj();
   MObject geometryRef = dagPathRef.node();   

   if (dagPathRef.isValid())
   {
      MFnMesh fnRefMesh(dagPathRef);
      // Find whether we're exporting points/normals/tangents
      MStatus stat;
      MPlug pExportRefPoints = fnMesh.findPlug("aiExportRefPoints", false,
                                               &stat);
      if (stat == MStatus::kSuccess && pExportRefPoints.asBool())
      {
         // Get vertices of the reference object in world space
         refVertices = fnRefMesh.getRawPoints(&stat);
      }

      MPlug pExportRefNormals = fnMesh.findPlug("aiExportRefNormals", false,
                                                &stat);
      if (stat == MStatus::kSuccess && pExportRefNormals.asBool())
      {
         // Get normals of the reference object
         unsigned int rNumNorms = fnRefMesh.numNormals();
         refNormals = AiArrayConvert(rNumNorms, 1, AI_TYPE_VECTOR, &(fnRefMesh.getRawNormals(&stat)[0]));
         
         MIntArray vertex_counts, normal_ids;
         
         fnRefMesh.getNormalIds(vertex_counts, normal_ids);
         // normal_ids should have the same length as the amount of polygon vertices
         rnidxs = AiArrayAllocate((int)normal_ids.length(), 1, AI_TYPE_UINT);
         
         if (polyVtxRemap.empty())
            for(unsigned int n = 0; n < normal_ids.length(); ++n) AiArraySetUInt(rnidxs, n, normal_ids[n]);
         else
            for(unsigned int n = 0; n < normal_ids.length(); ++n) AiArraySetUInt(rnidxs, polyVtxRemap[n], normal_ids[n]);
      }

      MPlug pExportRefTangents = fnMesh.findPlug("aiExportRefTangents", false,
                                                 &stat);
      if (stat == MStatus::kSuccess && pExportRefTangents.asBool())
      {
         // Get tangents of the reference object in world space
         // Also, even if subdivision is applied we want to get the tangent data
         GetTangents(geometryRef, refTangents, refBitangents,reftangentidxs, refBitangentidxs, MSpace::kWorld, true);
      }
      // If we are using a smoothed reference object, we need to delete it.
      //FIXME : This is somehow dirty, but I can't find a way to have a real "virtual" DAG object from generateSmoothMesh.
      if (m_isRefSmooth)
      {
         MObject node = m_dagPathRef.node();
         MGlobal::removeFromModel(node);
         m_dagPathRef = MDagPath();
         m_isRefSmooth = false;
      }
      return true;
   }
   else
      return false;
}

bool CPolygonGeometryTranslator::GetUVs(const MObject &geometry,
                                 std::vector<AtArray*>& uvs,
                                 std::vector<MString>& uvNames)
{
   MFnMesh fnMesh(geometry);

   // Get all UVs
   int numUVSets = fnMesh.numUVSets();
   MStringArray uvns;
   fnMesh.getUVSetNames(uvns);
   for (int i = 0; i < numUVSets; ++i)
   {
      MString uvName = uvns[i];
      int numUVs = fnMesh.numUVs(uvName);

      AtArray* uv = AiArrayAllocate(numUVs, 1, AI_TYPE_VECTOR2);
      
      MFloatArray uArray, vArray;
      fnMesh.getUVs(uArray, vArray, &uvName);
      
      for (int j = 0; j < numUVs; ++j)
      {
         AtVector2 atv;
         atv.x = uArray[j];
         atv.y = vArray[j];
         AiArraySetVec2(uv, j, atv);
      }
      uvs.push_back(uv);
      uvNames.push_back(uvName);
   }
   return uvs.size() > 0;
}

bool CPolygonGeometryTranslator::GetVertexColors(const MObject &geometry,
                                          unordered_map<std::string, std::vector<float> > &vcolors,
                                          const std::vector<unsigned int> &polyVtxRemap)
{
   MFnMesh fnMesh(geometry);

   bool exportColors = false;
   unsigned int numFaceVertices = fnMesh.numFaceVertices();

   if (fnMesh.numColorSets() > 0)
   {
      MPlug plug = fnMesh.findPlug("aiExportColors", true);
      if (!plug.isNull())
         exportColors = plug.asBool();
   }
   MStringArray names;
   fnMesh.getColorSetNames(names);
   unsigned int numColorSets = names.length();
   m_useMotionVectors = false;
   if (m_motionDeform && IsLocalMotionBlurEnabled())
   {
      for (unsigned int j = 0; j < numColorSets; ++j)
      {
         if (names[j] == m_motionVectorSource.asChar())
            m_useMotionVectors = true;
      }
   }

   if (!exportColors)
   {
      if (m_useMotionVectors)
      {
         names.clear();
         names.append(m_motionVectorSource);
         numColorSets = 1;
      }
      else
         numColorSets = 0;      
   }

   if (numColorSets)
   {      
      MIntArray faces;
      unsigned int i = 0;
      int dim = 4;
      MColor col;

      for (unsigned int j=0; j < numColorSets; ++j)
      {
         bool found_diff = false;

         std::vector<float> &colors = vcolors[names[j].asChar()];
         colors.resize(fnMesh.numVertices() * dim, 0.0f);

         MObject meshObject = fnMesh.object();
         MItMeshVertex itVertex(meshObject);

         // Since #2725, we support indexed data per face-vertex. So we first try to export user data per-vertex,
         // and if we find any difference we switch to indexed data.
         while (!itVertex.isDone())
         {
            faces.clear();
            itVertex.getConnectedFaces(faces);

            if (faces.length() > 0)
            {  
               i = itVertex.index() * dim;

               for (unsigned int k=0; k<faces.length(); ++k)
               {
                  itVertex.getColor(col, faces[k], &names[j]);
                  for (int l=0; l<dim; ++l)
                  {
                     if (k == 0)
                        colors[i+l] = col[l];
                     else if (std::abs(colors[i+l] - col[l]) > AI_EPSILON)
                     {
                        // found a difference between 2 vertex faces. We need to output "indexed" data instead.
                        found_diff = true;
                        break;
                     }
                  }
                  if (found_diff)
                     break;
               }    
            }
            if (found_diff)
               break;

            itVertex.next();
         }

         if (found_diff)
         {
            // We can't export "varying" per-vertex user data.
            // So we're going to export the colors per face-vertex

            MColorArray face_vtx_colors;

            // FIXME : in the first "per-vertex" code we don't set default color to black.
            // So we can get -1 values when no color was painted for a given vertex. Do we want to keep that feature ?
            MColor unsetColor(0.f, 0.f, 0.f, 0.f);
            MStatus status = fnMesh.getFaceVertexColors(face_vtx_colors, &names[j], &unsetColor);

            if (status == MS::kSuccess)
            {
               colors.assign(numFaceVertices * dim, 0.f);

               if (polyVtxRemap.empty())
                  memcpy(&colors[0], &face_vtx_colors[0], numFaceVertices * sizeof(MColor));
               else
               {
                  int colorId = 0;
                  for (unsigned int f = 0; f < numFaceVertices; ++f)
                  {
                     colors[colorId++] = face_vtx_colors[f][0];
                     colors[colorId++] = face_vtx_colors[f][1];
                     colors[colorId++] = face_vtx_colors[f][2];
                  }
               }
            }
         }
      }
   }
   return exportColors;
}

bool CPolygonGeometryTranslator::GetComponentIDs(const MObject &geometry,
      AtArray*& nsides,
      AtArray*& vidxs,
      AtArray*& nidxs,
      std::vector<AtArray*>& uvidxs,
      const std::vector<MString>& uvNames,
      bool exportNormals,
      bool exportUVs, 
      std::vector<unsigned int> &polyVtxRemap,
      std::vector<unsigned int> &arnoldPolygonHoles)
{   

   MFnMesh fnMesh(geometry);

   int uv_id = 0;
   // Traverse all polygons to export vidxs, uvindxs and nsides
   unsigned int np = fnMesh.numPolygons();
   size_t numUVSets = uvNames.size();

   //While calling this function we assume that returning false
   // means "no polygons in this mesh"
   if (np == 0) return false;

   
   std::vector<unsigned int> nsides_list;
   nsides_list.resize(np, 0u);

   bool hasHoles = false;
   int numHoles = 0;
   MIntArray holeInfoArray, holeVertexArray;
   std::vector<unsigned int> polygonHoleVertices;

   int holeIndex, faceIndex, holeFaceIndex, h, numPolygonVertexCount;
   unsigned int polygonVertexCount = 0; // for counting the number of ids
   
   // Loop over mesh polygons in order to get the amount of vertices per polygon
   MItMeshPolygon pit(geometry);
   while(!pit.isDone())
   {
      faceIndex = pit.index(); // polygon index

      // amount of vertices in this polygon
      numPolygonVertexCount = pit.polygonVertexCount();

      // check if this polygon has a hole
      if (numPolygonVertexCount >= 6 && pit.isHoled()) 
      {
         if (!hasHoles)
         {  
            // first time we find a hole in this mesh, let's
            // initialize the holes data
            hasHoles = true;
            MStatus status;
            numHoles = fnMesh.getHoles(holeInfoArray, holeVertexArray, &status);
            if (status != MS::kSuccess)
               numHoles = 0;  

            holeIndex = 0;
            for (h = 0; h < numHoles; ++h, holeIndex += 3)
            {
               // polygon on which this hole lies
               holeFaceIndex = holeInfoArray[holeIndex];
               if (holeFaceIndex >= (int)polygonHoleVertices.size())
               {
                  // resize dynamically the polygonHoleVertices table
                  polygonHoleVertices.resize(holeFaceIndex + 1, 0);
               }
               // add the amount of hole vertices to the list for each polygon
               polygonHoleVertices[holeFaceIndex] += holeInfoArray[holeIndex + 1];
            }
         }         

         if (faceIndex < (int)polygonHoleVertices.size())
         {
            // subtracting the amount of hole-vertices in this face
            numPolygonVertexCount -= polygonHoleVertices[faceIndex];
         }
      }

      polygonVertexCount += numPolygonVertexCount; // increment the total amount of polygon vertices
      nsides_list[faceIndex] = ((unsigned int)numPolygonVertexCount); // store the amount of vertices for this polygon

      pit.next(); // next polygon
   }

   // store the amount of polyVertex without holes
   unsigned int holesOffset = polygonVertexCount;

   if (hasHoles)
   {
      holeIndex = 0;

      nsides_list.resize(nsides_list.size() + numHoles, 0u);
      arnoldPolygonHoles.reserve(numHoles * 2);

      for (int h = 0; h < numHoles; ++h, holeIndex += 3)
      {
         int numPolygonVertexCount = holeInfoArray[holeIndex + 1];
         nsides_list[np+h] = (unsigned int)numPolygonVertexCount;
         polygonVertexCount += numPolygonVertexCount;
         arnoldPolygonHoles.push_back(np+h);
         arnoldPolygonHoles.push_back(holeInfoArray[holeIndex]);
      }
      polyVtxRemap.resize(polygonVertexCount, 0u);

   }

   nsides = AiArrayConvert(nsides_list.size(), 1, AI_TYPE_UINT, &nsides_list[0]);
   
   std::vector<unsigned int> vidxsVec(polygonVertexCount, 0u);
   
   if (exportUVs)
   {
      uvidxs.resize(numUVSets);
      for (size_t i = 0; i < numUVSets; ++i)
         uvidxs[i] = AiArrayAllocate(polygonVertexCount, 1, AI_TYPE_UINT);
   }

   MIntArray p_vidxs;
   unsigned int arnoldPolyVtxId = 0;
   unsigned int mayaPolyVtxId = 0;
   unsigned int id = 0;
   unsigned int holePolyVtxId = 0;
   unsigned int v, i;

   pit.reset();
   while(!pit.isDone())
   {
      pit.getVertices(p_vidxs);
      faceIndex = pit.index();

      // loop over real poly vertices
      for(v = 0; v < nsides_list[faceIndex]; ++v)
      {
         id = arnoldPolyVtxId++;
         vidxsVec[id] = p_vidxs[v];
         // UVs
         if (exportUVs)
         {
            for (i = 0; i < numUVSets; ++i)
            {
               if ( fnMesh.numUVs(uvNames[i]) > 0 )
               {
                  if (pit.getUVIndex(v, uv_id, &uvNames[i]) != MS::kSuccess)
                  {
                     uv_id = 0;
                     AiMsgWarning("[MtoA] No uv coordinate exists for uv set %s at polygon %i at vertex %i on mesh %s.",
                                 uvNames[i].asChar(), faceIndex, v, fnMesh.name().asChar());
                  }
                  AiArraySetUInt(uvidxs[i], id, uv_id);
               }
            }
         }
         if (hasHoles)
            polyVtxRemap[mayaPolyVtxId + v] = id; // maya-to-arnold polyVtx index remapping
      }

      if (p_vidxs.length() <= nsides_list[faceIndex])      
      {
         // No holes in this face
         pit.next();
         mayaPolyVtxId += p_vidxs.length();
         continue; 
      }

      // This face has holes, we need to loop over each hole now
      holeIndex = 0;
      for (int h = 0; h < numHoles; ++h, holeIndex += 3)
      {
         if (holeInfoArray[holeIndex] != faceIndex)
            continue; // not a hole in this current face

         int holeVtxCount = holeInfoArray[holeIndex + 1]; // how many vertices in this hole
         int startHoleIndex = holeInfoArray[holeIndex + 2]; // starting index for this hole in holeVertexArray

         // loop over hole vertices in inverted order
         for (int hv = holeVtxCount - 1; hv >= 0; --hv)
         {
            int vtxIndex = holeVertexArray[hv + startHoleIndex]; // index of this vertex

            // simple linear search over the "hole-vertices" for this face
            for (uint v = nsides_list[faceIndex]; v < p_vidxs.length(); ++v)
            {
               if (p_vidxs[v] != vtxIndex)
                  continue;

               // found the good vertex
               
               id = holesOffset + holePolyVtxId++; // arnold polyVtx index
               vidxsVec[id] = p_vidxs[v];
               // UVs
               if (exportUVs)
               {
                  for (size_t i = 0; i < numUVSets; ++i)
                  {
                     if ( fnMesh.numUVs(uvNames[i]) > 0 )
                     {  
                        // FIXME : I have a problem here, the index doesn't seem to be correct for holes
                        if (pit.getUVIndex(v, uv_id, &uvNames[i]) != MS::kSuccess)
                        {
                           uv_id = 0;
                           AiMsgWarning("[MtoA] No uv coordinate exists for uv set %s at polygon %i at vertex %i on mesh %s.",
                                       uvNames[i].asChar(), faceIndex, v, fnMesh.name().asChar());
                        }
                        AiArraySetUInt(uvidxs[i], id, uv_id);
                     }
                  }
               }
               polyVtxRemap[mayaPolyVtxId + v] = id;
               break;
            }
         }
      }

      mayaPolyVtxId += p_vidxs.length();
      pit.next();
   }
   // now convert the array
   vidxs =  AiArrayConvert(polygonVertexCount, 1, AI_TYPE_UINT, &vidxsVec[0]);
   
   // Normals.
   if (exportNormals)
   {
      std::vector<unsigned int> nidxsVec(polygonVertexCount, 0u);
      MIntArray vertex_counts, normal_ids;
   
      fnMesh.getNormalIds(vertex_counts, normal_ids);
      if (polyVtxRemap.empty())
         for(uint n(0); n < normal_ids.length(); ++n) nidxsVec[n] = normal_ids[n];
      else
         for(uint n(0); n < normal_ids.length(); ++n) nidxsVec[polyVtxRemap[n]] = normal_ids[n];

      nidxs =  AiArrayConvert(polygonVertexCount, 1, AI_TYPE_UINT, &nidxsVec[0]);
   }

   return true;
}

void CPolygonGeometryTranslator::ExportShaders()
{
   ExportMeshShaders(GetArnoldNode(), m_dagPath);
}

void CPolygonGeometryTranslator::GetDisplacement(MObject& obj, 
                                          float& dispPadding, 
                                          bool& enableAutoBump)
{
   MFnDependencyNode dNode(obj);
   MPlug plug = dNode.findPlug("aiDisplacementPadding", true);
   if (!plug.isNull())
      dispPadding = AiMax(dispPadding, plug.asFloat());
   if (!enableAutoBump)
   {
      plug = dNode.findPlug("aiDisplacementAutoBump", true);
      if (!plug.isNull())
         enableAutoBump = enableAutoBump || plug.asBool();
   }
}
// It eventually returns the MPlug of the assigned surface/volume shader
static MPlug MtoaGetAssignedShaderPlug(const MPlug &shadingGroupPlug, bool isVolume)
{
   if (shadingGroupPlug.isNull())
      return MPlug();

   MStatus status;
   MFnDependencyNode sgNode(shadingGroupPlug.node(), &status);
   if (status != MS::kSuccess)
      return MPlug();

   
   // check if it has custom AOVs
   bool hasCustomAovs = false;
   MPlugArray connections;
   MPlug arrayPlug = sgNode.findPlug("aiCustomAOVs", true);

   for (unsigned int i = 0; i < arrayPlug.numElements (); i++)
   {
      MPlug msgPlug = arrayPlug[i].child(1);
      msgPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         hasCustomAovs = true;
         break;
      }
   }

   if (hasCustomAovs && !isVolume)
      return shadingGroupPlug;

   // shading group doesn't have any custom AOV, 
   // in that case I must find the surface shader plug myself
   std::vector<AtNode*> aovShaders;
   
   MString shaderName = (isVolume) ? "volumeShader" : "surfaceShader";
   MString aiShaderName =  (isVolume) ? "aiVolumeShader" : "aiSurfaceShader";

   connections.clear();
   MPlug shaderPlug = sgNode.findPlug(aiShaderName, true);
   shaderPlug.connectedTo(connections, true, false);
   if (connections.length() == 0)
   {
      shaderPlug = sgNode.findPlug(shaderName, true);
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] CShadingEngineTranslator::Export found surfaceShader plug "+ shaderPlug.name());
      shaderPlug.connectedTo(connections, true, false);
   }
   
   return (connections.length() > 0) ? connections[0] : shadingGroupPlug;
}
void CPolygonGeometryTranslator::ExportMeshShaders(AtNode* polymesh,
                                            const MDagPath &path)
{
   MFnMesh fnMesh(path);

   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;
   std::vector<AtNode*> meshDisps;

   // First check if we need volume shading or not
   // bool isVolume = false;
   // FIXME when compatibility can be broken, we should refactor this,
   // I shouldn't have to do guesses based on the attribute name.
   // What's even worse is that GetNodeShadingGroup is static so I can't get the AtNode
   // => verify if this works with fluids
   MFnDependencyNode fnDGNode(path.node());
   MPlug stepSizePlug = fnDGNode.findPlug("aiStepSize", true);
//   if (!stepSizePlug.isNull())
//      isVolume = (stepSizePlug.asFloat() > AI_EPSILON);


   MPlug shadingGroupPlug = GetNodeShadingGroup(path.node(), instanceNum);

   m_displaced = false;
   
   float maximumDisplacementPadding = -AI_BIG;
   bool enableAutoBump = false;
   MPlug plug;

   // Only one Shading Group applied to Mesh
   if (!shadingGroupPlug.isNull())
   {
      // SURFACE MATERIAL EXPORT
      AtNode *shader = ExportConnectedNode(shadingGroupPlug);
      if (shader != NULL)
      {
         AiNodeSetPtr(polymesh, "shader", shader);
      }
      else
      {
         AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
               GetTranslatorName().asChar(), MFnDependencyNode(shadingGroupPlug.node()).name().asChar());
         AiNodeSetPtr(polymesh, "shader", NULL);
      }

      // DISPLACEMENT MATERIAL EXPORT
      MPlugArray        connections;
      MFnDependencyNode fnDGShadingGroup(shadingGroupPlug.node());
      MPlug shaderPlug = fnDGShadingGroup.findPlug("displacementShader", true);
      shaderPlug.connectedTo(connections, true, false);

      // are there any connections to displacementShader?
      if (connections.length() > 0)
      {
         m_displaced = true;
         MObject dispNode = connections[0].node();
         GetDisplacement(dispNode, maximumDisplacementPadding, enableAutoBump);
         
            AtNode* dispImage(ExportConnectedNode(connections[0]));
         AiNodeSetPtr(polymesh, "disp_map", dispImage);         
      }
   }

   // PER-FACE Shading Group applied to Mesh
   else
   {
      MIntArray indices;
      // Per-face assignment
      MObjectArray shadingGroups;
      // Indices are used later when exporting shidxs
      fnMesh.getConnectedShaders(instanceNum, shadingGroups, indices);

      for (int J = 0; (J < (int) shadingGroups.length()); J++)
      {
         // SURFACE MATERIAL EXPORT
         // We have an array of Shading Groups in shadingGroups, but we need the MPlugs to them
         // MPlugs to Shader Groups must be exported in the same order they appear in "shadingGroups"
         MFnDependencyNode fnDGNode(m_dagPath.node());
         MPlug plug(m_dagPath.node(), fnDGNode.attribute("instObjGroups"));
         plug = plug.elementByLogicalIndex(instanceNum);
         MObject obGr = MFnDependencyNode(GetMayaObject()).attribute("objectGroups");
         plug = plug.child(obGr);
         
         bool exported = false;
         // Loop over all MPlugs to Shader Nodes
         int plugElements = plug.evaluateNumElements();
         for (int i = 0; i < plugElements && !exported ; i++)
         {
            MPlugArray connections;
            plug.elementByPhysicalIndex(i).connectedTo(connections, false, true);
            for(unsigned int j=0; j < connections.length() && !exported ; j++)
            {
               // Only export if MPlug matches the connected Shader Group
               if (shadingGroups[J] == connections[j].node())
               {
                  // connections[j] is the MPlug to shadingGroups[J]
         
                  if (!connections[j].isNull())
                  {
                     AtNode *shader = ExportConnectedNode(connections[j]);
                     if (shader != NULL)
                     {
                        meshShaders.push_back(shader);
                        exported = true;
                     }
                  }
               }
            }
         }
         // If not exported, it means that the Shading Group MPlug has not been found
         if (!exported)
         {
            AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s MPlug not found",
                  GetTranslatorName().asChar(), MFnDependencyNode(shadingGroups[J]).name().asChar());
            meshShaders.push_back(NULL);
         }

         // DISPLACEMENT MATERIAL EXPORT

         MPlugArray        connections;
         MFnDependencyNode fnDGShadingGroup(shadingGroups[J]);
         MPlug shaderPlug = fnDGShadingGroup.findPlug("displacementShader", true);
         shaderPlug.connectedTo(connections, true, false);

         // are there any connections to displacementShader?
         // If no connection found, add a NULL to meshDisps to match
         //  meshShaders distribution
         if (connections.length() > 0)
         {            
            m_displaced = true;
            MObject dispNode = connections[0].node();
            GetDisplacement(dispNode, maximumDisplacementPadding, enableAutoBump);
            AtNode* dispImage(ExportConnectedNode(connections[0]));
            meshDisps.push_back(dispImage);
         }
         else
         {
            meshDisps.push_back(NULL);
         }
      }
      
      const size_t numMeshShaders = meshShaders.size();
      if (numMeshShaders > 0)
         AiNodeSetArray(polymesh, "shader", AiArrayConvert(numMeshShaders, 1, AI_TYPE_NODE, &meshShaders[0]));
      else
         AiNodeSetPtr(polymesh, "shader", NULL);

      const size_t numMeshDisps = meshDisps.size();
      for (size_t i = 0; i < numMeshDisps; ++i)
      {
         if (meshDisps[i] != 0)
         {
            AiNodeSetArray(polymesh, "disp_map", AiArrayConvert(numMeshDisps, 1, AI_TYPE_NODE, &meshDisps[0]));
            break;
         }
      }

      // Export face to shader indices
      // First convert from MIntArray to unsigned int vector
      
      int divisions = 0;
      int multiplier = 0;
      
      if (fnMesh.findPlug("displaySmoothMesh", true).asBool())
      {
         MMeshSmoothOptions options;
         MStatus status = fnMesh.getSmoothMeshDisplayOptions(options);
         
         CHECK_MSTATUS(status);

         if(!fnMesh.findPlug("useSmoothPreviewForRender", false, &status).asBool())
            options.setDivisions(fnMesh.findPlug("renderSmoothLevel", false, &status).asInt());
         
         divisions = options.divisions();
         if(divisions > 0)
            multiplier = static_cast<int> (pow(4.0f, (divisions-1)));
      }

      std::vector<unsigned char> shidxs;
      for (unsigned int i = 0; i < indices.length(); i++)
      {
         const int subdivisions = multiplier * fnMesh.polygonVertexCount(i);
         // indices[i] == -1 when a Shading Group is not connected
         const unsigned char indexToBePushed = (indices[i] == -1) ? 0 : (unsigned char)AiMin(indices[i], 255);
         shidxs.push_back(indexToBePushed);
         for (int j = 0; j < subdivisions -1; j++)
            shidxs.push_back(indexToBePushed);
      }

      // Need to add per-face data for the holes as well.  Since the holes are not visible
      // it does not matter what shader index we use so we just fill it in with 0's here.
      // Note the geometry processing adds the holes at the end so we also add the per face data 
      // for the holes at the end too.
      AtArray *holesArray = AiNodeGetArray(polymesh, "polygon_holes");
      if (holesArray != NULL)
      {
         // we already exported some holes here
         int numHoles = AiArrayGetNumElements(holesArray) / 2;
         for (int i = 0; i < numHoles; i++)
         {
            shidxs.push_back(0);
         }
      }

      int numFaceShaders = (int)shidxs.size();
      if (numFaceShaders > 0)
      {
         AiNodeSetArray(polymesh, "shidxs", AiArrayConvert(numFaceShaders, 1, AI_TYPE_BYTE, &(shidxs[0])));
      }
   }

   // Only export displacement attributes if a displacement is applied
   if (m_displaced)
   {
      // Note that disp_height has no actual influence on the scale of the displacement if it is vector based
      // it only influences the computation of the displacement bounds
      AiNodeSetFlt(polymesh, "disp_height",  FindMayaPlug("aiDispHeight").asFloat());
      AiNodeSetFlt(polymesh, "disp_padding", AiMax(maximumDisplacementPadding, FindMayaPlug("aiDispPadding").asFloat()));
      AiNodeSetFlt(polymesh, "disp_zero_value", FindMayaPlug("aiDispZeroValue").asFloat());
      AiNodeSetBool(polymesh, "disp_autobump", FindMayaPlug("aiDispAutobump").asBool() || enableAutoBump);
      AiNodeSetByte(polymesh, "autobump_visibility", FindMayaPlug("aiAutobumpVisibility").asInt());
   }
}

void CPolygonGeometryTranslator::ExportMeshGeoData(AtNode* polymesh)
{
   unsigned int step = GetMotionStep();

   MFnMesh fnMesh(m_geometry);
   MObject geometry(m_geometry);
   
   //
   // GEOMETRY
   //   
   unsigned int numVerts = fnMesh.numVertices();
   unsigned int numNorms = fnMesh.numNormals();
   unsigned int numFaceVertices = fnMesh.numFaceVertices();

   const float* vertices = 0;
   // Get all vertices
   bool exportVertices = GetVertices(geometry, vertices);

   const float* normals = 0;
   // Get all normals
   bool exportNormals = GetNormals(geometry, normals);  

   if (!IsExportingMotion())
   {
      std::vector<AtArray*> uvs;
      std::vector<MString> uvNames;
      std::vector<AtArray*> uvidxs;
      AtArray* nsides = 0;
      AtArray* vidxs = 0; AtArray* nidxs = 0;
      unordered_map<std::string, std::vector<float> > vcolors;
      AtArray* refNormals = 0; AtArray* rnidxs = 0; AtArray* refTangents = 0; AtArray* refBitangents = 0;
      AtArray* reftangentidxs = 0; AtArray* refBitangentidxs = 0;
      const float* refVertices = 0;

      std::vector<unsigned int> polyVtxRemap;
      std::vector<unsigned int> arnoldPolygonHoles;

      // Get UVs
      bool exportUVs = GetUVs(geometry, uvs, uvNames);

      // Get Component IDs
      bool exportCompIDs = GetComponentIDs(geometry, nsides, vidxs, nidxs, uvidxs, uvNames, exportNormals, 
         exportUVs, polyVtxRemap, arnoldPolygonHoles);
      // if GetComponentIDs returned false, it means that no polygons were found in the mesh. 
      // In that case uvidxs is empty, so we must not try to export the UVs
      if (!exportCompIDs) exportUVs = false;

      // Get reference objects
      bool exportReferenceObjects = GetRefObj(refVertices, refNormals, rnidxs,
                                              refTangents, refBitangents, reftangentidxs, refBitangentidxs, polyVtxRemap);
      bool exportRefVerts = refVertices != 0;
      bool exportRefNorms = refNormals != 0;
      bool exportRefTangents = refTangents != 0;

      // Get Vertex Colors
      MPlug plug = FindMayaPlug("aiMotionVectorSource");
      if (!plug.isNull())
         m_motionVectorSource = plug.asString();
      bool exportColors = GetVertexColors(geometry, vcolors, polyVtxRemap);

      // Get all tangents, bitangents
      AtArray* tangents; AtArray* bitangents;
      AtArray* tangentidxs; AtArray* bitangentidxs;
      if (GetTangents(geometry, tangents, bitangents, tangentidxs, bitangentidxs, MSpace::kObject)) // Arnold doesn't support motion blurred user data
      {
         if (tangentidxs)
         {
            // Per-face-vertex "indexed" user data
            if (AiNodeLookUpUserParameter(polymesh, "tangent") == NULL)
               AiNodeDeclare(polymesh, "tangent", "indexed VECTOR");
            if (AiNodeLookUpUserParameter(polymesh, "bitangent") == NULL)
               AiNodeDeclare(polymesh, "bitangent", "indexed VECTOR");
            AiNodeSetArray(polymesh, "tangent", tangents);
            AiNodeSetArray(polymesh, "bitangent", bitangents);
            
            if (AiNodeLookUpUserParameter(polymesh, "tangentidxs") == NULL)
               AiNodeDeclare(polymesh, "tangentidxs", "indexed UINT");
            if (AiNodeLookUpUserParameter(polymesh, "bitangentidxs") == NULL)
               AiNodeDeclare(polymesh, "bitangentidxs", "indexed UINT");
            AiNodeSetArray(polymesh, "tangentidxs", tangentidxs);
            AiNodeSetArray(polymesh, "bitangentidxs", bitangentidxs);
         } else
         {
            // per-vertex user data
            if (AiNodeLookUpUserParameter(polymesh, "tangent") == NULL)
               AiNodeDeclare(polymesh, "tangent", "varying VECTOR");
            if (AiNodeLookUpUserParameter(polymesh, "bitangent") == NULL)
               AiNodeDeclare(polymesh, "bitangent", "varying VECTOR");
            AiNodeSetArray(polymesh, "tangent", tangents);
            AiNodeSetArray(polymesh, "bitangent", bitangents);
         }
      }

      if (exportReferenceObjects)
      {
         if (exportRefVerts)
            AiNodeDeclare(polymesh, "Pref", "varying VECTOR");
         if (exportRefNorms)
         {
            AiNodeDeclare(polymesh, "Nref", "indexed VECTOR");
            AiNodeDeclare(polymesh, "Nrefidxs", "indexed UINT");
         }
         if (exportRefTangents)
         {                        
            if (reftangentidxs)
            {               
               AiNodeDeclare(polymesh, "Tref", "indexed VECTOR");
               AiNodeDeclare(polymesh, "BTref", "indexed VECTOR");
               AiNodeDeclare(polymesh, "Trefidxs", "indexed UINT");
               AiNodeDeclare(polymesh, "BTrefidxs", "indexed UINT");
            } else
            {
               AiNodeDeclare(polymesh, "Tref", "varying VECTOR");
               AiNodeDeclare(polymesh, "BTref", "varying VECTOR");
            }
         }
      }

      // Declare user parameters for color sets
      if (exportColors)
      {

         unordered_map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            if (it->second.size() == numVerts * 4)
               AiNodeDeclare(polymesh, it->first.c_str(), "varying RGBA");
            else
            {
               AiNodeDeclare(polymesh, it->first.c_str(), "indexed RGBA");
               std::string userDataIdxName = it->first + "idxs";
               AiNodeDeclare(polymesh, userDataIdxName.c_str(), "indexed UINT");
            }

            ++it;
         }
      }

      if (!m_motionDeform || !IsLocalMotionBlurEnabled())
      {
         // No deformation motion blur, so we create normal arrays
         if (exportVertices)
            AiNodeSetArray(polymesh, "vlist", AiArrayConvert(numVerts * 3, 1, AI_TYPE_FLOAT, &(vertices[0])));
         if (exportNormals)
            AiNodeSetArray(polymesh, "nlist", AiArrayConvert(numNorms * 3, 1, AI_TYPE_FLOAT, &(normals[0])));
      }
      else
      {
         // Deformation motion blur. We need to create keyable arrays for vlist and nlist
         if (m_useMotionVectors)
         {
            if (exportVertices)
            {
               const float motionVectorScale = FindMayaPlug("aiMotionVectorScale").asFloat();
               // 0 - unit / frame
               // 1 - unit / second
               const short motionVectorUnit = FindMayaPlug("aiMotionVectorUnit").asShort();
               std::vector<float>& motionVectors = vcolors[m_motionVectorSource.asChar()];
               const AtRGBA* motionVectorColors = (AtRGBA*)&motionVectors[0];
               AtArray* verticesArray = AiArrayAllocate(numVerts, 2, AI_TYPE_VECTOR);
               const float* vert = vertices;
               float motionRange = float(GetMotionByFrame()) * motionVectorScale;
               if (motionVectorUnit == 1)
               {
                  MTime oneSec(1.0, MTime::kSeconds);
                  const float fps =  (float)oneSec.asUnits(MTime::uiUnit());
                  motionRange /= fps;
               }
               for (unsigned int i = 0; i < numVerts; ++i)
               {                  
                  AtVector vec;
                  vec.x = *(vert++);
                  vec.y = *(vert++);
                  vec.z = *(vert++);
                  AiArraySetVec(verticesArray, i, vec);
                  const AtRGBA* motionVector = motionVectorColors + i;
                  vec.x += motionVector->r * motionRange;
                  vec.y += motionVector->g * motionRange;
                  vec.z += motionVector->b * motionRange;
                  AiArraySetVec(verticesArray, i + numVerts, vec);
               }
               AiNodeSetArray(polymesh, "vlist", verticesArray);
            }
            if (exportNormals)
            {
               AtArray* normalsArray = AiArrayAllocate(numNorms, 2, AI_TYPE_VECTOR);
               const float* norm = normals;
               for (unsigned int i = 0; i < numNorms; ++i)
               {                  
                  AtVector vec;
                  vec.x = *(norm++);
                  vec.y = *(norm++);
                  vec.z = *(norm++);
                  AiArraySetVec(normalsArray, i, vec);
                  AiArraySetVec(normalsArray, i + numNorms, vec);
               }
               AiNodeSetArray(polymesh, "nlist", normalsArray);
            }
         }
         else
         {
            if (exportVertices)
            {
               AtArray* vlist_array = AiArrayAllocate(numVerts, GetNumMotionSteps(), AI_TYPE_VECTOR);
               SetKeyData(vlist_array, step, vertices);
               AiNodeSetArray(polymesh, "vlist", vlist_array);
            }
            if (exportNormals)
            {
               AtArray* nlist_array = AiArrayAllocate(numNorms, GetNumMotionSteps(), AI_TYPE_VECTOR);
               SetKeyData(nlist_array, step, normals);
               AiNodeSetArray(polymesh, "nlist", nlist_array);
            }
         }
      }

      if (exportCompIDs)
      {
         AiNodeSetArray(polymesh, "nsides", nsides);
         AiNodeSetArray(polymesh, "vidxs", vidxs);
         if (exportNormals)
            AiNodeSetArray(polymesh, "nidxs", nidxs);

         if (!arnoldPolygonHoles.empty())
         {
            numFaceVertices = AiArrayGetNumElements(vidxs); // the amount of face vertices gets bigger with the holes
            AtArray *polygonHoles = AiArrayConvert(arnoldPolygonHoles.size(), 1, AI_TYPE_UINT, &arnoldPolygonHoles[0]);
            AiNodeSetArray(polymesh, "polygon_holes", polygonHoles);

            // make sure shidx has the proper amount of elements
            AtArray *shidxArray = AiNodeGetArray(polymesh, "shidxs");
            
            if (shidxArray != NULL)
            {
               unsigned int shidxCount = AiArrayGetNumElements(shidxArray);
               unsigned int polyCount = AiArrayGetNumElements(nsides);

               if (shidxCount > 0 && shidxCount < polyCount)
               {
                  // the "shidx" needs some extra-data here, to fill the holes...
                  unsigned char *shidxList = (unsigned char*)AiArrayMap(shidxArray);
                  std::vector<unsigned char> newShidxList (polyCount, 0);
                  memcpy(&newShidxList[0], shidxList, shidxCount * sizeof(unsigned char) );
                  AiArrayUnmap(shidxArray); // is it necessary ?

                  AtArray* newShidxArray = AiArrayConvert(polyCount, 1, AI_TYPE_BYTE, &newShidxList[0]);
                  AiNodeSetArray(polymesh, "shidxs", newShidxArray);

               }
            }
         
         }

      }
      
      if (exportReferenceObjects) // TODO : use local space for this and manually transform that later, 
         // that makes a few functions much simpler
      {         
         if (exportRefVerts)
         {
            AtMatrix worldMatrix;
            ConvertMatrix(worldMatrix, m_dagPathRef.inclusiveMatrix());
            AtArray* aRefVertices = AiArrayAllocate(numVerts, 1, AI_TYPE_VECTOR);
            const AtVector* vRefVertices = (const AtVector*)refVertices;
            for (unsigned int i = 0; i < numVerts; ++i)
            {
               AtVector v = AiM4PointByMatrixMult(worldMatrix, *(vRefVertices + i));
               AiArraySetVec(aRefVertices, i, v);
            }
            AiNodeSetArray(polymesh, "Pref", aRefVertices);
         }
         if (exportRefNorms)
         {
            AiNodeSetArray(polymesh, "Nref", refNormals);
            AiNodeSetArray(polymesh, "Nrefidxs", rnidxs);
         }
         if (exportRefTangents)
         {
            AiNodeSetArray(polymesh, "Tref", refTangents);
            AiNodeSetArray(polymesh, "BTref", refBitangents);
            if (reftangentidxs)
            {
               AiNodeSetArray(polymesh, "Trefidxs", reftangentidxs);
               AiNodeSetArray(polymesh, "BTrefidxs", refBitangentidxs);
            }
         }
      }
      if (exportUVs)
      {
         if (uvs.size() > 0 && uvidxs.size() > 0)
         {
            if ( AiArrayGetNumElements(uvs[0]) > 0 )
            {
               AiNodeSetArray(polymesh, "uvlist", uvs[0]);
               AiNodeSetArray(polymesh, "uvidxs", uvidxs[0]);
            }
            for (size_t i = 1; i < uvs.size(); ++i)
            {
               if (uvNames.size() > i && uvidxs.size() > i)
               {
                  MString idxsName = uvNames[i] + MString("idxs");
                  AiNodeDeclare(polymesh, uvNames[i].asChar(), "indexed VECTOR2");
                  AiNodeSetArray(polymesh, uvNames[i].asChar(), uvs[i]);
                  AiNodeSetArray(polymesh, idxsName.asChar(), uvidxs[i]);
               }
            }
         }
      }
      if (exportColors)
      {
         unordered_map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            if (it->second.size() == numVerts * 4)
               AiNodeSetArray(polymesh, it->first.c_str(), AiArrayConvert(numVerts, 1, AI_TYPE_RGBA, &(it->second[0])));
            else
            {
               AiNodeSetArray(polymesh, it->first.c_str(), AiArrayConvert(numFaceVertices, 1, AI_TYPE_RGBA, &(it->second[0])));
               std::vector<unsigned int> userDataIdx(numFaceVertices);
               for (size_t i = 0; i < numFaceVertices; ++i)
                  userDataIdx[i] = i;

               std::string userDataIdxName = it->first + "idxs";
               AiNodeSetArray(polymesh, userDataIdxName.c_str(), AiArrayConvert(numFaceVertices, 1, AI_TYPE_UINT, &(userDataIdx[0])));
            }
            ++it;
         }
      }

      // exporting crease edges
      // for the first version we always export them
      // since the user might override the subdiv options
      // from a procedural, node processor etc...
      if (!fnMesh.findPlug("displaySmoothMesh", true).asBool())
      {
         MUintArray creaseEdgeIds;
         MDoubleArray creaseEdgeDatas;

         MUintArray creaseVertexIds;
         MDoubleArray creaseVertexDatas;

         unsigned int creaseEdgeIdCount = 0;
         unsigned int creaseVertexIdCount = 0;
         
         if ((fnMesh.getCreaseEdges(creaseEdgeIds, creaseEdgeDatas) == MS::kSuccess) && (creaseEdgeIds.length() > 0))
            creaseEdgeIdCount = creaseEdgeIds.length();

         if ((fnMesh.getCreaseVertices(creaseVertexIds, creaseVertexDatas) == MS::kSuccess) && (creaseVertexIds.length() > 0))
            creaseVertexIdCount = creaseVertexIds.length();

         const unsigned int creaseIdCount = creaseEdgeIdCount + creaseVertexIdCount;

         if (creaseIdCount > 0)
         {
            AtArray* aCreaseEdges = AiArrayAllocate(creaseIdCount * 2, 1, AI_TYPE_UINT);
            AtArray* aCreaseData = AiArrayAllocate(creaseIdCount, 1, AI_TYPE_FLOAT);

            if (creaseEdgeIdCount > 0)
            {
               MItMeshEdge edgeIt(m_geometry); // we need this to access the 
               // connected vertices information
               int prevId; // junk
               for (unsigned int i = 0; i < creaseEdgeIdCount; ++i)
               {
                  const unsigned int edgeId = creaseEdgeIds[i];
                  edgeIt.setIndex(static_cast<int>(edgeId), prevId);
                  AiArraySetUInt(aCreaseEdges, i * 2, static_cast<unsigned int>(edgeIt.index(0)));
                  AiArraySetUInt(aCreaseEdges, i * 2 + 1, static_cast<unsigned int>(edgeIt.index(1)));
                  AiArraySetFlt(aCreaseData, i, static_cast<float>(creaseEdgeDatas[i]));
               }              
            }

            if (creaseVertexIdCount > 0)
            {
               const unsigned int baseId = creaseEdgeIdCount * 2;
               for (unsigned int i = 0; i < creaseVertexIdCount; ++i)
               {
                  AiArraySetUInt(aCreaseEdges, baseId + i * 2, creaseVertexIds[i]);
                  AiArraySetUInt(aCreaseEdges, baseId + i * 2 + 1, creaseVertexIds[i]);
                  AiArraySetFlt(aCreaseData, creaseEdgeIdCount + i, static_cast<float>(creaseVertexDatas[i]));
               }
            }

            AiNodeSetArray(polymesh, "crease_idxs", aCreaseEdges);
            AiNodeSetArray(polymesh, "crease_sharpness", aCreaseData);
         }         
      }
   }
   else if (!m_useMotionVectors)
   {
      // Export motion blur keys information (for deformation)

      // Vertices
      if (exportVertices)
      {
         AtArray* vlist_array = AiNodeGetArray(polymesh, "vlist");
         if (vlist_array == NULL)
            return;
         if (AiArrayGetNumElements(vlist_array) != numVerts)
            AiMsgError("[mtoa.translator]  %-30s | Number of vertices changed between motion steps: %d -> %d",
                       GetMayaNodeName().asChar(), AiArrayGetNumElements(vlist_array), numVerts);
         else
            SetKeyData(vlist_array, step, vertices);

      }
      // Normals
      if (exportNormals)
      {
         AtArray* nlist_array = AiNodeGetArray(polymesh, "nlist");
         if (AiArrayGetNumElements(nlist_array) != numNorms)
            AiMsgError("[mtoa.translator]  %-30s | Number of normals changed between motion steps: %d -> %d",
                       GetMayaNodeName().asChar(), AiArrayGetNumElements(nlist_array), numNorms);
         else
            SetKeyData(nlist_array, step, normals);
      }
   }
}

// Specific implementation for each geometry type
bool CPolygonGeometryTranslator::IsGeoDeforming()
{
   return true;
}

void CPolygonGeometryTranslator::ExportMeshParameters(AtNode* polymesh)
{
   // Visibility options
   ProcessRenderFlags(polymesh);

   // Check if custom attributes have been created, ignore them otherwise
   if (FindMayaPlug("aiSubdivType").isNull()) return;

   AiNodeSetBool(polymesh, "smoothing", FindMayaPlug("smoothShading").asBool());

   if (FindMayaPlug("doubleSided").asBool())
      AiNodeSetByte(polymesh, "sidedness", AI_RAY_ALL);
   else
   {
      AiNodeSetBool(polymesh, "invert_normals", FindMayaPlug("opposite").asBool());
      AiNodeSetByte(polymesh, "sidedness", 0);
   }

   AiNodeSetFlt(polymesh, "step_size", FindMayaPlug("aiStepSize").asFloat());
   AiNodeSetFlt(polymesh, "volume_padding", FindMayaPlug("aiVolumePadding").asFloat());
   
   // Subdivision surfaces
   //
   const int subdivision = FindMayaPlug("aiSubdivType").asInt();
   if (subdivision!=0)
   {
      if (subdivision==1)
         AiNodeSetStr(polymesh, "subdiv_type",           "catclark");
      else
         AiNodeSetStr(polymesh, "subdiv_type",           "linear");
      AiNodeSetByte(polymesh, "subdiv_iterations",     FindMayaPlug("aiSubdivIterations").asInt());
      AiNodeSetInt(polymesh, "subdiv_adaptive_metric",FindMayaPlug("aiSubdivAdaptiveMetric").asInt());
      AiNodeSetFlt(polymesh, "subdiv_adaptive_error",    FindMayaPlug("aiSubdivPixelError").asFloat());
      AiNodeSetInt(polymesh, "subdiv_adaptive_space",    FindMayaPlug("aiSubdivAdaptiveSpace").asInt());
      AiNodeSetInt(polymesh, "subdiv_uv_smoothing",   FindMayaPlug("aiSubdivUvSmoothing").asInt());
      AiNodeSetBool(polymesh, "subdiv_smooth_derivs", FindMayaPlug("aiSubdivSmoothDerivs").asBool());
      AiNodeSetBool(polymesh, "subdiv_frustum_ignore", FindMayaPlug("aiSubdivFrustumIgnore").asBool());
   }
}

// Note that this function is only called by CMeshTranslator
void CPolygonGeometryTranslator::ExportBBox(AtNode* polymesh)
{
   ExportMatrix(polymesh);
   // Visibility options
   ProcessRenderFlags(polymesh);

   if (FindMayaPlug("doubleSided").asBool())
      AiNodeSetByte(polymesh, "sidedness", AI_RAY_ALL);
   else
   {
      AiNodeSetBool(polymesh, "invert_normals", FindMayaPlug("opposite").asBool());
      AiNodeSetByte(polymesh, "sidedness", 0);
   }

   if (RequiresShaderExport())
      ExportMeshShaders(polymesh, m_dagPath);
   ExportLightLinking(polymesh);

   MFnMesh fnMesh(m_geometry);
   MBoundingBox bbox = fnMesh.boundingBox();
   AiNodeSetVec(polymesh, "min", (float)bbox.min().x, (float)bbox.min().y, (float)bbox.min().z);
   AiNodeSetVec(polymesh, "max", (float)bbox.max().x, (float)bbox.max().y, (float)bbox.max().z);
   //AiNodeSetFlt(polymesh, "step_size", FindMayaPlug("aiStepSize").asFloat());
}

AtNode* CPolygonGeometryTranslator::ExportMesh(AtNode* polymesh, bool update)
{   
   ExportMatrix(polymesh);   
   ExportMeshParameters(polymesh);
   if (RequiresShaderExport())
      ExportMeshShaders(polymesh, m_dagPath);
   ExportLightLinking(polymesh);
   // if enabled, double check motion deform
   m_motionDeform = m_motionDeform && IsGeoDeforming();
   if (!update)
   {
      ExportMeshGeoData(polymesh);
   }

   return polymesh;
}

AtNode* CPolygonGeometryTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   MFnDependencyNode masterDepNode(masterInstance.node());
   MPlug dummyPlug = masterDepNode.findPlug("matrix", true);
   // in case master instance wasn't exported (#648)
   // and also to create the reference between both translators
   AtNode *masterNode = (dummyPlug.isNull()) ? NULL : ExportConnectedNode(dummyPlug);

   int instanceNum = m_dagPath.instanceNumber();
   int masterInstanceNum = masterInstance.instanceNumber();

   ExportMatrix(instance);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);
   
   AtByte visibility = ComputeVisibility();
   AiNodeSetByte(instance, "visibility", visibility);

   if (RequiresShaderExport())
   {
      //
      // SHADERS
      //
      // MFnMesh           meshNode(m_dagPath.node());
      MFnMesh meshNode(m_geometry);
      MPlug plug = meshNode.findPlug("instObjGroups", true);

      MPlugArray conns0, connsI;

      bool shadersDifferent = false;

      // checking the connections from the master instance
      plug.elementByLogicalIndex(masterInstanceNum).connectedTo(conns0, false, true);
      // checking the connections from the actual instance
      plug.elementByLogicalIndex(instanceNum).connectedTo(connsI, false, true);

      // checking if it`s connected to a different shading network
      // this should be enough, because arnold does not supports
      // overriding per face assignment per instance
      // it`s safe to ignore if the instanced object is
      // using a different per face assignment
      // If the original object has per face assignment
      // then the length is zero (because the shading group is
      // connected to a different place)
      const unsigned int conns0Length = conns0.length();
      const unsigned int connsILength = connsI.length();
      if (conns0Length != connsILength)
         shadersDifferent = true;
      else
      {
         if (conns0Length  > 0)
         {
            if (conns0[0].node() != connsI[0].node())
               shadersDifferent = true;
         }
      }

      if (shadersDifferent)
      {
         MPlug stepSizePlug = meshNode.findPlug("aiStepSize", true);
         bool isVolume = (stepSizePlug.isNull()) ? false : (stepSizePlug.asFloat() > AI_EPSILON); 
         MPlug shadingGroupPlug = GetNodeShadingGroup(m_geometry, instanceNum);
         MPlug shaderPlug = MtoaGetAssignedShaderPlug(shadingGroupPlug, isVolume);

         // In case Instance has per face assignment, use first SG assigned to it
         if(shaderPlug.isNull())
         {
            MPlugArray        connections;
            MFnDependencyNode fnDGNode(m_geometry);
            MPlug plug(m_geometry, fnDGNode.attribute("instObjGroups"));
            plug = plug.elementByLogicalIndex(instanceNum);
            MObject obGr = MFnDependencyNode(GetMayaObject()).attribute("objectGroups");
            plug = plug.child(obGr);
            plug.elementByPhysicalIndex(0).connectedTo(connections, false, true);
            if(connections.length() > 0)
               shaderPlug = MtoaGetAssignedShaderPlug(connections[0], isVolume);
            
         }

         AtNode* shader = ExportConnectedNode(shaderPlug);
         AiNodeSetPtr(instance, "shader", shader);
      }
   }
   // Export light linking per instance
   ExportLightLinking(instance);

   return instance;
}

void CPolygonGeometryTranslator::Export(AtNode *anode)
{
   if (!IsExported())
   {
      const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
      if (strcmp(nodeType, "ginstance") == 0)
      {
         ExportInstance(anode, GetMasterInstance());
      }
      else if (strcmp(nodeType, "polymesh") == 0)
      {
         // Early return if we can't tessalate.
         if (!Tessellate(m_dagPath))
            return;
         ExportMesh(anode, false);
      }
      
   } else
   {
      // This is what we used to do, we should check if it's the good thing
      if (IsMasterInstance())
      {
         ExportMesh(anode, true);
      }
      else
      {
         ExportInstance(anode, GetMasterInstance());
      }
   }
}


void CPolygonGeometryTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "polymesh");

   // Node attributes
   CShapeTranslator::MakeCommonAttributes(helper);

   helper.MakeInput("subdiv_type");
   helper.MakeInput("subdiv_iterations");
   helper.MakeInput("subdiv_adaptive_metric");
   helper.MakeInput("subdiv_adaptive_error");
   helper.MakeInput("subdiv_adaptive_space");
   helper.MakeInput("subdiv_uv_smoothing");
   helper.MakeInput("subdiv_smooth_derivs");
   helper.MakeInput("subdiv_frustum_ignore");

   helper.MakeInput("disp_height");
   helper.MakeInput("disp_padding");
   helper.MakeInput("disp_zero_value");
   helper.MakeInput("disp_autobump");
   helper.MakeInput("autobump_visibility");

   CAttrData data;

   data.defaultValue.BOOL() = false;
   data.name = "aiExportTangents";
   data.shortName = "ai_exptan";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiExportColors";
   data.shortName = "ai_expcol";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = true;
   data.name = "aiExportRefPoints";
   data.shortName = "ai_exprpt";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiExportRefNormals";
   data.shortName = "ai_exprnrm";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiExportRefTangents";
   data.shortName = "ai_exprtan";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);
      
   data.defaultValue.FLT() = 0.f;
   data.name = "aiStepSize";
   data.shortName = "ai_step_size";
   data.channelBox = false;
   data.hasMin = true;
   data.min.FLT() = 0.f;
   data.hasSoftMax = true;
   data.softMax.FLT() = 1.f;
   helper.MakeInputFloat(data);

   data.defaultValue.FLT() = 0.f;
   data.name = "aiVolumePadding";
   data.shortName = "ai_volume_padding";
   data.channelBox = false;
   data.hasMin = true;
   data.min.FLT() = 0.f;
   helper.MakeInputFloat(data);

   data.stringDefault = "velocityPV";
   data.name = "aiMotionVectorSource";
   data.shortName = "ai_motion_vector_source";
   data.channelBox = false;
   helper.MakeInputString(data);

   data.defaultValue.INT() = 0;
   data.name = "aiMotionVectorUnit";
   data.shortName = "ai_motion_vector_unit";
   data.channelBox = false;
   data.enums = MStringArray();
   data.enums.append("Per Frame");
   data.enums.append("Per Second");
   helper.MakeInputEnum(data);

   data.defaultValue.FLT() = 1.f;
   data.name = "aiMotionVectorScale";
   data.shortName = "ai_motion_vector_scale";
   data.hasMin = false;
   data.hasMax = false;
   data.hasSoftMin = true;
   data.hasSoftMax = true;
   data.softMin.FLT() = 0.f;
   data.softMax.FLT() = 2.f;
   data.channelBox = false;
   helper.MakeInputFloat(data);
}
