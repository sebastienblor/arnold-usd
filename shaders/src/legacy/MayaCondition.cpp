#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaConditionMtd);

namespace
{

enum MayaConditionParams
{
   p_operation,
   p_firstTerm,
   p_secondTerm,
   p_colorIfTrue,
   p_colorIfFalse
};

enum OperationNames
{
   OP_EQUAL = 0,
   OP_NOT_EQUAL,
   OP_GREATER_THAN,
   OP_GREATER_OR_EQUAL,
   OP_LESS_THAN,
   OP_LESS_OR_EQUAL
};

const char* enum_operation[] =
{
   "equal",
   "notequal",
   "greaterthan",
   "greaterorequal",
   "lessthan",
   "lessorequal",
   NULL
};

};

node_parameters
{
   AiParameterEnum("operation", 0, enum_operation);
   AiParameterFlt("firstTerm", 0);
   AiParameterFlt("secondTerm", 0);
   AiParameterRGB("colorIfTrue", 0, 0, 0);
   AiParameterRGB("colorIfFalse", 1, 1, 1);

//   AiMetaDataSetStr(nentry, NULL, "maya.name", "condition");
//   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52434e44);
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
   int op             = AiShaderEvalParamInt(p_operation);
   float firstTerm    = AiShaderEvalParamFlt(p_firstTerm);
   float secondTerm   = AiShaderEvalParamFlt(p_secondTerm);
   AtRGB colorIfTrue  = AiShaderEvalParamRGB(p_colorIfTrue);
   AtRGB colorIfFalse = AiShaderEvalParamRGB(p_colorIfFalse);

   switch (op)
   {
   case OP_EQUAL:
      sg->out.RGB() = (firstTerm == secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_NOT_EQUAL:
      sg->out.RGB() = (firstTerm != secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_GREATER_THAN:
      sg->out.RGB() = (firstTerm > secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_GREATER_OR_EQUAL:
      sg->out.RGB() = (firstTerm >= secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_LESS_THAN:
      sg->out.RGB() = (firstTerm < secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_LESS_OR_EQUAL:
      sg->out.RGB() = (firstTerm <= secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   }
}