#ifndef ARNOLDSHAPETRANSLATOR_H
#define ARNOLDSHAPETRANSLATOR_H

#include "translators/NodeTranslator.h"

class DLLEXPORT CShapeTranslator : public CDagTranslator
{
public:
   virtual AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      m_atNode = CDagTranslator::Init(dagPath, scene, outputAttr);
      m_motion       = IsObjectMotionBlurEnabled();
      m_motionDeform = IsDeformMotionBlurEnabled();
      return m_atNode;
   }
   virtual bool RequiresMotionData()
   {
      return m_motion;
   }
   
   void ProcessRenderFlags(AtNode* node);
   // for initializer callbacks:
   static void MakeCommonAttributes(CBaseAttrHelper& helper);

protected:
   bool m_motion;
   bool m_motionDeform;
   
};

#endif // ARNOLDSHAPETRANSLATOR_H
