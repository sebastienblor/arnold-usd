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
   AiParameterFLT("inAlpha", 1.0f);

   AiMetaDataSetStr(mds, NULL, "maya.name", "colorConstant");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x81622);
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
   sg->out.RGBA.rgb() = inColor;
   sg->out.RGBA.a = inAlpha;
}
