#include <ai.h>
#include <vector>

#include "MayaSwitchUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaTripleShadingSwitchMtd);

node_parameters
{
   AiParameterRGB("default", 0.8f, 0.8f, 0.8f);
   AiParameterArray("inputs", AiArrayAllocate(0, 1, AI_TYPE_NODE));
   AiParameterArray("shapes", AiArrayAllocate(0, 1, AI_TYPE_NODE));

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

node_initialize
{
   CreateShadingSwitch(node);
}

node_update
{
   UpdateShadingSwitch(node);
}

node_finish
{
   FinishShadingSwitch(node);
}

shader_evaluate
{
   if (EvaluateShadingSwitch(sg, node))
      sg->out.RGB() = AiShaderEvalParamRGB(0);
}
