
#include "Cameras.h"
#include "utils/AttrHelper.h"

#include <ai_cameras.h>
#include <ai_constants.h>

#include <maya/MFnCamera.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnNumericAttribute.h>

using namespace std;

double MM_TO_INCH = 0.03937;


void CCameraTranslator::ExportImagePlane(AtUInt step)
{
   MPlugArray connectedPlugs;
   MPlug      imagePlanePlug;
   MPlug      imagePlaneNodePlug;
   MStatus    status;

   // first we get the image planes connected to this camera
   imagePlanePlug = m_fnNode.findPlug("imagePlane");

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
            int displayMode           = fnRes.findPlug("displayMode", &status).asInt();
            if ( displayMode > 1 )
            {

               MString imagePlaneName(m_fnNode.partialPathName());
               imagePlaneName += "_IP_";
               imagePlaneName += ips;
               double planeSizeX = fnRes.findPlug("sizeX", &status).asDouble();
               double planeSizeY = fnRes.findPlug("sizeY", &status).asDouble();
               double planeDepth = fnRes.findPlug("depth").asDouble();
               double camFocal = m_fnNode.findPlug("focalLength").asDouble();
               double camScale = m_fnNode.findPlug("cameraScale").asDouble();
               double ipWidth = (planeSizeX * planeDepth) / ((camFocal * 0.0393700787) / camScale);
               double ipHeight = (planeSizeY * planeDepth) / ((camFocal * 0.0393700787) / camScale);

               if ( (!m_motion) || (step == 0))
               {
                  // get data
                  MString imageName;
                  MString frameNumber("0");
                  frameNumber += m_scene->GetCurrentFrame() + fnRes.findPlug("frameOffset").asInt();
                  imageName = MRenderUtil::exactImagePlaneFileName(resNode);
                  imageName = MRenderUtil::exactFileTextureName(imageName, fnRes.findPlug("useFrameExtension").asBool(), frameNumber);

                  // CREATE PLANE
                  AtNode* imagePlane = AiNode("polymesh");
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
               }

               AtNode* imagePlane = AiNodeLookUpByName(imagePlaneName.asChar());

               AtMatrix depthMatrix;
               AtMatrix scaleMatrix;
               AtMatrix imagePlaneMatrix;
               AtVector depthVector;
               AtVector scaleVector;

               AiV3Create(depthVector, 0.0f, 0.0f, static_cast<float>(-planeDepth));
               AiV3Create(scaleVector, static_cast<float>(ipWidth), static_cast<float>(ipHeight), 1.0f);
               AiM4Translation(depthMatrix, &depthVector);
               AiM4Scaling(scaleMatrix, &scaleVector);

               // get cam's matrix
               AtMatrix translateMatrix;
               GetMatrix(translateMatrix);

               // multiply in order
               AiM4Identity(imagePlaneMatrix);
               AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, scaleMatrix);
               AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, depthMatrix);
               AiM4Mult(imagePlaneMatrix, imagePlaneMatrix, translateMatrix);

               // image plane should move with the camera to render it with no motion blur
               if (m_motion)
               {
                  if (step == 0)
                  {
                     AtArray* matrices = AiArrayAllocate(1, m_scene->GetNumMotionSteps(), AI_TYPE_MATRIX);
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
   }
}

void CCameraTranslator::ExportCameraData(AtNode* camera)
{
   AtMatrix matrix;

   AiNodeSetStr(camera, "name", m_fnNode.partialPathName().asChar());

   AiNodeSetFlt(camera, "near_clip", m_fnNode.findPlug("nearClipPlane").asFloat());
   AiNodeSetFlt(camera, "far_clip", m_fnNode.findPlug("farClipPlane").asFloat());
   
   if ( m_fnNode.findPlug("enableDOF").asBool())
   {
      AiNodeSetFlt(camera, "focal_distance", m_fnNode.findPlug("focal_distance").asFloat());
      AiNodeSetFlt(camera, "aperture_size", m_fnNode.findPlug("aperture_size").asFloat());
      AiNodeSetInt(camera, "aperture_blades", m_fnNode.findPlug("aperture_blades").asInt());
      AiNodeSetFlt(camera, "aperture_rotation", m_fnNode.findPlug("aperture_rotation").asFloat());
      AiNodeSetFlt(camera, "aperture_blade_curvature", m_fnNode.findPlug("aperture_blade_curvature").asFloat());
   }
   
   if (m_scene->IsMotionBlurEnabled())
   {
      float halfShutter = m_scene->GetShutterSize() * 0.5f;
      AiNodeSetFlt(camera, "shutter_start", 0.5f - halfShutter);
      AiNodeSetFlt(camera, "shutter_end", 0.5f + halfShutter);
      AiNodeSetInt(camera, "shutter_type", m_scene->GetShutterType());
   }

   GetMatrix(matrix);
   
   if (m_motion)
   {
      AtArray* matrices = AiArrayAllocate(1, m_scene->GetNumMotionSteps(), AI_TYPE_MATRIX);
      AiArraySetMtx(matrices, 0, matrix);
      AiNodeSetArray(camera, "matrix", matrices);
   }
   else
   {
      AiNodeSetMatrix(camera, "matrix", matrix);
   }
}

void CCameraTranslator::ExportCameraMBData(AtNode *camera, AtUInt step)
{
   AtMatrix matrix;
   GetMatrix(matrix);

   AtArray* matrices = AiNodeGetArray(camera, "matrix");
   AiArraySetMtx(matrices, step, matrix);
}

double CCameraTranslator::GetDeviceAspect()
{
   MStatus        status;
   MSelectionList list;
   MObject        node;
   double deviceAspect = 0;

   list.add("defaultRenderGlobals");
   list.getDependNode(0, node);
   MFnDependencyNode fnRenderGlobals(node);

   MPlugArray connectedPlugs;
   MPlug      resPlug = fnRenderGlobals.findPlug("resolution");

   resPlug.connectedTo(connectedPlugs,
                       true,  // asDestination
                       false, // asSource
                       &status);

   // Must be length 1 or we would have fan-in
   if (status && (connectedPlugs.length() == 1))
   {
      MObject resNode = connectedPlugs[0].node(&status);

      if (status)
      {
         MFnDependencyNode fnRes(resNode);
         deviceAspect = fnRes.findPlug("deviceAspectRatio").asFloat();
      }
   }

   m_cameraData.deviceAspect = deviceAspect;
   return deviceAspect;
}

void CCameraTranslator::ExportOrthoFilmback(AtNode* camera)
{
   double deviceAspect = GetDeviceAspect();

   double width = m_fnCamera.orthoWidth();

   MFnCamera::FilmFit filmFit = m_fnCamera.filmFit();
   
   //FILL FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kFillFilmFit){
      if(deviceAspect >= 1.0)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //OVERSCAN FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kOverscanFilmFit){
      if(deviceAspect <= 1.0)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   if(filmFit == MFnCamera::kHorizontalFilmFit)
   {
      width = width/2;
   }
   else if(filmFit == MFnCamera::kVerticalFilmFit)
   {
      width = (width/2)*deviceAspect;
   }
   
   MVectorArray filmTransforms = GetFilmTransform(width, false);
   
   m_cameraData.width = width;
   m_cameraData.factorX = filmTransforms[0].x;
   m_cameraData.factorY = filmTransforms[0].y;
   m_cameraData.focalLength = 35;

   AiNodeSetPnt2(camera, "screen_window_min", static_cast<float>(filmTransforms[0].x), static_cast<float>(filmTransforms[0].y));
   AiNodeSetPnt2(camera, "screen_window_max", static_cast<float>(filmTransforms[1].x), static_cast<float>(filmTransforms[1].y));
}

void CCameraTranslator::ExportPerspFilmback(AtNode* camera)
{

   double deviceAspect = GetDeviceAspect();
   float fov = 1.0f;
   double cameraAspect = m_fnCamera.aspectRatio();
   double lensSqueeze = m_fnCamera.lensSqueezeRatio();
   double focalLength = m_fnCamera.focalLength();
   double cameraScale = m_fnCamera.cameraScale();
   
   double apertureX = m_fnCamera.horizontalFilmAperture();
   double apertureY = m_fnCamera.verticalFilmAperture();
   
   double deviceApertureX = apertureX;
   double deviceApertureY = apertureY;
   
   double filmFitOffset = m_fnCamera.filmFitOffset();
   double filmOffsetX = m_fnCamera.horizontalFilmOffset();
   double filmOffsetY = m_fnCamera.verticalFilmOffset();
   double factorX = 0.0;
   double factorY = 0.0;

   MFnCamera::FilmFit filmFit = m_fnCamera.filmFit();
   
   //FILL FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kFillFilmFit){
      filmFitOffset = 0;
      if((cameraAspect * lensSqueeze) < deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //OVERSCAN FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kOverscanFilmFit){
      filmFitOffset = 0;
      if((cameraAspect * lensSqueeze) > deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //HORIZONTAL FILM RESOLUTION GATE
   if(filmFit == MFnCamera::kHorizontalFilmFit)
   {
      //Find the camera's Field Of View with Lens Squeeze and Camera Scale applied
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * apertureX * lensSqueeze) / ((focalLength / cameraScale) * MM_TO_INCH)));
      //Find the new vertical aperture based off of the Render Aspect Ratio
      deviceApertureY = apertureX / (deviceAspect / lensSqueeze);

      //Apply the film fit offset if the camera's aspect ratio is greater than the render aspect ratio
      if((cameraAspect * lensSqueeze) > deviceAspect && filmFitOffset != 0.0f)
      {
         factorY = ((apertureY - deviceApertureY) * filmFitOffset) / deviceApertureY;
      }
   }

   //VERTICAL FILM RESOLUTION GATE
   else if(filmFit == MFnCamera::kVerticalFilmFit)
   {
      //Find the camera's Field Of View with the Render Aspect Ratio and Camera Scale applied
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * (deviceAspect * apertureY)) / ((focalLength / cameraScale) * MM_TO_INCH)));
      //Find the new horizontal aperture based off of the Render Aspect Ratio
      deviceApertureX = apertureY * (deviceAspect / lensSqueeze);
      
      //Apply the film fit offset if the camera's aspect ratio is less than the render aspect ratio
      if((cameraAspect * lensSqueeze) < deviceAspect && filmFitOffset != 0.0f)
      {
         factorX = ((apertureX - deviceApertureX) * filmFitOffset) / deviceApertureX;
      }
   }
   
   //If we have any filmOffsets we add them here
   if(filmOffsetX!=0.0f || filmOffsetY!=0.0f)
   {
      factorX += (filmOffsetX/deviceApertureX)*2;
      factorY += (filmOffsetY/deviceApertureY)*2;
   }
   
   MVectorArray filmTransforms = GetFilmTransform();
   MVector minPoint = filmTransforms[0];
   MVector maxPoint = filmTransforms[1];

   //Add on any offsets from filmOffsetX or Y, or filmFitOffset
   minPoint += MVector(factorX, factorY);
   maxPoint += MVector(factorX, factorY);

   AiNodeSetPnt2(camera, "screen_window_min", static_cast<float>(minPoint.x), static_cast<float>(minPoint.y));
   AiNodeSetPnt2(camera, "screen_window_max", static_cast<float>(maxPoint.x), static_cast<float>(maxPoint.y));
   
   m_cameraData.apertureX = deviceApertureX;
   m_cameraData.apertureY = deviceApertureY;
   m_cameraData.fov = fov;
   m_cameraData.scale = cameraScale;
   m_cameraData.focalLength = focalLength;
   m_cameraData.factorX = factorX;
   m_cameraData.factorY = factorY;
   m_cameraData.lensSqueeze = lensSqueeze;

   //return fov;
}

MVectorArray CCameraTranslator::GetFilmTransform(double width, bool persp)
{
   double deviceAspect = GetDeviceAspect();
   double cameraAspect = m_fnCamera.aspectRatio();
   double preScale = m_fnCamera.preScale();
   double filmTranslateX = m_fnCamera.filmTranslateH();
   double filmTranslateY = m_fnCamera.filmTranslateV();
   double filmRollValue = m_fnCamera.filmRollValue();
   //We need a roll attribute from the guys at SolidAngle
   //double filmRollPivotX = m_fnCamera.verticalRollPivot();
   //double filmRollPivotY = m_fnCamera.horizontalRollPivot();
   MFnCamera::RollOrder filmRollOrder = m_fnCamera.filmRollOrder();
   double postScale = m_fnCamera.postScale();

   //2D Transform default Vectors for Perspective
   MVector minPoint(-1, -1);
   MVector maxPoint(1, 1);

   if (persp) // perspective camera
   {
      preScale = 1 / preScale;
      postScale = 1 / postScale;

      filmTranslateX = (filmTranslateX * preScale) / postScale;
      if(cameraAspect > deviceAspect)
         filmTranslateY = ((filmTranslateY/cameraAspect) * 2 * preScale) / postScale;
      else
         filmTranslateY = (filmTranslateY * deviceAspect * preScale) / postScale;
   }
   else //Ortho camera
   {
      preScale = 1 / preScale;
      postScale = 1 / postScale;
      double orthoWidth = m_fnCamera.orthoWidth() / 2;
      
      if(orthoWidth == width)//We are in Horizontal Mode
      {
         filmTranslateX = (filmTranslateX * orthoWidth * preScale) / postScale;
         filmTranslateY = (filmTranslateY * orthoWidth * deviceAspect * preScale) / postScale;
      }
      else//Vertical mode
      {
         filmTranslateX = (filmTranslateX * orthoWidth * deviceAspect * preScale) / postScale;
         filmTranslateY = (filmTranslateY * orthoWidth * deviceAspect * deviceAspect * preScale) / postScale;
      }

      //Set the point to the width passed in for Ortho mode.
      minPoint = MVector(-width, -width);
      maxPoint = MVector(width, width);
   }
   
   if(preScale != 1.0f)
   {
      minPoint *= preScale;
      maxPoint *= preScale;
   }

   if(filmRollOrder == MFnCamera::kTranslateRotate)
   {
      if(filmTranslateX != 0.0f || filmTranslateY != 0.0f)
      {
         minPoint += MVector(filmTranslateX, filmTranslateY);
         maxPoint += MVector(filmTranslateX, filmTranslateY);
      }
      if(filmRollValue != 0.0f)
      {
         //ROTATE
      }
   }
   else //Rotate-Translate order
   {
      if(filmRollValue != 0.0f)
      {
         //ROTATE
      }
      if(filmTranslateX != 0.0f || filmTranslateY != 0.0f)
      {
         minPoint += MVector(filmTranslateX, filmTranslateY);
         maxPoint += MVector(filmTranslateX, filmTranslateY);
      }
   }

   if(postScale != 1.0f)
   {
      minPoint *= postScale;
      maxPoint *= postScale;
   }

   MVector src[] = {minPoint, maxPoint};
   return MVectorArray(src, 2);
}

// Orthographic Camera
//
void CCameraTranslator::ExportOrtho(AtNode* camera)
{
   ExportCameraData(camera);
   ExportOrthoFilmback(camera);
   ExportImagePlane(0);
}

void CCameraTranslator::ExportOrthoMotion(AtNode* camera, AtInt step)
{
   ExportCameraMBData(camera, step);
   ExportImagePlane(step);
}

// Perspective Camera
//
void CCameraTranslator::ExportPersp(AtNode* camera)
{
   ExportPerspFilmback(camera);
   float fov = m_cameraData.fov;

   ExportCameraData(camera);
   ExportImagePlane(0);

   if (m_motion)
   {
      AtArray* fovs = AiArrayAllocate(1, m_scene->GetNumMotionSteps(), AI_TYPE_FLOAT);
      AiArraySetFlt(fovs, 0, fov);
      AiNodeSetArray(camera, "fov", fovs);
   }
   else
   {
      AiNodeSetFlt(camera, "fov", fov);
   }
}

void CCameraTranslator::ExportPerspMotion(AtNode* camera, AtInt step)
{
   ExportCameraMBData(camera, step);
   ExportImagePlane(step);

   AtArray* fovs = AiNodeGetArray(camera, "fov");
   AiArraySetFlt(fovs, step, m_cameraData.fov);
}

AtNode* CCameraTranslator::Export()
{
   AtNode* camera;
   if (m_fnCamera.isOrtho())
   {
      camera = AiNode("ortho_camera");
      ExportOrtho(camera);
   }
   else
   {
      camera = AiNode("persp_camera");
      ExportPersp(camera);
   }
   return camera;
}

void CCameraTranslator::Update(AtNode* camera)
{
   if (m_fnCamera.isOrtho())
   {
      ExportOrtho(camera);
   }
   else
   {
      ExportPersp(camera);
   }
}

void CCameraTranslator::ExportMotion(AtNode* camera, AtUInt step)
{
   if (m_fnCamera.isOrtho())
   {
      return ExportOrthoMotion(camera, step);
   }
   else
   {
      return ExportPerspMotion(camera, step);
   }
}

void CCameraTranslator::NodeInitializer(MObject& node)
{
   MFnNumericAttribute nAttr;
   MObject attr = nAttr.create("enableDOF", "edof", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   MFnDependencyNode fnNode = MFnDependencyNode(node);
   fnNode.addAttribute(attr);

   CDynamicAttrHelper* helper = new CDynamicAttrHelper(node, "persp_camera");
   helper->MakeInput("focal_distance");
   helper->MakeInput("aperture_size");
   helper->MakeInput("aperture_blades");
   helper->MakeInput("aperture_blade_curvature");
   helper->MakeInput("aperture_rotation");
   delete helper;
}
