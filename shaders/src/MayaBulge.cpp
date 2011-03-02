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
   AiParameterFLT("uWidth", 0.1f);
   AiParameterFLT("vWidth", 0.1f);
   AiParameterPNT2("uvCoord", 0.0f, 0.0f);
   AddMayaColorBalanceParams(params);

   AiMetaDataSetStr(mds, NULL, "maya.counterpart", "bulge");
   AiMetaDataSetInt(mds, NULL, "maya.counterpart_id", 0x52544255);
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
   AtPoint2 uv;

   uv.x = sg->u;
   uv.y = sg->v;

   if (AiNodeGetLink(node, "uvCoord") ||
       AiNodeGetLink(node, "uvCoord.x") ||
       AiNodeGetLink(node, "uvCoord.y"))
   {
      uv = AiShaderEvalParamPnt2(p_uvCoord);
   }

   float uWidth = AiShaderEvalParamFlt(p_uWidth);
   float vWidth = AiShaderEvalParamFlt(p_vWidth);

   if (!IsValidUV(uv.x, uv.y))
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);
      return;
   }

   float ss = uv.x;
   float tt = uv.y;
   ss = Mod(ss, 1.000001f);
   tt = Mod(tt, 1.000001f);

   float a = SmoothStep(uWidth * 0.5f, 0.5f, ss) * (1 - SmoothStep(0.5f, 1.0f - (uWidth * 0.5f), ss));
   float b = SmoothStep(vWidth * 0.5f, 0.5f, tt) * (1 - SmoothStep(0.5f, 1.0f - (vWidth * 0.5f), tt));
   float r = sqrt(fabs(a * b));

   AiRGBACreate(sg->out.RGBA, r, r, r, r);
   MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);
}
