#ifndef MAYAUTILS_H
#define MAYAUTILS_H

#include <ai.h>

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

#endif
