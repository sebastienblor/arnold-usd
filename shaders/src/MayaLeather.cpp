#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"
#include "MayaNoiseUtils.h"

#define MAYA_CLAMP(x,a,b)	((x) < (a) ? (a) : (x) > (b) ? (b) : (x))

AI_SHADER_NODE_EXPORT_METHODS(MayaLeatherMtd);

namespace
{
   enum MayaLeatherParams
   {
      p_cellColor = 0,
      p_creaseColor,
      p_cellSize,
      p_density,
      p_spottyness,
      p_randomness,
      p_threshold,
      p_creases,
      
      p_wrap,
      p_local,
      p_placementMatrix,
      MAYA_COLOR_BALANCE_ENUM
   };
}

node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterRGB("cellColor", 0.373f, 0.157f, 0.059f);
   AiParameterRGB("creaseColor", 0.235f, 0.118f, 0.0f);
   AiParameterFlt("cellSize", 0.5f);
   AiParameterFlt("density", 1.0f);
   AiParameterFlt("spottyness", 0.1f);
   AiParameterFlt("randomness", 0.5f);
   AiParameterFlt("threshold", 0.83f);
   AiParameterBool("creases", true);

   AiParameterBool("wrap", true);
   AiParameterBool("local", false);
   AiParameterMtx("placementMatrix", id);
   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "leather");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544D52);
   AiMetaDataSetStr(nentry, NULL, "_synonym", "mayaLeather");

}

node_initialize
{
    MayaNoiseUtils::cell_initialize();
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
   
   AtVector P;

   AtVector tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   P = AiM4PointByMatrixMult(*placementMatrix, (local ? sg->Po : sg->P));
   
   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      //AtRGB channel1 = AiShaderEvalParamRGB(p_channel1);
      float cellSize = AiShaderEvalParamFlt(p_cellSize);
      float edgeWidth = 1.0f - AiShaderEvalParamFlt(p_threshold);
      float spottyness = AiShaderEvalParamFlt(p_spottyness);
      float density = AiShaderEvalParamFlt(p_density);
      float randomness = AiShaderEvalParamFlt(p_randomness);
      bool creases = AiShaderEvalParamBool(p_creases);

      if (cellSize < 0.0001f)
          cellSize = 0.0001f;
      if (edgeWidth < 0.0001f)
          edgeWidth = 0.0001f;
      if (density < AI_EPSILON)
          density = AI_EPSILON;

      /* Get cell noise. */
      float leather = MayaNoiseUtils::cellGetValue3d(
          cellSize, density, spottyness, randomness,
          edgeWidth, creases, &P, NULL);

      if (creases && leather >= 20.0f) {
          leather -= 20.0f;
          leather = MAYA_CLAMP(leather, 0.0f, 1.0f);
      }
      else
          leather = (leather < edgeWidth) ?
          leather / edgeWidth : 1.0f;

      AtRGB outColor;
      if (leather == 0.0f)
      {
          outColor.r = 0.0f;
          outColor.g = 0.0f;
          outColor.b = 0.0f;
      }
      else {
          outColor = AiShaderEvalParamRGB(p_cellColor);
          outColor.r *= leather;
          outColor.g *= leather;
          outColor.b *= leather;
      }
      if (leather != 1.0f) {
          AtRGB creaseColor = AiShaderEvalParamRGB(p_creaseColor);
          outColor.r += (1.0f - leather) * creaseColor.r;
          outColor.g += (1.0f - leather) * creaseColor.g;
          outColor.b += (1.0f - leather) * creaseColor.b;
      }

      sg->out.RGBA() = AtRGBA(outColor);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
      
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA());
   }
   if (usePref) RestorePoints(sg, tmpPts);
}
