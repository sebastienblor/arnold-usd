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
      p_color8,
      p_color9,
      p_color10,
      p_color11,
      p_color12,
      p_color13,
      p_color14,
      p_color15,
      p_alpha0,
      p_alpha1,
      p_alpha2,
      p_alpha3,
      p_alpha4,
      p_alpha5,
      p_alpha6,
      p_alpha7,
      p_alpha8,
      p_alpha9,
      p_alpha10,
      p_alpha11,
      p_alpha12,
      p_alpha13,
      p_alpha14,
      p_alpha15,
      p_colorConnectedToAlpha0,
      p_colorConnectedToAlpha1,
      p_colorConnectedToAlpha2,
      p_colorConnectedToAlpha3,
      p_colorConnectedToAlpha4,
      p_colorConnectedToAlpha5,
      p_colorConnectedToAlpha6,
      p_colorConnectedToAlpha7,
      p_colorConnectedToAlpha8,
      p_colorConnectedToAlpha9,
      p_colorConnectedToAlpha10,
      p_colorConnectedToAlpha11,
      p_colorConnectedToAlpha12,
      p_colorConnectedToAlpha13,
      p_colorConnectedToAlpha14,
      p_colorConnectedToAlpha15,
      p_blendMode0,
      p_blendMode1,
      p_blendMode2,
      p_blendMode3,
      p_blendMode4,
      p_blendMode5,
      p_blendMode6,
      p_blendMode7,
      p_blendMode8,
      p_blendMode9,
      p_blendMode10,
      p_blendMode11,
      p_blendMode12,
      p_blendMode13,
      p_blendMode14,
      p_blendMode15,
      p_visible0,
      p_visible1,
      p_visible2,
      p_visible3,
      p_visible4,
      p_visible5,
      p_visible6,
      p_visible7,
      p_visible8,
      p_visible9,
      p_visible10,
      p_visible11,
      p_visible12,
      p_visible13,
      p_visible14,
      p_visible15,
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
   AiParameterRGBA("color8", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color9", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color10", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color11", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color12", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color13", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color14", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("color15", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterFLT("alpha0", 1.0f);
   AiParameterFLT("alpha1", 1.0f);
   AiParameterFLT("alpha2", 1.0f);
   AiParameterFLT("alpha3", 1.0f); 
   AiParameterFLT("alpha4", 1.0f);
   AiParameterFLT("alpha5", 1.0f);
   AiParameterFLT("alpha6", 1.0f);
   AiParameterFLT("alpha7", 1.0f);
   AiParameterFLT("alpha8", 1.0f);
   AiParameterFLT("alpha9", 1.0f);
   AiParameterFLT("alpha10", 1.0f);
   AiParameterFLT("alpha11", 1.0f); 
   AiParameterFLT("alpha12", 1.0f);
   AiParameterFLT("alpha13", 1.0f);
   AiParameterFLT("alpha14", 1.0f);
   AiParameterFLT("alpha15", 1.0f);
   AiParameterBOOL("colorConnectedToAlpha0", FALSE);
   AiParameterBOOL("colorConnectedToAlpha1", FALSE);
   AiParameterBOOL("colorConnectedToAlpha2", FALSE);
   AiParameterBOOL("colorConnectedToAlpha3", FALSE);
   AiParameterBOOL("colorConnectedToAlpha4", FALSE);
   AiParameterBOOL("colorConnectedToAlpha5", FALSE);
   AiParameterBOOL("colorConnectedToAlpha6", FALSE);
   AiParameterBOOL("colorConnectedToAlpha7", FALSE);
   AiParameterBOOL("colorConnectedToAlpha8", FALSE);
   AiParameterBOOL("colorConnectedToAlpha9", FALSE);
   AiParameterBOOL("colorConnectedToAlpha10", FALSE);
   AiParameterBOOL("colorConnectedToAlpha11", FALSE);
   AiParameterBOOL("colorConnectedToAlpha12", FALSE);
   AiParameterBOOL("colorConnectedToAlpha13", FALSE);
   AiParameterBOOL("colorConnectedToAlpha14", FALSE);
   AiParameterBOOL("colorConnectedToAlpha15", FALSE);
   AiParameterENUM("blendMode0", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode1", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode2", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode3", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode4", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode5", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode6", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode7", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode8", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode9", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode10", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode11", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode12", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode13", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode14", 0, gs_BlendModeNames);
   AiParameterENUM("blendMode15", 0, gs_BlendModeNames);
   AiParameterBOOL("visible0", FALSE);
   AiParameterBOOL("visible1", FALSE);
   AiParameterBOOL("visible2", FALSE);
   AiParameterBOOL("visible3", FALSE);
   AiParameterBOOL("visible4", FALSE);
   AiParameterBOOL("visible5", FALSE);
   AiParameterBOOL("visible6", FALSE);
   AiParameterBOOL("visible7", FALSE);
   AiParameterBOOL("visible8", FALSE);
   AiParameterBOOL("visible9", FALSE);
   AiParameterBOOL("visible10", FALSE);
   AiParameterBOOL("visible11", FALSE);
   AiParameterBOOL("visible12", FALSE);
   AiParameterBOOL("visible13", FALSE);
   AiParameterBOOL("visible14", FALSE);
   AiParameterBOOL("visible15", FALSE);

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
   unsigned int numInputs = AiShaderEvalParamUInt(p_numInputs);

   AtRGBA result = AI_RGBA_BLACK;

   if (numInputs > 0)
   {
      for (int i = numInputs-1; i >= 0; --i)
      {
         if (AiShaderEvalParamBool(p_visible0+i) == FALSE)   // Disabled, skip
            continue;

         AtRGBA color = AI_RGBA_BLACK;
         float alpha = AiShaderEvalParamFlt(p_alpha0+i);
         int blendMode = AiShaderEvalParamInt(p_blendMode0+i);

         AtBoolean colorConnectedToAlpha = AiShaderEvalParamBool(p_colorConnectedToAlpha0+i);

         // Multiply input alpha with the color's alpha value to support alpha texturing
         // Until Arnold has a proper connectable array attribute

         if (colorConnectedToAlpha)
         {
            ;//Only color is needed to be evaluated
            // Make a way to only evaluate color and use its alpha component as layer alpha to increase performance
         }

         switch (blendMode)
         {
            case BM_NONE:
            {
               color = AiShaderEvalParamRGBA(p_color0+i);
               result.r = color.r;
               result.g = color.g;
               result.b = color.b;
               result.a = alpha;
            }
            break;

            case BM_OVER:
            {
               if(alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r = color.r * alpha + (result.r * (1.0f - alpha));
                  result.g = color.g * alpha + (result.g * (1.0f - alpha));
                  result.b = color.b * alpha + (result.b * (1.0f - alpha));
                  result.a = 1.0f - ((1.0f - result.a) * (1.0f - alpha));
               }
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
               if(alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r += color.r * alpha;
                  result.g += color.g * alpha;
                  result.b += color.b * alpha;
               }
            }
            break;

            case BM_SUBTRACT:
            {
               if(alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r -= color.r * alpha;
                  result.g -= color.g * alpha;
                  result.b -= color.b * alpha;
               }
            }
            break;

            case BM_MULTIPLY:
            {
               if(alpha > 0.0f && (result.r > 0.0f || result.g > 0.0f || result.b > 0.0f))
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r *= (color.r * alpha + 1.0f - alpha);
                  result.g *= (color.g * alpha + 1.0f - alpha);
                  result.b *= (color.b * alpha + 1.0f - alpha);
               }
            }
            break;

            case BM_DIFFERENCE:
            {
               if(alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r = (fabs((color.r * alpha) - result.r)) * alpha + result.r * (1.0f - alpha);
                  result.g = (fabs((color.g * alpha) - result.g)) * alpha + result.g * (1.0f - alpha);
                  result.b = (fabs((color.b * alpha) - result.b)) * alpha + result.b * (1.0f - alpha);
               }
            }
            break;

            case BM_LIGHTEN:
            {
               if(alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r = (MAX((color.r * alpha), result.r)) * alpha + result.r * (1.0f - alpha);
                  result.g = (MAX((color.g * alpha), result.g)) * alpha + result.g * (1.0f - alpha);
                  result.b = (MAX((color.b * alpha), result.b)) * alpha + result.b * (1.0f - alpha);
               }
            }
            break;

            case BM_DARKEN:
            {
               if(alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r = (MIN((color.r * alpha), result.r)) * alpha + result.r * (1.0f - alpha);
                  result.g = (MIN((color.g * alpha), result.g)) * alpha + result.g * (1.0f - alpha);
                  result.b = (MIN((color.b * alpha), result.b)) * alpha + result.b * (1.0f - alpha);
               }
            }
            break;

            case BM_SATURATE:
            {
               if(alpha > 0.0f && (result.r > 0.0f || result.g > 0.0f || result.b > 0.0f))
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r *= (1.0f + (color.r * alpha));
                  result.g *= (1.0f + (color.g * alpha));
                  result.b *= (1.0f + (color.b * alpha));
               }
            }
            break;

            case BM_DESATURATE:
            {
               if(alpha > 0.0f && (result.r > 0.0f || result.g > 0.0f || result.b > 0.0f))
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r *= (1.0f - (color.r * alpha));
                  result.g *= (1.0f - (color.g * alpha));
                  result.b *= (1.0f - (color.b * alpha));
               }
            }
            break;

            case BM_ILLUMINATE:
            {
               if(alpha > 0.0f && (result.r > 0.0f || result.g > 0.0f || result.b > 0.0f))
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r *= (2.0f * color.r * alpha + 1.0f - alpha);
                  result.g *= (2.0f * color.g * alpha + 1.0f - alpha);
                  result.b *= (2.0f * color.b * alpha + 1.0f - alpha);
               }
            }
            break;
         }
      }
   }

   if (AiShaderEvalParamBool(p_alphaIsLuminance) == TRUE)
   {
      result.a = Luminance(result); // NTSC luminance
   }
   sg->out.RGBA = result;
}
