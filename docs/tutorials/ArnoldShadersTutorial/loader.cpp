#include <ai.h>
#include <cstring>

extern AtNodeMethods* MyShader1Mtd;
extern AtNodeMethods* MyShader2Mtd;

enum{
   MY_SHADER_1 = 0,
   MY_SHADER_2
};

node_loader
{
   switch (i)
   {
   case MY_SHADER_1:
      node->methods      = MyShader1Mtd;
      node->output_type  = AI_TYPE_RGB;
      node->name         = "myShader1";
      node->node_type    = AI_NODE_SHADER;
      break;

   case MY_SHADER_2:
      node->methods      = MyShader2Mtd;
      node->output_type  = AI_TYPE_RGB;
      node->name         = "myShader2";
      node->node_type    = AI_NODE_SHADER;
      break;

   default:
      return false;
   }

   strcpy(node->version, AI_VERSION);

   return true;
}