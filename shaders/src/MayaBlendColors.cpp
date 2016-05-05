#include <ai.h>

namespace
{

enum MayaBlendColorsParams
{
   p_blender,
   p_color1,
   p_color2
};

};

AI_SHADER_NODE_EXPORT_METHODS(MayaBlendColorsMtd);

node_parameters
{
   AiParameterFlt("blender", 0.5f);
   AiParameterRGB("color1", 1, 0, 0);
   AiParameterRGB("color2", 0, 0, 1);

   AiMetaDataSetStr(mds, NULL, "maya.name", "blendColors");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x52424c32);
}

shader_evaluate
{
   float blend  = AiShaderEvalParamFlt(p_blender);
   if (blend == 0.0f)
      sg->out.RGB() = AiShaderEvalParamRGB(p_color2);
   else if (blend == 1.0f)
      sg->out.RGB() = AiShaderEvalParamRGB(p_color1);
   else
   {
      AtRGB   color1 = AiShaderEvalParamRGB(p_color1);
      AtRGB   color2 = AiShaderEvalParamRGB(p_color2);
      sg->out.RGB() = AiColorLerp(blend, color2, color1);
   }
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
