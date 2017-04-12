#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(ldkColorLogicMtd);

namespace
{

enum ldkColorLogicParams
{
   p_colorA,
   p_colorB,
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
   AiParameterRGB("colorA", 1.0f, 0.0f, 0.5f);
   AiParameterRGB("colorB", 1.0f, 0.0f, 0.5f);
   AiParameterEnum("operation", OP_EQUAL, OperationStrings);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "colorLogic");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81638);
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
   const AtRGB colorA = AiShaderEvalParamRGB(p_colorA);
   const AtRGB colorB = AiShaderEvalParamRGB(p_colorB);
   const float floatA = colorA.r + colorA.g + colorA.b;
   const float floatB = colorB.r + colorB.g + colorB.b;
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
