#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaBrownianMtd);

namespace
{

enum MayaBrownianParams
{
   p_lacunarity,
   p_increment,
   p_octaves,
   p_weight3d,
   p_placementMatrix,
   p_wrap,
   p_local,
   MAYA_COLOR_BALANCE_ENUM
};

};

node_parameters
{
   AtMatrix id;
   AiM4Identity(id);

   AiParameterFLT("lacunarity", 4.0f);
   AiParameterFLT("increment", 0.1f);
   AiParameterFLT("octaves", 3.0f);
   AiParameterVEC("weight3d", 1.0f, 1.0f, 1.0f);
   AiParameterMTX("placementMatrix", id);
   AiParameterBOOL("wrap", true);
   AiParameterBOOL("local", false);
   AddMayaColorBalanceParams(params, mds);

   AiMetaDataSetStr(mds, NULL, "maya.name", "brownian");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x5246424d);
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
   bool wrap = AiShaderEvalParamBool(p_wrap);
   bool local = AiShaderEvalParamBool(p_local);

   AtPoint P;

   AtPoint tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AiM4PointByMatrixMult(&P, *placementMatrix, (local ? &(sg->Po) : &(sg->P)));

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      float lacunarity = AiShaderEvalParamFlt(p_lacunarity);
      float increment = AiShaderEvalParamFlt(p_increment);
      float octaves = AiShaderEvalParamFlt(p_octaves);
      AtVector weight3d = AiShaderEvalParamVec(p_weight3d);

      int i = 0;
      float curAmp = 1.0f;
      float curFreq = 1.0f;
      float noise = 0.0f;

      int ioctaves = int(floor(octaves));
      float foctave = octaves - float(ioctaves);

      float ratio = (float) exp(-0.5f * increment);

      P *= weight3d;

      while (i < ioctaves)
      {
         noise += curAmp * AiPerlin3(P * curFreq);
         curFreq *= lacunarity;
         curAmp *= ratio;
         ++i;
      }

      noise += foctave * curAmp * AiPerlin3(P * curFreq);

      noise = noise * 0.5f + 0.5f;

      AiRGBACreate(sg->out.RGBA, noise, noise, noise, 1.0f);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);
   }
   if (usePref) RestorePoints(sg, tmpPts);
}
