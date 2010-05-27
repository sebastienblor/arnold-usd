
#include <ai.h>

#include <stdio.h>

extern AtNodeMethods* MayaMultiplyDivideMtd;
extern AtNodeMethods* MayaClampMtd;
extern AtNodeMethods* MayaGammaCorrectMtd;
extern AtNodeMethods* MayaConditionMtd;
extern AtNodeMethods* MayaReverseMtd;
extern AtNodeMethods* MayaBlendColorsMtd;
extern AtNodeMethods* MayaFacingRatioMtd;
extern AtNodeMethods* MayaFlippedNormalMtd;

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

   default:
      return FALSE;
   }

   sprintf(node->version, AI_VERSION);

   return TRUE;
}
