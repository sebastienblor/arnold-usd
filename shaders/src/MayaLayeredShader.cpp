#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaLayeredShaderMtd);

namespace
{
   enum MayaLayeredShaderParams
   {
      p_compositingFlag = 0,
      p_numInputs,
      p_color0,
      p_color1,
      p_color2,
      p_color3,
      p_color4,
      p_color5,
      p_color6,
      p_color7,
      p_transparency0,
      p_transparency1,
      p_transparency2,
      p_transparency3,
      p_transparency4,
      p_transparency5,
      p_transparency6,
      p_transparency7,
      p_useTransparency0,
      p_useTransparency1,
      p_useTransparency2,
      p_useTransparency3,
      p_useTransparency4,
      p_useTransparency5,
      p_useTransparency6,
      p_useTransparency7,
   };

   enum CompositingFlag
   {
      CF_SHADER = 0,
      CF_TEXTURE
   };

   static const char* gs_CompositingFlagNames[] =
   {
      "shader",
      "texture",
      0,
   };
}

node_parameters
{
   AiParameterENUM("compositingFlag", 0, gs_CompositingFlagNames);
   AiParameterUINT("numInputs", 0);
   AiParameterRGB("color0", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color1", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color2", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color3", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color4", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color5", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color6", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color7", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("transparency0", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency1", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency2", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency3", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency4", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency5", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency6", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency7", 1.0f, 1.0f, 1.0f);
   AiParameterBOOL("useTransparency0", FALSE);
   AiParameterBOOL("useTransparency1", FALSE);
   AiParameterBOOL("useTransparency2", FALSE);
   AiParameterBOOL("useTransparency3", FALSE);
   AiParameterBOOL("useTransparency4", FALSE);
   AiParameterBOOL("useTransparency5", FALSE);
   AiParameterBOOL("useTransparency6", FALSE);
   AiParameterBOOL("useTransparency7", FALSE);

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
   AtInt flag = AiShaderEvalParamInt(p_compositingFlag);

   AtRGBA result = AI_RGBA_BLACK;
   AtRGB outOpacity = AI_RGB_WHITE;

   if (numInputs > 0)
   {
      AtColor orgOpacity = sg->out_opacity;

      AtRGB curColor = AI_RGB_BLACK;
      AtRGB curOpacity = AI_RGB_BLACK;

      for (AtUInt i = 0; i < numInputs; ++i)
      {
         AtRGB opacity;
         AtRGB color = AiShaderEvalParamRGB(p_color0+i);

         AtBoolean useTransparency = AiShaderEvalParamBool(p_useTransparency0+i);

         if (useTransparency)
         {
            opacity = 1.0f - AiShaderEvalParamRGB(p_transparency0+i);
            // Consider color pre-multiplied already
         }
         else
         {
            opacity = sg->out_opacity;
            // Pre-multiply color
            color.r *= opacity.r;
            color.g *= opacity.g;
            color.b *= opacity.b;
         }

         if (flag == CF_TEXTURE)
         {
            color *= opacity;
         }

         curColor += (1.0f - curOpacity) * color;
         curOpacity += (1.0f - curOpacity) * opacity;

         sg->out_opacity = orgOpacity;
      }

      // Un-premultiply color
      if (curOpacity.r > AI_EPSILON)
      {
         curColor.r /= curOpacity.r;
      }
      if (curOpacity.g > AI_EPSILON)
      {
         curColor.g /= curOpacity.g;
      }
      if (curOpacity.b > AI_EPSILON)
      {
         curColor.b /= curOpacity.b;
      }

      result.r = curColor.r;
      result.g = curColor.g;
      result.b = curColor.b;
      result.a = CLAMP(Luminance(curOpacity), 0.0f, 1.0f);

      outOpacity = curOpacity;
   }

   sg->out.RGBA = result;
   sg->out_opacity = outOpacity;
}
