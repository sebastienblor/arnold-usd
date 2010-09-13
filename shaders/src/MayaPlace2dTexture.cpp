
#include <ai.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

AI_SHADER_NODE_EXPORT_METHODS(MayaPlace2dTextureMtd);

namespace
{

enum Place2dTextureParams
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
   p_noise
};

inline float mod(float n, float d)
{
   return (n - (floor(n / d) * d));
}

inline void rotate2d(float a, float &u, float &v)
{
   if (a <= -AI_EPSILON || a >= AI_EPSILON)
   {
      float x = u - 0.5f;
      float y = v - 0.5f;
      float ca = cos(a);
      float sa = sin(a);
      u = 0.5f + ca * x - sa * y;
      v = 0.5f + ca * y + sa * x;
   }
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
   AiParameterPNT2("noiseUV", 0.0f, 0.0f);
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
   AtPoint2 noise = AiShaderEvalParamPnt2(p_noise);

   float inU = sg->u;
   float inV = sg->v;

   float outU = inU;
   float outV = inV;

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
   rotate2d(frotate, outU, outV);

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
      // we are out of the texture frame, return invalid u,v
      outU = -1000000.0f;
      outV = -1000000.0f;
   }
   else
   {
      if (coverage.x < 1.0f)
      {
         outU = mod(outU, 1.0f);
      }

      if (coverage.y < 1.0f)
      {
         outV = mod(outV, 1.0f);
      }

      outU /= coverage.x;
      outV /= coverage.y;

      // apply repetition factor
      outU *= repeat.x;
      outV *= repeat.y;

      // for UVs, translate first, then rotate
      outU += offset.x;
      outV += offset.y;

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
         }
      }

      if (mirrorU && mod(outU, 2.0f) >= 1.0f)
      {
         float center = floor(outU) + 0.5f;
         outU = center - (outU - center);
      }

      // finally rotate UV
      rotate2d(rotate, outU, outV);
   }

   sg->out.PNT2.x = outU;
   sg->out.PNT2.y = outV;
}
