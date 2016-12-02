#include <ai.h>

#include <cmath>
#include "MayaUtils.h"

namespace MSTR
{
   static const AtString geo_opacity("geo_opacity");
   static const AtString curve_id("curve_id");
}

AI_SHADER_NODE_EXPORT_METHODS(MayaHairMtd);

node_parameters
{
   AiParameterRGB("hairColor", .4f, .4f, .4f); // check the default values!
   AiParameterFlt("opacity", 1.f);
   AiParameterFlt("translucence", .5f);
   AiParameterRGB("specularColor", .35f, .35f, .299995f);
   AiParameterFlt("specularPower", 3.f);
   AiParameterBool("castShadows", true);
   
   AiParameterFlt("diffuseRand", .2f);
   AiParameterFlt("specularRand", .4f);
   AiParameterFlt("hueRand", .0f);
   AiParameterFlt("satRand", .0f);
   AiParameterFlt("valRand", .0f);
   AiParameterFlt("indirectDiffuse", 1.0f);

   AtArray* dummy = AiArray(2, 1, AI_TYPE_RGB, AI_RGB_WHITE, AI_RGB_WHITE);
   AiParameterArray("hairColorScale", dummy);
   

   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiMayaHair");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.swatch", false);
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x00115D1A);
}

enum MayaHairParams{
   p_hair_color = 0,
   p_opacity,
   p_translucence,
   p_specular_color,
   p_specular_power,
   p_cast_shadows,
   
   p_diffuse_rand,
   p_specular_rand,
   p_hue_rand,
   p_sat_rand,
   p_val_rand,
   p_indirect_diffuse,
   
   p_hair_color_scale,

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
   AtRGB opacity = AiShaderEvalParamFlt(p_opacity) * AI_RGB_WHITE;
   float geo_opacity;
   if (AiUDataGetFlt(MSTR::geo_opacity, geo_opacity))
      opacity *= geo_opacity;
   
   if (sg->Rt & AI_RAY_SHADOW)
   {
      sg->out_opacity = opacity;
      return;
   }   
   
   if (AiShaderGlobalsApplyOpacity(sg, opacity))
      return;
   
   AtRGB hairColor = AiShaderEvalParamRGB(p_hair_color);
   
   AtArray* hairColorScale = AiShaderEvalParamArray(p_hair_color_scale);
   if (AiArrayGetNumElements(hairColorScale) > 1)
   {
      const float id = (float)AiArrayGetNumElements(hairColorScale) * sg->v;
      const float idbf = floorf(id);
      const int idb = (int)idbf;
      if (idb < 0)
         hairColor *= AiArrayGetRGB(hairColorScale, 0);
      else if (idb >= (int)(AiArrayGetNumElements(hairColorScale) - 1))
         hairColor *= AiArrayGetRGB(hairColorScale, AiArrayGetNumElements(hairColorScale) - 1);
      else
      {
         const AtRGB b0 = AiArrayGetRGB(hairColorScale, idb);
         const AtRGB b1 = AiArrayGetRGB(hairColorScale, idb + 1);
         hairColor *= LERP(AI_RGB_WHITE * (id - idbf), b0, b1);
      }         
   }
   
   const float hueRand = AiShaderEvalParamFlt(p_hue_rand);
   const float satRand = AiShaderEvalParamFlt(p_sat_rand);
   const float valRand = AiShaderEvalParamFlt(p_val_rand);
   
   const bool enableHSVRand = (hueRand > AI_EPSILON) || 
      (satRand > AI_EPSILON) || 
      (valRand > AI_EPSILON);
   
   unsigned int seed = 0;
   if (enableHSVRand)
      AiUDataGetUInt(MSTR::curve_id, seed); // the translator exports curve_ids   
   // when needed, so no need for an extra check   
   
   const float diffuseRand = AiShaderEvalParamFlt(p_diffuse_rand);
   if ((diffuseRand > AI_EPSILON) && enableHSVRand)
   {
      // do not change the original seed
      // because it needs to stay the same
      // for both specular and diffuse colors
      unsigned int tseed = seed * 5000 + 142; 
      AtVector randHSV;
      randHSV.x = rand01(&tseed) * 360.f;
      randHSV.y = rand01(&tseed);
      randHSV.z = rand01(&tseed);      
      AtVector origHSV = RGBtoHSV(hairColor);
      origHSV.x = LERP(hueRand * diffuseRand, origHSV.x, randHSV.x);
      origHSV.y = LERP(satRand * diffuseRand, origHSV.y, randHSV.y);
      origHSV.z = LERP(valRand * diffuseRand, origHSV.z, randHSV.z);
      hairColor = HSVtoRGB(origHSV);
   }
   
   sg->fhemi = false;
   
   AtRGB diffuse(0.f, 0.f, 0.f);
   
   const AtVector T = AiV3Normalize(sg->dPdv);
   const AtVector V = -sg->Rd;
   
   if (!AiColorIsSmall(hairColor)) // diffuse and translucence calculation
   {
      const float indirectDiffuse = AiShaderEvalParamFlt(p_indirect_diffuse);
      
      AtLightSample light_sample;
      AiLightsPrepare(sg);
      while (AiLightsGetSample(sg, light_sample))
      {
         float light_diffuse = AiLightGetDiffuse(light_sample.Lp);
         if (light_diffuse > 0)
         {
            const float TdotL = AiV3Dot(T, sg->light_filter->Ld);
            float d = 1.f - TdotL * TdotL;
            d = d > 0.f ? sqrtf(d) : 0.f;
            AtRGB Li_over_pdf = light_sample.Li / light_sample.pdf;
            diffuse += Li_over_pdf * d;
         }
      }
      if (indirectDiffuse > 0.f) diffuse += AiIndirectDiffuse(V, sg, AI_RGB_WHITE) * indirectDiffuse;

      diffuse *= hairColor;
   }
   
   AtRGB specularColor = AiShaderEvalParamRGB(p_specular_color);
   
   AtRGB specular(0.f, 0.f, 0.f);
   
   const float specularRand = AiShaderEvalParamFlt(p_specular_rand);
   if ((specularRand > AI_EPSILON) && enableHSVRand)
   {
      // do not change the original seed
      // because it needs to stay the same
      // for both specular and diffuse colors
      unsigned int tseed = seed * 2500 + 842; 
      AtVector randHSV;
      randHSV.x = rand01(&tseed) * 360.f;
      randHSV.y = rand01(&tseed);
      randHSV.z = rand01(&tseed);
      AtVector origHSV = RGBtoHSV(specularColor);
      origHSV.x = LERP(hueRand * specularRand, origHSV.x, randHSV.x);
      origHSV.y = LERP(satRand * specularRand, origHSV.y, randHSV.y);
      origHSV.z = LERP(valRand * specularRand, origHSV.z, randHSV.z);
      specularColor = HSVtoRGB(origHSV);
   }   
   
   if (!AiColorIsSmall(specularColor) && !(sg->Rt & AI_RAY_DIFFUSE_REFLECT)) // specular contribution
   {
      const float specularPower = AiShaderEvalParamFlt(p_specular_power);
      AtLightSample light_sample;
      AiLightsPrepare(sg);      
      while (AiLightsGetSample(sg, light_sample))
      {
         float light_specular = AiLightGetSpecular(light_sample.Lp);
         if (light_specular > 0)
         {
            const AtVector H = AiV3Normalize(sg->light_filter->Ld + V);
            const float HdotT = AiV3Dot(H, T);
            float s = 1 - HdotT * HdotT;
            s = powf(s, specularPower);
            if (s > 0)
            {
               AtRGB Li_over_pdf = light_sample.Li / light_sample.pdf;
               specular += Li_over_pdf * s;
            }
         }
      }
      specular *= specularColor;
   }
   
   sg->out.RGB() = diffuse + specular;
   sg->out.RGBA().a = 1.0f;
   sg->out_opacity = opacity;
}
