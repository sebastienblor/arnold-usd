
#include "GeometryTranslator.h"

#include <maya/MNodeMessage.h>

#include <algorithm>

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

bool CGeometryTranslator::GetVertices(MFnMesh &fnMesh,
                                      std::vector<float> &vertices,
                                      MSpace::Space space)
{
   int nverts = fnMesh.numVertices();
   if (nverts > 0)
   {
      vertices.resize(nverts * 3);

      MFloatPointArray pointsArray;
      if (space == MSpace::kWorld)
      {
         // A mesh has no transform, we must trace it back in the DAG
         MDagPath dp;
         fnMesh.getPath(dp);
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

bool CGeometryTranslator::GetPerVertexNormals(MFnMesh &fnMesh,
                                              std::vector<float> &normals,
                                              MSpace::Space space,
                                              bool force)
{
   int nnorms = fnMesh.numNormals();
   if (nnorms > 0 && (force || (FindMayaObjectPlug("smoothShading").asBool() && !FindMayaObjectPlug("aiSubdivType").asBool())))
   {
      normals.resize(fnMesh.numVertices() * 3);

      MFloatVectorArray normalArray;
      if (space == MSpace::kWorld)
      {
         // A mesh has no transform, we must trace it back in the DAG
         MDagPath dp;
         fnMesh.getPath(dp);
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

bool CGeometryTranslator::GetNormals(MFnMesh &fnMesh, std::vector<float> &normals)
{
   int nnorms = fnMesh.numNormals();
   if (FindMayaObjectPlug("smoothShading").asBool() &&
         !FindMayaObjectPlug("aiSubdivType").asBool() &&
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

bool CGeometryTranslator::GetTangents(MFnMesh &fnMesh,
                                      std::vector<float> &tangents,
                                      std::vector<float> &bitangents,
                                      MSpace::Space space,
                                      bool force)
{
   bool doExport;
   if (force)
   {
      doExport = true;
   }
   else
   {
      MStatus stat;
      MPlug pExportTangents = fnMesh.findPlug("aiExportTangents", false, &stat);
      doExport = (stat == MStatus::kSuccess && pExportTangents.asBool());
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
      fnMesh.getPath(dp);
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

bool CGeometryTranslator::GetMeshRefObj(MFnMesh &fnMesh)
{
   if (!m_isRefSmooth)
   {
      MStatus stat;
      MObject referenceObj;
      // Check if there is a referenceObject plug
      MPlug pReferenceObject = fnMesh.findPlug("referenceObject", false, &stat);
      if (stat != MStatus::kSuccess)
         return false;
      MPlugArray connectedPlugs;
      // Check if anything is connected to .referenceObject plug
      pReferenceObject.connectedTo(connectedPlugs, true, true, &stat);
      if (stat != MStatus::kSuccess || 1 != connectedPlugs.length())
      {
         return false;
      }
      else
      {
         // Checking if a script is not going rogue : the object must be a mesh !
         referenceObj = connectedPlugs[0].node();
         if (referenceObj.hasFn(MFn::kMesh) != 1)
            return false;
      }

      m_fnMeshRef.setObject(referenceObj);

      // Check if the numbers of vertices is the same as the current object
      if (m_fnMeshRef.numVertices() != fnMesh.numVertices())
      {
         return false;
      }

      return true;
   }
   else
   {
      return true;
   }
}

bool CGeometryTranslator::GetRefObj(MFnMesh &fnMesh,
                                    std::vector<float> &refVertices,
                                    std::vector<float> &refNormals,
                                    std::vector<float> &refTangents,
                                    std::vector<float> &refBitangents)
{
   bool getMesh = GetMeshRefObj(fnMesh);

   if (getMesh)
   {
      // Find whether we're exporting points/normals/tangents
      MStatus stat;
      MPlug pExportRefPoints = fnMesh.findPlug("aiExportRefPoints", false,
                                               &stat);
      if (stat == MStatus::kSuccess && pExportRefPoints.asBool())
      {
         // Get vertices of the reference object in world space
         GetVertices(m_fnMeshRef, refVertices, MSpace::kWorld);
      }

      MPlug pExportRefNormals = fnMesh.findPlug("aiExportRefNormals", false,
                                                &stat);
      if (stat == MStatus::kSuccess && pExportRefNormals.asBool())
      {
         // Get normals of the reference object (as we are outputing this as a user-data varying data,
         // we must have 1 normal per vertex
         // Also, even if subdivision is applied we want to get the normals data
         GetPerVertexNormals(m_fnMeshRef, refNormals, MSpace::kWorld, true);
      }

      MPlug pExportRefTangents = fnMesh.findPlug("aiExportRefTangents", false,
                                                 &stat);
      if (stat == MStatus::kSuccess && pExportRefTangents.asBool())
      {
         // Get tangents of the reference object in world space
         // Also, even if subdivision is applied we want to get the tangent data
         GetTangents(m_fnMeshRef, refTangents, refBitangents, MSpace::kWorld,
                     true);
      }
      // If we are using a smoothed reference object, we need to delete it.
      //FIXME : This is somehow dirty, but I can't find a way to have a real "virtual" DAG object from generateSmoothMesh.
      if (m_isRefSmooth)
      {
         MDagPath dp;
         m_fnMeshRef.getPath(dp);
         MObject node = dp.node();
         MGlobal::removeFromModel(node);
      }
      return true;
   }
   else
      return false;
}

bool CGeometryTranslator::GetUVs(MFnMesh &fnMesh, std::vector<float> &uvs)
{
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

bool CGeometryTranslator::GetVertexColors(MFnMesh &fnMesh, std::map<std::string, std::vector<float> > &vcolors)
{
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

void CGeometryTranslator::GetComponentIDs(MFnMesh &fnMesh,
      std::vector<unsigned int> &nsides,
      std::vector<AtUInt32> &vidxs,
      std::vector<AtUInt32> &nidxs,
      std::vector<AtUInt32> &uvidxs,
      bool exportNormals,
      bool exportUVs)
{
   int uv_id = 0;
   // Traverse all polygons to export vidxs, uvindxs and nsides
   nsides.reserve(fnMesh.numPolygons());
   for (int p(0); p < fnMesh.numPolygons(); ++p)
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
            m_fnMesh.getPolygonUVid(p, v, uv_id);
            uvidxs.push_back(uv_id);
         }
      }
   }

   // Normals.
   if (exportNormals)
   {
      MIntArray vertex_counts, normal_ids;
      m_fnMesh.getNormalIds(vertex_counts, normal_ids);
      for(uint n(0); n < normal_ids.length(); ++n) nidxs.push_back(normal_ids[n]);
   }
}

void CGeometryTranslator::ExportShaders()
{
   ExportMeshShaders(GetArnoldRootNode(), m_fnMesh);
}



/// Nodes to be written to AOVs are connected to a special attribute on the shading group called aiCustomAOVs.
/// The simplest solution to exporting these custom AOVs would be to branch them in at the root of the network.
/// However, because Arnold lacks output caching, and considering that the nodes connected to aiCustomAOVs may
/// appear elsewhere in the shape's shading network, we must take great pains to build linear node networks in
/// order to avoid entire shading networks from being evaluated multiple times during render (i.e., we must avoid a
/// node's output being connected to more than one node). So instead of a simple branching design, we must insert the
/// AOV write nodes within the body of the network, immediately following the node whose output needs to be written.
/// Those AOVs are handled by CShaderTranslator::ProcessAOVOutput, while the remainder are processed in
/// CGeometryTranslator::ExportMeshShaders
void CGeometryTranslator::ExportMeshShaders(AtNode* polymesh, MFnMesh &fnMesh)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MObject shadingGroup = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroup.isNull())
   {
      AtNode *shader = ExportRootShader(shadingGroup);
      if (shader != NULL)
      {
         AiNodeSetPtr(polymesh, "shader", shader);
      }
      else
      {
         AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
               GetTranslatorName().asChar(), MFnDependencyNode(shadingGroup).name().asChar());
         AiNodeSetPtr(polymesh, "shader", NULL);
      }
   }
   else
   {
      MIntArray indices;
      // Per-face assignment
      MObjectArray shadingGroups;

      // Indices are used later when exporting shidxs
      fnMesh.getConnectedShaders(instanceNum, shadingGroups, indices);

      for (int J = 0; (J < (int) shadingGroups.length()); J++)
      {
         MPlugArray        connections;
         MFnDependencyNode fnDGNode(shadingGroups[J]);
         MPlug             shaderPlug(shadingGroups[J], fnDGNode.attribute("surfaceShader"));

         shaderPlug.connectedTo(connections, true, false);
         
         if (connections.length() > 0)
         {
            meshShaders.push_back(ExportRootShader(connections[0]));
         }
         else
         {
            AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
               GetTranslatorName().asChar(), fnDGNode.name().asChar());
            meshShaders.push_back(NULL);
         }
      }
      
      AiNodeSetArray(polymesh, "shader", AiArrayConvert((int)meshShaders.size(), 1, AI_TYPE_NODE, &meshShaders[0]));

      // Export face to shader indices
      // First convert from MIntArray to unsigned int vector
      
      int divisions = 0;
      // int facesOffset = 0;
      int multiplier = 0;
      
      if (m_fnMesh.findPlug("displaySmoothMesh").asBool())
      {
         MMeshSmoothOptions options;
         MStatus status = m_fnMesh.getSmoothMeshDisplayOptions(options);
         
         CHECK_MSTATUS(status);
         
         divisions = options.divisions();
         if(divisions > 0)
            multiplier = static_cast<int> (pow(4.0f, (divisions-1)));
      }
      
      std::vector<unsigned int> shidxs;
      for (unsigned int i = 0; i < indices.length(); i++)
      {
         int subdivisions = multiplier * m_fnMesh.polygonVertexCount(i);
         shidxs.push_back(indices[i]);
         for (int j = 0; j < subdivisions -1; j++)
            shidxs.push_back(indices[i]);
      }
      AiNodeSetArray(polymesh, "shidxs", AiArrayConvert((int)shidxs.size(), 1, AI_TYPE_UINT, &(shidxs[0])));
   }

   //
   // DISPLACEMENT
   //
   // Currently Arnold does not support displacement per-face assignment
   if (!shadingGroup.isNull())
   {
      MPlugArray        connections;
      MFnDependencyNode fnDGNode(shadingGroup);
      MPlug shaderPlug = fnDGNode.findPlug("displacementShader");
      shaderPlug.connectedTo(connections, true, false);

      m_displaced = false;

      // are there any connections to displacementShader?
      if (connections.length() > 0)
      {
         m_displaced = true;
         MFnDependencyNode dispNode(connections[0].node());

         // Note that disp_height has no actual influence on the scale of the displacement if it is vector based
         // it only influences the computation of the displacement bounds
         AiNodeSetFlt(polymesh, "disp_height", dispNode.findPlug("disp_height").asFloat());
         AiNodeSetFlt(polymesh, "disp_padding", dispNode.findPlug("disp_padding").asFloat());
         AiNodeSetFlt(polymesh, "disp_zero_value", dispNode.findPlug("disp_zero_value").asFloat());
         AiNodeSetBool(polymesh, "disp_autobump", dispNode.findPlug("disp_autobump").asBool());

         connections.clear();
         dispNode.findPlug("disp_map").connectedTo(connections, true, false);

         if (connections.length() > 0)
         {
            MString attrName = connections[0].partialName(false, false, false, false, false, true);
            AtNode* dispImage(ExportNode(connections[0]));

            // FIXME : why request a non networked plug?
            MPlug pVectorDisp = dispNode.findPlug("vector_displacement", false);
            if (!pVectorDisp.isNull() && pVectorDisp.asBool())
            {
               AtNode* tangentToObject = AiNode("TangentToObjectSpace");
               MPlug pVectorDispScale = dispNode.findPlug("vector_displacement_scale", false);
               // FIXME : do this using a translator instead
               ProcessParameter(tangentToObject, "scale", AI_TYPE_VECTOR, pVectorDispScale);
               AiNodeLink(dispImage, "map", tangentToObject);

               AiNodeSetPtr(polymesh, "disp_map", tangentToObject);
            }
            else
            {
               AiNodeSetPtr(polymesh, "disp_map", dispImage);
            }
         }
      }
   }
}

void CGeometryTranslator::ExportMeshGeoData(AtNode* polymesh, unsigned int step)
{
   m_isRefSmooth = false;

   // Check if the object is smoothed with maya method
   if (m_fnMesh.findPlug("displaySmoothMesh").asBool())
   {
      MMeshSmoothOptions options;
      MStatus status = m_fnMesh.getSmoothMeshDisplayOptions(options);

      CHECK_MSTATUS(status);

      if(!m_fnMesh.findPlug("useSmoothPreviewForRender", false, &status).asBool())
         options.setDivisions(m_fnMesh.findPlug("renderSmoothLevel", false, &status).asInt());

      if (options.divisions() > 0)
      {
         // Check if mesh got a reference object. If so, we must also smooth it and reconnect it
         //FIXME : This has to be done in a better way, but how ?
         bool getMesh = GetMeshRefObj(m_fnMesh);

         if (getMesh)
         {
            m_isRefSmooth = true;
            MDagPath dp;
            m_fnMeshRef.getPath(dp);
            MDagPath transform(dp);
            transform.pop();
            MObject mesh_mobj_ref = m_fnMeshRef.generateSmoothMesh(transform.node(), &options,
                  &status);
            CHECK_MSTATUS(status);
            m_fnMeshRef.setObject(mesh_mobj_ref);
         }

         MFnMeshData meshData;
         // This is a member variable. We have to keep hold of it or Maya will release it.
         m_data_mobj = meshData.create();
         MObject mesh_mobj = m_fnMesh.generateSmoothMesh(m_data_mobj, &options, &status);
         CHECK_MSTATUS(status);
         m_fnMesh.setObject(mesh_mobj);
      }
   }
   //
   // GEOMETRY
   //
   std::vector<float> vertices, normals, tangents, bitangents;

   // Get all vertices
   GetVertices(m_fnMesh, vertices, MSpace::kObject);

   // Get all normals
   bool exportNormals = GetNormals(m_fnMesh, normals);

   // Get all tangents, bitangents
   bool exportTangents = GetTangents(m_fnMesh, tangents, bitangents,
                                     MSpace::kObject);

   if (step == 0)
   {
      std::vector<float> uvs;
      std::vector<unsigned int> nsides;
      std::vector<AtUInt32> vidxs, nidxs, uvidxs;
      std::map<std::string, std::vector<float> > vcolors;
      std::vector<float> refVertices, refNormals, refTangents, refBitangents;

      // Get UVs
      bool exportUVs = GetUVs(m_fnMesh, uvs);

      // Get reference objects
      bool exportReferenceObjects = GetRefObj(m_fnMesh, refVertices, refNormals,
                                              refTangents, refBitangents);
      bool exportRefVerts = (refVertices.size() > 0);
      bool exportRefNorms = (refNormals.size() > 0);
      bool exportRefTangents = (refTangents.size() > 0 && refBitangents.size() > 0);

      // Get Component IDs
      GetComponentIDs(m_fnMesh, nsides, vidxs, nidxs, uvidxs, exportNormals, exportUVs);
      // Get Vertex Colors
      bool exportColors = GetVertexColors(m_fnMesh, vcolors);

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
         AiNodeSetArray(polymesh, "vlist", AiArrayConvert(m_fnMesh.numVertices() * 3, 1, AI_TYPE_FLOAT, &(vertices[0])));

         if (exportNormals && (m_fnMesh.numNormals() > 0))
            AiNodeSetArray(polymesh, "nlist", AiArrayConvert(m_fnMesh.numNormals() * 3, 1, AI_TYPE_FLOAT, &(normals[0])));

         if (exportTangents)
         {
            AiNodeSetArray(polymesh, "tangent", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(tangents[0])));
            AiNodeSetArray(polymesh, "bitangent", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(bitangents[0])));
         }
      }
      else
      {
         // Deformation motion blur. We need to create keyable arrays for vlist and nlist
         AtArray* vlist_array = AiArrayAllocate(m_fnMesh.numVertices(), GetNumMotionSteps(), AI_TYPE_POINT);
         SetKeyData(vlist_array, step, vertices, m_fnMesh.numVertices());
         AiNodeSetArray(polymesh, "vlist", vlist_array);

         if (exportNormals)
         {
            AtArray* nlist_array = AiArrayAllocate(m_fnMesh.numNormals(), GetNumMotionSteps(), AI_TYPE_VECTOR);
            SetKeyData(nlist_array, step, normals, m_fnMesh.numNormals());
            AiNodeSetArray(polymesh, "nlist", nlist_array);
         }

         if (exportTangents)
         {
            AtArray* tangent_array = AiArrayAllocate(m_fnMesh.numVertices(), GetNumMotionSteps(), AI_TYPE_VECTOR);
            SetKeyData(tangent_array, step, tangents, m_fnMesh.numVertices());
            AiNodeSetArray(polymesh, "tangent", tangent_array);

            AtArray* bitangent_array = AiArrayAllocate(m_fnMesh.numVertices(), GetNumMotionSteps(), AI_TYPE_VECTOR);
            SetKeyData(bitangent_array, step, bitangents, m_fnMesh.numVertices());
            AiNodeSetArray(polymesh, "bitangent", bitangent_array);
         }
      }

      AiNodeSetArray(polymesh, "nsides", AiArrayConvert((int)nsides.size(), 1, AI_TYPE_UINT, &(nsides[0])));

      // Passing vidxs directly put Arnold in trouble
      //AiNodeSetArray(polymesh, "vidxs", AiArrayConvert(vidxs.size(), 1, AI_TYPE_UINT, &(vidxs[0])));
      AtArray *vidxsTmp = AiArrayAllocate((int)vidxs.size(), 1, AI_TYPE_UINT);
      for(unsigned int i = 0; (i < vidxs.size()); i++)
         AiArraySetUInt(vidxsTmp, i, vidxs[i]);
      AiNodeSetArray(polymesh, "vidxs", vidxsTmp);

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
            AiNodeSetArray(polymesh, "Pref", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_POINT, &(refVertices[0])));
         }
         if (exportRefNorms)
         {
            AiNodeSetArray(polymesh, "Nref", AiArrayConvert(m_fnMesh.numNormals(), 1, AI_TYPE_VECTOR, &(refNormals[0])));
         }
         if (exportRefTangents)
         {
            AiNodeSetArray(polymesh, "Tref", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(refTangents[0])));
            AiNodeSetArray(polymesh, "BTref", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(refBitangents[0])));
         }
      }

      if (exportUVs)
      {
         AiNodeSetArray(polymesh, "uvlist", AiArrayConvert(m_fnMesh.numUVs() * 2, 1, AI_TYPE_FLOAT, &(uvs[0])));
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
               AiNodeSetArray(polymesh, it->first.c_str(), AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_RGBA, &(it->second[0])));
            }
            else
            {
               int m_colorId;
               float m_count = 0.0f;
               MColorArray colors;
               MString m_colorSetName = "sss_faceset";
               MColor m_defaultColor  = MColor(0.0f, 0.0f, 0.0f);

               AtArray *m_sss_faceset_bool = AiArray(m_fnMesh.numPolygons(), 1, AI_TYPE_BOOLEAN, NULL);

               m_fnMesh.getFaceVertexColors(colors, &m_colorSetName, &m_defaultColor);

               for (int m_polygonId = 0; (m_polygonId < (int)m_fnMesh.numPolygons()); m_polygonId++)
               {
                  MIntArray m_vertexList;
                  m_fnMesh.getPolygonVertices(m_polygonId, m_vertexList);

                  m_count = 0.0f;
                  for (int m_vertexId = 0; (m_vertexId < (int)m_vertexList.length()); m_vertexId++)
                  {
                     m_fnMesh.getFaceVertexColorIndex(m_polygonId, m_vertexId, m_colorId, &m_colorSetName);
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
      AtArray* vlist_array = AiNodeGetArray(polymesh, "vlist");
      SetKeyData(vlist_array, step, vertices, m_fnMesh.numVertices());

      // Normals
      if (exportNormals)
      {
         AtArray* nlist_array = AiNodeGetArray(polymesh, "nlist");
         SetKeyData(nlist_array, step, normals, m_fnMesh.numNormals());
      }

      // Tangents
      if (exportTangents)
      {
         AtArray* tangent_array = AiNodeGetArray(polymesh, "tangent");
         SetKeyData(tangent_array, step, tangents, m_fnMesh.numVertices());

         AtArray* bitangent_array = AiNodeGetArray(polymesh, "bitangent");
         SetKeyData(bitangent_array, step, bitangents, m_fnMesh.numVertices());
      }
   }
}

bool CGeometryTranslator::IsGeoDeforming()
{
    bool history = false;
    bool pnts = false;

    MPlug inMeshPlug = m_fnMesh.findPlug("inMesh");
    MPlugArray conn;
    inMeshPlug.connectedTo(conn, true, false);
    if (conn.length())
    {
        history = true;
    }
 
    inMeshPlug = m_fnMesh.findPlug("pnts");
    inMeshPlug.connectedTo(conn, true, false);
    if (conn.length())
    {
        pnts = true;
    }

    if (!history && !pnts && !m_displaced)
        return false;
    return true;
}

void CGeometryTranslator::ExportMeshParameters(AtNode* polymesh)
{
   // Check if custom attributes have been created, ignore them otherwise
   if (FindMayaObjectPlug("aiSubdivType").isNull()) return;

   AiNodeSetBool(polymesh, "smoothing", FindMayaObjectPlug("smoothShading").asBool());

   if (FindMayaObjectPlug("doubleSided").asBool())
      AiNodeSetInt(polymesh, "sidedness", 65535);
   else
   {
      AiNodeSetBool(polymesh, "invert_normals", FindMayaObjectPlug("opposite").asBool());
      AiNodeSetInt(polymesh, "sidedness", 0);
   }

   // Visibility options
   ProcessRenderFlags(polymesh);

   // Subdivision surfaces
   //
   const int subdivision = FindMayaObjectPlug("aiSubdivType").asInt();
   if (subdivision!=0)
   {
      if (subdivision==1)
         AiNodeSetStr(polymesh, "subdiv_type",           "catclark");
      else
         AiNodeSetStr(polymesh, "subdiv_type",           "linear");
      AiNodeSetInt(polymesh, "subdiv_iterations",     FindMayaObjectPlug("aiSubdivIterations").asInt());
      AiNodeSetInt(polymesh, "subdiv_adaptive_metric",FindMayaObjectPlug("aiSubdivAdaptiveMetric").asInt());
      AiNodeSetFlt(polymesh, "subdiv_pixel_error",    FindMayaObjectPlug("aiSubdivPixelError").asFloat());
      AiNodeSetInt(polymesh, "subdiv_uv_smoothing",   FindMayaObjectPlug("aiSubdivUvSmoothing").asInt());
      AiNodeSetBool(polymesh, "subdiv_smooth_derivs", FindMayaObjectPlug("aiSubdivSmoothDerivs").asBool());

      ProcessParameter(polymesh, "subdiv_dicing_camera", AI_TYPE_NODE, FindMayaObjectPlug("aiSubdivDicingCamera"));

   }
}

AtNode* CGeometryTranslator::ExportMesh(AtNode* polymesh, bool update)
{
   ExportMatrix(polymesh, 0);
   ExportMeshParameters(polymesh);
   ExportMeshShaders(polymesh, m_fnMesh);
   ExportLightLinking(polymesh);
   // if enabled, double check motion deform
   if (m_motionDeform)
      m_motionDeform = IsGeoDeforming();

   IsGeoDeforming();
   if (!update)
      ExportMeshGeoData(polymesh, 0);
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
   MFnMesh           meshNode(m_dagPath.node());
   MObjectArray      shaders, shadersMaster;
   MIntArray         indices, indicesMaster;

   meshNode.getConnectedShaders(instanceNum, shaders, indices);
   // FIXME: it is incorrect to assume that instance 0 is the master as it may be hidden (chad)
   meshNode.getConnectedShaders(0, shadersMaster, indicesMaster);

   // As arnold does not support different shaders per face
   // on ginstances
   // we keep the master's per face assignment only
   // if it's completely the same
   bool equalShaderArrays = ((shaders.length() == shadersMaster.length()) && (indices.length() == indicesMaster.length()));

   // Compare face arrays
   for(unsigned int j=0; (equalShaderArrays && (j < indices.length())); j++)
   {
      if (indices[j] != indicesMaster[j])
      {
         equalShaderArrays = false;
      }
   }
   // Compare shader (actually SG) arrays
   for(unsigned int i=0; (equalShaderArrays && (i < shaders.length())); i++)
   {
      if (shaders[i] != shadersMaster[i])
      {
         equalShaderArrays = false;
      }
   }

   if ((shaders.length() > 0) && (shadersMaster.length() > 0))
   {
      MPlugArray        connections;
      MFnDependencyNode fnDGNode(shaders[0]);
      MPlug             shaderPlug(shaders[0], fnDGNode.attribute("surfaceShader"));
      MPlug             shaderPlugMaster(shadersMaster[0], fnDGNode.attribute("surfaceShader"));

      shaderPlug.connectedTo(connections, true, false);

      if ((shaderPlug != shaderPlugMaster) || (!equalShaderArrays))
      {
         AtNode* shader = ExportNode(connections[0]);
         AiNodeSetPtr(instance, "shader", shader);
      }
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
   // Ran into a strange bug where the object must be reset to
   // avoid a crash.  even calling .hasObj() was enough to avoid it
   m_fnMesh.setObject(m_object);
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
   AddShaderAssignmentCallbacks(m_object);
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
         // FIXME: this is not working. Interrupting render wasn't working either in 0.8
         // so removed it.
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
