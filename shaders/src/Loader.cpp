#include <ai.h>

#include <cstdio>

extern const AtNodeMethods* MayaBulgeMtd;
extern const AtNodeMethods* MayaClothMtd;
extern const AtNodeMethods* MayaGridMtd;
extern const AtNodeMethods* MayaFractalMtd;
extern const AtNodeMethods* MayaNoiseMtd;
extern const AtNodeMethods* MayaEnvSphereMtd;
extern const AtNodeMethods* MayaCloudMtd;
extern const AtNodeMethods* MayaCloudAlphaMtd;
extern const AtNodeMethods* MayaSnowMtd;
extern const AtNodeMethods* MayaSolidFractalMtd;
extern const AtNodeMethods* MayaVolumeNoiseMtd;
extern const AtNodeMethods* MayaBrownianMtd;
extern const AtNodeMethods* MayaStuccoMtd;
extern const AtNodeMethods* WriteColorMtd;
extern const AtNodeMethods* WriteFloatMtd;
extern const AtNodeMethods* ShadowCatcherMtd;
extern const AtNodeMethods* MayaFluidMtd;
extern const AtNodeMethods* MayaMarbleMtd;
extern const AtNodeMethods* MayaMountainMtd;
extern const AtNodeMethods* MayaWaterMtd;
extern const AtNodeMethods* MayaCraterMtd;
extern const AtNodeMethods* MayaLeatherMtd;
extern const AtNodeMethods* MayaGraniteMtd;
extern const AtNodeMethods* MayaRockMtd;
extern const AtNodeMethods* MayaFluidDataMtd;
extern const AtNodeMethods* MayaFluidTexture2DMtd;
extern const AtNodeMethods* SkinMtd;
extern const AtNodeMethods* VolumeCollectorMtd;
extern const AtNodeMethods* MPlayDriverMtd;
extern const AtNodeMethods* VolumeSampleRgbMethods;
extern const AtNodeMethods* VolumeSampleFloatMethods;
extern const AtNodeMethods* CurvatureMethods;
extern const AtNodeMethods* ThinFilmMethods;
extern const AtNodeMethods* ComplexIORMethods;

enum{
   SHADER_BULGE = 0,
   SHADER_CLOTH,
   SHADER_GRID,
   SHADER_FRACTAL,
   SHADER_NOISE,
   SHADER_CLOUD,
   SHADER_CLOUDALPHA,
   SHADER_SNOW,
   SHADER_SOLIDFRACTAL,
   SHADER_VOLUMENOISE,
   SHADER_BROWNIAN,
   SHADER_STUCCO,
   SHADER_WRITECOLOR,
   SHADER_WRITEFLOAT,
   SHADER_ENVSPHERE,
   SHADER_MAYAFLUID,
   SHADER_MAYAMARBLE,
   SHADER_MAYAMOUNTAIN,
   SHADER_MAYAWATER,
   SHADER_MAYACRATER,
   SHADER_MAYALEATHER,
   SHADER_MAYAGRANITE,
   SHADER_MAYAROCK,
   SHADER_MAYAFLUIDDATA,
   SHADER_MAYAFLUIDTEXTURE2D
};

node_loader
{
   switch (i)
   {

   case SHADER_BULGE:
      node->methods     = MayaBulgeMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaBulge";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_CLOTH:
      node->methods     = MayaClothMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaCloth";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_GRID:
      node->methods     = MayaGridMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaGrid";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_FRACTAL:
      node->methods     = MayaFractalMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaFractal";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_NOISE:
      node->methods     = MayaNoiseMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaNoise";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_CLOUD:
      node->methods     = MayaCloudMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaCloud";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_CLOUDALPHA:
      node->methods     = MayaCloudAlphaMtd;
      node->output_type = AI_TYPE_FLOAT;
      node->name        = "MayaCloudAlpha";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_SNOW:
      node->methods     = MayaSnowMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaSnow";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_SOLIDFRACTAL:
      node->methods     = MayaSolidFractalMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaSolidFractal";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_VOLUMENOISE:
      node->methods     = MayaVolumeNoiseMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaVolumeNoise";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_BROWNIAN:
      node->methods     = MayaBrownianMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaBrownian";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_STUCCO:
      node->methods     = MayaStuccoMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaStucco";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_WRITECOLOR:
      node->methods     = WriteColorMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MtoaWriteColor";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_WRITEFLOAT:
      node->methods     = WriteFloatMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MtoaWriteFloat";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_ENVSPHERE:
      node->methods     = MayaEnvSphereMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaEnvSphere";
      node->node_type   = AI_NODE_SHADER;
      break;      
      
   case SHADER_MAYAFLUID:
      node->methods     = MayaFluidMtd;
      node->output_type = AI_TYPE_CLOSURE;
      node->name        = "MayaFluid";
      node->node_type   = AI_NODE_SHADER;
      break;
      
   case SHADER_MAYAMARBLE:
      node->methods     = MayaMarbleMtd;
      node->output_type = AI_TYPE_RGBA;
      node->name        = "MayaMarble";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_MAYAMOUNTAIN:
       node->methods = MayaMountainMtd;
       node->output_type = AI_TYPE_RGBA;
       node->name = "MayaMountain";
       node->node_type = AI_NODE_SHADER;
       break;

   case SHADER_MAYAWATER:
       node->methods = MayaWaterMtd;
       node->output_type = AI_TYPE_RGBA;
       node->name = "MayaWater";
       node->node_type = AI_NODE_SHADER;
       break;

   case SHADER_MAYACRATER:
       node->methods = MayaCraterMtd;
       node->output_type = AI_TYPE_RGBA;
       node->name = "MayaCrater";
       node->node_type = AI_NODE_SHADER;
       break;

   case SHADER_MAYALEATHER:
       node->methods = MayaLeatherMtd;
       node->output_type = AI_TYPE_RGBA;
       node->name = "MayaLeather";
       node->node_type = AI_NODE_SHADER;
       break;

   case SHADER_MAYAGRANITE:
       node->methods = MayaGraniteMtd;
       node->output_type = AI_TYPE_RGBA;
       node->name = "MayaGranite";
       node->node_type = AI_NODE_SHADER;
       break;

   case SHADER_MAYAROCK:
       node->methods = MayaRockMtd;
       node->output_type = AI_TYPE_RGBA;
       node->name = "MayaRock";
       node->node_type = AI_NODE_SHADER;
       break;

   case SHADER_MAYAFLUIDDATA:
      node->methods     = MayaFluidDataMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaFluidData";
      node->node_type   = AI_NODE_SHADER;
      break;

   case SHADER_MAYAFLUIDTEXTURE2D:
      node->methods     = MayaFluidTexture2DMtd;
      node->output_type = AI_TYPE_RGB;
      node->name        = "MayaFluidTexture2D";
      node->node_type   = AI_NODE_SHADER;
      break;
   
   default:
      return false;
   }

   sprintf(node->version, AI_VERSION);

   return true;
}
