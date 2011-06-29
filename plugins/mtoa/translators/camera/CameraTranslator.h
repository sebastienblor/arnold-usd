#ifndef ARNOLDCAMERATRANSLATOR_H
#define ARNOLDCAMERATRANSLATOR_H

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
   virtual AtNode* Init(CMayaScene* scene, MDagPath& dagPath, MString outputAttr="")
   {
      m_atNode = CDagTranslator::Init(scene, dagPath, outputAttr);
      m_motion = scene->IsMotionBlurEnabled(MTOA_MBLUR_CAMERA);
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

#endif // ARNOLDCAMERATRANSLATOR_H
