#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum WriteFloatInlineParams
{
   p_input,
   p_name,
   p_sets
};


};

AI_SHADER_NODE_EXPORT_METHODS(WriteFloatInlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);

   AiParameterFLT("input", 0.0f);
   AiParameterSTR("aov_name", "");
   AiParameterARRAY("sets", AiArray(0, 0, AI_TYPE_NODE));
}

shader_evaluate
{
   sg->out.FLT = AiShaderEvalParamFlt(p_input);
   if ((sg->Rt & AI_RAY_CAMERA) && IsInShadingGroup(AiShaderEvalParamArray(p_sets), sg))
      AiAOVSetFlt(sg, AiShaderEvalParamStr(p_name), sg->out.FLT);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}
