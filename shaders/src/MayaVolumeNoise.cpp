#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaVolumeNoiseMtd);

namespace
{

enum MayaVolumeNoiseParams
{
   p_threshold,
   p_amplitude,
   p_ratio,
   p_frequencyRatio,
   p_depthMax,
   p_inflection,
   p_time,
   p_frequency,
   p_scale,
   p_origin,
   p_implode,
   p_implodeCenter,
   p_noiseType,
   p_density,
   p_spottyness,
   p_sizeRand,
   p_randomness,
   p_falloff,
   p_numWaves,
   p_placementMatrix,
   p_wrap,
   p_local,
   MAYA_COLOR_BALANCE_ENUM
};

enum NoiseType
{
   NT_PERLIN = 0,
   NT_BILLOW,
   NT_VOLUME_WAVE,
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

static const char* NoiseTypeNames[] =
{
   "Perlin",
   "Billow",
   "VolumeWave",
   "Wispy",
   "SpaceTime"
};

static const char* FalloffTypeNames[] =
{
   "Linear",
   "Smooth",
   "Fast",
   "Bubble"
};

};

node_parameters
{
   AtMatrix id;
   AiM4Identity(id);

   AiParameterFLT("threshold", 0.0f);
   AiParameterFLT("amplitude", 1.0f);
   AiParameterFLT("ratio", 0.707f);
   AiParameterFLT("frequencyRatio", 2.0f);
   AiParameterINT("depthMax", 3);
   AiParameterBOOL("inflection", false);
   AiParameterFLT("time", 0.0f);
   AiParameterFLT("frequency", 8.0f);
   AiParameterVEC("scale", 1.0f, 1.0f, 1.0f);
   AiParameterPNT("origin", 0.0f, 0.0f, 0.0f);
   AiParameterFLT("implode", 0.0f);
   AiParameterPNT("implodeCenter", 0.0f, 0.0f, 0.0f);
   AiParameterENUM("noiseType", 1, NoiseTypeNames);
   AiParameterFLT("density", 1.0f); // for: billow
   AiParameterFLT("spottyness", 0.1f); // for: billow
   AiParameterFLT("sizeRand", 0.0f); // for: billow
   AiParameterFLT("randomness", 1.0f); // for: billow
   AiParameterENUM("falloff", 2, FalloffTypeNames); // for: billow
   AiParameterINT("numWaves", 5); // for: VolumeWave
   AiParameterMTX("placementMatrix", id);
   AiParameterBOOL("wrap", true);
   AiParameterBOOL("local", false);
   AddMayaColorBalanceParams(params);
   
   AiMetaDataSetStr(mds, NULL, "maya.counterpart", "volumeNoise");
   AiMetaDataSetInt(mds, NULL, "maya.counterpart_id", 0x52545633);
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
   AtFloat threshold = AiShaderEvalParamFlt(p_threshold);
   AtFloat amplitude = AiShaderEvalParamFlt(p_amplitude);
   AtFloat ratio = AiShaderEvalParamFlt(p_ratio);
   AtFloat frequencyRatio = AiShaderEvalParamFlt(p_frequencyRatio);
   AtInt depthMax = AiShaderEvalParamInt(p_depthMax);
   AtBoolean inflection = AiShaderEvalParamBool(p_inflection);
   AtFloat time = AiShaderEvalParamFlt(p_time);
   AtFloat frequency = AiShaderEvalParamFlt(p_frequency);
   AtPoint origin = AiShaderEvalParamPnt(p_origin);
   AtVector scale = AiShaderEvalParamVec(p_scale);
   AtFloat implode = AiShaderEvalParamFlt(p_implode);
   AtPoint implodeCenter = AiShaderEvalParamPnt(p_implodeCenter);
   AtInt noiseType = AiShaderEvalParamInt(p_noiseType);
   AtFloat density = AiShaderEvalParamFlt(p_density);
   AtFloat spottyness = AiShaderEvalParamFlt(p_spottyness);
   AtFloat sizeRand = AiShaderEvalParamFlt(p_sizeRand);
   AtFloat randomness = AiShaderEvalParamFlt(p_randomness);
   AtInt falloff = AiShaderEvalParamInt(p_falloff);
   AtInt numWaves = AiShaderEvalParamInt(p_numWaves);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   AtBoolean wrap = AiShaderEvalParamBool(p_wrap);
   AtBoolean local = AiShaderEvalParamBool(p_local);

   AtPoint P;
   AtMatrix placement;

   AiM4Invert(*placementMatrix, placement);

   AiM4PointByMatrixMult(&P, placement, (local ? &(sg->Po) : &(sg->P)));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      // should we transform implodeCenter and origin?
      // is this the right order?
      P -= origin;
      P /= scale;

      if (implode)
      {
         AtVector dp = P - implodeCenter;
         dp = dp / pow(AiV3Length(dp), implode);
         P = implodeCenter + dp;
      }

      AtInt depth[2] = {0, depthMax};
      AtFloat ripples[3] = {1.0f, 1.0f, 1.0f};

      P *= frequency;

      AtFloat noiseVal = 0.0f;

      switch (noiseType)
      {
      case NT_PERLIN:
         if (inflection)
         {
            noiseVal = amplitude * fTurbulence(sg, P, time, 1.0f, frequencyRatio, depth, ratio, ripples);
         }
         else
         {
            noiseVal = fBm(sg, P, time, amplitude, depth, 1.0f, frequencyRatio, ratio);
         }
         break;
      case NT_BILLOW:
         {
            AtFloat radius = static_cast<AtFloat>(sqrt(0.5f * density));
            noiseVal = BillowNoise(P, time, 3, radius, sizeRand, randomness, falloff, spottyness, depthMax, frequencyRatio, ratio, amplitude);
         }
         break;
      case NT_VOLUME_WAVE:
         {
            float amp = amplitude;
            float timeRatio = 1.0f / frequencyRatio;
            float twopi = 2.0f * static_cast<float>(AI_PI);

            noiseVal = 0.0f;
            time *= twopi;

            for (int i=0; i<depthMax; ++i)
            {
               float waveVal = 0.0f;

               for (int j=0; j<numWaves; ++j)
               {
                  float tmp = twopi * (0.5f * (1 + i) * (1 + j));

                  AtVector v, d;

                  AiV3Create(v, tmp, 0, 0);
                  d.x = AiPerlin3(v);
                  
                  AiV3Create(v, 0, tmp, 0);
                  d.y = AiPerlin3(v);
                  
                  AiV3Create(v, 0, 0, tmp);
                  d.z = AiPerlin3(v);

                  AiV3Normalize(d, d);

                  waveVal += (float) cos(twopi * AiV3Dot(P, d) + time);
               }

               waveVal /= numWaves;

               if (inflection)
               {
                  waveVal = fabs(waveVal);
               }

               noiseVal += amp * waveVal;

               amp *= ratio;
               P *= frequencyRatio;
               time *= timeRatio;
            }

            if (!inflection)
            {
               noiseVal = 0.5f * noiseVal + 0.5f;
            }
         }
         break;
      case NT_WISPY:
         P += AiPerlin3(P / 2.0f) * 1.3f;
         if (inflection)
         {
            noiseVal = amplitude * fTurbulence(sg, P, time, 1.0f, frequencyRatio, depth, ratio, ripples);
         }
         else
         {
            noiseVal = fBm(sg, P, time, amplitude, depth, 1.0f, frequencyRatio, ratio);
         }
         break;
      case NT_SPACETIME:
         if (inflection)
         {
            noiseVal = amplitude * fTurbulence(sg, P, time, 1.0f, frequencyRatio, depth, ratio, ripples);
         }
         else
         {
            noiseVal = fBm(sg, P, time, amplitude, depth, 1.0f, frequencyRatio, ratio);
         }
         break;
      }

      noiseVal = CLAMP(noiseVal+threshold, 0.0f, 1.0f);

      AiRGBACreate(sg->out.RGBA, noiseVal, noiseVal, noiseVal, 1.0f);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);
   }
}

