#include <ai.h>

#include <memory.h>
#include <cmath>

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

const char* coordinateMethodEnums[] = {"Fixed", "Grid", 0};

enum coordinateMethod{
   CM_FIXED,
   CM_GRID
};

const char* filterTypeEnums[] = {"Closest", "Linear", "Cubic", 0};

enum filterType{
   FT_CLOSEST = 0,
   FT_LINEAR,
   FT_CUBIC
};

const char* gradientTypeEnums[] = {"Constant", "X Gradient", "Y Gradient",
                                    "Z Gradient", "Center Gradient", "Density",
                                    "Temperature", "Fuel", "Pressure", "Speed",
                                    "Density And Fuel", 0};

enum gradientType{
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

const char* dropoffShapeEnums[] = {"Off", "Sphere", "Cube", "Cone", "Double Cone",
                                   "X Gradient", "Y Gradient", "Z Gradient",
                                   "-X Gradient", "-Y Gradient", "-Z Gradient",
                                   "Use Falloff Grid", 0};

enum dropoffShape{
   DS_OFF = 0,
   DS_SPHERE,
   DS_CUBE,
   DS_CONE,
   DS_DOUBLE_CONE,
   DS_X_GRADIENT,
   DS_Y_GRADIENT,
   DS_Z_GRADIENT,
   DS_NX_GRADIENT,
   DS_NY_GRADIENT,
   DS_NZ_GRADIENT,
   DS_USE_FALLOFF_GRID
};

// http://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
inline float FastPow(float a, float b) {
   union {
      double d;
      int x[2];
   } u = { (double)a };
   u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
   u.x[0] = 0;
   return (float)u.d;
}

node_parameters
{
   AiParameterEnum("filter_type", FT_LINEAR, filterTypeEnums);
   
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
   
   AiParameterArray("falloff", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   
   AiParameterEnum("color_gradient_type", GT_CONSTANT, gradientTypeEnums);
   AiParameterArray("color_gradient", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   AiParameterFlt("color_gradient_input_bias", 0.0f);
   
   AiParameterEnum("incandescence_gradient_type", GT_TEMPERATURE, gradientTypeEnums);
   AiParameterArray("incandescence_gradient", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   AiParameterFlt("incandescence_gradient_input_bias", 0.0f);
   
   AiParameterEnum("opacity_gradient_type", GT_DENSITY, gradientTypeEnums);
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

   AiParameterEnum("dropoff_shape", 2, dropoffShapeEnums);
   AiParameterFlt("edge_dropoff", 0.05f);
   
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiMayaFluid");
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D1E);
}

enum MayaFluidParams{
   p_filter_type,
   
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
   p_falloff,
   
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

   p_dropoff_shape,
   p_edge_dropoff
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
   ArrayDescription<float> falloff;
   
   GradientDescription<AtRGB> colorGradient;
   GradientDescription<AtRGB> incandescenceGradient;
   GradientDescription<float> opacityGradient;  
   
   AtRGB transparency; 
   
   AtVector dmin, dmax;
   
   AtNode* volumeTexture;
   
   float phaseFunc;
   float edgeDropoff;
   
   int filterType;   
   int xres, yres, zres;      
   int textureType;
   int coordinateMethod;
   int dropoffShape;
   
   bool colorTexture;
   bool incandTexture;
   bool opacityTexture;
   bool textureNoise;   
   bool textureAffectColor;
   bool textureAffectIncand;
   bool textureAffectOpacity;
   bool textureDisabledInShadows;   
   
   ~MayaFluidData()
   {
      density.release();
      fuel.release();
      temperature.release();
      pressure.release();
      velocity.release();
      colors.release();
      coordinates.release();
      colorGradient.release();
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
   AiNodeSetLocalData(node, new MayaFluidData());
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
   
   data->filterType = AiNodeGetInt(node, "filter_type");
   
   data->xres = AiNodeGetInt(node, "xres");
   data->yres = AiNodeGetInt(node, "yres");
   data->zres = AiNodeGetInt(node, "zres");
   
   data->transparency = AiNodeGetRGB(node, "transparency");
   data->transparency.r = CLAMP((1.f - data->transparency.r) / data->transparency.r, 0.f, AI_BIG);
   data->transparency.g = CLAMP((1.f - data->transparency.g) / data->transparency.g, 0.f, AI_BIG);
   data->transparency.b = CLAMP((1.f - data->transparency.b) / data->transparency.b, 0.f, AI_BIG);
   data->phaseFunc = AiNodeGetFlt(node, "phase_func");

   data->edgeDropoff = AiNodeGetFlt(node, "edge_dropoff");
   if (ABS(data->edgeDropoff) > AI_EPSILON)
      data->dropoffShape = AiNodeGetInt(node, "dropoff_shape");
   else
      data->dropoffShape = DS_OFF;
   
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
   ReadArray(node, "falloff", numVoxels, data->falloff);
   
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

template <typename T>
T MonotonicCubicInterpolant(const T& f1, const T& f2, const T& f3, const T& f4,
                          float t)
{
   return GetDefaultValue<T>();
}

template <>
float MonotonicCubicInterpolant<float>(const float& f1, const float& f2, const float& f3, const float& f4, 
                                       float t)
{
  float d_k = .5f * (f3 - f1);
  float d_k1 = .5f * (f4 - f2);
  const float delta_k = f3 - f2;

  if (delta_k == 0.f) {
    d_k = 0.f;
    d_k1 = 0.f;
  }

  const float a0 = f2;
  const float a1 = d_k;
  const float a2 = (3.f * delta_k) - (2.f * d_k) - d_k1;
  const float a3 = d_k + d_k1 - (2.f * delta_k);

  const float t1 = t;
  const float t2 = t1 * t1;
  const float t3 = t2 * t1;

  return a3 * t3 + a2 * t2 + a1 * t1 + a0;
}

template <> // specialize this later, maybe the compiler can use SSE better that way
AtRGB MonotonicCubicInterpolant(const AtRGB& f1, const AtRGB& f2, const AtRGB& f3, const AtRGB& f4,
                                float t)
{
   AtRGB ret = {MonotonicCubicInterpolant(f1.r, f2.r, f3.r, f4.r, t),
      MonotonicCubicInterpolant(f1.g, f2.g, f3.g, f4.g, t),
      MonotonicCubicInterpolant(f1.b, f2.b, f3.b, f4.b, t)
   };
   return ret;
}

template <>
AtVector MonotonicCubicInterpolant(const AtVector& f1, const AtVector& f2, const AtVector& f3, const AtVector& f4,
                                float t)
{
   AtVector ret = {MonotonicCubicInterpolant(f1.x, f2.x, f3.x, f4.x, t),
      MonotonicCubicInterpolant(f1.y, f2.y, f3.y, f4.y, t),
      MonotonicCubicInterpolant(f1.z, f2.z, f3.z, f4.z, t)
   };
   return ret;
}

template <typename T>
T Filter(const MayaFluidData* data, const AtVector& lPt, const ArrayDescription<T>& arrayDesc)
{
   if (data->filterType == FT_CLOSEST)
   {
      if (arrayDesc.data == 0)
         return GetDefaultValue<T>();
      if (arrayDesc.single)
         return *arrayDesc.data;
      // position in the voxel grid
      const AtVector fc = {lPt.x * (float)data->xres, lPt.y * (float)data->yres, lPt.z * (float)data->zres};
      // voxel coordiantes
      const int lcx = CLAMP((int)fc.x, 0, data->xres - 1);
      const int lcy = CLAMP((int)fc.y, 0, data->yres - 1);
      const int lcz = CLAMP((int)fc.z, 0, data->zres - 1);
      return arrayDesc.data[lcx + lcy * data->xres + lcz * data->xres * data->yres];
   }
   else if (data->filterType == FT_LINEAR)
   {
      if (arrayDesc.data == 0)
         return GetDefaultValue<T>();
      if (arrayDesc.single)
         return *arrayDesc.data;
      // position in the voxel grid
      const AtVector fc = {lPt.x * (float)data->xres - .5f, lPt.y * (float)data->yres - .5f, lPt.z * (float)data->zres - .5f};

      // lower voxel coordiantes
      const int lcx = CLAMP((int)fc.x, 0, data->xres - 1);
      const int lcy = CLAMP((int)fc.y, 0, data->yres - 1);
      const int lcz = CLAMP((int)fc.z, 0, data->zres - 1);

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
   else
   {
      if (arrayDesc.data == 0)
         return GetDefaultValue<T>();
      if (arrayDesc.single)
         return *arrayDesc.data;
      // position in the voxel grid
      const AtVector fc = {lPt.x * (float)data->xres - .5f, lPt.y * (float)data->yres - .5f, lPt.z * (float)data->zres - .5f};
      // lower voxel coordiantes
      const int t2x = CLAMP((int)fc.x, 0, data->xres - 1);
      const int t2y = CLAMP((int)fc.y, 0, data->yres - 1);
      const int t2z = CLAMP((int)fc.z, 0, data->zres - 1);
      
      const AtVector t = {fc.x - (float)t2x, fc.y - (float)t2y, fc.z - (float)t2z};
      
      const int t1x = MAX(t2x - 1, 0);
      const int t1y = MAX(t2y - 1, 0);
      const int t1z = MAX(t2z - 1, 0);
      
      const int t3x = MIN(t2x + 1, data->xres - 1);
      const int t3y = MIN(t2y + 1, data->yres - 1);
      const int t3z = MIN(t2z + 1, data->zres - 1);
      
      const int t4x = MIN(t3x + 1, data->xres - 1);
      const int t4y = MIN(t3y + 1, data->yres - 1);
      const int t4z = MIN(t3z + 1, data->zres - 1);
      const int xres = data->xres;
      const int xyres = xres * data->yres;
      
      const T z11 = MonotonicCubicInterpolant(arrayDesc.data[t1x + t1y * xres + t1z * xyres],
              arrayDesc.data[t1x + t1y * xres + t2z * xyres],
              arrayDesc.data[t1x + t1y * xres + t3z * xyres],
              arrayDesc.data[t1x + t1y * xres + t4z * xyres], t.z);
      
      const T z12 = MonotonicCubicInterpolant(arrayDesc.data[t1x + t2y * xres + t1z * xyres],
              arrayDesc.data[t1x + t2y * xres + t2z * xyres],
              arrayDesc.data[t1x + t2y * xres + t3z * xyres],
              arrayDesc.data[t1x + t2y * xres + t4z * xyres], t.z);
      
      const T z13 = MonotonicCubicInterpolant(arrayDesc.data[t1x + t3y * xres + t1z * xyres],
              arrayDesc.data[t1x + t3y * xres + t2z * xyres],
              arrayDesc.data[t1x + t3y * xres + t3z * xyres],
              arrayDesc.data[t1x + t3y * xres + t4z * xyres], t.z);
      
      const T z14 = MonotonicCubicInterpolant(arrayDesc.data[t1x + t4y * xres + t1z * xyres],
              arrayDesc.data[t1x + t4y * xres + t2z * xyres],
              arrayDesc.data[t1x + t4y * xres + t3z * xyres],
              arrayDesc.data[t1x + t4y * xres + t4z * xyres], t.z);
      
      const T z21 = MonotonicCubicInterpolant(arrayDesc.data[t2x + t1y * xres + t1z * xyres],
              arrayDesc.data[t2x + t1y * xres + t2z * xyres],
              arrayDesc.data[t2x + t1y * xres + t3z * xyres],
              arrayDesc.data[t2x + t1y * xres + t4z * xyres], t.z);
      
      const T z22 = MonotonicCubicInterpolant(arrayDesc.data[t2x + t2y * xres + t1z * xyres],
              arrayDesc.data[t2x + t2y * xres + t2z * xyres],
              arrayDesc.data[t2x + t2y * xres + t3z * xyres],
              arrayDesc.data[t2x + t2y * xres + t4z * xyres], t.z);
      
      const T z23 = MonotonicCubicInterpolant(arrayDesc.data[t2x + t3y * xres + t1z * xyres],
              arrayDesc.data[t2x + t3y * xres + t2z * xyres],
              arrayDesc.data[t2x + t3y * xres + t3z * xyres],
              arrayDesc.data[t2x + t3y * xres + t4z * xyres], t.z);
      
      const T z24 = MonotonicCubicInterpolant(arrayDesc.data[t2x + t4y * xres + t1z * xyres],
              arrayDesc.data[t2x + t4y * xres + t2z * xyres],
              arrayDesc.data[t2x + t4y * xres + t3z * xyres],
              arrayDesc.data[t2x + t4y * xres + t4z * xyres], t.z);
      
      const T z31 = MonotonicCubicInterpolant(arrayDesc.data[t3x + t1y * xres + t1z * xyres],
              arrayDesc.data[t3x + t1y * xres + t2z * xyres],
              arrayDesc.data[t3x + t1y * xres + t3z * xyres],
              arrayDesc.data[t3x + t1y * xres + t4z * xyres], t.z);
      
      const T z32 = MonotonicCubicInterpolant(arrayDesc.data[t3x + t2y * xres + t1z * xyres],
              arrayDesc.data[t3x + t2y * xres + t2z * xyres],
              arrayDesc.data[t3x + t2y * xres + t3z * xyres],
              arrayDesc.data[t3x + t2y * xres + t4z * xyres], t.z);
      
      const T z33 = MonotonicCubicInterpolant(arrayDesc.data[t3x + t3y * xres + t1z * xyres],
              arrayDesc.data[t3x + t3y * xres + t2z * xyres],
              arrayDesc.data[t3x + t3y * xres + t3z * xyres],
              arrayDesc.data[t3x + t3y * xres + t4z * xyres], t.z);
      
      const T z34 = MonotonicCubicInterpolant(arrayDesc.data[t3x + t4y * xres + t1z * xyres],
              arrayDesc.data[t3x + t4y * xres + t2z * xyres],
              arrayDesc.data[t3x + t4y * xres + t3z * xyres],
              arrayDesc.data[t3x + t4y * xres + t4z * xyres], t.z);
      
      const T z41 = MonotonicCubicInterpolant(arrayDesc.data[t4x + t1y * xres + t1z * xyres],
              arrayDesc.data[t4x + t1y * xres + t2z * xyres],
              arrayDesc.data[t4x + t1y * xres + t3z * xyres],
              arrayDesc.data[t4x + t1y * xres + t4z * xyres], t.z);
      
      const T z42 = MonotonicCubicInterpolant(arrayDesc.data[t4x + t2y * xres + t1z * xyres],
              arrayDesc.data[t4x + t2y * xres + t2z * xyres],
              arrayDesc.data[t4x + t2y * xres + t3z * xyres],
              arrayDesc.data[t4x + t2y * xres + t4z * xyres], t.z);
      
      const T z43 = MonotonicCubicInterpolant(arrayDesc.data[t4x + t3y * xres + t1z * xyres],
              arrayDesc.data[t4x + t3y * xres + t2z * xyres],
              arrayDesc.data[t4x + t3y * xres + t3z * xyres],
              arrayDesc.data[t4x + t3y * xres + t4z * xyres], t.z);
      
      const T z44 = MonotonicCubicInterpolant(arrayDesc.data[t4x + t4y * xres + t1z * xyres],
              arrayDesc.data[t4x + t4y * xres + t2z * xyres],
              arrayDesc.data[t4x + t4y * xres + t3z * xyres],
              arrayDesc.data[t4x + t4y * xres + t4z * xyres], t.z);
      
      const T y1 = MonotonicCubicInterpolant(z11, z12, z13, z14, t.y);
      const T y2 = MonotonicCubicInterpolant(z21, z22, z23, z24, t.y);
      const T y3 = MonotonicCubicInterpolant(z31, z32, z33, z34, t.y);
      const T y4 = MonotonicCubicInterpolant(z41, z42, z43, z44, t.y);
      
      return MonotonicCubicInterpolant(y1, y2, y3, y4, t.x);
   }
}

inline
float ApplyBias(const float& value, const float& bias)
{
   if (bias == 0.f)
      return value;
   else
   {
      const float b = bias < -.99f ? -.99f : bias;
      const float x = value < 0.f ? 0.f : value;
      
      return FastPow(x, (b - 1.f) / (-b - 1.f));
   }
}

template <typename T>
T GetGradientValue(const GradientDescription<T>& gradient, const float& v, const float& bias = 0.f)
{
   if (gradient.resolution == 0)
      return GetDefaultValue<T>();
   const float _v = ApplyBias(v, bias);
   const float p = _v * gradient.resolution;
   const int pi = (int)p;
   const int b = CLAMP(pi, 0, gradient.resolution - 1);
   const int e = MIN(b + 1, gradient.resolution - 1);
   const float pf = p - (float)pi;
   return gradient.data[b] * (1.f - pf) + gradient.data[e] * pf;
}

inline
AtVector ConvertToLocalSpace(const MayaFluidData* data, const AtVector& cPt)
{
   AtVector lPt;
   lPt = (cPt - data->dmin) * data->dmax;
   return lPt;
}

template <typename T>
T GetValue(const MayaFluidData* data, const AtVector& lPt, const GradientDescription<T>& gradient)
{
   static const AtVector middlePoint = {0.5f, 0.5f, 0.5f};
   float gradientValue = 0.f;
   switch (gradient.type)
   {
      case GT_CONSTANT:
         gradientValue = 1.f;
         break;
      case GT_X_GRADIENT:
         gradientValue = 1.f - lPt.x;
         break;
      case GT_Y_GRADIENT:
         gradientValue = 1.f - lPt.y;
         break;
      case GT_Z_GRADIENT:
         gradientValue = 1.f - lPt.z;
         break;
      case GT_CENTER_GRADIENT:
         gradientValue = 1.f - AiV3Length(lPt - middlePoint);
         break;
      case GT_DENSITY:
         gradientValue = Filter(data, lPt, data->density);
         break;
      case GT_TEMPERATURE:
         gradientValue = Filter(data, lPt, data->temperature);
         break;
      case GT_FUEL:
         gradientValue = Filter(data, lPt, data->fuel);
         break;
      case GT_PRESSURE:
         gradientValue = Filter(data, lPt, data->pressure);
         break;
      case GT_SPEED:
         gradientValue = AiV3Length(Filter(data, lPt, data->velocity));
         break;
      default:
         return GetDefaultValue<T>();
   }
   return GetGradientValue(gradient, gradientValue, gradient.inputBias);
}

inline
void ApplyImplode( AtVector& v, float implode, const AtVector& implodeCenter)
{
   if ((implode > AI_EPSILON) || (implode < -AI_EPSILON))
   {
      v -= implodeCenter;
      const float dist = AiV3Length(v);
      if (dist > AI_EPSILON)
      {
         const float fac = FastPow(dist, 1.f - implode) / dist;
         v *= fac;
      }
      v += implodeCenter;
   }
}

inline
float DropoffGradient(float value, float edgeDropoff)
{
   float ret;
   if (edgeDropoff < .5f)
      ret = (1.f - value - (1.f - 2.f * edgeDropoff)) / (2.f * edgeDropoff);
   else
      ret = (value - (2.f* (edgeDropoff - .5f))) / (1.f - 2.f * (edgeDropoff - .5f));
	return CLAMP(ret, 0.f, 1.f);
}

inline
float CalculateDropoff(const MayaFluidData* data, const AtVector& lPt)
{
   if (data->dropoffShape == DS_OFF)
      return 1.f;
   const AtVector cPt = (lPt - AI_V3_HALF) * 2.f;
   const float edgeDropoff = data->edgeDropoff;
   switch(data->dropoffShape)
   {
      case DS_SPHERE:
         {
            const float d = 1.f - AiV3Length(cPt);
            if (d < 0.f)
               return 0.f;
            else if (d < edgeDropoff)
               return 0.5f * sinf((float)AI_PI * (d / edgeDropoff - .5f)) + .5f;
            else
               return 1.f;
         }
         return 1.f - CLAMP((AiV3Length(cPt) - 1.f + edgeDropoff) / edgeDropoff, 0.f, 1.f);
      case DS_CUBE:
         cPt.x = (1.f - ABS(cPt.x)) / edgeDropoff;
         cPt.y = (1.f - ABS(cPt.y)) / edgeDropoff;
         cPt.z = (1.f - ABS(cPt.z)) / edgeDropoff;
         if (cPt.x < 0.f || cPt.y < 0.f || cPt.z < 0.f)
            return 0.f;
         else
         {
            if (cPt.x < 1.f)
               cPt.x = .5f * sinf((float)AI_PI * (cPt.x - .5f)) + .5f;
            else
               cPt.x = 1.f;
            if (cPt.y < 1.f)
               cPt.y = .5f * sinf((float)AI_PI * (cPt.y - .5f)) + .5f;
            else
               cPt.y = 1.f;
            if (cPt.z < 1.f)
               cPt.z = .5f * sinf((float)AI_PI * (cPt.z - .5f)) + .5f;
            else
               cPt.z = 1.f;
            return cPt.x * cPt.y * cPt.z;
         }
      case DS_CONE:
         {
            const float d = -2.f * sqrtf(cPt.x * cPt.x + cPt.y * cPt.y) + ABS(cPt.z + 1.f);
            if (d < 0.f)
               return 0.f;
            else if (d < edgeDropoff)
               return 0.5f * sinf((float)AI_PI * (d / edgeDropoff - .5f)) + .5f;
            else
               return 1.f;
         }
      case DS_DOUBLE_CONE:         
         {
            const float d = -2.f * sqrtf(cPt.x * cPt.x + cPt.y * cPt.y) + ABS(cPt.z);
            if (d < 0.f)
               return 0.f;
            else if (d < edgeDropoff)
               return 0.5f * sinf((float)AI_PI * (d / edgeDropoff - .5f)) + .5f;
            else
               return 1.f;
         }
      case DS_X_GRADIENT:
         return DropoffGradient(.5f - cPt.x * .5f, edgeDropoff);
      case DS_Y_GRADIENT:
         return DropoffGradient(.5f - cPt.y * .5f, edgeDropoff);
      case DS_Z_GRADIENT:
         return DropoffGradient(.5f - cPt.z * .5f, edgeDropoff);
      case DS_NX_GRADIENT:
         return DropoffGradient(cPt.x * .5f + .5f, edgeDropoff);
      case DS_NY_GRADIENT:
         return DropoffGradient(cPt.y * .5f + .5f, edgeDropoff);
      case DS_NZ_GRADIENT:
         return DropoffGradient(cPt.z * .5f + .5f, edgeDropoff);
      case DS_USE_FALLOFF_GRID:
         {
            const float d = Filter(data, lPt, data->falloff);
            if (d > 0.f && edgeDropoff < 0.9999f)
               return powf(d, edgeDropoff / (1.f - edgeDropoff));
            else
               return 0.f;
         }
      default:
         return 1.f;
   }
}

#if AI_VERSION_MINOR_NUM > 11

shader_evaluate
{
   const MayaFluidData* data = (const MayaFluidData*)AiNodeGetLocalData(node);
   
   const AtVector lPt = ConvertToLocalSpace(data, sg->Po);

   float opacityNoise = CalculateDropoff(data, lPt);

   if (data->textureDisabledInShadows && (sg->Rt & AI_RAY_SHADOW))
   {
      const float opacity = GetValue(data, lPt, data->opacityGradient) * AiShaderEvalParamFlt(p_shadow_opacity) * opacityNoise;
      AiShaderGlobalsSetVolumeAttenuation(sg, data->transparency * opacity);
      return;
   }
   
   float colorNoise = 1.f; // colors?
   float incandNoise = 1.f;
   if (data->volumeTexture)
   {
      if (data->coordinateMethod == CM_GRID)
      {
         const AtVector oldP = sg->P;
         const AtVector oldPo = sg->Po;
         sg->P = Filter(data, lPt, data->coordinates);
         sg->Po = sg->P;
         AiShaderEvaluate(data->volumeTexture, sg);
         sg->P = oldP;
         sg->Po = oldPo;
      }
      else
         AiShaderEvaluate(data->volumeTexture, sg);
      float volumeNoise = sg->out.FLT;
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
      if (data->coordinateMethod == CM_GRID)
         P = Filter(data, lPt, data->coordinates);
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
         opacityNoise *= AiShaderEvalParamFlt(p_opacity_tex_gain) * volumeNoise;
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
}

#else

shader_evaluate
{
   
}

#endif
