#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkColorConditionMtd);

namespace
{

enum ldkColorConditionParams
{
   p_colorA,
   p_alphaA,
   p_colorB,
   p_alphaB,
   p_condition
};

};

node_parameters
{
   AiParameterRGB("colorA", 1.0f, 0.0f, 0.0f);
   AiParameterFlt("alphaA", 1.0f);
   AiParameterRGB("colorB", 0.0f, 1.0f, 0.0f);
   AiParameterFlt("alphaB", 1.0f);
   AiParameterBool("condition", false);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "colorCondition");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81634);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
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

shader_evaluate
{
   if (AiShaderEvalParamBool(p_condition))
   {
      sg->out.RGB() = AiShaderEvalParamRGB(p_colorA);
      sg->out.RGBA().a = AiShaderEvalParamFlt(p_alphaA);
   }
   else
   {
      sg->out.RGB() = AiShaderEvalParamRGB(p_colorB);
      sg->out.RGBA().a = AiShaderEvalParamFlt(p_alphaB);
   }
}
