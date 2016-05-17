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
      p_color8,
      p_color9,
      p_color10,
      p_color11,
      p_color12,
      p_color13,
      p_color14,
      p_color15,
      p_transparency0,
      p_transparency1,
      p_transparency2,
      p_transparency3,
      p_transparency4,
      p_transparency5,
      p_transparency6,
      p_transparency7,
      p_transparency8,
      p_transparency9,
      p_transparency10,
      p_transparency11,
      p_transparency12,
      p_transparency13,
      p_transparency14,
      p_transparency15,
      p_useTransparency0,
      p_useTransparency1,
      p_useTransparency2,
      p_useTransparency3,
      p_useTransparency4,
      p_useTransparency5,
      p_useTransparency6,
      p_useTransparency7,
      p_useTransparency8,
      p_useTransparency9,
      p_useTransparency10,
      p_useTransparency11,
      p_useTransparency12,
      p_useTransparency13,
      p_useTransparency14,
      p_useTransparency15,
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
   AtString name;
};

struct ShaderData
{
   AtArray *aovs;
   unsigned int naovs;
};

void layer_op(AtShaderGlobals *sg, int flag, AtRGB color, const AtRGB& transparency, bool useTransparency,
              AtRGB &curColor, AtRGB &curOpacity)
{
   AtRGB opacity = AI_RGB_BLACK;
   if (useTransparency)
   {
      opacity = 1.0f - transparency;
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
   AiParameterEnum("compositingFlag", 0, gs_CompositingFlagNames);
   AiParameterUInt("numInputs", 0);
   AiParameterRGB("color0", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color1", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color2", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color3", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color4", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color5", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color6", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color7", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color8", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color9", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color10", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color11", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color12", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color13", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color14", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color15", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("transparency0", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency1", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency2", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency3", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency4", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency5", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency6", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency7", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency8", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency9", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency10", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency11", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency12", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency13", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency14", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency15", 1.0f, 1.0f, 1.0f);
   AiParameterBool("useTransparency0", false);
   AiParameterBool("useTransparency1", false);
   AiParameterBool("useTransparency2", false);
   AiParameterBool("useTransparency3", false);
   AiParameterBool("useTransparency4", false);
   AiParameterBool("useTransparency5", false);
   AiParameterBool("useTransparency6", false);
   AiParameterBool("useTransparency7", false);
   AiParameterBool("useTransparency8", false);
   AiParameterBool("useTransparency9", false);
   AiParameterBool("useTransparency10", false);
   AiParameterBool("useTransparency11", false);
   AiParameterBool("useTransparency12", false);
   AiParameterBool("useTransparency13", false);
   AiParameterBool("useTransparency14", false);
   AiParameterBool("useTransparency15", false);

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

node_initialize
{
}

node_update
{
   ShaderData* localData = (ShaderData*) AiMalloc(sizeof(ShaderData));
   localData->aovs = NULL;
   localData->naovs = 0;

   if (AiNodeLookUpUserParameter(node, "mtoa_aovs"))
   {
      localData->aovs = AiNodeGetArray(node, "mtoa_aovs");
      if (localData->aovs)
         localData->naovs = AiArrayGetNumElements(localData->aovs);
   }
   AiNodeSetLocalData(node, localData);
}

node_finish
{
   ShaderData* localData = (ShaderData*) AiNodeGetLocalData(node);
   AiFree(localData);
}

shader_evaluate
{
   unsigned int numInputs = AiShaderEvalParamUInt(p_numInputs);
   int flag = AiShaderEvalParamInt(p_compositingFlag);

   AtRGBA result = AI_RGBA_ZERO;
   AtRGB outOpacity = AI_RGB_WHITE;

   if (numInputs > 0)
   {
      ShaderData* localData = (ShaderData*) AiNodeGetLocalData(node);
      AOVLayer* AOVValues = 0;
      if (localData->naovs > 0)
         AOVValues = (AOVLayer*)AiShaderGlobalsQuickAlloc(sg, sizeof(AOVLayer) * localData->naovs);
      for (unsigned int ii = 0; ii < localData->naovs; ++ii)
      {
         AOVLayer& it = AOVValues[ii];
         it.color = AI_RGB_BLACK;
         it.opacity = AI_RGB_BLACK;
         it.name = AiArrayGetStr(localData->aovs, ii);
      }
      
      AtRGB curColor = AI_RGB_BLACK;
      AtRGB curOpacity = AI_RGB_BLACK;

      AtColor orgOpacity = sg->out_opacity;
      
      for (unsigned int i = 0; i < numInputs; ++i)
      {
         AtRGB color = AI_RGB_BLACK;
         AtRGB transparency = AI_RGB_BLACK;
         bool useTransparency = AiShaderEvalParamBool(p_useTransparency0 + i);
         
         if (useTransparency && (curOpacity.r < 1.f || curOpacity.g < 1.f || curOpacity.b < 1.f))
            transparency = AiShaderEvalParamRGB(p_transparency0 + i);
         if ((curOpacity.r < 1.f || curOpacity.g < 1.f || curOpacity.b < 1.f ) &&
            ((flag != CF_TEXTURE)  || (transparency.r < 1.f || transparency.g < 1.f || transparency.b < 1.f)))
         {
            for (unsigned int ii = 0; ii < localData->naovs; ++ii)
            {
               AOVLayer& it = AOVValues[ii];
               // setting a black value before evaluating the layer
               AiAOVSetRGB(sg, it.name, AI_RGB_BLACK);
            }

            color = AiShaderEvalParamRGB(p_color0 + i);
            layer_op(sg, flag,
                     color,
                     transparency,
                     useTransparency,
                     curColor, curOpacity);

            sg->out_opacity = orgOpacity;
            
            for (unsigned int ii = 0; ii < localData->naovs; ++ii)
            {
               AOVLayer& it = AOVValues[ii];
               AtRGB AOVColor = AI_RGB_BLACK;
               // save the current value
               AiAOVGetRGB(sg, it.name, AOVColor);
               // layer it into the accumulated results for this AOV
               // TODO: look into getting a pointer from AiAOVGetRGB
               layer_op(sg, flag,
                        AOVColor,
                        transparency,
                        useTransparency,
                        it.color, it.opacity);
            }
         }
      }
      result = post_process(curColor, curOpacity);
      outOpacity = curOpacity;
      for (unsigned int ii = 0; ii < localData->naovs; ++ii)
      {
         AOVLayer& it = AOVValues[ii];
         AtRGBA aovResult = post_process(it.color, it.opacity);
         AiAOVSetRGBA(sg, it.name, aovResult);
      }
   }

   sg->out.RGBA() = result;
   sg->out_opacity = outOpacity;
}
