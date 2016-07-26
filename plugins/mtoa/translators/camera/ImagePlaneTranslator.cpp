#include "ImagePlaneTranslator.h"
#include "CameraTranslator.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"

#include <ai_cameras.h>
#include <ai_constants.h>
#include <ai_msg.h>

#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MImage.h>
#include <maya/MFnTransform.h>
#include <maya/MFnCamera.h>
using namespace std;

void CImagePlaneTranslator::Export(AtNode *imagePlane)
{
   CNodeTranslator::Export(imagePlane);
   ExportImagePlane(0, GetMayaObject());
}
void CImagePlaneTranslator::ExportMotion(AtNode *imagePlane, unsigned int step)
{
   CNodeTranslator::ExportMotion(imagePlane, step);
   ExportImagePlane(step, GetMayaObject());

}
void CImagePlaneTranslator::SetCamera(MString cameraName)
{
   m_camera = cameraName;
   ExportImagePlane(0, GetMayaObject());
}
static void GetCameraRotationMatrix(MDagPath camera, AtMatrix& matrix)
{
   MObject transform = camera.transform();
   MFnTransform mTransform;
   mTransform.setObject(transform);
   MTransformationMatrix mTransformMatrix = mTransform.transformation();

   MMatrix tm = mTransformMatrix.asRotateMatrix();
   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) tm[I][J];
      }
   }
}
static void GetCameraMatrix(MDagPath camera, CArnoldSession *session, AtMatrix& matrix)
{
   MStatus status;
   MMatrix mayaMatrix = camera.inclusiveMatrix(&status);
   if (status)
   {
      if (session)
      {
         MTransformationMatrix trMat = mayaMatrix;
         trMat.addTranslation((-1.0) * session->GetOrigin(), MSpace::kWorld);
         MMatrix copyMayaMatrix = trMat.asMatrix();
         copyMayaMatrix[3][0] = session->ScaleDistance(copyMayaMatrix[3][0]); // is this a copy or a reference?
         copyMayaMatrix[3][1] = session->ScaleDistance(copyMayaMatrix[3][1]);
         copyMayaMatrix[3][2] = session->ScaleDistance(copyMayaMatrix[3][2]);

         for (int J = 0; (J < 4); ++J)
         {
            for (int I = 0; (I < 4); ++I)
            {
               matrix[I][J] = (float) copyMayaMatrix[I][J];
            }
         }
      }
      else
      {
         for (int J = 0; (J < 4); ++J)
         {
            for (int I = 0; (I < 4); ++I)
            {
               matrix[I][J] = (float) mayaMatrix[I][J];
            }
         }  
      }
   }
}


void CImagePlaneTranslator::ExportImagePlane(unsigned int step, MObject& imgPlane)
{
   // get the dependency node of the image plane
   MFnDependencyNode fnRes(imgPlane);
   MStatus status;

   MSelectionList activeList;
   bool validCamera = false;
   
   MDagPath pathCamera;

   if (m_camera.length() > 0)
   {
      activeList.add(m_camera);
      if (activeList.length() > 0)
      {
         activeList.getDagPath(0,pathCamera);         
         if (pathCamera.isValid())
         {
            validCamera = true;
         }
      }
   }
    
   // a camera must have been specified for us to do that  
   double camFocal = 1.f;
   double camScale = 1.f;
   double lensSqueeze = 1.f;

   if (validCamera)
   {
      // If displayOnlyIfCurrent is true and this is not the current camera, do not export Image Plane
      bool displayOnlyIfCurrent = fnRes.findPlug("displayOnlyIfCurrent", &status).asBool();
      MFnCamera fnCamera(pathCamera);
      if(displayOnlyIfCurrent && (GetSession()->GetExportCamera().partialPathName() != fnCamera.partialPathName()))
      {
         return;
      }
      camFocal = fnCamera.findPlug("focalLength").asDouble();
      camScale = fnCamera.findPlug("cameraScale").asDouble();
      lensSqueeze = fnCamera.findPlug("lensSqueezeRatio").asDouble();   
   }

   // check if the image plane should be created
   int displayMode = fnRes.findPlug("displayMode", &status).asInt();
   if (displayMode > 1)
   {
      //MString imagePlaneName = GetMayaPartialPathName();
      MString imagePlaneName = GetMayaNodeName();
      imagePlaneName += fnRes.name();
      //imagePlaneName += "_IP_";
      //imagePlaneName += ips;
      MString imageName;
      MImage mImage;

      //type: 0 == Image, 1 == Texture, 2 == Movie
      int type = fnRes.findPlug("type", &status).asInt();

      if (type == 2)//Not supporting type Movie for now....
      {
         AiMsgWarning("[mtoa] [translator %s] Image Planes of type Movie are unsupported", GetTranslatorName().asChar());
         return;
      }

      double planeSizeX;
      double planeSizeY;

      // UV Values
      double uMin = 0.0f;
      double vMin = 0.0f;
      double uMax = 1.0f;
      double vMax = 1.0f;

      int lockedToCamera = fnRes.findPlug("lockedToCamera", &status).asInt();
      if (lockedToCamera)
      {
         planeSizeX = fnRes.findPlug("sizeX", &status).asDouble();
         planeSizeY = fnRes.findPlug("sizeY", &status).asDouble();
      }
      else
      {
         planeSizeX = fnRes.findPlug("width", &status).asDouble();
         planeSizeY = fnRes.findPlug("height", &status).asDouble();
      }

      double planeDepth = fnRes.findPlug("depth").asDouble();

      double ipWidth;
      double ipHeight;

      unsigned int iWidth = 0;
      unsigned int iHeight = 0;

      if (type == 0)
      {
         imageName = MRenderUtil::exactImagePlaneFileName(imgPlane);
         mImage = MImage();
         mImage.readFromFile(imageName);

         //0:Fill 1:Best 2:Horizontal 3:Vertical 4:ToSize
         mImage.getSize(iWidth, iHeight);
         double iAspect = 1.0;
         if (iWidth > 0 && iHeight > 0)
         {
            iAspect = double(iWidth) / iHeight;
         }
         double planeAspect = (planeSizeX * lensSqueeze) / planeSizeY;
         if (iAspect != planeAspect)
         {
            FitType fit = (FitType)fnRes.findPlug("fit", &status).asInt();

            if (fit == FIT_BEST)
            {
               // fit all of the image in the plane
               if (iAspect > planeAspect)
                  fit = FIT_HORIZONTAL;
               else
                  fit = FIT_VERTICAL;
            }
            else if (fit == FIT_FILL)
            {
               // fill the entire plane with the image
               if (iAspect < planeAspect)
                  fit = FIT_HORIZONTAL;
               else
                  fit = FIT_VERTICAL;
            }

            // in either case we trim the plane to the dimensions of the image, or
            // the image overflows and we adjust the plane's UVs to compensate
            if (fit == FIT_HORIZONTAL)
            {
               // left and right side of image registers with plane
               if (iAspect > planeAspect)
               {
                  // trim the plane vertically to fit image
                  planeSizeY *= (planeAspect / iAspect);
               }
               else
               {
                  // adjust the uv's vertically
                  vMin += ((1.0f - (iAspect / planeAspect)) * 0.5f);
                  vMax -= ((1.0f - (iAspect / planeAspect)) * 0.5f);
               }

            }
            else if (fit == FIT_VERTICAL)
            {
               // top and bottom of image registers with plane
               if (iAspect < planeAspect)
               {
                  // trim the plane horizontally to fit image
                  planeSizeX *= (iAspect / planeAspect);
               }
               else
               {
                  // adjust the uv's horizontally
                  uMin += ((1.0f - (planeAspect / iAspect)) * 0.5f);
                  uMax -= ((1.0f - (planeAspect / iAspect)) * 0.5f);
               }
            }
            else if (fit ==  FIT_TOSIZE)
            {
               // stretch image to fit plane
               double squeezeCorrection = fnRes.findPlug("squeezeCorrection").asDouble();
               if (squeezeCorrection < 0.0001f)
                  squeezeCorrection = 1.0f;
               planeSizeX /= squeezeCorrection;
            }
         }
      }

      if (lockedToCamera)
      {
         ipWidth = (planeSizeX * planeDepth * lensSqueeze) / ((camFocal * MM_TO_INCH) / camScale);
         ipHeight = (planeSizeY * planeDepth) / ((camFocal * MM_TO_INCH) / camScale);
      }
      else
      {
         ipWidth = planeSizeX;
         ipHeight = planeSizeY;
      }

      if (step == 0)
      {
         // CREATE PLANE
         AtNode* imagePlane = GetArnoldNode("polymesh");
         AiNodeSetStr(imagePlane, "name", imagePlaneName.asChar());

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

         /*
         if (type == 0)
         {
            double coverageX = fnRes.findPlug("coverageX", &status).asDouble();
            double coverageY = fnRes.findPlug("coverageY", &status).asDouble();
            double uOffset = 1.0f + (((0.0f - coverageX)/(0.0f - iWidth)) * (0.0f - 1.0f));
            double vOffset = 1.0f + (((0.0f - coverageY)/(0.0f - iHeight)) * (0.0f - 1.0f));

            int coverageOriginX = fnRes.findPlug("coverageOriginX", &status).asInt();
            int coverageOriginY = fnRes.findPlug("coverageOriginY", &status).asInt();

            double uOriginOffset = (((coverageOriginX - 0.0f)/(iWidth - 0.0f)) * (1.0f - 0.0f));
            double vOriginOffset = (((coverageOriginY - 0.0f)/(iHeight - 0.0f)) * (1.0f - 0.0f));

            uMin = uOriginOffset;
            uMax = 1.0f - (uOffset - uOriginOffset);
            if (uMax > 1)
               uMax = 1.0f;

            vMin = vOriginOffset;
            vMax = 1.0f - (vOffset - vOriginOffset);
            if (vMax > 1)
               vMax = 1.0f;
         }
         */
         AiV2Create(uv1, (float)uMin, (float)vMin);
         AiV2Create(uv2, (float)uMax, (float)vMin);
         AiV2Create(uv3, (float)uMin, (float)vMax);
         AiV2Create(uv4, (float)uMax, (float)vMax);

         AiNodeSetArray(imagePlane, "vlist", AiArray(4, 1, AI_TYPE_POINT, p1, p2, p3, p4));
         AiNodeSetArray(imagePlane, "nlist", AiArray(4, 1, AI_TYPE_VECTOR, n1, n1, n1, n1));
         AiNodeSetArray(imagePlane, "uvlist", AiArray(4, 1, AI_TYPE_POINT2, uv1, uv2, uv3, uv4));
         AtByte visibilityFlag = AI_RAY_CAMERA | AI_RAY_DIFFUSE;
         if (fnRes.findPlug("visibleInReflections").asBool())
            visibilityFlag |= AI_RAY_REFLECTED;
         if (fnRes.findPlug("visibleInRefractions").asBool())
            visibilityFlag |= AI_RAY_REFRACTED;
         AiNodeSetByte(imagePlane, "visibility", visibilityFlag);

         // create a flat shader with the needed image
         MPlug colorPlug;
         MPlugArray conn;

         AtNode* imagePlaneShader = GetArnoldRootNode();
         //AtNode* imagePlaneShader = AiNode("flat");
         char nodeName[MAX_NAME_SIZE];
         AiNodeSetStr(imagePlaneShader, "name", NodeUniqueName(imagePlaneShader, nodeName));

         if (type == 0)
         {
            /*
            AtNode* file = AiNode("image");
            AiNodeSetStr(file, "name", NodeUniqueName(file, nodeName));
            AiNodeSetStr(file, "filename", imageName.asChar());
            AiNodeLink(file, "color", imagePlaneShader);
            */
            
            
            AiNodeSetStr(imagePlaneShader, "filename", imageName.asChar());
            AiNodeSetInt(imagePlaneShader, "displayMode", displayMode);
            AiNodeSetPnt2(imagePlaneShader, "coverage", 1.f, 1.f);
            //AiNodeSetPnt2(imagePlaneShader, "translate", coverageOriginX, coverageOriginY);

            colorPlug  = fnRes.findPlug("colorGain");
            colorPlug.connectedTo(conn, true, false);
            if (!conn.length())
              AiNodeSetRGB(imagePlaneShader, "colorGain", colorPlug.child(0).asFloat(), colorPlug.child(1).asFloat(), colorPlug.child(2).asFloat());
            else
            {
               MPlug outputPlug = conn[0];
               ExportNode(outputPlug);
            }

            colorPlug  = fnRes.findPlug("colorOffset");
            colorPlug.connectedTo(conn, true, false);
            if (!conn.length())
               AiNodeSetRGB(imagePlaneShader, "colorOffset", colorPlug.child(0).asFloat(), colorPlug.child(1).asFloat(), colorPlug.child(2).asFloat());
            else
            {
               MPlug outputPlug = conn[0];
               ExportNode(outputPlug);
            }

            float alphaGain = fnRes.findPlug("alphaGain", &status).asFloat();
            AiNodeSetFlt(imagePlaneShader, "alphaGain", alphaGain);
            
         }
         else if (type == 1)
         {
            MPlug colorPlug  = fnRes.findPlug("sourceTexture");
            MPlugArray conn;
            colorPlug.connectedTo(conn, true, false);
            if (conn.length())
            {
               MPlug outputPlug = conn[0];
               AiNodeLink(ExportNode(outputPlug), "color", imagePlaneShader);
            }
         }

         AiNodeSetPtr(imagePlane, "shader", imagePlaneShader);
         AiNodeSetBool(imagePlane, "opaque", 0);

      }// (step == 0)

      AtNode* imagePlane = AiNodeLookUpByName(imagePlaneName.asChar());

      AtMatrix offsetMatrix;
      AtMatrix scaleMatrix;
      AtMatrix rotationMatrix;
      AtMatrix imagePlaneMatrix;
      AtVector offsetVector;
      AtVector scaleVector;

      double offsetX;
      double offsetY;
      double offsetZ;

      //if the plane is locked to the camera find and use the offset values
      //otherwise it isn't locked so use the center attribute values to offset the plane
      if (lockedToCamera)
      {
         offsetX = (fnRes.findPlug("offsetX", &status).asDouble() * planeDepth) / ((camFocal * MM_TO_INCH) / camScale);
         offsetY = (fnRes.findPlug("offsetY", &status).asDouble() * planeDepth) / ((camFocal * MM_TO_INCH) / camScale);
         offsetZ = -planeDepth;
      }
      else
      {
         offsetX = fnRes.findPlug("centerX", &status).asDouble();
         offsetY = fnRes.findPlug("centerY", &status).asDouble();
         offsetZ = fnRes.findPlug("centerZ", &status).asDouble();
      }

      // useless to compute matrices if no camera is provided
      if (validCamera)
      {
         MFnCamera fnCamera(pathCamera);
   
         AiV3Create(offsetVector, static_cast<float>(offsetX), static_cast<float>(offsetY), static_cast<float>(offsetZ));
         AiV3Create(scaleVector, static_cast<float>(ipWidth), static_cast<float>(ipHeight), 1.0f);

         AiM4Translation(offsetMatrix, &offsetVector);
         AiM4Scaling(scaleMatrix, &scaleVector);

         if (lockedToCamera)
         {
            double ipRotate = fnRes.findPlug("rotate", &status).asDouble() * AI_RTOD * -1.0f;
            AiM4RotationZ(rotationMatrix, float(ipRotate));
         }
         else
         {
            //Get the camera's object space rotation matrix
            GetCameraRotationMatrix(pathCamera, rotationMatrix);
         }

         // multiply in order
         AiM4Identity(imagePlaneMatrix);
         AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, scaleMatrix);
         AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, rotationMatrix);
         AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, offsetMatrix);
         //if the imageplane is locked we use the camera's matrix
         if (lockedToCamera)
         {

            // get cam's matrix
            AtMatrix translateMatrix;

            GetCameraMatrix(pathCamera, m_session, translateMatrix);
            AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, translateMatrix);
         }

         // image plane should move with the camera to render it with no motion blur
         if (m_session->IsMotionBlurEnabled(MTOA_MBLUR_CAMERA))
         {
            if (step == 0)
            {
               AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
               AiArraySetMtx(matrices, 0, imagePlaneMatrix);
               AiNodeSetArray(imagePlane, "matrix", matrices);
            }
            else
            {
               AtArray* matrices = AiNodeGetArray(imagePlane, "matrix");
               AiArraySetMtx(matrices, step, imagePlaneMatrix);
            }
         }
         else
         {
            AiNodeSetMatrix(imagePlane, "matrix", imagePlaneMatrix);
         }
      }
   }
}


void CImagePlaneTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "MayaImagePlane");
   CAttrData data;
   data.defaultValue.BOOL = true;
   data.name = "aiAutoTx";
   data.shortName = "autotx";
   helper.MakeInputBoolean(data);
}

AtNode*  CImagePlaneTranslator::CreateArnoldNodes()
{
   AddArnoldNode("polymesh", "polymesh");
   return AddArnoldNode("MayaImagePlane");
}
