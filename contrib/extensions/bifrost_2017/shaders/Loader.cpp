#include <ai.h>

#include <cstdio>

extern const AtNodeMethods* BifrostFoamMaterialMtd;
extern const AtNodeMethods* BifrostAeroMaterialMtd;

enum{
   SHADER_BIFROST_FOAM_MATERIAL = 0,
   SHADER_BIFROST_AERO_MATERIAL
};

node_loader
{
   node->output_type  = AI_TYPE_RGB;
   node->node_type    = AI_NODE_SHADER;
   switch (i)
   {
   case SHADER_BIFROST_FOAM_MATERIAL:
      node->methods      = BifrostFoamMaterialMtd;
      node->name         = "bifrostFoamMaterial";
      break;
   case SHADER_BIFROST_AERO_MATERIAL:
      node->methods      = BifrostAeroMaterialMtd;
      node->name         = "bifrostAeroMaterial";
      break;

   default:
      return false;
   }

   sprintf(node->version, AI_VERSION);

   return true;
}
