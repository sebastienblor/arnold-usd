#include "AutoCameraTranslator.h"

AtNode* CAutoCameraTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_abstract.arnold.asChar());
}

bool CAutoCameraTranslator::RequiresMotionData()
{
   return IsMotionBlurEnabled(MTOA_MBLUR_CAMERA) &&  IsLocalMotionBlurEnabled();
}
