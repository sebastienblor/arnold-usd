#include "LightBlockerTranslator.h"

AtNode* CLightBlockerTranslator::Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr)
{
   CDagTranslator::Init(session, dagPath, outputAttr);
   return m_atNode;
}
   
bool CLightBlockerTranslator::RequiresMotionData()
{
   return false;
}
   
void* CLightBlockerTranslator::creator()
{
   return new CLightBlockerTranslator();
}
   
AtNode* CLightBlockerTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("light_blocker");
}

void CLightBlockerTranslator::Export(AtNode* blocker)
{
   
}
