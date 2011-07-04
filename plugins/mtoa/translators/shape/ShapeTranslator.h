#ifndef SHAPETRANSLATOR_H
#define SHAPETRANSLATOR_H

#include "translators/NodeTranslator.h"



class DLLEXPORT CShapeTranslator : public CDagTranslator
{
public:
   virtual AtNode* Init(CExportSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      m_atNode       = CDagTranslator::Init(session, dagPath, outputAttr);
      m_motion       = IsMotionBlurEnabled(MTOA_MBLUR_OBJECT);
      m_motionDeform = IsMotionBlurEnabled(MTOA_MBLUR_DEFORM);
      return m_atNode;
   }
   virtual bool RequiresMotionData()
   {
      return (m_motion && IsLocalMotionBlurEnabled());
   }
   
   void ProcessRenderFlags(AtNode* node);
   // for initializer callbacks:
   static void MakeCommonAttributes(CBaseAttrHelper& helper);

protected:
   bool m_motion;
   bool m_motionDeform;
   
};

#endif // SHAPETRANSLATOR_H
