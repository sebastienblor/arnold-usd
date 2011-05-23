#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(ColorToFloatMtd);

namespace
{

enum ColorToFloatParams
{
   p_color,
   p_channel
};

enum Channel
{
   CH_RED = 0,
   CH_GREEN,
   CH_BLUE,
   CH_ALPHA,
   CH_LUMINANCE
};

const char* enum_channel[] =
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
   AiParameterRGBA("color", 0, 0, 0, 1);
   AiParameterENUM("channel", CH_LUMINANCE, enum_channel);

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
   AtRGBA color = AiShaderEvalParamRGBA(p_color);
   AtInt channel = AiShaderEvalParamEnum(p_channel);
   switch (channel)
   {
   case CH_RED:
      sg->out.FLT = color.r;
      break;
   case CH_GREEN:
      sg->out.FLT = color.g;
      break;
   case CH_BLUE:
      sg->out.FLT = color.b;
      break;
   case CH_ALPHA:
      sg->out.FLT = color.a;
      break;
   case CH_LUMINANCE:
   default:
      sg->out.FLT = Luminance(color);
      break;
   }
}
