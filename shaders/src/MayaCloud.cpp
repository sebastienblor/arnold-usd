#include <ai.h>

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
   AtMatrix id;
   AiM4Identity(id);

   AiParameterRGB("color1", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color2", 1.0f, 1.0f, 1.0f);
   AiParameterFLT("contrast", 0.5f);
   AiParameterFLT("amplitude", 1.0f);
   AiParameterPNT2("depth", 0.0f, 8.0f);
   AiParameterVEC("ripples", 1.0f, 1.0f, 1.0f);
   AiParameterBOOL("softEdges", true);
   AiParameterFLT("edgeThresh", 0.9f);
   AiParameterFLT("centerThresh", 0.0f);
   AiParameterFLT("transpRange", 0.5f);
   AiParameterFLT("ratio", 0.707f);
   AiParameterMTX("placementMatrix", id);
   AiParameterRGB("defaultColor", 0.5f, 0.5f, 0.5f);
   AiParameterRGB("colorGain", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("colorOffset", 0.0f, 0.0f, 0.0f);
   AiParameterBOOL("invert", false);
   AiParameterBOOL("local", false);
   AiParameterBOOL("wrap", true);

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
   AtFloat contrast = AiShaderEvalParamFlt(p_contrast);
   AtFloat amplitude = AiShaderEvalParamFlt(p_amplitude);
   AtPoint2 depth = AiShaderEvalParamPnt2(p_depth);
   AtVector ripples = AiShaderEvalParamVec(p_ripples);
   AtBoolean softEdges = AiShaderEvalParamBool(p_softEdges);
   AtFloat edgeThresh = AiShaderEvalParamFlt(p_edgeThresh);
   AtFloat centerThresh = AiShaderEvalParamFlt(p_centerThresh);
   AtFloat transpRange = AiShaderEvalParamFlt(p_transpRange);
   AtFloat ratio = AiShaderEvalParamFlt(p_ratio);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   AtBoolean local = AiShaderEvalParamBool(p_local);
   AtBoolean wrap = AiShaderEvalParamBool(p_wrap);
   AtBoolean invert = AiShaderEvalParamBool(p_invert);
   AtRGB defaultColor = AiShaderEvalParamRGB(p_defaultColor);
   AtRGB colorGain = AiShaderEvalParamRGB(p_colorGain);
   AtRGB colorOffset = AiShaderEvalParamRGB(p_colorOffset);

   AtRGB result;
   AtPoint P;
   AtMatrix placement;

   AiM4Invert(*placementMatrix, placement);

   AiM4PointByMatrixMult(&P, placement, (local ? &(sg->Po) : &(sg->P)));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      float iterations = MAX(depth.x, depth.y);
      float loop = 0.0f;
      float curAmp = amplitude;
      float curFreq = 1.0f;
      float noise = 0.0f;

      P *= 0.5f * ripples;

      while (loop < iterations)
      {
         noise += curAmp * AiPerlin4(curFreq * P, 0.0f);
         curAmp *= ratio;
         curFreq *= 2.0f;
         loop += 1.0f;
      }

      noise = CLAMP((0.5f * noise) + 0.5f, 0.0f, 1.0f);

      noise = 0.5f * noise + 0.5f;

      noise = CLAMP((2.5f - centerThresh) / 2.5f, 0.0f, 1.0f) * pow(noise, 1.0f + edgeThresh);

      noise = SmoothStep(0.5f * (1.0f - transpRange), 0.5f * (1.0f + transpRange), noise);

      // Now apply color (use softEdges and contrast)

      AtColor c0 = Mix(color2, color1, contrast);
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

   sg->out.RGB = result;
}
