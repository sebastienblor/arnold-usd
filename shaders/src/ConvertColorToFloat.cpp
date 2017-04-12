#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(ColorToFloatMtd);

namespace
{

enum ColorToFloatParams
{
   p_input,
   p_component
};

enum component
{
   CH_RED = 0,
   CH_GREEN,
   CH_BLUE,
   CH_ALPHA,
   CH_LUMINANCE
};

const char* enum_component[] =
{
   "R",
   "G",
   "B",
   "A",
   "luminance",
   NULL
};

};

node_parameters
{
   AiParameterRGBA("input", 0, 0, 0, 1);
   AiParameterEnum("component", CH_LUMINANCE, enum_component);

   AiMetaDataSetStr(nentry, NULL, "_synonym", "colorToFloat");
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiColorToFloat_rgba");
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
   AtRGBA color = AiShaderEvalParamRGBA(p_input);
   int component = AiShaderEvalParamEnum(p_component);
   switch (component)
   {
   case CH_RED:
      sg->out.FLT() = color.r;
      break;
   case CH_GREEN:
      sg->out.FLT() = color.g;
      break;
   case CH_BLUE:
      sg->out.FLT() = color.b;
      break;
   case CH_ALPHA:
      sg->out.FLT() = color.a;
      break;
   case CH_LUMINANCE:
   default:
      sg->out.FLT() = Luminance(color);
      break;
   }
}
