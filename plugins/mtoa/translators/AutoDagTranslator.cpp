#include "AutoDagTranslator.h"
#include "translators/NodeTranslatorImpl.h"

// Auto DAG translator
//

AtNode* CAutoDagTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_impl->m_abstract.arnold.asChar());
}

bool CAutoDagTranslator::RequiresMotionData()
{
   return IsMotionBlurEnabled(MTOA_MBLUR_OBJECT) &&  IsLocalMotionBlurEnabled();
}
