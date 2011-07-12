#include "test.h"

#include <ai_msg.h>
#include <ai_nodes.h>

void CTestTranslatorCmd::Export(AtNode* shader)
{
   AiMsgInfo("[test extension] Exporting %s", GetMayaNodeName().asChar());
   AiNodeSetStr(shader, "name", GetMayaNodeName().asChar());
   Update(shader);
}

void CTestTranslatorCmd::Update(AtNode* shader)
{
   AiMsgInfo("[test extension] Making %s red", GetMayaNodeName().asChar());
   AiNodeSetRGB(shader, "Kd_color", 1.0f, 0.0f, 0.0f);
}

AtNode* CTestTranslatorCmd::CreateArnoldNodes()
{
   return AddArnoldNode("standard");
}

void* CTestTranslatorCmd::creator()
{
   return new CTestTranslatorCmd();
}
