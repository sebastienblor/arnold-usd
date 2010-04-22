
#include "MayaScene.h"

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

void CMayaScene::ExportMeshGeometryData(AtNode* polymesh, MObject mayaMesh, const MDagPath& dagPath, AtUInt step)
{
   MFnMesh fnMesh(mayaMesh);
   MFnDagNode fnDagNode(dagPath.node());

   MIntArray indices;
   bool multiShader = false;

   if (step == 0)
   {
      //
      // SHADERS
      //

      std::vector<AtNode*> meshShaders;
      MObject mayaShader = GetNodeShader(dagPath.node());

      if (!mayaShader.isNull())
      {
         AtNode* shader = ExportShader(mayaShader);

         AiNodeSetPtr(polymesh, "shader", shader);

         meshShaders.push_back(shader);
      }
      else
      {
         MObjectArray shaders;

         int instanceNum = dagPath.isInstanced() ? dagPath.instanceNumber() : 0;

         fnMesh.getConnectedShaders(instanceNum, shaders, indices);

         for (int J = 0; (J < (int) shaders.length()); ++J)
         {
            MPlugArray        connections;
            MFnDependencyNode fnDGNode(shaders[J]);
            MPlug             shaderPlug(shaders[J], fnDGNode.attribute("surfaceShader"));

            shaderPlug.connectedTo(connections, true, false);

            meshShaders.push_back(ExportShader(connections[0].node()));
         }

         multiShader = true;

         AiNodeSetArray(polymesh, "shader", AiArrayConvert(meshShaders.size(), 1, AI_TYPE_POINTER, &meshShaders[0], TRUE));
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

   std::vector<AtByte> nsides;
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

         for (size_t V = 0; (V < vertexCount); ++V)
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

   if (step == 0)
   {
      bool mb_deform = m_motionBlurData.enabled &&
                       m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
                       fnDagNode.findPlug("motionBlur").asBool();

      if (!mb_deform)
      {
         // No deformation motion blur, so we create normal arrays
         AiNodeSetArray(polymesh, "vlist", AiArrayConvert(fnMesh.numVertices() * 3, 1, AI_TYPE_FLOAT, &(vertices[0]), TRUE));
      
         if (useNormals && (fnMesh.numNormals() > 0))
            AiNodeSetArray(polymesh, "nlist", AiArrayConvert(fnMesh.numNormals() * 3, 1, AI_TYPE_FLOAT, &(normals[0]), TRUE));
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
      }

      AiNodeSetArray(polymesh, "nsides", AiArrayConvert(fnMesh.numPolygons(), 1, AI_TYPE_BYTE, &(nsides[0]), TRUE));
      AiNodeSetArray(polymesh, "vidxs", AiArrayConvert(vidxs.size(), 1, AI_TYPE_UINT, &(vidxs[0]), TRUE));

      if (useNormals && (fnMesh.numNormals() > 0))
      {
         AiNodeSetArray(polymesh, "nidxs", AiArrayConvert(nidxs.size(), 1, AI_TYPE_UINT, &(nidxs[0]), TRUE));
      }

      if (hasUVs)
      {
         AiNodeSetArray(polymesh, "uvlist", AiArrayConvert(fnMesh.numUVs() * 2, 1, AI_TYPE_FLOAT, &(uvs[0]), TRUE));
         AiNodeSetArray(polymesh, "uvidxs", AiArrayConvert(uvidxs.size(), 1, AI_TYPE_UINT, &(uvidxs[0]), TRUE));
      }

      if (multiShader)
         AiNodeSetArray(polymesh, "shidxs", AiArrayConvert(shidxs.size(), 1, AI_TYPE_UINT, &(shidxs[0]), TRUE));
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

   // we need to get the matrix from the transform node
   MFnDagNode dagNodeTransform(dagPath.transform());
   MDagPath   dagPathTransform;
   dagNodeTransform.getPath(dagPathTransform);
   GetMatrix(matrix, dagPathTransform);

   if (step == 0)
   {
      polymesh = AiNode("polymesh");

      AiNodeSetStr(polymesh, "name", dagPathTransform.partialPathName().asChar());

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
         AiNodeSetBool(polymesh, "inv_normals", fnDagNode.findPlug("opposite").asBool());
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
         bool subdivision = fnDagNode.findPlug("subdiv_type").asInt();

         if (subdivision)
         {
            AiNodeSetInt(polymesh, "subdiv_type", 1);
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
      polymesh = AiNodeLookUpByName(dagPathTransform.partialPathName().asChar());

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
   AtMatrix matrix, masterMatrix, masterMatrixInv;
   MFloatVector vector;
   AtNode* instanceNode = NULL;
   MFnDagNode fnDagNodeInstance(dagPath);
   MFnDagNode fnDagNodeMaster(masterInstance.transform());
   AtNode* masterNode = AiNodeLookUpByName(fnDagNodeMaster.partialPathName().asChar());

   bool mb = m_motionBlurData.enabled &&
             m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool() &&
             fnDagNodeInstance.findPlug("motionBlur").asBool();

   // we need to get the matrix from the transform node
   MFnDagNode dagNodeTransform(dagPath.transform());
   MDagPath   dagPathTransform;
   dagNodeTransform.getPath(dagPathTransform);

   MFnDagNode dagNodeMasterTransform(masterInstance.transform());
   MDagPath   dagPathMasterTransform;
   dagNodeMasterTransform.getPath(dagPathMasterTransform);

   GetMatrix(matrix, dagPathTransform);
   GetMatrix(masterMatrix, dagPathMasterTransform);
   AiM4Invert(masterMatrix,masterMatrixInv);
   AiM4Mult(matrix, matrix, masterMatrixInv);

   if (step == 0)
   {
      instanceNode = AiNode("ginstance");
      AiNodeSetStr(instanceNode, "name", dagPathTransform.partialPathName().asChar());

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
   }
   else
   {
      instanceNode = AiNodeLookUpByName(dagPathTransform.partialPathName().asChar());

      if (mb)
      {
         AtArray* matrices = AiNodeGetArray(instanceNode, "matrix");
         AiArraySetMtx(matrices, step, matrix);
      }
   }
}
