#pragma once

#include <ai.h>

enum CompositeOperation
{
   COP_ADD = 0,
   COP_SUBTRACT,
   COP_MIX,
   COP_MULTIPLY,
   COP_SCREEN,
   COP_OVERLAY,
   COP_DIFFERENCE,
   COP_DODGE,
   COP_BURN
};

extern const char* CompositeOperationNames[];

// This is the clamp to use for this shader library.
// Don't use the built in CLAMP, it gives a different result when lo > hi.
inline float Clamp(const float x, const float lo, const float hi)
{
   return (AiMin(hi, AiMax(lo, x)));
}

inline float Gamma(const float v, const float g)
{
   return pow(v, (1.0f / g));
}

inline float CompositeAdd(float a, float b, float factor)
{
   return a + b * factor;
}

inline float CompositeSubtract(float a, float b, float factor)
{
   return a + (b - 1.0f) * factor;
}

inline float CompositeMix(float a, float b, float factor)
{
   return AiLerp(factor, a, b);
}

inline float CompositeMultiply(float a, float b, float factor)
{
   return a * (b * factor + (1.0f - factor));
}

inline float CompositeScreen(float a, float b, float factor)
{
   return a + b * factor;
}

inline float CompositeOverlay(float a, float b, float factor)
{
   return b > 0.5f ? (2.0f * a * b * factor ) + a * (1.0f - factor) : 1.0f - ((1.0f - a) * (1.0f - b * factor)) * (2.0f - (1.0f - factor));
}

inline float CompositeDifference(float a, float b, float factor)
{
   return fabs(a - (b * factor));
}

inline float CompositeDodge(float a, float b, float factor)
{
   return Clamp(AiLerp(factor, a, a / AiMax(1.0f - b, 0.00001f)), a, b);
}

inline float CompositeBurn(float a, float b, float factor)
{
   return Clamp(AiLerp(factor, a, 1.0f - (a / AiMax(1.0f - b, 0.00001f))), a, b);
}

AtVector RGBtoHSV(const AtRGB& inRgb);

AtRGB HSVtoRGB(const AtVector& inHsv);

