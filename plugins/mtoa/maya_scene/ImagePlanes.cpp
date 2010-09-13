#include "MayaScene.h"
   
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>
   
void CMayaScene::ExportImagePlanes(const MDagPath& dagPath, bool isRenderingCamera)
   {
   
   MFnDagNode fnDagNode(dagPath);
   MPlugArray connectedPlugs;
   MPlug      imagePlanePlug;
   MPlug      imagePlaneNodePlug;
   MStatus    status;
   
   // first we get the image planes connected to this camera
   imagePlanePlug = fnDagNode.findPlug("imagePlane");
   
   if (imagePlanePlug.numConnectedElements() > 0)
   {
      for(AtUInt ips = 0; (ips < imagePlanePlug.numElements()); ips++)
      {
         imagePlaneNodePlug = imagePlanePlug.elementByPhysicalIndex(ips);
         imagePlaneNodePlug.connectedTo(connectedPlugs, true, false, &status);
         MObject resNode = connectedPlugs[0].node(&status);
   
         if (status)
         {
            // get the dependency node of the image plane 
            MFnDependencyNode fnRes(resNode);
   
            // check if the image plane should be created
            bool displayOnlyIfCurrent = fnRes.findPlug("displayOnlyIfCurrent", &status).asBool();
            int displayMode           = fnRes.findPlug("displayMode", &status).asBool();
            
            if ( (!displayOnlyIfCurrent || isRenderingCamera) && ( displayMode > 1 ))
            {
   
               // get data
               MString imageName = fnRes.findPlug("imageName", &status).asString();
               double planeSizeX = fnRes.findPlug("sizeX", &status).asDouble();
               double planeSizeY = fnRes.findPlug("sizeY", &status).asDouble();
               double planeDepth = fnRes.findPlug("depth").asDouble();
               double camFocal = fnDagNode.findPlug("focalLength").asDouble();
               double camScale = fnDagNode.findPlug("cameraScale").asDouble();
               double ipWidth = (planeSizeX * planeDepth) / ((camFocal * 0.0393700787) / camScale);
               double ipHeight = (planeSizeY * planeDepth) / ((camFocal * 0.0393700787) / camScale);
   
               // CREATE PLANE
   
               // get cam's matrix
               AtMatrix translateMatrix;
               GetMatrix(translateMatrix, dagPath);
   
               AtNode* imagePlane = AiNode("polymesh");
               AiNodeSetArray(imagePlane, "nsides", AiArray(1, 1, AI_TYPE_BYTE, 4));
               AiNodeSetArray(imagePlane, "vidxs", AiArray(4, 1, AI_TYPE_UINT, 0, 1, 3, 2));
               AiNodeSetArray(imagePlane, "nidxs", AiArray(4, 1, AI_TYPE_UINT, 0, 1, 2, 3));
               AiNodeSetArray(imagePlane, "uvidxs", AiArray(4, 1, AI_TYPE_UINT, 0, 1, 3, 2));
               AtPoint p1, p2, p3, p4, n1;
               AtPoint2 uv1, uv2, uv3, uv4;
               AiV3Create(p1, -0.5, -0.5, 0.0);
               AiV3Create(p2, 0.5, -0.5, 0.0);
               AiV3Create(p3, -0.5, 0.5, 0.0);
               AiV3Create(p4, 0.5, 0.5, 0.0);
               AiV3Create(n1, 0.0, 0.0, 1.0);
               AiV2Create(uv1, 0, 0);
               AiV2Create(uv2, 1, 0);
               AiV2Create(uv3, 0, 1);
               AiV2Create(uv4, 1, 1);
               AiNodeSetArray(imagePlane, "vlist", AiArray(4, 1, AI_TYPE_POINT, p1, p2, p3, p4));
               AiNodeSetArray(imagePlane, "nlist", AiArray(4, 1, AI_TYPE_VECTOR, n1, n1, n1, n1));
               AiNodeSetArray(imagePlane, "uvlist", AiArray(4, 1, AI_TYPE_POINT2, uv1, uv2, uv3, uv4));
               AiNodeSetInt(imagePlane, "visibility", 65425);
   
               // create a flat shader with the needed image
               AtNode* imagePlaneShader = AiNode("flat");
               AtNode* imagePlaneImage = AiNode("image");
               AiNodeSetStr(imagePlaneImage, "filename", imageName.asChar());
               AiNodeLink(imagePlaneImage, "color", imagePlaneShader);
               AiNodeLink(imagePlaneImage, "opacity", imagePlaneShader);
               AiNodeSetPtr(imagePlane, "shader", imagePlaneShader);
   
               AtMatrix depthMatrix;
               AtMatrix scaleMatrix;
               AtMatrix imagePlaneMatrix;
               AtVector depthVector;
               AtVector scaleVector;
   
               AiV3Create(depthVector, 0.0f, 0.0f, static_cast<float>(-planeDepth));
               AiV3Create(scaleVector, static_cast<float>(ipWidth), static_cast<float>(ipHeight), 1.0f);
               AiM4Translation(depthMatrix, &depthVector);
               AiM4Scaling(scaleMatrix, &scaleVector);
   
               // multiply in order
               AiM4Identity(imagePlaneMatrix);
               AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, scaleMatrix);
               AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, depthMatrix);
               AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, translateMatrix);
               AiNodeSetMatrix(imagePlane, "matrix", imagePlaneMatrix);
            }
         }
      }
   }
}
