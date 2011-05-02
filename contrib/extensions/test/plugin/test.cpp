#include "test.h"

#include <ai_msg.h>
#include <ai_nodes.h>

void CTestTranslatorCmd::Export(AtNode* shader)
{
   AiMsgInfo("[test extension] Exporting %s", GetFnNode().name().asChar());
   AiNodeSetStr(shader, "name", GetFnNode().name().asChar());
   Update(shader);
}

void CTestTranslatorCmd::Update(AtNode* shader)
{
   AiMsgInfo("[test extension] Making %s red", GetFnNode().name().asChar());
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
