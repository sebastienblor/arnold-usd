#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaFractalMtd);

namespace
{

enum MayaFractalParams
{
   p_amplitude,
   p_threshold,
   p_ratio,
   p_frequencyRatio,
   p_levelMin,
   p_levelMax,
   p_bias,
   p_inflection,
   p_animated,
   p_time,
   p_timeRatio,
   p_uvCoord,
   MAYA_COLOR_BALANCE_ENUM
};

inline float get2DNoise(float x, float y, float px, float py, AtBoolean inflection)
{
   float noise;
   AtPoint2 p;
   p.x = x;
   p.y = y;

   noise = AiPeriodicPerlin2(p, (AtInt)px, (AtInt)py);
   if (inflection) noise = fabs(noise);

   return noise;
}

inline float get3DNoise(float x, float y, float z, float px, float py, float pz, AtBoolean inflection)
{
   float noise;
   AtPoint p;
   AiV3Create(p, x, y, z);

   noise = AiPeriodicPerlin3(p, (AtInt)px, (AtInt)py, (AtInt)pz);  
   if (inflection) noise = fabs(noise);

   return noise;
}

};

node_parameters
{
   AiParameterFLT("amplitude", 1.0f);
   AiParameterFLT("threshold", 0.0f);
   AiParameterFLT("ratio", 0.707f);
   AiParameterFLT("frequencyRatio", 2.0f);
   AiParameterFLT("levelMin", 0.0f);
   AiParameterFLT("levelMax", 9.0f);
   AiParameterFLT("bias", 0.0f);
   AiParameterBOOL("inflection", false);
   AiParameterBOOL("animated", false);
   AiParameterFLT("time", 0.0f);
   AiParameterFLT("timeRatio", 2.0f);
   AiParameterPNT2("uvCoord", 0.0f, 0.0f);
   AddMayaColorBalanceParams(params);

   AiMetaDataSetStr(mds, NULL, "maya.name", "fractal");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x52543246);
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

   float amplitude = AiShaderEvalParamFlt(p_amplitude);
   float ratio = AiShaderEvalParamFlt(p_ratio);
   float frequencyRatio = AiShaderEvalParamFlt(p_frequencyRatio);
   float levelMin = AiShaderEvalParamFlt(p_levelMin);
   float levelMax = AiShaderEvalParamFlt(p_levelMax);
   float threshold = AiShaderEvalParamFlt(p_threshold);
   AtBoolean animated = AiShaderEvalParamBool(p_animated);
   AtBoolean inflection = AiShaderEvalParamBool(p_inflection);

   float ss = uv.x;
   float tt = uv.y;
   ss = Mod(ss, 1.000001f);
   tt = Mod(tt, 1.000001f);

   AtPoint pp;
   pp.x = ss * 15.0f;
   pp.y = tt * 15.0f;
   pp.z = 0.0f;

   float curFreq = 10.0f;
   float curTimeFreq = 10.0f;
   float pixelSize = float(AI_EPSILON);
   float nyquist = 2.0f * pixelSize;
   float pixel = 1.0f;

   float time = 0.0f;
   float timeRatio = 0.0f;

   if (animated)
   {
      time = AiShaderEvalParamFlt(p_time);
      timeRatio = AiShaderEvalParamFlt(p_timeRatio);
   }

   int counter = 0;
   float noiseval = 0.0f;
   while ((counter < levelMax && pixel > nyquist) || (counter < levelMin))
   {
      float noise;
      if (animated)
      {
         noise = get3DNoise(ss * curFreq, tt * curFreq, time * curTimeFreq, curFreq, curFreq, curTimeFreq, inflection);
         curTimeFreq *= timeRatio;
      }
      else
      {
         noise = get2DNoise(ss * curFreq, tt * curFreq, curFreq, curFreq, inflection);
      }

      noiseval += amplitude * noise;
      curFreq *= frequencyRatio;
      amplitude *= ratio;
      pixel /= frequencyRatio;
      ++counter;
   }

   if (pixel > pixelSize && counter <= levelMax)
   {
      float noise;
      if (animated)
      {
         noise = get3DNoise(ss * curFreq, tt * curFreq, time * curTimeFreq, curFreq, curFreq, curTimeFreq, inflection);
      }
      else
      {
         noise = get2DNoise(ss * curFreq, tt * curFreq, curFreq, curFreq, inflection);
      }
      noiseval += amplitude * noise;
   }

   if (!inflection)
   {
      noiseval = (noiseval * 0.5f + 0.5f);
   }
   noiseval += threshold;
   noiseval = CLAMP(noiseval, 0.0f, 1.0f);
     
   float n = noiseval;
   
   AiRGBACreate(sg->out.RGBA, n, n, n, 1.0f);
   MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);
}

