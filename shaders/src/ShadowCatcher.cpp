#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ShadowCatcherMtd);

enum ShadowCatcherParams
{
   p_catch_shadows = 0,
   p_surface_shader,
   p_shadow_color,
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
   AiParameterRGBA("surfaceShader", 0.0f, 0.0f, 0.0f, 0.0f);
   AiParameterRGB("shadowColor", 0.0f, 0.0f, 0.0f);
   AiParameterBOOL("enableTransparency", false);
   AiParameterRGB("shadowTransparency", 0.0f, 0.0f, 0.0f);
   
   AiParameterBOOL("catchDiffuse", false);
   AiParameterRGB("diffuseColor", 0.0f, 0.0f, 0.0f);
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
   if (!(sg->Rt & AI_RAY_CAMERA))
   {
      sg->out.RGBA = AiShaderEvalParamRGBA(p_surface_shader);
      return;
   }
           
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
      matte *= AiShaderEvalParamRGB(p_shadow_color);
      result = matte;
   }
   
   if (AiShaderEvalParamBool(p_catch_diffuse))
   {
      result += AiShaderEvalParamRGB(p_diffuse_color) *
              AiIndirectDiffuse(&sg->Nf, sg);
   }
   
   sg->out.RGB = result;
   sg->out_opacity = resultOpacity;
   sg->out.RGBA.a = resultAlpha;
}
