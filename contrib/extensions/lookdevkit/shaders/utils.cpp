#include "utils.h"

const char* CompositeOperationNames[] = 
{
   "Add",
   "Subtract",
   "Mix",
   "Multiply",
   "Screen",
   "Overlay",
   "Difference",
   "Dodge",
   "Burn",
   NULL
};

AtVector RGBtoHSV(const AtRGB& inRgb)
{
   const float r = inRgb.r;
   const float g = inRgb.g;
   const float b = inRgb.b;

   const float min = AiMin(r, g, b);
   const float max = AiMax(r, g, b);
   float delta = max - min;

   float h = 0.0f;
   float s = (max > 0.0f) ? (delta / max) : 0.0f;
   float v = max;

   if ( s == 0.0f )
   {
      h = 0.0f;
   }
   else
   {
      if ( r == max )
      {
         h = (g - b) / delta;
      }
      else if ( g == max )
      {
         h = 2.0f + (b - r) / delta;
      }
      else if ( b == max )
      {
         h = 4.0f + (r - g) / delta;
      }

      h *= 60.0f;

      if ( h < 0.0f )
      {
         h += 360.0f;
      }
   }

   return AtVector(h,s,v);
}

AtRGB HSVtoRGB(const AtVector& inHsv)
{
   float h = inHsv.x;
   float s = inHsv.y;
   float v = inHsv.z;

   if ( s == 0.0f )
   {
      return AtRGB(v);
   }
   else
   {
      h = (h == 360.0f ? 0.0f : h / 360.0f);
      h -= floorf(h);
      h *= 6.0f;

      const int i = static_cast<int>(floorf(h));
      const float f = h - static_cast<float>(i);

      const float p = v * (1.0f - s);
      const float q = v * (1.0f - (s * f));
      const float t = v * (1.0f - (s * (1.0f - f)));

      switch(i)
      {
         case 0:
            return AtRGB(v, t, p);
         case 1:
            return AtRGB(q, v, p);
         case 2:
            return AtRGB(p, v, t);
         case 3:
            return AtRGB(p, q, v);
         case 4:
            return AtRGB(t, p, v);
         case 5:
         default:
            return AtRGB(v, p, q);
      }
   }
}
