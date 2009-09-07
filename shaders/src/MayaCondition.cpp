
#include <ai_nodes.h>
#include <ai_shaderglobals.h>
#include <ai_shaders.h>
#include <ai_shader_parameval.h>

enum MayaConditionParams {
   p_operation,
   p_first_term,
   p_second_term,
   p_color_if_true,
   p_color_if_false
};

extern "C" { AI_SHADER_NODE_EXPORT_METHODS(MayaConditionMtd); }

static const char* enum_operation[] = { "equal", "notequal", "greaterthan", "greaterorequal", "lessthan", "lessorequal", NULL };

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
   case 0: // Equal
      sg->out.RGB = (firstTerm == secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case 1: // Not equal
      sg->out.RGB = (firstTerm != secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case 2: // Greater than
      sg->out.RGB = (firstTerm > secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case 3: // Greater or equal
      sg->out.RGB = (firstTerm >= secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case 4: // Less than
      sg->out.RGB = (firstTerm < secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   case 5: // Less or equal
      sg->out.RGB = (firstTerm <= secondTerm) ? colorIfTrue : colorIfFalse;
      break;
   }
}
