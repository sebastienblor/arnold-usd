#include <ai.h>

#include "MayaUtils.h"

#include <vector>

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

struct AOVLayer
{
   AtRGB color;
   AtRGB opacity;
   const char* name;
};

void layer_op(AtShaderGlobals *sg, AtInt flag, AtRGB color, AtRGB opacity, AtBoolean useTransparency,
              AtRGB &curColor, AtRGB &curOpacity)
{
   AtColor orgOpacity = sg->out_opacity;
   if (useTransparency)
   {
      opacity = 1.0f - opacity;
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

AtRGBA post_process(const AtRGB &curColor, const AtRGB &curOpacity)
{
   AtRGBA result;
   result.r = curColor.r;
   result.g = curColor.g;
   result.b = curColor.b;

   // Un-premultiply color
   if (curOpacity.r > AI_EPSILON)
   {
      result.r /= curOpacity.r;
   }
   if (curOpacity.g > AI_EPSILON)
   {
      result.g /= curOpacity.g;
   }
   if (curOpacity.b > AI_EPSILON)
   {
      result.b /= curOpacity.b;
   }

   result.a = CLAMP(Luminance(curOpacity), 0.0f, 1.0f);
   return result;
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
   unsigned int numInputs = AiShaderEvalParamUInt(p_numInputs);
   int flag = AiShaderEvalParamInt(p_compositingFlag);

   AtRGBA result = AI_RGBA_BLACK;
   AtRGB outOpacity = AI_RGB_WHITE;

   if (numInputs > 0)
   {
      AtArray *aovs = NULL;
      AtUInt naovs = 0;
      if (AiUDataGetArray("mtoa_aovs", &aovs))
        naovs = aovs->nelements;
      // array for the accumulated results, will be empty if there are no aovs
      std::vector<AOVLayer> AOVValues(naovs);
      AtUInt i=0;
      for (std::vector<AOVLayer>::iterator it = AOVValues.begin(); it!=AOVValues.end(); ++it)
      {
         it->color = AI_RGB_BLACK;
         it->opacity = AI_RGB_BLACK;
         it->name = AiArrayGetStr(aovs, i);
         ++i;
      }

      AtRGB curColor = AI_RGB_BLACK;
      AtRGB curOpacity = AI_RGB_BLACK;

      for (unsigned int i = 0; i < numInputs; ++i)
      {
         AtRGB transparency = AiShaderEvalParamRGB(p_transparency0+i);
         AtBoolean useTransparency = AiShaderEvalParamBool(p_useTransparency0+i);
         layer_op(sg, flag,
                  AiShaderEvalParamRGB(p_color0+i),
                  transparency,
                  useTransparency,
                  curColor, curOpacity);

         if (naovs)
         {
            for (std::vector<AOVLayer>::iterator it = AOVValues.begin(); it!=AOVValues.end(); ++it)
            {
               AtRGB color = AI_RGB_BLACK;
               // save the current value
               AiAOVGetRGB(sg, it->name, color);
               // layer it into the accumulated results for this AOV
               // TODO: look into getting a pointer from AiAOVGetRGB
               layer_op(sg, flag,
                        color,
                        transparency,
                        useTransparency,
                        it->color, it->opacity);
            }
         }
      }
      result = post_process(curColor, curOpacity);
      outOpacity = curOpacity;

      for (std::vector<AOVLayer>::iterator it = AOVValues.begin(); it!=AOVValues.end(); ++it)
      {
         AtRGBA aovResult = post_process(it->color, it->opacity);
         AiAOVSetRGBA(sg, it->name, aovResult);
      }
   }

   sg->out.RGBA = result;
   sg->out_opacity = outOpacity;
}
