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


};


AtRGBA process_layers(AtShaderGlobals *sg, AtNode* node, int start, int end)
{
   AtRGBA result = AI_RGBA_BLACK;
   bool finished = false;

   for (int i = start; i < end && !finished; ++i)
   {
      if (AiShaderEvalParamBool(p_visible0+i) == FALSE)   // Disabled, skip
         continue;

      AtRGBA color = AI_RGBA_BLACK;
      float alpha = 0.0f;
      int blendMode = AiShaderEvalParamInt(p_blendMode0+i);

      AtBoolean colorConnectedToAlpha = AiShaderEvalParamBool(p_colorConnectedToAlpha0+i);

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
            alpha = AiShaderEvalParamFlt(p_alpha0+i);
            result.r += color.r * (1.0f - result.a);
            result.g += color.g * (1.0f - result.a);
            result.b += color.b * (1.0f - result.a);
            result.a += alpha * (1.0f - result.a);
            finished = true;
         }
         break;

         case BM_OVER:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r += color.r * alpha * (1.0f - result.a);
                  result.g += color.g * alpha * (1.0f - result.a);
                  result.b += color.b * alpha * (1.0f - result.a);
                  result.a += alpha * (1.0f - result.a);
               }
            }
         }
         break;

         case BM_IN:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               result.a += (1.0f - alpha) * (1.0f - result.a);
            }
         }
         break;

         case BM_OUT:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               result.a += (alpha) * (1.0f - result.a);
            }
         }
         break;

         case BM_ADD:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r += color.r * alpha * (1.0f - result.a);
                  result.g += color.g * alpha * (1.0f - result.a);
                  result.b += color.b * alpha * (1.0f - result.a);
               }
            }
         }
         break;

         case BM_SUBTRACT:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r -= color.r * alpha * (1.0f - result.a);
                  result.g -= color.g * alpha * (1.0f - result.a);
                  result.b -= color.b * alpha * (1.0f - result.a);
               }
            }
         }
         break;
         
         case BM_MULTIPLY:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               AtRGBA bottom = AI_RGBA_BLACK;
               bottom = process_layers(sg, node, i+1, end);
               float tmpa = result.a;
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r += (color.r * alpha * (1.0f - tmpa) * bottom.r);
                  result.g += (color.g * alpha * (1.0f - tmpa) * bottom.g);
                  result.b += (color.b * alpha * (1.0f - tmpa) * bottom.b);
                  result.a += alpha * (1.0f - tmpa) * bottom.a;
               }
               if (alpha < 1.0f)
               {
                  result.r += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.r);
                  result.g += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.g);
                  result.b += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.b);
                  result.a += (1.0f - alpha) * (1.0f - tmpa)  * bottom.a;
               }
            }
            finished = true;
         }
         break;
         
         case BM_DIFFERENCE:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               AtRGBA bottom = AI_RGBA_BLACK;
               bottom = process_layers(sg, node, i+1, end);
               float tmpa = result.a;
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r += (fabs(color.r - bottom.r)) * alpha * (1.0f - tmpa);
                  result.g += (fabs(color.g - bottom.g)) * alpha * (1.0f - tmpa);
                  result.b += (fabs(color.b - bottom.b)) * alpha * (1.0f - tmpa);
                  result.a += (fabs(alpha - bottom.a)) * alpha * (1.0f - tmpa);
               }
               if (alpha < 1.0f)
               {
                  result.r += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.r);
                  result.g += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.g);
                  result.b += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.b);
                  result.a += (1.0f - alpha) * (1.0f - tmpa)  * bottom.a;
               }
            }
            finished = true;
         }
         break;
         
         case BM_LIGHTEN:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               AtRGBA bottom = AI_RGBA_BLACK;
               bottom = process_layers(sg, node, i+1, end);
               float tmpa = result.a;
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r += (MAX(color.r, bottom.r)) * alpha * (1.0f - tmpa);
                  result.g += (MAX(color.g, bottom.g)) * alpha * (1.0f - tmpa);
                  result.b += (MAX(color.b, bottom.b)) * alpha * (1.0f - tmpa);
                  result.a += (MAX(alpha, bottom.a)) * alpha * (1.0f - tmpa);
               }
               if (alpha < 1.0f)
               {
                  result.r += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.r);
                  result.g += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.g);
                  result.b += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.b);
                  result.a += (1.0f - alpha) * (1.0f - tmpa)  * bottom.a;
               }
            }
            finished = true;
         }
         break;
         
         case BM_DARKEN:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               AtRGBA bottom = AI_RGBA_BLACK;
               bottom = process_layers(sg, node, i+1, end);
               float tmpa = result.a;
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r += (MIN(color.r, bottom.r)) * alpha * (1.0f - tmpa);
                  result.g += (MIN(color.g, bottom.g)) * alpha * (1.0f - tmpa);
                  result.b += (MIN(color.b, bottom.b)) * alpha * (1.0f - tmpa);
                  result.a += (MIN(alpha, bottom.a)) * alpha * (1.0f - tmpa);
               }
               if (alpha < 1.0f)
               {
                  result.r += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.r);
                  result.g += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.g);
                  result.b += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.b);
                  result.a += (1.0f - alpha) * (1.0f - tmpa)  * bottom.a;
               }
            }
            finished = true;
         }
         break;
         
         case BM_SATURATE:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               AtRGBA bottom = AI_RGBA_BLACK;
               bottom = process_layers(sg, node, i+1, end);
               float tmpa = result.a;
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r += color.r * bottom.r * alpha * (1.0f - tmpa);
                  result.g += color.g * bottom.g * alpha * (1.0f - tmpa);
                  result.b += color.b * bottom.b * alpha * (1.0f - tmpa);
                  result.a += alpha * bottom.a * alpha * (1.0f - tmpa);
               }

               result.r += ((1.0f - tmpa)  * bottom.r);
               result.g += ((1.0f - tmpa)  * bottom.g);
               result.b += ((1.0f - tmpa)  * bottom.b);
               result.a += (1.0f - tmpa)  * bottom.a;
            }
            finished = true;
         }
         break;

         case BM_DESATURATE:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               AtRGBA bottom = AI_RGBA_BLACK;
               bottom = process_layers(sg, node, i+1, end);
               float tmpa = result.a;
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r -= color.r * bottom.r * alpha * (1.0f - tmpa);
                  result.g -= color.g * bottom.g * alpha * (1.0f - tmpa);
                  result.b -= color.b * bottom.b * alpha * (1.0f - tmpa);
                  result.a -= alpha * bottom.a * alpha * (1.0f - tmpa);
               }

               result.r += ((1.0f - tmpa)  * bottom.r);
               result.g += ((1.0f - tmpa)  * bottom.g);
               result.b += ((1.0f - tmpa)  * bottom.b);
               result.a += (1.0f - tmpa)  * bottom.a;
            }
            finished = true;
         }
         break;
         
         case BM_ILLUMINATE:
         {
            if (result.a < 1.0f)
            {
               alpha = AiShaderEvalParamFlt(p_alpha0+i);
               AtRGBA bottom = AI_RGBA_BLACK;
               bottom = process_layers(sg, node, i+1, end);
               float tmpa = result.a;
               if (alpha > 0.0f)
               {
                  color = AiShaderEvalParamRGBA(p_color0+i);
                  result.r += 2.0f * color.r * bottom.r * alpha * (1.0f - tmpa);
                  result.g += 2.0f * color.g * bottom.g * alpha * (1.0f - tmpa);
                  result.b += 2.0f * color.b * bottom.b * alpha * (1.0f - tmpa);
                  result.a += alpha * bottom.a * alpha * (1.0f - tmpa);
               }
               if (alpha < 1.0f)
               {
                  result.r += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.r);
                  result.g += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.g);
                  result.b += ((1.0f - alpha) * (1.0f - tmpa)  * bottom.b);
                  result.a += (1.0f - alpha) * (1.0f - tmpa)  * bottom.a;
               }
            }
            finished = true;
         }
         break;
      }
   }

   return result;
}

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
   bool finished = false;

   if (numInputs > 0)
   {
      result = process_layers(sg, node, 0, numInputs);
   }

   if (AiShaderEvalParamBool(p_alphaIsLuminance) == TRUE)
   {
      result.a = Luminance(result); // NTSC luminance
   }
   sg->out.RGBA = result;
}
