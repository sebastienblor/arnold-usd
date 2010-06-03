#include "MayaUtils.h"
#ifdef _MSC_VER
#  define _USE_MATH_DEFINES
#endif
#include <cmath>

// This one is defined for the RampT template function to work properly
static float Luminance(float v)
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

template <typename ValType>
static void RampT(AtArray *p, AtArray *c, float t, RampInterpolationType it, ValType &result, ValType (*getv)(AtArray*, int))
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
      result = getv(c, p->nelements - 1);
      return;
   }

   int icur = inext - 1;

   if (icur < 0)
   {
      result = getv(c, 0);
      return;
   }

   float tcur = AiArrayGetFlt(p, icur);
   float tnext = AiArrayGetFlt(p, inext);
   ValType ccur = getv(c, icur);
   ValType cnext = getv(c, inext);
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
      u = 0.5f * (cos((u + 1.0f) * M_PI) + 1.0f);
      break;
   case RIT_BUMP:
      {
         float lcur = Luminance(ccur);
         float lnext = Luminance(cnext);
         if (lcur > lnext)
         {
            u = sin(u * M_PI / 2.0f);
         }
         else
         {
            u = sin((u - 1.0f) * M_PI / 2.0f) + 1.0f;
         }
      }
      break;
   case RIT_SPIKE:
      {
         float lcur = Luminance(ccur);
         float lnext = Luminance(cnext);
         if (lcur < lnext)
         {
            u = sin(u * M_PI / 2.0f);
         }
         else
         {
            u = sin((u - 1.0f) * M_PI / 2.0f) + 1.0f;
         }
      }
      break;
   case RIT_NONE:
   default:
      u = 0.0f;
   }

   result = Mix(ccur, cnext, u);
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

void Ramp(AtArray *p, AtArray *v, float t, RampInterpolationType it, float &out)
{
   RampT(p, v, t, it, out, _GetArrayFlt);
}

void Ramp(AtArray *p, AtArray *v, float t, RampInterpolationType it, AtRGB &out)
{
   RampT(p, v, t, it, out, _GetArrayRGB);
}

