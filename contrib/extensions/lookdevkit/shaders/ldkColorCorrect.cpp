#include <ai.h>
#include "utils.h"

AI_SHADER_NODE_EXPORT_METHODS(ldkColorCorrectMtd);

namespace
{

enum ldkColorCorrectParams
{
   p_inColor,
   p_inAlpha,
   p_unpremultiplyInput,
   p_premultiplyResult,
   p_hueShift,
   p_satGain,
   p_valGain,
   p_colGain,
   p_colOffset,
   p_colGamma,
   p_colClamp,
   p_colClampMin,
   p_colClampMax,
   p_alphaGain,
   p_alphaOffset,
   p_alphaGamma,
   p_alphaClamp,
   p_alphaClampMin,
   p_alphaClampMax
};

};

node_parameters
{
   AiParameterRGB("inColor", 0.3f, 0.3f, 0.3f);
   AiParameterFlt("inAlpha", 1.0f);
   AiParameterBool("unpremultiplyInput", false);
   AiParameterBool("premultiplyResult", false);

   AiParameterFlt("hueShift", 0.0f);
   AiParameterFlt("satGain", 1.0f);
   AiParameterFlt("valGain", 1.0f);

   AiParameterRGB("colGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("colOffset", 0.0f, 0.0f, 0.0f);
   AiParameterVec("colGamma", 1.0f, 1.0f, 1.0f);

   AiParameterBool("colClamp", false);
   AiParameterRGB("colClampMin", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("colClampMax", 1.0f, 1.0f, 1.0f);

   AiParameterFlt("alphaGain", 1.0f);
   AiParameterFlt("alphaOffset", 0.0f);
   AiParameterFlt("alphaGamma", 1.0f);

   AiParameterBool("alphaClamp", false);
   AiParameterFlt("alphaClampMin", 0.0f);
   AiParameterFlt("alphaClampMax", 1.0f);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "colorCorrect");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81652);
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
   AtRGB resultColor = AiShaderEvalParamRGB(p_inColor);
   float resultAlpha = AiShaderEvalParamFlt(p_inAlpha);

   if (AiShaderEvalParamBool(p_unpremultiplyInput))
   {
      resultColor *= resultAlpha;
   }

   const float hueShift = AiShaderEvalParamFlt(p_hueShift);
   const float satGain = AiShaderEvalParamFlt(p_satGain);
   const float valGain = AiShaderEvalParamFlt(p_valGain);

   if (hueShift != 0.0f || satGain != 1.0f || valGain != 1.0f)
   {
      AtVector v = RGBtoHSV(resultColor);
      v.x += hueShift;
      v.y *= satGain;
      v.z *= valGain;
      resultColor = HSVtoRGB(v);
   }

   const AtRGB colGain = AiShaderEvalParamRGB(p_colGain);
   resultColor *= colGain;

   const AtRGB colOffset = AiShaderEvalParamRGB(p_colOffset);
   resultColor += colOffset;

   const AtRGB colGamma = AiShaderEvalParamRGB(p_colGamma);
   if (colGamma.r != 1.0f || colGamma.g != 1.0f || colGamma.b != 1.0f)
   {
      resultColor.r = Gamma(resultColor.r, colGamma.r);
      resultColor.g = Gamma(resultColor.g, colGamma.g);
      resultColor.b = Gamma(resultColor.b, colGamma.b);
   }

   if (AiShaderEvalParamBool(p_colClamp))
   {
      const AtRGB colMin = AiShaderEvalParamRGB(p_colClampMin);
      const AtRGB colMax = AiShaderEvalParamRGB(p_colClampMax);
      resultColor.r = Clamp(resultColor.r, colMin.r, colMax.r);
      resultColor.g = Clamp(resultColor.g, colMin.g, colMax.g);
      resultColor.b = Clamp(resultColor.b, colMin.b, colMax.b);
   }

   const float alphaGain = AiShaderEvalParamFlt(p_alphaGain);
   resultAlpha *= alphaGain;

   const float alphaOffset = AiShaderEvalParamFlt(p_alphaOffset);
   resultAlpha += alphaOffset;

   const float alphaGamma = AiShaderEvalParamFlt(p_alphaGamma);
   if (alphaGamma != 1.0f)
   {
      resultAlpha = Gamma(resultAlpha, alphaGamma);
   }

   if (AiShaderEvalParamBool(p_alphaClamp))
   {
      const float alphaMin = AiShaderEvalParamFlt(p_alphaClampMin);
      const float alphaMax = AiShaderEvalParamFlt(p_alphaClampMax);
      resultAlpha = Clamp(resultAlpha, alphaMin, alphaMax);
   }

   if (AiShaderEvalParamBool(p_premultiplyResult))
   {
      resultColor *= (1.0f / resultAlpha);
   }

   sg->out.RGB() = resultColor;
   sg->out.RGBA().a = resultAlpha;
}
