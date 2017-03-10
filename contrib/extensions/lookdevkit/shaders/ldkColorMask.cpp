#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkColorMaskMtd);

namespace
{

enum ldkColorMaskParams
{
   p_inColor,
   p_inAlpha,
   p_preserveColor,
   p_mask,
   p_maskAlpha,
   p_maskAlphaIsLuminance
};

};

node_parameters
{
   AiParameterRGB("inColor", 0.3f, 0.3f, 0.3f);
   AiParameterFlt("inAlpha", 1.0f);
   AiParameterBool("preserveColor", false);

   AiParameterRGB("mask", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("maskAlpha", 1.0f);
   AiParameterBool("maskAlphaIsLuminance", false);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "colorMask");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81626);
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
   const AtRGB inColor = AiShaderEvalParamRGB(p_inColor);
   const float inAlpha = AiShaderEvalParamFlt(p_inAlpha);

   const AtRGB maskColor = AiShaderEvalParamRGB(p_mask);
   const float maskAlpha = AiShaderEvalParamBool(p_maskAlphaIsLuminance) ? 
      (1.0f - (maskColor[0] * 0.3f + maskColor[1] * 0.59f + maskColor[2] * 0.11f)) : AiShaderEvalParamFlt(p_maskAlpha);

   sg->out.RGB() = inColor;
   sg->out.RGBA().a = inAlpha;

   if (maskAlpha > 0.0f)
   {
      if (!AiShaderEvalParamBool(p_preserveColor))
      {
         sg->out.RGB() = inColor * (1.0f - maskAlpha);
      }
      sg->out.RGBA().a -= maskAlpha;
   }
}
