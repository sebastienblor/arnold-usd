#include <ai.h>

#include <cstdio>

extern AtNodeMethods* MayaMultiplyDivideMtd;
extern AtNodeMethods* MayaClampMtd;
extern AtNodeMethods* MayaGammaCorrectMtd;
extern AtNodeMethods* MayaConditionMtd;
extern AtNodeMethods* MayaReverseMtd;
extern AtNodeMethods* MayaBlendColorsMtd;
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
extern AtNodeMethods* MayaCheckerMtd;
extern AtNodeMethods* MayaBulgeMtd;
extern AtNodeMethods* MayaClothMtd;
extern AtNodeMethods* MayaGridMtd;
extern AtNodeMethods* MayaFractalMtd;
extern AtNodeMethods* MayaNoiseMtd;
extern AtNodeMethods* MayaPlace3DTextureMtd;
extern AtNodeMethods* MayaEnvSphereMtd;
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
extern AtNodeMethods* MayaSurfaceShaderMtd;
extern AtNodeMethods* ColorToFloatMtd;
extern AtNodeMethods* VectorToFloatMtd;
extern AtNodeMethods* PointToFloatMtd;
extern AtNodeMethods* Point2ToFloatMtd;
extern AtNodeMethods* WriteColorMtd;
extern AtNodeMethods* WriteFloatMtd;
extern AtNodeMethods* WriteColorInlineMtd;
extern AtNodeMethods* WriteFloatInlineMtd;
extern AtNodeMethods* WriteVectorInlineMtd;
extern AtNodeMethods* WritePointInlineMtd;
extern AtNodeMethods* WritePoint2InlineMtd;
extern AtNodeMethods* AnimMatrixMtd;
extern AtNodeMethods* AnimFloatMtd;
extern AtNodeMethods* AnimPointMtd;
extern AtNodeMethods* AnimVectorMtd;
extern AtNodeMethods* AnimColorMtd;
extern AtNodeMethods* UserDataFloatMtd;
extern AtNodeMethods* UserDataVectorMtd;
extern AtNodeMethods* UserDataColorMtd;
extern AtNodeMethods* UserDataStringMtd;
extern AtNodeMethods* UserDataBoolMtd;
extern AtNodeMethods* MayaShadingEngineMtd;
extern AtNodeMethods* SkinSssMethods;
extern AtNodeMethods* MayaSamplerInfo1DMtd;
extern AtNodeMethods* MayaSamplerInfo2DMtd;
extern AtNodeMethods* MayaSamplerInfo3DMtd;
extern AtNodeMethods* MayaVectorDisplacementMtd;
extern AtNodeMethods* MayaNormalDisplacementMtd;
extern AtNodeMethods* ShadowCatcherMtd;
extern AtNodeMethods* MayaHairMtd;
extern AtNodeMethods* MeshLightMaterialMtd;
<<<<<<< local
extern AtNodeMethods* MayaFluidMtd;
=======
extern AtNodeMethods* SocketDriverMtd;
extern AtNodeMethods* UserDataPnt2Mtd;
extern AtNodeMethods* UserDataIntMtd;
extern AtNodeMethods* MayaBump2DMtd;
>>>>>>> other

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
      node->methods     = MayaSamplerInfo1DMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaSamplerInfo1D";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 7:
      node->methods     = MayaSamplerInfo2DMtd;
      node->output_type = AI_TYPE_POINT2;
      node->name        = "MayaSamplerInfo2D";
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
      node->methods     = MayaCheckerMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaChecker";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 20:
      node->methods     = MayaBulgeMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaBulge";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 21:
      node->methods     = MayaClothMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaCloth";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 22:
      node->methods     = MayaGridMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaGrid";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 23:
      node->methods     = MayaFractalMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaFractal";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 24:
      node->methods     = MayaNoiseMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaNoise";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 25:
      node->methods     = MayaPlace3DTextureMtd;
      node->output_type = AI_TYPE_MATRIX;
      node->name        = "MayaPlace3DTexture";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 26:
      node->methods     = MayaRgbToHsvMtd;
      node->output_type = AI_TYPE_VECTOR;
      node->name        = "MayaRgbToHsv";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 27:
      node->methods     = MayaHsvToRgbMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaHsvToRgb";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 28:
      node->methods     = MayaLuminanceMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaLuminance";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 29:
      node->methods     = MayaCloudMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaCloud";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 30:
      node->methods     = MayaCloudAlphaMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaCloudAlpha";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 31:
      node->methods     = MayaSnowMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaSnow";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 32:
      node->methods     = MayaContrastMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaContrast";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 33:
      node->methods     = MayaLayeredTextureMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaLayeredTexture";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 34:
      node->methods     = MayaLayeredShaderMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaLayeredShader";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 35:
      node->methods     = MayaSolidFractalMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaSolidFractal";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 36:
      node->methods     = MayaVolumeNoiseMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaVolumeNoise";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 37:
      node->methods     = MayaBrownianMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaBrownian";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 38:
      node->methods     = MayaStuccoMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaStucco";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 39:
      node->methods     = MayaRemapHsvMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaRemapHsv";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 40:
      node->methods     = MayaSetRangeMtd;
      node->output_type = AI_TYPE_VECTOR;
      node->name        = "MayaSetRange";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 41:
      node->methods     = MayaImagePlaneMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaImagePlane";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 42:
      node->methods     = MayaSurfaceShaderMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaSurfaceShader";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 43:
      node->methods     = ColorToFloatMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "colorToFloat";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 44:
      node->methods     = VectorToFloatMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "vectorToFloat";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 45:
      node->methods     = PointToFloatMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "pointToFloat";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 46:
      node->methods     = Point2ToFloatMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "point2ToFloat";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 47:
      node->methods     = WriteColorMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "writeColor";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 48:
      node->methods     = WriteColorInlineMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "aovWriteColor";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 49:
      node->methods     = WriteFloatMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "writeFloat";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 50:
      node->methods     = WriteFloatInlineMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "aovWriteFloat";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 51:
      node->methods     = MayaEnvSphereMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaEnvSphere";
      node->node_type   = AI_NODE_SHADER;
      break;      

   case 52:
      node->methods     = AnimMatrixMtd;
      node->output_type = AI_TYPE_MATRIX;
      node->name        = "anim_matrix";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 53:
      node->methods     = AnimFloatMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "anim_float";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 54:
      node->methods     = AnimPointMtd;
      node->output_type = AI_TYPE_POINT;
      node->name        = "anim_point";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 55:
      node->methods     = AnimVectorMtd;
      node->output_type = AI_TYPE_VECTOR;
      node->name        = "anim_vector";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 56:
      node->methods     = AnimColorMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "anim_color";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 57:
      node->methods     = UserDataFloatMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "userDataFloat";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 58:
      node->methods     = UserDataVectorMtd;
      node->output_type = AI_TYPE_VECTOR;
      node->name        = "userDataVector";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 59:
      node->methods     = UserDataColorMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "userDataColor";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 60:
      node->methods     = UserDataStringMtd;
      node->output_type = AI_TYPE_STRING;
      node->name        = "userDataString";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 61:
      node->methods     = MayaShadingEngineMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaShadingEngine";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 62:
      node->methods     = SkinSssMethods;
      node->output_type = AI_TYPE_RGB;
      node->name        = "skin_sss";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 63:
      node->methods     = MayaSamplerInfo3DMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaSamplerInfo3D";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 64:
      node->methods     = WriteVectorInlineMtd;
      node->output_type = AI_TYPE_VECTOR;
      node->name        = "aovWriteVector";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 65:
      node->methods     = WritePointInlineMtd;
      node->output_type = AI_TYPE_POINT;
      node->name        = "aovWritePoint";
      node->node_type   = AI_NODE_SHADER;
      break;

   case 66:
      node->methods     = WritePoint2InlineMtd;
      node->output_type = AI_TYPE_POINT2;
      node->name        = "aovWritePoint2";
      node->node_type   = AI_NODE_SHADER;
      break;
      
   case 67:
      node->methods     = MayaNormalDisplacementMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaNormalDisplacement";
      node->node_type   = AI_NODE_SHADER;
      break;
      
   case 68:
      node->methods     = MayaVectorDisplacementMtd;
      node->output_type = AI_TYPE_VECTOR;
      node->name        = "MayaVectorDisplacement";
      node->node_type   = AI_NODE_SHADER;
      break;
      
   case 69:
      node->methods     = UserDataBoolMtd;
      node->output_type = AI_TYPE_BOOLEAN;
      node->name        = "userDataBool";
      node->node_type   = AI_NODE_SHADER;
      break;
      
   case 70:
      node->methods     = ShadowCatcherMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "shadowCatcher";
      node->node_type   = AI_NODE_SHADER;
      break;
      
   case 71:
      node->methods     = MayaHairMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaHair";
      node->node_type   = AI_NODE_SHADER;
      break;
      
   case 72:
      node->methods     = MeshLightMaterialMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "meshLightMaterial";
      node->node_type   = AI_NODE_SHADER;
      break;
<<<<<<< local
   
=======
      
   case 73:
      node->methods     = SocketDriverMtd;
      node->name        = "driver_socket";
      node->node_type   = AI_NODE_DRIVER;
      break;
      
>>>>>>> other
   case 74:
<<<<<<< local
      node->methods     = MayaFluidMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "mayaFluid";
=======
      node->methods     = UserDataPnt2Mtd;
      node->output_type = AI_TYPE_POINT2;
      node->name        = "userDataPnt2";
>>>>>>> other
      node->node_type   = AI_NODE_SHADER;
      break;
<<<<<<< local
=======
      
   case 75:
      node->methods     = UserDataIntMtd;
      node->output_type = AI_TYPE_INT;
      node->name        = "userDataInt";
      node->node_type   = AI_NODE_SHADER;
      break;
      
   case 76:
      node->methods     = MayaBump2DMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "mayaBump2D";
      node->node_type   = AI_NODE_SHADER;
      break;
>>>>>>> other

   default:
      return false;
   }

   sprintf(node->version, AI_VERSION);

   return true;
}
