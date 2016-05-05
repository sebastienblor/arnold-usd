#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(PointToFloatMtd);

namespace
{

enum PointToFloatParams
{
   p_input,
   p_component
};

enum component
{
   CH_X = 0,
   CH_Y,
   CH_Z
};

const char* enum_component[] =
{
   "X",
   "Y",
   "Z",
   NULL
};

};

node_parameters
{
   AiParameterVec("input", 0, 0, 0);
   AiParameterEnum("component", CH_X, enum_component);

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
   AtVector point = AiShaderEvalParamVec(p_input);
   int component = AiShaderEvalParamEnum(p_component);
   switch (component)
   {
   case CH_X:
      sg->out.FLT() = point.x;
      break;
   case CH_Y:
      sg->out.FLT() = point.y;
      break;
   case CH_Z:
      sg->out.FLT() = point.z;
      break;
   }
}
