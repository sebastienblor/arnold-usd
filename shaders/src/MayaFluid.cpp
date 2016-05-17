#include <ai.h>

#include <memory.h>
#include <cmath>

#include <string>
#include <sstream>
#include <algorithm>
#include <map>

#include "MayaUtils.h"
#include "MayaFluidData.h"
#include "FluidUtils.h"

namespace MSTR
{
   static const AtString mtoa_fluid_data("mtoa_fluid_data");
}

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidMtd);

const char* textureTypeEnums[] = {"Perlin Noise", "Billow", "Volume Wave", "Wispy", "Space Time", "Mandelbrot", 0};

enum textureType{
   TT_PERLIN_NOISE = 0,
   TT_BILLOW,
   TT_VOLUME_WAVE,
   TT_WISPY,
   TT_SPACE_TIME,
   TT_MANDELBROT
};

const char* filterTypeEnums[] = {"Closest", "Linear", "Cubic", 0};

const char* billowFalloffEnums[] = {"Linear", "Smooth", "Fast", "Bubble", 0};

enum billowFalloff{
   BF_LINEAR = 0,
   BF_SMOOTH,
   BF_FAST,
   BF_BUBBLE
};

node_parameters
{
   AiParameterEnum("filter_type", FT_LINEAR, filterTypeEnums);
   
   AiParameterRGB("transparency", .1f, .1f, .1f);
   AiMetaDataSetBool(nentry, "transparency", "always_linear", true);
   AiParameterFlt("phase_func", 0.f);
   
   AiParameterBool("color_texture", false);
   AiParameterBool("incand_texture", false);
   AiParameterBool("opacity_texture", false);
   
   AiParameterEnum("texture_type", 0, textureTypeEnums);
   
   AiParameterFlt("color_tex_gain", 1.f);   
   AiParameterFlt("incand_tex_gain", 1.f);
   AiParameterFlt("opacity_tex_gain", 1.f);
   
   AiParameterFlt("threshold", 0.f);
   AiParameterFlt("amplitude", 1.f);
   AiParameterFlt("ratio", 0.707f);
   AiParameterFlt("frequency_ratio", 2.f);
   AiParameterInt("depth_max", 2);
   
   AiParameterBool("invert_texture", false);
   AiParameterBool("inflection", false);
   
   AiParameterFlt("texture_time", 0.f);
   AiParameterFlt("zoom_factor", 1.f);
   AiParameterFlt("frequency", 1.f);
   
   AiParameterFlt("texture_origin_x", 0.f);
   AiParameterFlt("texture_origin_y", 0.f);
   AiParameterFlt("texture_origin_z", 0.f);
   
   AiParameterVec("texture_scale", 1.f, 1.f, 1.f);
   AiParameterVec("texture_rotate", 0.f, 0.f, 0.f);
   
   AiParameterFlt("implode", 0.f);
   AiParameterVec("implode_center", 0.f, 0.f, 0.f);
   
   AiParameterFlt("billow_density", 1.f);
   AiParameterFlt("spottyness", 0.1f);
   AiParameterFlt("size_rand", 0.f);
   AiParameterFlt("randomness", 1.f);
   AiParameterEnum("billow_falloff", 2, billowFalloffEnums);
   
   AiParameterInt("num_waves", 5);
   
   AiParameterBool("texture_affect_color", false);
   AiParameterBool("texture_affect_incand", false);
   AiParameterBool("texture_affect_opacity", false);
   
   AiParameterNode("volume_texture", 0);
   
   AiParameterFlt("shadow_opacity", 0.5f);
   AiParameterBool("enable_deformation_blur", false);
   AiParameterFlt("motion_vector_scale", 1.f);


   InitializeFluidShaderAdditionalParameters(params);
   InitializeFluidShaderParameters(params);
   
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.swatch", false);
}

enum MayaFluidParams{
   p_filter_type,
   
   p_transparency,
   p_phase_func,
   
   p_color_texture,
   p_incand_texture,
   p_opacity_texture,
   
   p_texture_type,
   
   p_color_tex_gain,   
   p_incand_tex_gain,
   p_opacity_tex_gain,
   
   p_threshold,
   p_amplitude,
   p_ratio,
   p_frequency_ratio,
   p_depth_max,
   
   p_invert_texture,
   p_inflection,
   
   p_texture_time,
   p_zoom_factor,
   p_frequency,
   
   p_texture_origin_x,
   p_texture_origin_y,
   p_texture_origin_z,
   
   p_texture_scale,
   p_texture_rotate,
   
   p_implode,
   p_implode_center,
   
   p_billow_density,
   p_spottyness,
   p_size_rand,
   p_randomness,
   p_billow_falloff,
   
   p_num_waves,
   
   p_noise_affect_color,
   p_noise_affect_incand,
   p_noise_affect_opacity,
   
   p_volume_noise,
   
   p_shadow_opacity,
   p_enable_deformation_blur,
   p_motion_vector_scale,

   p_edge_dropoff  
};



struct MayaFluidData{
   GradientDescription<AtRGB> colorGradient;
   GradientDescription<AtRGB> incandescenceGradient;
   GradientDescription<float, false, false> opacityGradient;  
   
   AtRGB transparency; 
   AtVector velocityScale;
   
   AtNode* volumeTexture;
   CMayaFluidData<true>* fluidData;

   std::map<AtNode*, CMayaFluidData<true>*> fluidDataMap[AI_MAX_THREADS];
   std::pair<AtNode*, CMayaFluidData<true>*> fluidDataCache[AI_MAX_THREADS];
   
   float colorTexGain;
   float incandTexGain;
   float opacityTexGain;
   float fpsDivider;
   
   int filterType;   
   int textureType;
   int coordinateMethod;
   int dropoffShape;
   int numWaves;
   int depthMax;
   int billowFalloff;
   
   bool colorTexture;
   bool incandTexture;
   bool opacityTexture;
   bool textureNoise;   
   bool textureAffectColor;
   bool textureAffectIncand;
   bool textureAffectOpacity;
   bool textureDisabledInShadows;   
   bool inflection;
   bool invertTexture;
   bool enableDeformationBlur;

   MayaFluidData() : fluidData(0)
   { }
   
   ~MayaFluidData()
   {
   }
   
   static void* operator new(size_t size)
   {
      return AiMalloc(size);
   }
   
   static void operator delete(void* p)
   {
      AiFree(p);
   }
};

node_initialize
{
   AiNodeSetLocalData(node, new MayaFluidData());
}

node_update
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);

   for (int i = 0; i < AI_MAX_THREADS; ++i)
   {
      data->fluidDataCache[i].first = 0;
      data->fluidDataCache[i].second = 0;
      data->fluidDataMap[i].clear();
   }
   
   data->filterType = AiNodeGetInt(node, "filter_type");
   
   data->transparency = AiNodeGetRGB(node, "transparency");
   data->transparency.r = CLAMP((1.f - data->transparency.r) / data->transparency.r, 0.f, AI_BIG);
   data->transparency.g = CLAMP((1.f - data->transparency.g) / data->transparency.g, 0.f, AI_BIG);
   data->transparency.b = CLAMP((1.f - data->transparency.b) / data->transparency.b, 0.f, AI_BIG);

   data->dropoffShape = AiNodeGetInt(node, "dropoff_shape");
   if (data->fluidData != 0)
      delete data->fluidData;
   data->fluidData = new CMayaFluidData<true>(node);
   
   data->velocityScale = AiNodeGetVec(node, "velocity_scale");
   data->velocityScale *= AiNodeGetFlt(node, "motion_vector_scale");
   
   data->colorGradient.type = AiNodeGetInt(node, "color_gradient_type");   
   data->colorGradient.inputBias = AiNodeGetFlt(node, "color_gradient_input_bias");
   data->colorGradient.ReadValues(node, "color_gradient_values",
                                  AiNodeGetArray(node, "color_gradient_positions"),
                                  AiNodeGetArray(node, "color_gradient_interps"));
   data->incandescenceGradient.type = AiNodeGetInt(node, "incandescence_gradient_type");   
   data->incandescenceGradient.inputBias = AiNodeGetFlt(node, "incandescence_gradient_input_bias");
   data->incandescenceGradient.ReadValues(node, "incandescence_gradient_values",
                                          AiNodeGetArray(node, "incandescence_gradient_positions"),
                                          AiNodeGetArray(node, "incandescence_gradient_interps"));   
   data->opacityGradient.type = AiNodeGetInt(node, "opacity_gradient_type");
   data->opacityGradient.inputBias = AiNodeGetFlt(node, "opacity_gradient_input_bias");
   data->opacityGradient.ReadValues(node, "opacity_gradient_values",
                                    AiNodeGetArray(node, "opacity_gradient_positions"),
                                    AiNodeGetArray(node, "opacity_gradient_interps"));   
   data->colorTexture = AiNodeGetBool(node, "color_texture");
   data->incandTexture = AiNodeGetBool(node, "incand_texture");
   data->opacityTexture = AiNodeGetBool(node, "opacity_texture");
   data->colorTexGain = AiNodeGetFlt(node, "color_tex_gain");
   data->incandTexGain = AiNodeGetFlt(node, "incand_tex_gain");
   data->opacityTexGain = AiNodeGetFlt(node, "opacity_tex_gain");
   
   data->textureNoise = data->colorTexture || data->incandTexture || data->opacityTexture;
   
   data->textureType = AiNodeGetInt(node, "texture_type");
   
   data->volumeTexture = (AtNode*) AiNodeGetPtr(node, "volume_texture");
   
   data->textureAffectColor = AiNodeGetBool(node, "texture_affect_color");
   data->textureAffectIncand = AiNodeGetBool(node, "texture_affect_incand");
   data->textureAffectOpacity = AiNodeGetBool(node, "texture_affect_opacity");
   
   data->numWaves = AiNodeGetInt(node, "num_waves");
   data->depthMax = AiNodeGetInt(node, "depth_max");
   data->billowFalloff = AiNodeGetInt(node, "billow_falloff");
   data->inflection = AiNodeGetBool(node, "inflection");
   data->invertTexture = AiNodeGetBool(node, "invert_texture");
   
   if (!(data->textureAffectColor || data->textureAffectIncand || data->textureAffectOpacity))
      data->volumeTexture = 0;
   
   data->textureDisabledInShadows = false;
   if (data->volumeTexture)
   {
      if (!data->textureAffectOpacity)
         data->textureDisabledInShadows = true;
   }
   else if (data->textureNoise)
   {
      if (!data->opacityTexture)
         data->textureDisabledInShadows = true;
   }
   
   data->coordinateMethod = AiNodeGetInt(node, "coordinate_method");
   data->enableDeformationBlur = AiNodeGetBool(node, "enable_deformation_blur");
   data->fpsDivider = AI_BIG;

   AtNode* options = AiUniverseGetOptions();
   if (AiNodeLookUpUserParameter(options, "fps"))
   {
      data->fpsDivider = 1.0f / AiNodeGetFlt(options, "fps");
      if (!AiIsFinite(data->fpsDivider) || (data->fpsDivider < AI_EPSILON))
         data->fpsDivider = AI_BIG;
   }
}

node_finish
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   delete data;
}

shader_evaluate
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);

   CMayaFluidData<true>* fluidData = data->fluidData;

   if (sg->Op == data->fluidDataCache[sg->tid].first)
      fluidData = data->fluidDataCache[sg->tid].second;
   {
      std::map<AtNode*, CMayaFluidData<true>*>::iterator it = data->fluidDataMap[sg->tid].find(sg->Op);
      if (it == data->fluidDataMap[sg->tid].end())
      {
         AtNode* fluidDataContainer = 0;
         if (AiUDataGetNode(MSTR::mtoa_fluid_data, &fluidDataContainer) && (fluidDataContainer != 0))
            fluidData = (CMayaFluidData<true>*)AiNodeGetLocalData(fluidDataContainer);
         data->fluidDataMap[sg->tid].insert(std::pair<AtNode*, CMayaFluidData<true>*>(sg->Op, fluidData));         
      }
      else
         fluidData = it->second;
      data->fluidDataCache[sg->tid].first = sg->Op;
      data->fluidDataCache[sg->tid].second = fluidData;
   }
   
   AtVector lPt = fluidData->ConvertToLocalSpace(sg->Po);
   if (data->enableDeformationBlur)
   {
      const AtVector velocity = fluidData->readVelocity(lPt, data->filterType);
      lPt = lPt - sg->time * velocity * data->velocityScale * data->fpsDivider;
   }
   

   AtVector scaledDir;
   AiM4VectorByMatrixMult(&scaledDir, sg->Minv, &sg->Rd);

   float dropoff = CalculateDropoff(fluidData, lPt, data->dropoffShape, CLAMP(AiShaderEvalParamFlt(p_edge_dropoff), 0.0f, 1.0f), data->filterType)
                   * AiV3Length(scaledDir);

   if (data->textureDisabledInShadows && (sg->Rt & AI_RAY_SHADOW))
   {
      const float opacity = AiMax(0.f, GetValue(sg, fluidData, lPt, data->opacityGradient, data->filterType, 1.0f, data->velocityScale)) * dropoff * AiShaderEvalParamFlt(p_shadow_opacity);
      AiShaderGlobalsSetVolumeAttenuation(sg, data->transparency * opacity);
      return;
   }
   
   float opacityNoise = 1.0f;
   float colorNoise = 1.f; // colors?
   float incandNoise = 1.f;
   const AtVector old_dPdx = sg->dPdx;
   const AtVector old_dPdy = sg->dPdy;
   sg->dPdx = AI_V3_ZERO;
   sg->dPdy = AI_V3_ZERO;
   if (data->volumeTexture)
   {
      if ((data->coordinateMethod == CM_GRID) && !fluidData->coordinatesEmpty())
      {
         const AtVector oldP = sg->P;
         const AtVector oldPo = sg->Po;
         sg->P = fluidData->readCoordinates(lPt, data->filterType);
         sg->Po = sg->P;
         AiShaderEvaluate(data->volumeTexture, sg);
         sg->P = oldP;
         sg->Po = oldPo;
      }
      else
         AiShaderEvaluate(data->volumeTexture, sg);
      float volumeNoise = sg->out.FLT();
      if (data->textureAffectColor)
         colorNoise = volumeNoise;
      if (data->textureAffectIncand)
         incandNoise = volumeNoise;
      if (data->textureAffectOpacity)
         opacityNoise *= volumeNoise;
   }
   else if (data->textureNoise) // TODO optimize these evaluations based on raytype!
   {
      AtVector P;
      if ((data->coordinateMethod == CM_GRID) && !fluidData->coordinatesEmpty())
         P = fluidData->readCoordinates(lPt, data->filterType);
      else
         P = sg->Po;
      ApplyImplode(P, AiShaderEvalParamFlt(p_implode), AiShaderEvalParamVec(p_implode_center));     
      
      AtVector textureScale = AiShaderEvalParamVec(p_texture_scale);
      textureScale.x = AiMax(AI_EPSILON, textureScale.x);
      textureScale.y = AiMax(AI_EPSILON, textureScale.y);
      textureScale.z = AiMax(AI_EPSILON, textureScale.z);
      const float frequency = AiShaderEvalParamFlt(p_frequency);
      P.x *= frequency / textureScale.x;
      P.y *= frequency / textureScale.y;
      P.z *= frequency / textureScale.z;      
      P.x += AiShaderEvalParamFlt(p_texture_origin_x);
      P.y += AiShaderEvalParamFlt(p_texture_origin_y);
      P.z += AiShaderEvalParamFlt(p_texture_origin_z);
      const float amplitude = AiShaderEvalParamFlt(p_amplitude);
      float volumeNoise = 0.f;
      const float frequencyRatio = AiShaderEvalParamFlt(p_frequency_ratio);
      const float ratio = AiShaderEvalParamFlt(p_ratio);
      float textureTime = AiShaderEvalParamFlt(p_texture_time);
      
      int depth[2] = {0, data->depthMax};
      float ripples[3] = {1.0f, 1.0f, 1.0f};
      
      switch (data->textureType)
      {
         case TT_PERLIN_NOISE:
            if (data->inflection)
               volumeNoise = amplitude * fTurbulence(sg, P, textureTime, 1.0f, frequencyRatio, depth, ratio, ripples);
            else
               volumeNoise = fBm(sg, P, textureTime, amplitude, depth, 1.0f, frequencyRatio, ratio);
            break;
         case TT_BILLOW:
            {               
               const float billow_density = AiShaderEvalParamFlt(p_billow_density);
               const float radius = sqrt(.5f * billow_density);
               const float sizeRand = AiShaderEvalParamFlt(p_size_rand);
               const float randomness = AiShaderEvalParamFlt(p_randomness);
               const float spottyness = AiShaderEvalParamFlt(p_spottyness);
               volumeNoise = BillowNoise(P, textureTime, 3, radius, sizeRand, randomness, data->billowFalloff, spottyness, data->depthMax, frequencyRatio, ratio, amplitude);
            }
            break;
         case TT_VOLUME_WAVE:
            {
               float amp = amplitude;
               const float timeRatio = 1.0f / frequencyRatio;
               textureTime *= (float)AI_PITIMES2;

               for (int i=0; i < data->depthMax; ++i)
               {
                 float waveVal = 0.0f;

                  for (int j=0; j < data->numWaves; ++j)
                  {
                     float tmp = (float)AI_PITIMES2 * (0.5f * (1 + i) * (1 + j));

                     AtVector v, d;

                     v = AtVector(tmp, 0, 0);
                     d.x = AiPerlin3(v);

                     v = AtVector(0, tmp, 0);
                     d.y = AiPerlin3(v);

                     v = AtVector(0, 0, tmp);
                     d.z = AiPerlin3(v);

                     d = AiV3Normalize(d);

                     waveVal += cosf((float)AI_PITIMES2 * AiV3Dot(P, d) + textureTime);
                  }

                  waveVal /= (float)data->numWaves;

                  if (data->inflection)
                  {
                     waveVal = fabs(waveVal);
                  }

                  volumeNoise += amp * waveVal;

                  amp *= ratio;
                  P *= frequencyRatio;
                  textureTime *= timeRatio;
               }

               if (!data->inflection)
               {
                  volumeNoise = 0.5f * volumeNoise + 0.5f;
               }
            }
            break;
         case TT_WISPY:
            P += AiPerlin3(P / 2.0f) * 1.3f;
            if (data->inflection)
               volumeNoise = amplitude * fTurbulence(sg, P, textureTime, 1.0f, frequencyRatio, depth, ratio, ripples);
            else
               volumeNoise = fBm(sg, P, textureTime, amplitude, depth, 1.0f, frequencyRatio, ratio);
         break;
         case TT_SPACE_TIME:
            if (data->inflection)
               volumeNoise = amplitude * fTurbulence(sg, P, textureTime, 1.0f, frequencyRatio, depth, ratio, ripples);
            else
               volumeNoise = fBm(sg, P, textureTime, amplitude, depth, 1.0f, frequencyRatio, ratio);
            break;
         default:
            volumeNoise = 1.f;
            break;
      }
      if (volumeNoise > 1.f)
         volumeNoise = 1.f;
      else if (volumeNoise < 0.f)
         volumeNoise = 0.f;
      
      if (data->invertTexture)
         volumeNoise = AiMax(1.f - volumeNoise, 0.f);
      if (data->colorTexture)
         colorNoise = data->colorTexGain * volumeNoise;
      if (data->incandTexture)
         incandNoise = data->incandTexGain * volumeNoise;
      if (data->opacityTexture)
         opacityNoise *= data->opacityTexGain * volumeNoise;
   }
   sg->dPdx = old_dPdx;
   sg->dPdy = old_dPdy;
   
   if (sg->Rt & AI_RAY_SHADOW)
   {
      const float opacity = AiMax(0.f, GetValue(sg, fluidData, lPt, data->opacityGradient, data->filterType, opacityNoise, data->velocityScale)) * dropoff * AiShaderEvalParamFlt(p_shadow_opacity);
      AiShaderGlobalsSetVolumeAttenuation(sg, data->transparency * opacity);
      return;
   }
   
   const AtRGB opacity = AiMax(0.f, GetValue(sg, fluidData, lPt, data->opacityGradient, data->filterType, opacityNoise, data->velocityScale)) * dropoff * data->transparency;
   AtRGB color = AI_RGB_BLACK;
   if (fluidData->colorGridEmpty())
      color = GetValue(sg, fluidData, lPt, data->colorGradient, data->filterType, colorNoise, data->velocityScale);
   else
      color = fluidData->readColors(lPt, data->filterType);

   color.r = AiMax(0.f, color.r);
   color.g = AiMax(0.f, color.g);
   color.b = AiMax(0.f, color.b);
   AtRGB incandescence = GetValue(sg, fluidData, lPt, data->incandescenceGradient, data->filterType, incandNoise, data->velocityScale);
   incandescence.r = AiMax(0.f, incandescence.r);
   incandescence.g = AiMax(0.f, incandescence.g);
   incandescence.b = AiMax(0.f, incandescence.b);
   
   AiShaderGlobalsSetVolumeAttenuation(sg, opacity * AI_RGB_WHITE);
   AiShaderGlobalsSetVolumeEmission(sg, opacity * incandescence);
   AiShaderGlobalsSetVolumeScattering(sg, opacity * color, CLAMP(AiShaderEvalParamFlt(p_phase_func), -1.0f, 1.0f));
}
