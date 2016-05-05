#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaSurfaceShaderMtd);

namespace
{

enum MayaSurfaceShaderParams
{
   p_outColor,
   p_outTransparency,
   p_outMatteOpacity
};

};

node_parameters
{
   AiParameterRGB("outColor", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("outTransparency", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("outMatteOpacity", 1.0f, 1.0f, 1.0f);

   AiMetaDataSetStr(mds, NULL, "maya.name", "surfaceShader");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x52535348);
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
   sg->out_opacity = AI_RGB_WHITE - AiShaderEvalParamRGB(p_outTransparency);
   AtRGB color = AiShaderEvalParamRGB(p_outColor);
   sg->out.RGBA().r = color.r;
   sg->out.RGBA().g = color.g;
   sg->out.RGBA().b = color.b;
   sg->out.RGBA().a = Luminance(AiShaderEvalParamRGB(p_outMatteOpacity));
}
