#include "AutoCameraTranslator.h"
#include "../NodeTranslatorImpl.h"

AtNode* CAutoCameraTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_impl->m_abstract.arnold.asChar());
}

void CAutoCameraTranslator::Export(AtNode* camera)
{
   const AtNodeEntry* nentry = AiNodeGetNodeEntry(camera);

   static const char* exportedParams[] = {
      "name", "fov", "matrix", 
      "exposure", "near_clip", "far_clip",
      "rolling_shutter", "rolling_shutter_duration", "shutter_start", "shutter_end", 0
   };

   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nentry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      bool skipParam = false;
      for (const char** it = exportedParams; *it != 0; ++it)
      {
         if (strcmp(paramName, *it) == 0)
         {
            skipParam = true;
            break;
         }
      }
      if (!skipParam) ProcessParameter(camera, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);

   const AtParamEntry* pentry = AiNodeEntryLookUpParameter(nentry, "fov");
   if (pentry != 0)
   {
      if (AiParamGetType(pentry) == AI_TYPE_FLOAT)
      {
         m_exportFOV = true;
         m_fovAnimated = false;
      }
      else if (AiParamGetType(pentry) == AI_TYPE_ARRAY)
      {
         const AtParamValue* pvalue = AiParamGetDefault(pentry);
         if (pvalue->ARRAY->type == AI_TYPE_FLOAT)
         {
            m_exportFOV = true;
            m_fovAnimated = true;
         }
      }
   }

   ExportCameraData(camera);
   if (m_exportFOV)
   {
      if (RequiresMotionData() && m_fovAnimated)
      {
         AtArray* fovs = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_FLOAT);
         AiArraySetFlt(fovs, GetMotionStep(), GetFOV(camera));
         AiNodeSetArray(camera, "fov", fovs);
      }
      else
         AiNodeSetFlt(camera, "fov", GetFOV(camera));
   }
}

void CAutoCameraTranslator::ExportMotion(AtNode* camera)
{
   ExportCameraData(camera);
   if (m_fovAnimated)
   {      
      AtArray* fovs = AiNodeGetArray(camera, "fov");
      AiArraySetFlt(fovs, GetMotionStep(), GetFOV(camera));
   }
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
