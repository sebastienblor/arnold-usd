#include "MayaFluidData.h"

const char* contentsGradientTypeEnums[] = {"Constant", "X Gradient", "Y Gradient", "Z Gradient",
                                           "-X Gradient", "-Y Gradient", "-Z Gradient",
                                           "Center Gradient", 0};

const char* contentsMethodEnums[] = {"Grid", "Gradient", 0};

template <typename T>
void ReadArray(AtArray* array, int cm, int cmg, int numVoxels, ArrayDescription<T>& arrayDesc)
{
   arrayDesc.release();

   if (cm == CSM_GRID)
   {
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
      arrayDesc.isGradient = false;
   }
   else
   {
      arrayDesc.isGradient = true;
      arrayDesc.gradientType = cmg;
   }
}

CMayaFluidData::CMayaFluidData(AtNode* node)
{
   xres = AiNodeGetInt(node, "xres");
   yres = AiNodeGetInt(node, "yres");
   zres = AiNodeGetInt(node, "zres");

   const int numVoxels = xres * yres * zres;
   
   if (numVoxels == 0)
   {
      AiMsgWarning("[aiMayaFluid] The resolution of the fluid grid is zero!");
      return;
   }

   dmin = AiNodeGetVec(node, "min");
   dmax = AiNodeGetVec(node, "max");
   dmax = dmax - dmin;
   dmax.x = 1.f / dmax.x;
   dmax.y = 1.f / dmax.y;
   dmax.z = 1.f / dmax.z;

   ReadArray(AiNodeGetArray(node, "density"), AiNodeGetInt(node, "density_method"), AiNodeGetInt(node, "density_gradient"), numVoxels, density);
   ReadArray(AiNodeGetArray(node, "fuel"), AiNodeGetInt(node, "fuel_method"), AiNodeGetInt(node, "fuel_gradient"), numVoxels, fuel);
   ReadArray(AiNodeGetArray(node, "temperature"), AiNodeGetInt(node, "temperature_method"), AiNodeGetInt(node, "temperature_gradient"), numVoxels, temperature);
   ReadArray(AiNodeGetArray(node, "pressure"), CSM_GRID, CG_CONSTANT, numVoxels, pressure);
   ReadArray(AiNodeGetArray(node, "velocity"), AiNodeGetInt(node, "velocity_method"), AiNodeGetInt(node, "velocity_gradient"), numVoxels, velocity);
   ReadArray(AiNodeGetArray(node, "colors"), CSM_GRID, CG_CONSTANT, numVoxels, colors);
   ReadArray(AiNodeGetArray(node, "coordinates"), CSM_GRID, CG_CONSTANT, numVoxels, coordinates);
   ReadArray(AiNodeGetArray(node, "falloff"), CSM_GRID, CG_CONSTANT, numVoxels, falloff);
}

CMayaFluidData::~CMayaFluidData()
{
   density.release();
   fuel.release();
   temperature.release();
   pressure.release();
   velocity.release();
   colors.release();
   coordinates.release();
}

AtVector CMayaFluidData::ConvertToLocalSpace(const AtVector& cPt) const
{
   AtVector lPt;
   lPt = (cPt - dmin) * dmax;
   return lPt;
}


template <typename T>
T GetConstantValue()
{
   return 1;
}

template <>
float GetConstantValue<float>()
{
   return 1.f;
}

template <>
AtRGB GetConstantValue<AtRGB>()
{
   return AI_RGB_WHITE;
}

template <>
AtVector GetConstantValue<AtVector>()
{
   return AI_V3_ONE;
}

template <typename T>
T ConvertFloat(float f)
{
   return f;
}

template<>
AtRGB ConvertFloat<AtRGB>(float f)
{
   AtRGB ret = {f, f, f};
   return ret;
}

template<>
AtVector ConvertFloat<AtVector>(float f)
{
   AtVector ret = {f, f, f};
   return ret;
}

template <typename T>
inline T MonotonicCubicInterpolant(const T& f1, const T& f2, const T& f3, const T& f4,
                                   float t)
{
   return GetDefaultValue<T>();
}

template <>
inline float MonotonicCubicInterpolant<float>(const float& f1, const float& f2, const float& f3, const float& f4, 
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
inline AtRGB MonotonicCubicInterpolant(const AtRGB& f1, const AtRGB& f2, const AtRGB& f3, const AtRGB& f4,
                                       float t)
{
   AtRGB ret = {MonotonicCubicInterpolant(f1.r, f2.r, f3.r, f4.r, t),
      MonotonicCubicInterpolant(f1.g, f2.g, f3.g, f4.g, t),
      MonotonicCubicInterpolant(f1.b, f2.b, f3.b, f4.b, t)
   };
   return ret;
}

template <>
inline AtVector MonotonicCubicInterpolant(const AtVector& f1, const AtVector& f2, const AtVector& f3, const AtVector& f4,
                                          float t)
{
   AtVector ret = {MonotonicCubicInterpolant(f1.x, f2.x, f3.x, f4.x, t),
      MonotonicCubicInterpolant(f1.y, f2.y, f3.y, f4.y, t),
      MonotonicCubicInterpolant(f1.z, f2.z, f3.z, f4.z, t)
   };
   return ret;
}

template <typename T>
inline T Filter(const AtVector& lPt, const ArrayDescription<T>& arrayDesc, int filterType, int xres, int yres, int zres)
{
   static const AtVector middlePoint = {0.5f, 0.5f, 0.5f};
   if (arrayDesc.isGradient)
   {
      switch(arrayDesc.gradientType)
      {
         case CG_CONSTANT:
            return GetConstantValue<T>();
         case CG_X_GRADIENT:
            return ConvertFloat<T>(1.f - lPt.x);
         case CG_Y_GRADIENT:
            return ConvertFloat<T>(1.f - lPt.y);
         case CG_Z_GRADIENT:
            return ConvertFloat<T>(1.f - lPt.z);
         case CG_NX_GRADIENT:
            return ConvertFloat<T>(lPt.x);
         case CG_NY_GRADIENT:
            return ConvertFloat<T>(lPt.y);
         case CG_NZ_GRADIENT:
            return ConvertFloat<T>(lPt.z);
         case CG_CENTER_GRADIENT:
            return ConvertFloat<T>(1.f - 1.41421356f * AiV3Length(lPt - middlePoint));
         default:
            return GetDefaultValue<T>();
      }

   }
   else if (filterType == FT_CLOSEST)
   {
      if (arrayDesc.data == 0)
         return GetDefaultValue<T>();
      if (arrayDesc.single)
         return *arrayDesc.data;
      // position in the voxel grid
      const AtVector fc = {lPt.x * (float)xres, lPt.y * (float)yres, lPt.z * (float)zres};
      // voxel coordiantes
      const int lcx = CLAMP((int)fc.x, 0, xres - 1);
      const int lcy = CLAMP((int)fc.y, 0, yres - 1);
      const int lcz = CLAMP((int)fc.z, 0, zres - 1);
      return arrayDesc.data[lcx + lcy * xres + lcz * xres * yres];
   }
   else if (filterType == FT_LINEAR)
   {
      if (arrayDesc.data == 0)
         return GetDefaultValue<T>();
      if (arrayDesc.single)
         return *arrayDesc.data;
      // position in the voxel grid
      const AtVector fc = {lPt.x * (float)xres - .5f, lPt.y * (float)yres - .5f, lPt.z * (float)zres - .5f};

      // lower voxel coordiantes
      const int lcx = CLAMP((int)fc.x, 0, xres - 1);
      const int lcy = CLAMP((int)fc.y, 0, yres - 1);
      const int lcz = CLAMP((int)fc.z, 0, zres - 1);

      // higher voxel coordinates
      const int hcx = MIN(lcx + 1, xres - 1);
      const int hcy = MIN(lcy + 1, yres - 1);
      const int hcz = MIN(lcz + 1, zres - 1);

      // weight for the lower coordinates
      const AtVector pc = {fc.x - (float)lcx, fc.y - (float)lcy, fc.z - (float)lcz};
      // weight for the upper coordinates
      const AtVector npc = {1.f - pc.x, 1.f - pc.y, 1.f - pc.z};
      const int xyres = xres * yres;
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
      const AtVector fc = {lPt.x * (float)xres - .5f, lPt.y * (float)yres - .5f, lPt.z * (float)zres - .5f};
      // lower voxel coordiantes
      const int t2x = CLAMP((int)fc.x, 0, xres - 1);
      const int t2y = CLAMP((int)fc.y, 0, yres - 1);
      const int t2z = CLAMP((int)fc.z, 0, zres - 1);
      
      const AtVector t = {fc.x - (float)t2x, fc.y - (float)t2y, fc.z - (float)t2z};
      
      const int t1x = MAX(t2x - 1, 0);
      const int t1y = MAX(t2y - 1, 0);
      const int t1z = MAX(t2z - 1, 0);
      
      const int t3x = MIN(t2x + 1, xres - 1);
      const int t3y = MIN(t2y + 1, yres - 1);
      const int t3z = MIN(t2z + 1, zres - 1);
      
      const int t4x = MIN(t3x + 1, xres - 1);
      const int t4y = MIN(t3y + 1, yres - 1);
      const int t4z = MIN(t3z + 1, zres - 1);
      const int xyres = xres * yres;
      
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

float CMayaFluidData::readDensity(const AtVector& lPt, int filterType) const
{
   return Filter(lPt, density, filterType, xres, yres, zres);
}

float CMayaFluidData::readTemperature(const AtVector& lPt, int filterType) const
{
   return Filter(lPt, temperature, filterType, xres, yres, zres);
}

float CMayaFluidData::readFuel(const AtVector& lPt, int filterType) const
{
   return Filter(lPt, fuel, filterType, xres, yres, zres);
}

float CMayaFluidData::readPressure(const AtVector& lPt, int filterType) const
{
   return Filter(lPt, pressure, filterType, xres, yres, zres);
}

AtVector CMayaFluidData::readVelocity(const AtVector& lPt, int filterType) const
{
   return Filter(lPt, velocity, filterType, xres, yres, zres);
}

AtRGB CMayaFluidData::readColors(const AtVector& lPt, int filterType) const
{
   return Filter(lPt, colors, filterType, xres, yres, zres);
}

AtVector CMayaFluidData::readCoordinates(const AtVector& lPt, int filterType) const
{
   return Filter(lPt, coordinates, filterType, xres, yres, zres);
}

float CMayaFluidData::readFalloff(const AtVector& lPt, int filterType) const
{
   return Filter(lPt, falloff, filterType, xres, yres, zres);
}

bool CMayaFluidData::coordinatesEmpty() const
{
   return coordinates.data == 0;
}

void CMayaFluidData::InitializeShaderParameters(AtList* params)
{
	AiParameterInt("xres", 0);
   AiParameterInt("yres", 0);
   AiParameterInt("zres", 0);
   
   AiParameterVec("min", 0.f, 0.f, 0.f);
   AiParameterVec("max", 0.f, 0.f, 0.f);
   
   AiParameterEnum("density_method", CSM_GRADIENT, contentsMethodEnums);
   AiParameterEnum("density_gradient", CG_CONSTANT, contentsGradientTypeEnums);
   AiParameterArray("density", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));

   AiParameterEnum("fuel_method", CSM_GRADIENT, contentsMethodEnums);
   AiParameterEnum("fuel_gradient", CG_CONSTANT, contentsGradientTypeEnums);
   AiParameterArray("fuel", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));

   AiParameterEnum("temperature_method", CSM_GRADIENT, contentsMethodEnums);
   AiParameterEnum("temperature_gradient", CG_CONSTANT, contentsGradientTypeEnums);
   AiParameterArray("temperature", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));

   AiParameterArray("pressure", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));

   AiParameterEnum("velocity_method", CSM_GRADIENT, contentsMethodEnums);
   AiParameterEnum("velocity_gradient", CG_CONSTANT, contentsGradientTypeEnums);
   AiParameterArray("velocity", AiArrayAllocate(0, 1, AI_TYPE_VECTOR));
   
   AiParameterArray("colors", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   
   AiParameterArray("coordinates", AiArrayAllocate(0, 1, AI_TYPE_VECTOR));
   
   AiParameterArray("falloff", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
}

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidDataMtd);

node_parameters
{
   CMayaFluidData::InitializeShaderParameters(params);
}

node_initialize
{
   AiNodeSetLocalData(node, 0);
}

node_update
{
   CMayaFluidData* data = (CMayaFluidData*)AiNodeGetLocalData(node);
   if (data != 0)
      delete data;
   data = new CMayaFluidData(node);
   AiNodeSetLocalData(node, data);
}

node_finish
{
   CMayaFluidData* data = (CMayaFluidData*)AiNodeGetLocalData(node);
   if (data != 0)
      delete data;
}

shader_evaluate
{

}
