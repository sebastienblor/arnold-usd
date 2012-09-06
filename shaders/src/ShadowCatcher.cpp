#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ShadowCatcherMtd);

enum ShadowCatcherParams
{
   p_catch_shadows = 0,
   p_background_color,
   p_enable_transparency,
   p_shadow_transparency,
   
   p_catch_diffuse,
   p_diffuse_color
};

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiShadowCatcher");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D19);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/surface");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);
   
   AiParameterBOOL("catchShadows", true);
   AiParameterRGB("backgroundColor", 0.0f, 0.0f, 0.0f);
   AiParameterBOOL("enableTransparency", false);
   AiParameterRGB("shadowTransparency", 0.0f, 0.0f, 0.0f);
   
   AiParameterBOOL("catchDiffuse", false);
   AiParameterRGB("diffuseColor", 1.0f, 1.0f, 1.0f);
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
   if (sg->Rt & AI_RAY_SHADOW)
   {
      sg->out_opacity = AI_RGB_BLACK;
      return;
   }
   
   AtRGB backgroundColor = AiShaderEvalParamRGB(p_background_color);

   AtRGB result = AI_RGB_BLACK;
   AtRGB resultOpacity = AI_RGB_WHITE;
   float resultAlpha = 0.0f;   
      
   if (AiShaderEvalParamBool(p_catch_shadows))
   {
      AtRGB matte = AI_RGB_BLACK;
      matte = AiLightsGetShadowMatte(sg);
      resultAlpha = (matte.r + matte.g + matte.b) / 3.0f;      
      if (AiShaderEvalParamBool(p_enable_transparency))
         resultOpacity = matte * (1.0f - AiShaderEvalParamRGB(p_shadow_transparency));
      result = (AI_RGB_WHITE - matte) * backgroundColor;
   }
   else
      result = backgroundColor;
   
   if (AiShaderEvalParamBool(p_catch_diffuse))
   {
      AtRGB diffuse_color = AiShaderEvalParamRGB(p_diffuse_color);
      if (!AiColorIsSmall(diffuse_color))
         result += diffuse_color * AiIndirectDiffuse(&sg->Nf, sg) * backgroundColor;
   }
   
   sg->out.RGB = result;
   sg->out_opacity = resultOpacity;
   sg->out.RGBA.a = resultAlpha;
}
