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
   p_u_wave,
   p_v_wave,
   p_uvCoord,
   p_noise,
   p_noise_freq,
   MAYA_COLOR_BALANCE_ENUM,
   p_numEntries,
   p_position0,
   p_position1,
   p_position2,
   p_position3,
   p_position4,
   p_position5,
   p_position6,
   p_position7,
   p_color0,
   p_color1,
   p_color2,
   p_color3,
   p_color4,
   p_color5,
   p_color6,
   p_color7
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
   AiParameterFLT("uWave", 0.0f);
   AiParameterFLT("vWave", 0.0f);
   AiParameterPNT2("uvCoord", 0.0f, 0.0f);
   AiParameterFLT("noise", 0.0f);
   AiParameterFLT("noiseFreq", 0.5f);

   AiParameterUINT("numEntries", 0);
   AiParameterFLT("position0", 0.0f);
   AiParameterFLT("position1", 0.0f);
   AiParameterFLT("position2", 0.0f);
   AiParameterFLT("position3", 0.0f);
   AiParameterFLT("position4", 0.0f);
   AiParameterFLT("position5", 0.0f);
   AiParameterFLT("position6", 0.0f);
   AiParameterFLT("position7", 0.0f);

   AiParameterRGB("color0", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color1", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color2", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color3", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color4", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color5", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color6", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color7", 0.0f, 0.0f, 0.0f);

   AddMayaColorBalanceParams(params);

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
   AtRGB result = AI_RGB_BLACK;
   // Read positions and colors
   // Not using array and AiShaderEvalParamArray to allow individual connections
   AtUInt numEntries = AiShaderEvalParamUInt(p_numEntries);
   if (numEntries > 0)
   {
      AtArray *positions = AiArrayAllocate(numEntries, 1, AI_TYPE_FLOAT);
      AtArray *colors = AiArrayAllocate(numEntries, 1, AI_TYPE_RGB);
      for (AtUInt32 i=0; i<numEntries; ++i)
      {
         AiArraySetFlt(positions, i, AiShaderEvalParamFlt(p_position0+i));
         AiArraySetRGB(colors, i, AiShaderEvalParamRGB(p_color0+i));
      }
      // Sort the arrays, since positions can be connected, order can change
      // Sort position array
      if (numEntries > 1)
      {
         AtUInt* shuffle = new AtUInt[positions->nelements];
         if (SortFloatArray(positions, shuffle))
         {
            ShuffleArray(colors, shuffle, AI_TYPE_RGB);
         }
         delete[] shuffle;
      }

      float u = sg->u;
      float v = sg->v;

      if (AiNodeGetLink(node, "uvCoord") ||
          AiNodeGetLink(node, "uvCoord.x") ||
          AiNodeGetLink(node, "uvCoord.y"))
      {
         AtPoint2 uv = AiShaderEvalParamPnt2(p_uvCoord);
         if (AiNodeGetLink(node, "uvCoord") || AiNodeGetLink(node, "uvCoord.x")) u = uv.x;
         if (AiNodeGetLink(node, "uvCoord") || AiNodeGetLink(node, "uvCoord.y")) v = uv.y;
      }

      if (!IsValidUV(u, v))
      {
         MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);
         return;
      }

      u = Mod(u, 1.000001f);
      v = Mod(v, 1.000001f);

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

      AtInt type = AiShaderEvalParamInt(p_type);
      RampInterpolationType interp = (RampInterpolationType) AiShaderEvalParamInt(p_interp);
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
   }

   AiRGBtoRGBA(result, sg->out.RGBA);
   // Alpha output is always the luminance
   // TODO: js - Fix this for the new function.
   // alphaIsLuminance = true;
   MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);
}
