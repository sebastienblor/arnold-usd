#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaFileMtd);

namespace
{

enum MayaFileParams
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
   p_noise,
   MAYA_COLOR_BALANCE_ENUM
};

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
   AiParameterPNT2("noiseUV", 0.0f, 0.0f);
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
   AtPoint2 noise = AiShaderEvalParamPnt2(p_noise);
   EVAL_MAYA_COLOR_BALANCE_PARAMS

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
   if (Mod(outU, 1.0f) > coverage.x ||
       Mod(outV, 1.0f) > coverage.y ||
       (!wrapU && (outU < 0 || outU > 1)) ||
       (!wrapV && (outV < 0 || outV > 1)))
   {
      MAYA_DEFAULT_COLOR(sg->out.RGBA);
   }
   else
   {
      float icx = 1.0f / coverage.x;
      float icy = 1.0f / coverage.y;

      if (coverage.x < 1.0f)
      {
         outU = Mod(outU, 1.0f);
      }

      if (coverage.y < 1.0f)
      {
         outV = Mod(outV, 1.0f);
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
      if (Mod(outV, 2.0f) >= 1.0f)
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
      
      if (mirrorU && Mod(outU, 2.0f) >= 1.0f)
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

      sg->out.RGBA = AiTextureAccess(sg, filename, &texparams);
      MAYA_COLOR_BALANCE(sg->out.RGBA);

      // restore shader globals
      sg->u = inU;
      sg->v = inV;
      sg->dudx = inDuDx;
      sg->dudy = inDuDy;
      sg->dvdx = inDvDx;
      sg->dvdy = inDvDy;
   }
}
