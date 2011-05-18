#ifndef ARNOLDLIGHTTRANSLATOR_H
#define ARNOLDLIGHTTRANSLATOR_H

#include "translators/NodeTranslator.h"

class DLLEXPORT CArnoldLightTranslator
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
   static void NodeInitializer(MString nodeClassName);
protected:
   virtual void Export(AtNode* light, bool mayaAttrs=true);
   virtual void ExportMotion(AtNode* light, AtUInt step);
   virtual void ExportLightFilters(AtNode* light, const MObjectArray &filters);
   virtual void Delete();

protected:
   bool m_motion;
};

#endif // ARNOLDLIGHTTRANSLATOR_H
