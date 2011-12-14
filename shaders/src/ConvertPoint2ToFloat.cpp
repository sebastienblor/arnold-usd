#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(Point2ToFloatMtd);

namespace
{

enum Point2ToFloatParams
{
   p_input,
   p_component
};

enum component
{
   CH_X = 0,
   CH_Y
};

const char* enum_component[] =
{
   "X",
   "Y",
   NULL
};

};

node_parameters
{
   AiParameterPNT2("input", 0, 0);
   AiParameterENUM("component", CH_X, enum_component);

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
   AtPoint2 point = AiShaderEvalParamPnt2(p_input);
   int component = AiShaderEvalParamEnum(p_component);
   switch (component)
   {
   case CH_X:
      sg->out.FLT = point.x;
      break;
   case CH_Y:
      sg->out.FLT = point.y;
      break;
   }
}
