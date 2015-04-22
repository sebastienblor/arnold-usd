#include <ai.h>

#include <cstdio>

extern AtNodeMethods* BifrostFoamMaterialMtd;
extern AtNodeMethods* BifrostAeroMaterialMtd;

enum{
   SHADER_BIFROST_FOAM = 0,
   SHADER_BIFROST_AERO
};

node_loader
{
   switch (i)
   {
   case SHADER_BIFROST_FOAM:
      node->methods      = BifrostFoamMaterialMtd;
      node->output_type  = AI_TYPE_RGB;
      node->name         = "bifrostFoamMaterial";
      node->node_type    = AI_NODE_SHADER;
      break;

   case SHADER_BIFROST_AERO:
      node->methods     = BifrostAeroMaterialMtd;
      node->output_type  = AI_TYPE_RGB;
      node->name         = "bifrostAeroMaterial";
      node->node_type    = AI_NODE_SHADER;

      break;

   default:
      return false;
   }

   sprintf(node->version, AI_VERSION);

   return true;
}
