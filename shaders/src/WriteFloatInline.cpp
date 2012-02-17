#include <ai.h>

namespace
{

enum WriteFloatInlineParams
{
   p_input,
   p_name
};


};

AI_SHADER_NODE_EXPORT_METHODS(WriteFloatInlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);

   AiParameterFLT("input", 0.0f);
   AiParameterSTR("aov_name", "");
}

shader_evaluate
{
   sg->out.FLT = AiShaderEvalParamFlt(p_input);
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
