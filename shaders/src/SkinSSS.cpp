#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(SkinSSSMtd);

node_parameters
{
   AiParameterRGB("diffuse_color", 1.0f, 1.0f, 1.0f);
   AiParameterFLT("diffuse_weight", 0.3f);
   AiParameterFLT("diffuse_roughness" , 0.0f);  // in [0,1]
   AiParameterFLT("sss_weight", 1.0f);
   AiParameterRGB("shallow_scatter_color", 1.0f, 0.909f, 0.769f);
   AiParameterFLT("shallow_scatter_weight", 0.5f);
   AiParameterFLT("shallow_scatter_radius", 0.15f);
   AiParameterRGB("mid_scatter_color", 0.949f, 0.714f, 0.56f);
   AiParameterFLT("mid_scatter_weight", 0.25f);
   AiParameterFLT("mid_scatter_radius", 0.25f);
   AiParameterRGB("deep_scatter_color", 0.7f, 0.1f, 0.1f);
   AiParameterFLT("deep_scatter_weight", 1.0f);
   AiParameterFLT("deep_scatter_radius", 0.6f);
   AiParameterRGB("primary_specular_color", 0.75f, 0.9f, 1.0f);
   AiParameterFLT("primary_specular_weight", 0.8f);
   AiParameterFLT("primary_specular_roughness", 0.5f);
   AiParameterBOOL("primary_specular_enable_fresnel_falloff", true);
   AiParameterFLT("primary_specular_ior", 1.33f);
   AiParameterRGB("secondary_specular_color", 0.75f, 0.9f, 1.0f);
   AiParameterFLT("secondary_specular_weight", 0.6f);
   AiParameterFLT("secondary_specular_roughness", 0.35f);
   AiParameterBOOL("secondary_specular_enable_fresnel_falloff", true);
   AiParameterFLT("secondary_specular_ior", 1.33f);
   AiParameterFLT("global_sss_radius_multiplier", 10.0f);
   AiParameterBOOL("sample_sss_only_in_gi_rays", true);
   AiParameterBOOL("sample_sss_only_in_glossy_rays", true);
   AiParameterBOOL("combine_sss_queries", false);
   AiParameterStr("aov_direct_diffuse", "direct_diffuse");
   AiParameterStr("aov_indirect_diffuse", "indirect_diffuse");
   AiParameterStr("aov_primary_specular", "primary_specular");
   AiParameterStr("aov_secondary_specular", "secondary_specular");
   AiParameterStr("aov_shallow_scatter", "shallow_scatter");
   AiParameterStr("aov_mid_scatter", "mid_scatter");
   AiParameterStr("aov_deep_scatter", "deep_scatter");

   AiMetaDataSetStr(mds, NULL, "maya.name", "aiSkinSSSEx");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D20);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/surface");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", true);
}

enum SSSParams {
   p_diffuse_color,
   p_diffuse_weight,
   p_diffuse_roughness,
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
   p_primary_specular_color,
   p_primary_specular_weight,
   p_primary_specular_roughness,
   p_primary_specular_enable_fresnel_falloff,
   p_primary_specular_ior,
   p_secondary_specular_color,
   p_secondary_specular_weight,
   p_secondary_specular_roughness,
   p_secondary_specular_enable_fresnel_falloff,
   p_secondary_specular_ior,
   p_global_sss_radius_multiplier,
   p_sample_sss_only_in_gi_rays,
   p_sample_sss_only_in_glossy_rays,
   p_combine_sss_queries,
   p_aov_direct_diffuse,
   p_aov_indirect_diffuse,
   p_aov_primary_specular,
   p_aov_secondary_specular,
   p_aov_shallow_scatter,
   p_aov_mid_scatter,
   p_aov_deep_scatter
};

node_initialize
{

}

node_update
{

}

node_finish
{

}

float SimpleFresnel(float dt, float ior) // TODO : OPTIMIZE
{
   return ((ior - 1.0f) * (ior - 1.0f) + 4.0f * ior * powf(MAX(1.0f - dt, 0.0f), 5.0f)) / ((ior + 1.0f) * (ior + 1.0f));
}

shader_evaluate
{
   if (sg->Rt & AI_RAY_SHADOW)
      return;

   bool sampleOnlySSS = false;
   if (sg->Rt & AI_RAY_DIFFUSE && AiShaderEvalParamBool(p_sample_sss_only_in_gi_rays))
      sampleOnlySSS = true;
   else if (sg->Rt & AI_RAY_GLOSSY && AiShaderEvalParamBool(p_sample_sss_only_in_glossy_rays))
      sampleOnlySSS = true;


   bool enableDiffuse = false;
   AtRGB diffuseColor = AI_RGB_BLACK;
   if (!sampleOnlySSS)
   {
      diffuseColor = AiShaderEvalParamRGB(p_diffuse_color) * AiShaderEvalParamFlt(p_diffuse_weight);
      enableDiffuse = !AiColorIsSmall(diffuseColor);
   }

   AtRGB directDiffuse = AI_RGB_BLACK;
   AtRGB indirectDiffuse = AI_RGB_BLACK;
   if (enableDiffuse)
   {
      const float diffuseRoughness = AiShaderEvalParamFlt(p_diffuse_roughness);   
      void* orenNayarBRDF = AiOrenNayarMISCreateData(sg, diffuseRoughness);

      AiLightsPrepare(sg);
      while (AiLightsGetSample(sg))
      {
         if (AiLightGetAffectDiffuse(sg->Lp))
         {
            const float diffuseAffect = AiLightGetDiffuse(sg->Lp);
            if (diffuseAffect > AI_EPSILON)
               directDiffuse += AiEvaluateLightSample(sg, orenNayarBRDF, AiOrenNayarMISSample, AiOrenNayarMISBRDF, AiOrenNayarMISPDF) * diffuseAffect;
         }
      }

      if (diffuseRoughness > 0)
         indirectDiffuse = AiOrenNayarIntegrate(&sg->Nf, sg, diffuseRoughness);
      else
         indirectDiffuse = AiIndirectDiffuse(&sg->Nf, sg);
      directDiffuse *= diffuseColor;
      indirectDiffuse *= diffuseColor;
   }

   float minRoughness = 0.0f;
   if (sg->Rr_diff > 0)
      minRoughness = 0.1f;
   else if (sg->Rr_gloss > 0)
   {
      // after a specular bounce clamp in proportion to its roughness (scaled by a "sharpness" coefficient)
      float minRoughness = 0;
      AiStateGetMsgFlt("previous_roughness", &minRoughness);
      minRoughness  = 0.9f * minRoughness;
   }

   AtRGB primarySpecular = AI_RGB_BLACK;
   AtRGB primarySpecularWeight = AI_RGB_BLACK;
   bool enablePrimarySpecular = false;
   if ((sg->Rr_diff == 0) && (sg->Rr_gloss == 0) && !sampleOnlySSS)
   {
      primarySpecularWeight = AiShaderEvalParamRGB(p_primary_specular_color) * AiShaderEvalParamFlt(p_primary_specular_weight);
      if (AiShaderEvalParamBool(p_primary_specular_enable_fresnel_falloff))
         primarySpecularWeight *= SimpleFresnel(-AiV3Dot(sg->Rd, sg->Nf), AiShaderEvalParamFlt(p_primary_specular_ior));
      enablePrimarySpecular = !AiColorIsSmall(primarySpecularWeight);
   }

   float lastSpecRoughness = 1.0f;
   if (enablePrimarySpecular)
   {
      float specularExponent = AiShaderEvalParamFlt(p_primary_specular_roughness);      
      specularExponent *= specularExponent;
      if (sg->Rr_gloss > 0)
         specularExponent = MAX(specularExponent, minRoughness);
      AiStateSetMsgFlt("previous_roughness", specularExponent);
      void* brdfData = AiCookTorranceMISCreateData(sg, &sg->dPdu, &sg->dPdv, specularExponent, specularExponent);
      AiLightsPrepare(sg);
      while (AiLightsGetSample(sg))
      {
         if (AiLightGetAffectSpecular(sg->Lp))
         {
            const float affectSpecular = AiLightGetSpecular(sg->Lp);
            if (affectSpecular > AI_EPSILON)
               primarySpecular += AiEvaluateLightSample(sg, brdfData, AiCookTorranceMISSample, AiCookTorranceMISBRDF, AiCookTorranceMISPDF) * affectSpecular;
         }
      }

      primarySpecular += AiCookTorranceIntegrate(&sg->Nf, sg, &sg->dPdu, &sg->dPdv, specularExponent, specularExponent);

      primarySpecular *= primarySpecularWeight;
   }

   AtRGB secondarySpecular = AI_RGB_BLACK;
   AtRGB secondarySpecularWeight = AI_RGB_BLACK;
   if ((sg->Rr_diff == 0) && (sg->Rr_gloss == 0))
   {
      secondarySpecularWeight = AiShaderEvalParamRGB(p_secondary_specular_color) * AiShaderEvalParamFlt(p_secondary_specular_weight);
      if (AiShaderEvalParamBool(p_secondary_specular_enable_fresnel_falloff))
         secondarySpecularWeight *= SimpleFresnel(-AiV3Dot(sg->Rd, sg->Nf), AiShaderEvalParamFlt(p_secondary_specular_ior));
   }
   const bool enableSecondarySpecular = !AiColorIsSmall(secondarySpecularWeight);

   if (enableSecondarySpecular)
   {
      float specularExponent = AiShaderEvalParamFlt(p_secondary_specular_roughness);
      specularExponent *= specularExponent;
      if (sg->Rr_gloss > 0)
         specularExponent = MAX(specularExponent, minRoughness);
      if (specularExponent < lastSpecRoughness)
         AiStateSetMsgFlt("previous_roughness", specularExponent);
      void* brdfData = AiCookTorranceMISCreateData(sg, &sg->dPdu, &sg->dPdv, specularExponent, specularExponent);
      AiLightsPrepare(sg);
      while (AiLightsGetSample(sg))
      {
         if (AiLightGetAffectSpecular(sg->Lp))
         {
            const float affectSpecular = AiLightGetSpecular(sg->Lp);
            if (affectSpecular > AI_EPSILON)
               secondarySpecular += AiEvaluateLightSample(sg, brdfData, AiCookTorranceMISSample, AiCookTorranceMISBRDF, AiCookTorranceMISPDF) * affectSpecular;
         }
      }

      secondarySpecular += AiCookTorranceIntegrate(&sg->Nf, sg, &sg->dPdu, &sg->dPdv, specularExponent, specularExponent);
      secondarySpecular *= secondarySpecularWeight;
   }

   AtRGB shallowScatter = AI_RGB_BLACK;
   AtRGB midScatter = AI_RGB_BLACK;
   AtRGB deepScatter = AI_RGB_BLACK;

#if AI_VERSION_ARCH_NUM == 4 && AI_VERSION_MAJOR_NUM == 1
   const float sssWeight = AiShaderEvalParamFlt(p_sss_weight);
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

      if (AiShaderEvalParamBool(p_combine_sss_queries))
      {
         if (!AiColorIsSmall(colorWeights[0]) && (radiuses[0] > AI_EPSILON))
            shallowScatter = AiBSSRDFCubic(sg, radiuses, colorWeights, 3);
      }         
      else
      {
         if (!AiColorIsSmall(colorWeights[0]) && (radiuses[0] > AI_EPSILON))
            shallowScatter = AiBSSRDFCubic(sg, &radiuses[0], &colorWeights[0], 1);
         if (!AiColorIsSmall(colorWeights[1]) && (radiuses[1] > AI_EPSILON))
            midScatter = AiBSSRDFCubic(sg, &radiuses[1], &colorWeights[1], 1);
         if (!AiColorIsSmall(colorWeights[2]) && (radiuses[2] > AI_EPSILON))
            deepScatter = AiBSSRDFCubic(sg, &radiuses[2], &colorWeights[2], 1);
      }
   }
#endif


   sg->out.RGB = directDiffuse + indirectDiffuse +
                 primarySpecular + secondarySpecular +
                 shallowScatter + midScatter + deepScatter;
}
