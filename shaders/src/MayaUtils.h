#ifndef MAYAUTILS_H
#define MAYAUTILS_H

#include <ai.h>

extern float Luminance(const AtRGB &color);
extern float Luminance(const AtRGBA &color);

extern float Mix(float f0, float f1, float t);
extern AtRGB Mix(const AtRGB &c0, const AtRGB &c1, float t);
extern AtRGBA Mix(const AtRGBA &c0, const AtRGBA &c1, float t);

enum InterpolationType
{
   IT_NONE = 0,
   IT_LINEAR,
   IT_SMOOTH,
   IT_SPLINE
};

extern const char* InterpolationNames[];

extern InterpolationType InterpolationNameToType(const char *n);
extern void Interpolate(AtArray *p, AtArray *v, AtArray *it, float t, float &out);
extern void Interpolate(AtArray *p, AtArray *v, AtArray *it, float t, AtRGB &out);

enum RampInterpolationType
{
   RIT_NONE = 0,
   RIT_LINEAR,
   RIT_EXP_UP,
   RIT_EXP_DOWN,
   RIT_SMOOTH,
   RIT_BUMP,
   RIT_SPIKE
};

extern const char* RampInterpolationNames[];

extern RampInterpolationType RampInterpolationNameToType(const char *n);
extern void Ramp(AtArray *p, AtArray *v, float t, RampInterpolationType rit, float &out);
extern void Ramp(AtArray *p, AtArray *v, float t, RampInterpolationType rit, AtRGB &out);

#endif // MAYAUTILS_H
