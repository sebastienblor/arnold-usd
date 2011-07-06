#ifndef CAMERA_TRANSLATOR_H
#define CAMERA_TRANSLATOR_H

#include "translators/NodeTranslator.h"

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
   virtual AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      m_atNode = CDagTranslator::Init(dagPath, scene, outputAttr);
      m_motion = scene->IsCameraMotionBlurEnabled();
      m_fnCamera.setObject(dagPath);
      return m_atNode;
   }
   
   // FIXME: this method shouldn't be required.
   virtual bool RequiresMotionData()
   {
      return m_motion;
   }

protected:
   double GetDeviceAspect();
   void SetFilmTransform(AtNode* camera, double factorX=0, double factorY=0, double width=0, bool persp=true);
   void ExportImagePlanes(AtUInt step);
   void ExportImagePlane(AtUInt step, MObject& imgPlane);
   void ExportDOF(AtNode* camera);
   void ExportCameraData(AtNode* camera);
   void ExportCameraMBData(AtNode* camera, AtUInt step);
   static void MakeDefaultAttributes(CExtensionAttrHelper &helper);
   static void MakeDOFAttributes(CExtensionAttrHelper &helper);

protected:
   bool m_motion;
   MFnCamera m_fnCamera;
};

#endif // CAMERA_TRANSLATOR_H
