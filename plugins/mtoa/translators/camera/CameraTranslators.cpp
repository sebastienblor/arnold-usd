#include "CameraTranslators.h"

#include <maya/MPlugArray.h>

using namespace std;

// Orthographic Camera
//

AtNode*  COrthoCameraTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("ortho_camera");
}


void COrthoCameraTranslator::ExportFilmback(AtNode* camera)
{
   double deviceAspect = GetDeviceAspect();

   double width = m_fnCamera.orthoWidth();

   MFnCamera::FilmFit filmFit = m_fnCamera.filmFit();
   
   //FILL FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kFillFilmFit){
      if (deviceAspect >= 1.0)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //OVERSCAN FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kOverscanFilmFit){
      if (deviceAspect <= 1.0)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   if (filmFit == MFnCamera::kHorizontalFilmFit)
   {
      width = width/2;
   }
   else if (filmFit == MFnCamera::kVerticalFilmFit)
   {
      width = (width/2)*deviceAspect;
   }
   
   SetFilmTransform(camera, 0, 0, width, false);
}

void COrthoCameraTranslator::Export(AtNode* camera)
{
   ExportCameraData(camera);
   ExportFilmback(camera);
   ExportImagePlanes(0);
}

void COrthoCameraTranslator::ExportMotion(AtNode* camera, AtUInt step)
{
   ExportCameraMBData(camera, step);
   ExportImagePlanes(step);
}

void COrthoCameraTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "ortho_camera");
   MakeDefaultAttributes(helper);
}

// Perspective Camera
//
AtNode*  CPerspCameraTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("persp_camera");
}


float CPerspCameraTranslator::ExportFilmback(AtNode* camera)
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
   bool   shakeEnabled = m_fnCamera.shakeEnabled();
   double shakeX = m_fnCamera.horizontalShake();
   double shakeY = m_fnCamera.verticalShake();
   double factorX = 0.0;
   double factorY = 0.0;

   MFnCamera::FilmFit filmFit = m_fnCamera.filmFit();
   
   //FILL FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kFillFilmFit){
      filmFitOffset = 0;
      if ((cameraAspect * lensSqueeze) < deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //OVERSCAN FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kOverscanFilmFit){
      filmFitOffset = 0;
      if ((cameraAspect * lensSqueeze) > deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   //HORIZONTAL FILM RESOLUTION GATE
   if (filmFit == MFnCamera::kHorizontalFilmFit)
   {
      //Find the camera's Field Of View with Lens Squeeze and Camera Scale applied
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * apertureX * lensSqueeze) / ((focalLength / cameraScale) * MM_TO_INCH)));
      //Find the new vertical aperture based off of the Render Aspect Ratio
      deviceApertureY = apertureX / (deviceAspect / lensSqueeze);

      //Apply the film fit offset if the camera's aspect ratio is greater than the render aspect ratio
      if ((cameraAspect * lensSqueeze) > deviceAspect && filmFitOffset != 0.0f)
      {
         factorY = ((apertureY - deviceApertureY) * filmFitOffset) / deviceApertureY;
      }
   }

   //VERTICAL FILM RESOLUTION GATE
   else if (filmFit == MFnCamera::kVerticalFilmFit)
   {
      //Find the camera's Field Of View with the Render Aspect Ratio and Camera Scale applied
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * (deviceAspect * apertureY)) / ((focalLength / cameraScale) * MM_TO_INCH)));
      //Find the new horizontal aperture based off of the Render Aspect Ratio
      deviceApertureX = apertureY * (deviceAspect / lensSqueeze);

      //Apply the film fit offset if the camera's aspect ratio is less than the render aspect ratio
      if ((cameraAspect * lensSqueeze) < deviceAspect && filmFitOffset != 0.0f)
      {
         factorX = ((apertureX - deviceApertureX) * filmFitOffset) / deviceApertureX;
      }
   }
   
   //If we have camera shake we add it here
   if (shakeEnabled)
   {
      filmOffsetX += shakeX;
      filmOffsetY += shakeY;
   }

   //If we have any filmOffsets we add them here
   if (filmOffsetX!=0.0f || filmOffsetY!=0.0f)
   {
      factorX += (filmOffsetX/deviceApertureX)*2;
      factorY += (filmOffsetY/deviceApertureY)*2;
   }
   
   SetFilmTransform(camera, factorX, factorY);

   return fov;
}


void CPerspCameraTranslator::Export(AtNode* camera)
{
   float fov = ExportFilmback(camera);

   ExportCameraData(camera);
   ExportDOF(camera);
   ExportImagePlanes(0);

   // UV Remap export
   MObject uvRemapNode;
   MPlugArray conns;
   MPlug pUVR = FindMayaObjectPlug("aiUvRemap");
   pUVR.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      uvRemapNode = conns[0].node();
   }
   else
   {
      uvRemapNode = MObject::kNullObj;
   }
   if (!uvRemapNode.isNull())
   {
      AiNodeLink(ExportNode(conns[0]), "uv_remap", camera);
   }
   else
   {
      ProcessParameter(camera, "uv_remap", AI_TYPE_RGBA, pUVR);
   }

   if (RequiresMotionData())
   {
      AtArray* fovs = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_FLOAT);
      AiArraySetFlt(fovs, 0, fov);
      AiNodeSetArray(camera, "fov", fovs);
   }
   else
   {
      AiNodeSetFlt(camera, "fov", fov);
   }
}

void CPerspCameraTranslator::ExportMotion(AtNode* camera, AtUInt step)
{
   // FIXME: fov can be animated, but ExportFilmback currently calculates and sets screen_min and screen_max
   // which we don't want to do at each step
   float fov = ExportFilmback(camera);
   ExportCameraMBData(camera, step);
   ExportImagePlanes(step);

   AtArray* fovs = AiNodeGetArray(camera, "fov");
   AiArraySetFlt(fovs, step, fov);
}

void CPerspCameraTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "persp_camera");
   MakeDefaultAttributes(helper);
   MakeDOFAttributes(helper);
   helper.MakeInput("uv_remap");
}


// Fish Eye Camera
//
AtNode*  CFishEyeCameraTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("fisheye_camera");
}

// returns FOV
float CFishEyeCameraTranslator::ExportFilmback(AtNode* camera)
{
   // FIXME: export the screen_min and screen_max
   SetFilmTransform(camera);
   return FindMayaObjectPlug("aiFov").asFloat();
}

void CFishEyeCameraTranslator::Export(AtNode* camera)
{
   float fov = ExportFilmback(camera);

   ExportCameraData(camera);
   ExportDOF(camera);
   ExportImagePlanes(0);

   MPlug plug = FindMayaObjectPlug("aiAutocrop");
   AiNodeSetBool(camera, "autocrop", plug.asBool());

   //plug = FindMayaObjectPlug("aiFiltermap");
   //AiNodeSetRGB(camera, "aiFiltermap", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());

   if (RequiresMotionData())
   {
      AtArray* fovs = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_FLOAT);
      AiArraySetFlt(fovs, 0, fov);
      AiNodeSetArray(camera, "fov", fovs);
   }
   else
   {
      AiNodeSetFlt(camera, "fov", fov);
   }
}

void CFishEyeCameraTranslator::ExportMotion(AtNode* camera, AtUInt step)
{
   float fov = ExportFilmback(camera);

   ExportCameraMBData(camera, step);
   ExportImagePlanes(step);

   AtArray* fovs = AiNodeGetArray(camera, "fov");
   AiArraySetFlt(fovs, step, fov);
}

void CFishEyeCameraTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "fisheye_camera");
   MakeDefaultAttributes(helper);
   MakeDOFAttributes(helper);

   helper.MakeInput("fov");
   helper.MakeInput("autocrop");
   helper.MakeInput("filtermap");
}

// Cyl Camera
//
AtNode*  CCylCameraTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("cyl_camera");
}

// returns FOV
void CCylCameraTranslator::ExportFilmback(AtNode* camera, float fovs[])
{
   // FIXME: export the screen_min and screen_max
   fovs[0] = FindMayaObjectPlug("aiHorizontalFov").asFloat();
   fovs[1] = FindMayaObjectPlug("aiVerticalFov").asFloat();
   SetFilmTransform(camera);
}

void CCylCameraTranslator::Export(AtNode* camera)
{
   float fovs[2];
   ExportFilmback(camera, fovs);

   ExportCameraData(camera);
   ExportDOF(camera);
   ExportImagePlanes(0);

   MPlug plug = FindMayaObjectPlug("aiProjective");
   AiNodeSetBool(camera, "projective", plug.asBool());

   if (RequiresMotionData())
   {
      AtArray* h_fovs = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_FLOAT);
      AtArray* v_fovs = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_FLOAT);
      AiArraySetFlt(h_fovs, 0, fovs[0]);
      AiArraySetFlt(v_fovs, 0, fovs[1]);
      AiNodeSetArray(camera, "horizontal_fov", h_fovs);
      AiNodeSetArray(camera, "vertical_fov", v_fovs);
   }
   else
   {
      AiNodeSetFlt(camera, "horizontal_fov", fovs[0]);
      AiNodeSetFlt(camera, "vertical_fov", fovs[1]);
   }
}

void CCylCameraTranslator::ExportMotion(AtNode* camera, AtUInt step)
{
   float fovs[2];
   ExportFilmback(camera, fovs);

   ExportCameraMBData(camera, step);
   ExportImagePlanes(step);

   AtArray* h_fovs = AiNodeGetArray(camera, "horizontal_fov");
   AiArraySetFlt(h_fovs, step, fovs[0]);
   
   AtArray* v_fovs = AiNodeGetArray(camera, "vertical_fov");
   AiArraySetFlt(v_fovs, step, fovs[1]);
}

void CCylCameraTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "cyl_camera");
   MakeDefaultAttributes(helper);
   helper.MakeInput("horizontal_fov");
   helper.MakeInput("vertical_fov");
   helper.MakeInput("projective");
}
