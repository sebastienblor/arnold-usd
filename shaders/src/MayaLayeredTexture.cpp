#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaLayeredTextureMtd);

namespace
{

   enum MayaLayeredTextureParams
   {
      p_alphaIsLuminance,
      p_color,
      p_alpha,
      p_blendMode,
      p_visible
   };

   enum BlendMode
   {
      BM_NONE = 0,
      BM_OVER,
      BM_IN,
      BM_OUT,
      BM_ADD,
      BM_SUBTRACT,
      BM_MULTIPLY,
      BM_DIFFERENCE,
      BM_LIGHTEN,
      BM_DARKEN,
      BM_SATURATE,
      BM_DESATURATE,
      BM_ILLUMINATE
   };


};

node_parameters
{
   AiParameterBOOL("alphaIsLuminance", false);
   AiParameterARRAY("color", AiArray(0, 0, AI_TYPE_RGB));
   AiParameterARRAY("alpha", AiArray(0, 0, AI_TYPE_FLOAT));
   AiParameterARRAY("blendMode", AiArray(0, 0, AI_TYPE_INT));
   AiParameterARRAY("visible", AiArray(0, false, AI_TYPE_BOOLEAN));

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
   AtArray* colors = AiShaderEvalParamArray(p_color);
   AtArray* alphas = AiShaderEvalParamArray(p_alpha);
   AtArray* modes = AiShaderEvalParamArray(p_blendMode);
   AtArray* visibles = AiShaderEvalParamArray(p_visible);
   unsigned int numInputs = colors->nelements;

   AtRGBA result = AI_RGBA_BLACK;

   if (numInputs > 0)
   {
      for (int i = numInputs-1; i >= 0; --i)
      {
         bool visible = AiArrayGetBool(visibles, i);
         if (visible == false) continue;

         AtRGB color = AiArrayGetRGB(colors, i);
         float alpha = AiArrayGetFlt(alphas, i);
         BlendMode mode = (BlendMode) AiArrayGetInt(modes, i);

         switch (mode)
         {
            case BM_NONE:
            {
               result.r = color.r;
               result.g = color.g;
               result.b = color.b;
               result.a = alpha;
            }
            break;

            case BM_OVER:
            {
               result.r = color.r * alpha + (result.r * (1.0f - alpha));
               result.g = color.g * alpha + (result.g * (1.0f - alpha));
               result.b = color.b * alpha + (result.b * (1.0f - alpha));
               result.a = 1.0f - ((1.0f - result.a) * (1.0f - alpha));
            }
            break;

            case BM_IN:
            {
               result.r *= alpha;
               result.g *= alpha;
               result.b *= alpha;
               result.a *= alpha;
            }
            break;

            case BM_OUT:
            {
               result.r *= (1.0f - alpha);
               result.g *= (1.0f - alpha);
               result.b *= (1.0f - alpha);
               result.a *= (1.0f - alpha);
            }
            break;

            case BM_ADD:
            {
               result.r += color.r * alpha;
               result.g += color.g * alpha;
               result.b += color.b * alpha;
            }
            break;

            case BM_SUBTRACT:
            {
               result.r -= color.r * alpha;
               result.g -= color.g * alpha;
               result.b -= color.b * alpha;
            }
            break;

            case BM_MULTIPLY:
            {
               result.r *= (color.r * alpha + 1.0f - alpha);
               result.g *= (color.g * alpha + 1.0f - alpha);
               result.b *= (color.b * alpha + 1.0f - alpha);
            }
            break;

            case BM_DIFFERENCE:
            {
               result.r = (fabs((color.r * alpha) - result.r)) * alpha + result.r * (1.0f - alpha);
               result.g = (fabs((color.g * alpha) - result.g)) * alpha + result.g * (1.0f - alpha);
               result.b = (fabs((color.b * alpha) - result.b)) * alpha + result.b * (1.0f - alpha);
            }
            break;

            case BM_LIGHTEN:
            {
               result.r = (MAX((color.r * alpha), result.r)) * alpha + result.r * (1.0f - alpha);
               result.g = (MAX((color.g * alpha), result.g)) * alpha + result.g * (1.0f - alpha);
               result.b = (MAX((color.b * alpha), result.b)) * alpha + result.b * (1.0f - alpha);
            }
            break;

            case BM_DARKEN:
            {
               result.r = (MIN((color.r * alpha), result.r)) * alpha + result.r * (1.0f - alpha);
               result.g = (MIN((color.g * alpha), result.g)) * alpha + result.g * (1.0f - alpha);
               result.b = (MIN((color.b * alpha), result.b)) * alpha + result.b * (1.0f - alpha);
            }
            break;

            case BM_SATURATE:
            {
               result.r *= (1.0f + (color.r * alpha));
               result.g *= (1.0f + (color.g * alpha));
               result.b *= (1.0f + (color.b * alpha));
            }
            break;

            case BM_DESATURATE:
            {
               result.r *= (1.0f - (color.r * alpha));
               result.g *= (1.0f - (color.g * alpha));
               result.b *= (1.0f - (color.b * alpha));
            }
            break;

            case BM_ILLUMINATE:
            {
               result.r *= (2.0f * color.r * alpha + 1.0f - alpha);
               result.g *= (2.0f * color.g * alpha + 1.0f - alpha);
               result.b *= (2.0f * color.b * alpha + 1.0f - alpha);
            }
            break;
         }
      }
   }

   // AiArrayDestroy(colors);
   // AiArrayDestroy(alphas);
   // AiArrayDestroy(modes);
   // AiArrayDestroy(visibles);

   if (AiShaderEvalParamBool(p_alphaIsLuminance) == true)
   {
      result.a =Luminance(result); // NTSC luminance
   }
   sg->out.RGBA = result;
}
