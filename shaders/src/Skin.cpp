#include <ai.h>

namespace MSTR
{
   static const AtString previous_roughness("previous_roughness");
}


AI_SHADER_NODE_EXPORT_METHODS(SkinMtd);

struct SkinData
{
    AtString aov_specular;
    AtString aov_sheen;
    AtString aov_sss;
    AtString aov_direct_sss;
    AtString aov_indirect_sss;
    bool     specular_in_secondary_rays;
    bool     fresnel_affect_sss;
};

node_parameters
{
   AiParameterFlt("sss_weight", 1.0f);
   AiParameterRGB("shallow_scatter_color", 1.0f, 0.909f, 0.769f);
   AiMetaDataSetBool(nentry, "shallow_scatter_color", "always_linear", true);
   AiParameterFlt("shallow_scatter_weight", 0.5f);
   AiParameterFlt("shallow_scatter_radius", 0.15f);
   AiParameterRGB("mid_scatter_color", 0.949f, 0.714f, 0.56f);
   AiMetaDataSetBool(nentry, "mid_scatter_color", "always_linear", true);
   AiParameterFlt("mid_scatter_weight", 0.25f);
   AiParameterFlt("mid_scatter_radius", 0.25f);
   AiParameterRGB("deep_scatter_color", 0.7f, 0.1f, 0.1f);
   AiMetaDataSetBool(nentry, "deep_scatter_color", "always_linear", true);
   AiParameterFlt("deep_scatter_weight", 1.0f);
   AiParameterFlt("deep_scatter_radius", 0.6f);
   AiParameterRGB("specular_color", 1.0f, 1.0f, 1.0f);
   AiMetaDataSetBool(nentry, "specular_color", "always_linear", true);
   AiParameterFlt("specular_weight", 0.8f);
   AiParameterFlt("specular_roughness", 0.5f);
   AiParameterFlt("specular_ior", 1.44f);
   AiParameterRGB("sheen_color", 1.0f, 1.0f, 1.0f);
   AiMetaDataSetBool(nentry, "sheen_color", "always_linear", true);
   AiParameterFlt("sheen_weight", 0.0f);
   AiParameterFlt("sheen_roughness", 0.35f);
   AiParameterFlt("sheen_ior", 1.44f);
   AiParameterFlt("global_sss_radius_multiplier", 1.0f);
   AiParameterBool("specular_in_secondary_rays", false);
   AiParameterStr("aov_specular", "specular");
   AiParameterStr("aov_sheen", "sheen");
   AiParameterStr("aov_sss", "sss");
   AiParameterStr("aov_direct_sss", "direct_sss");
   AiParameterStr("aov_indirect_sss", "indirect_sss");
   AiParameterBool("fresnel_affect_sss", true);
   AiParameterFlt("opacity", 1.0f);
   AiParameterRGB("opacity_color", 1.0f, 1.0f, 1.0f);
   AiMetaDataSetBool(nentry, "opacity_color", "always_linear", true);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiSkin");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x00115D20);
   AiMetaDataSetStr(nentry, NULL, "maya.classification", "shader/surface");
   AiMetaDataSetBool(nentry, NULL, "maya.swatch", true);

    // unlinkable parameters
   AiMetaDataSetBool(nentry, "specular_in_secondary_rays", "linkable", false);
   AiMetaDataSetBool(nentry, "fresnel_affect_sss", "linkable", false);
   AiMetaDataSetBool(nentry, "aov_specular", "linkable", false);
   AiMetaDataSetBool(nentry, "aov_sheen", "linkable", false);
   AiMetaDataSetBool(nentry, "aov_sss", "linkable", false);
   AiMetaDataSetBool(nentry, "aov_direct_sss", "linkable", false);
   AiMetaDataSetBool(nentry, "aov_indirect_sss", "linkable", false);
}
enum SSSParams {
   p_sss_weight,
   p_shallow_scatter_color,
   p_shallow_scatter_weight,
   p_shallow_scatter_radius,
   p_mid_scatter_color,
   p_mid_scatter_weight,
   p_mid_scatter_radius,
   p_deep_scatter_color,
   p_deep_scatter_weight,
   p_deep_scatter_radius,
   p_specular_color,
   p_specular_weight,
   p_specular_roughness,
   p_specular_ior,
   p_sheen_color,
   p_sheen_weight,
   p_sheen_roughness,
   p_sheen_ior,
   p_global_sss_radius_multiplier,
   p_specular_in_secondary_rays,
   p_aov_specular,
   p_aov_sheen,
   p_aov_sss,
   p_aov_direct_sss,
   p_aov_indirect_sss,
   p_fresnel_affect_sss,
   p_opacity,
   p_opacity_color   
};

node_initialize
{
   AiNodeSetLocalData(node, AiMalloc(sizeof(SkinData)));
}

node_update
{
   SkinData *data = (SkinData*)AiNodeGetLocalData(node);

   data->aov_specular     = AiNodeGetStr(node, AtString("aov_specular"));
   data->aov_sheen        = AiNodeGetStr(node, AtString("aov_sheen"));
   data->aov_sss          = AiNodeGetStr(node, AtString("aov_sss"));
   data->aov_direct_sss   = AiNodeGetStr(node, AtString("aov_direct_sss"));
   data->aov_indirect_sss = AiNodeGetStr(node, AtString("aov_indirect_sss"));

   data->specular_in_secondary_rays = AiNodeGetBool(node, AtString("specular_in_secondary_rays"));
   data->fresnel_affect_sss         = AiNodeGetBool(node, AtString("fresnel_affect_sss"));

   AiAOVRegister(data->aov_specular, AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(data->aov_sheen, AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(data->aov_sss, AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(data->aov_direct_sss, AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(data->aov_indirect_sss, AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);

}

node_finish
{
   AiFree(AiNodeGetLocalData(node));
}

float SimpleFresnel(float dt, float ior)
{
   const float iorn = ior - 1.0f;
   const float iorp = ior + 1.0f;
   const float dtc = 1.0f - CLAMP(dt, 0.0f, 1.0f);
   float dt5 = dtc * dtc;
   dt5 *= dt5;
   dt5 *= dtc;
   return (iorn * iorn + 4.0f * ior * dt5) / (iorp * iorp);
}

shader_evaluate
{
   if (sg->Rt & AI_RAY_SHADOW)
   {
      sg->out_opacity *= AiColorClamp(AiShaderEvalParamFlt(p_opacity) * AiShaderEvalParamRGB(p_opacity_color), 0.0f, 1.0f);
      return;
   }

   SkinData *data = (SkinData*)AiNodeGetLocalData(node);
   bool sampleOnlySSS = false;
   if ((sg->Rt & AI_RAY_DIFFUSE || sg->Rt & AI_RAY_GLOSSY) && (!data->specular_in_secondary_rays))
      sampleOnlySSS = true;

   float minRoughness = 0.0f;
   if (sg->Rr_diff > 0)
      minRoughness = 0.1f;
   else if (sg->Rr_gloss > 0)
   {
      // after a specular bounce clamp in proportion to its roughness (scaled by a "sharpness" coefficient)
      float minRoughness = 0;
      AiStateGetMsgFlt(MSTR::previous_roughness, &minRoughness);
      minRoughness  = 0.9f * minRoughness;
   }

   float sheenFresnel = 0.0f;
   AtRGB sheen = AI_RGB_BLACK;

   float specularFresnel = 0.0f;
   AtRGB specular = AI_RGB_BLACK;
   
   if ((sg->Rr_diff == 0) && !sampleOnlySSS)
   {
      AtRGB sheenWeight = AiShaderEvalParamRGB(p_sheen_color);
      AtVector reflected = AiReflect(sg->Rd, sg->Ns);
      const AtVector N = (AiV3Dot(sg->Ngf, reflected) < 0) ? sg->Ngf : sg->Nf;
      const float RDNF = -AiV3Dot(sg->Rd, N);
      sheenFresnel = SimpleFresnel(RDNF, AiShaderEvalParamFlt(p_sheen_ior)) * AiShaderEvalParamFlt(p_sheen_weight);
      sheenWeight *= sheenFresnel;
   
      float lastSpecRoughness = 1.0f;
      if (!AiColorIsSmall(sheenWeight))
      {
         float specularExponent = AiShaderEvalParamFlt(p_sheen_roughness);
         specularExponent *= specularExponent;
         if (sg->Rr_gloss > 0)
            specularExponent = AiMax(specularExponent, minRoughness);
         if (specularExponent < lastSpecRoughness)
            AiStateSetMsgFlt(MSTR::previous_roughness, specularExponent);
         void* brdfData = AiMicrofacetMISCreateData(sg, AI_MICROFACET_BECKMANN, &sg->dPdu, 0.0f, specularExponent, specularExponent);
         AiLightsPrepare(sg);
         while (AiLightsGetSample(sg))
         {
            if (AiLightGetAffectSpecular(sg->Lp))
            {
               const float affectSpecular = AiLightGetSpecular(sg->Lp);
               if (affectSpecular > AI_EPSILON)
                  sheen += AiEvaluateLightSample(sg, brdfData, NULL, NULL, NULL) * affectSpecular;
            }
         }

         sheen += AiBRDFIntegrate(sg, brdfData, NULL, NULL, NULL, AI_RAY_GLOSSY, AI_RGB_WHITE);
         sheen *= sheenWeight;
      }

      AtRGB specularWeight = AiShaderEvalParamRGB(p_specular_color);
      specularFresnel = SimpleFresnel(RDNF, AiShaderEvalParamFlt(p_specular_ior)) * AiShaderEvalParamFlt(p_specular_weight);
      specularWeight *= specularFresnel * (1.0f - sheenFresnel);

      if (!AiColorIsSmall(specularWeight))
      {
         float specularExponent = AiShaderEvalParamFlt(p_specular_roughness); 
         specularExponent *= specularExponent;
         if (sg->Rr_gloss > 0)
            specularExponent = AiMax(specularExponent, minRoughness);
         AiStateSetMsgFlt(MSTR::previous_roughness, specularExponent);
         void* brdfData = AiMicrofacetMISCreateData(sg, AI_MICROFACET_BECKMANN, &sg->dPdu, 0.0f, specularExponent, specularExponent);
         AiLightsPrepare(sg);
         while (AiLightsGetSample(sg))
         {
            if (AiLightGetAffectSpecular(sg->Lp))
            {
               const float affectSpecular = AiLightGetSpecular(sg->Lp);
               if (affectSpecular > AI_EPSILON)
                  specular += AiEvaluateLightSample(sg, brdfData, NULL, NULL, NULL) * affectSpecular;
            }
         }
         specular += AiBRDFIntegrate(sg, brdfData, NULL, NULL, NULL, AI_RAY_GLOSSY, AI_RGB_WHITE);
         specular *= specularWeight;
      }
   }

   AtRGB sss = AI_RGB_BLACK, directSSS = AI_RGB_BLACK, indirectSSS = AI_RGB_BLACK;

   float sssWeight = AiShaderEvalParamFlt(p_sss_weight);
   if (data->fresnel_affect_sss)
      sssWeight *= (1.0f - specularFresnel) * (1.0f - sheenFresnel);
   const bool enableSSS = sssWeight > AI_EPSILON;
   if (enableSSS)
   {
      const float globalSSSRadiusMultiplier = AiShaderEvalParamFlt(p_global_sss_radius_multiplier);
      const AtRGB colorWeights[3] = {
         AiShaderEvalParamRGB(p_shallow_scatter_color) * AiShaderEvalParamFlt(p_shallow_scatter_weight) * sssWeight,
         AiShaderEvalParamRGB(p_mid_scatter_color) * AiShaderEvalParamFlt(p_mid_scatter_weight) * sssWeight,
         AiShaderEvalParamRGB(p_deep_scatter_color) * AiShaderEvalParamFlt(p_deep_scatter_weight) * sssWeight
      };
      const float radiuses[3] = {
         AiShaderEvalParamFlt(p_shallow_scatter_radius) * globalSSSRadiusMultiplier, 
         AiShaderEvalParamFlt(p_mid_scatter_radius) * globalSSSRadiusMultiplier, 
         AiShaderEvalParamFlt(p_deep_scatter_radius) * globalSSSRadiusMultiplier
      };

      AiBSSRDFCubicSeparate(sg, directSSS, indirectSSS, radiuses, colorWeights, 3);
      sss = directSSS + indirectSSS;
      // sss = AiBSSRDFCubic(sg, radiuses, colorWeights, 3);
   }

   sg->out.RGB() = specular + sheen + sss;
   sg->out_opacity = AiColorClamp(AiShaderEvalParamFlt(p_opacity) * AiShaderEvalParamRGB(p_opacity_color), 0.0f, 1.0f);

   if (sg->Rt & AI_RAY_CAMERA)
   {      
      AiAOVSetRGB(sg, data->aov_specular, specular);
      AiAOVSetRGB(sg, data->aov_sheen, sheen);
      AiAOVSetRGB(sg, data->aov_sss, sss);
      AiAOVSetRGB(sg, data->aov_direct_sss, directSSS);
      AiAOVSetRGB(sg, data->aov_indirect_sss, indirectSSS);
   }
}
