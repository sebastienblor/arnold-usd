#include <ai.h>
#include <cstdio>

namespace
{

enum Utility2Parameters
{
   p_what = 0,
   p_colorSetName
};

enum WhatEnum
{
   TANGENT = 0,
   BITANGENT,
   COLOR
};

static const char *WhatNames[] =
{
   "tangent",
   "bitangent",
   "color",
   NULL
};

}

AI_SHADER_NODE_EXPORT_METHODS(Utility2Mtd);

node_parameters
{
   AiParameterENUM("what", 0, WhatNames);
   AiParameterSTR("colorSetName", "");
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   int what = AiShaderEvalParamInt(p_what);
   const AtChar *name = 0;

   AtVector v;
   AtRGBA c;

   switch (what)
   {
   case TANGENT:
      if (AiUDataGetVec("tangent", &v))
      {
         sg->out.RGB.r = v.x;
         sg->out.RGB.g = v.y;
         sg->out.RGB.b = v.z;
      }
      else
      {
         sg->out.RGB = AI_RGB_BLACK;
      }
      break;
   case BITANGENT:
      if (AiUDataGetVec("bitangent", &v))
      {
         sg->out.RGB.r = v.x;
         sg->out.RGB.g = v.y;
         sg->out.RGB.b = v.z;
      }
      else
      {
         sg->out.RGB = AI_RGB_BLACK;
      }
      break;
   case COLOR:
      name = AiShaderEvalParamStr(p_colorSetName);
      if (AiUDataGetRGBA(name, &c))
      {
         sg->out.RGB.r = c.r;
         sg->out.RGB.g = c.g;
         sg->out.RGB.b = c.b;
      }
      else
      {
         sg->out.RGB = AI_RGB_BLACK;
      }
      break;
   default:
      sg->out.RGB = AI_RGB_BLACK;
      break;
   }
}

