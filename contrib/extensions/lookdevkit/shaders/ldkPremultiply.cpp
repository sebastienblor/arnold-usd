#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkPremultiplyMtd);

namespace
{

enum ldkPremultiplyParams
{
   p_inColor,
   p_inAlpha
};

};

node_parameters
{
   AiParameterRGB("inColor", 0.3f, 0.3f, 0.3f);
   AiParameterFlt("inAlpha", 1.0f);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "premultiply");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81644);
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

   resultColor *= resultAlpha;

   sg->out.RGB() = resultColor;
   sg->out.RGBA().a = resultAlpha;
}
