#include "MayaUtils.h"
#include <string>

namespace
{

float _GetArrayFlt(AtArray *a, unsigned int i)
{
   return AiArrayGetFlt(a, i);
}

AtRGB _GetArrayRGB(AtArray *a, unsigned int i)
{
   return AiArrayGetRGB(a, i);
}

template <typename ValType>
void RampT(AtArray *p, AtArray *c, float t, RampInterpolationType it, ValType &result, ValType (*getv)(AtArray*, unsigned int), unsigned int *shuffle)
{
   unsigned int inext = p->nelements;

   for (unsigned int i = 0; (i < p->nelements); ++i)
   {
      if (t < AiArrayGetFlt(p, shuffle[i]))
      {
         inext = i;
         break;
      }
   }

   if (inext >= p->nelements)
   {
      result = getv(c, shuffle[p->nelements - 1]);
      return;
   }

   if (inext == 0)
   {
      result = getv(c, shuffle[0]);
      return;
   }

   unsigned int icur = inext - 1;
   float tcur = AiArrayGetFlt(p, shuffle[icur]);
   float tnext = AiArrayGetFlt(p, shuffle[inext]);
   ValType ccur = getv(c, shuffle[icur]);
   ValType cnext = getv(c, shuffle[inext]);
   float u = (t - tcur) / (tnext - tcur);

   switch (it)
   {
   case RIT_LINEAR:
      // u = u;
      break;
   case RIT_EXP_UP:
      u = u * u;
      break;
   case RIT_EXP_DOWN:
      u = 1.0f - (1.0f - u) * (1.0f - u);
      break;
   case RIT_SMOOTH:
      u = 0.5f * (static_cast<float>(cos((u + 1.0f) * AI_PI)) + 1.0f);
      break;
   case RIT_BUMP:
      {
         float lcur = RampLuminance(ccur);
         float lnext = RampLuminance(cnext);
         if (lcur < lnext)
         {
            u = sin(u * static_cast<float>(AI_PI) / 2.0f);
         }
         else
         {
            u = sin((u - 1.0f) * static_cast<float>(AI_PI) / 2.0f) + 1.0f;
         }
      }
      break;
   case RIT_SPIKE:
      {
         float lcur = RampLuminance(ccur);
         float lnext = RampLuminance(cnext);
         if (lcur > lnext)
         {
            u = sin(u * static_cast<float>(AI_PI) / 2.0f);
         }
         else
         {
            u = sin((u - 1.0f) * static_cast<float>(AI_PI) / 2.0f) + 1.0f;
         }
      }
      break;
   case RIT_NONE:
   default:
      u = 0.0f;
   }

   result = Mix(ccur, cnext, u);
}

};

// This one is defined for the RampT template function to work properly
float Luminosity(float v)
{
   return v;
}

float Luminosity(const AtRGB &color)
{
   return RGBtoHSL(color).z;
}

float Luminosity(const AtRGBA &color)
{
   return RGBtoHSL(AiRGBAtoRGB(color)).z;
}

// This one is defined for the RampT template function to work properly
float Luminance(float v)
{
   return v;
}

float Luminance(const AtRGB &color)
{
   return (0.3f * color.r + 0.59f * color.g + 0.11f * color.b);
}

float Luminance(const AtRGBA &color)
{
   return (0.3f * color.r + 0.59f * color.g + 0.11f * color.b);
}

// This one is defined for the RampT template function to work properly
float RampLuminance(float v)
{
   return v;
}

float RampLuminance(const AtRGB &color)
{
   return (0.3f * color.r + 0.3f * color.g + 0.3f * color.b);
}

float RampLuminance(const AtRGBA &color)
{
   return (0.3f * color.r + 0.3f * color.g + 0.3f * color.b);
}

float Mix(float a, float b, float t)
{
   return (a + t * (b - a));
}

AtRGB Mix(const AtRGB &c0, const AtRGB &c1, float t)
{
   return (c0 + t * (c1 - c0));
}

AtRGBA Mix(const AtRGBA &c0, const AtRGBA &c1, float t)
{
   AtRGBA rv;
   rv.r = c0.r + t * (c1.r - c0.r);
   rv.g = c0.g + t * (c1.g - c0.g);
   rv.b = c0.b + t * (c1.b - c0.b);
   rv.a = c0.a + t * (c1.a - c0.a);
   return rv;
}

float MapValue(float v, float vmin, float vmax)
{
   return ((v - vmin) / (vmax - vmin));
}

float UnmapValue(float v, float vmin, float vmax)
{
   return (vmin + (vmax - vmin) * v);
}

bool IsValidUV(AtPoint2 &uv)
{
   // place2dTexture return (UV_INVALID, UV_INVALID) for invalid UVs
   return (uv.x > UV_INVALID && uv.y > UV_INVALID);
}

float Integral(float t, float nedge)
{
   return ((1.0f - nedge) * FLOOR(t) + MAX(0.0f, t - FLOOR(t) - nedge));
}

float Mod(float n, float d)
{
   return (n - (floor(n / d) * d));
}

bool SortFloatArray(AtArray *a, unsigned int *shuffle)
{
   bool modified = false;

   if (a && a->nelements > 0)
   {
      float p0, p1;
      int tmp;

      bool swapped = true;
      AtUInt32 n = a->nelements;

      if (shuffle)
      {
         for (AtUInt32 i = 0; (i < n); ++i)
         {
            shuffle[i] = i;
         }
      }

      while (swapped)
      {
         swapped = false;
         n -= 1;
         for (AtUInt32 i = 0; (i < n); ++i)
         {
            p0 = AiArrayGetFlt(a, i);
            p1 = AiArrayGetFlt(a, i + 1);
            if (p0 > p1)
            {
               swapped = true;
               modified = true;

               AiArraySetFlt(a, i, p1);
               AiArraySetFlt(a, i + 1, p0);

               if (shuffle)
               {
                  tmp = shuffle[i];
                  shuffle[i] = shuffle[i + 1];
                  shuffle[i + 1] = tmp;
               }
            }
         }
      }
   }

   return modified;
}

bool SortFloatIndexArray(AtArray *a, unsigned int *shuffle)
{
   bool modified = false;

   if (a && shuffle && a->nelements > 0)
   {
      float p0, p1;
      int tmp;

      bool swapped = true;
      AtUInt32 n = a->nelements;

      for (AtUInt32 i = 0; (i < n); ++i)
      {
         shuffle[i] = i;
      }

      while (swapped)
      {
         swapped = false;
         n -= 1;
         for (AtUInt32 i = 0; (i < n); ++i)
         {
            p0 = AiArrayGetFlt(a, shuffle[i]);
            p1 = AiArrayGetFlt(a, shuffle[i + 1]);
            if (p0 > p1)
            {
               swapped = true;
               modified = true;

               tmp = shuffle[i];
               shuffle[i] = shuffle[i + 1];
               shuffle[i + 1] = tmp;
            }
         }
      }
   }

   return modified;
}

void ShuffleArray(AtArray *a, unsigned int *shuffle, int arnoldType)
{
   if (!a || !shuffle)
   {
      return;
   }

   if (arnoldType == AI_TYPE_FLOAT)
   {
      float tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetFlt(a, i);
         AiArraySetFlt(a, i, AiArrayGetFlt(a, shuffle[i]));
         AiArraySetFlt(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_RGB)
   {
      AtRGB tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetRGB(a, i);
         AiArraySetRGB(a, i, AiArrayGetRGB(a, shuffle[i]));
         AiArraySetRGB(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_RGBA)
   {
      AtRGBA tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetRGBA(a, i);
         AiArraySetRGBA(a, i, AiArrayGetRGBA(a, shuffle[i]));
         AiArraySetRGBA(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_STRING)
   {
      std::string tmp0, tmp1;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp0 = AiArrayGetStr(a, i);
         tmp1 = AiArrayGetStr(a, shuffle[i]);
         AiArraySetStr(a, i, tmp1.c_str());
         AiArraySetStr(a, shuffle[i], tmp0.c_str());
      }
   }
   else if (arnoldType == AI_TYPE_BYTE)
   {
      AtByte tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetByte(a, i);
         AiArraySetByte(a, i, AiArrayGetByte(a, shuffle[i]));
         AiArraySetByte(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_INT)
   {
      int tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetByte(a, i);
         AiArraySetByte(a, i, AiArrayGetByte(a, shuffle[i]));
         AiArraySetByte(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_UINT)
   {
      unsigned int tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetByte(a, i);
         AiArraySetByte(a, i, AiArrayGetByte(a, shuffle[i]));
         AiArraySetByte(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_BOOLEAN)
   {
      bool tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = (AiArrayGetBool(a, i) == TRUE);
         AiArraySetBool(a, i, AiArrayGetBool(a, shuffle[i]));
         AiArraySetBool(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_VECTOR)
   {
      AtVector tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetVec(a, i);
         AiArraySetVec(a, i, AiArrayGetVec(a, shuffle[i]));
         AiArraySetVec(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_POINT)
   {
      AtPoint tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetPnt(a, i);
         AiArraySetPnt(a, i, AiArrayGetPnt(a, shuffle[i]));
         AiArraySetPnt(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_POINT2)
   {
      AtPoint2 tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetPnt2(a, i);
         AiArraySetPnt2(a, i, AiArrayGetPnt2(a, shuffle[i]));
         AiArraySetPnt2(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_POINTER)
   {
      void *tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetPtr(a, i);
         AiArraySetPtr(a, i, AiArrayGetPtr(a, shuffle[i]));
         AiArraySetPtr(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_MATRIX)
   {
      AtMatrix tmp0;
      AtMatrix tmp1;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         AiArrayGetMtx(a, i, tmp0);
         AiArrayGetMtx(a, shuffle[i], tmp1);
         AiArraySetMtx(a, i, tmp1);
         AiArraySetMtx(a, shuffle[i], tmp0);
      }
   }
   else if (arnoldType == AI_TYPE_ENUM)
   {
      int tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetInt(a, i);
         AiArraySetInt(a, i, AiArrayGetInt(a, shuffle[i]));
         AiArraySetInt(a, shuffle[i], tmp);
      }
   }
}

const char* InterpolationNames[] =
{
   "none",
   "linear",
   "smooth",
   "spline",
   NULL
};

InterpolationType InterpolationNameToType(const char *n)
{
   return (InterpolationType) AiEnumGetValue(InterpolationNames, n);
}

template <typename ValType>
void InterpolateT(AtArray *p, AtArray *v, AtArray *it, float t, ValType &result, ValType (*getv)(AtArray*, unsigned int))
{
   unsigned int inext = p->nelements;

   for (unsigned int i = 0; (i < p->nelements); ++i)
   {
      if (t < AiArrayGetFlt(p, i))
      {
         inext = i;
         break;
      }
   }

   if (inext >= p->nelements)
   {
      result = getv(v, p->nelements - 1);
      return;
   }

   int iprev = inext - 1;
   
   if (iprev < 0)
   {
      result = getv(v, 0);
      return;
   }

   float p1 = AiArrayGetFlt(p, iprev);
   float p2 = AiArrayGetFlt(p, inext);

   ValType v1 = getv(v, iprev);
   ValType v2 = getv(v, inext);

   float dp = p2 - p1;
   float u = (t - p1) / dp;

   int itype = InterpolationNameToType(AiArrayGetStr(it, iprev));

   switch (itype)
   {
   case IT_NONE:
      result = v1;
      break;

   case IT_LINEAR:
      result = v1 + u * (v2 - v1);
      break;

   case IT_SMOOTH:
      {
         float u2 = u * u;
         float u3 = u * u2;
         float a =  2.0f * u3 - 3.0f * u2 + 1.0f;
         float b = -2.0f * u3 + 3.0f * u2 ;
         result = a * v1 + b * v2;
      }
      break;

   case IT_SPLINE:
      {
         float u2 = u * u;
         float u3 = u * u2;
         float a =  2.0f * u3 - 3.0f * u2 + 1.0f;
         float b = -2.0f * u3 + 3.0f * u2;
         float c = u3 - 2.0f * u2 + u;
         float d = u3 - u2;

         ValType t1, t2;

         if (iprev == 0)
         {
            // no prev key
            t1 = (v2 - v1) / (p2 - p1); 
         }
         else
         {
            float p0 = AiArrayGetFlt(p, iprev-1);
            ValType v0 = getv(v, iprev-1);

            t1 = 0.5f * (((v1 - v0) / (p1 - p0)) + ((v2 - v1) / (p2 - p1)));
         }

         if (inext == v->nelements - 1)
         {
            // no next key
            t2 = (v2 - v1) / (p2 - p1);
         }
         else
         {
            float p3 = AiArrayGetFlt(p, inext+1);
            ValType v3 = getv(v, inext+1);

            t2 = 0.5f * (((v2 - v1) / (p2 - p1)) + ((v3 - v2) / (p3 - p2)));
         }

         t1 *= dp;
         t2 *= dp;

         result = a * v1 + b * v2 + c * t1 + d * t2;
      }
      break;

   default:
      result = v1;
   }
}

const char* RampInterpolationNames[] =
{
   "none",
   "linear",
   "exponentialup",
   "exponentialdown",
   "smooth",
   "bump",
   "spike",
   NULL
};

RampInterpolationType RampInterpolationNameToType(const char *n)
{
   return (RampInterpolationType) AiEnumGetValue(RampInterpolationNames, n);
}

void Interpolate(AtArray *p, AtArray *v, AtArray *it, float t, float &out)
{
   InterpolateT(p, v, it, t, out, _GetArrayFlt);
}

void Interpolate(AtArray *p, AtArray *v, AtArray *it, float t, AtRGB &out)
{
   InterpolateT(p, v, it, t, out, _GetArrayRGB);
}

void Ramp(AtArray *p, AtArray *v, float t, RampInterpolationType it, float &out, unsigned int *shuffle)
{
   RampT(p, v, t, it, out, _GetArrayFlt, shuffle);
}

void Ramp(AtArray *p, AtArray *v, float t, RampInterpolationType it, AtRGB &out, unsigned int *shuffle)
{
   RampT(p, v, t, it, out, _GetArrayRGB, shuffle);
}


void AddMayaColorBalanceParams(AtList *params)
{
   AiParameterRGB ("defaultColor", 0.5f, 0.5f, 0.5f);
   AiParameterRGB ("colorGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB ("colorOffset", 0.0f, 0.0f, 0.0f);
   AiParameterFLT ("alphaGain", 1.0f);
   AiParameterFLT ("alphaOffset", 0.0f);
   AiParameterBOOL("alphaIsLuminance", false);
   AiParameterBOOL("invert", false);
}

void MayaColorBalance(AtShaderGlobals* sg,
                        AtNode* node,
                        int p_start,
                        AtRGBA & result)
{
   const AtRGB colorGain      = AiShaderEvalParamFuncRGB(sg, node, p_start + 1);  //p_colorGain);
   const AtRGB colorOffset    = AiShaderEvalParamFuncRGB(sg, node, p_start + 2);  //p_colorOffset);
   const float alphaGain      = AiShaderEvalParamFuncFlt(sg, node, p_start + 3);  //p_alphaGain);
   const float alphaOffset    = AiShaderEvalParamFuncFlt(sg, node, p_start + 4);  //p_alphaOffset);
   const AtBoolean alphaIsLuminance     = AiShaderEvalParamFuncBool(sg, node, p_start+ 5);  //alphaIsLuminance);
   const AtBoolean invert = AiShaderEvalParamFuncBool(sg, node, p_start+ 6); //p_invert);

   if (invert)
   {
      result.r = 1.f - result.r;
      result.g = 1.f - result.g;
      result.b = 1.f - result.b;
      result.a = 1.f - result.a;
   }

   if (alphaIsLuminance)
   {
      result.a = Luminance(result);
   }

   result.r = result.r * colorGain.r + colorOffset.r;
   result.g = result.g * colorGain.g + colorOffset.g;
   result.b = result.b * colorGain.b + colorOffset.b;
   result.a = result.a * alphaGain   + alphaOffset;
}

void MayaDefaultColor(AtShaderGlobals* sg,
                        AtNode* node,
                        int p_start,
                        AtRGBA & result)
{
   const AtRGB defaultColor   = AiShaderEvalParamFuncRGB(sg, node, p_start + 0);  //p_defaultColor);
   AiRGBtoRGBA(defaultColor, result);
   result.a = 0.0f;
}

AtVector RGBtoHSV(AtRGB inRgb)
{
   AtVector output(AI_V3_ZERO);

   float min = 0.0f;
   float max = 0.0f;
   int rgbMax = 0;

   min = MIN(inRgb.r, inRgb.g);
   min = MIN(min, inRgb.b);

   max = MAX(inRgb.r, inRgb.g);
   max = MAX(max, inRgb.b);

   if (fabs(max - inRgb.r) < AI_EPSILON)
   {
      rgbMax = 0;
   }
   else if (fabs(max - inRgb.g) < AI_EPSILON)
   {
      rgbMax = 1;
   }
   else
   {
      rgbMax = 2;
   }

   if (max - min < AI_EPSILON)
   {
      output.x = 0.0f;
   }
   else
   {
      if (rgbMax == 0)
      {
        output.x = 60.0f * (inRgb.g - inRgb.b) / (max - min);
      }
      else if (rgbMax == 1)
      {
        output.x = 60.0f * (inRgb.b - inRgb.r) / (max - min) + 120.0f;
      }
      else if (rgbMax == 2)
      {
        output.x = 60.0f * (inRgb.r - inRgb.g) / (max - min) + 240.0f;
      }

      while (output.x < 0.0f)
      {
         output.x += 360.0f;
      }
   }

   if (max < AI_EPSILON)
   {
      output.y = 0.0f;
   }
   else
   {
      output.y = (max - min) / max;
   }

   output.z = max;

   return output;
}

AtVector RGBtoHSL(AtRGB inRgb)
{
   AtVector output(AI_V3_ZERO);

   float min = 0.0f;
   float max = 0.0f;
   int rgbMax = 0;

   min = MIN(inRgb.r, inRgb.g);
   min = MIN(min, inRgb.b);

   max = MAX(inRgb.r, inRgb.g);
   max = MAX(max, inRgb.b);

   if (fabs(max - inRgb.r) < AI_EPSILON)
   {
      rgbMax = 0;
   }
   else if (fabs(max - inRgb.g) < AI_EPSILON)
   {
      rgbMax = 1;
   }
   else
   {
      rgbMax = 2;
   }

   // L
   output.z = 0.5f * (min + max);

   // S
   float d = max - min;

   if (d >= AI_EPSILON)
   {
      output.y = d / (output.z <= 0.5f ? (max + min) : (2.0f - max - min));

      d = 1.0f / d;

      // H
      if (rgbMax == 0)
      {
         output.x = d * (inRgb.g - inRgb.b);
      }
      else if (rgbMax == 1)
      {
         output.x = 2.0f + d * (inRgb.b - inRgb.r);
      }
      else
      {
         output.x = 4.0f + d * (inRgb.r - inRgb.g);
      }

      output.x /= 6.0f;
   }

   return output;
}

AtRGB HSVtoRGB(AtVector inHsv)
{
   AtRGB output;

   if (inHsv.y < AI_EPSILON)
   {
      output.r = inHsv.z;
      output.g = inHsv.z;
      output.b = inHsv.z;
   }
   else
   {
      float f = inHsv.x / 60.0f;

      int Hi = int(f) % 6;

      f -= Hi;

      float p = inHsv.z * (1 - inHsv.y);
      float q = inHsv.z * (1 - f * inHsv.y);
      float t = inHsv.z * (1 - (1 - f) * inHsv.y);

      switch (Hi)
      {
      case 0:
         output.r = inHsv.z;
         output.g = t;
         output.b = p;
         break;
      case 1:
         output.r = q;
         output.g = inHsv.z;
         output.b = p;
         break;
      case 2:
         output.r = p;
         output.g = inHsv.z;
         output.b = t;
         break;
      case 3:
         output.r = p;
         output.g = q;
         output.b = inHsv.z;
         break;
      case 4:
         output.r = t;
         output.g = p;
         output.b = inHsv.z;
         break;
      case 5:
      default:
         output.r = inHsv.z;
         output.g = p;
         output.b = q;
         break;
      }
   }

   return output;
}

// Taken from Advanced RenderMan
float FilteredPulseTrain(float edge, float period, float x, float dx)
{
   float invPeriod = 1.0f / period;

   float w = dx * invPeriod;
   float x0 = x * invPeriod - 0.5f * w;
   float x1 = x0 + w;
   float nedge = edge * invPeriod;

   float result;

   if (x0 == x1)
   {
     result = (x0 - FLOOR(x0) < nedge) ? 0.0f : 1.0f;
   }
   else
   {
      result = (Integral(x1, nedge) - Integral(x0, nedge)) / w;
   }

   return result;
}

float SmoothStep(float e0, float e1, float x)
{
   float t = (x - e0) / (e1 - e0);
   t = CLAMP(t, 0.0f, 1.0f);
   return t * t * (3.0f - 2.0f * t);
}

float Bias(float b, float x)
{
   return pow(x, log(b) / float(LOG_05));
}

float fBm(AtShaderGlobals *sg,
          const AtPoint &p,
          float time,
          float initialAmplitude,
          int octaves[2],
          float initialLacunarity,
          float frequencyRatio,
          float ratio)
{
   float amp = initialAmplitude;
   AtPoint pp = p;
   float sum = 0;
   int i = 0;
   float lacunarity = initialLacunarity;

   // NOTE: this is wrong, sg->area is "world-space" area
   float pixelSize = (float) sqrt(sg->area);
   float nyquist = 2.0f * pixelSize;
   float pixel = 1.0f;

   while ((i < octaves[1] && pixel > nyquist) || i < octaves[0])
   {
      sum += amp * AiPerlin4(pp * lacunarity, time);
      amp *= ratio;
      lacunarity *= frequencyRatio;
      pixel /= frequencyRatio;
      ++i;
   }

   if (pixel > pixelSize && i <= octaves[1])
   {
      float weight = CLAMP(pixel/pixelSize - 1.0f, 0.0f, 1.0f);
      sum += weight * amp * AiPerlin4(pp * lacunarity, time);
   }

   return sum * 0.5f + 0.5f;
}

float fTurbulence(AtShaderGlobals *sg,
                  const AtPoint &point,
                  float time,
                  float lacunarity,
                  float frequencyRatio,
                  int octaves[2],
                  float ratio,
                  float ripples[3])
{
   int i = 0;
   float mix = 0.0f;
   float amp = 1.0f;
   AtPoint pp;
   AiV3Create(pp, ripples[0], ripples[1], ripples[2]);
   pp = point * pp / 2.0f;

   // NOTE: this is wrong, sg->area is "world-space" area
   float pixelSize = (float) sqrt(sg->area);
   float niquist = 2.0f * pixelSize;
   float pixel = 1.0;

   while ((i < octaves[1] && pixel > niquist) || i < octaves[0])
   {
      AtPoint2 offset;
      AiV2Create(offset, lacunarity, lacunarity);
      mix += amp * fabs(AiPerlin4((pp + AiPerlin2(offset)) * lacunarity, time));
      lacunarity *= frequencyRatio;
      amp *= ratio;
      pixel /= frequencyRatio;
      ++i;
   }

   if (pixel > pixelSize && i <= octaves[1])
   {
      AtVector2 offset;
      AiV2Create(offset, lacunarity, lacunarity);
      float weight = CLAMP((pixel/pixelSize - 1.0f), 0.0f, 1.0f);
      mix += weight * amp * fabs(AiPerlin4((pp+AiPerlin2(offset)) * lacunarity, time));
   }

   return mix;
}

AtPoint AnimatedCellNoise(const AtPoint &p, float tt)
{
   float t = AiCellNoise4(p, tt);
   float tbase = floor(t);

   AtPoint n1 = AiVCellNoise4(p, tbase);
   float d = t - tbase;
   AtPoint n2 = AiVCellNoise4(p, tbase + 1.0f);
   n1 += d * (n2 - n1);

   return n1;
}

int SuspendedParticles(const AtPoint &Pn,
                       float time,
                       float particleRadius,
                       float jitter,
                       float octave,
                       float &f1,
                       AtPoint &pos1,
                       float &f2,
                       AtPoint &pos2,
                       AtPoint (&particlePos)[27])
{
   AtPoint thiscell;
   AiV3Create(thiscell, floor(Pn.x)+0.5f, floor(Pn.y)+0.5f, floor(Pn.z)+0.5f);

   f1 = f2 = 1e36f;
   int i, j, k;

   unsigned int curr_particle = 0;
   for (i=-1; i<=1; ++i)
   {
      for (j=-1; j<=1; ++j)
      {
         for (k=-1; k<=1; ++k)
         {
            AtVector testvec;
            AiV3Create(testvec, (float)i, (float)j, (float)k);
            AtPoint testcell = thiscell + testvec;

            if (jitter > 0)
            {
               AtVector jit = AnimatedCellNoise(testcell, time+1000.0f*octave) - 0.5f;
               testcell += jitter * jit;
            }

            float dist = AiV3Dist(testcell, Pn);

            if (dist < particleRadius)
            {
               particlePos[curr_particle] = testcell;
               ++curr_particle;
               if (dist < f1)
               {
                  f2 = f1;
                  pos2 = pos1;
                  f1 = dist;
                  pos1 = testcell;
               }
               else if (dist < f2)
               {
                  f2 = dist;
                  pos2 = testcell;
               }
            }
         }
      }
   }
   return curr_particle;
}

int SuspendedParticles2d(const AtPoint &Pn,
                         float time,
                         float particleRadius,
                         float jitter,
                         float octave,
                         AtPoint (&particlePos)[27])
{
   AtPoint thiscell;
   AiV3Create(thiscell, floor(Pn.x)+0.5f, floor(Pn.y)+0.5f, 0.0f);

   int i, j;
   unsigned int curr_particle = 0;

   for (i=-1; i<=1; ++i)
   {
      for (j=-1; j<=1; ++j)
      {
         AtVector testvec;
         AiV3Create(testvec, (float)i, (float)j, 0.0f);
         AtPoint testcell = thiscell + testvec;
         
         if (jitter > 0.0f)
         {
            AtVector vjit = AnimatedCellNoise(testcell, time+1000.0f*octave) - 0.5f;
            vjit *= jitter;
            vjit.z = 0.0f;
            testcell += vjit;
         }

         float dist = AiV3Dist(testcell, Pn);

         if (dist < particleRadius)
         {
            particlePos[curr_particle] = testcell;
            ++curr_particle;
         }
      }
   }
   return curr_particle;
}

float ParticleDensity(int falloff,
                      const AtPoint &particleCenter,
                      const AtPoint &P,
                      float radius)
{
   float distanceToCenter = AiV3Dist(particleCenter, P);
   float fadeout = 0;

   if (falloff == 0) // linear
   {
      fadeout = distanceToCenter / radius;
   }
   else if (falloff == 1) // smooth
   {
      fadeout = SmoothStep(0.0f, radius, distanceToCenter);
   }
   else
   {
      fadeout = distanceToCenter / radius;
      fadeout = Bias(0.25f, fadeout);

      if (falloff == 3)
      {
         fadeout = CLAMP(1.0f-fadeout, 0.0f, 1.0f);
         fadeout += 1 - SmoothStep(0.0, 0.1f, fadeout);
      }
   }
   return 1.0f - fadeout;
}

float BillowNoise(const AtPoint &p,
                  float time,
                  int dim,
                  float radius,
                  float sizeRand,
                  float jitter,
                  int falloff,
                  float spottyness,
                  int octaves,
                  float frequencyRatio,
                  float ratio,
                  float amplitude)
{
   AtVector v;
   AiV3Create(v, 0.425f, 0.6f, dim == 3 ? 0.215f : 0.0f);
   AtPoint pp = p + v;

   int i, j;
   float lacunarity = 1.0f;

   AtPoint particles[27];
   float f1, f2;
   AtPoint /*p1,*/ p2;
   int numParticles;
   float sum = 0.0f;
   float amp = 1.0f;
   float ampSum = 0.0f;

   for (i=0; i<octaves; ++i)
   {
      if (dim == 3)
      {
         numParticles = SuspendedParticles(pp * lacunarity, time, radius, jitter, (float)i, f1, p2, f2, p2, particles);
      }
      else
      {
         numParticles = SuspendedParticles2d(pp * lacunarity, time, radius, jitter, (float)i, particles);
      }

      if (numParticles > 0)
      {
         for (j=0; j<numParticles; ++j)
         {
            float radiusScale = 1.0f;

            if (sizeRand != 0.0f)
            {
               radiusScale = (0.5f - CLAMP((AiPerlin3(particles[j])+1)*0.5f * 0.75f - 0.25f, 0.0f, 0.5f) * sizeRand) * 2.0f;
            }

            float density = ParticleDensity(falloff, particles[j], pp*lacunarity, radius*radiusScale);

            if (spottyness > 0)
            {
               AtVector v;
               AiV3Create(v, 1.0f, 7.0f, 1023.0f);
               float l = spottyness * (AiCellNoise3(particles[j]+v)*2.0f-1.0f);
               density += density * l;
               density = CLAMP(density, 0.0f, 1.0f);
            }

            sum += amp * density;
         }
      }

      ampSum += amp;
      amp *= ratio;
      lacunarity *= frequencyRatio;
   }
   return amplitude * sum / ampSum;
}

float CosWaves(float posX,
               float posY,
               float posTime,
               int numWaves)
{
   float x = posX * 2.0f * (float)AI_PI;
   float y = posY * 2.0f * (float)AI_PI;
   float time = posTime * 2.0f * (float)AI_PI;

   float dirX = 0;
   float dirY = 0;
   float norm = 0;
   float noiseValue = 0;
   int i = 0;

   for (i=1; i<=numWaves; ++i)
   {
      float generator = (float)i * (float)AI_PI / (float)numWaves;
      AtPoint v;
      AiV3Create(v, generator, 0.0f, 0.0f);
      dirX = AiPerlin3(v);
      AiV3Create(v, 0.0f, generator, 0.0f);
      dirY = AiPerlin3(v);
      AiV3Create(v, 0.0f, 0.0f, generator);
      float offset = AiPerlin3(v);
      generator = 50.0f * dirX * dirY;

      AtPoint2 v2;
      AiV2Create(v2, generator, generator);
      float freqNoise = AiPerlin2(v2);

      norm = sqrt(dirX * dirX + dirY * dirY);

      if (norm > AI_EPSILON)
      {
         dirX /= norm;
         dirY /= norm;
         noiseValue += static_cast<float>(cos((dirX*x + dirY*y) * (AI_PI) / (6.0f * freqNoise) + offset * time));
      }
   }

   noiseValue /= (float)numWaves;
   return noiseValue;
}

// Set sg->P to Pref if Pref is existing on the object.
//
// How to use it in your shader :
// At the beginning of shader_evaluate, place these lines :
//
//       AtPoint tmpPts;
//      bool usePref = SetRefererencePoints(sg, tmpPts);
//
// Use  RestorePoints() when you don't use Pref anymore :
//
//      if (usePref) RestorePoints(sg, tmpPts);
//
// @param sg      shading global.
// @param tmpTps   keep the original sg->P in this variable.
// @return          True if Pref was found and sg->P replaced.
//
bool SetRefererencePoints(AtShaderGlobals *sg, AtPoint &tmpPts)
{
   AtPoint Pref;
   AtBoolean usePref = AiUDataGetPnt("Pref",&Pref);
   if (usePref)
   {
      tmpPts = sg->P;
      sg->P = Pref;
      return true;
   }
   return false;
}

// Set sg->N to Nref if Nref is existing on the object.
//
// How to use it in your shader :
// At the beginning of shader_evaluate, place these lines :
//
//      AtVector tmpNmrs;
//      bool useNref = SetRefererenceNormals(sg, tmpNmrs);
//
// Use  RestoreNormals() when you don't use Nref anymore :
//
//      if (useNref) RestoreNormals(sg, tmpNmrs);
//
// @param sg        shading global.
// @param tmpTps    keep the original sg->N in this variable.
// @return          True if Nref was found and sg->N replaced.
//
bool SetRefererenceNormals(AtShaderGlobals *sg, AtVector &tmpNmrs)
{
   AtVector Nref;
   AtBoolean useNref = AiUDataGetPnt("Nref",&Nref);
   if (useNref)
   {
      tmpNmrs = sg->N;
      sg->N = Nref;
      return true;
   }
   return false;
}

// Restore sg->P with original P. Use in combinaison with SetRefererencePoints.
//
// @param sg      shading global.
// @param tmpTps   The original P, given by SetRefererencePoints.
//
void RestorePoints(AtShaderGlobals *sg, AtPoint tmpPts)
{
   sg->P = tmpPts;
}

// Restore sg->N with original N. Use in combinaison with SetRefererenceNormals.
// @param sg      shading global.
// @param tmpNmrs   The original P, given by tmpNmrsv.
//
void RestoreNormals(AtShaderGlobals *sg, AtVector tmpNmrs)
{
   sg->N = tmpNmrs;
}

