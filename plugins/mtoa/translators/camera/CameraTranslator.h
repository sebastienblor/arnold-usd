#ifndef CAMERA_TRANSLATOR_H
#define CAMERA_TRANSLATOR_H

#include "translators/NodeTranslator.h"

#include <maya/MFnCamera.h>

const double MM_TO_INCH = 0.03937;

enum FitType
{
   FIT_FILL = 0,
   FIT_BEST,
   FIT_HORIZONTAL,
   FIT_VERTICAL,
   FIT_TOSIZE,
};

class DLLEXPORT CCameraTranslator
   :   public CDagTranslator
{
public:
   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      m_atNode = CDagTranslator::Init(session, dagPath, outputAttr);
      m_fnCamera.setObject(dagPath);
      return m_atNode;
   }
   
   // FIXME: this method shouldn't be required.
   virtual bool RequiresMotionData()
   {
      return m_session->IsMotionBlurEnabled(MTOA_MBLUR_CAMERA);
   }

protected:
   bool IsOrtho();
   double GetDeviceAspect();
   void SetFilmTransform(AtNode* camera, double factorX=0, double factorY=0, double width=0, bool persp=true);
   void ExportImagePlanes(unsigned int step);
   void ExportImagePlane(unsigned int step, MObject& imgPlane);
   void ExportDOF(AtNode* camera);
   void ExportCameraData(AtNode* camera);
   void ExportCameraMBData(AtNode* camera, unsigned int step);
   static void MakeDefaultAttributes(CExtensionAttrHelper &helper);
   static void MakeDOFAttributes(CExtensionAttrHelper &helper);

protected:
   MFnCamera m_fnCamera;
};

#endif // CAMERA_TRANSLATOR_H
