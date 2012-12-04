#ifndef LIGHT_TRANSLATOR_H
#define LIGHT_TRANSLATOR_H

#include "translators/NodeTranslator.h"

class DLLEXPORT CLightTranslator
   :   public CDagTranslator
{
public:

   virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      CDagTranslator::Init(session, dagPath, outputAttr);
      return m_atNode;
   }
   bool RequiresMotionData()
   {
      return m_session->IsMotionBlurEnabled(MTOA_MBLUR_LIGHT);
   }
protected:
   virtual bool IsMayaTypeLight() { return true; }
   virtual void Export(AtNode* light);
   virtual void ExportMotion(AtNode* light, unsigned int step);
   virtual void ExportLightFilters(AtNode* light, const MPlugArray &filters);
   virtual void Delete();
   static void MakeCommonAttributes(CBaseAttrHelper& helper);
   static AtRGB ConvertKelvinToRGB(float kelvin);
};

#endif // LIGHT_TRANSLATOR_H
