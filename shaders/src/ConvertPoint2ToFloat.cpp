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
   AiParameterVec2("input", 0, 0);
   AiParameterEnum("component", CH_X, enum_component);

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
   AtVector2 point = AiShaderEvalParamVec2(p_input);
   int component = AiShaderEvalParamEnum(p_component);
   switch (component)
   {
   case CH_X:
      sg->out.FLT() = point.x;
      break;
   case CH_Y:
      sg->out.FLT() = point.y;
      break;
   }
}
