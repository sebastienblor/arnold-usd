//#include "test.h"
#include "test.h"
#include <maya/MGlobal.h>
#include <ai_msg.h>
#include <ai_nodes.h>

AtNode* CTestTranslatorCmd::Export()
{
   AiMsgInfo("[test extension] Exporting %s", m_fnNode.name().asChar());
   AtNode* shader = AiNode("standard");

   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CTestTranslatorCmd::Update(AtNode* shader)
{
   AiMsgInfo("[test extension] Making %s red", m_fnNode.name().asChar());
   AiNodeSetRGB(shader, "Kd_color", 1.0f, 0.0f, 0.0f);
}

void* CTestTranslatorCmd::creator()
{
   return new CTestTranslatorCmd();
}
