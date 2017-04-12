#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(VectorToFloatMtd);

namespace
{

enum VectorToFloatParams
{
   p_input,
   p_component
};

enum component
{
   CH_X = 0,
   CH_Y,
   CH_Z,
   CH_LENGTH
};

const char* enum_component[] =
{
   "X",
   "Y",
   "Z",
   "length",
   NULL
};

};

node_parameters
{
   AiParameterVec("input", 0, 0, 0);
   AiParameterEnum("component", CH_LENGTH, enum_component);

   AiMetaDataSetStr(nentry, NULL, "_synonym", "vectorToFloat");
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiVectorToFloat");
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
   AtVector vector = AiShaderEvalParamVec(p_input);
   int component = AiShaderEvalParamEnum(p_component);
   switch (component)
   {
   case CH_X:
      sg->out.FLT() = vector.x;
      break;
   case CH_Y:
      sg->out.FLT() = vector.y;
      break;
   case CH_Z:
      sg->out.FLT() = vector.z;
      break;
   case CH_LENGTH:
   default:
      sg->out.FLT() = AiV3Length(vector);
      break;
   }
}
