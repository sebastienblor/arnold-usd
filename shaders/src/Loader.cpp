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
extern AtNodeMethods* MayaPlace2DTextureMtd;
extern AtNodeMethods* MayaRampMtd;
extern AtNodeMethods* MayaProjectionMtd;
extern AtNodeMethods* MayaLuminanceMtd;
extern AtNodeMethods* MayaSetRangeMtd;
extern AtNodeMethods* MeshInfoMtd;
extern AtNodeMethods* TangentToObjectSpaceMtd;
extern AtNodeMethods* MayaCheckerMtd;
extern AtNodeMethods* MayaBulgeMtd;
extern AtNodeMethods* MayaClothMtd;
extern AtNodeMethods* MayaGridMtd;
extern AtNodeMethods* MayaFractalMtd;
extern AtNodeMethods* MayaNoiseMtd;
extern AtNodeMethods* MayaPlace3DTextureMtd;
extern AtNodeMethods* MayaRgbToHsvMtd;
extern AtNodeMethods* MayaHsvToRgbMtd;
extern AtNodeMethods* MayaLuminanceMtd;
extern AtNodeMethods* MayaCloudMtd;
extern AtNodeMethods* MayaCloudAlphaMtd;
extern AtNodeMethods* MayaSnowMtd;
extern AtNodeMethods* MayaContrastMtd;
extern AtNodeMethods* MayaLayeredTextureMtd;
extern AtNodeMethods* MayaLayeredShaderMtd;
extern AtNodeMethods* MayaSolidFractalMtd;
extern AtNodeMethods* MayaVolumeNoiseMtd;
extern AtNodeMethods* MayaBrownianMtd;
extern AtNodeMethods* MayaStuccoMtd;
extern AtNodeMethods* MayaRemapHsvMtd;
extern AtNodeMethods* MayaImagePlaneMtd;
extern AtNodeMethods* ColorToFloatMtd;

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
      node->methods     = MayaPlace2DTextureMtd;
      node->output_type = AI_TYPE_POINT2;
      node->name        = "MayaPlace2DTexture";
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
      node->methods     = MeshInfoMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MeshInfo";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 19:
      node->methods     = TangentToObjectSpaceMtd;
      node->output_type = AI_TYPE_VECTOR;
      node->name        = "TangentToObjectSpace";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 20:
      node->methods     = MayaCheckerMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaChecker";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 21:
      node->methods     = MayaBulgeMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaBulge";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 22:
      node->methods     = MayaClothMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaCloth";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 23:
      node->methods     = MayaGridMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaGrid";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 24:
      node->methods     = MayaFractalMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaFractal";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 25:
      node->methods     = MayaNoiseMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaNoise";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 26:
      node->methods     = MayaPlace3DTextureMtd;
      node->output_type = AI_TYPE_MATRIX;
      node->name        = "MayaPlace3DTexture";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 27:
      node->methods     = MayaRgbToHsvMtd;
      node->output_type = AI_TYPE_VECTOR;
      node->name        = "MayaRgbToHsv";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 28:
      node->methods     = MayaHsvToRgbMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaHsvToRgb";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 29:
      node->methods     = MayaLuminanceMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaLuminance";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 30:
      node->methods     = MayaCloudMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaCloud";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 31:
      node->methods     = MayaCloudAlphaMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaCloudAlpha";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 32:
      node->methods     = MayaSnowMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaSnow";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 33:
      node->methods     = MayaContrastMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaContrast";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 34:
      node->methods     = MayaLayeredTextureMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaLayeredTexture";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 35:
      node->methods     = MayaLayeredShaderMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaLayeredShader";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 36:
      node->methods     = MayaSolidFractalMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaSolidFractal";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 37:
      node->methods     = MayaVolumeNoiseMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaVolumeNoise";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 38:
      node->methods     = MayaBrownianMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaBrownian";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 39:
      node->methods     = MayaStuccoMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaStucco";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 40:
      node->methods     = MayaRemapHsvMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaRemapHsv";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 41:
      node->methods     = MayaSetRangeMtd;
      node->output_type = AI_TYPE_VECTOR;
      node->name        = "MayaSetRange";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 42:
      node->methods     = MayaImagePlaneMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaImagePlane";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 43:
      node->methods     = ColorToFloatMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "colorToFloat";
      node->node_type   = AI_NODE_SHADER;
      break;

   default:
      return FALSE;
   }

   sprintf(node->version, AI_VERSION);

   return TRUE;
}
