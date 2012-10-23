#include <ai.h>

#include <memory.h>
#include <cmath>

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

node_parameters
{
   AiParameterRGB("color", 1.f, 1.f, 1.f);
   
   AiParameterFlt("step_size", 0.1f);
   
   AiParameterInt("xres", 0);
   AiParameterInt("yres", 0);
   AiParameterInt("zres", 0);
   
   AiParameterFlt("xdim", 0.f);
   AiParameterFlt("ydim", 0.f);
   AiParameterFlt("zdim", 0.f);
   
   AiParameterArray("density", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("fuel", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("temperature", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   AiParameterArray("pressure", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
   
   AiParameterArray("velocity", AiArrayAllocate(0, 1, AI_TYPE_VECTOR));
   
   AiParameterArray("colors", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   
   AiParameterArray("matrix", AiArrayAllocate(0, 1, AI_TYPE_MATRIX));
   
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
   
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiMayaFluid");
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D1E);
}

enum MayaFluidParams{
   p_color=0,
   
   p_xres,
   p_yres,
   p_zres,

   p_xdim,
   p_ydim,
   p_zdim,
   
   p_density,
   p_fuel,
   p_temperature,
   p_pressure,
   p_colors,
   
   p_matrix,
   
   p_color_gradient_type,
   p_color_gradient,
   p_color_gradient_input_bias,
   
   p_incandescence_gradient_type,
   p_incandescence_gradient,
   p_incandescence_gradient_input_bias,
   
   p_opacity_gradient_type,
   p_opacity_gradient,
   p_opacity_gradient_input_bias,
};

template<typename T>
struct GradientDescription{
   T* data;
   float inputBias;
   int type;
   int resolution;
   
   void release() {if(data) AiFree(data);}
};

template<typename T>
struct ArrayDescription{
   T* data;
   bool single;
   
   void release() {if(data) AiFree(data);}   
};

struct MayaFluidData{
   int xres, yres, zres;
   float xdim, ydim, zdim;
   float stepSize;
   
   ArrayDescription<float> density;
   ArrayDescription<float> fuel;
   ArrayDescription<float> temperature;
   ArrayDescription<float> pressure;
   ArrayDescription<AtVector> velocity;
   ArrayDescription<AtRGB> colors;
   
   GradientDescription<AtRGB> colorGradient;
   GradientDescription<AtRGB> incandescenceGradient;
   GradientDescription<float> opacityGradient;  
   
   AtMatrix worldMatrix;
   AtMatrix inverseWorldMatrix;
};

node_initialize
{
   MayaFluidData* data = (MayaFluidData*)AiMalloc(sizeof(MayaFluidData));
   AiNodeSetLocalData(node, data);
   
   memset(data, sizeof(MayaFluidData), 0); // setting all of the values to zero
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
   
   data->stepSize = AiNodeGetFlt(node, "step_size");
   
   const int numVoxels = data->xres * data->yres * data->zres;
   
   if (numVoxels == 0)
   {
      AiMsgWarning("[aiMayaFluid] The resolution of the fluid grid is zero!");
      return;
   }
   
   data->xdim = AiNodeGetFlt(node, "xdim");
   data->ydim = AiNodeGetFlt(node, "ydim");
   data->zdim = AiNodeGetFlt(node, "zdim");
   
   ReadArray(node, "density", numVoxels, data->density);
   ReadArray(node, "fuel", numVoxels, data->fuel);
   ReadArray(node, "temperature", numVoxels, data->temperature);
   ReadArray(node, "pressure", numVoxels, data->pressure);
   ReadArray(node, "velocity", numVoxels, data->velocity);
   ReadArray(node, "colors", numVoxels, data->colors);
   
   data->colorGradient.type = AiNodeGetInt(node, "color_gradient_type");
   data->colorGradient.inputBias = AiNodeGetFlt(node, "color_gradient_input_bias");
   ReadGradient(node, "color_gradient", data->colorGradient);
   data->incandescenceGradient.type = AiNodeGetInt(node, "incandescence_gradient_type");
   data->incandescenceGradient.inputBias = AiNodeGetFlt(node, "incandescence_gradient_input_bias");
   ReadGradient(node, "incandescence_gradient", data->incandescenceGradient);
   data->opacityGradient.type = AiNodeGetInt(node, "opacity_gradient_type");
   data->opacityGradient.inputBias = AiNodeGetFlt(node, "opacity_gradient_input_bias");
   ReadGradient(node, "opacity_gradient", data->opacityGradient);

   AtArray* matrixArray = AiNodeGetArray(node, "matrix");
   
   if (matrixArray->nelements != 0)
   { // only using the first matrix atm
      AiArrayGetMtx(matrixArray, 0, data->worldMatrix);
      AiM4Invert(data->worldMatrix, data->inverseWorldMatrix);
   }
   else
   {
      AiM4Identity(data->worldMatrix);
      AiM4Identity(data->inverseWorldMatrix);
      AiMsgWarning("[aiMayaFluid] The matrix array is empty!");
   }
}

node_finish
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   data->density.release();
   data->fuel.release();
   data->temperature.release();
   data->colors.release();
   
   data->colorGradient.release();
   data->incandescenceGradient.release();
   data->opacityGradient.release();
   
   AiFree(data);
}

template<typename T>
T GetFilteredValue(MayaFluidData* data, const AtVector& lPt, const ArrayDescription<T>& arrayDesc) // simple linear interpolation
{
   if (arrayDesc.data == 0)
      return GetDefaultValue<T>();
   if (arrayDesc.single)
      return *arrayDesc.data;   
   float fcx = lPt.x * (float)data->xres;
   float fcy = lPt.y * (float)data->yres;
   float fcz = lPt.z * (float)data->zres;
   
   int lcx = CLAMP((int)floorf(fcx), 0, data->xres - 1);
   int lcy = CLAMP((int)floorf(fcy), 0, data->yres - 1);
   int lcz = CLAMP((int)floorf(fcz), 0, data->zres - 1);
   
   int hcx = MIN(lcx + 1, data->xres - 1);
   int hcy = MIN(lcy + 1, data->yres - 1);
   int hcz = MIN(lcz + 1, data->zres - 1);
   
   float pcx = fcx - (float)lcx;
   float pcy = fcy - (float)lcy;
   float pcz = fcz - (float)lcz;
   
   float npcx = 1.f - pcx;
   float npcy = 1.f - pcy;
   float npcz = 1.f - pcz;
   int xmy = data->xres * data->yres;
   int c000 = lcx + lcy * data->xres + lcz * xmy;
   int c010 = lcx + hcy * data->xres + lcz * xmy;
   int c001 = lcx + lcy * data->xres + hcz * xmy;
   int c011 = lcx + hcy * data->xres + hcz * xmy;
   int c100 = hcx + lcy * data->xres + lcz * xmy;
   int c110 = hcx + hcy * data->xres + lcz * xmy;
   int c101 = hcx + lcy * data->xres + hcz * xmy;
   int c111 = hcx + hcy * data->xres + hcz * xmy;
   
   // TODO : rearrange the multiplications to save some code
   return (arrayDesc.data[c000] * npcx * npcy + arrayDesc.data[c110] * pcx * pcy +
           arrayDesc.data[c100] * pcx * npcy + arrayDesc.data[c010] * npcx * pcy) * npcz +
           (arrayDesc.data[c001] * npcx * npcy + arrayDesc.data[c111] * pcx * pcy +
           arrayDesc.data[c101] * pcx * npcy + arrayDesc.data[c011] * npcx * pcy) * pcz;   
}

template <typename T>
T GetGradientValue(const GradientDescription<T>& gradient, const float& v)
{
   if (gradient.resolution == 0)
      return GetDefaultValue<T>();
   float p = v * gradient.resolution;
   float pf = floorf(p);
   int b = CLAMP((int)pf, 0, gradient.resolution - 1);
   int e = MIN(b + 1, gradient.resolution - 1);
   pf = p - pf;
   return gradient.data[b] * (1.f - pf) + gradient.data[e] * pf;
}

AtVector ConvertToLocalSpace(MayaFluidData* data, const AtVector& cPt)
{
   AtVector lPt;
   lPt.x = cPt.x + data->xdim / 2;
   lPt.y = cPt.y + data->ydim / 2;
   lPt.z = cPt.z + data->zdim / 2;
   lPt.x = CLAMP(lPt.x / data->xdim, 0.f, 1.f);
   lPt.y = CLAMP(lPt.y / data->ydim, 0.f, 1.f);
   lPt.z = CLAMP(lPt.z / data->zdim, 0.f, 1.f);
   return lPt;
}

template <typename T>
T GetValue(MayaFluidData* data, const AtVector& lPt, GradientDescription<T>& gradient)
{
   static const AtVector middlePoint = {0.5f, 0.5f, 0.5f};
   switch (gradient.type)
   {
      case GT_CONSTANT:
         return GetGradientValue(gradient, 1.f + gradient.inputBias);
      case GT_X_GRADIENT:
         return GetGradientValue(gradient, 1.f - lPt.x + gradient.inputBias);
      case GT_Y_GRADIENT:
         return GetGradientValue(gradient, 1.f - lPt.y + gradient.inputBias);
      case GT_Z_GRADIENT:
         return GetGradientValue(gradient, 1.f - lPt.z + gradient.inputBias);
      case GT_CENTER_GRADIENT:
         return GetGradientValue(gradient, 1.0f - AiV3Length(lPt - middlePoint) + gradient.inputBias);
      case GT_DENSITY:
         return GetGradientValue(gradient, GetFilteredValue(data, lPt, data->density) + gradient.inputBias);
      case GT_TEMPERATURE:
         return GetGradientValue(gradient, GetFilteredValue(data, lPt, data->temperature) + gradient.inputBias);
      case GT_FUEL:
         return GetGradientValue(gradient, GetFilteredValue(data, lPt, data->fuel) + gradient.inputBias);
      case GT_PRESSURE:
         return GetGradientValue(gradient, GetFilteredValue(data, lPt, data->pressure) + gradient.inputBias);
      case GT_SPEED:
         return GetGradientValue(gradient, AiV3Length(GetFilteredValue(data, lPt, data->velocity)) + gradient.inputBias);
      default:
         return GetDefaultValue<T>();
   }
}

shader_evaluate
{
   MayaFluidData* data = (MayaFluidData*)AiNodeGetLocalData(node);
   
   AtVector lRo;
   AiM4PointByMatrixMult(&lRo, data->inverseWorldMatrix, &sg->Ro);
   const AtVector lPt = ConvertToLocalSpace(data, lRo);
   
   const float opacity = CLAMP(GetValue(data, lPt, data->opacityGradient) * (float)sg->Rl, 0.f, 1.f); 
   
   if (sg->Rt & AI_RAY_SHADOW) // only access the opacity here
   {
      sg->out.RGB = (1.f - opacity) * sg->Ci;
      return;
   }
   
   const AtRGB color = GetValue(data, lPt, data->colorGradient);
   const AtRGB incandescence = GetValue(data, lPt, data->incandescenceGradient);
   
   const AtPoint oldP = sg->P;
   const bool oldFHemi = sg->fhemi;
   
   sg->P = sg->Ro + sg->Rd * (float)sg->Rl * 0.5f;
   sg->fhemi = false;
   
   AiLightsPrepare(sg);
   AtRGB shading = AI_RGB_BLACK;
   while (AiLightsGetSample(sg))
      shading += sg->Li * sg->we / (4.f * (float)AI_PI);

   
   sg->P = oldP;
   sg->fhemi = oldFHemi;
   
   sg->Vo = (color * shading + incandescence) * opacity;
   sg->out.RGB = sg->Ci * (1.f - opacity) + sg->Vo;   
}
