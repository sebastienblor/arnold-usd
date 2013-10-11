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
   AiParameterRGB("primary_reflection_color", 0.75f, 0.9f, 1.0f);
   AiParameterFLT("primary_reflection_weight", 0.8f);
   AiParameterFLT("primary_reflection_roughness", 0.5f);
   AiParameterFLT("primary_reflection_specular_weight", 1.0f);
   AiParameterFLT("primary_reflection_reflection_weight", 0.0f);
   AiParameterBOOL("primary_reflection_enable_fresnel_falloff", true);
   AiParameterFLT("primary_reflection_fresnel_coefficient", 0.08f);
   AiParameterRGB("secondary_reflection_color", 0.75f, 0.9f, 1.0f);
   AiParameterFLT("secondary_reflection_weight", 0.6f);
   AiParameterFLT("secondary_reflection_roughness", 0.35f);
   AiParameterFLT("secondary_reflection_specular_weight", 1.0f);
   AiParameterFLT("secondary_reflection_reflection_weight", 0.0f);
   AiParameterBOOL("secondary_reflection_enable_fresnel_falloff", true);
   AiParameterFLT("secondary_reflection_fresnel_coefficient", 0.08f);
   AiParameterFLT("global_sss_radius_multiplier", 10.0f);
   AiParameterBOOL("sample_sss_only_in_gi_rays", true);
   AiParameterBOOL("sample_sss_only_in_glossy_rays", true);
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
   p_primary_reflection_color,
   p_primary_reflection_weight,
   p_primary_reflection_roughness,
   p_primary_reflection_specular_weight,
   p_primary_reflection_reflection_weight,
   p_primary_reflection_enable_fresnel_falloff,
   p_primary_reflection_fresnel_coefficient,
   p_secondary_reflection_color,
   p_secondary_reflection_weight,
   p_secondary_reflection_roughness,
   p_secondary_reflection_specular_weight,
   p_secondary_reflection_reflection_weight,
   p_secondary_reflection_enable_fresnel_falloff,
   p_secondary_reflection_fresnel_coefficient,
   p_global_sss_radius_multiplier,
   p_sample_sss_only_in_gi_rays,
   p_sample_sss_only_in_glossy_rays,
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

shader_evaluate
{

}
