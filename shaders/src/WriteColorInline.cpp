#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum WriteColorInlineParams
{
   p_input,
   p_name,
   p_sets
};


};

AI_SHADER_NODE_EXPORT_METHODS(WriteColorInlineMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);

   AiParameterRGBA("input", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterSTR("aov_name", "");
   AiParameterARRAY("sets", AiArray(0, 0, AI_TYPE_NODE));
}

shader_evaluate
{
   sg->out.RGBA = AiShaderEvalParamRGBA(p_input);

   if (IsInShadingGroup(AiShaderEvalParamArray(p_sets), sg))
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
