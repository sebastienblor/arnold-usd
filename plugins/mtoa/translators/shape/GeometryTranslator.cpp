
#include "GeometryTranslator.h"

#include <maya/MNodeMessage.h>

namespace
{
   void SetKeyData(AtArray* arr, AtUInt step, const std::vector<float>& data, AtUInt size)
   {
      AtUInt index = 0;

      switch(arr->type)
      {
         case AI_TYPE_POINT:
         {
            AtPoint pnt;
            for(AtUInt J = 0; (J < size); ++J)
            {
               AiV3Create(pnt, data[index++], data[index++], data[index++]);
               AiArraySetPnt(arr, J + (size * step), pnt);
            }
         }
         break;

         case AI_TYPE_VECTOR:
         {
            AtVector vec;
            for(AtUInt J = 0; (J < size); ++J)
            {
               AiV3Create(vec, data[index++], data[index++], data[index++]);
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

bool CGeometryTranslator::GetVerticesWorld(MFnMesh &fnMesh, std::vector<float> &vertices)
{
   if (fnMesh.numVertices() > 0)
   {
      vertices.resize(fnMesh.numVertices() * 3);

      // A mesh has no transform, we must trace it back in the DAG
      MDagPath dp;
      fnMesh.getPath(dp);
      MFnMesh pointFn(dp);
      MFloatPointArray pointsArray;
      pointFn.getPoints(pointsArray, MSpace::kWorld);
      for (int J = 0; (J < fnMesh.numVertices()); ++J)
      {
         vertices[J * 3 + 0] = pointsArray[J].x;
         vertices[J * 3 + 1] = pointsArray[J].y;
         vertices[J * 3 + 2] = pointsArray[J].z;
      }
      return true;
   }
   return false;
}

bool CGeometryTranslator::GetVertices(MFnMesh &fnMesh, std::vector<float> &vertices)
{
   int nverts = fnMesh.numVertices();
   if (nverts > 0)
   {
      vertices.resize(nverts * 3);

      MFloatPointArray pointsArray;
      fnMesh.getPoints(pointsArray, MSpace::kObject);

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

bool CGeometryTranslator::GetTangents(MFnMesh &fnMesh, std::vector<float> &tangents, std::vector<float> &bitangents)
{
   MStatus stat;
   MPlug pExportTangents = fnMesh.findPlug("aiExportTangents", false, &stat);

   if (stat != MStatus::kSuccess)
      return false;

   if (pExportTangents.asBool())
   {
      MObject meshObject = fnMesh.object();
      MItMeshVertex itVertex(meshObject);
      MIntArray iarray;
      MVector ttmp, btmp, tangent, bitangent;
      float scale = 1.0f;
      unsigned int i = 0;

      int nverts = fnMesh.numVertices();

      tangents.resize(nverts * 3);
      bitangents.resize(nverts * 3);

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
               fnMesh.getFaceVertexTangent(iarray[j], itVertex.index(), ttmp, MSpace::kObject);
               fnMesh.getFaceVertexBinormal(iarray[j], itVertex.index(), btmp, MSpace::kObject);
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
   return false;
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

bool CGeometryTranslator::GetRefObj(MFnMesh &fnMesh, std::vector<float> &refVertices, std::vector<float> &refNormals)
{
   bool getMesh = GetMeshRefObj(fnMesh);

   if (getMesh)
   {
      // Get vertices of the reference object in world space
      GetVerticesWorld(m_fnMeshRef, refVertices);
      // Get normals of the reference object
      GetNormals(m_fnMeshRef, refNormals);

      // If we are using a smoothed reference object, we need to delete it.
      //FIXME : This is somehow dirty, but I can't find a way to have a real "virtual" DAG object from generateSmoothMesh.
      if (m_isRefSmooth)
      {
         MDagPath dp;
         m_fnMeshRef.getPath(dp);
         MDagPath transform(dp);
         MObject node = dp.node();
         MGlobal::deleteNode(node);
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
      std::vector<AtUInt> &nsides,
      std::vector<AtLong> &vidxs,
      std::vector<AtLong> &nidxs,
      std::vector<AtLong> &uvidxs,
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

void CGeometryTranslator::ExportMeshShaders(AtNode* polymesh, MFnMesh &fnMesh)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MObject shadingGroup = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroup.isNull())
   {
      MPlugArray        connections;
      MFnDependencyNode fnDGNode(shadingGroup);
      MPlug shaderPlug = fnDGNode.findPlug("surfaceShader");
      shaderPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         // shader assigned to node
         AtNode* shader = ExportNode(connections[0]);

         AiNodeSetPtr(polymesh, "shader", shader);
         meshShaders.push_back(shader);
      }
      else
         AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
               GetTranslatorName().asChar(), fnDGNode.name().asChar());
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
         // FIXME: there should be a check if connections.length() > 0
         // this is not a simple fix because it will shift all the indices,
         // but as it is now, it will crash if nothing is connected to "surfaceShader"
         meshShaders.push_back(ExportNode(connections[0]));
      }

      AiNodeSetArray(polymesh, "shader", AiArrayConvert((AtInt)meshShaders.size(), 1, AI_TYPE_NODE, &meshShaders[0], TRUE));

      // Export face to shader indices
      // First convert from MIntArray to AtUInt vector
      std::vector<AtUInt> shidxs;
      for(AtUInt i = 0; i < indices.length(); i++)
         shidxs.push_back(indices[i]);
      AiNodeSetArray(polymesh, "shidxs", AiArrayConvert((AtInt)shidxs.size(), 1, AI_TYPE_UINT, &(shidxs[0]), TRUE));
   }

   //
   // DISPLACEMENT
   //
   // Currently does not work for per-face assignment
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

void CGeometryTranslator::ExportMeshGeoData(AtNode* polymesh, AtUInt step)
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
   GetVertices(m_fnMesh, vertices);

   // Get all normals
   bool exportNormals = GetNormals(m_fnMesh, normals);

   // Get all tangents, bitangents
   bool exportTangents = GetTangents(m_fnMesh, tangents, bitangents);

   if (step == 0)
   {
      std::vector<float> uvs;
      std::vector<AtUInt> nsides;
      std::vector<AtLong> vidxs, nidxs, uvidxs;
      std::map<std::string, std::vector<float> > vcolors;
      std::vector<float> refVertices, refNormals;

      // Get UVs
      bool exportUVs = GetUVs(m_fnMesh, uvs);

      // Get reference objects
      bool exportReferenceObjects = GetRefObj(m_fnMesh, refVertices, refNormals);

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
          AiNodeDeclare(polymesh, "Pref", "varying POINT");
          AiNodeDeclare(polymesh, "Nref", "varying VECTOR");
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
         AiNodeSetArray(polymesh, "vlist", AiArrayConvert(m_fnMesh.numVertices() * 3, 1, AI_TYPE_FLOAT, &(vertices[0]), TRUE));

         if (exportNormals && (m_fnMesh.numNormals() > 0))
            AiNodeSetArray(polymesh, "nlist", AiArrayConvert(m_fnMesh.numNormals() * 3, 1, AI_TYPE_FLOAT, &(normals[0]), TRUE));

         if (exportTangents)
         {
            AiNodeSetArray(polymesh, "tangent", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(tangents[0]), TRUE));
            AiNodeSetArray(polymesh, "bitangent", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(bitangents[0]), TRUE));
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

      AiNodeSetArray(polymesh, "nsides", AiArrayConvert((AtInt)nsides.size(), 1, AI_TYPE_UINT, &(nsides[0]), TRUE));

      // Passing vidxs directly put Arnold in trouble
      //AiNodeSetArray(polymesh, "vidxs", AiArrayConvert(vidxs.size(), 1, AI_TYPE_UINT, &(vidxs[0]), TRUE));
      AtArray *vidxsTmp = AiArrayAllocate((AtInt)vidxs.size(), 1, AI_TYPE_UINT);
      for(AtUInt i = 0; (i < vidxs.size()); i++)
         AiArraySetUInt(vidxsTmp, i, vidxs[i]);
      AiNodeSetArray(polymesh, "vidxs", vidxsTmp);

      if (exportNormals)
      {
         // Same goes here
         //AiNodeSetArray(polymesh, "nidxs", AiArrayConvert(nidxs.size(), 1, AI_TYPE_UINT, &(nidxs[0]), TRUE));
         AtArray *nidxsTmp = AiArrayAllocate((AtInt)nidxs.size(), 1, AI_TYPE_UINT);
         for(AtUInt i = 0; (i < nidxs.size()); i++)
            AiArraySetUInt(nidxsTmp, i, nidxs[i]);
         AiNodeSetArray(polymesh, "nidxs", nidxsTmp);
      }

      if (exportReferenceObjects)
      {
           AiNodeSetArray(polymesh, "Pref", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_POINT, &(refVertices[0]), TRUE));
         AiNodeSetArray(polymesh, "Nref", AiArrayConvert(m_fnMesh.numNormals(), 1, AI_TYPE_VECTOR, &(refNormals[0]), TRUE));

      }

      if (exportUVs)
      {
         AiNodeSetArray(polymesh, "uvlist", AiArrayConvert(m_fnMesh.numUVs() * 2, 1, AI_TYPE_FLOAT, &(uvs[0]), TRUE));
         // Same problem here
         //AiNodeSetArray(polymesh, "uvidxs", AiArrayConvert(uvidxs.size(), 1, AI_TYPE_UINT, &(uvidxs[0]), TRUE));
         AtArray *uvidxsTmp = AiArrayAllocate((AtInt)uvidxs.size(), 1, AI_TYPE_UINT);
         for(AtUInt i = 0; (i < uvidxs.size()); i++)
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
               AiNodeSetArray(polymesh, it->first.c_str(), AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_RGBA, &(it->second[0]), TRUE));
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
   for(AtUInt j=0; (equalShaderArrays && (j < indices.length())); j++)
   {
      if (indices[j] != indicesMaster[j])
      {
         equalShaderArrays = false;
      }
   }
   // Compare shader (actually SG) arrays
   for(AtUInt i=0; (equalShaderArrays && (i < shaders.length())); i++)
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

void CGeometryTranslator::ExportMotion(AtNode* anode, AtUInt step)
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

void CGeometryTranslator::UpdateMotion(AtNode* anode, AtUInt step)
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

   CAttrData data;

   data.defaultValue.BOOL = false;
   data.name = "aiExportTangents";
   data.shortName = "ai_exptan";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiExportColors";
   data.shortName = "ai_expcol";
   helper.MakeInputBoolean(data);
}
