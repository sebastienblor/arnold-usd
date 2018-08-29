#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaGridMtd);

namespace
{

enum MayaGridParams
{
   p_lineColor,
   p_fillerColor,
   p_uWidth,
   p_vWidth,
   p_contrast,
   p_uvCoord,
   MAYA_COLOR_BALANCE_ENUM,
   p_filter,
   p_filterOffset
};

inline AtRGB Contrast(const AtRGB &c, const AtRGB &d, float f) 
{
   return Mix((c + d) / 2.0f, c, f);
}

};

node_parameters
{
   AiParameterRGB("lineColor", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("fillerColor", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("uWidth", 0.1f);
   AiParameterFlt("vWidth", 0.1f);
   AiParameterFlt("contrast", 1.0);
   AiParameterVec2("uvCoord", 0.0f, 0.0f);
   AddMayaColorBalanceParams(params, nentry);
   AiParameterFlt("filter", 1.0f);
   AiParameterFlt("filterOffset", 0.0f);
   
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   AiMetaDataSetStr(nentry, NULL, "maya.name", "grid");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544744);
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

   float uWidth = AiShaderEvalParamFlt(p_uWidth);
   float vWidth = AiShaderEvalParamFlt(p_vWidth);
   AtRGB lineColor = AiShaderEvalParamRGB(p_lineColor);
   AtRGB fillerColor = AiShaderEvalParamRGB(p_fillerColor);
   float cont = AiShaderEvalParamFlt(p_contrast);

   float filter = AiShaderEvalParamFlt(p_filter);
   float filterOffset = AiShaderEvalParamFlt(p_filterOffset);

   float ss = uv.x;
   float tt = 1.0f - uv.y;
   ss = Mod(ss, 1.000001f);
   tt = Mod(tt, 1.000001f);

   // set fw to zero at the moment
   float fw = 0.0f;

   float dss = fw * filter + filterOffset * 2.0f;
   float dtt = fw * filter + filterOffset * 2.0f;

   float f = FilteredPulseTrain(uWidth, 1.0f, ss + uWidth * 0.5f, dss) * 
             FilteredPulseTrain(vWidth, 1.0f, tt + vWidth * 0.5f, dtt);

   AtRGB lc = Contrast(lineColor, fillerColor, cont);
   AtRGB fc = Contrast(fillerColor, lineColor, cont);

   sg->out.RGBA() = AtRGBA(Mix(lc, fc, f));
   sg->out.RGBA().a = 1.0f - f;
   MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
}
