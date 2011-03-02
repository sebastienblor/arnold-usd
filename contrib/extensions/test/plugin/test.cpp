#include "test.h"

#include <ai_msg.h>
#include <ai_nodes.h>

void CTestTranslatorCmd::Export(AtNode* shader)
{
   AiMsgInfo("[test extension] Exporting %s", m_fnNode.name().asChar());
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
}

void CTestTranslatorCmd::Update(AtNode* shader)
{
   AiMsgInfo("[test extension] Making %s red", m_fnNode.name().asChar());
   AiNodeSetRGB(shader, "Kd_color", 1.0f, 0.0f, 0.0f);
}

const char * CTestTranslatorCmd::GetArnoldNodeType()
{
   return "standard";
}

void* CTestTranslatorCmd::creator()
{
   return new CTestTranslatorCmd();
}
