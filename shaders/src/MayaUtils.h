#ifndef MAYAUTILS_H
#define MAYAUTILS_H

#include <ai.h>

extern float Luminance(float v);
extern float Luminance(const AtRGB &color);
extern float Luminance(const AtRGBA &color);

extern float Mix(float f0, float f1, float t);
extern AtRGB Mix(const AtRGB &c0, const AtRGB &c1, float t);
extern AtRGBA Mix(const AtRGBA &c0, const AtRGBA &c1, float t);

extern float MapValue(float v, float vmin, float vmax);
extern float UnmapValue(float v, float vmin, float vmax);

extern bool IsValidUV(float u, float v);
extern float Integral(float t, float nedge);
extern float Mod(float n, float d);

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

#ifndef LOG_05
#  define LOG_05 -0.693147180559945  /* log(0.5) */
#endif


#define MAYA_COLOR_BALANCE_ENUM \
   p_defaultColor,              \
   p_colorGain,                 \
   p_colorOffset,               \
   p_alphaGain,                 \
   p_alphaOffset,               \
   p_alphaIsLuminance,          \
   p_invert

void AddMayaColorBalanceParams(AtList *params);
   
extern void MayaColorBalance(AtShaderGlobals* sg,
                             AtNode* node,
                             AtInt p_start,
                             AtRGBA & result);

extern void MayaDefaultColor(AtShaderGlobals* sg,
                             AtNode* node,
                             AtInt p_start,
                             AtRGBA & result);

extern AtVector RGBtoHSV(AtRGB inRgb);

extern AtRGB HSVtoRGB(AtVector inHsv);

extern float FilteredPulseTrain(float edge, float period, float x, float dx);

extern float SmoothStep(float e0, float e1, float x);

extern float Bias(float b, float x);

extern float fBm(AtShaderGlobals *sg, const AtPoint &p, float time, float initialAmplitude,
                 int octaves[2], float initialLacunarity, float frequencyRatio, float ratio);

extern float fTurbulence(AtShaderGlobals *sg, const AtPoint &point, float time, float lacunarity,
                         float frequencyRatio, int octaves[2], float ratio, float ripples[3]);

extern AtPoint AnimatedCellNoise(const AtPoint &p, float tt);

extern int SuspendedParticles(const AtPoint &Pn, float time, float particleRadius, float jitter, float octave,
                              float &f1, AtPoint &pos1, float &f2, AtPoint &pos2, AtPoint (&particlePos)[27]);

extern int SuspendedParticles2d(const AtPoint &Pn, float time, float particleRadius, float jitter, float octave, AtPoint (&particlePos)[27]);

extern float ParticleDensity(int falloff, const AtPoint &particleCenter, const AtPoint &P, float radius);

extern float BillowNoise(const AtPoint &p, float time, int dim, float radius, float sizeRand, float jitter,
                         int falloff, float spottyness, int octaves, float frequencyRatio, float ratio, float amplitude);

extern float CosWaves(float posX, float posY, float posTime, int numWaves);

#endif // MAYAUTILS_H
