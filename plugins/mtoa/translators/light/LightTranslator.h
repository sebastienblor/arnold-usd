#ifndef LIGHTTRANSLATOR_H
#define LIGHTTRANSLATOR_H

#include "translators/NodeTranslator.h"

class DLLEXPORT CLightTranslator
   :   public CDagTranslator
{
public:

   virtual AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      CDagTranslator::Init(dagPath, scene, outputAttr);
      m_motion = scene->IsLightMotionBlurEnabled();
      return m_atNode;
   }
   bool RequiresMotionData()
   {
      return m_motion;
   }
protected:
   virtual void Export(AtNode* light);
   virtual void ExportMotion(AtNode* light, AtUInt step);
   virtual void ExportLightFilters(AtNode* light, const MObjectArray &filters);
   virtual void Delete();
   static void MakeCommonAttributes(CBaseAttrHelper& helper);

protected:
   bool m_motion;
};

#endif // LIGHTTRANSLATOR_H
