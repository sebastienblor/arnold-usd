
#include <ai.h>

#include <stdio.h>

extern void* MayaMultiplyDivideMtd;

node_loader
{
   switch (i)
   {
   case 0:
      node->methods		= MayaMultiplyDivideMtd;
      node->output_type	= AI_TYPE_RGB;
      node->name			= "MayaMultiplyDivide";
      node->node_type	= AI_NODE_SHADER;
      break;

   default:
      return FALSE;
   }

   sprintf(node->version, AI_VERSION);

   return TRUE;
}
