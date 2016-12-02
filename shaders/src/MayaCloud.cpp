#include <ai.h>

#include <limits.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaCloudMtd);

namespace
{
   enum MayaCloudParams
   {
      p_color1 = 0,
      p_color2,
      p_contrast,
      p_amplitude,
      p_depth,
      p_ripples,
      p_softEdges,
      p_edgeThresh,
      p_centerThresh,
      p_transpRange,
      p_ratio,
      p_placementMatrix,
      p_defaultColor,
      p_colorGain,
      p_colorOffset,
      p_invert,
      p_local,
      p_wrap
   };
}

node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterRGB("color1", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color2", 1.0f, 1.0f, 1.0f);
   AiParameterFlt("contrast", 0.5f);
   AiParameterFlt("amplitude", 1.0f);
   AiParameterVec2("depth", 0.0f, 8.0f);
   AiParameterVec("ripples", 1.0f, 1.0f, 1.0f);
   AiParameterBool("softEdges", true);
   AiParameterFlt("edgeThresh", 0.9f);
   AiParameterFlt("centerThresh", 0.0f);
   AiParameterFlt("transpRange", 0.5f);
   AiParameterFlt("ratio", 0.707f);
   AiParameterMtx("placementMatrix", id);
   AiParameterRGB("defaultColor", 0.5f, 0.5f, 0.5f);
   AiParameterRGB("colorGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("colorOffset", 0.0f, 0.0f, 0.0f);
   AiParameterBool("invert", false);
   AiParameterBool("local", false);
   AiParameterBool("wrap", true);

   AiMetaDataSetBool(nentry, "colorGain", "always_linear", true);
   AiMetaDataSetBool(nentry, "colorOffset", "always_linear", true);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "cloud");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544344);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   AtRGB color1 = AiShaderEvalParamRGB(p_color1);
   AtRGB color2 = AiShaderEvalParamRGB(p_color2);
   float contrast = AiShaderEvalParamFlt(p_contrast);
   float amplitude = AiShaderEvalParamFlt(p_amplitude);
   AtVector2 depth = AiShaderEvalParamVec2(p_depth);
   AtVector ripples = AiShaderEvalParamVec(p_ripples);
   bool softEdges = AiShaderEvalParamBool(p_softEdges);
   float edgeThresh = AiShaderEvalParamFlt(p_edgeThresh);
   float centerThresh = AiShaderEvalParamFlt(p_centerThresh);
   float transpRange = AiShaderEvalParamFlt(p_transpRange);
   float ratio = AiShaderEvalParamFlt(p_ratio);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   bool local = AiShaderEvalParamBool(p_local);
   bool wrap = AiShaderEvalParamBool(p_wrap);
   bool invert = AiShaderEvalParamBool(p_invert);
   AtRGB defaultColor = AiShaderEvalParamRGB(p_defaultColor);
   AtRGB colorGain = AiShaderEvalParamRGB(p_colorGain);
   AtRGB colorOffset = AiShaderEvalParamRGB(p_colorOffset);

   AtRGB result;

   AtVector tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AtVector P = AiM4PointByMatrixMult(*placementMatrix, (local ? sg->Po : sg->P));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      float iterations = AiMax(depth.x, depth.y);
      float loop = 0.0f;
      float curAmp = amplitude;
      float curFreq = 1.0f;
      float noise = 0.0f;

      P *= 0.5f * ripples;
      
      float maxP = (fabsf(P.x) > fabsf(P.y)) ? fabsf(P.x) : fabsf(P.y);
      maxP = (maxP > fabsf(P.z)) ? maxP : fabsf(P.z);

      float pixelSize = float(AI_EPSILON);
      float nyquist = 2.0f * pixelSize;
      float pixel = 1.0f;
      
      while (loop < iterations && pixel > nyquist)
      {
         if((maxP * curFreq) >= LONG_MAX)
            break;
         noise += curAmp * AiPerlin4(curFreq * P, 0.0f);
         curAmp *= ratio;
         curFreq *= 2.0f;
         
         pixel *= 0.5;
         
         loop += 1.0f;
      }

      noise = CLAMP((0.5f * noise) + 0.5f, 0.0f, 1.0f);

      noise = 0.5f * noise + 0.5f;

      noise = CLAMP((2.5f - centerThresh) / 2.5f, 0.0f, 1.0f) * pow(noise, 1.0f + edgeThresh);

      noise = SmoothStep(0.5f * (1.0f - transpRange), 0.5f * (1.0f + transpRange), noise);

      // Now apply color (use softEdges and contrast)

      AtRGB c0 = Mix(color2, color1, contrast);
      c0 = Mix(c0, color2, noise);

      if (softEdges)
      {
         c0 *= noise;
      }

      if (invert)
      {
         c0 = 1.0f - c0;
      }
      c0 *= colorGain;
      c0 += colorOffset;

      result = c0;
   }
   else
   {
      result = defaultColor;
   }

   if (usePref) RestorePoints(sg, tmpPts);
   sg->out.RGB() = result;

}
