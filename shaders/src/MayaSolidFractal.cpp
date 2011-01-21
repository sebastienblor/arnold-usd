#include <ai.h>

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

static float Noise3D(AtPoint &p, AtBoolean inflection)
{
   AtFloat noise = AiPerlin3(p);

   if (inflection)
   {
      noise = fabs(noise);
   }

   return noise;
}

static float Noise4D(AtPoint &p, float time, AtBoolean inflection)
{
   AtFloat noise = AiPerlin4(p, time);  

   if (inflection)
   {
      noise = fabs(noise);
   }

   return noise;
}

};

node_parameters
{
   AtMatrix id;
   AiM4Identity(id);

   AiParameterFLT("threshold", 0.0f);
   AiParameterFLT("amplitude", 1.0f);
   AiParameterFLT("ratio", 0.707f);
   AiParameterFLT("frequencyRatio", 2.0f);
   AiParameterVEC("ripples", 1.0f, 1.0f, 1.0f);
   AiParameterPNT2("depth", 0.0f, 8.0f);
   AiParameterFLT("bias", 0.0f);
   AiParameterBOOL("inflection", false);
   AiParameterBOOL("animated", false);
   AiParameterFLT("time", 0.0f);
   AiParameterFLT("timeRatio", 2.0f);
   AiParameterMTX("placementMatrix", id);
   AiParameterBOOL("wrap", true);
   AiParameterBOOL("local", false);
   MAYA_COLOR_BALANCE_PARAMS

   AiMetaDataSetStr(mds, NULL, "maya.counterpart", "solidFractal");
   AiMetaDataSetInt(mds, NULL, "maya.counterpart_id", 0x52544633);
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
   AtVector ripples = AiShaderEvalParamVec(p_ripples);
   AtPoint2 depth = AiShaderEvalParamPnt2(p_depth);
   AtFloat bias = AiShaderEvalParamFlt(p_bias);
   AtBoolean inflection = AiShaderEvalParamBool(p_inflection);
   AtBoolean animated = AiShaderEvalParamBool(p_animated);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   AtBoolean wrap = AiShaderEvalParamBool(p_wrap);
   AtBoolean local = AiShaderEvalParamBool(p_local);
   EVAL_MAYA_COLOR_BALANCE_PARAMS

   AtPoint P;
   AtMatrix placement;
   AtFloat time;
   AtFloat timeRatio;

   AiM4Invert(*placementMatrix, placement);

   AiM4PointByMatrixMult(&P, placement, (local ? &(sg->Po) : &(sg->P)));

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

      if (animated)
      {
         time = AiShaderEvalParamFlt(p_time);
         timeRatio = AiShaderEvalParamFlt(p_timeRatio);
      }

      while ((i < depth.x) || (i < depth.y && pixel > nyquist))
      {
         if (animated)
         {
            tmp = Noise4D(P * curFreq, time * curTimeFreq, inflection);
            curTimeFreq *= timeRatio;
         }
         else
         {
            tmp = Noise3D(P * curFreq, inflection);
         }

         noise += curAmp * tmp;
         curFreq *= frequencyRatio;
         curAmp *= ratio;
         pixel /= frequencyRatio;

         i += 1.0f;
      }

      if (pixel > pixelSize && i <= depth.y)
      {
         if (animated)
         {
            tmp = Noise4D(P * curFreq, time * curTimeFreq, inflection);
         }
         else
         {
            tmp = Noise3D(P * curFreq, inflection);
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

      AiRGBACreate(sg->out.RGBA, noise, noise, noise, 1.0f);
      MAYA_COLOR_BALANCE(sg->out.RGBA);
   }
   else
   {
      MAYA_DEFAULT_COLOR(sg->out.RGBA);
   }
}

