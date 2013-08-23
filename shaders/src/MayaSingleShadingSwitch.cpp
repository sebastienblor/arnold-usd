#include <ai.h>
#include <vector>

#include "MayaSwitchUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaSingleShadingSwitchMtd);

node_parameters
{
	AiParameterFlt("default", 1.0f);
   AiParameterARRAY("inputs", AiArrayAllocate(0, 1, AI_TYPE_NODE));
   AiParameterArray("shapes", AiArrayAllocate(0, 1, AI_TYPE_NODE));

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
      sg->out.FLT = AiShaderEvalParamFlt(0);
}
