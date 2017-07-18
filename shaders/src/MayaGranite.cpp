#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"
#include "MayaNoiseUtils.h"

#define MAYA_CLAMP(x,a,b)	((x) < (a) ? (a) : (x) > (b) ? (b) : (x))

AI_SHADER_NODE_EXPORT_METHODS(MayaGraniteMtd);

namespace
{
   enum MayaGraniteParams
   {
      p_color1 = 0,
      p_color2,
      p_color3,
      p_fillerColor,
      p_cellSize,
      p_density,
      p_mixRatio,
      p_spottyness,
      p_randomness,
      p_threshold,
      p_creases,
      
      p_wrap,
      p_local,
      p_placementMatrix,
      MAYA_COLOR_BALANCE_ENUM
   };
   struct MayaGraniteData {
      bool placementMatrixLinked;
   };
}

node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterRGB("color1", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("color2", 0.549f, 0.784f, 0.392f);
   AiParameterRGB("color3", 0.627f, 0.824f, 0.823f);
   AiParameterRGB("fillerColor", 0.588f, 0.294f, 0.196f);
   AiParameterFlt("cellSize", 0.15f);
   AiParameterFlt("density", 1.0f);
   AiParameterFlt("mixRatio", 0.5f);
   AiParameterFlt("spottyness", 0.3f);
   AiParameterFlt("randomness", 1.0f);
   AiParameterFlt("threshold", 0.5f);
   AiParameterBool("creases", true);

   AiParameterBool("wrap", true);
   AiParameterBool("local", false);
   AiParameterMtx("placementMatrix", id);
   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "granite");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544D52);

   AiMetaDataSetStr(nentry, NULL, "_synonym", "mayaGranite");

}

node_initialize
{
   AiNodeSetLocalData(node, new MayaGraniteData());   
   MayaNoiseUtils::cell_initialize();
}

node_update
{
   MayaGraniteData* data =(MayaGraniteData*)AiNodeGetLocalData(node);
   data->placementMatrixLinked = AiNodeIsLinked(node, "placementMatrix");
}

node_finish
{
   delete (MayaGraniteData*)AiNodeGetLocalData(node);
}

shader_evaluate
{
   MayaGraniteData* data =(MayaGraniteData*)AiNodeGetLocalData(node);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   bool local = AiShaderEvalParamBool(p_local);
   bool wrap = AiShaderEvalParamBool(p_wrap);
   
   AtVector P;

   AtVector tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   P = AiM4PointByMatrixMult(*placementMatrix, (local ? sg->Po : sg->P));
   if (data->placementMatrixLinked)
      delete placementMatrix;

   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      float cellSize = AiShaderEvalParamFlt(p_cellSize);
      float edgeWidth = 1.0f - AiShaderEvalParamFlt(p_threshold);
      float spottyness = AiShaderEvalParamFlt(p_spottyness);
      float density = AiShaderEvalParamFlt(p_density);
      float randomness = AiShaderEvalParamFlt(p_randomness);
      bool creases = AiShaderEvalParamBool(p_creases);
      float mixRatio = AiShaderEvalParamFlt(p_mixRatio);

      if (cellSize < 0.0001f)
          cellSize = 0.0001f;
      if (edgeWidth < 0.0001f)
          edgeWidth = 0.0001f;
      if (density < AI_EPSILON)
          density = AI_EPSILON;
      if (mixRatio > 0.99f)
          mixRatio = 0.99f;
          
      AtVector levelCount;

      mixRatio = mixRatio / (1.0f - mixRatio);
      levelCount.x = powf(0.333f, mixRatio);
      levelCount.y = powf(0.666f, mixRatio);
      levelCount.z = 0.0f;

      /* Get cell noise. */
      float granite = MayaNoiseUtils::cellGetValue3d(
          cellSize, density, spottyness, randomness,
          edgeWidth, creases, &P, &levelCount);

      AtRGB outColor;

      if (creases) {
          AtRGB blendColor;
          if (granite > 20.0f) {
              granite -= 20.0f;
              granite = MAYA_CLAMP(granite, 0.0f, 1.0f);
          }
          else {
              granite = (granite < edgeWidth) ?
                  granite / edgeWidth : 1.0f;
          }

          if (levelCount.x > levelCount.y) {
              blendColor = (levelCount.x > levelCount.z) ?
                  AiShaderEvalParamRGB(p_color1) :
                  AiShaderEvalParamRGB(p_color3);
          }
          else {
              blendColor = (levelCount.y > levelCount.z) ?
                  AiShaderEvalParamRGB(p_color2) :
                  AiShaderEvalParamRGB(p_color3);
          }
          if (granite != 1.0f) {
              AtRGB fillerColor = AiShaderEvalParamRGB(p_fillerColor);
              outColor.r =
                  granite * (blendColor.r - fillerColor.r) + fillerColor.r;
              outColor.g =
                  granite * (blendColor.g - fillerColor.g) + fillerColor.g;
              outColor.b =
                  granite * (blendColor.b - fillerColor.b) + fillerColor.b;
          }
          else
              outColor = blendColor;
      }
      else {
          levelCount.x = (levelCount.x < edgeWidth) ?
              levelCount.x / edgeWidth : 1.0f;
          levelCount.y = (levelCount.y < edgeWidth) ?
              levelCount.y / edgeWidth : 1.0f;
          levelCount.z = (levelCount.z < edgeWidth) ?
              levelCount.z / edgeWidth : 1.0f;

          // start with black
          outColor.r = 0.0f;
          outColor.g = 0.0f;
          outColor.b = 0.0f;

          if (levelCount.x != 0.0f) {
              AtRGB color = AiShaderEvalParamRGB(p_color1);
              outColor.r += color.r * levelCount.x;
              outColor.g += color.g * levelCount.x;
              outColor.b += color.b * levelCount.x;
          }
          if (levelCount.y != 0.0f) {
              AtRGB color = AiShaderEvalParamRGB(p_color2);
              outColor.r += color.r * levelCount.y;
              outColor.g += color.g * levelCount.y;
              outColor.b += color.b * levelCount.y;
          }
          if (levelCount.z != 0.0f) {
              AtRGB color = AiShaderEvalParamRGB(p_color3);
              outColor.r += color.r * levelCount.z;
              outColor.g += color.g * levelCount.z;
              outColor.b += color.b * levelCount.z;
          }
          granite = levelCount.x + levelCount.y + levelCount.z;
          if (granite < 1.0f) {
              AtRGB fillerColor = AiShaderEvalParamRGB(p_fillerColor);
              float invGranite = 1.0f - granite;
              outColor.r += fillerColor.r * invGranite;
              outColor.g += fillerColor.g * invGranite;
              outColor.b += fillerColor.b * invGranite;
          }
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
