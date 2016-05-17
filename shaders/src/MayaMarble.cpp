#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaMarbleMtd);

namespace
{
   enum MayaMarbleParams
   {
      p_fillerColor = 0,
      p_veinColor,
      p_veinWidth,
      p_diffusion,
      p_contrast,
      p_amplitude,
      p_ratio,
      p_ripples,
      p_depth,
      
      p_wrap,
      p_local,
      p_placementMatrix,
      MAYA_COLOR_BALANCE_ENUM
   };
}

node_parameters
{
   AtMatrix id;
   AiM4Identity(id);

   AiParameterRGB("fillerColor", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("veinColor", 0.298f, 0.0f, 0.0f);
   AiParameterFlt("veinWidth", 0.1f);
   AiParameterFlt("diffusion", 0.5f);
   AiParameterFlt("contrast", 0.5f);
   
   AiParameterFlt("amplitude", 1.5f);
   AiParameterFlt("ratio", 0.707f);
   AiParameterVec("ripples", 1.0f, 1.0f, 1.0f);
   AiParameterVec2("depth", 0.0f, 20.0f);
   
   
   AiParameterBool("wrap", true);
   AiParameterBool("local", false);
   AiParameterMtx("placementMatrix", id);
   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "marble");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544D52);
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
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   bool local = AiShaderEvalParamBool(p_local);
   bool wrap = AiShaderEvalParamBool(p_wrap);
   
   AtVector P, Q;

   AtVector tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AiM4PointByMatrixMult(&P, *placementMatrix, (local ? &(sg->Po) : &(sg->P)));
   
   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      AtRGB fillerColor = AiShaderEvalParamRGB(p_fillerColor);
      AtRGB veinColor = AiShaderEvalParamRGB(p_veinColor);
      float veinWidth = AiShaderEvalParamFlt(p_veinWidth);
      float diffusion = AiShaderEvalParamFlt(p_diffusion);
      float contrast = AiShaderEvalParamFlt(p_contrast);
      float amplitude = AiShaderEvalParamFlt(p_amplitude);
      
      float marble = 0.0f;
      if (amplitude > 0.0f)
      {
         float ratio = AiShaderEvalParamFlt(p_ratio);
         AtVector ripples = AiShaderEvalParamVec(p_ripples);
         AtVector2 depth = AiShaderEvalParamVec2(p_depth);
         
         
         float loop = 0.0f;
         
         float curAmp = 1.0f;
         float curFreq = 1.0f;
         
         Q = P * 0.5f * ripples;
         
         
         float maxQ = (fabsf(Q.x) > fabsf(Q.y)) ? fabsf(Q.x) : fabsf(Q.y);
         maxQ = (maxQ > fabsf(Q.z)) ? maxQ : fabsf(Q.z);
         
         int n;
         frexp (maxQ , &n);
         
         float iterations = AiMax(depth.x, depth.y);
         iterations = AiMin(iterations, (float)sizeof(float)*8-n);
         
         float pixelSize = float(AI_EPSILON);
         float nyquist = 2.0f * pixelSize;
         float pixel = 1.0f;
         
         while (loop < iterations && pixel > nyquist)
         {
            marble += curAmp * AiPerlin3(curFreq * Q + 0.5f);
            curAmp *= ratio;
            curFreq *= 2.0f;
            
            pixel *= 0.5f;
            loop += 1.0f;
         }
         curAmp *= (iterations - floorf(iterations));
         if(curAmp > 0.0f)
            marble += curAmp * AiPerlin3(curFreq * Q + 0.5f);

         marble *= amplitude * (1.0f - ratio*ratio);
         
      }
      marble += P.y;
      marble -= floorf(marble);
      if (marble >= 0.5f)
         marble = 1.0f - marble;
      marble *= 2.0f;
      
      float amount = 1.0f;
      
      if(marble >= veinWidth)
      {
         if (diffusion < 0.01f)
            diffusion = 0.01f;
         
         marble = expf((veinWidth - marble)/diffusion);
         marble *= 1.0f - contrast;
         amount = marble;
      }
      
      AtRGB c = Mix(fillerColor, veinColor, amount);

      sg->out.RGBA() = AtRGBA(c);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
      
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA());
   }
   if (usePref) RestorePoints(sg, tmpPts);
}
