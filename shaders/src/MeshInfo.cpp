#include <ai.h>

namespace MSTR
{
   static const AtString tangent("tangent");
   static const AtString bitangent("bitangent");
}

namespace
{

enum MeshInfoParams
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

AI_SHADER_NODE_EXPORT_METHODS(MeshInfoMtd);

node_parameters
{
   AiParameterEnum("what", 0, WhatNames);
   AiParameterStr("colorSetName", "");

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
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

   AtString name;
   AtVector v;
   AtRGBA c;

   switch (what)
   {
   case TANGENT:
      if (AiUDataGetVec(MSTR::tangent, &v))
      {
         sg->out.RGB().r = v.x;
         sg->out.RGB().g = v.y;
         sg->out.RGB().b = v.z;
      }
      else
      {
         sg->out.RGB() = AI_RGB_BLACK;
      }
      break;
   case BITANGENT:
      if (AiUDataGetVec(MSTR::bitangent, &v))
      {
         sg->out.RGB().r = v.x;
         sg->out.RGB().g = v.y;
         sg->out.RGB().b = v.z;
      }
      else
      {
         sg->out.RGB() = AI_RGB_BLACK;
      }
      break;
   case COLOR:
      name = AtString(AiShaderEvalParamStr(p_colorSetName));
      if (AiUDataGetRGBA(name, &c))
      {
         sg->out.RGB().r = c.r;
         sg->out.RGB().g = c.g;
         sg->out.RGB().b = c.b;
      }
      else
      {
         sg->out.RGB() = AI_RGB_BLACK;
      }
      break;
   default:
      sg->out.RGB() = AI_RGB_BLACK;
      break;
   }
}
