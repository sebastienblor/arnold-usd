#include <ai.h>

#include <memory.h>
#include <cmath>

#include "RandomNoise.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidMtd);

enum GradientType{
   GT_CONSTANT = 0,
   GT_X_GRADIENT,
   GT_Y_GRADIENT,
   GT_Z_GRADIENT,
   GT_CENTER_GRADIENT,
   GT_DENSITY,
   GT_TEMPERATURE,
   GT_FUEL,
   GT_PRESSURE,
   GT_SPEED,
   GT_DENSITY_AND_FUEL
};

const char* textureTypeEnums[] = {"Perlin Noise", "Billow", "Volume Wave", "Wispy", "Space Time", "Mandelbrot", 0};

enum textureType{
   TT_PERLIN_NOISE = 0,
   TT_BILLOW,
   TT_VOLUME_WAVE,
   TT_WISPY,
   TT_SPACE_TIME,
   TT_MANDELBROT
};

const char* coordinateMethodEnums[] = {"Fixed", "Grid"};

enum coordinateMethod{
   CM_FIXED,
   CM_GRID
};

node_parameters
{
   AiParameterRGB("color", 1.f, 1.f, 1.f);
   
   AiParameterRGB("transparency", .1f, .1f, .1f);
   AiMetaDataSetBool(mds, "transparency", "always_linear", true);
   AiParameterFlt("phase_func", 0.f);
   
   AiParameterInt("xres", 0);
   AiParameterInt("yres", 0);
   AiParameterInt("zres", 0);
   
   AiParameterVec("min", 0.f, 0.f, 0.f);
   AiParameterVec("max", 0.f, 0.f, 0.f);
   
   AiParameterArray("density", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("fuel", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("temperature", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("pressure", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   
   AiParameterArray("velocity", AiArrayAllocate(0, 1, AI_TYPE_VECTOR));
   
   AiParameterArray("colors", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   
   AiParameterArray("coordinates", AiArrayAllocate(0, 1, AI_TYPE_VECTOR));
   
   static const char* gradientTypes[] = {"Constant", "X Gradient", "Y Gradient", "Z Gradient",
      "Center Gradient", "Density", "Temperature", "Fuel", "Pressure", "Speed",
      "Density And Fuel", 0};
   
   AiParameterEnum("color_gradient_type", GT_CONSTANT, gradientTypes);
   AiParameterArray("color_gradient", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   AiParameterFlt("color_gradient_input_bias", 0.0f);
   
   AiParameterEnum("incandescence_gradient_type", GT_TEMPERATURE, gradientTypes);
   AiParameterArray("incandescence_gradient", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   AiParameterFlt("incandescence_gradient_input_bias", 0.0f);
   
   AiParameterEnum("opacity_gradient_type", GT_DENSITY, gradientTypes);
   AiParameterArray("opacity_gradient", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterFlt("opacity_gradient_input_bias", 0.0f);   
   
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
   
   AiParameterBool("texture_affect_color", false);
   AiParameterBool("texture_affect_incand", false);
   AiParameterBool("texture_affect_opacity", false);
   
   AiParameterNode("volume_texture", 0);
   
   AiParameterEnum("coordinate_method", 0, coordinateMethodEnums);
   
   AiParameterArray("matrix", AiArrayAllocate(0, 1, AI_TYPE_MATRIX));
   
   AiParameterFlt("shadow_opacity", 0.5f);
   
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiMayaFluid");
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D1E);
}

enum MayaFluidParams{
   p_color=0,
   
   p_transparency,
   p_phase_func,
   
   p_xres,
   p_yres,
   p_zres,

   p_min,
   p_max,
   
   p_density,
   p_fuel,
   p_temperature,
   p_pressure,
   p_velocity,
   p_colors,
   p_coordinates,
   
   p_color_gradient_type,
   p_color_gradient,
   p_color_gradient_input_bias,
   
   p_incandescence_gradient_type,
   p_incandescence_gradient,
   p_incandescence_gradient_input_bias,
   
   p_opacity_gradient_type,
   p_opacity_gradient,
   p_opacity_gradient_input_bias,
   
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
   
   p_noise_affect_color,
   p_noise_affect_incand,
   p_noise_affect_opacity,
   
   p_volume_noise,
   p_coordinate_method,
   
   p_matrix,
   
   p_shadow_opacity,
};

template<typename T>
struct GradientDescription{
   T* data;
   float inputBias;
   int type;
   int resolution;
   
   GradientDescription() : data(0) {}
   
   void release() {if(data) AiFree(data);}
};

template<typename T>
struct ArrayDescription{
   T* data;
   bool single;
   
   ArrayDescription() : data(0) {}
   
   void release() {if(data) AiFree(data);}   
};

struct MayaFluidData{
   ArrayDescription<float> density;
   ArrayDescription<float> fuel;
   ArrayDescription<float> temperature;
   ArrayDescription<float> pressure;
   ArrayDescription<AtVector> velocity;
   ArrayDescription<AtRGB> colors;
   ArrayDescription<AtVector> coordinates;
   
   GradientDescription<AtRGB> colorGradient;
   GradientDescription<AtRGB> incandescenceGradient;
   GradientDescription<float> opacityGradient;  
   
   AtRGB transparency; 
   float phaseFunc;
   
   int xres, yres, zres;
   AtVector dmin, dmax;
   
   bool colorTexture;
   bool incandTexture;
   bool opacityTexture;
   bool textureNoise;
   
   int textureType;
   
   bool textureAffectColor;
   bool textureAffectIncand;
   bool textureAffectOpacity;
   
   AtNode* volumeTexture;
   
   bool textureDisabledInShadows;
   int coordinateMethod;
   
   ~MayaFluidData()
   {
      density.release();
      fuel.release();
      temperature.release();
      pressure.release();
      velocity.release();
      colors.release();
      coordinates.release();
      incandescenceGradient.release();
      opacityGradient.release();
   }
   
   static void* operator new(size_t size)
   {
      return AiMalloc((unsigned long)size);
   }
   
   static void operator delete(void* p)
   {
      AiFree(p);
   }
};

node_initialize
{
   MayaFluidData* data = new MayaFluidData();
   AiNodeSetLocalData(node, data);
}

template <typename T>
T GetDefaultValue()
{
   return 0;
}

template <>
float GetDefaultValue<float>()
{
   return 0.f;
}

template <>
AtRGB GetDefaultValue<AtRGB>()
{
   return AI_RGB_BLACK;
}

template <>
AtVector GetDefaultValue<AtVector>()
{
   return AI_V3_ZERO;
}

template <typename T>
T ReadFromArray(AtArray* array, int element)
{
   return GetDefaultValue<T>();
}

template <>
float ReadFromArray(AtArray* array, int element)
{
   return AiArrayGetFlt(array, element);
}

template <>
AtRGB ReadFromArray(AtArray* array, int element)
{
   return AiArrayGetRGB(array, element);
}

template <>
AtVector ReadFromArray(AtArray* array, int element)
{
   return AiArrayGetVec(array, element);
}

template <typename T>
void ReadGradient(AtNode* node, const char* name, GradientDescription<T>& gradient)
{
   AtArray* array = AiNodeGetArray(node, name);
   
   gradient.release();
    
   if (array->nelements)
   {
      gradient.resolution = (int)array->nelements;
      gradient.data = (T*)AiMalloc(sizeof(T) * gradient.resolution);
      for (int i = 0; i < gradient.resolution; ++i)
         gradient.data[i] = ReadFromArray<T>(array, i);
   }
   else
   {
      gradient.resolution = 0;
      gradient.data = 0;
   }
}

template <typename T>
void ReadArray(AtNode* node, const char* name, int numVoxels, ArrayDescription<T>& arrayDesc)
{
   AtArray* array = AiNodeGetArray(node, name);
   
   arrayDesc.release();
   
   if ((int)array->nelements == numVoxels)
   {
      arrayDesc.single = false;
      arrayDesc.data = (T*)AiMalloc(sizeof(T) * numVoxels);
      for (int i = 0; i < numVoxels; ++i)
         arrayDesc.data[i] = ReadFromArray<T>(array, i);
   }
   else if (array->nelements == 1) // only one value
   {
      arrayDesc.single = true;
      arrayDesc.data = (T*)AiMalloc(sizeof(T));
      *arrayDesc.data = ReadFromArray<T>(array, 0);
   }
   else
   {
      arrayDesc.single = false;
      arrayDesc.data = 0;
   }
}

node_update
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   data->xres = AiNodeGetInt(node, "xres");
   data->yres = AiNodeGetInt(node, "yres");
   data->zres = AiNodeGetInt(node, "zres");
   
   data->transparency = AiNodeGetRGB(node, "transparency");
   data->transparency.r = CLAMP((1.f - data->transparency.r) / data->transparency.r, 0.f, AI_BIG);
   data->transparency.g = CLAMP((1.f - data->transparency.g) / data->transparency.g, 0.f, AI_BIG);
   data->transparency.b = CLAMP((1.f - data->transparency.b) / data->transparency.b, 0.f, AI_BIG);
   data->phaseFunc = AiNodeGetFlt(node, "phase_func");
   
   const int numVoxels = data->xres * data->yres * data->zres;
   
   if (numVoxels == 0)
   {
      AiMsgWarning("[aiMayaFluid] The resolution of the fluid grid is zero!");
      return;
   }
   
   data->dmin = AiNodeGetVec(node, "min");
   data->dmax = AiNodeGetVec(node, "max");
   data->dmax = data->dmax - data->dmin;
   data->dmax.x = 1.f / data->dmax.x;
   data->dmax.y = 1.f / data->dmax.y;
   data->dmax.z = 1.f / data->dmax.z;
   
   ReadArray(node, "density", numVoxels, data->density);
   ReadArray(node, "fuel", numVoxels, data->fuel);
   ReadArray(node, "temperature", numVoxels, data->temperature);
   ReadArray(node, "pressure", numVoxels, data->pressure);
   ReadArray(node, "velocity", numVoxels, data->velocity);
   ReadArray(node, "colors", numVoxels, data->colors);
   ReadArray(node, "coordinates", numVoxels, data->coordinates);
   
   data->colorGradient.type = AiNodeGetInt(node, "color_gradient_type");
   data->colorGradient.inputBias = AiNodeGetFlt(node, "color_gradient_input_bias");
   ReadGradient(node, "color_gradient", data->colorGradient);
   data->incandescenceGradient.type = AiNodeGetInt(node, "incandescence_gradient_type");
   data->incandescenceGradient.inputBias = AiNodeGetFlt(node, "incandescence_gradient_input_bias");
   ReadGradient(node, "incandescence_gradient", data->incandescenceGradient);
   data->opacityGradient.type = AiNodeGetInt(node, "opacity_gradient_type");
   data->opacityGradient.inputBias = AiNodeGetFlt(node, "opacity_gradient_input_bias");
   ReadGradient(node, "opacity_gradient", data->opacityGradient);
   
   data->colorTexture = AiNodeGetBool(node, "color_texture");
   data->incandTexture = AiNodeGetBool(node, "incand_texture");
   data->opacityTexture = AiNodeGetBool(node, "opacity_texture");
   
   data->textureNoise = data->colorTexture || data->incandTexture || data->opacityTexture;
   
   data->textureType = AiNodeGetInt(node, "texture_type");
   
   data->volumeTexture = (AtNode*) AiNodeGetPtr(node, "volume_texture");
   
   data->textureAffectColor = AiNodeGetBool(node, "texture_affect_color");
   data->textureAffectIncand = AiNodeGetBool(node, "texture_affect_incand");
   data->textureAffectOpacity = AiNodeGetBool(node, "texture_affect_opacity");
   
   if (!(data->textureAffectColor || data->textureAffectOpacity || data->textureAffectOpacity))
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
   if (data->coordinateMethod == CM_GRID && (data->coordinates.data == 0))
      data->coordinateMethod = CM_FIXED;
}

node_finish
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   delete data;
}

template<typename T>
T GetFilteredValue(MayaFluidData* data, const AtVector& lPt, const ArrayDescription<T>& arrayDesc) // simple linear interpolation
{
   if (arrayDesc.data == 0)
      return GetDefaultValue<T>();
   if (arrayDesc.single)
      return *arrayDesc.data;
   // position in the voxel grid
   const AtVector fc = {lPt.x * (float)data->xres - .5f, lPt.y * (float)data->yres - .5f, lPt.z * (float)data->zres - .5f};
   
   // lower voxel coordiantes
   const int lcx = CLAMP((int)floorf(fc.x), 0, data->xres - 1);
   const int lcy = CLAMP((int)floorf(fc.y), 0, data->yres - 1);
   const int lcz = CLAMP((int)floorf(fc.z), 0, data->zres - 1);
   
   // higher voxel coordinates
   const int hcx = MIN(lcx + 1, data->xres - 1);
   const int hcy = MIN(lcy + 1, data->yres - 1);
   const int hcz = MIN(lcz + 1, data->zres - 1);
   
   // weight for the lower coordinates
   const AtVector pc = {fc.x - (float)lcx, fc.y - (float)lcy, fc.z - (float)lcz};
   // weight for the upper coordinates
   const AtVector npc = {1.f - pc.x, 1.f - pc.y, 1.f - pc.z};
   const int xres = data->xres;
   const int xyres = xres * data->yres;
   // sample coordinates
   const int c000 = lcx + lcy * xres + lcz * xyres;
   const int c010 = lcx + hcy * xres + lcz * xyres;
   const int c001 = lcx + lcy * xres + hcz * xyres;
   const int c011 = lcx + hcy * xres + hcz * xyres;
   const int c100 = hcx + lcy * xres + lcz * xyres;
   const int c110 = hcx + hcy * xres + lcz * xyres;
   const int c101 = hcx + lcy * xres + hcz * xyres;
   const int c111 = hcx + hcy * xres + hcz * xyres;
   
   return ((arrayDesc.data[c000] * npc.y + arrayDesc.data[c010] * pc.y) * npc.x + 
           (arrayDesc.data[c110] * pc.y + arrayDesc.data[c100] * npc.y) * pc.x) * npc.z +
          ((arrayDesc.data[c001] * npc.y  + arrayDesc.data[c011] * pc.y) * npc.x +
           (arrayDesc.data[c111] * pc.y + arrayDesc.data[c101] * npc.y) * pc.x ) * pc.z;   
}

inline float ApplyBias(const float& value, const float& bias)
{
   if (bias == 0.f)
      return value;
   else
   {
      const float b = bias < -.99f ? -.99f : bias;
      const float x = value < 0.f ? 0.f : value;
      
      return powf(x, (b - 1.f) / (-b - 1.f));
   }
}

template <typename T>
T GetGradientValue(const GradientDescription<T>& gradient, const float& v, const float& bias = 0.f)
{
   if (gradient.resolution == 0)
      return GetDefaultValue<T>();
   const float _v = ApplyBias(v, bias);
   const float p = _v * gradient.resolution;
   float pf = floorf(p);
   const int b = CLAMP((int)pf, 0, gradient.resolution - 1);
   const int e = MIN(b + 1, gradient.resolution - 1);
   pf = p - pf;
   return gradient.data[b] * (1.f - pf) + gradient.data[e] * pf;
}

AtVector ConvertToLocalSpace(MayaFluidData* data, const AtVector& cPt)
{
   AtVector lPt;
   lPt = (cPt - data->dmin) * data->dmax;
   lPt.x = CLAMP(lPt.x, 0.f, 1.f);
   lPt.y = CLAMP(lPt.y, 0.f, 1.f);
   lPt.z = CLAMP(lPt.z, 0.f, 1.f);
   return lPt;
}

template <typename T>
T GetValue(MayaFluidData* data, const AtVector& lPt, const GradientDescription<T>& gradient)
{
   static const AtVector middlePoint = {0.5f, 0.5f, 0.5f};
   switch (gradient.type)
   {
      case GT_CONSTANT:
         return GetGradientValue(gradient, 1.f, gradient.inputBias);
      case GT_X_GRADIENT:
         return GetGradientValue(gradient, 1.f - lPt.x, gradient.inputBias);
      case GT_Y_GRADIENT:
         return GetGradientValue(gradient, 1.f - lPt.y, gradient.inputBias);
      case GT_Z_GRADIENT:
         return GetGradientValue(gradient, 1.f - lPt.z, gradient.inputBias);
      case GT_CENTER_GRADIENT:
         return GetGradientValue(gradient, 1.0f - AiV3Length(lPt - middlePoint), gradient.inputBias);
      case GT_DENSITY:
         return GetGradientValue(gradient, GetFilteredValue(data, lPt, data->density), gradient.inputBias);
      case GT_TEMPERATURE:
         return GetGradientValue(gradient, GetFilteredValue(data, lPt, data->temperature), gradient.inputBias);
      case GT_FUEL:
         return GetGradientValue(gradient, GetFilteredValue(data, lPt, data->fuel), gradient.inputBias);
      case GT_PRESSURE:
         return GetGradientValue(gradient, GetFilteredValue(data, lPt, data->pressure), gradient.inputBias);
      case GT_SPEED:
         return GetGradientValue(gradient, AiV3Length(GetFilteredValue(data, lPt, data->velocity)), gradient.inputBias);
      default:
         return GetDefaultValue<T>();
   }
}

void ApplyImplode( AtVector& v, float implode, const AtVector& implodeCenter)
{
   if ((implode > AI_EPSILON) || (implode < -AI_EPSILON))
   {
      v -= implodeCenter;
      const float dist = AiV3Length(v);
      if (dist > AI_EPSILON)
      {
         const float fac = powf(dist, 1.f - implode) / dist;
         v *= fac;
      }
      v += implodeCenter;
   }
}

shader_evaluate
{
#if AI_VERSION_MINOR_NUM > 11
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);

   const AtVector lPt = ConvertToLocalSpace(data, sg->Po);
   if (data->textureDisabledInShadows && (sg->Rt & AI_RAY_SHADOW))
   {
      const float opacity = GetValue(data, lPt, data->opacityGradient) * AiShaderEvalParamFlt(p_shadow_opacity); 
      AiShaderGlobalsSetVolumeAttenuation(sg, data->transparency * opacity);
      return;
   }
   
   float colorNoise = 1.f; // colors?
   float incandNoise = 1.f;
   float opacityNoise = 1.f;
   if (data->volumeTexture)
   {
      if (data->coordinateMethod == CM_GRID)
      {
         const AtVector oldP = sg->P;
         const AtVector oldPo = sg->Po;
         sg->P = GetFilteredValue(data, lPt, data->coordinates);
         sg->Po = sg->P;
         AtMatrix oldM, oldMinv;
         AiM4Copy(oldM, sg->M);
         AiM4Copy(oldMinv, sg->Minv);
         AiM4Identity(sg->M);
         AiM4Identity(sg->Minv);
         AiShaderEvaluate(data->volumeTexture, sg);
         sg->P = oldP;
         sg->Po = oldPo;
         AiM4Copy(sg->M, oldM);
         AiM4Copy(sg->Minv, oldMinv);
      }
      else
         AiShaderEvaluate(data->volumeTexture, sg);
      float volumeNoise = sg->out.FLT;
      if (data->textureAffectColor)
         colorNoise = volumeNoise;
      if (data->textureAffectIncand)
         incandNoise = volumeNoise;
      if (data->textureAffectOpacity)
         opacityNoise = volumeNoise;
   }
   else if (data->textureNoise) // TODO optimize these evaluations based on raytype!
   {
      AtVector P;
      if (data->coordinateMethod == CM_GRID)
         P = GetFilteredValue(data, lPt, data->coordinates);
      else
         P = sg->Po;
      ApplyImplode(P, AiShaderEvalParamFlt(p_implode), AiShaderEvalParamVec(p_implode_center));     
      
      AtVector textureScale = AiShaderEvalParamVec(p_texture_scale);
      textureScale.x = MAX(AI_EPSILON, textureScale.x);
      textureScale.y = MAX(AI_EPSILON, textureScale.y);
      textureScale.z = MAX(AI_EPSILON, textureScale.z);
      const float frequency = AiShaderEvalParamFlt(p_frequency);
      P.x *= frequency / textureScale.x;
      P.y *= frequency / textureScale.y;
      P.z *= frequency / textureScale.z;      
      P.x += AiShaderEvalParamFlt(p_texture_origin_x);
      P.y += AiShaderEvalParamFlt(p_texture_origin_y);
      P.z += AiShaderEvalParamFlt(p_texture_origin_z);
      float amp = AiShaderEvalParamFlt(p_amplitude);
      float volumeNoise = 0.f;
      const float frequencyRatio = AiShaderEvalParamFlt(p_frequency_ratio);
      const float ratio = AiShaderEvalParamFlt(p_ratio);
      const float timeRatio = sqrtf(frequencyRatio);
      float textureTime = AiShaderEvalParamFlt(p_texture_time);
      const int depthMax = AiShaderEvalParamInt(p_depth_max);
      const bool inflection = AiShaderEvalParamBool(p_inflection);
      
      for (int i = 0; i < depthMax; ++i)
      {
         float noise = AiPerlin4(P, textureTime);
         if (inflection)
            noise = ABS(noise);
         
         volumeNoise += amp * noise;
         
         amp *= ratio;
         P *= frequencyRatio;
         textureTime *= timeRatio;
      }
      
      const float threshold = AiShaderEvalParamFlt(p_threshold);
      if (inflection)
         volumeNoise += threshold;
      else
         volumeNoise = (volumeNoise * .5f + .5f) + threshold;
      
      if (volumeNoise > 1.f)
         volumeNoise = 1.f;
      else if (volumeNoise < 0.f)
         volumeNoise = 0.f;
      
      if (AiShaderEvalParamBool(p_invert_texture))
         volumeNoise = MAX(1.f - volumeNoise, 0.f);
      if (data->colorTexture)
         colorNoise = AiShaderEvalParamFlt(p_color_tex_gain) * volumeNoise;
      if (data->incandTexture)
         incandNoise = AiShaderEvalParamFlt(p_incand_tex_gain) * volumeNoise;
      if (data->opacityTexture)
         opacityNoise = AiShaderEvalParamFlt(p_opacity_tex_gain) * volumeNoise;
   }
   
   if (sg->Rt & AI_RAY_SHADOW)
   {
      const float opacity = GetValue(data, lPt, data->opacityGradient) * opacityNoise * AiShaderEvalParamFlt(p_shadow_opacity); 
      AiShaderGlobalsSetVolumeAttenuation(sg, data->transparency * opacity);
      return;
   }  
   
   const AtRGB opacity = GetValue(data, lPt, data->opacityGradient) * data->transparency * opacityNoise; 
   const AtRGB color = GetValue(data, lPt, data->colorGradient) * colorNoise;
   const AtRGB incandescence = GetValue(data, lPt, data->incandescenceGradient) * incandNoise;
   
   AiShaderGlobalsSetVolumeAttenuation(sg, opacity * AI_RGB_WHITE);
   AiShaderGlobalsSetVolumeEmission(sg, opacity * incandescence);
   AiShaderGlobalsSetVolumeScattering(sg, opacity * color, data->phaseFunc);
#endif
}
