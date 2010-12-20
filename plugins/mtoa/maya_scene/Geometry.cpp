
#include "MayaScene.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MDagPath.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MIntArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MItMeshVertex.h>
#include <maya/MGlobal.h>
#include <maya/MBoundingBox.h>

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

static bool StringInList(const MString &str, const MStringArray &ary)
{
   for (unsigned int i = 0; i < ary.length(); ++i)
   {
      if (ary[i] == str)
      {
         return true;
      }
   }
   return false;
}

void CMayaScene::ExportMeshGeometryData(AtNode* polymesh, MObject mayaMesh, const MDagPath& dagPath, AtUInt step)
{
   MFnMesh fnMesh(mayaMesh);
   MFnDagNode fnDagNode(dagPath.node());

   MIntArray indices;
   bool multiShader = false;

   if (step == 0)
   {
      int instanceNum = dagPath.isInstanced() ? dagPath.instanceNumber() : 0;

      //
      // SHADERS
      //

      std::vector<AtNode*> meshShaders;
      MObject mayaShader = GetNodeShader(dagPath.node(),instanceNum);

      if (!mayaShader.isNull())
      {
         AtNode* shader = ExportShader(mayaShader);

         AiNodeSetPtr(polymesh, "shader", shader);

         meshShaders.push_back(shader);
      }
      else
      {
         MObjectArray shaders;

         fnMesh.getConnectedShaders(instanceNum, shaders, indices);

         for (int J = 0; (J < (int) shaders.length()); J++)
         {
            MPlugArray        connections;
            MFnDependencyNode fnDGNode(shaders[J]);
            MPlug             shaderPlug(shaders[J], fnDGNode.attribute("surfaceShader"));

            shaderPlug.connectedTo(connections, true, false);

            meshShaders.push_back(ExportShader(connections[0].node()));
         }

         multiShader = true;

         AiNodeSetArray(polymesh, "shader", AiArrayConvert((AtInt)meshShaders.size(), 1, AI_TYPE_NODE, &meshShaders[0], TRUE));
      }

      //
      // DISPLACEMENT
      //

      MPlugArray        connections;
      MFnDependencyNode fnDGNode(fnDagNode);

      MPlug plug(dagPath.node(), fnDGNode.attribute("instObjGroups"));

      plug.elementByLogicalIndex(instanceNum).connectedTo(connections, false, true);

      // are there any connections to instObjGroups?
      if (connections.length() > 0)
      {
         MPlugArray connected(connections);
         connections.clear();
         for (unsigned int k=0; k<connected.length(); ++k)
         {
            MObject shadingGroup(connections[k].node());
            if (shadingGroup.apiType() == MFn::kShadingEngine )
            {
               fnDGNode.setObject(shadingGroup);
               MPlug shaderPlug(shadingGroup, fnDGNode.attribute("displacementShader"));
               shaderPlug.connectedTo(connections, true, false);
               break;
            }
         }

         // are there any connections to displacementShader?
         if (connections.length() > 0)
         {
            MObjectArray      shaderDisp;
            fnMesh.getConnectedShaders(instanceNum, shaderDisp, indices);

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
               AtNode* dispImage(ExportShader(connections[0].node(), attrName));

               MPlug pVectorDisp = dispNode.findPlug("vector_displacement");
               if (!pVectorDisp.isNull() && pVectorDisp.asBool())
               {
                  AtNode* tangentToObject = AiNode("tangentToObjectSpace");
                  ProcessShaderParameter(dispNode, "vector_displacement_scale", tangentToObject, "scale", AI_TYPE_VECTOR);
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

   //
   // GEOMETRY
   //

   // Get all vertices
   std::vector<float> vertices;
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
   }

   bool useNormals = fnDagNode.findPlug("smoothShading").asBool() && !fnDagNode.findPlug("subdiv_type").asBool();

   // Get all normals
   std::vector<float> normals;
   if (useNormals && (fnMesh.numNormals() > 0))
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
   }

   // Get all tangents, bitangents
   MStatus stat;
   bool exportTangents = false;
   std::vector<float> tangents;
   std::vector<float> bitangents;

   MPlug pExportTangents(mayaMesh, fnMesh.attribute("export_tangents", &stat));

   if (stat == MStatus::kSuccess)
   {
      exportTangents = pExportTangents.asBool();
   }

   if (exportTangents)
   {      
      MItMeshVertex itVertex(mayaMesh);
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
   }

   std::vector<AtUInt> nsides;
   std::vector<AtLong> vidxs, nidxs, uvidxs;
   std::vector<float> uvs;
   std::vector<AtUInt> shidxs;

   bool hasUVs = (fnMesh.numUVs() > 0);

   if (step == 0)
   {
      // Get all UVs
      if (hasUVs)
      {
         uvs.resize(fnMesh.numUVs() * 2);

         MFloatArray uArray, vArray;
         fnMesh.getUVs(uArray, vArray);

         for (int J = 0; (J < fnMesh.numUVs()); ++J)
         {
            uvs[J * 2 + 0] = uArray[J];
            uvs[J * 2 + 1] = vArray[J];
         }
      }

      // Traverse all polygons to export vidxs, nidxs, uvindxs y nsides
      nsides.resize(fnMesh.numPolygons());

      MItMeshPolygon itMeshPolygon(mayaMesh);
      unsigned int   polygonIndex = 0;

      for (; (!itMeshPolygon.isDone()); itMeshPolygon.next())
      {
         if (multiShader)
            shidxs.push_back(indices[itMeshPolygon.index()]);

         unsigned int vertexCount = itMeshPolygon.polygonVertexCount();

         nsides[polygonIndex] = vertexCount;

         for (unsigned int V = 0; (V < vertexCount); ++V)
         {
            vidxs.push_back(itMeshPolygon.vertexIndex(V));
            if (useNormals)
               nidxs.push_back(itMeshPolygon.normalIndex(V));

            if (hasUVs)
            {
               int uvIndex;

               itMeshPolygon.getUVIndex(V, uvIndex);

               uvidxs.push_back(uvIndex);
            }
         }

         ++polygonIndex;
      }
   }

   // Export Vertex Color
   //
   bool exportColors = false;
   std::map<std::string, std::vector<float> > vcolors;
   int dim = 4;

   if (fnMesh.numColorSets() > 0)
   {
      MPlug plug = fnMesh.findPlug("export_colors");
      if (!plug.isNull())
      {
         exportColors = plug.asBool();
      }
   }

   if (step == 0)
   {
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
   }

   if (step == 0)
   {
      bool mb_deform = m_motionBlurData.enabled &&
                       m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
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

         if (useNormals && (fnMesh.numNormals() > 0))
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
         AtArray* vlist_array = AiArrayAllocate(fnMesh.numVertices(), m_motionBlurData.motion_steps, AI_TYPE_POINT);
         SetKeyData(vlist_array, step, vertices, fnMesh.numVertices());
         AiNodeSetArray(polymesh, "vlist", vlist_array);

         if (useNormals && (fnMesh.numNormals() > 0))
         {
            AtArray* nlist_array = AiArrayAllocate(fnMesh.numNormals(), m_motionBlurData.motion_steps, AI_TYPE_VECTOR);
            SetKeyData(nlist_array, step, normals, fnMesh.numNormals());
            AiNodeSetArray(polymesh, "nlist", nlist_array);
         }

         if (exportTangents)
         {
            AtArray* tangent_array = AiArrayAllocate(fnMesh.numVertices(), m_motionBlurData.motion_steps, AI_TYPE_VECTOR);
            SetKeyData(tangent_array, step, tangents, fnMesh.numVertices());
            AiNodeSetArray(polymesh, "tangent", tangent_array);

            AtArray* bitangent_array = AiArrayAllocate(fnMesh.numVertices(), m_motionBlurData.motion_steps, AI_TYPE_VECTOR);
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

      if (useNormals && (fnMesh.numNormals() > 0))
      {
         // Same goes here
         //AiNodeSetArray(polymesh, "nidxs", AiArrayConvert(nidxs.size(), 1, AI_TYPE_UINT, &(nidxs[0]), TRUE));
         AtArray *nidxsTmp = AiArrayAllocate((AtInt)nidxs.size(), 1, AI_TYPE_UINT);
         for(AtUInt i = 0; (i < nidxs.size()); i++)
            AiArraySetUInt(nidxsTmp, i, nidxs[i]);
         AiNodeSetArray(polymesh, "nidxs", nidxsTmp);
      }

      if (hasUVs)
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
         AiNodeSetArray(polymesh, "shidxs", AiArrayConvert((AtInt)shidxs.size(), 1, AI_TYPE_UINT, &(shidxs[0]), TRUE));
         
      if (exportColors)
      {
         std::map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            AiNodeSetArray(polymesh, it->first.c_str(), AiArrayConvert(fnMesh.numVertices(), 1, AI_TYPE_RGBA, &(it->second[0]), TRUE));
            ++it;
         }
      }
   }
   else
   {
      // Export motion blur keys information (for deformation)
      AtArray* vlist_array = AiNodeGetArray(polymesh, "vlist");
      SetKeyData(vlist_array, step, vertices, fnMesh.numVertices());

      if (useNormals && (fnMesh.numNormals() > 0))
      {
         AtArray* nlist_array = AiNodeGetArray(polymesh, "nlist");
         SetKeyData(nlist_array, step, normals, fnMesh.numNormals());
      }

      if (exportTangents)
      {
         AtArray* tangent_array = AiNodeGetArray(polymesh, "tangent");
         SetKeyData(tangent_array, step, tangents, fnMesh.numVertices());

         AtArray* bitangent_array = AiNodeGetArray(polymesh, "bitangent");
         SetKeyData(bitangent_array, step, bitangents, fnMesh.numVertices());
      }
   }
}

void CMayaScene::ExportMesh(MObject mayaMesh, const MDagPath& dagPath, AtUInt step)
{
   AtMatrix matrix;
   AtNode* polymesh;
   MFnDagNode fnDagNode(dagPath.node());

   bool mb = m_motionBlurData.enabled &&
             m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool() &&
             fnDagNode.findPlug("motionBlur").asBool();

   // Check if custom attributes have been created, ignore them otherwise
   MStatus status;
   fnDagNode.findPlug("subdiv_type", &status);
   bool customAttributes = (status == MS::kSuccess);

   GetMatrix(matrix, dagPath);

   if (step == 0)
   {
      polymesh = AiNode("polymesh");

      AiNodeSetStr(polymesh, "name", dagPath.fullPathName().asChar());

      if (mb)
      {
         AtArray* matrices = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_MATRIX);
         AiArraySetMtx(matrices, step, matrix);
         AiNodeSetArray(polymesh, "matrix", matrices);
      }
      else
      {
         AiNodeSetMatrix(polymesh, "matrix", matrix);
      }

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
      AtInt visibility = 65535;

      if (!fnDagNode.findPlug("castsShadows").asBool())
         visibility &= ~AI_RAY_SHADOW;

      if (!fnDagNode.findPlug("primaryVisibility").asBool())
         visibility &= ~AI_RAY_CAMERA;

      if (!fnDagNode.findPlug("visibleInReflections").asBool())
         visibility &= ~AI_RAY_REFLECTED;

      if (!fnDagNode.findPlug("visibleInRefractions").asBool())
         visibility &= ~AI_RAY_REFRACTED;

      if (customAttributes)
      {
         if (!fnDagNode.findPlug("diffuse_visibility").asBool())
            visibility &= ~AI_RAY_DIFFUSE;

         if (!fnDagNode.findPlug("glossy_visibility").asBool())
            visibility &= ~AI_RAY_GLOSSY;
      }

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

      ExportMeshGeometryData(polymesh, mayaMesh, dagPath, step);
   }
   else
   {
      polymesh = AiNodeLookUpByName(dagPath.fullPathName().asChar());

      if (mb)
      {
         AtArray* matrices = AiNodeGetArray(polymesh, "matrix");
         AiArraySetMtx(matrices, step, matrix);
      }

      bool mb_deform = m_motionBlurData.enabled &&
                       m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
                       fnDagNode.findPlug("motionBlur").asBool();

      if (mb_deform)
      {
         ExportMeshGeometryData(polymesh, mayaMesh, dagPath, step);
      }
   }
}
void CMayaScene::ExportMeshInstance(const MDagPath& dagPath, const MDagPath& masterInstance, AtUInt step)
{
   MTransformationMatrix worldMatrix;
   AtMatrix matrix;
   MFloatVector vector;
   AtNode* instanceNode = NULL;
   MFnDagNode fnDagNodeInstance(dagPath);
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.fullPathName().asChar());

   bool mb = m_motionBlurData.enabled &&
             m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool() &&
             fnDagNodeInstance.findPlug("motionBlur").asBool();

   GetMatrix(matrix, dagPath);

   if (step == 0)
   {
      int instanceNum = dagPath.isInstanced() ? dagPath.instanceNumber() : 0;

      instanceNode = AiNode("ginstance");
      AiNodeSetStr(instanceNode, "name", dagPath.fullPathName().asChar());

      if (mb)
      {
         AtArray* matrices = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_MATRIX);
         AiArraySetMtx(matrices, step, matrix);
         AiNodeSetArray(instanceNode, "matrix", matrices);
      }
      else
      {
         AiNodeSetMatrix(instanceNode, "matrix", matrix);
      }

      AiNodeSetPtr(instanceNode, "node", masterNode);
      AiNodeSetBool(instanceNode, "inherit_xform", false);

      //
      // SHADERS
      //
      MFnMesh           meshNode(dagPath.node());
      MObjectArray      shaders, shadersMaster;
      MIntArray         indices, indicesMaster;

      meshNode.getConnectedShaders(instanceNum, shaders, indices);
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
            AtNode* shader = ExportShader(connections[0].node());
            AiNodeSetPtr(instanceNode, "shader", shader);
         }
      }
   }
   else
   {
      instanceNode = AiNodeLookUpByName(dagPath.fullPathName().asChar());

      if (mb)
      {
         AtArray* matrices = AiNodeGetArray(instanceNode, "matrix");
         AiArraySetMtx(matrices, step, matrix);
      }
   }
}

void CMayaScene::ExportCustomShape(const MDagPath &dagPath, AtUInt step, const MString &exportCmd, const MString &cleanupCmd)
{
   bool isMasterDag = false;

   MFnDagNode node(dagPath.node());

   bool transformBlur = m_motionBlurData.enabled &&
                        m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool() &&
                        node.findPlug("motionBlur").asBool();

   bool deformBlur = m_motionBlurData.enabled &&
                     m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
                     node.findPlug("motionBlur").asBool();

   char buffer[64];

   MString command = exportCmd;
   command += "(";

   sprintf(buffer, "%f", m_currentFrame);
   command += buffer;
   command += ", ";

   sprintf(buffer, "%d", step);
   command += buffer;
   command += ", ";

   sprintf(buffer, "%f", (step < m_motionBlurData.frames.size() ? m_motionBlurData.frames[step] : m_currentFrame));
   command += buffer;
   command += ", ";

   // List of arnold attributes the custom shape export command has overriden
   MStringArray attrs;

   MDagPath masterDag;
   if (IsMasterInstance(dagPath, masterDag))
   {
      command += "\"" + dagPath.fullPathName() + "\", 0, \"\")";
      isMasterDag = true;
      // If IsMasterInstance return true, masterDag won't get set
      masterDag = dagPath;
   }
   else
   {
      command += "\"" + dagPath.fullPathName() + "\", 1, \"" + masterDag.fullPathName() + "\")";
   }

   MStatus status = MGlobal::executeCommand(command, attrs);
   if (!status)
   {
      AiMsgError("[mtoa] ERROR: Failed to export custom shape \"%s\".", node.name().asChar());
      return;
   }


   MFnDependencyNode masterShadingEngine;
   MFnDependencyNode shadingEngine;
   float dispHeight = 0.0f;

   // Figure out displacement height
   // -> suppose disp_map has its value in [0, 1] range and disp_height is constant)
   // -> all instances share the same displacement shader, the one of the master dag
   // NOTE: this might not work properly if the master shape overrides the disp_height
   //       and instances do not use the ginstance but another procedural node

   GetCustomShapeInstanceShader(masterDag, masterShadingEngine);

   if (masterShadingEngine.object() != MObject::kNullObj)
   {
      MPlug pDispShader = masterShadingEngine.findPlug("displacementShader");
      if (!pDispShader.isNull())
      {
         MPlugArray conns;

         pDispShader.connectedTo(conns, true, false);

         if (conns.length() > 0)
         {
            MFnDependencyNode dispNode(conns[0].node());

            MPlug pDispHeight = dispNode.findPlug("disp_height");

            if (!pDispHeight.isNull())
            {
               dispHeight = pDispHeight.asFloat();
            }
         }
      }
   }

   AtNode *anode = AiNodeLookUpByName(dagPath.fullPathName().asChar());

   if (anode)
   {
      GetCustomShapeInstanceShader(dagPath, shadingEngine);

      AtMatrix matrix;
      MMatrix mmatrix = dagPath.inclusiveMatrix();
      ConvertMatrix(matrix, mmatrix);

      // Set transformation matrix
      if (transformBlur && !StringInList("matrix", attrs))
      {
         if (step == 0)
         {
            AtArray* matrices = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_MATRIX);
            AiArraySetMtx(matrices, step, matrix);
            AiNodeSetArray(anode, "matrix", matrices);
         }
         else
         {
            AtArray* matrices = AiNodeGetArray(anode, "matrix");
            AiArraySetMtx(matrices, step, matrix);
         }
      }
      else
      {
         AiNodeSetMatrix(anode, "matrix", matrix);
      }

      // Set bounding box (in local space)
      if (!StringInList("min", attrs) && !StringInList("max", attrs))
      {
         if (step == 0)
         {
            // set for the first time
            MBoundingBox bbox = node.boundingBox();

            MPoint bmin = bbox.min(); // * mmatrix;
            MPoint bmax = bbox.max(); // * mmatrix;

            bmin.x -= dispHeight;
            bmin.y -= dispHeight;
            bmin.z -= dispHeight;

            bmax.x += dispHeight;
            bmax.y += dispHeight;
            bmax.z += dispHeight;

            AiNodeSetPnt(anode, "min", static_cast<float>(bmin.x), static_cast<float>(bmin.y), static_cast<float>(bmin.z));
            AiNodeSetPnt(anode, "max", static_cast<float>(bmax.x), static_cast<float>(bmax.y), static_cast<float>(bmax.z));
         }
         else
         {
            if (transformBlur || deformBlur)
            {
               AtPoint cmin = AiNodeGetPnt(anode, "min");
               AtPoint cmax = AiNodeGetPnt(anode, "max");

               MBoundingBox bbox = node.boundingBox();

               MPoint bmin = bbox.min(); // * mmatrix;
               MPoint bmax = bbox.max(); // * mmatrix;

               bmin.x -= dispHeight;
               bmin.y -= dispHeight;
               bmin.z -= dispHeight;

               bmax.x += dispHeight;
               bmax.y += dispHeight;
               bmax.z += dispHeight;

               if (bmin.x < cmin.x)
                  cmin.x = static_cast<float>(bmin.x);
               if (bmin.y < cmin.y)
                  cmin.y = static_cast<float>(bmin.y);
               if (bmin.z < cmin.z)
                  cmin.z = static_cast<float>(bmin.z);
               if (bmax.x > cmax.x)
                  cmax.x = static_cast<float>(bmax.x);
               if (bmax.y > cmax.y)
                  cmax.y = static_cast<float>(bmax.y);
               if (bmax.z > cmax.z)
                  cmax.z = static_cast<float>(bmax.z);

               AiNodeSetPnt(anode, "min", cmin.x, cmin.y, cmin.z);
               AiNodeSetPnt(anode, "max", cmax.x, cmax.y, cmax.z);
            }
         }
      }

      if (step == 0)
      {
         // Set common attributes
         MPlug plug;

         if (isMasterDag)
         {
            // For master node only if there are instances

            if (!StringInList("subdiv_type", attrs))
            {
               plug = node.findPlug("subdiv_type");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_type") != 0 ||
                      AiNodeDeclare(anode, "subdiv_type", "constant INT"))
                  {
                     AiNodeSetInt(anode, "subdiv_type", plug.asInt());
                  }
               }
            }

            if (!StringInList("subdiv_iterations", attrs))
            {
               plug = node.findPlug("subdiv_iterations");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_iterations") != 0 ||
                      AiNodeDeclare(anode, "subdiv_iterations", "constant INT"))
                  {
                     AiNodeSetInt(anode, "subdiv_iterations", plug.asInt());
                  }
               }
            }

            if (!StringInList("subdiv_adaptive_metric", attrs))
            {
               plug = node.findPlug("subdiv_adaptive_metric");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_adaptive_metric") != 0 ||
                      AiNodeDeclare(anode, "subdiv_adaptive_metric", "constant INT"))
                  {
                     AiNodeSetInt(anode, "subdiv_adaptive_metric", plug.asInt());
                  }
               }
            }

            if (!StringInList("subdiv_pixel_error", attrs))
            {
               plug = node.findPlug("subdiv_pixel_error");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_pixel_error") != 0 ||
                      AiNodeDeclare(anode, "subdiv_pixel_error", "constant FLOAT"))
                  {
                     AiNodeSetFlt(anode, "subdiv_pixel_error", plug.asFloat());
                  }
               }
            }

            if (!StringInList("subdiv_dicing_camera", attrs))
            {
               plug = node.findPlug("subdiv_dicing_camera");
               if (!plug.isNull())
               {
                  MString cameraName = plug.asString();

                  AtNode *cameraNode = NULL;

                  if (cameraName != "" && cameraName != "Default")
                  {
                     cameraNode = AiNodeLookUpByName(cameraName.asChar());
                  }

                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_dicing_camera") != 0 ||
                      AiNodeDeclare(anode, "subdiv_dicing_camera", "constant NODE"))
                  {
                     AiNodeSetPtr(anode, "subdiv_dicing_camera", cameraNode);
                  }
               }
            }

            if (!StringInList("smoothing", attrs))
            {
               plug = node.findPlug("smoothShading");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "smoothing") != 0 ||
                      AiNodeDeclare(anode, "smoothing", "constant BOOL"))
                  {
                     AiNodeSetBool(anode, "smoothing", plug.asBool());
                  }
               }
            }

            // Set diplacement shader
            if (!StringInList("disp_map", attrs))
            {
               if (shadingEngine.object() != MObject::kNullObj)
               {
                  MPlugArray shaderConns;

                  MPlug shaderPlug = shadingEngine.findPlug("displacementShader");

                  shaderPlug.connectedTo(shaderConns, true, false);

                  if (shaderConns.length() > 0)
                  {
                     MFnDependencyNode dispNode(shaderConns[0].node());

                     if (!StringInList("disp_height", attrs) &&
                         (AiNodeEntryLookUpParameter(anode->base_node, "disp_height") != 0 ||
                          AiNodeDeclare(anode, "disp_height", "constant FLOAT")))
                     {
                        plug = dispNode.findPlug("disp_height");
                        if (!plug.isNull())
                        {
                           AiNodeSetFlt(anode, "disp_height", plug.asFloat());
                        }
                     }

                     if (!StringInList("disp_zero_value", attrs) &&
                         (AiNodeEntryLookUpParameter(anode->base_node, "disp_zero_value") != 0 ||
                          AiNodeDeclare(anode, "disp_zero_value", "constant FLOAT")))
                     {
                        plug = dispNode.findPlug("disp_zero_value");
                        if (!plug.isNull())
                        {
                           AiNodeSetFlt(anode, "disp_zero_value", plug.asFloat());
                        }
                     }

                     shaderConns.clear();
                     dispNode.findPlug("disp_map").connectedTo(shaderConns, true, false);

                     if (shaderConns.length() > 0 &&
                         (AiNodeEntryLookUpParameter(anode->base_node, "disp_map") != 0 ||
                          AiNodeDeclare(anode, "disp_map", "constant NODE")))
                     {
                        plug = dispNode.findPlug("disp_map");
                        if (!plug.isNull())
                        {
                           MString attrName = shaderConns[0].partialName(false, false, false, false, false, true);
                           AtNode* dispImage(ExportShader(shaderConns[0].node(), attrName));

                           MPlug pVectorDisp = dispNode.findPlug("vector_displacement");
                           if (!pVectorDisp.isNull() && pVectorDisp.asBool())
                           {
                              AtNode* tangentToObject = AiNode("tangentToObjectSpace");
                              ProcessShaderParameter(dispNode, "vector_displacement_scale", tangentToObject, "scale", AI_TYPE_VECTOR);
                              AiNodeLink(dispImage, "map", tangentToObject);

                              AiNodeSetPtr(anode, "disp_map", tangentToObject);
                           }
                           else
                           {
                              AiNodeSetPtr(anode, "disp_map", dispImage);
                           }
                        }
                     }

                     AiNodeSetBool(anode, "autobump", dispNode.findPlug("autobump").asBool());
                  }
               }
            }
         }

         if (!StringInList("sidedness", attrs))
         {
            plug = node.findPlug("doubleSided");
            if (!plug.isNull())
            {
               AiNodeSetInt(anode, "sidedness", plug.asBool() ? 65535 : 0);

               // only set invert_normals if doubleSided attribute could be found
               if (!StringInList("invert_normals", attrs))
               {
                  plug = node.findPlug("opposite");
                  if (!plug.isNull())
                  {
                     AiNodeSetBool(anode, "invert_normals", plug.asBool());
                  }
               }
            }
         }

         if (!StringInList("sss_max_samples", attrs))
         {
            plug = node.findPlug("sss_max_samples");
            if (!plug.isNull())
            {
               AiNodeSetInt(anode, "sss_max_samples", plug.asInt());
            }
         }

         if (!StringInList("sss_sample_spacing", attrs))
         {
            plug = node.findPlug("sss_sample_spacing");
            if (!plug.isNull())
            {
               AiNodeSetFlt(anode, "sss_sample_spacing", plug.asFloat());
            }
         }

         if (!StringInList("sss_use_gi", attrs))
         {
            plug = node.findPlug("sss_use_gi");
            if (!plug.isNull())
            {
               AiNodeSetBool(anode, "sss_use_gi", plug.asBool());
            }
         }

         if (!StringInList("receive_shadows", attrs))
         {
            plug = node.findPlug("receiveShadows");
            if (!plug.isNull())
            {
               AiNodeSetBool(anode, "receive_shadows", plug.asBool());
            }
         }

         if (!StringInList("self_shadows", attrs))
         {
            plug = node.findPlug("self_shadows");
            if (!plug.isNull())
            {
               AiNodeSetBool(anode, "self_shadows", plug.asBool());
            }
         }

         if (!StringInList("opaque", attrs))
         {
            plug = node.findPlug("opaque");
            if (!plug.isNull())
            {
               AiNodeSetBool(anode, "opaque", plug.asBool());
            }
         }

         if (!StringInList("visibility", attrs))
         {
            AtInt visibility = AI_RAY_ALL;

            plug = node.findPlug("castsShadows");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_SHADOW;
            }

            plug = node.findPlug("primaryVisibility");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_CAMERA;
            }

            plug = node.findPlug("visibleInReflections");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_REFLECTED;
            }

            plug = node.findPlug("visibleInRefractions");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_REFRACTED;
            }

            plug = node.findPlug("diffuse_visibility");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_DIFFUSE;
            }

            plug = node.findPlug("glossy_visibility");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_GLOSSY;
            }

            AiNodeSetInt(anode, "visibility", visibility);
         }

         // Set surface shader
         if (!StringInList("shader", attrs))
         {
            if (shadingEngine.object() != MObject::kNullObj)
            {
               MPlugArray shaderConns;

               MPlug shaderPlug = shadingEngine.findPlug("surfaceShader");

               shaderPlug.connectedTo(shaderConns, true, false);

               if (shaderConns.length() > 0)
               {
                  AtNode *shader = ExportShader(shaderConns[0].node());
                  AiNodeSetPtr(anode, "shader", shader);
               }
            }
         }
      }

      // Call cleanup command
      if ((m_motionBlurData.enabled == false || int(step) >= (int(m_motionBlurData.motion_steps) - 1)) && cleanupCmd != "")
      {
         command = cleanupCmd;
         command += "(\"";
         command += dagPath.fullPathName();
         command += "\", ";

         if (isMasterDag)
         {
            command += "0, \"\")";
         }
         else
         {
            command += "1, \"";
            command += masterDag.fullPathName();
            command += "\")";
         }

         status = MGlobal::executeCommand(command);

         if (!status)
         {
            AiMsgError("[mtoa] ERROR: Failed to cleanup custom shape \"%s\".", node.name().asChar());
            return;
         }
      }
   }
}

void CMayaScene::GetCustomShapes()
{
   static const char separators[] = ",:;";

   MString csVar = "$MTOA_CUSTOM_SHAPES";

   MString csExpVar = csVar.expandEnvironmentVariablesAndTilde();

   if (csExpVar != csVar)
   {
      std::string shapeType;
      std::string shapeList = csExpVar.asChar();

      size_t p0 = 0;
      size_t p1 = shapeList.find_first_of(separators, p0);

      while (p1 != std::string::npos)
      {
         shapeType = shapeList.substr(p0, p1-p0);

         if (RegisterCustomShape(shapeType))
         {
            MGlobal::displayInfo("[mtoa] Registered custom shape \"" + MString(shapeType.c_str()) + "\"");
         }

         p0 = p1 + 1;
         p1 = shapeList.find_first_of(separators, p0);
      }

      shapeType = shapeList.substr(p0);

      if (RegisterCustomShape(shapeType))
      {
         MGlobal::displayInfo("[mtoa] Registered custom shape \"" + MString(shapeType.c_str()) + "\"");
      }
   }
}
