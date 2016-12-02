#include <ai.h>

#include <limits.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaSolidFractalMtd);

namespace
{

enum MayaSolidFractalParams
{
   p_threshold,
   p_amplitude,
   p_ratio,
   p_frequencyRatio,
   p_ripples,
   p_depth,
   p_bias,
   p_inflection,
   p_animated,
   p_time,
   p_timeRatio,
   p_placementMatrix,
   p_wrap,
   p_local,
   MAYA_COLOR_BALANCE_ENUM
};

static float Noise3D(const AtVector &p, bool inflection)
{
   float noise = AiPerlin3(p);

   if (inflection)
   {
      noise = fabs(noise);
   }

   return noise;
}

static float Noise4D(const AtVector &p, float time, bool inflection)
{
   float noise = AiPerlin4(p, time);  

   if (inflection)
   {
      noise = fabs(noise);
   }

   return noise;
}

};

node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterFlt("threshold", 0.0f);
   AiParameterFlt("amplitude", 1.0f);
   AiParameterFlt("ratio", 0.707f);
   AiParameterFlt("frequencyRatio", 2.0f);
   AiParameterVec("ripples", 1.0f, 1.0f, 1.0f);
   AiParameterVec2("depth", 0.0f, 8.0f);
   AiParameterFlt("bias", 0.0f);
   AiParameterBool("inflection", false);
   AiParameterBool("animated", false);
   AiParameterFlt("time", 0.0f);
   AiParameterFlt("timeRatio", 2.0f);
   AiParameterMtx("placementMatrix", id);
   AiParameterBool("wrap", true);
   AiParameterBool("local", false);
   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "solidFractal");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544633);
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
   float threshold = AiShaderEvalParamFlt(p_threshold);
   float amplitude = AiShaderEvalParamFlt(p_amplitude);
   float ratio = AiShaderEvalParamFlt(p_ratio);
   float frequencyRatio = AiShaderEvalParamFlt(p_frequencyRatio);
   AtVector ripples = AiShaderEvalParamVec(p_ripples);
   AtVector2 depth = AiShaderEvalParamVec2(p_depth);
   float bias = AiShaderEvalParamFlt(p_bias);
   bool inflection = AiShaderEvalParamBool(p_inflection);
   bool animated = AiShaderEvalParamBool(p_animated);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   bool wrap = AiShaderEvalParamBool(p_wrap);
   bool local = AiShaderEvalParamBool(p_local);

   float time = 0.0f;
   float timeRatio = 0.0f;

   AtVector tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AtVector P = AiM4PointByMatrixMult(*placementMatrix, (local ? sg->Po : sg->P));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      float i = 0.0f;
      float tmp = 0.0f;
      float curAmp = amplitude;
      float curFreq = 1.0f;
      float curTimeFreq = 1.0f;
      float noise = 0.0f;
      float pixelSize = static_cast<float>(AI_EPSILON);
      float nyquist = 2.0f * pixelSize;
      float pixel = 1.0f;

      P *= 0.5f * ripples;
      
      float maxP = (fabsf(P.x) > fabsf(P.y)) ? fabsf(P.x) : fabsf(P.y);
      maxP = (maxP > fabsf(P.z)) ? maxP : fabsf(P.z);

      if (animated)
      {
         time = AiShaderEvalParamFlt(p_time);
         timeRatio = AiShaderEvalParamFlt(p_timeRatio);
      }

      while ((i < depth.x) || (i < depth.y && pixel > nyquist))
      {
         if((maxP * curFreq) >= LONG_MAX)
            break;
            
         AtVector sampleP = P * curFreq;

         if (animated)
         {
            tmp = Noise4D(sampleP, time * curTimeFreq, inflection);
            curTimeFreq *= timeRatio;
         }
         else
         {
            tmp = Noise3D(sampleP, inflection);
         }

         noise += curAmp * tmp;
         curFreq *= frequencyRatio;
         curAmp *= ratio;
         pixel /= frequencyRatio;

         i += 1.0f;
      }
      if ((maxP * curFreq) < LONG_MAX && pixel > pixelSize && i <= depth.y)
      {
         AtVector sampleP = P * curFreq;

         if (animated)
         {
            tmp = Noise4D(sampleP, time * curTimeFreq, inflection);
         }
         else
         {
            tmp = Noise3D(sampleP, inflection);
         }

         noise += curAmp * tmp;
      }

      if (!inflection)
      {
         noise = (noise * 0.5f + 0.5f);
      }

      noise += threshold;

      float a = 1.0f + bias;
      float b = -0.5f * bias;
      float x1 = -b / a;
      float x2 = (1.0f - b) / a;
      
      if (noise < x1)
      {
         noise = 0.0f;
      }
      else if (noise > x2)
      {
         noise = 1.0f;
      }
      else
      {
         noise = a * noise + b;
      }

      noise = CLAMP(noise, 0.0f, 1.0f);

      sg->out.RGBA() = AtRGBA(noise, noise, noise, 1.0f);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA());
   }
   if (usePref) RestorePoints(sg, tmpPts);
}

