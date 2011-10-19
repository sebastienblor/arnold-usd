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
      m_motion = session->IsMotionBlurEnabled(MTOA_MBLUR_LIGHT);
      return m_atNode;
   }
   bool RequiresMotionData()
   {
      return m_motion;
   }
protected:
   virtual void Export(AtNode* light);
   virtual void ExportMotion(AtNode* light, AtUInt step);
   virtual void ExportLightFilters(AtNode* light, const MPlugArray &filters);
   virtual void Delete();
   static void MakeCommonAttributes(CBaseAttrHelper& helper);

protected:
   bool m_motion;
};

#endif // LIGHT_TRANSLATOR_H
