#include <ai.h>

#include <limits.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaStuccoMtd);

namespace
{
   enum MayaStuccoParams
   {
      p_shaker = 0,
      p_channel1,
      p_channel2,
      p_wrap,
      p_local,
      p_placementMatrix,
      MAYA_COLOR_BALANCE_ENUM
   };
}

node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterFlt("shaker", 20.0f);
   AiParameterRGB("channel1", 1.0f, 0.0f, 0.0f);
   AiParameterRGB("channel2", 0.0f, 0.0f, 1.0f);
   AiParameterBool("wrap", true);
   AiParameterBool("local", false);
   AiParameterMtx("placementMatrix", id);
   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "stucco");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52533630);
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
   AtRGB channel1 = AiShaderEvalParamRGB(p_channel1);
   AtRGB channel2 = AiShaderEvalParamRGB(p_channel2);
   float shaker = AiShaderEvalParamFlt(p_shaker);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   bool local = AiShaderEvalParamBool(p_local);
   bool wrap = AiShaderEvalParamBool(p_wrap);

   AtVector tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AtVector P = AiM4PointByMatrixMult(*placementMatrix, (local ? sg->Po : sg->P));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      float noise = 0.0f;
      float amp = 1.0f;
      float freq = 0.5f;
      
      float maxP = (fabsf(P.x) > fabsf(P.y)) ? fabsf(P.x) : fabsf(P.y);
      maxP = (maxP > fabsf(P.z)) ? maxP : fabsf(P.z);

      // Compute base fractal noise
      for (int i=0; i<8; ++i)
      {
         if((maxP * freq) >= LONG_MAX)
            break;
         noise += amp * AiPerlin4(freq * P, 0.0f);
         freq *= 2.0f;
         amp *= 0.5f;
      }

      noise = AiClamp(0.5f * (noise + 1.0f), 0.0f, 1.0f);

      float a = shaker / 10.0f;

      float b = AiMin(2.0f, 4.0f / AiMax(a, 0.001f));

      float mix = pow(1.0f - pow(2.0f * fabs(noise - 0.5f), b), 4.0f*a*a);

      AtRGB out = Mix(channel1, channel2, mix);

      sg->out.RGBA() = AtRGBA(out);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA());
   }
   if (usePref) RestorePoints(sg, tmpPts);
}
