#include <ai.h>
#include <cstdio>

extern AtNodeMethods* MyShader1Mtd;
extern AtNodeMethods* MyShader2Mtd;

node_loader
{
   switch (i)
   {
   case 0:
      node->methods      = MyShader1Mtd;
      node->output_type  = AI_TYPE_RGB;
      node->name         = "myShader1";
      node->node_type    = AI_NODE_SHADER;
      break;

   case 1:
      node->methods      = MyShader2Mtd;
      node->output_type  = AI_TYPE_RGB;
      node->name         = "myShader2";
      node->node_type    = AI_NODE_SHADER;
      break;

   default:
      return false;
   }

   sprintf(node->version, AI_VERSION);

   return true;
}