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

   AiMetaDataSetStr(mds, NULL, "maya.counterpart", "snow");
   AiMetaDataSetInt(mds, NULL, "maya.counterpart_id", 0x5254534e);
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
   AtRGB snowColor = AiShaderEvalParamRGB(p_snowColor);
   AtRGB surfaceColor = AiShaderEvalParamRGB(p_surfaceColor);
   AtFloat threshold = AiShaderEvalParamFlt(p_threshold);
   AtFloat depthDecay = AiShaderEvalParamFlt(p_depthDecay);
   AtFloat thickness = AiShaderEvalParamFlt(p_thickness);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   AtBoolean local = AiShaderEvalParamBool(p_local);
   AtBoolean wrap = AiShaderEvalParamBool(p_wrap);

   AtPoint P;
   AtMatrix placement;

   AtPoint tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AiM4Invert(*placementMatrix, placement);

   AiM4PointByMatrixMult(&P, placement, (local ? &(sg->Po) : &(sg->P)));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
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

      AiM4VectorByMatrixMult(&N, placement, &N);
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
