#pragma once

#include <ai.h>
#include <sstream>
#include <algorithm>

#include "MayaFluidData.h"

template <typename T>
inline T GetValueFromSG(AtShaderGlobals* sg, int outputType = AI_TYPE_RGB)
{
   return GetDefaultValue<T>();
}

template <>
inline float GetValueFromSG(AtShaderGlobals* sg, int outputType)
{
   return sg->out.FLT();
}

template <>
inline AtRGB GetValueFromSG(AtShaderGlobals* sg, int outputType)
{
   AtRGB ret = AI_RGB_BLACK;
   switch (outputType)
   {
   case AI_TYPE_FLOAT:
      ret = sg->out.FLT();
      break;
   case AI_TYPE_VECTOR2:
      ret.r = sg->out.VEC2().x;
      ret.g = sg->out.VEC2().y;
      break;
   default:
      ret = sg->out.RGB();
   }   
   return ret;
}

enum gradientInterps{
   GI_NONE = 0,
   GI_LINEAR,
   GI_SMOOTH,
   GI_SPLINE
};


inline void AiColorGamma(AtColor *color, float gamma)
{
   if (gamma == 1.0f)
      return;
   gamma = 1.0f / gamma;
   color->r = powf(color->r, gamma);
   color->g = powf(color->g, gamma);
   color->b = powf(color->b, gamma);
}

template <typename T>
inline void GammaCorrect(T& d, float gamma)
{
   
}

template <>
inline void GammaCorrect<AtRGB>(AtRGB& d, float gamma)
{
   if (gamma != 1.f)
   {
      d.r = AiMax(0.f, d.r);
      d.g = AiMax(0.f, d.g);
      d.b = AiMax(0.f, d.b);
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
   uint32_t nelements;
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
   
   inline T GetElement(AtShaderGlobals* sg, uint32_t elem) const
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
         const int e = AiMin(b + 1, resolution - 1);
         const float pf = p - (float)pi;
         return data[b] * (1.f - pf) + data[e] * pf;
      }
      
      if (elements == 0)
         return GetDefaultValue<T>();
      if (nelements == 1)
         return GetElement(sg, 0);
      
      v = ApplyBias(v, inputBias);
      
      // look for the proper segment
      uint32_t index = nelements;
      for (uint32_t i = 0; i < nelements; ++i)
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
      const uint32_t prevIndex = index - 1;
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
               const float tx = AiMax(tanSize * dp, AI_EPSILON);

               float sx = AiMax(p2 - p0, AI_EPSILON);
               T sy = v2 - v0;

               sy *= tanSize * dp / sx;
               const T m1 = sy / tx;
               sx = AiMax(p3 - p1, AI_EPSILON);
               sy = v3 - v1;

               sy *= tanSize * dp / sx;
               const T m2 = sy / tx;

               float tFromP1 = (v - p1);
               float length = AiMin(1.f / (dp * dp), AI_BIG);
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
      
      nelements = AiArrayGetNumElements(positionsArray);
      if (nelements == 0)
         elements = 0;
      else
      {
         bool isConnected = false;
         elements = (GradientDescriptionElement*)AiMalloc(sizeof(GradientDescriptionElement) * nelements);
         for (uint32_t i = 0; i < nelements; ++i)
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


template <typename T, bool M, bool G, bool IS3D>
T GetValue(AtShaderGlobals* sg, const CMayaFluidData<IS3D>* fluidData, const AtVector& lPt, const GradientDescription<T, M, G>& gradient, int filterType, float texture, const AtVector& velocityScale)
{
   static const AtVector middlePoint(0.5f, 0.5f, 0.5f);
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
         gradientValue = fluidData->readDensity(lPt, filterType);
         break;
      case GT_TEMPERATURE:
         gradientValue = fluidData->readTemperature(lPt, filterType);
         break;
      case GT_FUEL:
         gradientValue = fluidData->readFuel(lPt, filterType);
         break;
      case GT_PRESSURE:
         gradientValue = fluidData->readPressure(lPt, filterType);
         break;
      case GT_SPEED:
         gradientValue = 1.0f - 1.0f / (1.0f + AiV3Length(fluidData->readVelocity(lPt, filterType) * velocityScale)); // todo fix this later
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

template <bool IS3D>
inline float CalculateDropoff(const CMayaFluidData<IS3D>* data, const AtVector& lPt, int dropoffShape, float edgeDropoff, int filterType)
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
         cPt.x = (1.f - fabsf(cPt.x)) / edgeDropoff;
         cPt.y = (1.f - fabsf(cPt.y)) / edgeDropoff;
         cPt.z = (1.f - fabsf(cPt.z)) / edgeDropoff;
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
            const float d = -2.f * sqrtf(cPt.x * cPt.x + cPt.y * cPt.y) + fabsf(cPt.z + 1.f);
            if (d < 0.f)
               return 0.f;
            else if (d < edgeDropoff)
               return 0.5f * sinf((float)AI_PI * (d / edgeDropoff - .5f)) + .5f;
            else
               return 1.f;
         }
      case DS_DOUBLE_CONE:         
         {
            const float d = -2.f * sqrtf(cPt.x * cPt.x + cPt.y * cPt.y) + fabsf(cPt.z);
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
