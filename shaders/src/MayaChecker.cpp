#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaCheckerMtd);

namespace
{

enum MayaCheckerParams
{
   p_color1,
   p_color2,
   p_contrast,
   p_uvCoord,
   MAYA_COLOR_BALANCE_ENUM,
   p_filter,
   p_filterOffset
};

};

node_parameters
{
   AiParameterRGB("color1", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("color2", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("contrast", 1.0);
   AiParameterVec2("uvCoord", 0.0f, 0.0f);
   AddMayaColorBalanceParams(params, nentry);
   AiParameterFlt("filter", 1.0f);
   AiParameterFlt("filterOffset", 0.0f);
   
   AiMetaDataSetStr(nentry, NULL, "maya.name", "checker");
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544348);
}

node_initialize
{
}

node_update
{
   // Unconnected render attributes (uvCoords, normalCamera, etc)
   // should use globals as following Maya's behavior
   if (!AiNodeGetLink(node, "uvCoord"))
   {
      AtVector2 uv = AI_P2_ZERO;
      if (!AiNodeGetLink(node, "uvCoord.x")) uv.x = UV_GLOBALS;
      if (!AiNodeGetLink(node, "uvCoord.y")) uv.y = UV_GLOBALS;
      AiNodeSetVec2(node, "uvCoord", uv.x, uv.y);
   }
}

node_finish
{
}

shader_evaluate
{
   AtVector2 uv;
   uv = AiShaderEvalParamVec2(p_uvCoord);
   // Will be set to GLOBALS by update if unconnected
   if (uv.x == UV_GLOBALS) uv.x = sg->u;
   if (uv.y == UV_GLOBALS) uv.y = sg->v;

   if (!IsValidUV(uv))
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA());
      return;
   }

   AtRGB color1 = AiShaderEvalParamRGB(p_color1);
   AtRGB color2 = AiShaderEvalParamRGB(p_color2);

   float ss = fmod(uv.x, 1.000001f);
   float tt = fmod(uv.y, 1.000001f);

   const float filter = AiShaderEvalParamFlt(p_filter) / 20.0f;
   const float filterOffset = AiShaderEvalParamFlt(p_filterOffset) / 20.0f;
   
   const float fw = AiMax(sqrtf(AiShaderGlobalsArea(sg)), AI_EPSILON) * filter + filterOffset;

   float dss = fw;
   float dtt = fw;

   float f = 0.5f - 2.0f *
             (FilteredPulseTrain(0.5f, 1.0f, ss, dss) - 0.5f) *
             (FilteredPulseTrain(0.5f, 1.0f, tt, dtt) - 0.5f);

   f = 0.5f + (f - 0.5f) * AiShaderEvalParamFlt(p_contrast);

   sg->out.RGBA().r = AiClamp(color1.r + (color2.r - color1.r) * f, 0.0f, 1.0f);
   sg->out.RGBA().g = AiClamp(color1.g + (color2.g - color1.g) * f, 0.0f, 1.0f);
   sg->out.RGBA().b = AiClamp(color1.b + (color2.b - color1.b) * f, 0.0f, 1.0f);
   sg->out.RGBA().a = AiClamp(1.0f - f, 0.0f, 1.0f);

   MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
}
