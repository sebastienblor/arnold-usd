#include "XGenHairMappingTranslator.h"
#include "extension/Extension.h"

AtNode* CXgHairMappingTranslator::CreateArnoldNodes()
{
   AddArnoldNode("state_float", "bv");
   return AddArnoldNode("layer_rgba");
}

void CXgHairMappingTranslator::Export(AtNode* shader)
{
   AtNode *bv = GetArnoldNode("bv");
   AiNodeSetStr(bv, "variable", "bv");
   AiNodeSetRGBA(shader, "input1", 0.f, 0.f, 0.f, 1.f);
   AiNodeLink(bv, "input1.g", shader);

   MPlug translateFramePlug = FindMayaPlug("translateFrame");
   AiNodeSetRGBA(shader, "input2", translateFramePlug.child(0).asFloat(), translateFramePlug.child(1).asFloat(), 0.f, 1.f);
   AiNodeSetStr(shader, "operation2", "minus");
   AiNodeSetStr(shader, "alpha_operation2", "preserve");

   MPlug coveragePlug = FindMayaPlug("coverage");
   AiNodeSetRGBA(shader, "input3", coveragePlug.child(0).asFloat(), coveragePlug.child(1).asFloat(), 1.f, 1.f);
   AiNodeSetStr(shader, "operation3", "divide");
   AiNodeSetStr(shader, "alpha_operation3", "preserve");

   MPlug repeatUVPlug = FindMayaPlug("repeatUV");
   AiNodeSetRGBA(shader, "input4", repeatUVPlug.child(0).asFloat(), repeatUVPlug.child(1).asFloat(), 1.f, 1.f);
   AiNodeSetStr(shader, "operation4", "multiply");
   AiNodeSetStr(shader, "alpha_operation4", "preserve");

   MPlug offsetUVPlug = FindMayaPlug("offset");
   AiNodeSetRGBA(shader, "input5", offsetUVPlug.child(0).asFloat(), offsetUVPlug.child(1).asFloat(), 0.f, 1.f);
   AiNodeSetStr(shader, "operation5", "plus");
   AiNodeSetStr(shader, "alpha_operation5", "preserve");
}
