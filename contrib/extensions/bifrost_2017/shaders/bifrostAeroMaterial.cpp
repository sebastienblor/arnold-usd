//
// Simple hair shader, roughly based on Kay/Kajiya's shading model
//

#include <ai.h>

#include <cstring>
#include <string>
#include <iostream>

#include "BifrostUtils.h"
AI_SHADER_NODE_EXPORT_METHODS(BifrostAeroMaterialMtd);



enum BifrostAeroParams
{
   p_density_scale,
   p_density_threshold,
   p_emission_color,
   p_emission_intensity,
   p_emission_offset,
   p_absorption_color,
   p_absorption_intensity,
   p_absorption_offset,
   p_shadow_opacity_scale,
   p_colored_opacity,
   p_scattering_color,
   p_scattering_intensity,
   p_scattering_offset,
   p_scattering_density_cutoff,
   p_scattering_directionality
};

node_parameters
{
   AiParameterFlt("densityScale", 1.0f);
   AiParameterFlt("densityThreshold", 0.10f);
   AiParameterRGB("emissionColor", 1.0f, 1.0f, 1.0f);
   AiParameterFlt("emissionIntensity", 1.0f);
   AiParameterFlt("emissionOffset", 0.0f);

   AiParameterRGB("absorptionColor", 1.0f, 1.0f, 1.0f);
   AiParameterFlt("absorptionIntensity", 1.0f);
   AiParameterFlt("absorptionOffset", 0.0f);

   AiParameterFlt("shadowOpacityScale", 1.0f);
   AiParameterFlt("coloredOpacity", 0.0f);

   AiParameterRGB("scatteringColor", 1.0f, 1.0f, 1.0f);
   AiParameterFlt("scatteringIntensity", 1.0f);
   AiParameterFlt("scatteringOffset", 0.0f);

   AiParameterFlt("scatteringDensityCutoff", 1.0f);
   AiParameterFlt("scatteringDirectionality", 0.0f);
   AiParameterFlt("aiStepSize", 0.1f);
   AiParameterInt("aiMaxSteps", 1000);
   AiParameterBool("aiShadowing", true);

   AiParameterFlt("aiShadowingStepSize", 0.1f);
   AiParameterInt("aiShadowingMaxSteps", 1000);

   AiParameterStr("densityRemapChannel", "none");
   AiParameterFlt("densityRemapInputMin", 0.f);
   AiParameterFlt("densityRemapInputMax", 1000.f);
   AiParameterArray("densityRemap_position", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("densityRemap_floatValue", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("densityRemap_interp", AiArrayAllocate(0, 1, AI_TYPE_INT));

   AiParameterStr("emissionColorRemapChannel", "none");
   AiParameterFlt("emissionColorRemapInputMin", 0.f);
   AiParameterFlt("emissionColorRemapInputMax", 1000.f);
   AiParameterArray("emissionColorRemap_position", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("emissionColorRemap_color", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   AiParameterArray("emissionColorRemap_interp", AiArrayAllocate(0, 1, AI_TYPE_INT));

   AiMetaDataSetStr(nentry, NULL, "maya.name", "bifrostAeroMaterial");
}

typedef struct
{
   GradientDescription<float> *densityGradient;
   GradientDescription<AtRGB> *emissionGradient;
   bool has_density_ramp;
   bool has_emission_ramp;
   AtCritSec lock;
}
ShaderData;

static void initializeEmissionGradient(AtNode *node, ShaderData *data, AtShaderGlobals *sg)
{
   AiCritSecEnter(&data->lock);
   if (data->emissionGradient == 0)
   {
      GradientDescription<AtRGB> *grad = new GradientDescription<AtRGB>();
      grad->voxels = true;

      int type = AI_TYPE_FLOAT;
      std::string channel = AiNodeGetStr(node, "emissionColorRemapChannel").c_str();
      if (channel == "velocity") type = AI_TYPE_VECTOR;

      grad->ReadValues(sg->Op, sg, channel.c_str(), 
      AiNodeGetArray(node, "emissionColorRemap_position"), AiNodeGetArray(node, "emissionColorRemap_color"), AiNodeGetArray(node, "emissionColorRemap_interp"),
      AiNodeGetFlt(node, "emissionColorRemapInputMin"), AiNodeGetFlt(node, "emissionColorRemapInputMax"), type);

      data->emissionGradient = grad;
   }
   AiCritSecLeave(&data->lock);
}

static void initializeDensityGradient(AtNode *node, ShaderData *data, AtShaderGlobals *sg)
{
   AiCritSecEnter(&data->lock);
   if (data->densityGradient == 0)
   {
      GradientDescription<float> * grad = new GradientDescription<float>();
      grad->voxels = true;

      int type = AI_TYPE_FLOAT;
      std::string channel = AiNodeGetStr(node, "densityRemapChannel").c_str();
      if (channel == "velocity") type = AI_TYPE_VECTOR;

      grad->ReadValues(sg->Op, sg, channel.c_str(), 
      AiNodeGetArray(node, "densityRemap_position"), AiNodeGetArray(node, "densityRemap_floatValue"), AiNodeGetArray(node, "densityRemap_interp"),
      AiNodeGetFlt(node, "densityRemapInputMin"), AiNodeGetFlt(node, "densityRemapInputMax"), type);
      data->densityGradient = grad;

   }
   AiCritSecLeave(&data->lock);
}

node_initialize
{
   ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
   data->emissionGradient = 0;
   data->densityGradient = 0;
   AiCritSecInit(&data->lock);
   AiNodeSetLocalData(node, data);
}

node_update
{
   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
   if (data->densityGradient)
   {
      delete data->densityGradient;
      data->densityGradient = 0;
   }

   if (data->emissionGradient)
   {
      delete data->emissionGradient;
      data->emissionGradient = 0;
   }

   std::string emissionColorRemapChannel = AiNodeGetStr(node, "emissionColorRemapChannel").c_str();
   data->has_emission_ramp = (emissionColorRemapChannel != "" && emissionColorRemapChannel != "none");

   std::string densityRemapChannel = AiNodeGetStr(node, "densityRemapChannel").c_str();
   data->has_density_ramp = (densityRemapChannel != "" && densityRemapChannel != "none");
}

node_finish
{
   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
   if (data->densityGradient)
   {
      delete data->densityGradient;
      data->densityGradient = 0;
   }

   if (data->emissionGradient)
   {
      delete data->emissionGradient;
      data->emissionGradient = 0;
   }
   AiFree(data);
}

shader_evaluate
{

   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
   float density = 0.f;

   float scaled_density = 1.f;

   if( data->has_density_ramp)
   {
      if (data->densityGradient == 0)
      {
         initializeDensityGradient(node, data, sg);
      }
      density = data->densityGradient->RemapResult(sg);
   } else
   {
      AiVolumeSampleFlt(AtString("smoke"), 0, &density);
      scaled_density =  density * AiShaderEvalParamFlt(p_density_scale);
   }

   // We get very weird results by taking this density into account
   // why doesn't this happen in MR ?
   float threshold = 0.f; //AiShaderEvalParamFlt(p_density_threshold);

   if (density > threshold)
   {
      //get Scatter
      AtRGB scatter = AiShaderEvalParamRGB(p_scattering_color) *  AiShaderEvalParamFlt(p_scattering_intensity);
      // offset and density cutoff
      float directionality = AiShaderEvalParamFlt(p_scattering_directionality);
      // get Absorb
      AtRGB absorb = AiShaderEvalParamRGB(p_absorption_color) *  AiShaderEvalParamFlt(p_absorption_intensity);

      scatter *= scaled_density;
      absorb *= scaled_density;

      if (sg->Rt & AI_RAY_SHADOW) 
      {
         sg->out.CLOSURE() = AiClosureVolumeAbsorption(sg, absorb + scatter);
         return;
      }

      // Emission
      AtRGB emission = AI_RGB_BLACK;
      if( data->has_emission_ramp)
      {
         if (data->emissionGradient == 0)
         {
            initializeEmissionGradient(node, data, sg);
         }
         emission = data->emissionGradient->RemapResult(sg);

      } else
      {
         emission = AiShaderEvalParamRGB(p_emission_color);
      }
      emission *= AiShaderEvalParamFlt(p_emission_intensity);
      emission *= scaled_density;

      // create closure
      sg->out.CLOSURE() = AiClosureVolumeHenyeyGreenstein(sg, absorb, scatter, emission);
   }
}

