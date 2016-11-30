#include <ai.h>

#include <cstdio>

extern AtNodeMethods* BifrostFoamMaterialMtd;
extern AtNodeMethods* BifrostAeroMaterialMtd;
extern AtNodeMethods* BifrostHydroMethod;
extern AtNodeMethods* BifrostPointMethod;
extern AtNodeMethods* BifrostAeroMethod;

enum{
   SHADER_BIFROST_FOAM_MATERIAL = 0,
   SHADER_BIFROST_AERO_MATERIAL,
   SHADER_BIFROST_HYDRO,
   SHADER_BIFROST_POINT,
   SHADER_BIFROST_AERO
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
   case SHADER_BIFROST_HYDRO:
      node->methods      = BifrostHydroMethod;
      node->name         = "bifrostHydro";
      break;
   case SHADER_BIFROST_POINT:
      node->methods      = BifrostPointMethod;
      node->name         = "bifrostPoint";
      break;
   case SHADER_BIFROST_AERO:
      node->methods      = BifrostAeroMethod;
      node->name         = "bifrostAero";
      break;

   default:
      return false;
   }

   sprintf(node->version, AI_VERSION);

   return true;
}
