#include <ai.h>

#include "MayaUtils.h"

#include <limits.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaBrownianMtd);

namespace
{

enum MayaBrownianParams
{
   p_lacunarity,
   p_increment,
   p_octaves,
   p_weight3d,
   p_placementMatrix,
   p_wrap,
   p_local,
   MAYA_COLOR_BALANCE_ENUM
};

};

node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterFlt("lacunarity", 4.0f);
   AiParameterFlt("increment", 0.1f);
   AiParameterFlt("octaves", 3.0f);
   AiParameterVec("weight3d", 1.0f, 1.0f, 1.0f);
   AiParameterMtx("placementMatrix", id);
   AiParameterBool("wrap", true);
   AiParameterBool("local", false);
   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "brownian");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x5246424d);
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
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   bool wrap = AiShaderEvalParamBool(p_wrap);
   bool local = AiShaderEvalParamBool(p_local);

   AtVector tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AtVector P = AiM4PointByMatrixMult(*placementMatrix, (local ? sg->Po : sg->P));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      float lacunarity = AiShaderEvalParamFlt(p_lacunarity);
      float increment = AiShaderEvalParamFlt(p_increment);
      float octaves = AiShaderEvalParamFlt(p_octaves);
      AtVector weight3d = AiShaderEvalParamVec(p_weight3d);

      int i = 0;
      float curAmp = 1.0f;
      float curFreq = 1.0f;
      float noise = 0.0f;

      int ioctaves = int(floor(octaves));
      float foctave = octaves - float(ioctaves);

      float ratio = (float) exp(-0.5f * increment);

      P *= weight3d;
      
      float maxP = (fabsf(P.x) > fabsf(P.y)) ? fabsf(P.x) : fabsf(P.y);
      maxP = (maxP > fabsf(P.z)) ? maxP : fabsf(P.z);

      float pixelSize = float(AI_EPSILON);
      float nyquist = 2.0f * pixelSize;
      float pixel = 1.0f;
      
      while (i < ioctaves && pixel > nyquist)
      {
         if((maxP * curFreq) >= LONG_MAX)
            break;
         noise += curAmp * AiPerlin3(P * curFreq);
         curFreq *= lacunarity;
         
         pixel /= lacunarity;
         
         curAmp *= ratio;
         ++i;
      }

      if((maxP * curFreq) < LONG_MAX)
      {
         float tmp = AiPerlin3(P * curFreq);
         tmp = curAmp * tmp;
         tmp = foctave * tmp;
         noise += foctave * curAmp * AiPerlin3(P * curFreq);
      }

      noise = noise * 0.5f + 0.5f;

      sg->out.RGBA() = AtRGBA(noise, noise, noise, 1.0f);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA());
   }
   if (usePref) RestorePoints(sg, tmpPts);
}
