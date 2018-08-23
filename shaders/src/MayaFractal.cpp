#include <ai.h>

#include <limits.h>

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

inline float get2DNoise(float x, float y, float px, float py, bool inflection)
{
   float noise;
   AtVector2 p;
   p.x = x;
   p.y = y;

   noise = AiPeriodicPerlin2(p, (int)px, (int)py);
   if (inflection) noise = fabs(noise);

   return noise;
}

inline float get3DNoise(float x, float y, float z, float px, float py, float pz, bool inflection)
{
   float noise;
   AtVector p(x, y, z);

   noise = AiPeriodicPerlin3(p, (int)px, (int)py, (int)pz);  
   if (inflection) noise = fabs(noise);

   return noise;
}

};

node_parameters
{
   AiParameterFlt("amplitude", 1.0f);
   AiParameterFlt("threshold", 0.0f);
   AiParameterFlt("ratio", 0.707f);
   AiParameterFlt("frequencyRatio", 2.0f);
   AiParameterFlt("levelMin", 0.0f);
   AiParameterFlt("levelMax", 9.0f);
   AiParameterFlt("bias", 0.0f);
   AiParameterBool("inflection", false);
   AiParameterBool("animated", false);
   AiParameterFlt("time", 0.0f);
   AiParameterFlt("timeRatio", 2.0f);
   AiParameterVec2("uvCoord", 0.0f, 0.0f);
   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "fractal");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52543246);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   
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

   float amplitude = AiShaderEvalParamFlt(p_amplitude);
   float ratio = AiShaderEvalParamFlt(p_ratio);
   float frequencyRatio = AiShaderEvalParamFlt(p_frequencyRatio);
   float levelMin = AiShaderEvalParamFlt(p_levelMin);
   float levelMax = AiShaderEvalParamFlt(p_levelMax);
   float threshold = AiShaderEvalParamFlt(p_threshold);
   bool animated = AiShaderEvalParamBool(p_animated);
   bool inflection = AiShaderEvalParamBool(p_inflection);

   float ss = uv.x;
   float tt = uv.y;
   ss = Mod(ss, 1.000001f);
   tt = Mod(tt, 1.000001f);

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
   
   
   float maxP = (fabsf(ss) > fabsf(tt)) ? fabsf(ss) : fabsf(tt);
   
   while ((counter < levelMax && pixel > nyquist) || (counter < levelMin))
   {
      float noise;
      if((maxP * curFreq) >= LONG_MAX || (time * curTimeFreq) >= LONG_MAX)
         break;
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

   if (pixel > pixelSize && counter <= levelMax && !(((maxP * curFreq) >= LONG_MAX || (time * curTimeFreq) >= LONG_MAX)))
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
   noiseval = AiClamp(noiseval, 0.0f, 1.0f);
     
   float n = noiseval;
   
   sg->out.RGBA() = AtRGBA(n, n, n, 1.0f);
   MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
}

