#include <ai.h>

namespace
{

enum WriteColorInlineParams
{
   p_input,
   p_name
};


};

AI_SHADER_NODE_EXPORT_METHODS(WriteColorInlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);

   AiParameterRGBA("input", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterSTR("aov_name", "");
}

shader_evaluate
{
   sg->out.RGBA = AiShaderEvalParamRGBA(p_input);
   AiAOVSetRGBA(sg, AiShaderEvalParamStr(p_name), sg->out.RGBA);
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
