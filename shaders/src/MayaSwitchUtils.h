#pragma once

#include <ai.h>
#include <vector>

struct MayaShadingSwitchData{
   std::vector<AtNode*> inputs;
   std::vector<AtNode*> shapes;
};

void CreateShadingSwitch(AtNode* node);
void UpdateShadingSwitch(AtNode* node);
void FinishShadingSwitch(AtNode* node);
bool EvaluateShadingSwitch(AtShaderGlobals* sg, AtNode* node);
