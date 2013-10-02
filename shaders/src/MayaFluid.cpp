#include <ai.h>

#include <memory.h>
#include <cmath>

#include <string>
#include <sstream>
#include <algorithm>
#include <map>

#include "MayaUtils.h"
#include "MayaFluidData.h"

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
   CM_FIXED = 0,
   CM_GRID
};

const char* filterTypeEnums[] = {"Closest", "Linear", "Cubic", 0};

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
   AiMetaDataSetBool(mds, "transparency", "always_linear", true);
   AiParameterFlt("phase_func", 0.f);
   
   AiParameterEnum("color_gradient_type", GT_CONSTANT, gradientTypeEnums);
   AiParameterArray("color_gradient_positions", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("color_gradient_values", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   AiParameterArray("color_gradient_interps", AiArrayAllocate(0, 1, AI_TYPE_INT));   
   AiParameterFlt("color_gradient_input_bias", 0.0f);
   
   AiParameterEnum("incandescence_gradient_type", GT_TEMPERATURE, gradientTypeEnums);
   AiParameterArray("incandescence_gradient_positions", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("incandescence_gradient_values", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   AiParameterArray("incandescence_gradient_interps", AiArrayAllocate(0, 1, AI_TYPE_INT));   
   AiParameterFlt("incandescence_gradient_input_bias", 0.0f);
   
   AiParameterEnum("opacity_gradient_type", GT_DENSITY, gradientTypeEnums);
   AiParameterArray("opacity_gradient_positions", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("opacity_gradient_values", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("opacity_gradient_interps", AiArrayAllocate(0, 1, AI_TYPE_INT));   
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
   
   AiParameterEnum("coordinate_method", 0, coordinateMethodEnums);
   
   AiParameterFlt("shadow_opacity", 0.5f);

   AiParameterEnum("dropoff_shape", 2, dropoffShapeEnums);
   AiParameterFlt("edge_dropoff", 0.05f);

   AiParameterVec("velocity_scale", 1.f, 1.f, 1.f);

   InitializeFluidShaderParameters(params);
   
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);
}

enum MayaFluidParams{
   p_filter_type,
   
   p_transparency,
   p_phase_func,
   
   p_color_gradient_type,
   p_color_gradient_positions,
   p_color_gradient_values,
   p_color_gradient_interps,   
   p_color_gradient_input_bias,
   
   p_incandescence_gradient_type,
   p_incandescence_gradient_positions,
   p_incandescence_gradient_values,
   p_incandescence_gradient_interps,   
   p_incandescence_gradient_input_bias,
   
   p_opacity_gradient_type,
   p_opacity_gradient_positions,
   p_opacity_gradient_values,
   p_opacity_gradient_interps,   
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
   p_coordinate_method,
   
   p_shadow_opacity,

   p_dropoff_shape,
   p_edge_dropoff,

   p_velocity_scale
};

template <typename T>
T GetValueFromSG(AtShaderGlobals* sg, int outputType = AI_TYPE_RGB)
{
   return GetDefaultValue<T>();
}

template <>
float GetValueFromSG(AtShaderGlobals* sg, int outputType)
{
   return sg->out.FLT;
}

template <>
AtRGB GetValueFromSG(AtShaderGlobals* sg, int outputType)
{
   AtRGB ret = AI_RGB_BLACK;
   switch (outputType)
   {
   case AI_TYPE_FLOAT:
      ret = sg->out.FLT;
      break;
   case AI_TYPE_POINT2:
      ret.r = sg->out.PNT2.x;
      ret.g = sg->out.PNT2.y;
      break;
   default:
      ret = sg->out.RGB;
   }   
   return ret;
}

enum gradientInterps{
   GI_NONE = 0,
   GI_LINEAR,
   GI_SMOOTH,
   GI_SPLINE
};

template <typename T>
void GammaCorrect(T& d, float gamma)
{
   
}

template <>
void GammaCorrect<AtRGB>(AtRGB& d, float gamma)
{
   if (gamma != 1.f)
   {
      d.r = MAX(0.f, d.r);
      d.g = MAX(0.f, d.g);
      d.b = MAX(0.f, d.b);
      AiColorGamma(&d, gamma);
   }
}

template<typename T, bool M = true, bool G = true>
class GradientDescription{
public:
   struct GradientDescriptionElement{
      AtNode* node;
      T value;
      float position;
      int interp;
      int outputType;
   };
   GradientDescriptionElement* elements;
   AtUInt32 nelements;
   float inputBias;
   float gamma;
   
   T* data;   
   int type;
   int resolution;  
   
   GradientDescription() : elements(0), inputBias(0.f), data(0) {}
   
   void Release() 
   {
      if(data) AiFree(data); data = 0;
      if(elements) AiFree(elements); elements = 0;
   }
   
   static bool CompareElements(const GradientDescriptionElement& a,
                               const GradientDescriptionElement& b)
   {
      return a.position < b.position;
   }
   
   static inline float ApplyBias(const float& value, const float& bias)
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
   
   inline T GetElement(AtShaderGlobals* sg, AtUInt32 elem) const
   {
      if (M && (elements[elem].node != 0))
      {
         AiShaderEvaluate(elements[elem].node, sg);
         return GetValueFromSG<T>(sg, elements[elem].outputType);
      }
      else
         return elements[elem].value;
   }
   
   T GetValue(AtShaderGlobals* sg, float v) const
   {
      if (data != 0)
      {
         // No need to apply bias here, since
         // the cache already contains the applied bias
         const float p = v * resolution;
         const int pi = (int)p;
         const int b = CLAMP(pi, 0, resolution - 1);
         const int e = MIN(b + 1, resolution - 1);
         const float pf = p - (float)pi;
         return data[b] * (1.f - pf) + data[e] * pf;
      }
      
      if (elements == 0)
         return GetDefaultValue<T>();
      if (nelements == 1)
         return GetElement(sg, 0);
      
      v = ApplyBias(v, inputBias);
      
      // look for the proper segment
      AtUInt32 index = nelements;
      for (AtUInt32 i = 0; i < nelements; ++i)
      {
         if (v < elements[i].position)
         {
            index = i;
            break;
         }
      }
      
      // return the first value
      if (index == 0)
         return GetElement(sg, 0);
      
      // return the last value
      if (index == nelements)
         return GetElement(sg, nelements - 1);
      
      // interpolate between two values
      const AtUInt32 prevIndex = index - 1;
      const int interp = elements[prevIndex].interp;
      T ret = GetDefaultValue<T>();
      switch(interp)
      {
         case GI_NONE:
            ret = GetElement(sg, prevIndex);
            break;
         case GI_LINEAR:
            {
               const float interpValue = (v - elements[prevIndex].position) /
                                         (elements[index].position - elements[prevIndex].position);
               if (interpValue < AI_EPSILON)
                  ret = GetElement(sg, prevIndex);
               else if (interpValue > (1.f - AI_EPSILON))
                  ret = GetElement(sg, index);
               else
                  ret = GetElement(sg, prevIndex) * (1.f - interpValue) + GetElement(sg, index) * interpValue;
            }
            break;
         case GI_SMOOTH:
            {
               const float interpValue = (v - elements[prevIndex].position) /
                                         (elements[index].position - elements[prevIndex].position);
               const float interpValue2 = interpValue * interpValue;
               const float interpValue3 = interpValue * interpValue2;
               const float w0 =  2.f * interpValue3 - 3.f * interpValue2 + 1.f;
               const float w1 = -2.f * interpValue3 + 3.f * interpValue2 ;
               if (w0 > AI_EPSILON)
                  ret += GetElement(sg, prevIndex) * w0;
               if (w1 > AI_EPSILON)
                  ret += GetElement(sg, index) * w1;
            }
            break;
         case GI_SPLINE:
            {
               const T v1 = GetElement(sg, prevIndex);
               const T v2 = GetElement(sg, index);
               const float p1 = elements[prevIndex].position;
               const float p2 = elements[index].position;
               const float dp = p2 - p1;
               const T dv = v2 - v1;

               T v0, v3;
               float p0, p3;

               if (prevIndex == 0)
               {
                  p0 = p1 - dp;
                  v0 = v1;
               }
               else
               {
                  p0 = elements[prevIndex - 1].position;
                  v0 = GetElement(sg, prevIndex - 1);
               }

               if (index == (nelements - 1))
               {
                  p3 = p2 + dp;
                  v3 = v2;
               }
               else
               {
                  p3 = elements[index + 1].position;
                  v3 = GetElement(sg, index + 1);
               }

               const static float tanSize = .2f;
               const float tx = MAX(tanSize * dp, AI_EPSILON);

               float sx = MAX(p2 - p0, AI_EPSILON);
               T sy = v2 - v0;

               sy *= tanSize * dp / sx;
               const T m1 = sy / tx;
               sx = MAX(p3 - p1, AI_EPSILON);
               sy = v3 - v1;

               sy *= tanSize * dp / sx;
               const T m2 = sy / tx;

               float tFromP1 = (v - p1);
               float length = MIN(1.f / (dp * dp), AI_BIG);
               const T d1 = dp * m1;
               const T d2 = dp * m2;

               const T c0 = (d1 + d2 - 2.f * dv) * length / dp;
               const T c1 = (3.f * dv - 2.f * d1 - d2) * length;
               ret = tFromP1 * (tFromP1 * (tFromP1 * c0 + c1) + m1) + v1;
            }
            break;
         default:
            break;
      }
      if (G)
         GammaCorrect(ret, gamma);
      return ret;
   }
   
   void ReadValues(AtNode* node, const char* valuesName, AtArray* positionsArray, AtArray* interpsArray)
   {
      Release();
      
      AtArray* valuesArray = AiNodeGetArray(node, valuesName);
      
      AtNode* options = AiUniverseGetOptions();
      const float invGamma = AiNodeGetFlt(options, "shader_gamma");
      if (invGamma == 1.f)
         gamma = 1.f;
      else
         gamma = 1.f / invGamma;
      
      nelements = positionsArray->nelements;      
      if (nelements == 0)
         elements = 0;
      else
      {
         bool isConnected = false;
         elements = (GradientDescriptionElement*)AiMalloc(sizeof(GradientDescriptionElement) * nelements);
         for (AtUInt32 i = 0; i < nelements; ++i)
         {
            elements[i].position = AiArrayGetFlt(positionsArray, i);
            elements[i].interp = AiArrayGetInt(interpsArray, i);
            elements[i].value = GetDefaultValue<T>();
            if (M)
            {
               std::stringstream ss;
               ss << valuesName << "[" << i << "]";
               elements[i].node = AiNodeGetLink(node, ss.str().c_str());
               if (elements[i].node == 0)
                  elements[i].value = ReadFromArray<T>(valuesArray, i);
               else
               {
                  isConnected = true;
                  const AtNodeEntry* nentry = AiNodeGetNodeEntry(elements[i].node);
                  elements[i].outputType = AiNodeEntryGetOutputType(nentry);
               }
            }
            else
               elements[i].value = ReadFromArray<T>(valuesArray, i);
            GammaCorrect(elements[i].value, invGamma);
         }
         if (nelements > 1)
            std::sort(elements, elements + nelements, CompareElements);
         
         if (!isConnected && (nelements > 1))
         {
            resolution = 1024;
            T* _data = (T*)AiMalloc(resolution * sizeof(T));
            for (int i = 0; i < resolution; ++i)
            {
               const float v = (float) i / (float)(resolution - 1);
               _data[i] = GetValue(0, v);
            }

            data = _data;
         }
      }      
   }
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

   MayaFluidData() : fluidData(0)
   { }
   
   ~MayaFluidData()
   {
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
}

node_finish
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   delete data;
}

template <typename T, bool M, bool G>
T GetValue(AtShaderGlobals* sg, const MayaFluidData* data, const CMayaFluidData<true>* fluidData, const AtVector& lPt, const GradientDescription<T, M, G>& gradient, float texture)
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
         gradientValue = 1.f - 1.41421356f * AiV3Length(lPt - middlePoint);
         break;
      case GT_DENSITY:
         gradientValue = fluidData->readDensity(lPt, data->filterType);
         break;
      case GT_TEMPERATURE:
         gradientValue = fluidData->readTemperature(lPt, data->filterType);
         break;
      case GT_FUEL:
         gradientValue = fluidData->readFuel(lPt, data->filterType);
         break;
      case GT_PRESSURE:
         gradientValue = fluidData->readPressure(lPt, data->filterType);
         break;
      case GT_SPEED:
         gradientValue = 1.0f - 1.0f / (1.0f + AiV3Length(fluidData->readVelocity(lPt, data->filterType) * data->velocityScale));
         break;
      default:
         return GetDefaultValue<T>() * texture;
   }
   return gradient.GetValue(sg, gradientValue * texture);
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
         const float fac = powf(dist, 1.f - implode) / dist;
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
   {
      if (edgeDropoff < AI_EPSILON)
         return 0.0f;
      ret = (1.f - value - (1.f - 2.f * edgeDropoff)) / (2.f * edgeDropoff);
   }
   else
   {
      if (edgeDropoff > (1.0f - AI_EPSILON))
         return 0.0f;
      ret = (value - (2.f* (edgeDropoff - .5f))) / (1.f - 2.f * (edgeDropoff - .5f));
   }
	return CLAMP(ret, 0.f, 1.f);
}

inline
float CalculateDropoff(const CMayaFluidData<true>* data, const AtVector& lPt, int dropoffShape, float edgeDropoff, int filterType)
{
   if (dropoffShape == DS_OFF)
      return 1.f;
   AtVector cPt = (lPt - AI_V3_HALF) * 2.f;
   switch(dropoffShape)
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
            const float d = data->readFalloff(lPt, filterType);
            if (d > 0.f && edgeDropoff < 0.9999f)
               return powf(d, edgeDropoff / (1.f - edgeDropoff));
            else
               return 0.f;
         }
      default:
         return 1.f;
   }
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
         if (AiUDataGetNode("mtoa_fluid_data", &fluidDataContainer) && (fluidDataContainer != 0))
            fluidData = (CMayaFluidData<true>*)AiNodeGetLocalData(fluidDataContainer);
         data->fluidDataMap[sg->tid].insert(std::pair<AtNode*, CMayaFluidData<true>*>(sg->Op, fluidData));         
      }
      else
         fluidData = it->second;
      data->fluidDataCache[sg->tid].first = sg->Op;
      data->fluidDataCache[sg->tid].second = fluidData;
   }
   
   const AtVector lPt = fluidData->ConvertToLocalSpace(sg->Po);

   AtVector scaledDir;
   AiM4VectorByMatrixMult(&scaledDir, sg->Minv, &sg->Rd);

   float dropoff = CalculateDropoff(fluidData, lPt, data->dropoffShape, CLAMP(AiShaderEvalParamFlt(p_edge_dropoff), 0.0f, 1.0f), data->filterType)
                   * AiV3Length(scaledDir);

   if (data->textureDisabledInShadows && (sg->Rt & AI_RAY_SHADOW))
   {
      const float opacity = MAX(0.f, GetValue(sg, data, fluidData, lPt, data->opacityGradient, 1.0f)) * dropoff * AiShaderEvalParamFlt(p_shadow_opacity);
      AiShaderGlobalsSetVolumeAttenuation(sg, data->transparency * opacity);
      return;
   }
   
   float opacityNoise = 1.0f;
   float colorNoise = 1.f; // colors?
   float incandNoise = 1.f;
   const float old_area = sg->area;
   sg->area = 0.f;
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
      if ((data->coordinateMethod == CM_GRID) && !fluidData->coordinatesEmpty())
         P = fluidData->readCoordinates(lPt, data->filterType);
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

                     AiV3Create(v, tmp, 0, 0);
                     d.x = AiPerlin3(v);

                     AiV3Create(v, 0, tmp, 0);
                     d.y = AiPerlin3(v);

                     AiV3Create(v, 0, 0, tmp);
                     d.z = AiPerlin3(v);

                     AiV3Normalize(d, d);

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
         volumeNoise = MAX(1.f - volumeNoise, 0.f);
      if (data->colorTexture)
         colorNoise = data->colorTexGain * volumeNoise;
      if (data->incandTexture)
         incandNoise = data->incandTexGain * volumeNoise;
      if (data->opacityTexture)
         opacityNoise *= data->opacityTexGain * volumeNoise;
   }
   sg->area = old_area;
   
   if (sg->Rt & AI_RAY_SHADOW)
   {
      const float opacity = MAX(0.f, GetValue(sg, data, fluidData, lPt, data->opacityGradient, opacityNoise)) * dropoff * AiShaderEvalParamFlt(p_shadow_opacity);
      AiShaderGlobalsSetVolumeAttenuation(sg, data->transparency * opacity);
      return;
   }
   
   const AtRGB opacity = MAX(0.f, GetValue(sg, data, fluidData, lPt, data->opacityGradient, opacityNoise)) * dropoff * data->transparency;
   AtRGB color = AI_RGB_BLACK;
   if (fluidData->colorGridEmpty())
      color = GetValue(sg, data, fluidData, lPt, data->colorGradient, colorNoise);
   else
      color = fluidData->readColors(lPt, data->filterType);

   color.r = MAX(0.f, color.r);
   color.g = MAX(0.f, color.g);
   color.b = MAX(0.f, color.b);
   AtRGB incandescence = GetValue(sg, data, fluidData, lPt, data->incandescenceGradient, incandNoise);
   incandescence.r = MAX(0.f, incandescence.r);
   incandescence.g = MAX(0.f, incandescence.g);
   incandescence.b = MAX(0.f, incandescence.b);
   
   AiShaderGlobalsSetVolumeAttenuation(sg, opacity * AI_RGB_WHITE);
   AiShaderGlobalsSetVolumeEmission(sg, opacity * incandescence);
   AiShaderGlobalsSetVolumeScattering(sg, opacity * color, CLAMP(AiShaderEvalParamFlt(p_phase_func), -1.0f, 1.0f));
}
