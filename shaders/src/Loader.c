
#include <ai.h>

#include <stdio.h>

extern void* MayaMultiplyDivideMtd;
extern void* MayaClampMtd;
extern void* MayaGammaCorrectMtd;

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

   default:
      return FALSE;
   }

   sprintf(node->version, AI_VERSION);

   return TRUE;
}
