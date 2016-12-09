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

   AiMetaDataSetStr(nentry, NULL, "maya.name", "surfaceShader");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52535348);
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
   AtRGB color = AiShaderEvalParamRGB(p_outColor);
   AtRGB outMatteOpacity = AiShaderEvalParamRGB(p_outMatteOpacity);
   AtRGB outTransparency = AiShaderEvalParamRGB(p_outTransparency);


   AtClosureList closures;
   closures.add(AiClosureEmission(sg, color));
   closures.add(AiClosureMatte(sg, outMatteOpacity));
   closures *= AI_RGB_WHITE - outTransparency;
   closures.add(AiClosureTransparent(sg, outTransparency));
   sg->out.CLOSURE() = closures;
}
