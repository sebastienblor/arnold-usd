
#include "MayaScene.h"

#include <ai_cameras.h>
#include <ai_constants.h>
#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/M3DView.h>
#include <maya/MDagPath.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnAmbientLight.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnCamera.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnStringData.h>
#include <maya/MImage.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>

#include <vector>

namespace  // <anonymous>
{

   struct CShaderData
   {

      MObject mayaShader;
      AtNode* arnoldShader;

   };  // struct CShaderData

   std::vector<CShaderData> processedShaders;

   MObject GetNodeShader(MObject dagNode)
   {

      MPlugArray        connections;
      MFnDependencyNode fnDGNode(dagNode);

      // Find the "instObjGroups" attribute. Follow that plug to see where it is connected.
      MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

      plug.elementByLogicalIndex(0).connectedTo(connections, false, true);

      if (connections.length() != 1)
      {
         return MObject::kNullObj;
      }

      MObject shadingGroup(connections[0].node());

      fnDGNode.setObject(shadingGroup);

      MPlug shaderPlug(shadingGroup, fnDGNode.attribute("surfaceShader"));

      connections.clear();

      shaderPlug.connectedTo(connections, true, false);

      return connections[0].node();

   }  // GetNodeShader()


   AtNode* ProcessShader(MObject mayaShader)
   {
      // First check if this shader has already been processed
      for (std::vector<CShaderData>::const_iterator it = processedShaders.begin(); (it != processedShaders.end()); ++it)
      {
         if (it->mayaShader == mayaShader)
         {
            return it->arnoldShader;
         }
      }

      AtNode* shader = NULL;

      switch (mayaShader.apiType())
      {
      case MFn::kLambert:
         {
            MFnLambertShader fnShader(mayaShader);

            shader = AiNode("lambert");
         }
         break;

      case MFn::kPhong:
         {
         }
         break;

      case MFn::kBlinn:
         {
         }
         break;
      }

      if (shader)
      {
         CShaderData   data;

         data.mayaShader   = mayaShader;
         data.arnoldShader = shader;

         processedShaders.push_back(data);
      }

      return shader;

   }  // ProcessShader()


   void ProcessCamera(const MDagPath& dagPath)
   {

      MPoint     point;
      MVector    vector;
      MFnCamera  fnCamera(dagPath);
      MFnDagNode fnDagNode(dagPath);

      AtNode* camera = AiNode("persp_camera");

      AiNodeSetStr(camera, "name", fnDagNode.name().asChar());

      point = fnCamera.eyePoint(MSpace::kWorld);
      AiNodeSetPnt(camera, "position", point.x, point.y, point.z);

      vector = fnCamera.viewDirection(MSpace::kWorld);
      point  = point + vector;
      AiNodeSetPnt(camera, "look_at", point.x, point.y, point.z);

      vector = fnCamera.upDirection(MSpace::kWorld);
      AiNodeSetVec(camera, "up", vector.x, vector.y, vector.z);

      AiNodeSetFlt(camera, "fov", static_cast<float>(AI_RTOD * fnCamera.horizontalFieldOfView()));
      AiNodeSetStr(camera, "handedness", "right");

   }  // ProcessCamera()


   void ProcessLight(const MDagPath& dagPath)
   {

      MTransformationMatrix   lightWorldMatrix;
      MFloatVector            vector;
      MColor                  color;
      AtNode*                 light = NULL;
      MFnDagNode              fnDagNode(dagPath);

      if (dagPath.hasFn(MFn::kAmbientLight))
      {
      }
      else if (dagPath.hasFn(MFn::kDirectionalLight))
      {
         MFnDirectionalLight fnLight(dagPath);

         light = AiNode("distant_light");

         AiNodeSetStr(light, "name", fnDagNode.name().asChar());

         color = fnLight.color();

         AiNodeSetRGB(light, "color", color.r, color.g, color.b);
         AiNodeSetFlt(light, "intensity", fnLight.intensity());

         vector = fnLight.lightDirection(0, MSpace::kWorld);

         AiNodeSetVec(light, "direction", vector.x, vector.y, vector.z);
         AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
      }
      else if (dagPath.hasFn(MFn::kPointLight))
      {
         MFnPointLight fnLight(dagPath);

         light = AiNode("point_light");

         AiNodeSetStr(light, "name", fnDagNode.name().asChar());

         color = fnLight.color();

         AiNodeSetRGB(light, "color", color.r, color.g, color.b);
         AiNodeSetFlt(light, "intensity", fnLight.intensity());

         lightWorldMatrix = dagPath.inclusiveMatrix();
         vector           = lightWorldMatrix.translation(MSpace::kWorld);

         AiNodeSetVec(light, "position", vector.x, vector.y, vector.z);

         AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
      }
      else if (dagPath.hasFn(MFn::kSpotLight))
      {
      }
      else if (dagPath.hasFn(MFn::kAreaLight))
      {
      }
      else
      {
         MFnLight fnLight(dagPath);

         AiMsgError("[mtoa] ERROR: Unknown light type (%s).\n", fnLight.typeName().asChar());
      }

   }  // ProcessLight()


   void ProcessMesh(MObject mayaMesh, MObject dagNode, MMatrix tm)
   {

      MFnMesh    fnMesh(mayaMesh);
      MFnDagNode fnDagNode(dagNode);
      bool       hasUVs = (fnMesh.numUVs() > 0);
      AtNode*    polymesh = AiNode("polymesh");

      AiNodeSetStr(polymesh, "name", fnDagNode.name().asChar());

      AtMatrix matrix;

      for (int J = 0; (J < 4); ++J)
      {
         for (int I = 0; (I < 4); ++I)
         {
            matrix[I][J] = (float) tm[I][J];
         }
      }

      AiNodeSetMatrix(polymesh, "matrix", matrix);

      AiNodeSetBool(polymesh, "smoothing", 1);

      //
      // GEOMETRY
      //

      // Get all vertices
      if (fnMesh.numVertices() > 0)
      {
         MFloatPointArray pointsArray;

         fnMesh.getPoints(pointsArray, MSpace::kObject);

         float* vertices = new float[fnMesh.numVertices() * 3];

         for (int J = 0; ( J < fnMesh.numVertices() ); ++J)
         {
            vertices[J * 3 + 0] = pointsArray[J].x;
            vertices[J * 3 + 1] = pointsArray[J].y;
            vertices[J * 3 + 2] = pointsArray[J].z;
         }

         AiNodeSetArray(polymesh, "vlist", AiArrayConvert(fnMesh.numVertices() * 3, 1, AI_TYPE_FLOAT, vertices, TRUE));

         delete[] vertices;
      }

      // Get all normals
      if (fnMesh.numNormals() > 0)
      {
         MFloatVectorArray normalArray;

         fnMesh.getNormals(normalArray, MSpace::kObject);

         float* normals = new float[fnMesh.numNormals() * 3];

         for (int J = 0; (J < fnMesh.numNormals()); ++J)
         {
            normals[J * 3 + 0] = normalArray[J].x;
            normals[J * 3 + 1] = normalArray[J].y;
            normals[J * 3 + 2] = normalArray[J].z;
         }

         AiNodeSetArray(polymesh, "nlist", AiArrayConvert(fnMesh.numNormals() * 3, 1, AI_TYPE_FLOAT, normals, TRUE));

         delete[] normals;
      }

      // Get all UVs
      if (hasUVs)
      {
         MFloatArray uArray, vArray;

         fnMesh.getUVs(uArray, vArray);

         float* uvs = new float[fnMesh.numUVs() * 2];

         for (int J = 0; (J < fnMesh.numUVs()); ++J)
         {
            uvs[J * 2 + 0] = uArray[J];
            uvs[J * 2 + 1] = vArray[J];
         }

         AiNodeSetArray(polymesh, "uvlist", AiArrayConvert(fnMesh.numUVs() * 2, 1, AI_TYPE_FLOAT, uvs, TRUE));

         delete[] uvs;
      }

      // Traverse all polygons to export vidxs, nidxs, uvindxs y nsides
      AtByte* nsides = new AtByte[fnMesh.numPolygons()];

      std::vector<AtLong> vidxs, nidxs, uvidxs;

      MItMeshPolygon itMeshPolygon(mayaMesh);
      unsigned int   polygonIndex = 0;

      for (; (!itMeshPolygon.isDone()); itMeshPolygon.next())
      {
         unsigned int vertexCount = itMeshPolygon.polygonVertexCount();

         nsides[polygonIndex] = vertexCount;

         for (size_t V = 0; (V < vertexCount); ++V)
         {
            vidxs.push_back(itMeshPolygon.vertexIndex(V));
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

      AiNodeSetArray(polymesh, "nsides", AiArrayConvert(fnMesh.numPolygons(), 1, AI_TYPE_BYTE, nsides, TRUE));
      AiNodeSetArray(polymesh, "vidxs", AiArrayConvert(vidxs.size(), 1, AI_TYPE_UINT, &(vidxs[0]), TRUE));
      AiNodeSetArray(polymesh, "nidxs", AiArrayConvert(nidxs.size(), 1, AI_TYPE_UINT, &(nidxs[0]), TRUE));

      if (hasUVs)
      {
         AiNodeSetArray(polymesh, "uvidxs", AiArrayConvert(uvidxs.size(), 1, AI_TYPE_UINT, &(uvidxs[0]), TRUE));
      }

      delete[] nsides;

   }  // processMesh()

}  // namespace <anonymous>


MStatus ProcessMayaScene(MItDag::TraversalType traversalType)
{

   MDagPath dagPath;
   MStatus  status;
   MItDag   dagIterator(traversalType, MFn::kInvalid);

   // Export current camera
   MDagPath cameraPath;

   M3dView::active3dView().getCamera(cameraPath);

   ProcessCamera(cameraPath);

   for (dagIterator.reset(); (!dagIterator.isDone()); dagIterator.next())
   {
      // MItDag::getPath() gets the reference to the object that the iterator is currently on.
      // This DAG path can then be used in a function set to operate on the object.
      // In general it is not a good idea to rearrange the DAG from with an iterator.
      if (!dagIterator.getPath(dagPath))
      {
         AiMsgError("[mtoa] ERROR: Could not get path for DAG iterator.");

         return status;
      }

      MMatrix tm = dagPath.inclusiveMatrix();

      if (dagIterator.item().hasFn(MFn::kLight))
      {
         ProcessLight(dagPath);
      }
      else if (dagIterator.item().hasFn(MFn::kNurbsSurface))
      {
         MFnNurbsSurface surface(dagPath, &status);

         if (!status)
         {
            AiMsgError("[mtoa] ERROR: Could not create NURBS surface.");

            return status;
         }

         MFnMeshData meshData;
         MObject     meshFromNURBS;
         MObject     meshDataObject = meshData.create();

         meshFromNURBS = surface.tesselate(MTesselationParams::fsDefaultTesselationParams, meshDataObject);

         ProcessMesh(meshFromNURBS, dagIterator.item(), tm);
      }
      else if (dagIterator.item().hasFn(MFn::kMesh))
      {
         unsigned int      numMeshGroups;
         MFnDependencyNode fnDGNode(dagIterator.item());

         // Buscamos el atributo "instObjGroups" del nodo para ver si esta conectado
         MPlug plug(dagIterator.item(), fnDGNode.attribute("instObjGroups"));

         if (plug.elementByLogicalIndex(0).isConnected())
         {
            numMeshGroups = 1;
         }
         else
         {
            MFnMesh      mesh(dagIterator.item());
            MObjectArray shaders;
            MIntArray    indices;

            mesh.getConnectedShaders(0, shaders, indices);

            unsigned int numMeshGroups = shaders.length();
         }

         if (numMeshGroups == 0)
         {
            AiMsgError("[mtoa] ERROR: Mesh not exported. It has 0 groups.");
         }
         else
         {
            ProcessMesh(dagIterator.item(), dagIterator.item(), tm);
         }
      }
      else
      {
         if (!status)
         {
            AiMsgError("[mtoa] ERROR: Unrecognized node found while iterating DAG.");

            return status;
         }
      }
   }

   return MS::kSuccess;

}  // ProcessMayaScene()
