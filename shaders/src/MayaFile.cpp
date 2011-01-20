
#include "MayaUtils.h"
#include <ai.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

AI_SHADER_NODE_EXPORT_METHODS(MayaFileMtd);

namespace
{

enum FileParams
{
   p_coverage = 0,
   p_translate_frame,
   p_rotate_frame,
   p_mirror_u,
   p_mirror_v,
   p_wrap_u,
   p_wrap_v,
   p_stagger,
   p_repeat,
   p_offset,
   p_rotate,
   p_filename,
   p_color_gain,
   p_color_offset,
   p_default_color,
   p_alpha_gain,
   p_alpha_offset,
   p_alpha_is_luminance,
   p_invert,
   p_noise
};

inline float mod(float n, float d)
{
   return (n - (floor(n / d) * d));
}

};

node_parameters
{
   AiParameterPNT2("coverage", 1.0f, 1.0f);
   AiParameterPNT2("translateFrame", 0.0f, 0.0f);
   AiParameterFLT("rotateFrame", 0.0f);
   AiParameterBOOL("mirrorU", false);
   AiParameterBOOL("mirrorV", false);
   AiParameterBOOL("wrapU", true);
   AiParameterBOOL("wrapV", true);
   AiParameterBOOL("stagger", false);
   AiParameterPNT2("repeatUV", 1.0f, 1.0f);
   AiParameterPNT2("offsetUV", 0.0f, 0.0f);
   AiParameterFLT("rotateUV", 0.0f);
   AiParameterSTR("filename", "");
   AiParameterRGB("colorGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("colorOffset", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("defaultColor", 0.5f, 0.5f, 0.5f);
   AiParameterFLT("alphaGain", 1.0f);
   AiParameterFLT("alphaOffset", 0.0f);
   AiParameterBOOL("alphaIsLuminance", false);
   AiParameterBOOL("invert", false);
   AiParameterPNT2("noiseUV", 0.0f, 0.0f);

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
   const char *filename = AiShaderEvalParamStr(p_filename);
   AtPoint2 coverage = AiShaderEvalParamPnt2(p_coverage);
   AtPoint2 translate = AiShaderEvalParamPnt2(p_translate_frame);
   float frotate = AiShaderEvalParamFlt(p_rotate_frame);
   bool mirrorU = (AiShaderEvalParamBool(p_mirror_u) == TRUE);
   bool mirrorV = (AiShaderEvalParamBool(p_mirror_v) == TRUE);
   bool wrapU = (AiShaderEvalParamBool(p_wrap_u) == TRUE);
   bool wrapV = (AiShaderEvalParamBool(p_wrap_v) == TRUE);
   bool stagger = (AiShaderEvalParamBool(p_stagger) == TRUE);
   AtPoint2 repeat = AiShaderEvalParamPnt2(p_repeat);
   AtPoint2 offset = AiShaderEvalParamPnt2(p_offset);
   float rotate = AiShaderEvalParamFlt(p_rotate);
   AtRGB colorGain = AiShaderEvalParamRGB(p_color_gain);
   AtRGB colorOffset = AiShaderEvalParamRGB(p_color_offset);
   AtRGB defaultColor = AiShaderEvalParamRGB(p_default_color);
   float alphaGain = AiShaderEvalParamFlt(p_alpha_gain);
   float alphaOffset = AiShaderEvalParamFlt(p_alpha_offset);
   bool alphaIsLuminance = (AiShaderEvalParamBool(p_alpha_is_luminance) == TRUE);
   bool invert = (AiShaderEvalParamBool(p_invert) == TRUE);
   AtPoint2 noise = AiShaderEvalParamPnt2(p_noise);

   float inU = sg->u;
   float inV = sg->v;
   float inDuDx = sg->dudx;
   float inDuDy = sg->dudy;
   float inDvDx = sg->dvdx;
   float inDvDy = sg->dvdy;

   float outU = inU;
   float outV = inV;
   float outDuDx = inDuDx;
   float outDuDy = inDuDy;
   float outDvDx = inDvDx;
   float outDvDy = inDvDy;

   if (noise.x > 0.0f)
   {
      AtPoint uv;
      uv.x = inU * 16;
      uv.y = inV * 16;
      uv.z = 0.0f;
      outU += noise.x * AiPerlin3(uv);
   }

   if (noise.y > 0.0f)
   {
      AtPoint uv;
      uv.x = (1 - inU) * 16;
      uv.y = (1 - inV) * 16;
      uv.z = 0.0f;
      outV += noise.y * AiPerlin3(uv);
   }

   // for frame, rotate first then translate
   if (frotate <= -AI_EPSILON || frotate >= AI_EPSILON)
   {
      float x, y;
      float ca = cos(frotate);
      float sa = sin(frotate);

      x = outU - 0.5f;
      y = outV - 0.5f;
      outU = 0.5f + ca * x - sa * y;
      outV = 0.5f + ca * y + sa * x;

      x = outDuDx;
      y = outDuDy;
      outDuDx = ca * x - sa * y;
      outDuDy = ca * y + sa * x;

      x = outDvDx;
      y = outDvDy;
      outDvDx = ca * x - sa * y;
      outDvDy = ca * y + sa * x;
   }

   outU -= translate.x;
   outV -= translate.y;

   // If coverage.x or coverage.y are <= 1.0f
   //   check of the wrapped u or v coordinades respectively wraps in a valid range
   // If wrap is off, check incoming coordinate is in the range [0, 1]
   if (mod(outU, 1.0f) > coverage.x ||
       mod(outV, 1.0f) > coverage.y ||
       (!wrapU && (outU < 0 || outU > 1)) ||
       (!wrapV && (outV < 0 || outV > 1)))
   {
      sg->out.RGBA.r = defaultColor.r;
      sg->out.RGBA.g = defaultColor.g;
      sg->out.RGBA.b = defaultColor.b;
      sg->out.RGBA.a = (alphaIsLuminance ? Luminance(defaultColor) : 1.0f);
   }
   else
   {
      float icx = 1.0f / coverage.x;
      float icy = 1.0f / coverage.y;

      if (coverage.x < 1.0f)
      {
         outU = mod(outU, 1.0f);
      }

      if (coverage.y < 1.0f)
      {
         outV = mod(outV, 1.0f);
      }

      outU *= icx;
      outV *= icy;

      // apply repetition factor
      outU *= repeat.x;
      outV *= repeat.y;

      // for UVs, translate first, then rotate
      outU += offset.x;
      outV += offset.y;

      outDuDx *= icx * repeat.x;
      outDuDy *= icy * repeat.y;
      outDvDx *= icx * repeat.x;
      outDvDy *= icy * repeat.y;

      // do mirror, stagger before rotation
      if (mod(outV, 2.0f) >= 1.0f)
      {
         if (stagger)
         {
            outU += 0.5f;
         }

         if (mirrorV)
         {
            float center = floor(outV) + 0.5f;
            outV = center - (outV - center);

            outDuDy = -outDuDy;
            outDvDy = -outDvDy;
         }
      }
      
      if (mirrorU && mod(outU, 2.0f) >= 1.0f)
      {
         float center = floor(outU) + 0.5f;
         outU = center - (outU - center);

         outDuDx = -outDuDx;
         outDvDx = -outDvDx;
      }

      // finally rotate UV
      if (rotate <= -AI_EPSILON || rotate >= AI_EPSILON)
      {
         float x, y;
         float ca = cos(rotate);
         float sa = sin(rotate);

         x = outU - 0.5f;
         y = outV - 0.5f;
         outU = 0.5f + ca * x - sa * y;
         outV = 0.5f + ca * y + sa * x;

         x = outDuDx;
         y = outDuDy;
         outDuDx = ca * x - sa * y;
         outDuDy = ca * y + sa * x;

         x = outDvDx;
         y = outDvDy;
         outDvDx = ca * x - sa * y;
         outDvDy = ca * y + sa * x;
      }
      
      // replace shader globals
      sg->u = outU;
      sg->v = outV;
      sg->dudx = outDuDx;
      sg->dudy = outDuDy;
      sg->dvdx = outDvDx;
      sg->dvdy = outDvDy;

      // do texture lookup
      AtTextureParams texparams;
      AiTextureParamsSetDefaults(&texparams);
      // setup filter?

      AtRGBA result = AiTextureAccess(sg, filename, &texparams);

      // restore shader globals
      sg->u = inU;
      sg->v = inV;
      sg->dudx = inDuDx;
      sg->dudy = inDuDy;
      sg->dvdx = inDvDx;
      sg->dvdy = inDvDy;

      // Do color balance
      if (invert)
      {
         result.r = 1.0f - result.r;
         result.g = 1.0f - result.g;
         result.b = 1.0f - result.b;
         result.a = 1.0f - result.a;
      }
      if (alphaIsLuminance)
      {
         result.a = Luminance(result);
      }
      result.r = (result.r * colorGain.r) + colorOffset.r;
      result.g = (result.g * colorGain.g) + colorOffset.g;
      result.b = (result.b * colorGain.b) + colorOffset.b;
      result.a = (result.a * alphaGain) + alphaOffset;
      sg->out.RGBA = result;
   }
}
