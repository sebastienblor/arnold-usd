#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaLayeredShaderMtd);

namespace
{
   enum MayaLayeredShaderParams
   {
      p_compositingMode = 0,
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
      p_aov_emission,
      p_aov_direct_diffuse,
      p_aov_direct_specular,
      p_aov_indirect_diffuse,
      p_aov_indirect_specular,
      p_aov_reflection,
      p_aov_refraction,
      p_aov_sss
   };

   enum CompositingMode
   {
      CM_SHADER = 0,
      CM_TEXTURE
   };

   static const char* gs_CompositingModeNames[] =
   {
      "shader",
      "texture",
      0,
   };
}

node_parameters
{
   AiParameterENUM("compositingMode", 0, gs_CompositingModeNames);
   AiParameterINT("numInputs", 0);
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
   AiParameterBOOL("useTransparency0", false);
   AiParameterBOOL("useTransparency1", false);
   AiParameterBOOL("useTransparency2", false);
   AiParameterBOOL("useTransparency3", false);
   AiParameterBOOL("useTransparency4", false);
   AiParameterBOOL("useTransparency5", false);
   AiParameterBOOL("useTransparency6", false);
   AiParameterBOOL("useTransparency7", false);
   AiParameterBOOL("useTransparency8", false);
   AiParameterBOOL("useTransparency9", false);
   AiParameterBOOL("useTransparency10", false);
   AiParameterBOOL("useTransparency11", false);
   AiParameterBOOL("useTransparency12", false);
   AiParameterBOOL("useTransparency13", false);
   AiParameterBOOL("useTransparency14", false);
   AiParameterBOOL("useTransparency15", false);
   AiParameterSTR("aov_emission", "emission");
   AiParameterSTR("aov_direct_diffuse", "direct_diffuse");
   AiParameterSTR("aov_direct_specular", "direct_specular");
   AiParameterSTR("aov_indirect_diffuse", "indirect_diffuse");
   AiParameterSTR("aov_indirect_specular", "indirect_specular");
   AiParameterSTR("aov_reflection", "reflection");
   AiParameterSTR("aov_refraction", "refraction");
   AiParameterSTR("aov_sss", "sss");

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
   AtInt numInputs = AiShaderEvalParamInt(p_numInputs);
   AtInt mode = AiShaderEvalParamInt(p_compositingMode);

   AtRGBA result = AI_RGBA_BLACK;
   AtRGB outOpacity = AI_RGB_WHITE;

   AtRGB aovEmission = AI_RGB_BLACK;
   AtRGB aovDirectDiffuse = AI_RGB_BLACK;
   AtRGB aovIndirectDiffuse = AI_RGB_BLACK;
   AtRGB aovDirectSpecular = AI_RGB_BLACK;
   AtRGB aovIndirectSpecular = AI_RGB_BLACK;
   AtRGB aovReflection = AI_RGB_BLACK;
   AtRGB aovRefraction = AI_RGB_BLACK;
   AtRGB aovSSS = AI_RGB_BLACK;

   const char *aovEmissionName = AiShaderEvalParamStr(p_aov_emission);
   const char *aovDirectDiffuseName = AiShaderEvalParamStr(p_aov_direct_diffuse);
   const char *aovDirectSpecularName = AiShaderEvalParamStr(p_aov_direct_specular);
   const char *aovSSSName = AiShaderEvalParamStr(p_aov_sss);
   const char *aovReflectionName = AiShaderEvalParamStr(p_aov_reflection);
   const char *aovRefractionName = AiShaderEvalParamStr(p_aov_refraction);
   const char *aovIndirectDiffuseName = AiShaderEvalParamStr(p_aov_indirect_diffuse);
   const char *aovIndirectSpecularName = AiShaderEvalParamStr(p_aov_indirect_specular);

   bool doEmission = (AiAOVEnabled(aovEmissionName, AI_TYPE_RGB) == TRUE);
   bool doDirectDiffuse = (AiAOVEnabled(aovDirectDiffuseName, AI_TYPE_RGB) == TRUE);
   bool doDirectSpecular = (AiAOVEnabled(aovDirectSpecularName, AI_TYPE_RGB) == TRUE);
   bool doIndirectDiffuse = (AiAOVEnabled(aovIndirectDiffuseName, AI_TYPE_RGB) == TRUE);
   bool doIndirectSpecular = (AiAOVEnabled(aovIndirectSpecularName, AI_TYPE_RGB) == TRUE);
   bool doReflection = (AiAOVEnabled(aovReflectionName, AI_TYPE_RGB) == TRUE);
   bool doRefraction = (AiAOVEnabled(aovRefractionName, AI_TYPE_RGB) == TRUE);
   bool doSSS = (AiAOVEnabled(aovSSSName, AI_TYPE_RGB) == TRUE);

   if (numInputs > 0)
   {
      if (numInputs >= 16)
      {
         numInputs = 16;
      }

      AtColor orgOpacity = sg->out_opacity;

      AtRGB curColor = AI_RGB_BLACK;
      AtRGB curOpacity = AI_RGB_BLACK;
      AtRGB aov;

      for (AtInt i=0; i<numInputs; ++i)
      {
         AtRGB opacity;
         AtRGB color = AiShaderEvalParamRGB(p_color0+i);

         AtBoolean useTransparency = AiShaderEvalParamBool(p_useTransparency0+i);

         if (useTransparency)
         {
            opacity = 1.0f - AiShaderEvalParamRGB(p_transparency0+i);
            // consider color pre-multiplied already
         }
         else
         {
            opacity = sg->out_opacity;
            // pre-multiply color
            color.r *= opacity.r;
            color.g *= opacity.g;
            color.b *= opacity.b;
         }

         if (mode == CM_TEXTURE)
         {
            color *= opacity;
         }

         if (doDirectDiffuse && AiAOVGetRGB(sg, aovDirectDiffuseName, aov))
         {
            aovDirectDiffuse += (1.0f - curOpacity) * (mode == CM_TEXTURE ? opacity : AI_RGB_WHITE) * aov;
         }
         if (doDirectSpecular && AiAOVGetRGB(sg, aovDirectSpecularName, aov))
         {
            aovDirectSpecular += (1.0f - curOpacity) * (mode == CM_TEXTURE ? opacity : AI_RGB_WHITE) * aov;
         }
         if (doIndirectDiffuse && AiAOVGetRGB(sg, aovIndirectDiffuseName, aov))
         {
            aovIndirectDiffuse += (1.0f - curOpacity) * (mode == CM_TEXTURE ? opacity : AI_RGB_WHITE) * aov;
         }
         if (doIndirectSpecular && AiAOVGetRGB(sg, aovIndirectSpecularName, aov))
         {
            aovIndirectSpecular += (1.0f - curOpacity) * (mode == CM_TEXTURE ? opacity : AI_RGB_WHITE) * aov;
         }
         if (doEmission && AiAOVGetRGB(sg, aovEmissionName, aov))
         {
            aovEmission += (1.0f - curOpacity) * (mode == CM_TEXTURE ? opacity : AI_RGB_WHITE) * aov;
         }
         if (doSSS && AiAOVGetRGB(sg, aovSSSName, aov))
         {
            aovSSS += (1.0f - curOpacity) * (mode == CM_TEXTURE ? opacity : AI_RGB_WHITE) * aov;
         }
         if (doReflection && AiAOVGetRGB(sg, aovReflectionName, aov))
         {
            aovReflection += (1.0f - curOpacity) * (mode == CM_TEXTURE ? opacity : AI_RGB_WHITE) * aov;
         }
         if (doRefraction && AiAOVGetRGB(sg, aovRefractionName, aov))
         {
            aovRefraction += (1.0f - curOpacity) * (mode == CM_TEXTURE ? opacity : AI_RGB_WHITE) * aov;
         }

         curColor += (1.0f - curOpacity) * color;
         curOpacity += (1.0f - curOpacity) * opacity;

         sg->out_opacity = orgOpacity;
      }

      // un-premultiply color
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

   AiAOVSetRGB(sg, aovDirectDiffuseName, aovDirectDiffuse);
   AiAOVSetRGB(sg, aovDirectSpecularName, aovDirectSpecular);
   AiAOVSetRGB(sg, aovIndirectDiffuseName, aovIndirectDiffuse);
   AiAOVSetRGB(sg, aovIndirectSpecularName, aovIndirectSpecular);
   AiAOVSetRGB(sg, aovSSSName, aovSSS);
   AiAOVSetRGB(sg, aovEmissionName, aovEmission);
   AiAOVSetRGB(sg, aovReflectionName, aovReflection);
   AiAOVSetRGB(sg, aovRefractionName, aovRefraction);
}
