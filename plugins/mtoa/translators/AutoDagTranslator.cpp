#include "AutoDagTranslator.h"

// Auto DAG translator
//

AtNode* CAutoDagTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_abstract.arnold.asChar());
}

bool CAutoDagTranslator::RequiresMotionData()
{
   return IsMotionBlurEnabled(MTOA_MBLUR_OBJECT) &&  IsLocalMotionBlurEnabled() && (!m_isStatic);
}
