#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaCloudAlphaMtd);

namespace
{
   enum MayaCloudAlphaParams
   {
      p_amplitude,
      p_depth,
      p_ripples,
      p_edgeThresh,
      p_centerThresh,
      p_transpRange,
      p_ratio,
      p_placementMatrix,
      p_alphaGain,
      p_alphaOffset,
      p_invert,
      p_local,
      p_wrap
   };
}

node_parameters
{
   AtMatrix id;
   AiM4Identity(id);

   AiParameterFlt("amplitude", 1.0f);
   AiParameterVec2("depth", 0.0f, 8.0f);
   AiParameterVec("ripples", 1.0f, 1.0f, 1.0f);
   AiParameterFlt("edgeThresh", 0.9f);
   AiParameterFlt("centerThresh", 0.0f);
   AiParameterFlt("transpRange", 0.5f);
   AiParameterFlt("ratio", 0.707f);
   AiParameterMtx("placementMatrix", id);
   AiParameterFlt("alphaGain", 1.0f);
   AiParameterFlt("alpahOffset", 0.0f);
   AiParameterBool("invert", false);
   AiParameterBool("local", false);
   AiParameterBool("wrap", true);

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
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
   float amplitude = AiShaderEvalParamFlt(p_amplitude);
   AtVector2 depth = AiShaderEvalParamVec2(p_depth);
   AtVector ripples = AiShaderEvalParamVec(p_ripples);
   float edgeThresh = AiShaderEvalParamFlt(p_edgeThresh);
   float centerThresh = AiShaderEvalParamFlt(p_centerThresh);
   float transpRange = AiShaderEvalParamFlt(p_transpRange);
   float ratio = AiShaderEvalParamFlt(p_ratio);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   bool local = AiShaderEvalParamBool(p_local);
   bool wrap = AiShaderEvalParamBool(p_wrap);
   bool invert = AiShaderEvalParamBool(p_invert);
   float alphaGain = AiShaderEvalParamFlt(p_alphaGain);
   float alphaOffset = AiShaderEvalParamFlt(p_alphaOffset);

   float result;
   AtVector P;

   AtVector tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AiM4PointByMatrixMult(&P, *placementMatrix, (local ? &(sg->Po) : &(sg->P)));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      float iterations = AiMax(depth.x, depth.y);
      float i = 0.0f;
      float curAmp = amplitude;
      float curFreq = 1.0f;
      float noise = 0.0f;

      P *= 0.5f * ripples;

      while (i < iterations)
      {
         noise += curAmp * AiPerlin4(curFreq * P, 0.0f);
         curAmp *= ratio;
         curFreq *= 2.0f;
         i += 1.0f;
      }

      noise = CLAMP((0.5f * noise) + 0.5f, 0.0f, 1.0f);

      noise = 0.5f * noise + 0.5f;

      noise = CLAMP((2.5f - centerThresh) / 2.5f, 0.0f, 1.0f) * pow(noise, 1.0f + edgeThresh);

      result = SmoothStep(0.5f * (1.0f - transpRange), 0.5f * (1.0f + transpRange), noise);

      if (invert)
      {
         result = 1.0f - result;
      }

      result *= alphaGain;
      result += alphaOffset;
   }
   else
   {
      result = 0.0f;
   }

   if (usePref) RestorePoints(sg, tmpPts);
   sg->out.FLT() = result;
}
