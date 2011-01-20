
#include "Geometry.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MDagPath.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MIntArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MItMeshVertex.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>

#include <vector>

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

MObject CGeoTranslator::GetNodeShadingGroup(MObject dagNode, int instanceNum)
{
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(instanceNum).connectedTo(connections, false, true);

   for (unsigned int k=0; k<connections.length(); ++k)
   {
      MObject shadingGroup(connections[k].node());
      if (shadingGroup.apiType() == MFn::kShadingEngine )
      {
         return shadingGroup;
      }
   }
   return MObject::kNullObj;
}

MObject CGeoTranslator::GetNodeShader(MObject dagNode, int instanceNum)
{
   MPlugArray        connections;
   MObject shadingGroup = GetNodeShadingGroup(dagNode, instanceNum);
   MFnDependencyNode fnDGNode(shadingGroup);
   MPlug shaderPlug = fnDGNode.findPlug("surfaceShader");
   shaderPlug.connectedTo(connections, true, false);

   return connections[0].node();
}

bool CGeoTranslator::GetVertices(MFnMesh &fnMesh, std::vector<float> &vertices)
{
   if (fnMesh.numVertices() > 0)
   {
      vertices.resize(fnMesh.numVertices() * 3);

      MFloatPointArray pointsArray;
      fnMesh.getPoints(pointsArray, MSpace::kObject);

      for (int J = 0; ( J < fnMesh.numVertices() ); ++J)
      {
         vertices[J * 3 + 0] = pointsArray[J].x;
         vertices[J * 3 + 1] = pointsArray[J].y;
         vertices[J * 3 + 2] = pointsArray[J].z;
      }
      return true;
   }
   return false;
}

bool CGeoTranslator::GetNormals(MFnMesh &fnMesh, std::vector<float> &normals)
{
   if (m_fnNode.findPlug("smoothShading").asBool() &&
         !m_fnNode.findPlug("subdiv_type").asBool() &&
         fnMesh.numNormals() > 0)
   {
      normals.resize(fnMesh.numNormals() * 3);

      MFloatVectorArray normalArray;
      fnMesh.getNormals(normalArray, MSpace::kObject);

      for (int J = 0; (J < fnMesh.numNormals()); ++J)
      {
         normals[J * 3 + 0] = normalArray[J].x;
         normals[J * 3 + 1] = normalArray[J].y;
         normals[J * 3 + 2] = normalArray[J].z;
      }
      return true;
   }
   return false;
}

bool CGeoTranslator::GetTangents(MFnMesh &fnMesh, std::vector<float> &tangents, std::vector<float> &bitangents)
{
   MStatus stat;
   MPlug pExportTangents = fnMesh.findPlug("export_tangents", false, &stat);

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

      tangents.resize(fnMesh.numVertices() * 3);
      bitangents.resize(fnMesh.numVertices() * 3);

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

bool CGeoTranslator::GetUVs(MFnMesh &fnMesh, std::vector<float> &uvs)
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

bool CGeoTranslator::GetVertexColors(MFnMesh &fnMesh, std::map<std::string, std::vector<float> > &vcolors)
{
   bool exportColors = false;

   int dim = 4;

   if (fnMesh.numColorSets() > 0)
   {
      MPlug plug = fnMesh.findPlug("export_colors");
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

void CGeoTranslator::GetComponentIDs(MFnMesh &fnMesh,
      std::vector<AtUInt> &nsides,
      std::vector<AtLong> &vidxs,
      std::vector<AtLong> &nidxs,
      std::vector<AtLong> &uvidxs,
      bool exportNormals,
      bool exportUVs
      )
{
   // Traverse all polygons to export vidxs, nidxs, uvindxs y nsides
   nsides.resize(fnMesh.numPolygons());

   MObject mayaMesh = fnMesh.object();
   MItMeshPolygon itMeshPolygon(mayaMesh);
   unsigned int   polygonIndex = 0;

   for (; (!itMeshPolygon.isDone()); itMeshPolygon.next())
   {
      unsigned int vertexCount = itMeshPolygon.polygonVertexCount();

      nsides[polygonIndex] = vertexCount;

      for (unsigned int V = 0; (V < vertexCount); ++V)
      {
         vidxs.push_back(itMeshPolygon.vertexIndex(V));
         if (exportNormals)
            nidxs.push_back(itMeshPolygon.normalIndex(V));

         if (exportUVs)
         {
            int uvIndex;
            itMeshPolygon.getUVIndex(V, uvIndex);
            uvidxs.push_back(uvIndex);
         }
      }

      ++polygonIndex;
   }
}

void CGeoTranslator::ExportMeshGeometryData(AtNode* polymesh, MObject mayaMesh, AtUInt step)
{
   MFnMesh fnMesh(mayaMesh);
   MFnDagNode fnDagNode(m_dagPath.node());

   MIntArray indices;
   bool multiShader = false;

   if (step == 0)
   {
      int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

      //
      // SHADERS
      //

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
            AtNode* shader = m_scene->ExportShader(connections[0].node());

            AiNodeSetPtr(polymesh, "shader", shader);
            meshShaders.push_back(shader);
         }
         else
            AiMsgWarning("[mtoa] shadingGroup %s has no surfaceShader input.", fnDGNode.name().asChar());
      }
      else
      {
         // per-face assignment
         MObjectArray shadingGroups;

         // indices are used later when exporting shidxs
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
            meshShaders.push_back(m_scene->ExportShader(connections[0].node()));
         }

         multiShader = true;

         AiNodeSetArray(polymesh, "shader", AiArrayConvert((AtInt)meshShaders.size(), 1, AI_TYPE_NODE, &meshShaders[0], TRUE));
      }

      //
      // DISPLACEMENT
      //
      // currently does not work for per-face assignment
      if (!shadingGroup.isNull())
      {
         MPlugArray        connections;
         MFnDependencyNode fnDGNode(shadingGroup);
         MPlug shaderPlug = fnDGNode.findPlug("displacementShader");
         shaderPlug.connectedTo(connections, true, false);

         // are there any connections to displacementShader?
         if (connections.length() > 0)
         {
            MFnDependencyNode dispNode(connections[0].node());

            // Note that disp_height has no actual influence on the scale of the displacement if it is vector based
            // it only influences the computation of the displacement bounds
            AiNodeSetFlt(polymesh, "disp_height", dispNode.findPlug("disp_height").asFloat());
            AiNodeSetFlt(polymesh, "disp_zero_value", dispNode.findPlug("disp_zero_value").asFloat());
            AiNodeSetBool(polymesh, "autobump", dispNode.findPlug("autobump").asBool());

            connections.clear();
            dispNode.findPlug("disp_map").connectedTo(connections, true, false);

            if (connections.length() > 0)
            {
               MString attrName = connections[0].partialName(false, false, false, false, false, true);
               AtNode* dispImage(m_scene->ExportShader(connections[0].node(), attrName));

               MPlug pVectorDisp = dispNode.findPlug("vector_displacement", false);
               if (!pVectorDisp.isNull() && pVectorDisp.asBool())
               {
                  AtNode* tangentToObject = AiNode("tangentToObjectSpace");
                  m_scene->ProcessShaderParameter(dispNode, "vector_displacement_scale", tangentToObject, "scale", AI_TYPE_VECTOR);
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
   } // if step == 0

   //
   // GEOMETRY
   //

   std::vector<float> vertices, normals, tangents, bitangents;

   // Get all vertices
   GetVertices(fnMesh, vertices);

   // Get all normals
   bool exportNormals = GetNormals(fnMesh, normals);

   // Get all tangents, bitangents
   bool exportTangents = GetTangents(fnMesh, tangents, bitangents);

   if (step == 0)
   {

      std::vector<float> uvs;
      std::vector<AtUInt> nsides;
      std::vector<AtLong> vidxs, nidxs, uvidxs;
      std::map<std::string, std::vector<float> > vcolors;

      // Get UVs
      bool exportUVs = GetUVs(fnMesh, uvs);

      // Get Component IDs
      GetComponentIDs(fnMesh, nsides, vidxs, nidxs, uvidxs, exportNormals, exportUVs);

      // Get Vertex Colors
      bool exportColors = GetVertexColors(fnMesh, vcolors);

      bool mb_deform = m_scene->m_motionBlurData.enabled &&
                       m_scene->m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
                       fnDagNode.findPlug("motionBlur").asBool();

      // declare user defined attributes
      if (exportTangents)
      {
         AiNodeDeclare(polymesh, "tangent", "varying VECTOR");
         AiNodeDeclare(polymesh, "bitangent", "varying VECTOR");
      }

      if (exportColors)
      {
         std::map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            AiNodeDeclare(polymesh, it->first.c_str(), "varying RGBA");
            ++it;
         }
      }

      if (!mb_deform)
      {
         // No deformation motion blur, so we create normal arrays
         AiNodeSetArray(polymesh, "vlist", AiArrayConvert(fnMesh.numVertices() * 3, 1, AI_TYPE_FLOAT, &(vertices[0]), TRUE));

         if (exportNormals && (fnMesh.numNormals() > 0))
            AiNodeSetArray(polymesh, "nlist", AiArrayConvert(fnMesh.numNormals() * 3, 1, AI_TYPE_FLOAT, &(normals[0]), TRUE));

         if (exportTangents)
         {
            AiNodeSetArray(polymesh, "tangent", AiArrayConvert(fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(tangents[0]), TRUE));
            AiNodeSetArray(polymesh, "bitangent", AiArrayConvert(fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(bitangents[0]), TRUE));
         }
      }
      else
      {
         // Deformation motion blur. We need to create keyable arrays for vlist and nlist
         AtArray* vlist_array = AiArrayAllocate(fnMesh.numVertices(), m_scene->m_motionBlurData.motion_steps, AI_TYPE_POINT);
         SetKeyData(vlist_array, step, vertices, fnMesh.numVertices());
         AiNodeSetArray(polymesh, "vlist", vlist_array);

         if (exportNormals)
         {
            AtArray* nlist_array = AiArrayAllocate(fnMesh.numNormals(), m_scene->m_motionBlurData.motion_steps, AI_TYPE_VECTOR);
            SetKeyData(nlist_array, step, normals, fnMesh.numNormals());
            AiNodeSetArray(polymesh, "nlist", nlist_array);
         }

         if (exportTangents)
         {
            AtArray* tangent_array = AiArrayAllocate(fnMesh.numVertices(), m_scene->m_motionBlurData.motion_steps, AI_TYPE_VECTOR);
            SetKeyData(tangent_array, step, tangents, fnMesh.numVertices());
            AiNodeSetArray(polymesh, "tangent", tangent_array);

            AtArray* bitangent_array = AiArrayAllocate(fnMesh.numVertices(), m_scene->m_motionBlurData.motion_steps, AI_TYPE_VECTOR);
            SetKeyData(bitangent_array, step, bitangents, fnMesh.numVertices());
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

      if (exportUVs)
      {
         AiNodeSetArray(polymesh, "uvlist", AiArrayConvert(fnMesh.numUVs() * 2, 1, AI_TYPE_FLOAT, &(uvs[0]), TRUE));
         // Same problem here
         //AiNodeSetArray(polymesh, "uvidxs", AiArrayConvert(uvidxs.size(), 1, AI_TYPE_UINT, &(uvidxs[0]), TRUE));
         AtArray *uvidxsTmp = AiArrayAllocate((AtInt)uvidxs.size(), 1, AI_TYPE_UINT);
         for(AtUInt i = 0; (i < uvidxs.size()); i++)
            AiArraySetUInt(uvidxsTmp, i, uvidxs[i]);
         AiNodeSetArray(polymesh, "uvidxs", uvidxsTmp);

      }

      if (multiShader)
      {
         // export face to shader indices
         // first convert from MIntArray to AtUInt vector
         std::vector<AtUInt> shidxs;
         for(AtUInt i = 0; i < indices.length(); i++)
            shidxs.push_back(indices[i]);
         AiNodeSetArray(polymesh, "shidxs", AiArrayConvert((AtInt)shidxs.size(), 1, AI_TYPE_UINT, &(shidxs[0]), TRUE));
      }
      if (exportColors)
      {
         std::map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            AiNodeSetArray(polymesh, it->first.c_str(), AiArrayConvert(fnMesh.numVertices(), 1, AI_TYPE_RGBA, &(it->second[0]), TRUE));
            ++it;
         }
      }
   } // step == 0
   else
   {
      // Export motion blur keys information (for deformation)

      // vertices
      AtArray* vlist_array = AiNodeGetArray(polymesh, "vlist");
      SetKeyData(vlist_array, step, vertices, fnMesh.numVertices());

      // normals
      if (exportNormals)
      {
         AtArray* nlist_array = AiNodeGetArray(polymesh, "nlist");
         SetKeyData(nlist_array, step, normals, fnMesh.numNormals());
      }

      // tangents
      if (exportTangents)
      {
         AtArray* tangent_array = AiNodeGetArray(polymesh, "tangent");
         SetKeyData(tangent_array, step, tangents, fnMesh.numVertices());

         AtArray* bitangent_array = AiNodeGetArray(polymesh, "bitangent");
         SetKeyData(bitangent_array, step, bitangents, fnMesh.numVertices());
      }
   }
}

AtNode* CGeoTranslator::ExportMesh(MObject mayaMesh)
{
   AtNode* polymesh;
   MFnDagNode fnDagNode(m_dagPath.node());

   // Check if custom attributes have been created, ignore them otherwise
   MStatus status;
   fnDagNode.findPlug("subdiv_type", &status);
   bool customAttributes = (status == MS::kSuccess);

   polymesh = AiNode("polymesh");

   AiNodeSetStr(polymesh, "name", m_dagPath.fullPathName().asChar());

   ExportMatrix(polymesh, 0);

   AiNodeSetBool(polymesh, "smoothing", fnDagNode.findPlug("smoothShading").asBool());
   AiNodeSetBool(polymesh, "receive_shadows", fnDagNode.findPlug("receiveShadows").asBool());

   if (fnDagNode.findPlug("doubleSided").asBool())
      AiNodeSetInt(polymesh, "sidedness", 65535);
   else
   {
      AiNodeSetBool(polymesh, "invert_normals", fnDagNode.findPlug("opposite").asBool());
      AiNodeSetInt(polymesh, "sidedness", 0);
   }

   // Visibility options
   AtInt visibility = ComputeVisibility();
   AiNodeSetInt(polymesh, "visibility", visibility);

   if (customAttributes)
   {
      AiNodeSetBool(polymesh, "self_shadows", fnDagNode.findPlug("self_shadows").asBool());
      AiNodeSetBool(polymesh, "opaque", fnDagNode.findPlug("opaque").asBool());

      // Subdivision surfaces
      //
      bool subdivision = (fnDagNode.findPlug("subdiv_type").asInt() != 0);

      if (subdivision)
      {
         AiNodeSetStr(polymesh, "subdiv_type", "catclark");
         AiNodeSetInt(polymesh, "subdiv_iterations", fnDagNode.findPlug("subdiv_iterations").asInt());
         AiNodeSetInt(polymesh, "subdiv_adaptive_metric", fnDagNode.findPlug("subdiv_adaptive_metric").asInt());
         AiNodeSetFlt(polymesh, "subdiv_pixel_error", fnDagNode.findPlug("subdiv_pixel_error").asFloat());

         MString cameraName = fnDagNode.findPlug("subdiv_dicing_camera").asString();
         AtNode* camera = ((cameraName != "") && (cameraName != "Default")) ? AiNodeLookUpByName(cameraName.asChar()) : NULL;
         AiNodeSetPtr(polymesh, "subdiv_dicing_camera", camera);
      }

      // Subsurface Scattering
      //
      AiNodeSetInt(polymesh, "sss_max_samples", fnDagNode.findPlug("sss_max_samples").asInt());
      AiNodeSetFlt(polymesh, "sss_sample_spacing", fnDagNode.findPlug("sss_sample_spacing").asFloat());
      AiNodeSetBool(polymesh, "sss_use_gi", fnDagNode.findPlug("sss_use_gi").asBool());
   }

   ExportMeshGeometryData(polymesh, mayaMesh, 0);
   return polymesh;
}

void CGeoTranslator::ExportMeshMotion(AtNode* polymesh, MObject mayaMesh, AtUInt step)
{
   ExportMatrix(polymesh, step);

   bool mb_deform = m_scene->m_motionBlurData.enabled &&
                    m_scene->m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
                    m_fnNode.findPlug("motionBlur").asBool();

   if (mb_deform)
   {
      ExportMeshGeometryData(polymesh, mayaMesh, step);
   }
}

AtNode* CGeoTranslator::ExportMeshInstance(const MDagPath& masterInstance)
{
   MTransformationMatrix worldMatrix;
   MFloatVector vector;
   AtNode* instanceNode = NULL;
   MFnDagNode fnDagNodeInstance(m_dagPath);
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.fullPathName().asChar());

   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   instanceNode = AiNode("ginstance");
   AiNodeSetStr(instanceNode, "name", m_dagPath.fullPathName().asChar());

   ExportMatrix(instanceNode, 0);

   AiNodeSetPtr(instanceNode, "node", masterNode);
   AiNodeSetBool(instanceNode, "inherit_xform", false);

   //
   // SHADERS
   //
   MFnMesh           meshNode(m_dagPath.node());
   MObjectArray      shaders, shadersMaster;
   MIntArray         indices, indicesMaster;

   meshNode.getConnectedShaders(instanceNum, shaders, indices);
   // FIXME: it is incorrect to assume that instance 0 is the master as it may be hidden
   meshNode.getConnectedShaders(0, shadersMaster, indicesMaster);

   // As arnold does not support different shaders per face
   // on ginstances
   // we keep the master's per face assignment only
   // if it's completely the same
   bool equalShaderArrays = ((shaders.length() == shadersMaster.length()) && (indices.length() == indicesMaster.length()));

   for(AtUInt j=0; (equalShaderArrays && (j < indices.length())); j++)
   {
      if(indices[j] != indicesMaster[j])
      {
         equalShaderArrays = false;
      }
   }
   for(AtUInt i=0; (equalShaderArrays && (i < shaders.length())); i++)
   {
      if (shaders[i] != shadersMaster[i])
      {
         equalShaderArrays = false;
      }
   }

   if ( (shaders.length() > 0) && (shadersMaster.length() > 0) )
   {
      MPlugArray        connections;
      MFnDependencyNode fnDGNode(shaders[0]);
      MPlug             shaderPlug(shaders[0], fnDGNode.attribute("surfaceShader"));
      MPlug             shaderPlugMaster(shadersMaster[0], fnDGNode.attribute("surfaceShader"));

      shaderPlug.connectedTo(connections, true, false);

      if ((shaderPlug != shaderPlugMaster) || (!equalShaderArrays))
      {
         AtNode* shader = m_scene->ExportShader(connections[0].node());
         AiNodeSetPtr(instanceNode, "shader", shader);
      }
   }
   return instanceNode;
}

void CGeoTranslator::ExportMeshInstanceMotion(AtNode* instance, AtUInt step)
{
   ExportMatrix(instance, step);
}

// CNurbsTranslator
//
AtNode* CNurbsSurfaceTranslator::Export()
{
   MStatus status;
   MFnNurbsSurface surface(m_dagPath, &status);

   if (!status)
   {
      AiMsgError("[mtoa] ERROR: Could not create NURBS surface.");
      return NULL;
   }
   MDagPath masterDag;
   m_isMasterDag = IsMasterInstance(masterDag);
   if (m_isMasterDag)
   {
      MFnMeshData meshData;
      MObject     meshDataObject = meshData.create();

      m_tesselatedMesh = surface.tesselate(MTesselationParams::fsDefaultTesselationParams, meshDataObject);
      // in order to get displacement, we need a couple of attributes
      MFnNumericAttribute  nAttr;

      MObject subdiv_type = nAttr.create("subdiv_type", "sdbt", MFnNumericData::kInt, 1);
      surface.addAttribute(subdiv_type);
      MObject subdiv_iterations = nAttr.create("subdiv_iterations", "sdbit", MFnNumericData::kInt, 1);
      surface.addAttribute(subdiv_iterations);
      MObject subdiv_adaptive_metric = nAttr.create("subdiv_adaptive_metric", "sdbam", MFnNumericData::kInt, 1);
      surface.addAttribute(subdiv_adaptive_metric);
      MObject subdiv_pixel_error = nAttr.create("subdiv_pixel_error", "sdbpe", MFnNumericData::kInt, 0);
      surface.addAttribute(subdiv_pixel_error);
      return ExportMesh(m_tesselatedMesh);
   }
   else
   {
      return ExportMeshInstance(masterDag);
   }
}

// FIXME: bogus Update method
void CNurbsSurfaceTranslator::Update(AtNode *polymesh)
{
}

void CNurbsSurfaceTranslator::ExportMotion(AtNode* polymesh, AtUInt step)
{
   if (m_isMasterDag)
   {
      ExportMeshMotion(polymesh, m_tesselatedMesh, step);
   }
   else
   {
      ExportMeshInstanceMotion(polymesh, step);
   }
}

// CPolyTranslator
//
unsigned int CMeshTranslator::GetNumMeshGroups()
{
   MObject node = m_dagPath.node();
   MFnDependencyNode fnDGNode(node);
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   MPlug plug = fnDGNode.findPlug("instObjGroups");

   if (plug.elementByLogicalIndex(instanceNum).isConnected())
   {
      return 1;
   }
   else
   {
      MFnMesh      mesh(node);
      MObjectArray shaders;
      MIntArray    indices;

      mesh.getConnectedShaders(instanceNum, shaders, indices);

      return shaders.length();
   }
}

AtNode* CMeshTranslator::Export()
{
   if (GetNumMeshGroups() == 0)
   {
      AiMsgError("[mtoa] ERROR: Mesh not exported. It has 0 groups.");
      return NULL;
   }

   MDagPath masterDag;
   m_isMasterDag = IsMasterInstance(masterDag);
   if (m_isMasterDag)
   {
      return ExportMesh(m_dagPath.node());
   }
   else
   {
      return ExportMeshInstance(masterDag);
   }
}

// FIXME: bogus Update method
void CMeshTranslator::Update(AtNode *polymesh)
{
}

void CMeshTranslator::ExportMotion(AtNode* polymesh, AtUInt step)
{
   if (m_isMasterDag)
   {
      ExportMeshMotion(polymesh, m_dagPath.node(), step);
   }
   else
   {
      ExportMeshInstanceMotion(polymesh, step);
   }
}

