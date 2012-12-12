#include <ai.h>
#include <cstdio>

extern AtNodeMethods* MyShader1Mtd;
extern AtNodeMethods* MyShader2Mtd;
extern AtNodeMethods* ParametersShaderMtd;
extern AtNodeMethods* SimpleLightFilterMtd;

enum{
   MY_SHADER_1 = 0,
   MY_SHADER_2,
   PARAMETERS_SHADER,
   SIMPLE_LIGHT_FILTER
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
      
   case PARAMETERS_SHADER:
      node->methods      = ParametersShaderMtd;
      node->output_type  = AI_TYPE_RGB;
      node->name         = "parametersShader";
      node->node_type    = AI_NODE_SHADER;
      break;
      
   case SIMPLE_LIGHT_FILTER:
      node->methods      = SimpleLightFilterMtd;
      node->output_type  = AI_TYPE_RGB;
      node->name         = "simpleLightFilter";
      node->node_type    = AI_NODE_SHADER;
      break;

   default:
      return false;
   }

   sprintf(node->version, AI_VERSION);

   return true;
}