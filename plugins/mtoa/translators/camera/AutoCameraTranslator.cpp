#include "AutoCameraTranslator.h"

AtNode* CAutoCameraTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_abstract.arnold.asChar());
}
