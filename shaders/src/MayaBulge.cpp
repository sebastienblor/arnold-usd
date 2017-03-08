#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaBulgeMtd);

namespace
{

enum MayaBulgeParams
{
   p_uWidth,
   p_vWidth,
   p_uvCoord,
   MAYA_COLOR_BALANCE_ENUM
};

};

node_parameters
{
   AiParameterFlt("uWidth", 0.1f);
   AiParameterFlt("vWidth", 0.1f);
   AiParameterVec2("uvCoord", 0.0f, 0.0f);
   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "bulge");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544255);
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

   float ss = uv.x;
   float tt = uv.y;
   ss = Mod(ss, 1.000001f);
   tt = Mod(tt, 1.000001f);

   float a = SmoothStep(uWidth * 0.5f, 0.5f, ss) * (1 - SmoothStep(0.5f, 1.0f - (uWidth * 0.5f), ss));
   float b = SmoothStep(vWidth * 0.5f, 0.5f, tt) * (1 - SmoothStep(0.5f, 1.0f - (vWidth * 0.5f), tt));
   float r = sqrt(fabs(a * b));

   sg->out.RGBA() = AtRGBA(r, r, r, r);
   MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
}
