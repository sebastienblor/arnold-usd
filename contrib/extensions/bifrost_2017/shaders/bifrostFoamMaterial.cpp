//
// Simple hair shader, roughly based on Kay/Kajiya's shading model
//

#include <ai.h>

#include <cstring>
#include <string>
#include <iostream>

#include "BifrostUtils.h"
AI_SHADER_NODE_EXPORT_METHODS(BifrostFoamMaterialMtd);

enum BifrostFoamParams
{
   p_diffuse_color,
   p_diffuse_color_remap_input_min,
   p_diffuse_color_remap_input_max,
   p_diffuse_color_remap_channel,
   p_diffuse_color_remap_position,
   p_diffuse_color_remap_color,
   p_diffuse_color_remap_interp,
   p_diffuse_color_value_remap_position,
   p_diffuse_color_value_remap_float_value,
   p_diffuse_color_value_remap_interp,

   p_scatter_use_diffuse_color,
   p_scatter_color,
   p_scatter_weight,
   p_scatter_weight_remap_input_min,
   p_scatter_weight_remap_input_max,
   p_scatter_weight_remap_channel,
   p_scatter_weight_remap_position,
   p_scatter_weight_remap_float_value,
   p_scatter_weight_remap_interp,

   p_reflection_use_diffuse_color,
   p_reflection_color,
   p_reflection_weight,
   p_reflection_weight_remap_input_min,
   p_reflection_weight_remap_input_max,
   p_reflection_weight_remap_channel,
   p_reflection_weight_remap_position,
   p_reflection_weight_remap_float_value,
   p_reflection_weight_remap_interp,

   p_transparency_color,
   p_transparency_weight,
   p_transparency_weight_remap_input_min,
   p_transparency_weight_remap_input_max,
   p_transparency_weight_remap_channel,
   p_transparency_weight_remap_position,
   p_transparency_weight_remap_float_value,
   p_transparency_weight_remap_interp
};

node_parameters
{
   AiParameterRGB(       "diffuseColor", 1.0f, 1.0f, 1.0f);
   AiParameterFlt(       "diffuseColorRemapInputMin", 0.0f);
   AiParameterFlt(       "diffuseColorRemapInputMax", 20.0f);
   AiParameterStr(       "diffuseColorRemapChannel", "none");
   AiParameterArray(     "diffuseColorRemap_Position", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray(     "diffuseColorRemap_Color", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   AiParameterArray(     "diffuseColorRemap_Interp", AiArrayAllocate(0, 1, AI_TYPE_INT));
   AiParameterArray(     "diffuseColorValueRemap_Position", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray(     "diffuseColorValueRemap_FloatValue", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray(     "diffuseColorValueRemap_Interp", AiArrayAllocate(0, 1, AI_TYPE_INT));


   AiParameterBool(      "scatterUseDiffuseColor", true);
   AiParameterRGB(       "scatterColor", 1.0f, 1.0f, 1.0f);
   AiParameterFlt(       "scatterWeight", 1.0f);
   AiParameterFlt(       "scatterWeightRemapInputMin", 0.0f);
   AiParameterFlt(       "scatterWeightRemapInputMax", 20.0f);
   AiParameterStr(       "scatterWeightRemapChannel", "none");
   AiParameterArray(     "scatterWeightRemap_Position", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray(     "scatterWeightRemap_FloatValue", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray(     "scatterWeightRemap_Interp", AiArrayAllocate(0, 1, AI_TYPE_INT));
   

   AiParameterBool(      "reflectionUseDiffuseColor", true);
   AiParameterRGB(       "reflectionColor", 1.0f, 1.0f, 1.0f);
   AiParameterFlt(       "reflectionWeight", 1.0f);
   AiParameterFlt(       "reflectionWeightRemapInputMin", 0.0f);
   AiParameterFlt(       "reflectionWeightRemapInputMax", 20.0f);
   AiParameterStr(       "reflectionWeightRemapChannel", "none");
   AiParameterArray(     "reflectionWeightRemap_Position", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray(     "reflectionWeightRemap_FloatValue", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray(     "reflectionWeightRemap_Interp", AiArrayAllocate(0, 1, AI_TYPE_INT));
   

   AiParameterRGB(       "transparencyColor", 1.0f, 1.0f, 1.0f);
   AiParameterFlt(       "transparencyWeight", 1.0f);
   AiParameterFlt(       "transparencyWeightRemapInputMin", 0.0f);
   AiParameterFlt(       "transparencyWeightRemapInputMax", 20.0f);
   AiParameterStr(       "transparencyWeightRemapChannel", "none");
   AiParameterArray(     "transparencyWeightRemap_Position", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray(     "transparencyWeightRemap_FloatValue", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray(     "transparencyWeightRemap_Interp", AiArrayAllocate(0, 1, AI_TYPE_INT));


   AiMetaDataSetStr(nentry, NULL, "maya.name", "bifrostFoamMaterial");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81424a18);   
}



typedef struct
{
   bool opaque;
   bool has_diffuse_ramp;
   bool has_reflection_ramp;
   bool has_scatter_ramp;
   bool has_transparency_ramp;

   bool has_diffuse;
   bool has_reflection;
   bool has_scatter;
   AtCritSec lock;

  

   GradientDescription<AtRGB> *diffuseGradient;
   GradientDescription<float> *reflectionGradient;
   GradientDescription<float> *scatterGradient;
   GradientDescription<float> *transparencyGradient;

}
ShaderData;

static void initializeDiffuseGradient(AtNode *node, ShaderData *data, AtShaderGlobals *sg)
{
   AiCritSecEnter(&data->lock);
   if (data->diffuseGradient == 0)
   {
      GradientDescription<AtRGB> *grad = new GradientDescription<AtRGB>();
      
       // get the user data type // SPECIFIC TO POINTS using user data
      std::string userData = AiNodeGetStr(node, "diffuseColorRemapChannel").c_str();
      int type = AI_TYPE_FLOAT;
      const AtUserParamEntry* pentry = AiUserGetParameterFunc(AtString(userData.c_str()), sg);
      if (pentry) type = AiUserParamGetType(pentry);

      grad->ReadValues(sg->Op, sg, userData.c_str(), 
            AiNodeGetArray(node, "diffuseColorRemap_Position"), AiNodeGetArray(node, "diffuseColorRemap_Color"), AiNodeGetArray(node, "diffuseColorRemap_Interp"),
            AiNodeGetFlt(node, "diffuseColorRemapInputMin"), AiNodeGetFlt(node, "diffuseColorRemapInputMax"), type);

      grad->AddScalarRamp(AiNodeGetArray(node, "diffuseColorValueRemap_Position"), AiNodeGetArray(node, "diffuseColorValueRemap_FloatValue"), AiNodeGetArray(node, "diffuseColorValueRemap_Interp"));
      data->diffuseGradient = grad;

   }

   AiCritSecLeave(&data->lock);
}

/*
static void initializeScatterGradient(AtNode *node, ShaderData *data, AtShaderGlobals *sg)
{
   AiCritSecEnter(&data->lock);
   if (data->scatterGradient == 0)
   {
      GradientDescription<float> * grad = new GradientDescription<float>();
      
      std::string userData = AiNodeGetStr(node, "scatterWeightRemapChannel");
      int type = AI_TYPE_FLOAT;
      const AtUserParamEntry* pentry = AiUserGetParameterFunc(userData.c_str(), sg);
      if (pentry) type = AiUserParamGetType(pentry);

      grad->ReadValues(sg->Op, sg, userData.c_str(), 
         AiNodeGetArray(node, "scatterWeightRemap_Position"), AiNodeGetArray(node, "scatterWeightRemap_FloatValue"), AiNodeGetArray(node, "scatterWeightRemap_Interp"),
         AiNodeGetFlt(node, "scatterWeightRemapInputMin"), AiNodeGetFlt(node, "scatterWeightRemapInputMax"), type);
      data->scatterGradient = grad;

   }
   AiCritSecLeave(&data->lock);
}*/

static void initializeReflectionGradient(AtNode *node, ShaderData *data, AtShaderGlobals *sg)
{
   AiCritSecEnter(&data->lock);
   if (data->reflectionGradient == 0)
   {
      GradientDescription<float> * grad = new GradientDescription<float>();
      std::string userData = AiNodeGetStr(node, "reflectionWeightRemapChannel").c_str();
      int type = AI_TYPE_FLOAT;
      const AtUserParamEntry* pentry = AiUserGetParameterFunc(AtString(userData.c_str()), sg);
      if (pentry) type = AiUserParamGetType(pentry);

      grad->ReadValues(sg->Op, sg, userData.c_str(), 
            AiNodeGetArray(node, "reflectionWeightRemap_Position"), AiNodeGetArray(node, "reflectionWeightRemap_FloatValue"), AiNodeGetArray(node, "reflectionWeightRemap_Interp"),
            AiNodeGetFlt(node, "reflectionWeightRemapInputMin"), AiNodeGetFlt(node, "reflectionWeightRemapInputMax"), type);
      data->reflectionGradient = grad;

   }
   AiCritSecLeave(&data->lock);
}
/*
static void initializeTransparencyGradient(AtNode *node, ShaderData *data, AtShaderGlobals *sg)
{
   AiCritSecEnter(&data->lock);
   if (data->transparencyGradient == 0)
   {
      GradientDescription<float> * grad = new GradientDescription<float>();
      std::string userData = AiNodeGetStr(node, "transparencyWeightRemapChannel");
      int type = AI_TYPE_FLOAT;
      const AtUserParamEntry* pentry = AiUserGetParameterFunc(AtString(userData.c_str()), sg);
      if (pentry) type = AiUserParamGetType(pentry);

      grad->ReadValues(sg->Op, sg, userData.c_str(), 
            AiNodeGetArray(node, "transparencyWeightRemap_Position"), AiNodeGetArray(node, "transparencyWeightRemap_FloatValue"), AiNodeGetArray(node, "transparencyWeightRemap_Interp"),
            AiNodeGetFlt(node, "transparencyWeightRemapInputMin"), AiNodeGetFlt(node, "transparencyWeightRemapInputMax"), type);
      data->transparencyGradient = grad;

   }
   AiCritSecLeave(&data->lock);
}
*/

node_initialize
{
   ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
   data->diffuseGradient = 0;
   data->reflectionGradient = data->transparencyGradient = data->scatterGradient = 0;
   AiCritSecInit(&data->lock);

   AiNodeSetLocalData(node, data);
}


node_update
{
   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
   AtShaderGlobals *sg = AiShaderGlobals ();

   if (data->diffuseGradient)
   {
      delete data->diffuseGradient;
      data->diffuseGradient = 0;
   }

   if (data->reflectionGradient)
   {
      delete data->reflectionGradient;
      data->reflectionGradient = 0;
   }
   if (data->scatterGradient)
   {
      delete data->scatterGradient;
      data->scatterGradient = 0;
   }
   if (data->transparencyGradient)
   {
      delete data->transparencyGradient;
      data->transparencyGradient = 0;
   }


   std::string diffuseColorRemapChannel = AiNodeGetStr(node, "diffuseColorRemapChannel").c_str();

   if (diffuseColorRemapChannel == "" || diffuseColorRemapChannel == "none")
   {
      data->has_diffuse_ramp = false;
      
      AtRGB col = AiNodeGetRGB(node, "diffuseColor");
      data->has_diffuse = ((col.r > AI_EPSILON ||
                           col.g > AI_EPSILON ||
                           col.b > AI_EPSILON));

   }
   else
   {
      data->has_diffuse_ramp = true;
      // no way to know if diffuse will be > 0
      data->has_diffuse = true;
   }

   
   data->has_scatter = true;

   std::string scatterWeightRemapChannel = AiNodeGetStr(node, "scatterWeightRemapChannel").c_str();
   if (scatterWeightRemapChannel == "" || scatterWeightRemapChannel == "none")
   {  
      data->has_scatter_ramp = false;
      if (AiNodeGetFlt(node, "scatterWeight") < AI_EPSILON) data->has_scatter = false;
   }
   else data->has_scatter_ramp = true;
   
   if (AiNodeGetBool(node, "scatterUseDiffuseColor"))
   {
      if (!data->has_diffuse) data->has_scatter = false;
   } else
   {
      AtRGB col =  AiNodeGetRGB(node, "scatterColor");
      if((col.r < AI_EPSILON &&
          col.g < AI_EPSILON &&
          col.b < AI_EPSILON))    data->has_scatter = false;
   }

   data->has_reflection = true;
   std::string reflectionWeightRemapChannel = AiNodeGetStr(node, "reflectionWeightRemapChannel").c_str();
   if (reflectionWeightRemapChannel == "" || reflectionWeightRemapChannel == "none")
   {  
      data->has_reflection_ramp = false;
      if (AiNodeGetFlt(node, "reflectionWeight") < AI_EPSILON) data->has_reflection = false;
   }
   else data->has_reflection_ramp = true;
        

   if (AiNodeGetBool(node, "reflectionUseDiffuseColor"))
   {
      if (!data->has_diffuse) data->has_reflection = false;
   } else
   {
      AtRGB col =  AiNodeGetRGB(node, "reflectionColor");
      if((col.r < AI_EPSILON &&
          col.g < AI_EPSILON &&
          col.b < AI_EPSILON))    data->has_reflection = false;
   }

   data->opaque = false;
   std::string transparencyWeightRemapChannel = AiNodeGetStr(node, "transparencyWeightRemapChannel").c_str();   
   if (transparencyWeightRemapChannel == "" || transparencyWeightRemapChannel == "none") 
   {
      data->has_transparency_ramp = false;
      if (AiNodeGetFlt(node, "transparencyWeight") < AI_EPSILON) data->opaque = true;
   }
   else data->has_transparency_ramp = true;
      
   AtRGB col =  AiNodeGetRGB(node, "transparencyColor");
   if((col.r < AI_EPSILON &&
      col.g < AI_EPSILON &&
      col.b < AI_EPSILON))    data->opaque = true;

   AiShaderGlobalsDestroy(sg);
}

node_finish
{
   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
    if (data->diffuseGradient)
   {
      delete data->diffuseGradient;
      data->diffuseGradient = 0;
   }

   if (data->reflectionGradient)
   {
      delete data->reflectionGradient;
      data->reflectionGradient = 0;
   }
   if (data->scatterGradient)
   {
      delete data->scatterGradient;
      data->scatterGradient = 0;
   }
   if (data->transparencyGradient)
   {
      delete data->transparencyGradient;
      data->transparencyGradient = 0;
   }
   AiFree(data);

}

shader_evaluate
{

   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
   sg->out.RGB() = AI_RGB_BLACK;

//   float w_transp = 0.f;
   float w_refl = 0.f;
   
   bool has_reflection = data->has_reflection;
   
   // first compute w_refl, since we need it for opacity
   if( has_reflection )
   {      
      if( data->has_reflection_ramp)
      {
         if (data->reflectionGradient == 0)
         {
            initializeReflectionGradient(node, data, sg);
         }
         w_refl = data->reflectionGradient->RemapResult(sg);
         
      } else w_refl = AiShaderEvalParamFlt(p_reflection_weight);

      w_refl *= AiSchlickFresnel(sg->N, sg->Rd, 0.2f);
      
      if (w_refl < AI_EPSILON) has_reflection = false;
   }
   
   /* FIXME Arnold5
   if (data->opaque) sg->out_opacity = AI_RGB_WHITE;
   else
   {
      AtRGB transparency_color =  AiShaderEvalParamRGB(p_transparency_color);
      w_transp = 1.f;
      if(data->has_transparency_ramp)
      {
         if (data->transparencyGradient == 0)
         {
            initializeTransparencyGradient(node, data, sg);
         }
         w_transp = data->transparencyGradient->RemapResult(sg);
      } else w_transp = AiShaderEvalParamFlt(p_transparency_weight);
      
      // reflection attenuates transparency
      w_transp *= (1.f - w_refl);
      
      transparency_color *= w_transp;

      sg->out_opacity.r = AiClamp( 1.f - transparency_color.r, 0.f, 1.f);
      sg->out_opacity.g = AiClamp( 1.f - transparency_color.g, 0.f, 1.f);
      sg->out_opacity.b = AiClamp( 1.f - transparency_color.b, 0.f, 1.f);
      AiShaderGlobalsApplyOpacity(sg, sg->out_opacity);
   }*/

   // shadow ray -> nothing else to compute
   if (sg->Rt & AI_RAY_SHADOW) return;

/*  FIXME Arnold5
   // full transparent -> useless to continue
   if ((sg->out_opacity.r < AI_EPSILON &&
      sg->out_opacity.g < AI_EPSILON &&
      sg->out_opacity.b < AI_EPSILON)) return;
*/

   bool has_diffuse = data->has_diffuse;
   AtRGB diffuse_color = AI_RGB_BLACK;
   if( has_diffuse )
   {
      if( data->has_diffuse_ramp)
      {

         if (data->diffuseGradient == 0) 
         {
            initializeDiffuseGradient(node, data, sg);
         }

         diffuse_color = data->diffuseGradient->RemapResult(sg);
         
      } else diffuse_color = AiShaderEvalParamRGB(p_diffuse_color);

      diffuse_color *= 1.f - w_refl;

      if ((diffuse_color.r < AI_EPSILON &&
            diffuse_color.g < AI_EPSILON &&
            diffuse_color.b < AI_EPSILON)) has_diffuse = false;
   }

   // now handle reflection_color
   AtRGB reflection_color =  AI_RGB_BLACK;
   if (has_reflection)
   {
      reflection_color = AiShaderEvalParamBool(p_reflection_use_diffuse_color) ? 
                     diffuse_color : AiShaderEvalParamRGB(p_reflection_color);

      reflection_color *= w_refl;

      if ((reflection_color.r < AI_EPSILON &&
         reflection_color.g < AI_EPSILON &&
         reflection_color.b < AI_EPSILON)) has_reflection = false;
   }
   
/*
   float sum = w_refl + w_diff + w_scatt;

   if(sum > 1.f)
   {
      float invSum = 1.f / sum;
      reflection_color *= invSum;
      diffuse_color *= invSum;
      //scatter_color *= invSum;
      
   }
*/
   // only transparency -> no shading to compute
   if (!has_reflection && !has_diffuse) return;


   // FIXME Arnold5
   //void *r_brdf_data = NULL;//(has_reflection) ? AiStretchedPhongMISCreateData(sg, 0.0001f) : NULL;
   //void *d_brdf_data = NULL;//(has_diffuse) ? AiOrenNayarMISCreateData(sg, 0.f) : NULL;

   AtRGB d_direct_result = AI_RGB_BLACK;
   AtRGB r_direct_result = AI_RGB_BLACK;

   AtRGB d_indirect_result = AI_RGB_BLACK;
   AtRGB r_indirect_result = AI_RGB_BLACK;
   

/*
    FIXME Arnold5

   AiLightsPrepare(sg);
   while(AiLightsGetSample(sg))
   {
      
      if (has_diffuse)
         d_direct_result += AiEvaluateLightSample(sg, d_brdf_data, AiOrenNayarMISSample, AiOrenNayarMISBRDF, AiOrenNayarMISPDF) * AiLightGetDiffuse(sg->Lp);

      if (has_reflection)
         r_direct_result += AiEvaluateLightSample(sg, r_brdf_data,AiStretchedPhongMISSample , AiStretchedPhongMISBRDF, AiStretchedPhongMISPDF) * AiLightGetSpecular(sg->Lp);
  
   }
*/
   
   if (has_reflection)
   {
      AtScrSample sample;
      AtVector reflect_dir = AiReflect (sg->Rd, sg->N);
      AtRay    ray = AiMakeRay(AI_RAY_SPECULAR_REFLECT, sg->P, &reflect_dir, AI_BIG, sg);

      // FIXME Arnold5 I've added white for the weight, is that right ?
      AiTrace(ray, AI_RGB_WHITE, sample);      
      r_indirect_result = sample.color;
   }

   // FIXME Arnold5
   //if (has_diffuse) d_indirect_result = AiOrenNayarIntegrate(&sg->Nf, sg, 0.f);
   

   sg->out.RGB() = diffuse_color * (d_indirect_result + d_direct_result);
   sg->out.RGB() += reflection_color * (r_indirect_result + r_direct_result);


}
