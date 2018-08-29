#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include <string>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaPlace2DTextureMtd);

namespace
{

enum MayaPlace2DTextureParams
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
   p_noise,
   p_uvset_name
};

inline void rotate2d(float rot, float &u, float &v)
{
   if (rot <= -AI_EPSILON || rot >= AI_EPSILON)
   {
      float x = u - 0.5f;
      float y = v - 0.5f;
      float crot = cos(rot);
      float srot = sin(rot);
      u = 0.5f + crot * x - srot * y;
      v = 0.5f + crot * y + srot * x;
   }
}

};

struct MayaPlace2DTextureData{
   AtString uvSetName;
   bool useCustomUVSet;

   MayaPlace2DTextureData() : useCustomUVSet(false)
   {
   }   

   static void* operator new(size_t s)
   {
      return AiMalloc(s);
   }
   
   static void operator delete(void* p)
   {
      AiFree(p);
   }
};

node_parameters
{
   AiParameterVec2("coverage", 1.0f, 1.0f);
   AiParameterVec2("translateFrame", 0.0f, 0.0f);
   AiParameterFlt("rotateFrame", 0.0f);
   AiParameterBool("mirrorU", false);
   AiParameterBool("mirrorV", false);
   AiParameterBool("wrapU", true);
   AiParameterBool("wrapV", true);
   AiParameterBool("stagger", false);
   AiParameterVec2("repeatUV", 1.0f, 1.0f);
   AiParameterVec2("offsetUV", 0.0f, 0.0f);
   AiParameterFlt("rotateUV", 0.0f);
   AiParameterVec2("noiseUV", 0.0f, 0.0f);
   AiParameterStr("uvSetName", "");

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
}

node_initialize
{
   AiNodeSetLocalData(node, new MayaPlace2DTextureData());
}

node_update
{
   MayaPlace2DTextureData* data = (MayaPlace2DTextureData*)AiNodeGetLocalData(node);
   data->uvSetName = AiNodeGetStr(node, "uvSetName");
   data->useCustomUVSet = data->uvSetName.length() > 0;
}

node_finish
{
   delete (MayaPlace2DTextureData*)AiNodeGetLocalData(node);
}

shader_evaluate
{
   AtVector2 coverage = AiShaderEvalParamVec2(p_coverage);
   AtVector2 translate = AiShaderEvalParamVec2(p_translate_frame);
   float frotate = AiShaderEvalParamFlt(p_rotate_frame);
   bool mirrorU = (AiShaderEvalParamBool(p_mirror_u) == true);
   bool mirrorV = (AiShaderEvalParamBool(p_mirror_v) == true);
   bool wrapU = (AiShaderEvalParamBool(p_wrap_u) == true);
   bool wrapV = (AiShaderEvalParamBool(p_wrap_v) == true);
   bool stagger = (AiShaderEvalParamBool(p_stagger) == true);
   AtVector2 repeat = AiShaderEvalParamVec2(p_repeat);
   AtVector2 offset = AiShaderEvalParamVec2(p_offset);
   float rotate = AiShaderEvalParamFlt(p_rotate);
   AtVector2 noise = AiShaderEvalParamVec2(p_noise);

   MayaPlace2DTextureData* data = (MayaPlace2DTextureData*)AiNodeGetLocalData(node);
   float inU, inV;

   if (data->useCustomUVSet)
   {
      AtVector2 altuv;
      if (AiUDataGetVec2(data->uvSetName, altuv))
      {         
         inU = altuv.x;
         inV = altuv.y;
      }
      else
      {
         inU = sg->u;
         inV = sg->v;
      }
   }
   else
   {
      inU = sg->u;
      inV = sg->v;
   }  

   float outU = inU;
   float outV = inV;

   if (noise.x > 0.0f)
   {
      AtVector uv;
      uv.x = inU * 16;
      uv.y = inV * 16;
      uv.z = 0.0f;
      outU += noise.x * AiPerlin3(uv);
   }

   if (noise.y > 0.0f)
   {
      AtVector uv;
      uv.x = (1 - inU) * 16;
      uv.y = (1 - inV) * 16;
      uv.z = 0.0f;
      outV += noise.y * AiPerlin3(uv);
   }

   // For frame, rotate first then translate
   rotate2d(frotate, outU, outV);

   outU -= translate.x;
   outV -= translate.y;

   // If coverage.x or coverage.y are <= 1.0f
   // Check of the wrapped u or v coordinades respectively wraps in a valid range
   // If wrap is off, check incoming coordinate is in the range [0, 1]
   if (Mod(outU, 1.0f) > coverage.x ||
       Mod(outV, 1.0f) > coverage.y ||
       (!wrapU && (outU < 0 || outU > coverage.x)) ||
       (!wrapV && (outV < 0 || outV > coverage.y)))
   {
      // We are out of the texture frame, return invalid u,v
      outU = -1000000.0f;
      outV = -1000000.0f;
   }
   else
   {
      if (coverage.x < 1.0f)
      {
         outU = Mod(outU, 1.0f);
      }

      if (coverage.y < 1.0f)
      {
         outV = Mod(outV, 1.0f);
      }

      outU /= coverage.x;
      outV /= coverage.y;

      // Apply repetition factor
      outU *= repeat.x;
      outV *= repeat.y;

      // For UVs, translate first, then rotate
      outU += offset.x;
      outV += offset.y;

      // Do mirror, stagger before rotation
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
         }
      }

      if (mirrorU && Mod(outU, 2.0f) >= 1.0f)
      {
         float center = floor(outU) + 0.5f;
         outU = center - (outU - center);
      }

      // Finally rotate UV
      rotate2d(rotate, outU, outV);
   }

   sg->out.VEC2().x = outU;
   sg->out.VEC2().y = outV;
}
