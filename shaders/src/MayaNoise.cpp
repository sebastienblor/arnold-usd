#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaNoiseMtd);

namespace
{

enum MayaNoiseParams
{
   p_threshold,
   p_amplitude,
   p_ratio,
   p_frequencyRatio,
   p_depthMax,
   p_inflection,
   p_time,
   p_frequency,
   p_implode,
   p_implodeCenter,
   p_noiseType,
   p_density,
   p_spottyness,
   p_sizeRand,
   p_randomness,
   p_falloff,
   p_numWaves,
   p_uvCoord,
   MAYA_COLOR_BALANCE_ENUM
};

enum NoiseType
{
   NT_PERLIN = 0,
   NT_BILLOW,
   NT_WAVE,
   NT_WISPY,
   NT_SPACETIME
};

enum FalloffType
{
   FT_LINEAR = 0,
   FT_SMOOTH,
   FT_FAST,
   FT_BUBBLE
};

const char* noise_type_enum[] =
{
   "perlin",
   "billow",
   "wave",
   "wispy",
   "spacetime",
   NULL
};

const char* falloff_type_enum[] =
{
   "linear",
   "smooth",
   "fast",
   "bubble",
   NULL
};

};

node_parameters
{
   AiParameterFLT("threshold", 0.0f);
   AiParameterFLT("amplitude", 1.0f);
   AiParameterFLT("ratio", 0.707f);
   AiParameterFLT("frequencyRatio", 2.0f);
   AiParameterINT("depthMax", 3);
   AiParameterBOOL("inflection", false);
   AiParameterFLT("time", 0.0f);
   AiParameterFLT("frequency", 8.0);
   AiParameterFLT("implode", 0.0f);
   AiParameterPNT2("implodeCenter", 0.5f, 0.5f);
   AiParameterENUM("noiseType", 1, noise_type_enum);
   AiParameterFLT("density", 1.0f);
   AiParameterFLT("spottyness", 0.1f);
   AiParameterFLT("sizeRand", 0.0f);
   AiParameterFLT("randomness", 1.0f);
   AiParameterENUM("falloff", 2, falloff_type_enum);
   AiParameterINT("numWaves", 6);
   AiParameterPNT2("uvCoord", 0.0f, 0.0f);
   AddMayaColorBalanceParams(params);

   AiMetaDataSetStr(mds, NULL, "maya.name", "noise");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x52544e33);
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
      AtPoint2 uv = AI_P2_ZERO;
      if (!AiNodeGetLink(node, "uvCoord.x")) uv.x = UV_GLOBALS;
      if (!AiNodeGetLink(node, "uvCoord.y")) uv.y = UV_GLOBALS;
      AiNodeSetPnt2(node, "uvCoord", uv.x, uv.y);
   }
}

node_finish
{
}

shader_evaluate
{
   AtPoint2 uv;
   uv = AiShaderEvalParamPnt2(p_uvCoord);
   // Will be set to GLOBALS by update if unconnected
   if (uv.x == UV_GLOBALS) uv.x = sg->u;
   if (uv.y == UV_GLOBALS) uv.y = sg->v;

   if (!IsValidUV(uv))
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);
      return;
   }

   float threshold = AiShaderEvalParamFlt(p_threshold);
   float amplitude = AiShaderEvalParamFlt(p_amplitude);
   float ratio = AiShaderEvalParamFlt(p_ratio);
   float time = AiShaderEvalParamFlt(p_time);
   float frequencyRatio = AiShaderEvalParamFlt(p_frequencyRatio);
   int depthMax = AiShaderEvalParamInt(p_depthMax);
   bool inflection = AiShaderEvalParamBool(p_inflection);
   float implode = AiShaderEvalParamFlt(p_implode);
   float frequency = AiShaderEvalParamFlt(p_frequency);
   AtPoint2 implodeCenter = AiShaderEvalParamPnt2(p_implodeCenter);
   int noiseType = AiShaderEvalParamInt(p_noiseType);
   float density = AiShaderEvalParamFlt(p_density);
   float spottyness = AiShaderEvalParamFlt(p_spottyness);
   float sizeRand = AiShaderEvalParamFlt(p_sizeRand);
   float randomness = AiShaderEvalParamFlt(p_randomness);
   int falloff = AiShaderEvalParamInt(p_falloff);
   int numWaves = AiShaderEvalParamInt(p_numWaves);

   float ss = uv.x;
   float tt = uv.y;

   AtPoint pp;
   AiV3Create(pp, ss, tt, 0.0f);

   if (implode)
   {
      AtPoint icp;
      AiV3Create(icp, implodeCenter.x, implodeCenter.y, 0.0f);
      AtVector dp = pp - icp;
      dp = dp / pow(AiV3Length(dp), implode);
      pp = dp + icp;
   }

   int depth[2] = {0, depthMax};
   float ripples[3] = {2, 2, 2};
   AtPoint pn = pp * frequency;

   float noiseVal = 0.0f;

   switch (noiseType)
   {
   case NT_PERLIN:
      if (inflection)
      {
         noiseVal = amplitude * fTurbulence(sg, pn, time, 1.0f, frequencyRatio, depth, ratio, ripples);
      }
      else
      {
         noiseVal = fBm(sg, pn, time, amplitude, depth, 1, frequencyRatio, ratio);
      }
      break;
   case NT_BILLOW:
      {
         float radius = sqrt(0.5f * density);
         noiseVal = BillowNoise(pn, time, 2, radius, sizeRand, randomness, falloff, spottyness, depthMax, frequencyRatio, ratio, amplitude);
      }
      break;
   case NT_WAVE:
      {
         float x = ss * frequency;
         float y = tt * frequency;
         float amp = amplitude;

         float waveValue = 0.0f;
         int i;

         for (i=0; i<depthMax; ++i)
         {
            float noiseValue = CosWaves(x, y, time, numWaves);
            x *= frequencyRatio;
            y *= frequencyRatio;

            if (inflection)
            {
               noiseValue = fabs(noiseValue);
            }

            waveValue += amp * noiseValue;
            amp *= ratio;
         }

         if (!inflection)
         {
            waveValue = 0.5f * waveValue + 0.5f;
         }
         
         noiseVal = waveValue;
      }
      break;
   case NT_WISPY:
      pn += AiPerlin3(pn / 2.0f) * 1.3f;
      if (inflection)
      {
         noiseVal = amplitude * fTurbulence(sg, pn, time, 1.0f, frequencyRatio, depth, ratio, ripples);
      }
      else
      {
         noiseVal = fBm(sg, pn, time, amplitude, depth, 1, frequencyRatio, ratio);
      }
      break;
   case NT_SPACETIME:
      if (inflection)
      {
         noiseVal = amplitude * fTurbulence(sg, pn, time, 1.0f, frequencyRatio, depth, ratio, ripples);
      }
      else
      {
         noiseVal = fBm(sg, pn, time, amplitude, depth, 1, frequencyRatio, ratio);
      }
      break;
   }

   noiseVal = CLAMP(noiseVal + threshold, 0.0f, 1.0f);

   AiRGBACreate(sg->out.RGBA, noiseVal, noiseVal, noiseVal, noiseVal);
   MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);
}
