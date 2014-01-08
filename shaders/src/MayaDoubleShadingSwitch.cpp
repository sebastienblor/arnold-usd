#include <ai.h>
#include <vector>

#include "MayaSwitchUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaDoubleShadingSwitchMtd);

node_parameters
{
   AiParameterPnt2("default", 0.8f, 0.8f);
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
      sg->out.PNT2 = AiShaderEvalParamPnt2(0);
}
