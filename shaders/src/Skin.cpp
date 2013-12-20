#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(SkinMtd);

node_parameters
{
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
   AiParameterRGB("specular_color", 0.75f, 0.9f, 1.0f);
   AiParameterFLT("specular_weight", 0.8f);
   AiParameterFLT("specular_roughness", 0.5f);
   AiParameterFLT("specular_ior", 1.44f);
   AiParameterRGB("coat_color", 0.75f, 0.9f, 1.0f);
   AiParameterFLT("coat_weight", 0.0f);
   AiParameterFLT("coat_roughness", 0.35f);
   AiParameterFLT("coat_ior", 1.44f);
   AiParameterFLT("global_sss_radius_multiplier", 1.0f);
   AiParameterBOOL("specular_in_secondary_rays", false);
   AiParameterStr("aov_specular", "specular");
   AiParameterStr("aov_coat", "coat");
   AiParameterStr("aov_sss", "sss");

   AiMetaDataSetStr(mds, NULL, "maya.name", "aiSkin");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D20);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/surface");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", true);
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
   p_coat_color,
   p_coat_weight,
   p_coat_roughness,
   p_coat_ior,
   p_global_sss_radius_multiplier,
   p_specular_in_secondary_rays,
   p_aov_specular,
   p_aov_coat,
   p_aov_sss,
};

node_initialize
{

}

node_update
{
   AiAOVRegister(AiNodeGetStr(node, "aov_specular"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_coat"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_sss"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
}

node_finish
{

}

float SimpleFresnel(float dt, float ior) // TODO : OPTIMIZE
{
   return ((ior - 1.0f) * (ior - 1.0f) + 4.0f * ior * powf(CLAMP(1.0f - dt, 0.0f, 1.0f), 5.0f)) / ((ior + 1.0f) * (ior + 1.0f));
}

shader_evaluate
{
   if (sg->Rt & AI_RAY_SHADOW)
      return;

   bool sampleOnlySSS = false;
   if ((sg->Rt & AI_RAY_DIFFUSE || sg->Rt & AI_RAY_GLOSSY) && (!AiShaderEvalParamBool(p_specular_in_secondary_rays)))
      sampleOnlySSS = true;

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

   float coatFresnel = 0.0f;
   AtRGB coat = AI_RGB_BLACK;

   float specularFresnel = 0.0f;
   AtRGB specular = AI_RGB_BLACK;
   
   if ((sg->Rr_diff == 0) && !sampleOnlySSS)
   {
      AtRGB coatWeight = AiShaderEvalParamRGB(p_coat_color);
      const float RDNF = -AiV3Dot(sg->Rd, sg->Nf);
      coatFresnel = SimpleFresnel(RDNF, AiShaderEvalParamFlt(p_coat_ior)) * AiShaderEvalParamFlt(p_coat_weight);
      coatWeight *= coatFresnel;
   
      float lastSpecRoughness = 1.0f;
      if (!AiColorIsSmall(coatWeight))
      {
         float specularExponent = AiShaderEvalParamFlt(p_coat_roughness);
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
                  coat += AiEvaluateLightSample(sg, brdfData, AiCookTorranceMISSample, AiCookTorranceMISBRDF, AiCookTorranceMISPDF) * affectSpecular;
            }
         }

         coat += AiCookTorranceIntegrate(&sg->Nf, sg, &sg->dPdu, &sg->dPdv, specularExponent, specularExponent);
         coat *= coatWeight;
      }

      AtRGB specularWeight = AiShaderEvalParamRGB(p_specular_color);
      specularFresnel = SimpleFresnel(RDNF, AiShaderEvalParamFlt(p_specular_ior)) * AiShaderEvalParamFlt(p_specular_weight);
      specularWeight *= specularFresnel * (1.0f - coatFresnel);

      if (!AiColorIsSmall(specularWeight))
      {
         float specularExponent = AiShaderEvalParamFlt(p_specular_roughness); 
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
                  specular += AiEvaluateLightSample(sg, brdfData, AiCookTorranceMISSample, AiCookTorranceMISBRDF, AiCookTorranceMISPDF) * affectSpecular;
            }
         }
         specular += AiCookTorranceIntegrate(&sg->Nf, sg, &sg->dPdu, &sg->dPdv, specularExponent, specularExponent);
         specular *= specularWeight;
      }
   }

   AtRGB sss = AI_RGB_BLACK;

   float sssWeight = AiShaderEvalParamFlt(p_sss_weight) * (1.0f - specularFresnel) * (1.0f - coatFresnel);
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

      sss = AiBSSRDFCubic(sg, radiuses, colorWeights, 3);
   }

   sg->out.RGB = specular + coat + sss;

   if (sg->Rt & AI_RAY_CAMERA)
   {      
      AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_specular), specular);
      AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_coat), coat);
      AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_sss), sss);
   }
}
