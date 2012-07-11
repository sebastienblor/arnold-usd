
#include "GeometryTranslator.h"

#include <maya/MNodeMessage.h>

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

bool CGeometryTranslator::GetVertices(const MObject &geometry,
                                      std::vector<float> &vertices,
                                      MSpace::Space space)
{
   MStatus status;
   MFnMesh fnMesh(geometry);
   int nverts = fnMesh.numVertices();
   if (nverts > 0)
   {
      vertices.resize(nverts * 3);

      MFloatPointArray pointsArray;
      if (space == MSpace::kWorld)
      {
         // A mesh has no transform, we must trace it back in the DAG
         // FIXME : won't work for smoothed mesh or tessellated nurbs
         MDagPath dp;
         status = fnMesh.getPath(dp);
         CHECK_MSTATUS(status);
         MFnMesh pointFn(dp);
         pointFn.getPoints(pointsArray, MSpace::kWorld);
      }
      else
      {
         fnMesh.getPoints(pointsArray, space);
      }
      for (int J = 0; (J < nverts); ++J)
      {
         vertices[J * 3 + 0] = pointsArray[J].x;
         vertices[J * 3 + 1] = pointsArray[J].y;
         vertices[J * 3 + 2] = pointsArray[J].z;
      }
      return true;
   }
   return false;
}

bool CGeometryTranslator::GetPerVertexNormals(const MObject &geometry,
                                              std::vector<float> &normals,
                                              MSpace::Space space,
                                              bool force)
{
   MStatus status;
   MFnMesh fnMesh(geometry);

   int nnorms = fnMesh.numNormals();
   if (nnorms > 0 && (force || (FindMayaPlug("smoothShading").asBool() && !FindMayaPlug("aiSubdivType").asBool())))
   {
      normals.resize(fnMesh.numVertices() * 3);

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

      for (int J = 0; (J < fnMesh.numVertices()); ++J)
      {
         normals[J * 3 + 0] = normalArray[J].x;
         normals[J * 3 + 1] = normalArray[J].y;
         normals[J * 3 + 2] = normalArray[J].z;
      }
      return true;
   }
   return false;
}

bool CGeometryTranslator::GetNormals(const MObject &geometry,
                                     std::vector<float> &normals)
{
   MFnMesh fnMesh(geometry);

   int nnorms = fnMesh.numNormals();
   if (FindMayaPlug("smoothShading").asBool() &&
         !FindMayaPlug("aiSubdivType").asBool() &&
         nnorms > 0)
   {
      normals.resize(nnorms * 3);

      MFloatVectorArray normalArray;
      fnMesh.getNormals(normalArray, MSpace::kObject);

      for (int J = 0; (J < nnorms); ++J)
      {
         normals[J * 3 + 0] = normalArray[J].x;
         normals[J * 3 + 1] = normalArray[J].y;
         normals[J * 3 + 2] = normalArray[J].z;
      }
      return true;
   }
   return false;
}

bool CGeometryTranslator::GetTangents(const MObject &geometry,
                                      std::vector<float> &tangents,
                                      std::vector<float> &bitangents,
                                      MSpace::Space space,
                                      bool force)
{
   MStatus status;
   MFnMesh fnMesh(geometry);

   bool doExport;
   if (force)
   {
      doExport = true;
   }
   else
   {
      MPlug pExportTangents = fnMesh.findPlug("aiExportTangents", false, &status);
      doExport = (status == MStatus::kSuccess && pExportTangents.asBool());
   }

   if (!doExport)
      return false;

   MObject meshObject = fnMesh.object();
   MItMeshVertex itVertex(meshObject);
   MIntArray iarray;
   MVector ttmp, btmp, tangent, bitangent;
   float scale = 1.0f;
   unsigned int i = 0;

   int nverts = fnMesh.numVertices();

   tangents.resize(nverts * 3);
   bitangents.resize(nverts * 3);

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
   {
      tangentFn.setObject(fnMesh.object());
   }
    
   while (!itVertex.isDone())
   {
      iarray.clear();
      itVertex.getConnectedFaces(iarray);

      i = itVertex.index() * 3;

      tangent = MVector::zero;
      bitangent = MVector::zero;

      if (iarray.length() > 0)
      {
         scale = 1.0f / float(iarray.length());

         for (unsigned int j=0; j<iarray.length(); ++j)
         {
            tangentFn.getFaceVertexTangent(iarray[j], itVertex.index(), ttmp, space);
            tangentFn.getFaceVertexBinormal(iarray[j], itVertex.index(), btmp, space);
            tangent += ttmp;
            bitangent += btmp;
         }

         tangent *= scale;
         bitangent *= scale;
      }

      tangents[i] = (float) tangent.x;
      tangents[i+1] = (float) tangent.y;
      tangents[i+2] = (float) tangent.z;

      bitangents[i] = (float) bitangent.x;
      bitangents[i+1] = (float) bitangent.y;
      bitangents[i+2] = (float) bitangent.z;

      itVertex.next();
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

bool CGeometryTranslator::GetRefObj(std::vector<float> &refVertices,
                                    std::vector<float> &refNormals,
                                    std::vector<float> &refTangents,
                                    std::vector<float> &refBitangents)
{
   MFnMesh fnMesh(m_dagPath);
   MDagPath dagPathRef = GetMeshRefObj();
   MObject geometryRef = dagPathRef.node();

   if (dagPathRef.isValid())
   {
      // Find whether we're exporting points/normals/tangents
      MStatus stat;
      MPlug pExportRefPoints = fnMesh.findPlug("aiExportRefPoints", false,
                                               &stat);
      if (stat == MStatus::kSuccess && pExportRefPoints.asBool())
      {
         // Get vertices of the reference object in world space
         GetVertices(geometryRef, refVertices, MSpace::kWorld);
      }

      MPlug pExportRefNormals = fnMesh.findPlug("aiExportRefNormals", false,
                                                &stat);
      if (stat == MStatus::kSuccess && pExportRefNormals.asBool())
      {
         // Get normals of the reference object (as we are outputing this as a user-data varying data,
         // we must have 1 normal per vertex
         // Also, even if subdivision is applied we want to get the normals data
         GetPerVertexNormals(geometryRef, refNormals, MSpace::kWorld, true);
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
                                 std::vector<float> &uvs)
{
   MFnMesh fnMesh(geometry);

   // Get all UVs
   if (fnMesh.numUVs() > 0)
   {
      uvs.resize(fnMesh.numUVs() * 2);

      MFloatArray uArray, vArray;
      fnMesh.getUVs(uArray, vArray);

      for (int J = 0; (J < fnMesh.numUVs()); ++J)
      {
         uvs[J * 2 + 0] = uArray[J];
         uvs[J * 2 + 1] = vArray[J];
      }
      return true;
   }
   return false;
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
      {
         exportColors = plug.asBool();
      }
   }
   if (exportColors)
   {
      MStringArray names;
      MIntArray faces;
      unsigned int i = 0;
      float scale = 1.0f;
      int dim = 4;
      MColor col;

      fnMesh.getColorSetNames(names);
      for (unsigned int j=0; j<names.length(); ++j)
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
                  {
                     colors[i+l] += col[l];
                  }
               }
               for (int l=0; l<dim; ++l)
               {
                  colors[i+l] *= scale;
               }
            }

            itVertex.next();
         }
      }
   }
   return exportColors;
}

bool CGeometryTranslator::GetComponentIDs(const MObject &geometry,
      std::vector<unsigned int> &nsides,
      std::vector<AtUInt32> &vidxs,
      std::vector<AtUInt32> &nidxs,
      std::vector<AtUInt32> &uvidxs,
      bool exportNormals,
      bool exportUVs)
{
   MFnMesh fnMesh(geometry);

   int uv_id = 0;
   // Traverse all polygons to export vidxs, uvindxs and nsides
   unsigned int np = fnMesh.numPolygons();
   if (np > 0)
   {
      nsides.reserve(np);
      for (unsigned int p(0); p < np; ++p)
      {
         // Num points/sides to the poly.
         nsides.push_back(fnMesh.polygonVertexCount(p));
         // Vertex indicies.
         MIntArray p_vidxs;
         fnMesh.getPolygonVertices(p, p_vidxs);
         for(uint v(0); v < p_vidxs.length(); ++v)
         {
            vidxs.push_back(p_vidxs[v]);
            // UVs
            if (exportUVs)
            {
               fnMesh.getPolygonUVid(p, v, uv_id);
               uvidxs.push_back(uv_id);
            }
         }
      }

      // Normals.
      if (exportNormals)
      {
         MIntArray vertex_counts, normal_ids;
         fnMesh.getNormalIds(vertex_counts, normal_ids);
         for(uint n(0); n < normal_ids.length(); ++n) nidxs.push_back(normal_ids[n]);
      }

      return true;
   }

   return false;
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
      
      int numMeshShaders = (int)meshShaders.size();
      if (numMeshShaders > 0)
      { 
         AiNodeSetArray(polymesh, "shader", AiArrayConvert(numMeshShaders, 1, AI_TYPE_NODE, &meshShaders[0]));
      }

      int numMeshDisps = (int)meshDisps.size();
      if (numMeshDisps > 0)
      { 
         AiNodeSetArray(polymesh, "disp_map", AiArrayConvert(numMeshDisps, 1, AI_TYPE_NODE, &meshDisps[0]));
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
         
         divisions = options.divisions();
         if(divisions > 0)
            multiplier = static_cast<int> (pow(4.0f, (divisions-1)));
      }
      
      std::vector<unsigned int> shidxs;
      for (unsigned int i = 0; i < indices.length(); i++)
      {
         int subdivisions = multiplier * fnMesh.polygonVertexCount(i);
         // indices[i] == -1 when a Shading Group is not connected
         if (indices[i] == -1)
            shidxs.push_back(0);
         else
            shidxs.push_back(indices[i]);
         for (int j = 0; j < subdivisions -1; j++)
         {
            if (indices[i] == -1)
               shidxs.push_back(0);
            else
               shidxs.push_back(indices[i]);
         }
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
   std::vector<float> vertices, normals, tangents, bitangents;
   unsigned int numVerts = fnMesh.numVertices();
   unsigned int numNorms = fnMesh.numNormals();
   unsigned int numUVs = fnMesh.numUVs();
   unsigned int numPolys = fnMesh.numPolygons();

   // Get all vertices
   bool exportVertices = GetVertices(geometry, vertices, MSpace::kObject);

   // Get all normals
   bool exportNormals = GetNormals(geometry, normals);

   // Get all tangents, bitangents
   bool exportTangents = GetTangents(geometry, tangents, bitangents,
                                     MSpace::kObject);

   if (step == 0)
   {
      std::vector<float> uvs;
      std::vector<unsigned int> nsides;
      std::vector<AtUInt32> vidxs, nidxs, uvidxs;
      std::map<std::string, std::vector<float> > vcolors;
      std::vector<float> refVertices, refNormals, refTangents, refBitangents;

      // Get UVs
      bool exportUVs = GetUVs(geometry, uvs);

      // Get reference objects
      bool exportReferenceObjects = GetRefObj(refVertices, refNormals,
                                              refTangents, refBitangents);
      bool exportRefVerts = (refVertices.size() > 0);
      bool exportRefNorms = (refNormals.size() > 0);
      bool exportRefTangents = (refTangents.size() > 0 && refBitangents.size() > 0);

      // Get Component IDs
      bool exportCompIDs = GetComponentIDs(geometry, nsides, vidxs, nidxs, uvidxs, exportNormals, exportUVs);
      // Get Vertex Colors
      bool exportColors = GetVertexColors(geometry, vcolors);

      // Declare user parameters for tangents
      if (exportTangents)
      {
         AiNodeDeclare(polymesh, "tangent", "varying VECTOR");
         AiNodeDeclare(polymesh, "bitangent", "varying VECTOR");
      }

      if (exportReferenceObjects)
      {
         if (exportRefVerts)
         {
            AiNodeDeclare(polymesh, "Pref", "varying POINT");
         }
         if (exportRefNorms)
         {
            AiNodeDeclare(polymesh, "Nref", "varying VECTOR");
         }
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
            if (strcmp(it->first.c_str(), "sss_faceset") != 0)
            {
               AiNodeDeclare(polymesh, it->first.c_str(), "varying RGBA");
            }
            else
            {
               AiNodeDeclare(polymesh, "sss_faceset", "uniform BOOL");
            }
            ++it;
         }
      }

      if (!m_motionDeform || !IsLocalMotionBlurEnabled())
      {
         // No deformation motion blur, so we create normal arrays
         if (exportVertices)
         {
            AiNodeSetArray(polymesh, "vlist", AiArrayConvert(numVerts * 3, 1, AI_TYPE_FLOAT, &(vertices[0])));
         }
         if (exportNormals)
         {
            AiNodeSetArray(polymesh, "nlist", AiArrayConvert(numNorms * 3, 1, AI_TYPE_FLOAT, &(normals[0])));
         }
         if (exportTangents)
         {
            AiNodeSetArray(polymesh, "tangent", AiArrayConvert(numVerts, 1, AI_TYPE_VECTOR, &(tangents[0])));
            AiNodeSetArray(polymesh, "bitangent", AiArrayConvert(numVerts, 1, AI_TYPE_VECTOR, &(bitangents[0])));
         }
      }
      else
      {
         // Deformation motion blur. We need to create keyable arrays for vlist and nlist
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
         if (exportTangents)
         {
            AtArray* tangent_array = AiArrayAllocate(numVerts, GetNumMotionSteps(), AI_TYPE_VECTOR);
            SetKeyData(tangent_array, step, tangents, numVerts);
            AiNodeSetArray(polymesh, "tangent", tangent_array);

            AtArray* bitangent_array = AiArrayAllocate(numVerts, GetNumMotionSteps(), AI_TYPE_VECTOR);
            SetKeyData(bitangent_array, step, bitangents, numVerts);
            AiNodeSetArray(polymesh, "bitangent", bitangent_array);
         }
      }

      if (exportCompIDs)
      {
         AiNodeSetArray(polymesh, "nsides", AiArrayConvert((int)nsides.size(), 1, AI_TYPE_UINT, &(nsides[0])));

         // Passing vidxs directly put Arnold in trouble
         //AiNodeSetArray(polymesh, "vidxs", AiArrayConvert(vidxs.size(), 1, AI_TYPE_UINT, &(vidxs[0])));
         AtArray *vidxsTmp = AiArrayAllocate((int)vidxs.size(), 1, AI_TYPE_UINT);
         for(unsigned int i = 0; (i < vidxs.size()); i++)
         {
            AiArraySetUInt(vidxsTmp, i, vidxs[i]);
         }
         AiNodeSetArray(polymesh, "vidxs", vidxsTmp);
      }
      if (exportNormals)
      {
         // Same goes here
         //AiNodeSetArray(polymesh, "nidxs", AiArrayConvert(nidxs.size(), 1, AI_TYPE_UINT, &(nidxs[0])));
         AtArray *nidxsTmp = AiArrayAllocate((int)nidxs.size(), 1, AI_TYPE_UINT);
         for(unsigned int i = 0; (i < nidxs.size()); i++)
            AiArraySetUInt(nidxsTmp, i, nidxs[i]);
         AiNodeSetArray(polymesh, "nidxs", nidxsTmp);
      }
      if (exportReferenceObjects)
      {
          if (exportRefVerts)
         {
            AiNodeSetArray(polymesh, "Pref", AiArrayConvert(numVerts, 1, AI_TYPE_POINT, &(refVertices[0])));
         }
         if (exportRefNorms)
         {
            AiNodeSetArray(polymesh, "Nref", AiArrayConvert(numNorms, 1, AI_TYPE_VECTOR, &(refNormals[0])));
         }
         if (exportRefTangents)
         {
            AiNodeSetArray(polymesh, "Tref", AiArrayConvert(numVerts, 1, AI_TYPE_VECTOR, &(refTangents[0])));
            AiNodeSetArray(polymesh, "BTref", AiArrayConvert(numVerts, 1, AI_TYPE_VECTOR, &(refBitangents[0])));
         }
      }
      if (exportUVs)
      {
         AiNodeSetArray(polymesh, "uvlist", AiArrayConvert(numUVs * 2, 1, AI_TYPE_FLOAT, &(uvs[0])));
         // Same problem here
         //AiNodeSetArray(polymesh, "uvidxs", AiArrayConvert(uvidxs.size(), 1, AI_TYPE_UINT, &(uvidxs[0])));
         AtArray *uvidxsTmp = AiArrayAllocate((int)uvidxs.size(), 1, AI_TYPE_UINT);
         for(unsigned int i = 0; (i < uvidxs.size()); i++)
            AiArraySetUInt(uvidxsTmp, i, uvidxs[i]);
         AiNodeSetArray(polymesh, "uvidxs", uvidxsTmp);
      }
      if (exportColors)
      {
         std::map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            if (strcmp(it->first.c_str(), "sss_faceset") != 0)
            {
               AiNodeSetArray(polymesh, it->first.c_str(), AiArrayConvert(numVerts, 1, AI_TYPE_RGBA, &(it->second[0])));
            }
            else
            {
               int m_colorId;
               float m_count = 0.0f;
               MColorArray colors;
               MString m_colorSetName = "sss_faceset";
               MColor m_defaultColor  = MColor(0.0f, 0.0f, 0.0f);

               AtArray *m_sss_faceset_bool = AiArray(numPolys, 1, AI_TYPE_BOOLEAN, NULL);

               fnMesh.getFaceVertexColors(colors, &m_colorSetName, &m_defaultColor);

               for (int m_polygonId = 0; (m_polygonId < (int)numPolys); m_polygonId++)
               {
                  MIntArray m_vertexList;
                  fnMesh.getPolygonVertices(m_polygonId, m_vertexList);

                  m_count = 0.0f;
                  for (int m_vertexId = 0; (m_vertexId < (int)m_vertexList.length()); m_vertexId++)
                  {
                     fnMesh.getFaceVertexColorIndex(m_polygonId, m_vertexId, m_colorId, &m_colorSetName);
                     m_count += (colors[m_colorId][0]+colors[m_colorId][1]+colors[m_colorId][2])/3.0f;
                  }
                  if (m_count/(float)m_vertexList.length() >= 0.5f)
                  {
                     AiArraySetBool(m_sss_faceset_bool, m_polygonId, true);
                  }
                  else
                  {
                     AiArraySetBool(m_sss_faceset_bool, m_polygonId, false);
                  }
               }
               AiNodeSetArray(polymesh, "sss_faceset", m_sss_faceset_bool);
            }
            ++it;
         }
      }
   } // step == 0
   else
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
      // Tangents
      if (exportTangents)
      {
         AtArray* tangent_array = AiNodeGetArray(polymesh, "tangent");
         if (tangent_array->nelements != numVerts)
            AiMsgError("[mtoa.translator]  %-30s | Number of tangents changed between motion steps: %d -> %d",
                       GetMayaNodeName().asChar(), tangent_array->nelements, numVerts);
         else
            SetKeyData(tangent_array, step, tangents, numVerts);

         AtArray* bitangent_array = AiNodeGetArray(polymesh, "bitangent");
         if (bitangent_array->nelements != numVerts)
            AiMsgError("[mtoa.translator]  %-30s | Number of bi-tangents changed between motion steps: %d -> %d",
                       GetMayaNodeName().asChar(), bitangent_array->nelements, numVerts);
         else
            SetKeyData(bitangent_array, step, bitangents, numVerts);
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
      AiNodeSetInt(polymesh, "sidedness", 65535);
   else
   {
      AiNodeSetBool(polymesh, "invert_normals", FindMayaPlug("opposite").asBool());
      AiNodeSetInt(polymesh, "sidedness", 0);
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
      AiNodeSetInt(polymesh, "subdiv_iterations",     FindMayaPlug("aiSubdivIterations").asInt());
      AiNodeSetInt(polymesh, "subdiv_adaptive_metric",FindMayaPlug("aiSubdivAdaptiveMetric").asInt());
      AiNodeSetFlt(polymesh, "subdiv_pixel_error",    FindMayaPlug("aiSubdivPixelError").asFloat());
      AiNodeSetInt(polymesh, "subdiv_uv_smoothing",   FindMayaPlug("aiSubdivUvSmoothing").asInt());
      AiNodeSetBool(polymesh, "subdiv_smooth_derivs", FindMayaPlug("aiSubdivSmoothDerivs").asBool());

      ProcessParameter(polymesh, "subdiv_dicing_camera", AI_TYPE_NODE, "aiSubdivDicingCamera");

   }
}

AtNode* CGeometryTranslator::ExportMesh(AtNode* polymesh, bool update)
{
   ExportMatrix(polymesh, 0);
   ExportMeshParameters(polymesh);
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
   
   int visibility = AiNodeGetInt(masterNode, "visibility");
   AiNodeSetInt(instance, "visibility", visibility);

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
      MObjectArray shaders;
      MIntArray indices;
      meshNode.getConnectedShaders(instanceNum, shaders, indices);
      
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
            AiArrayConvert(1, 1, AI_TYPE_NODE, shader));
   }

   // Export light linking per instance
   ExportLightLinking(instance);

   return instance;
}

void CGeometryTranslator::Update(AtNode *anode)
{
   if (m_isMasterDag)
   {
      ExportMesh(anode, true);
   }
   else
   {
      ExportInstance(anode, m_masterDag);
   }
}

void CGeometryTranslator::ExportMotion(AtNode* anode, unsigned int step)
{
   if (m_isMasterDag)
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
         // Interrupt render before exporting shaders
         CMayaScene::GetRenderSession()->InterruptRender();
         // Export the new shaders.
         translator->ExportShaders();
         // Update Arnold without passing a translator, this just forces a redraw.
         translator->RequestUpdate();
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
   helper.MakeInput("subdiv_pixel_error");
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
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiExportColors";
   data.shortName = "ai_expcol";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL = true;
   data.name = "aiExportRefPoints";
   data.shortName = "ai_exprpt";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiExportRefNormals";
   data.shortName = "ai_exprnrm";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiExportRefTangents";
   data.shortName = "ai_exprtan";
   helper.MakeInputBoolean(data);

}
