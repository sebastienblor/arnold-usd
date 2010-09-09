
#include <ai_nodes.h>
#include <ai_shaderglobals.h>
#include <ai_shaders.h>
#include <ai_shader_parameval.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaConditionMtd);

namespace
{

enum MayaConditionParams
{
   p_operation,
   p_first_term,
   p_second_term,
   p_color_if_true,
   p_color_if_false
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
   AiParameterENUM("operation", 0, enum_operation);
   AiParameterFLT("first_term", 0);
   AiParameterFLT("second_term", 0);
   AiParameterRGB("color_if_true", 0, 0, 0);
   AiParameterRGB("color_if_false", 1, 1, 1);
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
   int op               = AiShaderEvalParamInt(p_operation);
   float firstTerm      = AiShaderEvalParamFlt(p_first_term);
   float secondTerm     = AiShaderEvalParamFlt(p_second_term);
   AtColor colorIfTrue  = AiShaderEvalParamRGB(p_color_if_true);
   AtColor colorIfFalse = AiShaderEvalParamRGB(p_color_if_false);

   switch (op)
   {
   case OP_EQUAL:
      sg->out.RGB = (firstTerm == secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_NOT_EQUAL:
      sg->out.RGB = (firstTerm != secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_GREATER_THAN:
      sg->out.RGB = (firstTerm > secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_GREATER_OR_EQUAL:
      sg->out.RGB = (firstTerm >= secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_LESS_THAN:
      sg->out.RGB = (firstTerm < secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case OP_LESS_OR_EQUAL:
      sg->out.RGB = (firstTerm <= secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   }
}
