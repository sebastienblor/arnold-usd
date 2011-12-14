#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaSnowMtd);

namespace
{
   enum MayaSnowParams
   {
      p_snowColor = 0,
      p_surfaceColor,
      p_threshold,
      p_depthDecay,
      p_thickness,
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

   AiParameterRGB("snowColor", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("surfaceColor", 0.5f, 0.0f, 0.0f);
   AiParameterFLT("threshold", 0.5f);
   AiParameterFLT("depthDecay", 5.0f);
   AiParameterFLT("thickness", 1.0f);
   AiParameterBOOL("wrap", true);
   AiParameterBOOL("local", false);
   AiParameterMTX("placementMatrix", id);
   AddMayaColorBalanceParams(params);

   AiMetaDataSetStr(mds, NULL, "maya.name", "snow");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x5254534e);
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
   AtBoolean local = AiShaderEvalParamBool(p_local);
   AtBoolean wrap = AiShaderEvalParamBool(p_wrap);

   AtPoint P;

   AtPoint tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AiM4PointByMatrixMult(&P, *placementMatrix, (local ? &(sg->Po) : &(sg->P)));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      AtRGB snowColor = AiShaderEvalParamRGB(p_snowColor);
      AtRGB surfaceColor = AiShaderEvalParamRGB(p_surfaceColor);
      float threshold = AiShaderEvalParamFlt(p_threshold);
      float depthDecay = AiShaderEvalParamFlt(p_depthDecay);
      float thickness = AiShaderEvalParamFlt(p_thickness);
      AtVector U, N;

      AiV3Create(U, 0.0f, 1.0f, 0.0f);

      if (local)
      {
         AiM4VectorByMatrixMult(&N, sg->Minv, &(sg->N));
      }
      else
      {
         N = sg->N;
      }

      AiM4VectorByMatrixMult(&N, *placementMatrix, &N);
      AiV3Normalize(N, N);

      float NdU = AiV3Dot(N, U);

      float amount = 0.0f;

      if (NdU > threshold)
      {
         snowColor = Mix(snowColor, surfaceColor, 1.0f - thickness);

         amount = (NdU - threshold) / (1.0f - threshold);
         amount = 1.0f - pow((1.0f - amount), 0.5f * depthDecay);
      }

      AtRGB c = Mix(surfaceColor, snowColor, amount);

      AiRGBtoRGBA(c, sg->out.RGBA);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);
   }
   if (usePref) RestorePoints(sg, tmpPts);
}
