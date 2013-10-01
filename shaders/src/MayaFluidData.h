#pragma once

#include <ai.h>

enum contentsMethod{
   CSM_GRID = 0,
   CSM_GRADIENT
};

enum contentsGradientType{
   CG_CONSTANT = 0,
   CG_X_GRADIENT,
   CG_Y_GRADIENT,
   CG_Z_GRADIENT,
   CG_NX_GRADIENT,
   CG_NY_GRADIENT,
   CG_NZ_GRADIENT,
   CG_CENTER_GRADIENT
};

enum filterType{
   FT_CLOSEST = 0,
   FT_LINEAR,
   FT_CUBIC
};

template<typename T>
struct ArrayDescription{
   T* data;
   int gradientType;
   bool single;
   bool isGradient;

   ArrayDescription() : data(0) {}
   
   void release() {if(data) AiFree(data); data = 0;}   
};

template <typename T>
inline T GetDefaultValue()
{
   return 0;
}

template <>
inline float GetDefaultValue<float>()
{
   return 0.f;
}

template <>
inline AtRGB GetDefaultValue<AtRGB>()
{
   return AI_RGB_BLACK;
}

template <>
inline AtVector GetDefaultValue<AtVector>()
{
   return AI_V3_ZERO;
}

template <typename T>
inline T ReadFromArray(AtArray* array, int element)
{
   return GetDefaultValue<T>();
}

template <>
inline float ReadFromArray(AtArray* array, int element)
{
   return AiArrayGetFlt(array, element);
}

template <>
inline AtRGB ReadFromArray(AtArray* array, int element)
{
   return AiArrayGetRGB(array, element);
}

template <>
inline AtVector ReadFromArray(AtArray* array, int element)
{
   return AiArrayGetVec(array, element);
}

class CMayaFluidData{
public:
	static void InitializeShaderParameters(AtList* params); // this is required so we can keep backwards compatibility for a while

   CMayaFluidData(AtNode* node);

   ~CMayaFluidData();

   static void* operator new(size_t size)
   {
      return AiMalloc((unsigned long)size);
   }
   
   static void operator delete(void* p)
   {
      AiFree(p);
   }

   inline AtVector ConvertToLocalSpace(const AtVector& cPt) const;

   inline float readDensity(const AtVector& lPt, int filterType) const;
   inline float readTemperature(const AtVector& lPt, int filterType) const;
   inline float readFuel(const AtVector& lPt, int filterType) const;
   inline float readPressure(const AtVector& lPt, int filterType) const;
   inline AtVector readVelocity(const AtVector& lPt, int filterType) const;
   inline AtRGB readColors(const AtVector& lPt, int filterType) const;
   inline AtVector readCoordinates(const AtVector& lPt, int filterType) const;
   inline float readFalloff(const AtVector& lPt, int filterType) const;

   inline bool coordinatesEmpty() const;
   inline bool colorGridEmpty() const;
private:
   int xres, yres, zres;
   AtVector dmin, dmax;

   ArrayDescription<float> density;
   ArrayDescription<float> fuel;
   ArrayDescription<float> temperature;
   ArrayDescription<float> pressure;
   ArrayDescription<AtVector> velocity;
   ArrayDescription<AtRGB> colors;
   ArrayDescription<AtVector> coordinates;
   ArrayDescription<float> falloff;
};

AtVector CMayaFluidData::ConvertToLocalSpace(const AtVector& cPt) const
{
   AtVector lPt;
   lPt = (cPt - dmin) * dmax;
   return lPt;
}

template <typename T>
inline T GetConstantValue()
{
   return 1;
}

template <>
inline float GetConstantValue<float>()
{
   return 1.f;
}

template <>
inline AtRGB GetConstantValue<AtRGB>()
{
   return AI_RGB_WHITE;
}

template <>
inline AtVector GetConstantValue<AtVector>()
{
   return AI_V3_ONE;
}

template <typename T>
inline T ConvertFloat(float f)
{
   return f;
}

template<>
inline AtRGB ConvertFloat<AtRGB>(float f)
{
   AtRGB ret = {f, f, f};
   return ret;
}

template<>
inline AtVector ConvertFloat<AtVector>(float f)
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

bool CMayaFluidData::colorGridEmpty() const
{
   return colors.data == 0;
}
