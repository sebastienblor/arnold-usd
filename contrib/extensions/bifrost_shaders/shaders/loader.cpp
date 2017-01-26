
#include <ai.h>

#include <cstdio>

extern AtNodeMethods* BifrostHydroMtd;
extern AtNodeMethods* BifrostPointMtd;
extern AtNodeMethods* BifrostAeroMtd;

enum{
   SHADER_BIFROST_HYDRO = 0,
   SHADER_BIFROST_POINT,
   SHADER_BIFROST_AERO
};

node_loader
{
   node->output_type  = AI_TYPE_RGB;
   node->node_type    = AI_NODE_SHADER;
   switch (i)
   {
   case SHADER_BIFROST_HYDRO:
      node->methods      = BifrostHydroMtd;
      node->name         = "bifrostHydro";
      break;
   case SHADER_BIFROST_POINT:
      node->methods      = BifrostPointMtd;
      node->name         = "bifrostPoint";
      break;
   case SHADER_BIFROST_AERO:
      node->methods      = BifrostAeroMtd;
      node->name         = "bifrostAero";
      break;

   default:
      return false;
   }

   sprintf(node->version, AI_VERSION);

   return true;
}
