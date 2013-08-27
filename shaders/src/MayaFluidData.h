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

   AtVector ConvertToLocalSpace(const AtVector& cPt);

   float readDensity(const AtVector& lPt, int filterType);
   float readTemperature(const AtVector& lPt, int filterType);
   float readFuel(const AtVector& lPt, int filterType);
   float readPressure(const AtVector& lPt, int filterType);
   AtVector readVelocity(const AtVector& lPt, int filterType);
   AtRGB readColors(const AtVector& lPt, int filterType);
   AtVector readCoordinates(const AtVector& lPt, int filterType);
   float readFalloff(const AtVector& lPt, int filterType);

   bool coordinatesEmpty();
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
