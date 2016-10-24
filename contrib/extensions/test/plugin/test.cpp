#include "test.h"

#include <ai_msg.h>
#include <ai_nodes.h>

void CTestTranslatorCmd::Export(AtNode* shader)
{
   AiMsgInfo("[test extension] Exporting %s", GetMayaNodeName().asChar());
   if(!IsExported())
   {
        AiNodeSetStr(shader, "name", GetMayaNodeName().asChar());
    }
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
