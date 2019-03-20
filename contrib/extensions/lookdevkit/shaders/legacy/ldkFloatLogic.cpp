#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkFloatLogicMtd);

namespace
{

enum ldkFloatLogicParams
{
   p_floatA,
   p_floatB,
   p_operation
};

enum Operation
{
   OP_EQUAL = 0,
   OP_NOT_EQUAL,
   OP_LESS,
   OP_GREATER,
   OP_LESS_OR_EQUAL,
   OP_GREATER_OR_EQUAL
};

const char* OperationStrings[] =
{
   "==",
   "!=",
   "<",
   ">",
   "<=",
   ">=",
   NULL
};

};

node_parameters
{
   AiParameterFlt("floatA", 1.0f);
   AiParameterFlt("floatB", 1.0f);
   AiParameterEnum("operation", OP_EQUAL, OperationStrings);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "floatLogic");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81636);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
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
   case OP_EQUAL:
      sg->out.BOOL() = floatA == floatB;
      break;
   case OP_NOT_EQUAL:
      sg->out.BOOL() = floatA != floatB;
      break;
   case OP_LESS:
      sg->out.BOOL() = floatA < floatB;
      break;
   case OP_GREATER:
      sg->out.BOOL() = floatA > floatB;
      break;
   case OP_LESS_OR_EQUAL:
      sg->out.BOOL() = floatA <= floatB;
      break;
   case OP_GREATER_OR_EQUAL:
      sg->out.BOOL() = floatA >= floatB;
      break;
   default:
      sg->out.BOOL() = false;
      break;
   }
}
