#pragma once

#include <ai.h>
#include <sstream>
#include <algorithm>


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
inline void BfPrintValue(const T& v)
{
}


template <>
inline void BfPrintValue(const float &v)
{
   std::cerr<<v<<std::endl;
}
template <>
inline void BfPrintValue(const AtRGB &v)
{
   std::cerr<<v.r<<" "<<v.g<<" "<<v.b<<std::endl;
}



template <typename T>
inline T ReadFromArray(AtArray* array, int element)
{
   return GetDefaultValue<T>();
}

template <>
inline AtRGB ReadFromArray(AtArray* array, int element)
{
   return AiArrayGetRGB(array, element);
}


template <>
inline float ReadFromArray(AtArray* array, int element)
{
   return AiArrayGetFlt(array, element);
}


template <>
inline AtVector ReadFromArray(AtArray* array, int element)
{
   return AiArrayGetVec(array, element);
}

template <typename T>
inline T BfGradientGetValueFromSG(AtShaderGlobals* sg, int outputType = AI_TYPE_RGB)
{
   return GetDefaultValue<T>();
}

template <>
inline float BfGradientGetValueFromSG(AtShaderGlobals* sg, int outputType)
{
   return sg->out.FLT;
}

template <>
inline AtRGB BfGradientGetValueFromSG(AtShaderGlobals* sg, int outputType)
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

template<typename T>
struct GradientDescriptionElement{
   AtNode* node;
   T value;
   float position;
   int interp;
   int outputType;
};

template<typename T>
static bool CompareElements(const GradientDescriptionElement<T>& a,
                            const GradientDescriptionElement<T>& b)
{
   return a.position < b.position;
}

template<typename T>
inline T BfGradientGetElements(AtShaderGlobals* sg, AtUInt32 elem, GradientDescriptionElement<T> *elements)
{

   if (elements[elem].node != 0)
   {
      AiShaderEvaluate(elements[elem].node, sg);
      return BfGradientGetValueFromSG<T>(sg, elements[elem].outputType);
   }
   
   return elements[elem].value;
}


template <typename T>
T BfGradientGetValue(AtShaderGlobals* sg, float v, GradientDescriptionElement<T>* elements, AtUInt32 nelements, T*data, int resolution, AtNode *points_op)
{
   T ret = GetDefaultValue<T>();
   if (data != 0 && sg != 0 && sg->Op != 0 && sg->Op == points_op) // only use baked data is it's the correct geometry
   {
      const float p = v * resolution;
      const int pi = (int)p;
      const int b = CLAMP(pi, 0, resolution - 1);
      const int e = MIN(b + 1, resolution - 1);
      const float pf = p - (float)pi;

      //T dd = data[b] * (1.f - pf) + data[e] * pf;
      
      return data[b] * (1.f - pf) + data[e] * pf;
   }
   
   if (elements == 0)
      return GetDefaultValue<T>();
   if (nelements == 1)
      return BfGradientGetElements<T>(sg, 0, elements);

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
      return BfGradientGetElements<T>(sg, 0, elements);
   
   // return the last value
   if (index == nelements)
      return BfGradientGetElements<T>(sg, nelements - 1, elements);
   
   // interpolate between two values
   const AtUInt32 prevIndex = index - 1;
   const int interp = elements[prevIndex].interp;
   
   switch(interp)
   {
      case GI_NONE:
         ret = BfGradientGetElements<T>(sg, prevIndex, elements);
         break; 
      case GI_LINEAR:
         {
            const float interpValue = (v - elements[prevIndex].position) /
                                      (elements[index].position - elements[prevIndex].position);

            if (interpValue < AI_EPSILON)
               ret = BfGradientGetElements<T>(sg, prevIndex, elements);
            else if (interpValue > (1.f - AI_EPSILON))
               ret = BfGradientGetElements<T>(sg, index, elements);
            else
               ret = BfGradientGetElements<T>(sg, prevIndex, elements) * (1.f - interpValue) + BfGradientGetElements<T>(sg, index, elements) * interpValue;
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
               ret += BfGradientGetElements<T>(sg, prevIndex, elements) * w0;
            if (w1 > AI_EPSILON)
               ret += BfGradientGetElements<T>(sg, index, elements) * w1;
         }
         break;
      case GI_SPLINE:
         {
            const T v1 = BfGradientGetElements<T>(sg, prevIndex, elements);
            const T v2 = BfGradientGetElements<T>(sg, index, elements);
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
               v0 = BfGradientGetElements<T>(sg, prevIndex - 1, elements);
            }

            if (index == (nelements - 1))
            {
               p3 = p2 + dp;
               v3 = v2;
            }
            else
            {
               p3 = elements[index + 1].position;
               v3 = BfGradientGetElements<T>(sg, index + 1, elements);
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

   return ret;
}

template<typename T>
class GradientDescription{
public:
   
   GradientDescriptionElement<T>* elements;   
   GradientDescriptionElement<float>* extraScalarElements;

   AtUInt32 nelements;
   AtUInt32 nExtraScalarElements;
   float inputMin;
   float inputMax;

   float invInputDelta;
   
   T* data;   
   float* extraScalarData;
   
   int resolution;  
   std::string dataName;
   int type;
   AtNode *points_op;
   bool voxels;
   
   GradientDescription() : elements(0), extraScalarElements(0), nExtraScalarElements(0), inputMax(1.f), invInputDelta(1.f), data(0), extraScalarData(0), points_op( 0), voxels(false) 
   {}
   ~GradientDescription() {Release();}
   
   void Release() 
   {
      if(data) AiFree(data); data = 0;
      if(elements) AiFree(elements); elements = 0;
      if(extraScalarElements) AiFree(extraScalarElements); extraScalarElements = 0;
      if(extraScalarData) AiFree(extraScalarData); extraScalarData = 0;
      nelements = 0;
      nExtraScalarElements = 0;
   }

   float RemapInput(float v)
   {
      return (v - inputMin) * invInputDelta;
   }


   T RemapResult(AtShaderGlobals *sg) const
   {
      T ret = GetDefaultValue<T>();
      float v = 0.f;

      if (voxels)
      {
         
          // get the input value from User Data (depending on the attribute type)
         switch(type)
         {
            default:
            case AI_TYPE_FLOAT:
               AiVolumeSampleFltFunc (dataName.c_str(), sg, 0, &v);
            break;
            
            {
            case AI_TYPE_VECTOR:
               AtColor col;
               AiVolumeSampleRGBFunc (dataName.c_str(), sg, 0, &col);
               AtVector vec;
               vec.x = col.r; vec.y = col.g; vec.z = col.b;
               v = AiV3Length(vec); 
               break;
            }
         }
      } else
      {
         // get the input value from User Data (depending on the attribute type)
         switch(type)
         {
            default:
            case AI_TYPE_FLOAT:
               if (!AiUDataGetFlt(dataName.c_str(), &v))return ret;
            break;
            {
            case AI_TYPE_POINT2:
               AtPoint2 pnt2;
               if (!AiUDataGetPnt2(dataName.c_str(), &pnt2)) return ret;
               v = AiV2Length(pnt2);  // or another formula ?
            break;
            }
            {
            case AI_TYPE_VECTOR:
               AtVector vec;
               if (!AiUDataGetVec(dataName.c_str(), &vec)) return ret;
               v = AiV3Length(vec); 
               break;
            }
         }
      }

      // remap the user data value from [InputMin,inputMax] to [0,1]
      v = (v - inputMin) * invInputDelta;// RemapInput(v);
      ret = BfGradientGetValue<T>(sg, v, elements, nelements, data, resolution, points_op);

      if (extraScalarElements)
      {
         ret *= BfGradientGetValue<float>(sg, v, extraScalarElements, nExtraScalarElements, extraScalarData, resolution, points_op);
         
      }

      return ret;
   }

   
   bool ReadValues(AtNode* node, AtShaderGlobals *sg, const char* userData, AtArray* positionsArray, 
         AtArray* valuesArray, AtArray* interpsArray, float _inputMin, float _inputMax, int _type)
   {
      // release previous values;
      Release();

      inputMin = _inputMin;
      inputMax = MAX(_inputMax, inputMin + AI_EPSILON);
      invInputDelta = 1.f / (inputMax - inputMin);

      points_op = (sg) ? sg->Op : 0;
      dataName = userData;

      type = _type;

      if (positionsArray == 0 || valuesArray == 0 || interpsArray == 0){ 
         return false;
      }
      
      nelements = MIN(MIN(positionsArray->nelements, valuesArray->nelements), interpsArray->nelements) ;
      
      if (nelements == 0)
         elements = 0;
      else
      {
         bool isConnected = false;
         elements = (GradientDescriptionElement<T>*)AiMalloc(sizeof(GradientDescriptionElement<T>) * nelements);

         for (AtUInt32 i = 0; i < nelements; ++i)
         {
            
            elements[i].position = AiArrayGetFlt(positionsArray, i);
            elements[i].interp = AiArrayGetInt(interpsArray, i);
            elements[i].value = ReadFromArray<T>(valuesArray, i);
            elements[i].node = 0;
            

         }
         if (nelements > 1)
            std::sort(elements, elements + nelements, CompareElements<T>);
         
         
         if (!isConnected && (nelements > 1))
         {
            resolution = 1024;
            T* _data = (T*)AiMalloc(resolution * sizeof(T));
            for (int i = 0; i < resolution; ++i)
            {
               const float v = (float) i / (float)(resolution - 1);
               _data[i] = BfGradientGetValue<T>(0, v, elements, nelements, 0, resolution, points_op);
            }

            data = _data;
         }
         
         
      }
      return true;
   }

   void AddScalarRamp( AtArray* positionsArray, AtArray* valuesArray, AtArray* interpsArray)
   {
      if(extraScalarElements) AiFree(extraScalarElements); extraScalarElements = 0;
      if(extraScalarData) AiFree(extraScalarData); extraScalarData = 0;

      if (positionsArray == 0 || valuesArray == 0 || interpsArray == 0) return;

      nExtraScalarElements = MIN(MIN(positionsArray->nelements, valuesArray->nelements), interpsArray->nelements) ;
      
      extraScalarElements = (GradientDescriptionElement<float>*)AiMalloc(sizeof(GradientDescriptionElement<float>) * nExtraScalarElements);
      for (AtUInt32 i = 0; i < nExtraScalarElements; ++i)
      {
         extraScalarElements[i].position = AiArrayGetFlt(positionsArray, i);
         extraScalarElements[i].interp = AiArrayGetInt(interpsArray, i);

         extraScalarElements[i].value = ReadFromArray<float>(valuesArray, i);
         extraScalarElements[i].node = 0;
      }

      if (nExtraScalarElements > 1)
         std::sort(extraScalarElements, extraScalarElements + nExtraScalarElements, CompareElements<float>);
      
      if (nExtraScalarElements > 1)
      {
         resolution = 1024;
         float* _data = (float*)AiMalloc(resolution * sizeof(float));
         for (int i = 0; i < resolution; ++i)
         {
            const float v = (float) i / (float)(resolution - 1);
            _data[i] = BfGradientGetValue<float>(0, v, extraScalarElements, nExtraScalarElements, 0, resolution, points_op);
         }

         extraScalarData = _data;
      }
   }
};


