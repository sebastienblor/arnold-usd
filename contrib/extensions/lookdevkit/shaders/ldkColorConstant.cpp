#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkColorConstantMtd);

namespace
{

enum ldkFloatMathParams
{
   p_inColor,
   p_inAlpha
};

};

node_parameters
{
   AiParameterRGB("inColor", 0.3f, 0.3f, 0.3f);
   AiParameterFlt("inAlpha", 1.0f);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "colorConstant");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81622);
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
   const AtRGB inColor = AiShaderEvalParamRGB(p_inColor);
   const float inAlpha = AiShaderEvalParamFlt(p_inAlpha);
   sg->out.RGB() = inColor;
   sg->out.RGBA().a = inAlpha;
}
