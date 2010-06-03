#include "MayaUtils.h"
#ifdef _MSC_VER
#  define _USE_MATH_DEFINES
#endif
#include <cmath>

float Luminance(const AtRGB &color)
{
   return (0.3f * color.r + 0.59f * color.g + 0.11f * color.b);
}

float Luminance(const AtRGBA &color)
{
   return (0.3f * color.r + 0.59f * color.g + 0.11f * color.b);
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
void InterpolateT(AtArray *p, AtArray *v, AtArray *it, float t, ValType &result, ValType (*getv)(AtArray*, int))
{
   int inext = p->nelements;

   for (int i=0; i<p->nelements; ++i)
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

static float _GetArrayFlt(AtArray *a, int i)
{
   return AiArrayGetFlt(a, i);
}

static AtRGB _GetArrayRGB(AtArray *a, int i)
{
   return AiArrayGetRGB(a, i);
}

void Interpolate(AtArray *p, AtArray *v, AtArray *it, float t, float &out)
{
   InterpolateT(p, v, it, t, out, _GetArrayFlt);
}

void Interpolate(AtArray *p, AtArray *v, AtArray *it, float t, AtRGB &out)
{
   InterpolateT(p, v, it, t, out, _GetArrayRGB);
}

