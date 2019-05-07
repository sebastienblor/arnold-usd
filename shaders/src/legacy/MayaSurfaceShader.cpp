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

/*
   AiMetaDataSetStr(nentry, NULL, "maya.name", "surfaceShader");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52535348);
*/
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   
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
   if (AiMax(outTransparency.r, outTransparency.b, outTransparency.b) > AI_EPSILON)
   {
      closures *= AI_RGB_WHITE - outTransparency;
      closures.add(AiClosureTransparent(sg, outTransparency));
   }
   closures.add(AiClosureMatte(sg, AI_RGB_WHITE - outMatteOpacity));
   sg->out.CLOSURE() = closures;
}
