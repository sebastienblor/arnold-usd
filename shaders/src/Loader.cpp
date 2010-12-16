
#include <ai.h>

#include <cstdio>

extern AtNodeMethods* MayaMultiplyDivideMtd;
extern AtNodeMethods* MayaClampMtd;
extern AtNodeMethods* MayaGammaCorrectMtd;
extern AtNodeMethods* MayaConditionMtd;
extern AtNodeMethods* MayaReverseMtd;
extern AtNodeMethods* MayaBlendColorsMtd;
extern AtNodeMethods* MayaFacingRatioMtd;
extern AtNodeMethods* MayaFlippedNormalMtd;
extern AtNodeMethods* MayaPlusMinusAverage1DMtd;
extern AtNodeMethods* MayaPlusMinusAverage2DMtd;
extern AtNodeMethods* MayaPlusMinusAverage3DMtd;
extern AtNodeMethods* MayaRemapValueToValueMtd;
extern AtNodeMethods* MayaRemapValueToColorMtd;
extern AtNodeMethods* MayaRemapColorMtd;
extern AtNodeMethods* MayaFileMtd;
extern AtNodeMethods* MayaPlace2dTextureMtd;
extern AtNodeMethods* MayaRampMtd;
extern AtNodeMethods* MayaProjectionMtd;
extern AtNodeMethods* Utility2Mtd;

node_loader
{
   switch (i)
   {
   case 0:
      node->methods     = MayaMultiplyDivideMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaMultiplyDivide";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 1:
      node->methods     = MayaClampMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaClamp";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 2:
      node->methods     = MayaGammaCorrectMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaGammaCorrect";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 3:
      node->methods     = MayaConditionMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaCondition";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 4:
      node->methods     = MayaReverseMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaReverse";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 5:
      node->methods     = MayaBlendColorsMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaBlendColors";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 6:
      node->methods     = MayaFacingRatioMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaFacingRatio";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 7:
      node->methods     = MayaFlippedNormalMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaFlippedNormal";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 8:
      node->methods     = MayaPlusMinusAverage1DMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaPlusMinusAverage1D";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 9:
      node->methods     = MayaPlusMinusAverage2DMtd;
      node->output_type = AI_TYPE_POINT2;
      node->name        = "MayaPlusMinusAverage2D";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 10:
      node->methods     = MayaPlusMinusAverage3DMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaPlusMinusAverage3D";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 11:
      node->methods     = MayaRemapValueToValueMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaRemapValueToValue";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 12:
      node->methods     = MayaRemapValueToColorMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaRemapValueToColor";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 13:
      node->methods     = MayaRemapColorMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaRemapColor";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 14:
      node->methods     = MayaFileMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaFile";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 15:
      node->methods     = MayaPlace2dTextureMtd;
      node->output_type = AI_TYPE_POINT2;
      node->name        = "MayaPlace2dTexture";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 16:
      node->methods     = MayaRampMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaRamp";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 17:
      node->methods     = MayaProjectionMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaProjection";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 18:
      node->methods     = Utility2Mtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "utility2";
      node->node_type   = AI_NODE_SHADER;
      break;

   default:
      return FALSE;
   }

   sprintf(node->version, AI_VERSION);

   return TRUE;
}
