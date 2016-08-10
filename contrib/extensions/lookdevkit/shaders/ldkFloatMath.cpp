#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkFloatMathMtd);

namespace
{

enum ldkFloatMathParams
{
   p_floatA,
   p_floatB,
   p_operation
};

enum operation
{
   OP_ADD = 0,
   OP_SUBTRACT,
   OP_MULTIPLY,
   OP_DIVIDE,
   OP_MIN,
   OP_MAX,
   OP_POW
};

const char* enum_operation[] =
{
   "Add",
   "Subtract",
   "Multiply",
   "Divide",
   "Min",
   "Max",
   "Pow",
   NULL
};

};

node_parameters
{
   AiParameterFLT("floatA", 1.0f);
   AiParameterFLT("floatB", 1.0f);
   AiParameterENUM("operation", OP_ADD, enum_operation);

   AiMetaDataSetStr(mds, NULL, "maya.name", "floatMath");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x816840);
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
   const float floatA = AiShaderEvalParamFlt(p_floatA);
   const float floatB = AiShaderEvalParamFlt(p_floatB);
   const int op = AiShaderEvalParamEnum(p_operation);
   switch (op)
   {
   case OP_ADD:
      sg->out.FLT = floatA + floatB;
      break;
   case OP_SUBTRACT:
      sg->out.FLT = floatA - floatB;
      break;
   case OP_MULTIPLY:
      sg->out.FLT = floatA * floatB;
      break;
   case OP_DIVIDE:
      sg->out.FLT = floatB != 0.0f ? floatA / floatB : 0.0f;
      break;
   case OP_MIN:
      sg->out.FLT = floatA < floatB ? floatA : floatB;
      break;
   case OP_MAX:
      sg->out.FLT = floatA > floatB ? floatA : floatB;
      break;
   case OP_POW:
      sg->out.FLT = pow(floatA, floatB);
      break;
   default:
      sg->out.FLT = 0.0f;
      break;
   }
}
