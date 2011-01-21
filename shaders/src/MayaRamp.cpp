#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaRampMtd);

namespace
{

enum MayaRampParams
{
   p_type = 0,
   p_interp,
   p_positions,
   p_colors,
   p_u_wave,
   p_v_wave,
   p_uvCoord,
   p_noise,
   p_noise_freq,
   MAYA_COLOR_BALANCE_ENUM
};

enum RampType
{
   RT_V = 0,
   RT_U,
   RT_DIAGONAL,
   RT_RADIAL,
   RT_CIRCULAR,
   RT_BOX,
   RT_UV,
   RT_4CORNER,
   RT_TARTAN
};

const char* RampTypeNames[] =
{
   "v",
   "u",
   "diagonal",
   "radial",
   "circular",
   "box",
   "uv",
   "fourcorner",
   "tartan",
   NULL
};

};

node_parameters
{
   AiParameterENUM("type", 1, RampTypeNames);
   AiParameterENUM("interpolation", 1, RampInterpolationNames);
   AiParameterARRAY("positions", AiArray(3, 1, AI_TYPE_FLOAT, 0.0f, 0.5f, 1.0f));
   AtArray *cdef = AiArrayAllocate(3, 1, AI_TYPE_RGB);
   AiArraySetRGB(cdef, 0, AI_RGB_RED);
   AiArraySetRGB(cdef, 1, AI_RGB_GREEN);
   AiArraySetRGB(cdef, 2, AI_RGB_BLUE);
   AiParameterARRAY("colors", cdef);
   AiParameterFLT("uWave", 0.0f);
   AiParameterFLT("vWave", 0.0f);
   AiParameterPNT2("uvCoord", 0.0f, 0.0f);
   AiParameterFLT("noise", 0.0f);
   AiParameterFLT("noiseFreq", 0.5f);
   MAYA_COLOR_BALANCE_PARAMS

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
   AtInt type = AiShaderEvalParamInt(p_type);
   RampInterpolationType interp = (RampInterpolationType) AiShaderEvalParamInt(p_interp);
   AtArray *positions = AiShaderEvalParamArray(p_positions);
   AtArray *colors = AiShaderEvalParamArray(p_colors);
   EVAL_MAYA_COLOR_BALANCE_PARAMS

   float u = sg->u;
   float v = sg->v;

   if (AiNodeGetLink(node, "uvCoord") ||
       AiNodeGetLink(node, "uvCoord.x") ||
       AiNodeGetLink(node, "uvCoord.y"))
   {
      AtPoint2 uv = AiShaderEvalParamPnt2(p_uvCoord);
      u = uv.x;
      v = uv.y;
   }

   if (!IsValidUV(u, v))
   {
      MAYA_DEFAULT_COLOR(sg->out.RGBA);
      return;
   }

   u = Mod(u, 1.000001f);
   v = Mod(v, 1.000001f);

   AtRGB result;

   float uWave = AiShaderEvalParamFlt(p_u_wave);
   float vWave = AiShaderEvalParamFlt(p_v_wave);
   float noiseAmp = AiShaderEvalParamFlt(p_noise);
   float noiseFreq = AiShaderEvalParamFlt(p_noise_freq);

   if (noiseAmp > 0.0f)
   {
      AtPoint2 uv;
      uv.x = u * 16 * noiseFreq;
      uv.y = v * 16 * noiseFreq;
      float n = noiseAmp * AiPerlin2(uv);
      u += n;
      v += n;
   }

   float preWaveV = v;

   if (vWave > 0.0f)
   {
      v += vWave * static_cast<float>(sin(u * AI_PITIMES2));
   }

   if (uWave > 0.0f)
   {
      u -= uWave * static_cast<float>(sin(preWaveV * AI_PITIMES2));
   }

   switch (type)
   {
   case RT_U:
      Ramp(positions, colors, u, interp, result);
      break;
   case RT_V:
      Ramp(positions, colors, v, interp, result);
      break;
   case RT_DIAGONAL:
      {
         float t = 0.5f * (u + v);
         Ramp(positions, colors, t, interp, result);
      }
      break;
   case RT_RADIAL:
      {
         float x = u - 0.5f;
         float y = v - 0.5f;
         float a = atan2(y, x);
         float t = static_cast<float>(a * AI_ONEOVER2PI);
         t = - t - 0.25f;
         if (t < 0.0f)
            t += 1.0f;
         Ramp(positions, colors, t, interp, result);
      }
      break;
   case RT_CIRCULAR:
      {
         float x = u - 0.5f;
         float y = v - 0.5f;
         float t = float(sqrt(2.0) * sqrt(x*x + y*y));
         Ramp(positions, colors, t, interp, result);
      }
      break;
   case RT_BOX:
      {
         float t = 2.0f * MAX(fabs(u-0.5f), fabs(v-0.5f));
         Ramp(positions, colors, t, interp, result);
      }
      break;
   case RT_UV:
      {
         float t0 = 2.0f * fabs(u - 0.5f);
         float t1 = 2.0f * fabs(v - 0.5f);
         AtRGB c0, c1;
         Ramp(positions, colors, t0, interp, c0);
         Ramp(positions, colors, t1, interp, c1);
         result = c0 * c1;
      }
      break;
   case RT_4CORNER:
      {
         if (positions->nelements < 1)
         {
            result.r = 0.0f;
            result.r = 0.0f;
            result.r = 0.0f;
         }
         else
         {
            result = (1.0f - u) * (1.0f - v) * AiArrayGetRGB(colors, 0);
            if (positions->nelements > 1)
            {
               result = result + (u * (1.0f - v) * AiArrayGetRGB(colors, 1));
            }
            if (positions->nelements > 2)
            {
               result = result + ((1.0f - u) * v * AiArrayGetRGB(colors, 2));
            }
            if (positions->nelements > 3)
            {
               result = result + (u * v * AiArrayGetRGB(colors, 3));
            }
         }
      }
      break;
   case RT_TARTAN:
      {
         float t0 = 2.0f * fabs(u - 0.5f);
         float t1 = 2.0f * fabs(v - 0.5f);
         AtRGB c0, c1;
         Ramp(positions, colors, t0, interp, c0);
         Ramp(positions, colors, t1, interp, c1);
         result = 0.5f * (c0 + c1);
      }
      break;
   default:
      Ramp(positions, colors, v, interp, result);
      break;
   }

   AiRGBtoRGBA(result, sg->out.RGBA);
   // Alpha output is always the luminance
   alphaIsLuminance = true;
   MAYA_COLOR_BALANCE(sg->out.RGBA);
}
