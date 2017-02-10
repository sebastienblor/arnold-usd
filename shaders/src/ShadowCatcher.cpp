#include <ai.h>

namespace MSTR
{
   static const AtString shadow("shadow");
   static const AtString shadow_matte("shadow_matte");
   static const AtString shadow_diff("shadow_diff");
   static const AtString shadow_mask("shadow_mask");
   static const AtString indirect_diffuse("indirect_diffuse");
   static const AtString reflection("reflection");
}

AI_SHADER_NODE_EXPORT_METHODS(ShadowCatcherMtd);

enum ShadowCatcherParams
{
   p_catch_shadows = 0,
   p_background_color,
   p_shadow_color,
   p_backlighting,
   p_enable_transparency,
   p_shadow_transparency,
   
   p_catch_diffuse,
   p_diffuse_color,
   
   p_reflection,
   p_legacy,
   p_aov_shadow,
   p_aov_shadow_matte,
   p_aov_shadow_diff,
   p_aov_shadow_mask,
   p_aov_indirect_diffuse,
   p_aov_reflection
};

node_parameters
{
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiShadowCatcher");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x00115D19);
   AiMetaDataSetStr(nentry, NULL, "maya.classification", "shader/surface");
   AiMetaDataSetBool(nentry, NULL, "maya.swatch", false);

   AiParameterBool("catchShadows", true);
   AiParameterRGB("backgroundColor", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("shadowColor", 0.f, 0.f, 0.f);
   AiParameterFlt("backlighting", 0.f);
   AiParameterBool("enableTransparency", false);
   AiParameterRGB("shadowTransparency", 0.0f, 0.0f, 0.0f);
   
   AiParameterBool("catchDiffuse", false);
   AiParameterRGB("diffuseColor", 1.0f, 1.0f, 1.0f);
   
   AiParameterRGB("reflection", 0.f, 0.f, 0.f);
   AiParameterBool("legacy", false);

   AiParameterStr("aov_shadow", "shadow");
   AiParameterStr("aov_shadow_matte", "shadow_matte");
   AiParameterStr("aov_shadow_diff", "shadow_diff");
   AiParameterStr("aov_shadow_mask", "shadow_mask");
   AiParameterStr("aov_indirect_diffuse", "indirect_diffuse");
   AiParameterStr("aov_reflection", "reflection");

}

node_initialize
{

}

node_update
{
   AiAOVRegister(AiNodeGetStr(node, "aov_shadow"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_shadow_matte"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_shadow_diff"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_shadow_mask"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_indirect_diffuse"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_reflection"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
}

node_finish
{
}

shader_evaluate
{
   if (sg->Rt & AI_RAY_SHADOW)
      return;
   
   AtRGB backgroundColor = AiShaderEvalParamRGB(p_background_color);
   AtRGB shadowColor = AiShaderEvalParamRGB(p_shadow_color);

   AtRGB result = AI_RGB_BLACK;
   AtRGB resultOpacity = AI_RGB_WHITE;
   
   
   AtRGB Li = AI_RGB_BLACK;
   AtRGB Liu = AI_RGB_BLACK;
   AtRGB matte = AI_RGB_BLACK;
   
   if (AiShaderEvalParamBool(p_catch_shadows))
   {
      
      if (AiShaderEvalParamBool(p_legacy))
      {
         matte = AiLightsIntegrateShadowMatte(sg);
      } else
      {
         float Kb = AiShaderEvalParamFlt(p_backlighting);
         

         if (Kb > 0.0f)
            sg->fhemi = false;

         AtLightSample light_sample;
         AiLightsPrepare(sg);
         while (AiLightsGetSample(sg, light_sample))
         {
            if (AiLightGetDiffuse(light_sample.Lp) > 0.0f)
            {
               float d = AiV3Dot(sg->Nf, light_sample.Ld) / light_sample.pdf;
               // backlighting
               if (d < 0.0f)
                   d *= -Kb;

               Li += light_sample.Li * d;
               Liu += light_sample.Liu * d;

            }
         }

         // shadow matte
         // NOTE We invert the value, so we have a black background for the whole image.
         // Otherwise we have white backgroung on the geometry (e.g. Plane) the shadow_matte
         // shader is assigned to and black background everywhere else.
         // In composite this AOV has to be inverted and multiplied by the direct diffuse.
         if (Liu.r != 0.0f) matte.r = 1.0f - Li.r / Liu.r;
         if (Liu.g != 0.0f) matte.g = 1.0f - Li.g / Liu.g;
         if (Liu.b != 0.0f) matte.b = 1.0f - Li.b / Liu.b;
      }
      
      if (AiShaderEvalParamBool(p_enable_transparency))
         resultOpacity = matte * (1.0f - AiShaderEvalParamRGB(p_shadow_transparency));
      result = AiLerp(matte, backgroundColor, shadowColor);
   }
   else
      result = backgroundColor;
   
   AtRGB indirect_diffuse = AI_RGB_BLACK;
   AtRGB Kd = AI_RGB_WHITE;
   if (AiShaderEvalParamBool(p_catch_diffuse))
   {
      AtRGB diffuse_color = AiShaderEvalParamRGB(p_diffuse_color);
      Kd = diffuse_color;
      if (!AiColorIsSmall(diffuse_color))
         indirect_diffuse = diffuse_color * AiIndirectDiffuse(sg->Nf, sg, AI_RGB_WHITE) * backgroundColor;
   }
   float resultAlpha = (matte.r + matte.g + matte.b) / 3.0f;      
   
   // shadow diff
   AtRGB shadow_diff = AI_ONEOVERPI * Kd * (Liu - Li);

   // shadow mask
   AtRGB shadow_mask = AiAll(matte < AI_EPSILON) ? AI_RGB_BLACK : AI_RGB_WHITE;

   AtRGB reflection = AiShaderEvalParamRGB(p_reflection);

   AiAOVSetRGB(sg, MSTR::shadow, result);
   AiAOVSetRGB(sg, MSTR::shadow_matte, matte);
   AiAOVSetRGB(sg, MSTR::shadow_diff, shadow_diff);
   AiAOVSetRGB(sg, MSTR::shadow_mask, shadow_mask);
   AiAOVSetRGB(sg, MSTR::indirect_diffuse, indirect_diffuse);
   AiAOVSetRGB(sg, MSTR::reflection, reflection);


   result += indirect_diffuse;
   result += reflection;

   AtClosureList closures;
   closures.add(AiClosureEmission(sg, resultOpacity * result));
   closures.add(AiClosureMatte(sg, resultOpacity * (AI_RGB_WHITE - resultAlpha)));
   closures.add(AiClosureTransparent(sg, AI_RGB_WHITE - resultOpacity));
   sg->out.CLOSURE() = closures;   
}
