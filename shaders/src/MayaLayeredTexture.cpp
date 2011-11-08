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
      p_numInputs,
      p_alphaIsLuminance,
      p_color0,
      p_color1,
      p_color2,
      p_color3,
      p_color4,
      p_color5,
      p_color6,
      p_color7,
      p_alpha0,
      p_alpha1,
      p_alpha2,
      p_alpha3,
      p_alpha4,
      p_alpha5,
      p_alpha6,
      p_alpha7,
      p_colorConnectedToAlpha0,
      p_colorConnectedToAlpha1,
      p_colorConnectedToAlpha2,
      p_colorConnectedToAlpha3,
      p_colorConnectedToAlpha4,
      p_colorConnectedToAlpha5,
      p_colorConnectedToAlpha6,
      p_colorConnectedToAlpha7,
      p_blendMode0,
      p_blendMode1,
      p_blendMode2,
      p_blendMode3,
      p_blendMode4,
      p_blendMode5,
      p_blendMode6,
      p_blendMode7,
      p_visible0,
      p_visible1,
      p_visible2,
      p_visible3,
      p_visible4,
      p_visible5,
      p_visible6,
      p_visible7,
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

   static const char* gs_BlendModeNames[] =
   {
      "None",
      "Over",
      "In",
      "Out",
      "Add",
      "Subtract",
      "Multiply",
      "Difference",
      "Lighten",
      "Darken",
      "Saturate",
      "Desaturate",
      "Illuminate",
      0,
   };

};

node_parameters
{
   AiParameterUINT("numInputs", 0);
   AiParameterBOOL("alphaIsLuminance", FALSE);
   AiParameterRGBA("color0", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color1", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color2", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color3", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color4", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color5", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color6", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color7", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterFLT("alpha0", 1.0f);
   AiParameterFLT("alpha1", 1.0f);
   AiParameterFLT("alpha2", 1.0f);
   AiParameterFLT("alpha3", 1.0f); 
   AiParameterFLT("alpha4", 1.0f);
   AiParameterFLT("alpha5", 1.0f);
   AiParameterFLT("alpha6", 1.0f);
   AiParameterFLT("alpha7", 1.0f);
   AiParameterBOOL("colorConnectedToAlpha0", FALSE);
   AiParameterBOOL("colorConnectedToAlpha1", FALSE);
   AiParameterBOOL("colorConnectedToAlpha2", FALSE);
   AiParameterBOOL("colorConnectedToAlpha3", FALSE);
   AiParameterBOOL("colorConnectedToAlpha4", FALSE);
   AiParameterBOOL("colorConnectedToAlpha5", FALSE);
   AiParameterBOOL("colorConnectedToAlpha6", FALSE);
   AiParameterBOOL("colorConnectedToAlpha7", FALSE);
   AiParameterENUM("blendMode0", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode1", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode2", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode3", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode4", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode5", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode6", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode7", 0, gs_BlendModeNames);
   AiParameterBOOL("visible0", FALSE);
   AiParameterBOOL("visible1", FALSE);
   AiParameterBOOL("visible2", FALSE);
   AiParameterBOOL("visible3", FALSE);
   AiParameterBOOL("visible4", FALSE);
   AiParameterBOOL("visible5", FALSE);
   AiParameterBOOL("visible6", FALSE);
   AiParameterBOOL("visible7", FALSE);

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
   AtUInt numInputs = AiShaderEvalParamUInt(p_numInputs);

   AtRGBA result = AI_RGBA_BLACK;

   if (numInputs > 0)
   {
      for (int i = numInputs-1; i >= 0; --i)
      {
         if (AiShaderEvalParamBool(p_visible0+i) == FALSE)   // Disabled, skip
            continue;

         AtRGBA color = AiShaderEvalParamRGBA(p_color0+i);
         AtFloat alpha = AiShaderEvalParamFlt(p_alpha0+i);
         AtInt blendMode = AiShaderEvalParamInt(p_blendMode0+i);

         AtBoolean colorConnectedToAlpha = AiShaderEvalParamBool(p_colorConnectedToAlpha0+i);

         // Multiply input alpha with the color's alpha value to support alpha texturing
         // Until Arnold has a proper connectable array attribute

         if (colorConnectedToAlpha)
            alpha *= color.a;

         switch (blendMode)
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

   if (AiShaderEvalParamBool(p_alphaIsLuminance) == TRUE)
   {
      result.a = result.r*0.30f + result.g*0.59f + result.b*0.11f; // NTSC luminance
   }
   sg->out.RGBA = result;
}
