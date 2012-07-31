#include <ai.h>
#include <cstring>
#include <math.h>
#include "skin_sss.h"

AI_SHADER_NODE_EXPORT_METHODS(SkinSssMethods);

node_parameters
{
   AiParameterRGB ("diffuse_color", 1.0f, 1.0f, 1.0f);
   AiParameterFLT ("diffuse_weight", 0.3f);
   AiParameterFLT ("sss_weight", 1.0f);
   AiParameterRGB ("shallow_scatter_color", 1.0f, 0.909f, 0.769f);
   AiParameterFLT ("shallow_scatter_weight", 0.5f);
   AiParameterFLT ("shallow_scatter_radius", 0.15f);
   AiParameterRGB ("mid_scatter_color", 0.949f, 0.714f, 0.56f);
   AiParameterFLT ("mid_scatter_weight", 0.25f);
   AiParameterFLT ("mid_scatter_radius", 0.25f);
   AiParameterRGB ("deep_scatter_color", 0.7f, 0.1f, 0.1f);
   AiParameterFLT ("deep_scatter_weight", 1.0f);
   AiParameterFLT ("deep_scatter_radius", 0.6f);
   AiParameterRGB ("primary_reflection_color", 0.75f, 0.9f, 1.0f);
   AiParameterFLT ("primary_reflection_weight", 0.8f);
   AiParameterFLT ("primary_reflection_roughness", 0.5f);
   AiParameterFLT ("primary_reflection_specular_weight", 1.0f);
   AiParameterFLT ("primary_reflection_reflection_weight", 0.0f);
   AiParameterBOOL("primary_reflection_enable_fresnel_falloff", true);
   AiParameterFLT ("primary_reflection_fresnel_coefficient", 0.08f);
   AiParameterRGB ("secondary_reflection_color", 0.75f, 0.9f, 1.0f);
   AiParameterFLT ("secondary_reflection_weight", 0.6f);
   AiParameterFLT ("secondary_reflection_roughness", 0.35f);
   AiParameterFLT ("secondary_reflection_specular_weight", 1.0f);
   AiParameterFLT ("secondary_reflection_reflection_weight", 0.0f);
   AiParameterBOOL("secondary_reflection_enable_fresnel_falloff", true);
   AiParameterFLT ("secondary_reflection_fresnel_coefficient", 0.08f);
   AiParameterFLT ("global_sss_radius_multiplier", 10.0f);
   AiParameterBOOL("use_screen_compositing_for_colors", true);
   AiParameterBOOL("sample_sss_only_in_gi_rays", true);
   AiParameterBOOL("sample_sss_only_in_glossy_rays", true);
   // initialize the aov names by the MtoA nomenclature, it makes no difference for Soft
   AiParameterStr ("aov_direct_diffuse",     "direct_diffuse");
   AiParameterStr ("aov_indirect_diffuse",   "indirect_diffuse");
   AiParameterStr ("aov_primary_specular",   "primary_specular");
   AiParameterStr ("aov_secondary_specular", "secondary_specular");
   AiParameterStr ("aov_shallow_scatter",    "shallow_scatter");
   AiParameterStr ("aov_mid_scatter",        "mid_scatter");
   AiParameterStr ("aov_deep_scatter",       "deep_scatter");
}


// Param evaluation, called at shader_evaluate time. Standard param evaluation
void CSSSParams::Evaluate(AtNode* node, AtShaderGlobals *sg)
{
   diffuse_color          = AiShaderEvalParamRGB(p_diffuse_color);
   diffuse_weight         = AiShaderEvalParamFlt(p_diffuse_weight);
   sss_weight             = AiShaderEvalParamFlt(p_sss_weight);
   shallow_scatter_color  = AiShaderEvalParamRGB(p_shallow_scatter_color);
   shallow_scatter_weight = AiShaderEvalParamFlt(p_shallow_scatter_weight);
   shallow_scatter_radius = AiShaderEvalParamFlt(p_shallow_scatter_radius);
   mid_scatter_color      = AiShaderEvalParamRGB(p_mid_scatter_color);
   mid_scatter_weight     = AiShaderEvalParamFlt(p_mid_scatter_weight);
   mid_scatter_radius     = AiShaderEvalParamFlt(p_mid_scatter_radius);
   deep_scatter_color     = AiShaderEvalParamRGB(p_deep_scatter_color);
   deep_scatter_weight    = AiShaderEvalParamFlt(p_deep_scatter_weight);
   deep_scatter_radius    = AiShaderEvalParamFlt(p_deep_scatter_radius);
   primary_reflection_color               = AiShaderEvalParamRGB(p_primary_reflection_color);
   primary_reflection_weight              = AiShaderEvalParamFlt(p_primary_reflection_weight);
   primary_reflection_roughness           = AiShaderEvalParamFlt(p_primary_reflection_roughness);
   primary_reflection_specular_weight     = AiShaderEvalParamFlt(p_primary_reflection_specular_weight);
   primary_reflection_reflection_weight   = AiShaderEvalParamFlt(p_primary_reflection_reflection_weight);
   primary_reflection_enable_fresnel_falloff   = AiShaderEvalParamBool(p_primary_reflection_enable_fresnel_falloff);
   primary_reflection_fresnel_coefficient      = AiShaderEvalParamFlt(p_primary_reflection_fresnel_coefficient);
   secondary_reflection_color               = AiShaderEvalParamRGB(p_secondary_reflection_color);
   secondary_reflection_weight              = AiShaderEvalParamFlt(p_secondary_reflection_weight);
   secondary_reflection_roughness           = AiShaderEvalParamFlt(p_secondary_reflection_roughness);
   secondary_reflection_specular_weight     = AiShaderEvalParamFlt(p_secondary_reflection_specular_weight);
   secondary_reflection_reflection_weight   = AiShaderEvalParamFlt(p_secondary_reflection_reflection_weight);
   secondary_reflection_enable_fresnel_falloff   = AiShaderEvalParamBool(p_secondary_reflection_enable_fresnel_falloff);
   secondary_reflection_fresnel_coefficient      = AiShaderEvalParamFlt(p_secondary_reflection_fresnel_coefficient);
   global_sss_radius_multiplier = AiShaderEvalParamFlt(p_global_sss_radius_multiplier);
   use_screen_compositing_for_colors   = AiShaderEvalParamBool(p_use_screen_compositing_for_colors);
   sample_sss_only_in_gi_rays          = AiShaderEvalParamBool(p_sample_sss_only_in_gi_rays);
   sample_sss_only_in_glossy_rays      = AiShaderEvalParamBool(p_sample_sss_only_in_glossy_rays);
}


node_initialize
{
   // Initialize by options. iData hosts all the nodes in the graph
   CInstanceData *iData = new CInstanceData(AiUniverseGetOptions());
   // Set iData as the shader's local data
   AiNodeSetLocalData(node, (void*)iData);

   // Build the graph, exactly as in the Andreas compound.
   // The connections are established through the p_* CNode pointer of the connectable parameters.
   // This must be done only once

   // Ray_Switch
   iData->Ray_Switch.p_camera = &iData->Screen_Switch;
   iData->Ray_Switch.p_reflection = &iData->Screen_Switch;
   iData->Ray_Switch.p_refraction = &iData->Screen_Switch;
   iData->Ray_Switch.p_diffuse = &iData->Diffuse_Switch;
   iData->Ray_Switch.p_glossy = &iData->Glossy_Switch;
   // Diffuse_Switch 
   iData->Diffuse_Switch.p_input1 = &iData->Screen_Switch;
   iData->Diffuse_Switch.p_input2 = &iData->Standard;
   // Glossy_Switch 
   iData->Glossy_Switch.p_input1 = &iData->Screen_Switch;
   iData->Glossy_Switch.p_input2 = &iData->Standard;
   // Screen_Switch
   iData->Screen_Switch.p_input1 = &iData->Standards_Add;
   iData->Screen_Switch.p_input2 = &iData->Screen;
   // Standard
   iData->Standard.p_Ksss_color = &iData->Colors_Add;
   iData->Standard.p_sss_radius = &iData->Radius_To_Color;

   // #1311
   iData->Store_Secondary_Specular.p_input = &iData->Standard_Secondary_Specular;
   iData->Store_Shallow_Scatter.p_input = &iData->Standard_SSS_Shallow;
   iData->Store_Mid_Scatter.p_input = &iData->Standard_SSS_Mid;
   iData->Store_Deep_Scatter.p_input = &iData->Standard_SSS_Deep;

   // Standards_Add
   // iData->Standards_Add.p_baseColor = &iData->Standard_Diffuse_Primary_Specular;
   // iData->Standards_Add.p_color1 = &iData->Standard_Secondary_Specular;
   // iData->Standards_Add.p_color2 = &iData->Standard_SSS_Shallow;
   // iData->Standards_Add.p_color3 = &iData->Standard_SSS_Mid;
   // iData->Standards_Add.p_color4 = &iData->Standard_SSS_Deep;
   // #1311
   iData->Standards_Add.p_baseColor = &iData->Standard_Diffuse_Primary_Specular;
   iData->Standards_Add.p_color1 = &iData->Store_Secondary_Specular;
   iData->Standards_Add.p_color2 = &iData->Store_Shallow_Scatter;
   iData->Standards_Add.p_color3 = &iData->Store_Mid_Scatter;
   iData->Standards_Add.p_color4 = &iData->Store_Deep_Scatter;

   // Screen
   iData->Screen.p_baseColor = &iData->Clip_Diffuse_Primary_Specular;
   iData->Screen.p_color1 = &iData->Clip_Secondary_Specular;
   iData->Screen.p_color2 = &iData->Clip_Shallow;
   iData->Screen.p_color3 = &iData->Clip_Mid;
   iData->Screen.p_color4 = &iData->Clip_Deep;


   // Clip_Diffuse_Primary_Specular
   iData->Clip_Diffuse_Primary_Specular.p_color = &iData->Standard_Diffuse_Primary_Specular;
   // Clip_Secondary_Specular
   iData->Clip_Secondary_Specular.p_color = &iData->Store_Secondary_Specular;
   // Clip_Shallow
   iData->Clip_Shallow.p_color = &iData->Store_Shallow_Scatter;
   // Clip_Mid
   iData->Clip_Mid.p_color = &iData->Store_Mid_Scatter;
   // Clip_Deep
   iData->Clip_Deep.p_color = &iData->Store_Deep_Scatter;
   // Standard_SSS_Shallow
   iData->Standard_SSS_Shallow.p_Ksss = &iData->Multiply_ShallowSSS;
   iData->Standard_SSS_Shallow.p_sss_radius = &iData->Shallow_Radius_Scalar_To_Color;
   // Standard_SSS_Mid
   iData->Standard_SSS_Mid.p_Ksss = &iData->Multiply_MidSSS;
   iData->Standard_SSS_Mid.p_sss_radius = &iData->Mid_Radius_Scalar_To_Color;
   // Standard_SSS_Deep
   iData->Standard_SSS_Deep.p_Ksss = &iData->Multiply_DeepSSS;
   iData->Standard_SSS_Deep.p_sss_radius = &iData->Deep_Radius_Scalar_To_Color;
   // Shallow_Radius_Scalar_To_Color
   iData->Shallow_Radius_Scalar_To_Color.p_input = &iData->Multiply_Shallow_Radius;
   // Mid_Radius_Scalar_To_Color
   iData->Mid_Radius_Scalar_To_Color.p_input = &iData->Multiply_Mid_Radius;
   // Deep_Radius_Scalar_To_Color
   iData->Deep_Radius_Scalar_To_Color.p_input = &iData->Multiply_Deep_Radius;
}


node_update 
{
   AiAOVRegister(AiNodeGetStr(node, "aov_direct_diffuse"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_indirect_diffuse"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_primary_specular"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_secondary_specular"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_shallow_scatter"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_mid_scatter"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
   AiAOVRegister(AiNodeGetStr(node, "aov_deep_scatter"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);

   // Get the instance data
   CInstanceData *iData = (CInstanceData*)AiNodeGetLocalData(node);

   const char *directDiffuse     = AiNodeGetStr(node, "aov_direct_diffuse");
   const char *inDirectDiffuse   = AiNodeGetStr(node, "aov_indirect_diffuse");
   const char *primarySpecular   = AiNodeGetStr(node, "aov_primary_specular");
   const char *secondarySpecular = AiNodeGetStr(node, "aov_secondary_specular");
   const char *shallowScatter    = AiNodeGetStr(node, "aov_shallow_scatter");
   const char *midScatter        = AiNodeGetStr(node, "aov_mid_scatter");
   const char *deepScatter       = AiNodeGetStr(node, "aov_deep_scatter");

   // wasting time here, the string is read only, so thread safe.
   // Anyway, it's a one time only assignment, let's keep the param structure consistent
   // with all the other CNode types
   for (int i=0; i<MAX_NB_THREADS; i++)
   {
      strcpy(iData->Store_Secondary_Specular.params[i].channel, secondarySpecular);
      strcpy(iData->Store_Shallow_Scatter.params[i].channel,    shallowScatter);
      strcpy(iData->Store_Mid_Scatter.params[i].channel,        midScatter);
      strcpy(iData->Store_Deep_Scatter.params[i].channel,       deepScatter);
      // the only standard contributing to the diffuse AOVs:
      iData->Standard_Diffuse_Primary_Specular.params[i].writeAOVs = true;
      strcpy(iData->Standard_Diffuse_Primary_Specular.params[i].primarySpecularAOV, primarySpecular);
      strcpy(iData->Standard_Diffuse_Primary_Specular.params[i].directDiffuseAOV,   directDiffuse);
      strcpy(iData->Standard_Diffuse_Primary_Specular.params[i].inDirectDiffuseAOV, inDirectDiffuse);
   }

}


node_finish
{
   delete (CInstanceData*)AiNodeGetLocalData(node);
}


shader_evaluate
{
   CSSSParams params;
   // Evaluate all the texturable paramaters
   params.Evaluate(node, sg);
   // Get the instance data
   CInstanceData *iData = (CInstanceData*)AiNodeGetLocalData(node);
   COptions options = iData->options;

   // Set the texturable parameters.
   // So, the plain parameters are set ("input"), and NEVER the CNode pointer ("p_input");
   // In other words, we are now simply setting the parameters of the compound
   // displayed as dotted lines, so getting the input from the compound ppg.

   // #1306: all the parameters must be set into the appropriate param box params[sg->tid], else
   // multiple threads would write into the same structure, if unique instead of per-thread

   // Ray_Switch
   iData->Screen_Switch.params[sg->tid].switcher = params.use_screen_compositing_for_colors;
   // Diffuse_Switch 
   iData->Diffuse_Switch.params[sg->tid].switcher = params.sample_sss_only_in_gi_rays;
   // Glossy_Switch
   iData->Glossy_Switch.params[sg->tid].switcher = params.sample_sss_only_in_glossy_rays;
   // Standard
   iData->Standard.params[sg->tid].Ksss = params.sss_weight;
   iData->Radius_To_Color.params[sg->tid].input = (params.shallow_scatter_radius * params.shallow_scatter_weight +
                                              params.mid_scatter_radius * params.mid_scatter_weight +
                                              params.deep_scatter_radius * params.deep_scatter_weight) /
                                              (params.shallow_scatter_weight + params.mid_scatter_weight + params.deep_scatter_weight) *
                                              params.global_sss_radius_multiplier;

   // Standard_Secondary_Specular
   iData->Standard_Secondary_Specular.params[sg->tid].Ks_color = params.secondary_reflection_color;
   iData->Standard_Secondary_Specular.params[sg->tid].Ks = params.secondary_reflection_weight;
   // Spec roughness added in the latest compound version
   iData->Standard_Secondary_Specular.params[sg->tid].specular_roughness = params.secondary_reflection_roughness;

   iData->Standard_Secondary_Specular.params[sg->tid].direct_specular = params.secondary_reflection_specular_weight;
   iData->Standard_Secondary_Specular.params[sg->tid].indirect_specular = params.secondary_reflection_reflection_weight;
   iData->Standard_Secondary_Specular.params[sg->tid].spec_Fresnel = params.secondary_reflection_enable_fresnel_falloff;
   iData->Standard_Secondary_Specular.params[sg->tid].Ksn = params.secondary_reflection_fresnel_coefficient;
   // Standard_SSS_Shallow
   iData->Standard_SSS_Shallow.params[sg->tid].Ksss_color = params.shallow_scatter_color;
   // Standard_SSS_Mid
   iData->Standard_SSS_Mid.params[sg->tid].Ksss_color = params.mid_scatter_color;
   // Standard_SSS_Deep
   iData->Standard_SSS_Deep.params[sg->tid].Ksss_color = params.deep_scatter_color;
   // Standard_SSS_Shallow
   iData->Standard_Diffuse_Primary_Specular.params[sg->tid].Kd_color = params.diffuse_color;
   iData->Standard_Diffuse_Primary_Specular.params[sg->tid].Kd = params.diffuse_weight;
   iData->Standard_Diffuse_Primary_Specular.params[sg->tid].Ks_color = params.primary_reflection_color;
   iData->Standard_Diffuse_Primary_Specular.params[sg->tid].Ks = params.primary_reflection_weight;
   // Spec roughness added in the latest compound version
   iData->Standard_Diffuse_Primary_Specular.params[sg->tid].specular_roughness = params.primary_reflection_roughness;
   
   iData->Standard_Diffuse_Primary_Specular.params[sg->tid].direct_specular = params.primary_reflection_specular_weight;
   iData->Standard_Diffuse_Primary_Specular.params[sg->tid].indirect_specular = params.primary_reflection_reflection_weight;
   iData->Standard_Diffuse_Primary_Specular.params[sg->tid].spec_Fresnel = params.primary_reflection_enable_fresnel_falloff;
   iData->Standard_Diffuse_Primary_Specular.params[sg->tid].Ksn = params.primary_reflection_fresnel_coefficient;
   // Colors_Add
   iData->Colors_Add.params[sg->tid].baseColor = params.diffuse_color * params.diffuse_weight;
   iData->Colors_Add.params[sg->tid].color1 = params.shallow_scatter_color * params.shallow_scatter_weight;
   iData->Colors_Add.params[sg->tid].color2 = params.mid_scatter_color * params.mid_scatter_weight;
   iData->Colors_Add.params[sg->tid].color3 = params.deep_scatter_color * params.deep_scatter_weight;
   // Multiply_ShallowSSS
   iData->Multiply_ShallowSSS.params[sg->tid].input1 = params.shallow_scatter_weight;
   iData->Multiply_ShallowSSS.params[sg->tid].input2 = params.sss_weight;
   // Multiply_MidSSS
   iData->Multiply_MidSSS.params[sg->tid].input1 = params.mid_scatter_weight;
   iData->Multiply_MidSSS.params[sg->tid].input2 = params.sss_weight;
   // Multiply_DeepSSS
   iData->Multiply_DeepSSS.params[sg->tid].input1 = params.deep_scatter_weight;
   iData->Multiply_DeepSSS.params[sg->tid].input2 = params.sss_weight;
   // Multiply_Shallow_Radius
   iData->Multiply_Shallow_Radius.params[sg->tid].input1 = params.shallow_scatter_radius;
   iData->Multiply_Shallow_Radius.params[sg->tid].input2 = params.global_sss_radius_multiplier;
   // Multiply_Mid_Radius
   iData->Multiply_Mid_Radius.params[sg->tid].input1 = params.mid_scatter_radius;
   iData->Multiply_Mid_Radius.params[sg->tid].input2 = params.global_sss_radius_multiplier;
   // Multiply_Deep_Radius
   iData->Multiply_Deep_Radius.params[sg->tid].input1 = params.deep_scatter_radius;
   iData->Multiply_Deep_Radius.params[sg->tid].input2 = params.global_sss_radius_multiplier;

   /*
   // Test, just to check that a single CStandard node works
   CStandard Standard_Diffuse_Primary_Specular();
   Standard_Diffuse_Primary_Specular.params[sg->tid].Kd_color = params.diffuse_color;
   Standard_Diffuse_Primary_Specular.params[sg->tid].Kd = params.diffuse_weight;
   Standard_Diffuse_Primary_Specular.params[sg->tid].Ks_color = params.primary_reflection_color;
   Standard_Diffuse_Primary_Specular.params[sg->tid].Ks = params.primary_reflection_weight;
   Standard_Diffuse_Primary_Specular.params[sg->tid].direct_specular = params.primary_reflection_specular_weight;
   Standard_Diffuse_Primary_Specular.params[sg->tid].indirect_specular = params.primary_reflection_reflection_weight;
   Standard_Diffuse_Primary_Specular.params[sg->tid].Ksn = params.primary_reflection_fresnel_coefficient;

   sg->out.RGB = *(AtColor*)Standard_Diffuse_Primary_Specular.Evaluate(node, sg, options);
   */

   // The output is simply the Evaluate result of the right-most node of the compound (Ray_Switch)
   sg->out.RGB = *(AtColor*)iData->Ray_Switch.Evaluate(node, sg, options);
}

