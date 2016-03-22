
#include "GeometryTranslator.h"

#include <maya/MNodeMessage.h>
#include <maya/MBoundingBox.h>
#include <maya/MUintArray.h>
#include <maya/MItMeshEdge.h>

#include <algorithm>

#include "utils/time.h"
#include "scene/MayaScene.h"

namespace
{
   void SetKeyData(AtArray* arr, unsigned int step, const std::vector<float>& data, unsigned int size)
   {
      unsigned int index = 0;

      switch(arr->type)
      {
         case AI_TYPE_POINT:
         {
            AtPoint pnt;
            for(unsigned int J = 0; (J < size); ++J)
            {
               AiV3Create(pnt, data[index+0], data[index+1], data[index+2]);
               index += 3;
               AiArraySetPnt(arr, J + (size * step), pnt);
            }
         }
         break;

         case AI_TYPE_VECTOR:
         {
            AtVector vec;
            for(unsigned int J = 0; (J < size); ++J)
            {
               AiV3Create(vec, data[index+0], data[index+1], data[index+2]);
               index += 3;
               AiArraySetVec(arr, J + (size * step), vec);
            }
         }
      }
   }
   
   void SetKeyData(AtArray* arr, unsigned int step, const float* data, unsigned int size)
   {
      unsigned int index = 0;

      switch(arr->type)
      {
         case AI_TYPE_POINT:
         {
            AtPoint pnt;
            for(unsigned int J = 0; (J < size); ++J)
            {
               AiV3Create(pnt, data[index+0], data[index+1], data[index+2]);
               index += 3;
               AiArraySetPnt(arr, J + (size * step), pnt);
            }
         }
         break;

         case AI_TYPE_VECTOR:
         {
            AtVector vec;
            for(unsigned int J = 0; (J < size); ++J)
            {
               AiV3Create(vec, data[index+0], data[index+1], data[index+2]);
               index += 3;
               AiArraySetVec(arr, J + (size * step), vec);
            }
         }
      }
   }
}

MObject CGeometryTranslator::GetNodeShader(MObject dagNode, int instanceNum)
{
   MPlugArray        connections;
   MObject shadingGroup = CShapeTranslator::GetNodeShadingGroup(dagNode, instanceNum);
   MFnDependencyNode fnDGNode(shadingGroup);
   MPlug shaderPlug = fnDGNode.findPlug("surfaceShader");
   shaderPlug.connectedTo(connections, true, false);

   return connections[0].node();
}

bool CGeometryTranslator::GetVertices(const MObject& geometry,
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

bool CGeometryTranslator::GetPerVertexNormals(const MObject &geometry,
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

bool CGeometryTranslator::GetNormals(const MObject& geometry,
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

bool CGeometryTranslator::GetTangents(const MObject &geometry,
                                      AtArray*& tangents,
                                      AtArray*& bitangents,
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

   int nverts = fnMesh.numVertices();
   
   tangents = AiArrayAllocate(nverts, 1, AI_TYPE_VECTOR);
   bitangents = AiArrayAllocate(nverts, 1, AI_TYPE_VECTOR);
   
   for (int i = 0; i < nverts; ++i)
   {
      AiArraySetVec(tangents, i, AI_V3_ZERO);
      AiArraySetVec(bitangents, i, AI_V3_ZERO);
   }  
   
   std::vector<int> weights;
   weights.resize(nverts, 0);
   
   MItMeshPolygon iter(fnMesh.object());
   MIntArray vids;
   
   for (;!iter.isDone(); iter.next())
   {
      iter.getVertices(vids);
      for (unsigned int i = 0; i < vids.length(); ++i)
      {
         unsigned int tid = iter.tangentIndex(i);
         unsigned int vid = vids[i];
         ++weights[vid];
         MFloatVector tv = mayaTangents[tid];
         AtVector atv = AiArrayGetVec(tangents, vid);
         atv.x += tv.x;
         atv.y += tv.y;
         atv.z += tv.z;
         AiArraySetVec(tangents, vid, atv);
         tv = mayaBitangents[tid];
         atv = AiArrayGetVec(bitangents, vid);
         atv.x += tv.x;
         atv.y += tv.y;
         atv.z += tv.z;
         AiArraySetVec(bitangents, vid, atv);
      }
   }
   
   for (int i = 0; i < nverts; ++i)
   {
      if (weights[i] != 0)
      {
         const float w = 1.0f / (float)weights[i];
         
         AtVector atv = AiArrayGetVec(tangents, i);
         atv *= w;
         AiArraySetVec(tangents, i, atv);         
         
         atv = AiArrayGetVec(bitangents, i);
         atv *= w;
         AiArraySetVec(bitangents, i, atv);
      }
   }

   return true;
}

MDagPath CGeometryTranslator::GetMeshRefObj()
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

bool CGeometryTranslator::GetRefObj(const float*& refVertices,
                                    AtArray*& refNormals,
                                    AtArray*& rnidxs,
                                    AtArray*& refTangents,
                                    AtArray*& refBitangents)
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
         rnidxs = AiArrayAllocate(rNumNorms, 1, AI_TYPE_UINT);
         unsigned int id = 0;
         fnRefMesh.getNormalIds(vertex_counts, normal_ids);
         for(uint n(0); n < normal_ids.length(); ++n, ++id) AiArraySetUInt(rnidxs, id, normal_ids[n]);
         
      }

      MPlug pExportRefTangents = fnMesh.findPlug("aiExportRefTangents", false,
                                                 &stat);
      if (stat == MStatus::kSuccess && pExportRefTangents.asBool())
      {
         // Get tangents of the reference object in world space
         // Also, even if subdivision is applied we want to get the tangent data
         GetTangents(geometryRef, refTangents, refBitangents, MSpace::kWorld, true);
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

bool CGeometryTranslator::GetUVs(const MObject &geometry,
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
      if (numUVs < 1)
         continue;
      AtArray* uv = AiArrayAllocate(numUVs, 1, AI_TYPE_POINT2);
      
      MFloatArray uArray, vArray;
      fnMesh.getUVs(uArray, vArray, &uvName);
      
      for (int j = 0; j < numUVs; ++j)
      {
         AtPoint2 atv;
         atv.x = uArray[j];
         atv.y = vArray[j];
         AiArraySetPnt2(uv, j, atv);
      }
      uvs.push_back(uv);
      uvNames.push_back(uvName);
   }
   return uvs.size() > 0;
}

bool CGeometryTranslator::GetVertexColors(const MObject &geometry,
                                          std::map<std::string, std::vector<float> > &vcolors)
{
   MFnMesh fnMesh(geometry);

   bool exportColors = false;

   if (fnMesh.numColorSets() > 0)
   {
      MPlug plug = fnMesh.findPlug("aiExportColors");
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
      float scale = 1.0f;
      int dim = 4;
      MColor col;

      for (unsigned int j=0; j<numColorSets; ++j)
      {
         std::vector<float> &colors = vcolors[names[j].asChar()];
         colors.resize(fnMesh.numVertices() * dim, 0.0f);

         MObject meshObject = fnMesh.object();
         MItMeshVertex itVertex(meshObject);

         while (!itVertex.isDone())
         {
            faces.clear();
            itVertex.getConnectedFaces(faces);

            if (faces.length() > 0)
            {
               scale = 1.0f / float(faces.length());
               i = itVertex.index() * dim;

               for (unsigned int k=0; k<faces.length(); ++k)
               {
                  itVertex.getColor(col, faces[k], &names[j]);
                  for (int l=0; l<dim; ++l)
                     colors[i+l] += col[l];
               }
               for (int l=0; l<dim; ++l)
                  colors[i+l] *= scale;
            }

            itVertex.next();
         }
      }
   }
   return exportColors;
}

bool CGeometryTranslator::GetComponentIDs(const MObject &geometry,
      AtArray*& nsides,
      AtArray*& vidxs,
      AtArray*& nidxs,
      std::vector<AtArray*>& uvidxs,
      const std::vector<MString>& uvNames,
      bool exportNormals,
      bool exportUVs)
{

   MFnMesh fnMesh(geometry);

   int uv_id = 0;
   // Traverse all polygons to export vidxs, uvindxs and nsides
   unsigned int np = fnMesh.numPolygons();
   size_t numUVSets = uvNames.size();

   //While calling this function we assume that returning false
   // means "no polygons in this mesh"
   if (np == 0) return false;

   nsides = AiArrayAllocate(np, 1, AI_TYPE_UINT);
   unsigned int polygonVertexCount = 0; // for counting the number of ids
   for (unsigned int p(0); p < np; ++p)
   {
      int numPolygonVertexCount = fnMesh.polygonVertexCount(p);
      polygonVertexCount += numPolygonVertexCount;
      // Num points/sides to the poly.
      AiArraySetUInt(nsides, p, (unsigned int)numPolygonVertexCount);
   }
   vidxs = AiArrayAllocate(polygonVertexCount, 1, AI_TYPE_UINT);
   if (exportUVs)
   {
      uvidxs.resize(numUVSets);
      for (size_t i = 0; i < numUVSets; ++i)
         uvidxs[i] = AiArrayAllocate(polygonVertexCount, 1, AI_TYPE_UINT);
   }
   // Vertex indicies.
   MIntArray p_vidxs;
   unsigned int id = 0;
   for (unsigned int p(0); p < np; ++p)
   {
      fnMesh.getPolygonVertices(p, p_vidxs);
      for(uint v(0); v < p_vidxs.length(); ++v, ++id)
      {
         AiArraySetUInt(vidxs, id, p_vidxs[v]);
         // UVs
         if (exportUVs)
         {
            for (size_t i = 0; i < numUVSets; ++i)
            {
               if (fnMesh.getPolygonUVid(p, v, uv_id, &uvNames[i]) != MS::kSuccess)
               {
                  uv_id = 0;
                  AiMsgWarning("[MtoA] No uv coordinate exists for uv set %s at polygon %i at vertex %i on mesh %s.",
                               uvNames[i].asChar(), p, v, fnMesh.name().asChar());
               }
               AiArraySetUInt(uvidxs[i], id, uv_id);
            }
         }
      }
   }

   MIntArray vertex_counts, normal_ids;
   // Normals.
   if (exportNormals)
   {
      nidxs = AiArrayAllocate(polygonVertexCount, 1, AI_TYPE_UINT);
      id = 0;
      fnMesh.getNormalIds(vertex_counts, normal_ids);
      for(uint n(0); n < normal_ids.length(); ++n, ++id) AiArraySetUInt(nidxs, id, normal_ids[n]);
   }

   return true;

}

void CGeometryTranslator::ExportShaders()
{
   ExportMeshShaders(GetArnoldRootNode(), m_dagPath);
}

void CGeometryTranslator::GetDisplacement(MObject& obj, 
                                          float& dispPadding, 
                                          bool& enableAutoBump)
{
   MFnDependencyNode dNode(obj);
   MPlug plug = dNode.findPlug("aiDisplacementPadding");
   if (!plug.isNull())
      dispPadding = MAX(dispPadding, plug.asFloat());
   if (!enableAutoBump)
   {
      plug = dNode.findPlug("aiDisplacementAutoBump");
      if (!plug.isNull())
         enableAutoBump = enableAutoBump || plug.asBool();
   }
}

void CGeometryTranslator::ExportMeshShaders(AtNode* polymesh,
                                            const MDagPath &path)
{
   MFnMesh fnMesh(path);

   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;
   std::vector<AtNode*> meshDisps;

   MPlug shadingGroupPlug = GetNodeShadingGroup(path.node(), instanceNum);
   m_displaced = false;
   
   float maximumDisplacementPadding = -AI_BIG;
   bool enableAutoBump = false;
   MPlug plug;

   // Only one Shading Group applied to Mesh
   if (!shadingGroupPlug.isNull())
   {
      // SURFACE MATERIAL EXPORT
      AtNode *shader = ExportNode(shadingGroupPlug);
      if (shader != NULL)
      {
         // Push the shader in the vector to be assigned later to mtoa_shading_groups
         meshShaders.push_back(shader);
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
      MPlug shaderPlug = fnDGShadingGroup.findPlug("displacementShader");
      shaderPlug.connectedTo(connections, true, false);

      // are there any connections to displacementShader?
      if (connections.length() > 0)
      {
         m_displaced = true;
         MObject dispNode = connections[0].node();
         GetDisplacement(dispNode, maximumDisplacementPadding, enableAutoBump);
         
         AtNode* dispImage(ExportNode(connections[0]));
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
         MObject obGr = GetMayaObjectAttribute("objectGroups");
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
                  AtNode *shader = ExportNode(connections[j]);
                  if (shader != NULL)
                  {
                    meshShaders.push_back(shader);
                    exported = true;
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
         MPlug shaderPlug = fnDGShadingGroup.findPlug("displacementShader");
         shaderPlug.connectedTo(connections, true, false);

         // are there any connections to displacementShader?
         // If no connection found, add a NULL to meshDisps to match
         //  meshShaders distribution
         if (connections.length() > 0)
         {            
            m_displaced = true;
            MObject dispNode = connections[0].node();
            GetDisplacement(dispNode, maximumDisplacementPadding, enableAutoBump);
            AtNode* dispImage(ExportNode(connections[0]));
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
      
      if (fnMesh.findPlug("displaySmoothMesh").asBool())
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

      std::vector<unsigned int> shidxs;
      for (unsigned int i = 0; i < indices.length(); i++)
      {
         const int subdivisions = multiplier * fnMesh.polygonVertexCount(i);
         // indices[i] == -1 when a Shading Group is not connected
         const unsigned int indexToBePushed = (indices[i] == -1) ? 0 : indices[i];
         shidxs.push_back(indexToBePushed);
         for (int j = 0; j < subdivisions -1; j++)
            shidxs.push_back(indexToBePushed);
      }

      int numFaceShaders = (int)shidxs.size();
      if (numFaceShaders > 0)
      {
         AiNodeSetArray(polymesh, "shidxs", AiArrayConvert(numFaceShaders, 1, AI_TYPE_UINT, &(shidxs[0])));
      }
   }

   // Only export displacement attributes if a displacement is applied
   if (m_displaced)
   {
      // Note that disp_height has no actual influence on the scale of the displacement if it is vector based
      // it only influences the computation of the displacement bounds
      AiNodeSetFlt(polymesh, "disp_height",  FindMayaObjectPlug("aiDispHeight").asFloat());
      AiNodeSetFlt(polymesh, "disp_padding", MAX(maximumDisplacementPadding, FindMayaObjectPlug("aiDispPadding").asFloat()));
      AiNodeSetFlt(polymesh, "disp_zero_value", FindMayaObjectPlug("aiDispZeroValue").asFloat());
      AiNodeSetBool(polymesh, "disp_autobump", FindMayaObjectPlug("aiDispAutobump").asBool() || enableAutoBump);
   }

   // we must write this as user data bc AiNodeGet* is thread-locked while AIUDataGet* is not
   if (meshShaders.size() > 0)
   {
      AiNodeDeclare(polymesh, "mtoa_shading_groups", "constant ARRAY NODE");
      AiNodeSetArray(polymesh, "mtoa_shading_groups",
         AiArrayConvert(meshShaders.size(), 1, AI_TYPE_NODE, &(meshShaders[0])));
   }
}

void CGeometryTranslator::ExportMeshGeoData(AtNode* polymesh, unsigned int step)
{
   MFnMesh fnMesh(m_geometry);
   MObject geometry(m_geometry);
   
   //
   // GEOMETRY
   //   
   unsigned int numVerts = fnMesh.numVertices();
   unsigned int numNorms = fnMesh.numNormals();
   
   const float* vertices = 0;
   // Get all vertices
   bool exportVertices = GetVertices(geometry, vertices);

   const float* normals = 0;
   // Get all normals
   bool exportNormals = GetNormals(geometry, normals);  

   if (step == 0)
   {
      std::vector<AtArray*> uvs;
      std::vector<MString> uvNames;
      std::vector<AtArray*> uvidxs;
      AtArray* nsides = 0;
      AtArray* vidxs = 0; AtArray* nidxs = 0;
      std::map<std::string, std::vector<float> > vcolors;
      AtArray* refNormals = 0; AtArray* rnidxs = 0; AtArray* refTangents = 0; AtArray* refBitangents = 0;
      const float* refVertices = 0;

      // Get UVs
      bool exportUVs = GetUVs(geometry, uvs, uvNames);

      // Get reference objects
      bool exportReferenceObjects = GetRefObj(refVertices, refNormals, rnidxs,
                                              refTangents, refBitangents);
      bool exportRefVerts = refVertices != 0;
      bool exportRefNorms = refNormals != 0;
      bool exportRefTangents = refTangents != 0;

      // Get Component IDs
      bool exportCompIDs = GetComponentIDs(geometry, nsides, vidxs, nidxs, uvidxs, uvNames, exportNormals, exportUVs);
      // if GetComponentIDs returned false, it means that no polygons were found in the mesh. 
      // In that case uvidxs is empty, so we must not try to export the UVs
      if (!exportCompIDs) exportUVs = false;


      // Get Vertex Colors
      MPlug plug = FindMayaPlug("aiMotionVectorSource");
      if (!plug.isNull())
         m_motionVectorSource = plug.asString();
      bool exportColors = GetVertexColors(geometry, vcolors);

      // Get all tangents, bitangents
      AtArray* tangents; AtArray* bitangents;
      if (GetTangents(geometry, tangents, bitangents, MSpace::kObject)) // Arnold doesn't support motion blurred user data
      {
         AiNodeDeclare(polymesh, "tangent", "varying VECTOR");
         AiNodeDeclare(polymesh, "bitangent", "varying VECTOR");
         
         AiNodeSetArray(polymesh, "tangent", tangents);
         AiNodeSetArray(polymesh, "bitangent", bitangents);
      }

      if (exportReferenceObjects)
      {
         if (exportRefVerts)
            AiNodeDeclare(polymesh, "Pref", "varying POINT");
         if (exportRefNorms)
            AiNodeDeclare(polymesh, "Nref", "indexed VECTOR");
            AiNodeDeclare(polymesh, "Nrefidxs", "indexed UINT");
         if (exportRefTangents)
         {
            AiNodeDeclare(polymesh, "Tref", "varying VECTOR");
            AiNodeDeclare(polymesh, "BTref", "varying VECTOR");
         }
      }

      // Declare user parameters for color sets
      if (exportColors)
      {
         std::map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            AiNodeDeclare(polymesh, it->first.c_str(), "varying RGBA");
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
               AtArray* verticesArray = AiArrayAllocate(numVerts, 2, AI_TYPE_POINT);
               const float* vert = vertices;
               float motionRange = (float)m_session->GetMotionByFrame() * motionVectorScale;
               if (motionVectorUnit == 1)
               {
                  MTime oneSec(1.0, MTime::kSeconds);
                  const float fps =  (float)oneSec.asUnits(MTime::uiUnit());
                  motionRange /= fps;
               }
               for (unsigned int i = 0; i < numVerts; ++i)
               {                  
                  AtVector vec = {*(vert++), *(vert++), *(vert++)};
                  AiArraySetPnt(verticesArray, i, vec);
                  const AtRGBA* motionVector = motionVectorColors + i;
                  vec.x += motionVector->r * motionRange;
                  vec.y += motionVector->g * motionRange;
                  vec.z += motionVector->b * motionRange;
                  AiArraySetPnt(verticesArray, i + numVerts, vec);
               }
               AiNodeSetArray(polymesh, "vlist", verticesArray);
            }
            if (exportNormals)
            {
               AtArray* normalsArray = AiArrayAllocate(numNorms, 2, AI_TYPE_VECTOR);
               const float* norm = normals;
               for (unsigned int i = 0; i < numNorms; ++i)
               {                  
                  AtVector vec = {*(norm++), *(norm++), *(norm++)};
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
               AtArray* vlist_array = AiArrayAllocate(numVerts, GetNumMotionSteps(), AI_TYPE_POINT);
               SetKeyData(vlist_array, step, vertices, numVerts);
               AiNodeSetArray(polymesh, "vlist", vlist_array);
            }
            if (exportNormals)
            {
               AtArray* nlist_array = AiArrayAllocate(numNorms, GetNumMotionSteps(), AI_TYPE_VECTOR);
               SetKeyData(nlist_array, step, normals, numNorms);
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
      }
      
      if (exportReferenceObjects) // TODO : use local space for this and manually transform that later, 
         // that makes a few functions much simpler
      {         
         if (exportRefVerts)
         {
            AtMatrix worldMatrix;
            ConvertMatrix(worldMatrix, m_dagPathRef.inclusiveMatrix(), m_session);
            AtArray* aRefVertices = AiArrayAllocate(numVerts, 1, AI_TYPE_POINT);
            const AtVector* vRefVertices = (const AtVector*)refVertices;
            for (unsigned int i = 0; i < numVerts; ++i)
            {
               AtVector v;
               AiM4PointByMatrixMult(&v, worldMatrix, vRefVertices + i);
               AiArraySetVec(aRefVertices, i, v);
            }
            AiNodeSetArray(polymesh, "Pref", aRefVertices);
         }
         if (exportRefNorms)
            AiNodeSetArray(polymesh, "Nref", refNormals);
            AiNodeSetArray(polymesh, "Nrefidxs", rnidxs);
         if (exportRefTangents)
         {
            AiNodeSetArray(polymesh, "Tref", refTangents);
            AiNodeSetArray(polymesh, "BTref", refBitangents);
         }
      }
      if (exportUVs)
      {
         if (uvs.size() > 0 && uvidxs.size() > 0)
         {
            AiNodeSetArray(polymesh, "uvlist", uvs[0]);
            AiNodeSetArray(polymesh, "uvidxs", uvidxs[0]);
            for (size_t i = 1; i < uvs.size(); ++i)
            {
               if (uvNames.size() > i && uvidxs.size() > i)
               {
                  MString idxsName = uvNames[i] + MString("idxs");
                  AiNodeDeclare(polymesh, uvNames[i].asChar(), "indexed POINT2");
                  AiNodeSetArray(polymesh, uvNames[i].asChar(), uvs[i]);
                  AiNodeSetArray(polymesh, idxsName.asChar(), uvidxs[i]);
               }
            }
         }
      }
      if (exportColors)
      {
         std::map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            AiNodeSetArray(polymesh, it->first.c_str(), AiArrayConvert(numVerts, 1, AI_TYPE_RGBA, &(it->second[0])));
            ++it;
         }
      }

      // exporting crease edges
      // for the first version we always export them
      // since the user might override the subdiv options
      // from a procedural, node processor etc...
      if (!fnMesh.findPlug("displaySmoothMesh").asBool())
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
   } // step == 0
   else if (!m_useMotionVectors)
   {
      // Export motion blur keys information (for deformation)

      // Vertices
      if (exportVertices)
      {
         AtArray* vlist_array = AiNodeGetArray(polymesh, "vlist");
         if (vlist_array->nelements != numVerts)
            AiMsgError("[mtoa.translator]  %-30s | Number of vertices changed between motion steps: %d -> %d",
                       GetMayaNodeName().asChar(), vlist_array->nelements, numVerts);
         else
            SetKeyData(vlist_array, step, vertices, numVerts);

      }
      // Normals
      if (exportNormals)
      {
         AtArray* nlist_array = AiNodeGetArray(polymesh, "nlist");
         if (nlist_array->nelements != numNorms)
            AiMsgError("[mtoa.translator]  %-30s | Number of normals changed between motion steps: %d -> %d",
                       GetMayaNodeName().asChar(), nlist_array->nelements, numNorms);
         else
            SetKeyData(nlist_array, step, normals, numNorms);
      }
   }
}

// Specific implementation for each geometry type
bool CGeometryTranslator::IsGeoDeforming()
{
   return true;
}

void CGeometryTranslator::ExportMeshParameters(AtNode* polymesh)
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

      ProcessParameter(polymesh, "subdiv_dicing_camera", AI_TYPE_NODE, "aiSubdivDicingCamera");
   }
}

void CGeometryTranslator::ExportBBox(AtNode* polymesh)
{
   ExportMatrix(polymesh, 0);
   // Visibility options
   ProcessRenderFlags(polymesh);

   if (FindMayaPlug("doubleSided").asBool())
      AiNodeSetByte(polymesh, "sidedness", AI_RAY_ALL);
   else
   {
      AiNodeSetBool(polymesh, "invert_normals", FindMayaPlug("opposite").asBool());
      AiNodeSetByte(polymesh, "sidedness", 0);
   }

   if ((CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER) ||
       CMayaScene::GetRenderSession()->RenderOptions()->forceTranslateShadingEngines())
      ExportMeshShaders(polymesh, m_dagPath);
   ExportLightLinking(polymesh);

   MFnMesh fnMesh(m_geometry);
   MBoundingBox bbox = fnMesh.boundingBox();
   AiNodeSetPnt(polymesh, "min", (float)bbox.min().x, (float)bbox.min().y, (float)bbox.min().z);
   AiNodeSetPnt(polymesh, "max", (float)bbox.max().x, (float)bbox.max().y, (float)bbox.max().z);
   AiNodeSetFlt(polymesh, "step_size", FindMayaPlug("aiStepSize").asFloat());
}

AtNode* CGeometryTranslator::ExportMesh(AtNode* polymesh, bool update)
{
   
   // Check if this geometry is renderable
   // if it is not, set it as Disabled
   AiNodeSetDisabled(polymesh, !m_session->IsRenderablePath(m_dagPath));
   
   ExportMatrix(polymesh, 0);   
   ExportMeshParameters(polymesh);
   if ((CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER) ||
       CMayaScene::GetRenderSession()->RenderOptions()->forceTranslateShadingEngines())
      ExportMeshShaders(polymesh, m_dagPath);
   ExportLightLinking(polymesh);
   // if enabled, double check motion deform
   m_motionDeform = m_motionDeform && IsGeoDeforming();
   if (!update)
   {
      ExportMeshGeoData(polymesh, 0);
   }

   return polymesh;
}

AtNode* CGeometryTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.partialPathName().asChar());

   int instanceNum = m_dagPath.instanceNumber();

   ExportMatrix(instance, 0);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);
   
   AtByte visibility = ComputeVisibility();
   AiNodeSetByte(instance, "visibility", visibility);

   if ((CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER) ||
       CMayaScene::GetRenderSession()->RenderOptions()->forceTranslateShadingEngines())
   {
      //
      // SHADERS
      //
      // MFnMesh           meshNode(m_dagPath.node());
      MFnMesh meshNode(m_geometry);
      MPlug plug = meshNode.findPlug("instObjGroups");

      MPlugArray conns0, connsI;

      bool shadersDifferent = false;

      // checking the connections from the master instance
      plug.elementByLogicalIndex(0).connectedTo(conns0, false, true); 
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
         MPlug shadingGroupPlug = GetNodeShadingGroup(m_geometry, instanceNum);

         // In case Instance has per face assignment, use first SG assigned to it
         if(shadingGroupPlug.isNull())
         {
            MPlugArray        connections;
            MFnDependencyNode fnDGNode(m_geometry);
            MPlug plug(m_geometry, fnDGNode.attribute("instObjGroups"));
            plug = plug.elementByLogicalIndex(instanceNum);
            MObject obGr = GetMayaObjectAttribute("objectGroups");
            plug = plug.child(obGr);
            plug.elementByPhysicalIndex(0).connectedTo(connections, false, true);
            if(connections.length() > 0)
            {
               shadingGroupPlug = connections[0];
            }
         }

         AtNode* shader = ExportNode(shadingGroupPlug);
         AiNodeSetPtr(instance, "shader", shader);
         // we must write this as user data bc AiNodeGet* is thread-locked while AIUDataGet* is not
         AiNodeDeclare(instance, "mtoa_shading_groups", "constant ARRAY NODE");
         AiNodeSetArray(instance, "mtoa_shading_groups",
               AiArrayConvert(1, 1, AI_TYPE_NODE, &shader));
      }
   }
   // Export light linking per instance
   ExportLightLinking(instance);

   return instance;
}

void CGeometryTranslator::Update(AtNode *anode)
{
   if (IsMasterInstance())
   {
      ExportMesh(anode, true);
   }
   else
   {
      ExportInstance(anode, GetMasterInstance());
   }
}

void CGeometryTranslator::ExportMotion(AtNode* anode, unsigned int step)
{
   if (IsMasterInstance())
   {
      ExportMatrix(anode, step);
      if (m_motionDeform)
      {
         ExportMeshGeoData(anode, step);
      }
   }
   else
   {
      ExportMatrix(anode, step);
   }
}

void CGeometryTranslator::UpdateMotion(AtNode* anode, unsigned int step)
{
   ExportMatrix(anode, step);
}

void CGeometryTranslator::AddUpdateCallbacks()
{
   MObject dagPathNode= m_dagPath.node();
   AddShaderAssignmentCallbacks(dagPathNode);
   CDagTranslator::AddUpdateCallbacks();
}

void CGeometryTranslator::AddShaderAssignmentCallbacks(MObject & dagNode)
{
   MStatus status;
   MCallbackId id = MNodeMessage::addAttributeChangedCallback(dagNode, ShaderAssignmentCallback, this, &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);
}

void CGeometryTranslator::ShaderAssignmentCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void*clientData)
{
   // Shading assignments are done with the instObjGroups attr, so we only
   // need to update when that is the attr that changes.
   if ((msg & MNodeMessage::kConnectionMade) && (plug.partialName() == "iog"))
   {
      CGeometryTranslator * translator = static_cast< CGeometryTranslator* >(clientData);
      if (translator != NULL)
      {
         // Problem ! If a rendering is in progress, I can't create these new nodes
         // so I need to interrupt the rendering, even if Continuous Updates are OFF
         if (!AiRendering())
         {
            translator->ExportShaders();
            translator->RequestUpdate();
         } else
         {
            CMayaScene::GetRenderSession()->InterruptRender(true);
            // Export the new shaders.
            translator->ExportShaders();
   		
            // Update Arnold without passing a translator, this just forces a redraw.
            translator->RequestUpdate();
            
         }
      }
   }
}

void CGeometryTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "polymesh");

   // Node attributes
   CShapeTranslator::MakeCommonAttributes(helper);

   helper.MakeInput("subdiv_type");
   helper.MakeInput("subdiv_iterations");
   helper.MakeInput("subdiv_adaptive_metric");
   helper.MakeInput("subdiv_adaptive_error");
   helper.MakeInput("subdiv_adaptive_space");
   helper.MakeInput("subdiv_dicing_camera");
   helper.MakeInput("subdiv_uv_smoothing");
   helper.MakeInput("subdiv_smooth_derivs");

   helper.MakeInput("disp_height");
   helper.MakeInput("disp_padding");
   helper.MakeInput("disp_zero_value");
   helper.MakeInput("disp_autobump");

   CAttrData data;

   data.defaultValue.BOOL = false;
   data.name = "aiExportTangents";
   data.shortName = "ai_exptan";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiExportColors";
   data.shortName = "ai_expcol";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL = true;
   data.name = "aiExportRefPoints";
   data.shortName = "ai_exprpt";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiExportRefNormals";
   data.shortName = "ai_exprnrm";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiExportRefTangents";
   data.shortName = "ai_exprtan";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);
      
   data.defaultValue.FLT = 0.f;
   data.name = "aiStepSize";
   data.shortName = "ai_step_size";
   data.channelBox = false;
   data.hasMin = true;
   data.min.FLT = 0.f;
   data.hasSoftMax = true;
   data.softMax.FLT = 1.f;
   helper.MakeInputFloat(data);

   data.stringDefault = "velocityPV";
   data.name = "aiMotionVectorSource";
   data.shortName = "ai_motion_vector_source";
   data.channelBox = false;
   helper.MakeInputString(data);

   data.defaultValue.INT = 0;
   data.name = "aiMotionVectorUnit";
   data.shortName = "ai_motion_vector_unit";
   data.channelBox = false;
   data.enums = MStringArray();
   data.enums.append("Per Frame");
   data.enums.append("Per Second");
   helper.MakeInputEnum(data);

   data.defaultValue.FLT = 1.f;
   data.name = "aiMotionVectorScale";
   data.shortName = "ai_motion_vector_scale";
   data.hasMin = false;
   data.hasMax = false;
   data.hasSoftMin = true;
   data.hasSoftMax = true;
   data.softMin.FLT = 0.f;
   data.softMax.FLT = 2.f;
   data.channelBox = false;
   helper.MakeInputFloat(data);
}
