#include "AutoCameraTranslator.h"

AtNode* CAutoCameraTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_abstract.arnold.asChar());
}

void CAutoCameraTranslator::Export(AtNode* camera)
{
   CNodeTranslator::Export(camera);
   ExportCameraData(camera);
   if (RequiresMotionData())
   {
      AtArray* fovs = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_FLOAT);
      AiArraySetFlt(fovs, 0, GetFOV(camera));
      AiNodeSetArray(camera, "fov", fovs);
   }
   else
      AiNodeSetFlt(camera, "fov", GetFOV(camera));
}

void CAutoCameraTranslator::ExportMotion(AtNode* camera, unsigned int step)
{
   ExportCameraMBData(camera, step);
   AtArray* fovs = AiNodeGetArray(camera, "fov");
   AiArraySetFlt(fovs, step, GetFOV(camera));
}

float CAutoCameraTranslator::GetFOV(AtNode* camera)
{
   double deviceAspect = GetDeviceAspect();
   float fov = 1.0f;
   double cameraAspect = m_fnCamera.aspectRatio();
   double lensSqueeze = m_fnCamera.lensSqueezeRatio();
   double focalLength = m_fnCamera.focalLength();
   double cameraScale = m_fnCamera.cameraScale();
   
   double apertureX = m_fnCamera.horizontalFilmAperture();
   double apertureY = m_fnCamera.verticalFilmAperture();
      
   MFnCamera::FilmFit filmFit = m_fnCamera.filmFit();
   
   if (filmFit == MFnCamera::kFillFilmFit)
   {
      if ((cameraAspect * lensSqueeze) < deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   if (filmFit == MFnCamera::kOverscanFilmFit)
   {
      if ((cameraAspect * lensSqueeze) > deviceAspect)
         filmFit = MFnCamera::kHorizontalFilmFit;
      else
         filmFit = MFnCamera::kVerticalFilmFit;
   }

   if (filmFit == MFnCamera::kHorizontalFilmFit)
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * apertureX * lensSqueeze) / ((focalLength / cameraScale) * MM_TO_INCH)));
   else if (filmFit == MFnCamera::kVerticalFilmFit)
      fov = static_cast<float>(AI_RTOD * 2 * atan((0.5 * (deviceAspect * apertureY)) / ((focalLength / cameraScale) * MM_TO_INCH)));

   return fov;
}