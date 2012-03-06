#include <ai.h>
#include <stdio.h>
#include <math.h>
#include "skin_sss.h"


AI_SHADER_NODE_EXPORT_METHODS(SkinSssMethods);

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
   // Initialize by options. iData hosts all the nodes in teh graph
   CInstanceData *iData = new CInstanceData(AiUniverseGetOptions());
   // Set iData as the shader's local data
   AiNodeSetLocalData(node, (void*)iData);

   // Build the graph, exactly as in the Andreas compound.
   // The connections are established through the p_* CNode pointer of the connectable parameters.
   // This must be done only once

   // Ray_Switch
   iData->Ray_Switch.params.p_camera = &iData->Screen_Switch;
   iData->Ray_Switch.params.p_reflection = &iData->Screen_Switch;
   iData->Ray_Switch.params.p_refraction = &iData->Screen_Switch;
   iData->Ray_Switch.params.p_diffuse = &iData->Diffuse_Switch;
   iData->Ray_Switch.params.p_glossy = &iData->Glossy_Switch;
   // Diffuse_Switch 
   iData->Diffuse_Switch.params.p_input1 = &iData->Screen_Switch;
   iData->Diffuse_Switch.params.p_input2 = &iData->Standard;
   // Glossy_Switch 
   iData->Glossy_Switch.params.p_input1 = &iData->Screen_Switch;
   iData->Glossy_Switch.params.p_input2 = &iData->Standard;
   // Screen_Switch
   iData->Screen_Switch.params.p_input1 = &iData->Standards_Add;
   iData->Screen_Switch.params.p_input2 = &iData->Screen;
   // Standard
   iData->Standard.params.p_Ksss_color = &iData->Divide_By_Four;
   iData->Standard.params.p_Ksss = &iData->Multiply_By_Global_SSS_Weight;
   iData->Standard.params.p_sss_radius = &iData->Radius_To_Color;

   // Radius_To_Color
   iData->Radius_To_Color.params.p_input = &iData->Multiply_By_Global_SSS_Radius;
   // Multiply_By_Global_SSS_Radius
   iData->Multiply_By_Global_SSS_Radius.params.p_input2 = &iData->Divide_By_Three;
   // Divide_By_Three
   iData->Divide_By_Three.params.p_input1 = &iData->Add_Deep_Radius;
   // Add_Deep_Radius
   iData->Add_Deep_Radius.params.p_input1 = &iData->Add_Shallow_Mid_Radius;

   // Multiply_By_Global_SSS_Weight
   iData->Multiply_By_Global_SSS_Weight.params.p_input1 = &iData->Add_Deep_Weight;

   // Divide_By_Four
   iData->Divide_By_Four.params.p_color1 = &iData->Colors_Add;
   // Add_Deep_Weight
   iData->Add_Deep_Weight.params.p_input1 = &iData->Add_Scatter_Weight;
   // Add_Scatter_Weight
   iData->Add_Scatter_Weight.params.p_input1 = &iData->Add_Diffuse_Shallow_Weights;
   // Standards_Add
   iData->Standards_Add.params.p_baseColor = &iData->Standard_Diffuse_Primary_Specular;
   iData->Standards_Add.params.p_color1 = &iData->Standard_Secondary_Specular;
   iData->Standards_Add.params.p_color2 = &iData->Standard_SSS_Shallow;
   iData->Standards_Add.params.p_color3 = &iData->Standard_SSS_Mid;
   iData->Standards_Add.params.p_color4 = &iData->Standard_SSS_Deep;
   // Screen
   iData->Screen.params.p_baseColor = &iData->Clip_Diffuse_Primary_Specular;
   iData->Screen.params.p_color1 = &iData->Clip_Secondary_Specular;
   iData->Screen.params.p_color2 = &iData->Clip_Shallow;
   iData->Screen.params.p_color3 = &iData->Clip_Mid;
   iData->Screen.params.p_color4 = &iData->Clip_Deep;
   // Clip_Diffuse_Primary_Specular
   iData->Clip_Diffuse_Primary_Specular.params.p_color = &iData->Standard_Diffuse_Primary_Specular;
   // Clip_Secondary_Specular
   iData->Clip_Secondary_Specular.params.p_color = &iData->Standard_Secondary_Specular;
   // Clip_Shallow
   iData->Clip_Shallow.params.p_color = &iData->Standard_SSS_Shallow;
   // Clip_Mid
   iData->Clip_Mid.params.p_color = &iData->Standard_SSS_Mid;
   // Clip_Deep
   iData->Clip_Deep.params.p_color = &iData->Standard_SSS_Deep;
   // Standard_SSS_Shallow
   iData->Standard_SSS_Shallow.params.p_Ksss = &iData->Multiply_ShallowSSS;
   iData->Standard_SSS_Shallow.params.p_sss_radius = &iData->Shallow_Radius_Scalar_To_Color;
   // Standard_SSS_Mid
   iData->Standard_SSS_Mid.params.p_Ksss = &iData->Multiply_MidSSS;
   iData->Standard_SSS_Mid.params.p_sss_radius = &iData->Mid_Radius_Scalar_To_Color;
   // Standard_SSS_Deep
   iData->Standard_SSS_Deep.params.p_Ksss = &iData->Multiply_DeepSSS;
   iData->Standard_SSS_Deep.params.p_sss_radius = &iData->Deep_Radius_Scalar_To_Color;
   // Shallow_Radius_Scalar_To_Color
   iData->Shallow_Radius_Scalar_To_Color.params.p_input = &iData->Multiply_Shallow_Radius;
   // Mid_Radius_Scalar_To_Color
   iData->Mid_Radius_Scalar_To_Color.params.p_input = &iData->Multiply_Mid_Radius;
   // Deep_Radius_Scalar_To_Color
   iData->Deep_Radius_Scalar_To_Color.params.p_input = &iData->Multiply_Deep_Radius;
}


node_update {}


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

   // Ray_Switch
   iData->Screen_Switch.params.switcher = params.use_screen_compositing_for_colors;
   // Diffuse_Switch 
   iData->Diffuse_Switch.params.switcher = params.sample_sss_only_in_glossy_rays;
   // Glossy_Switch
   iData->Glossy_Switch.params.switcher = params.sample_sss_only_in_glossy_rays;
   // Multiply_By_Global_SSS_Weight
   iData->Multiply_By_Global_SSS_Weight.params.input2 = params.sss_weight;
   // Add_Deep_Weight
   iData->Add_Deep_Weight.params.input2 = params.deep_scatter_weight;
   // Add_Scatter_Weight
   iData->Add_Scatter_Weight.params.input2 = params.mid_scatter_weight;

   // The radii section, changed with the latest compound version from https://trac.solidangle.com/sitoa/wiki/SkinShader
   // Multiply_By_Global_SSS_Radius
   iData->Multiply_By_Global_SSS_Radius.params.input1 = params.global_sss_radius_multiplier;
   // Divide_By_Three == multiply by 0.3333
   iData->Divide_By_Three.params.input2 = 0.3333f;
   // Add_Deep_Radius
   iData->Add_Deep_Radius.params.input2 = params.deep_scatter_radius;
   // Add_Shallow_Mid_Radius
   iData->Add_Shallow_Mid_Radius.params.input1 = params.shallow_scatter_radius;
   iData->Add_Shallow_Mid_Radius.params.input2 = params.mid_scatter_radius;

   // Add_Diffuse_Shallow_Weights
   iData->Add_Diffuse_Shallow_Weights.params.input1 = params.diffuse_weight;
   iData->Add_Diffuse_Shallow_Weights.params.input2 = params.shallow_scatter_weight;
   // Standard_Secondary_Specular
   iData->Standard_Secondary_Specular.params.Ks_color = params.secondary_reflection_color;
   iData->Standard_Secondary_Specular.params.Ks = params.secondary_reflection_weight;
   // Spec roughness added in the latest compound version
   iData->Standard_Secondary_Specular.params.specular_roughness = params.secondary_reflection_roughness;

   iData->Standard_Secondary_Specular.params.direct_specular = params.secondary_reflection_specular_weight;
   iData->Standard_Secondary_Specular.params.indirect_specular = params.secondary_reflection_reflection_weight;
   iData->Standard_Secondary_Specular.params.spec_Fresnel = params.secondary_reflection_enable_fresnel_falloff;
   iData->Standard_Secondary_Specular.params.Ksn = params.secondary_reflection_fresnel_coefficient;
   // Standard_SSS_Shallow
   iData->Standard_SSS_Shallow.params.Ksss_color = params.shallow_scatter_color;
   // Standard_SSS_Mid
   iData->Standard_SSS_Mid.params.Ksss_color = params.mid_scatter_color;
   // Standard_SSS_Deep
   iData->Standard_SSS_Deep.params.Ksss_color = params.deep_scatter_color;
   // Standard_SSS_Shallow
   iData->Standard_Diffuse_Primary_Specular.params.Kd_color = params.diffuse_color;
   iData->Standard_Diffuse_Primary_Specular.params.Kd = params.diffuse_weight;
   iData->Standard_Diffuse_Primary_Specular.params.Ks_color = params.primary_reflection_color;
   iData->Standard_Diffuse_Primary_Specular.params.Ks = params.primary_reflection_weight;
   // Spec roughness added in the latest compound version
   iData->Standard_Diffuse_Primary_Specular.params.specular_roughness = params.primary_reflection_roughness;
   
   iData->Standard_Diffuse_Primary_Specular.params.direct_specular = params.primary_reflection_specular_weight;
   iData->Standard_Diffuse_Primary_Specular.params.indirect_specular = params.primary_reflection_reflection_weight;
   iData->Standard_Diffuse_Primary_Specular.params.spec_Fresnel = params.primary_reflection_enable_fresnel_falloff;
   iData->Standard_Diffuse_Primary_Specular.params.Ksn = params.primary_reflection_fresnel_coefficient;
   // Colors_Add
   iData->Colors_Add.params.baseColor = params.diffuse_color;
   iData->Colors_Add.params.color1 = params.shallow_scatter_color;
   iData->Colors_Add.params.color2 = params.mid_scatter_color;
   iData->Colors_Add.params.color3 = params.deep_scatter_color;
   // Multiply_ShallowSSS
   iData->Multiply_ShallowSSS.params.input1 = params.shallow_scatter_weight;
   iData->Multiply_ShallowSSS.params.input2 = params.sss_weight;
   // Multiply_MidSSS
   iData->Multiply_MidSSS.params.input1 = params.mid_scatter_weight;
   iData->Multiply_MidSSS.params.input2 = params.sss_weight;
   // Multiply_DeepSSS
   iData->Multiply_DeepSSS.params.input1 = params.deep_scatter_weight;
   iData->Multiply_DeepSSS.params.input2 = params.sss_weight;
   // Multiply_Shallow_Radius
   iData->Multiply_Shallow_Radius.params.input1 = params.shallow_scatter_radius;
   iData->Multiply_Shallow_Radius.params.input2 = params.global_sss_radius_multiplier;
   // Multiply_Mid_Radius
   iData->Multiply_Mid_Radius.params.input1 = params.mid_scatter_radius;
   iData->Multiply_Mid_Radius.params.input2 = params.global_sss_radius_multiplier;
   // Multiply_Deep_Radius
   iData->Multiply_Deep_Radius.params.input1 = params.deep_scatter_radius;
   iData->Multiply_Deep_Radius.params.input2 = params.global_sss_radius_multiplier;

   /*
   // Test, just to check that a single CStandard node works
   CStandard Standard_Diffuse_Primary_Specular("Standard_Test");
   Standard_Diffuse_Primary_Specular.params.Kd_color = params.diffuse_color;
   Standard_Diffuse_Primary_Specular.params.Kd = params.diffuse_weight;
   Standard_Diffuse_Primary_Specular.params.Ks_color = params.primary_reflection_color;
   Standard_Diffuse_Primary_Specular.params.Ks = params.primary_reflection_weight;
   Standard_Diffuse_Primary_Specular.params.direct_specular = params.primary_reflection_specular_weight;
   Standard_Diffuse_Primary_Specular.params.indirect_specular = params.primary_reflection_reflection_weight;
   Standard_Diffuse_Primary_Specular.params.Ksn = params.primary_reflection_fresnel_coefficient;

   sg->out.RGB = *(AtColor*)Standard_Diffuse_Primary_Specular.Evaluate(node, sg, options);
   */

   // The output is simply the Evaluate result of the right-most node of the compound (Ray_Switch)
   sg->out.RGB = *(AtColor*)iData->Ray_Switch.Evaluate(node, sg, options);
}

