
#include "MayaUtils.h"
#include <ai_nodes.h>
#include <ai_shaderglobals.h>
#include <ai_shaders.h>
#include <ai_shader_parameval.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <algorithm>

AI_SHADER_NODE_EXPORT_METHODS(MayaRampMtd);

namespace
{

enum RampParameters
{
   p_type = 0,
   p_interp,
   p_positions,
   p_colors,
   p_u_wave,
   p_v_wave,
   p_color_gain,
   p_color_offset,
   p_alpha_gain,
   p_alpha_offset,
   p_default_color,
   p_invert,
   p_override_uv,
   p_uv,
   p_noise,
   p_noise_freq
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

const char* ramp_type_enum[] =
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

inline float mod(float n, float d)
{
   return (n - (floor(n / d) * d));
}

};

node_parameters
{
   AiParameterENUM("type", 1, ramp_type_enum);
   AiParameterENUM("interpolation", 1, RampInterpolationNames);
   AiParameterARRAY("positions", AiArray(3, 1, AI_TYPE_FLOAT, 0.0f, 0.5f, 1.0f));
   AtArray *cdef = AiArrayAllocate(3, 1, AI_TYPE_RGB);
   AiArraySetRGB(cdef, 0, AI_RGB_RED);
   AiArraySetRGB(cdef, 1, AI_RGB_GREEN);
   AiArraySetRGB(cdef, 2, AI_RGB_BLUE);
   AiParameterARRAY("colors", cdef);
   AiParameterFLT("uWave", 0.0f);
   AiParameterFLT("vWave", 0.0f);
   AiParameterRGB("colorGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("colorOffset", 0.0f, 0.0f, 0.0f);
   AiParameterFLT("alphaGain", 1.0f);
   AiParameterFLT("alphaOffset", 0.0f);
   AiParameterRGB("defaultColor", 0.5f, 0.5f, 0.5f);
   AiParameterBOOL("invert", false);
   AiParameterBOOL("overrideUV", false);
   AiParameterPNT2("uv", 0.0f, 0.0f);
   AiParameterFLT("noise", 0.0f);
   AiParameterFLT("noiseFreq", 0.5f);
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

   float u = sg->u;
   float v = sg->v;

   if (AiShaderEvalParamBool(p_override_uv))
   {
      AtPoint2 uv = AiShaderEvalParamPnt2(p_uv);
      u = uv.x;
      v = uv.y;
   }

   if (fabs(u) >= 1000000.0f || fabs(v) >= 1000000.0f)
   {
      AtRGB dc = AiShaderEvalParamRGB(p_default_color);
      sg->out.RGBA.r = dc.r;
      sg->out.RGBA.g = dc.g;
      sg->out.RGBA.b = dc.b;
      sg->out.RGBA.a = 1.0f;
      return;
   }

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
      v += vWave * sin(u * 2.0f * M_PI);
      v = mod(v, 1);
   }

   if (uWave > 0.0f)
   {
      u -= uWave * sin(preWaveV * 2.0f * M_PI);
      u = mod(u, 1);
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
         float t = a / (2.0f * M_PI);
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
         float t = 2.0f * std::max(fabs(u-0.5f), fabs(v-0.5f));
         Ramp(positions, colors, t, interp, result);
      }
      break;
   case RT_UV:
      {
         float t0 = 2.0f * fabs(u - 0.5);
         float t1 = 2.0f * fabs(v - 0.5);
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
         float t0 = 2.0f * fabs(u - 0.5);
         float t1 = 2.0f * fabs(v - 0.5);
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

   // Do color balance
   AtRGB cg = AiShaderEvalParamRGB(p_color_gain);
   AtRGB co = AiShaderEvalParamRGB(p_color_offset);
   float ag = AiShaderEvalParamFlt(p_alpha_gain);
   float ao = AiShaderEvalParamFlt(p_alpha_offset);
   bool invert = AiShaderEvalParamBool(p_invert);

   if (invert)
   {
      result.r = 1.0f - result.r;
      result.g = 1.0f - result.g;
      result.b = 1.0f - result.b;
   }
   float alpha = Luminance(result);

   sg->out.RGBA.r = (result.r * cg.r) + co.r;
   sg->out.RGBA.g = (result.g * cg.g) + co.g;
   sg->out.RGBA.b = (result.b * cg.b) + co.b;
   sg->out.RGBA.a = (alpha * ag) + ao;
}
