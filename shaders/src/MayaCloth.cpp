#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaClothMtd);

namespace
{

enum MayaClothParams
{
   p_gapColor,
   p_uColor,
   p_vColor,
   p_uWidth,
   p_vWidth,
   p_uWave,
   p_vWave,
   p_randomness,
   p_widthSpread,
   p_brightSpread,
   p_uvCoord,
   MAYA_COLOR_BALANCE_ENUM
};

};

node_parameters
{
   AiParameterRGB("gapColor", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("uColor", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("vColor", 0.5f, 0.5f, 0.5f);
   AiParameterFlt("uWidth", 0.75f);
   AiParameterFlt("vWidth", 0.75f);
   AiParameterFlt("uWave", 0.0f);
   AiParameterFlt("vWave", 0.0f);
   AiParameterFlt("randomness", 0.0f);
   AiParameterFlt("widthSpread", 0.0f);
   AiParameterFlt("brightSpread", 0.0f);
   AiParameterVec2("uvCoord", 0.0f, 0.0f);
   AddMayaColorBalanceParams(params, nentry);
   
   AiMetaDataSetStr(nentry, NULL, "maya.name", "cloth");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x5254434C);
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

   float randomness = AiShaderEvalParamFlt(p_randomness);
   float uWave = AiShaderEvalParamFlt(p_uWave);
   float vWave = AiShaderEvalParamFlt(p_vWave);
   float uWidth = AiShaderEvalParamFlt(p_uWidth);
   float vWidth = AiShaderEvalParamFlt(p_vWidth);
   float widthSpread = AiShaderEvalParamFlt(p_widthSpread);
   float brightSpread = AiShaderEvalParamFlt(p_brightSpread);
   float ss = uv.x;
   float tt = uv.y;

   ss = Mod(ss, 1.000001f);
   tt = Mod(tt, 1.000001f);

   if (randomness > 0.0f)
   {
      AtVector2 point;

      point.x = uv.x / 23.0f;
      point.y = uv.y / 23.0f;
      tt += AiNoise2(point, 2, 10.0f, 2.0f) * randomness;

      point.x = uv.x / 22.0f;
      point.y = uv.y / 24.0f;
      ss += AiNoise2(point, 2, 10.0f, 2.0f) * randomness;
   }

   float prewavedSS = ss;

   if (uWave > 0.0f)
   {
      ss -= uWave * sin(tt * 2.0f * (float)AI_PI);
   }

   if (vWave > 0.0f)
   {
      tt += vWave * sin(prewavedSS * 2.0f * (float)AI_PI);
   }

   if (widthSpread > 0.0f)
   {
      AtVector p;

      p.x = uv.x;
      p.y = uv.y * 0.5f;
      p.z = 1.0f;
      float spread = AiPerlin3(0.5f * p);
      uWidth -= widthSpread * spread;

      p.x = uv.x * 0.5f;
      p.y = uv.y;
      p.z = 2.0f;
      spread = AiPerlin3(0.5f * p);
      vWidth -= widthSpread * spread;
   }

   ss = Mod(ss, 1.00001f);
   tt = Mod(tt, 1.00001f);

   float threadColor = 0;

   if ((ss >= 0.5f && tt < 0.5f) || (ss < 0.5f && tt >= 0.5f))
   {
      float tmp = ss;
      ss = tt;
      tt = tmp;

      tmp = uWidth;
      uWidth = vWidth;
      vWidth = tmp;

      threadColor = 1.0;
   }

   ss = Mod(2.0f * ss, 1.00001f);
   tt = Mod(2.0f * tt, 1.00001f);

   float cloth = 0.0f; // 0 = gap color, 1 = thread color
   float inGap = 0;

   if (tt < vWidth)
   {
      float cs = ss - 0.5f * uWidth;
      float ct = 2.0f * tt / vWidth - 1.0f;
      cloth = 0.75f * (SQR(cs) + SQR(ct));
   }
   else if (ss < uWidth)
   {
      float cs = 2.0f * ss / uWidth - 1.0f;
      float ct = tt - 0.5f * vWidth - 1.0f;
      cloth = 0.75f * (SQR(cs) + SQR(ct));
      threadColor = 1.0f - threadColor;
   }
   else
   {
      inGap = 1.0f;
   }

   if (inGap < 1.0f)
   {
      cloth = 1.0f - cloth;

      if (brightSpread > 0.0f)
      {
         AtVector2 p;
         p.x = (threadColor ? uv.x : uv.y) * 2.0f;
         p.y = p.x;
         float spread = AiPerlin2(p);
         cloth = Mix(cloth, cloth * spread, brightSpread);
      }
   }

   cloth = AiClamp(cloth, 0.0f, 1.0f);

   AtRGB a = (1.0f - cloth) * AiShaderEvalParamRGB(p_gapColor) +
             cloth * (threadColor ? AiShaderEvalParamRGB(p_uColor) : AiShaderEvalParamRGB(p_vColor));

   sg->out.RGBA() = AtRGBA(a);
   // From maya node documentation cloth ignores alphaIsLuminance
   // TODO: js - fix this for the new function.
   // alphaIsLuminance = false;
   MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
}
