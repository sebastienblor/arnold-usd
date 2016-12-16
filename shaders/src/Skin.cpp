#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(SkinMtd);

struct SkinData
{
    bool     specular_in_secondary_rays;
    bool     fresnel_affect_sss;
    bool     hasNormal;
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
   AiParameterBool("fresnel_affect_sss", true);
   AiParameterFlt("opacity", 1.0f);
   AiParameterRGB("opacity_color", 1.0f, 1.0f, 1.0f);

   AiParameterVec("normal", 0.f, 1.f, 0.f);
   AiMetaDataSetBool(nentry, "opacity_color", "always_linear", true);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiSkin");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x00115D20);
   AiMetaDataSetStr(nentry, NULL, "maya.classification", "shader/surface");
   AiMetaDataSetBool(nentry, NULL, "maya.swatch", true);

    // unlinkable parameters
   AiMetaDataSetBool(nentry, "specular_in_secondary_rays", "linkable", false);
   AiMetaDataSetBool(nentry, "fresnel_affect_sss", "linkable", false);
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
   p_fresnel_affect_sss,
   p_opacity,
   p_opacity_color,
   p_normal   
};

node_initialize
{
   AiNodeSetLocalData(node, AiMalloc(sizeof(SkinData)));
}

node_update
{
   SkinData *data = (SkinData*)AiNodeGetLocalData(node);

   data->specular_in_secondary_rays = AiNodeGetBool(node, AtString("specular_in_secondary_rays"));
   data->fresnel_affect_sss         = AiNodeGetBool(node, AtString("fresnel_affect_sss"));
   data->hasNormal = AiNodeIsLinked(node, AtString("normal"));
}

node_finish
{
   AiFree(AiNodeGetLocalData(node));
}

float SimpleFresnel(float dt, float ior)
{
   const float iorn = ior - 1.0f;
   const float iorp = ior + 1.0f;
   const float dtc = 1.0f - AiClamp(dt, 0.0f, 1.0f);
   float dt5 = dtc * dtc;
   dt5 *= dt5;
   dt5 *= dtc;
   return (iorn * iorn + 4.0f * ior * dt5) / (iorp * iorp);
}

shader_evaluate
{
   // handle opacity
   AtRGB opacity = AiShaderEvalParamFlt(p_opacity) * AiShaderEvalParamRGB(p_opacity_color);
   opacity = AiShaderGlobalsStochasticOpacity(sg, opacity);

   if (opacity != AI_RGB_WHITE)
   {
      sg->out.CLOSURE() = AiClosureTransparent(sg, AI_RGB_WHITE - opacity);

      // early out for nearly fully transparent objects
      if (AiAll(opacity < AI_OPACITY_EPSILON))
         return;
   }

   // early out for shadow rays
   if (sg->Rt & AI_RAY_SHADOW)
      return;

   SkinData *data = (SkinData*)AiNodeGetLocalData(node);

   AtVector oldN, oldNf;
   if (data->hasNormal)
   {
      oldN = sg->N;
      oldNf = sg->Nf;
      sg->N = AiShaderEvalParamVec(p_normal);
      sg->Nf = AiFaceViewer(sg);
   }
   AtClosureList closures;

   bool sampleOnlySSS = false;
   if (sg->bounces > 0 && (!data->specular_in_secondary_rays))
      sampleOnlySSS = true;

   float sheenFresnel = 0.0f;
   AtRGB sheen = AI_RGB_BLACK;

   float specularFresnel = 0.0f;
   AtRGB specular = AI_RGB_BLACK;
   
   if ((sg->bounces_diffuse == 0) && !sampleOnlySSS)
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
         AtBSDF* bsdfData = AiMicrofacetBSDF(sg, sheenWeight, AI_MICROFACET_BECKMANN, sg->Nf, &sg->dPdu, 0.0f, specularExponent, specularExponent);
         closures.add(bsdfData);
      }

      AtRGB specularWeight = AiShaderEvalParamRGB(p_specular_color);
      specularFresnel = SimpleFresnel(RDNF, AiShaderEvalParamFlt(p_specular_ior)) * AiShaderEvalParamFlt(p_specular_weight);
      specularWeight *= specularFresnel * (1.0f - sheenFresnel);

      if (!AiColorIsSmall(specularWeight))
      {
         float specularExponent = AiShaderEvalParamFlt(p_specular_roughness); 
         specularExponent *= specularExponent;
         AtBSDF* bsdfData = AiMicrofacetBSDF(sg, specularWeight, AI_MICROFACET_BECKMANN, sg->Nf, &sg->dPdu, 0.0f, specularExponent, specularExponent);
         closures.add(bsdfData);
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

      closures.add(AiClosureCubicBSSRDF(sg, colorWeights[0], AtVector(radiuses[0], radiuses[0], radiuses[0])));
      closures.add(AiClosureCubicBSSRDF(sg, colorWeights[1], AtVector(radiuses[1], radiuses[1], radiuses[1])));
      closures.add(AiClosureCubicBSSRDF(sg, colorWeights[2], AtVector(radiuses[2], radiuses[2], radiuses[2])));
   }

   if (data->hasNormal)
   {
      sg->N = oldN;
      sg->Nf = oldNf;
   }

   // write closures
   if (opacity != AI_RGB_WHITE)
   {
      closures *= opacity;
      sg->out.CLOSURE().add(closures);
   }
   else
   {
      sg->out.CLOSURE() = closures;
   }
}
