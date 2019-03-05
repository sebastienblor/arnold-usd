#include <ai.h>
#include "../utils.h"

AI_SHADER_NODE_EXPORT_METHODS(ldkFloatCorrectMtd);

namespace
{

enum ldkFloatCorrectParams
{
   p_inFloat,
   p_gain,
   p_offset,
   p_gammaScale,
   p_clampOutput,
   p_clampMin,
   p_clampMax
};

};

node_parameters
{
   AiParameterFlt("inFloat", 1.0f);
   AiParameterFlt("gain", 1.0f);
   AiParameterFlt("offset", 0.0f);
   AiParameterFlt("gammaScale", 1.0f);

   AiParameterBool("clampOutput", false);
   AiParameterFlt("clampMin", 0.0f);
   AiParameterFlt("clampMax", 1.0f);

   //AiMetaDataSetStr(nentry, NULL, "maya.name", "floatCorrect");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81650);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
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
   float result = AiShaderEvalParamFlt(p_inFloat);

   const float gain = AiShaderEvalParamFlt(p_gain);
   result *= gain;

   const float offset = AiShaderEvalParamFlt(p_offset);
   result += offset;

   const float gamma = AiShaderEvalParamFlt(p_gammaScale);
   if (gamma != 1.0f)
   {
      result = Gamma(result, gamma);
   }

   if (AiShaderEvalParamBool(p_clampOutput))
   {
      const float clampMin = AiShaderEvalParamFlt(p_clampMin);
      const float clampMax = AiShaderEvalParamFlt(p_clampMax);
      result = Clamp(result, clampMin, clampMax);
   }

   sg->out.FLT() = result;
}
