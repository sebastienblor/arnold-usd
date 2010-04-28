
#include <ai_nodes.h>
#include <ai_shaderglobals.h>
#include <ai_shaders.h>
#include <ai_shader_parameval.h>

enum MayaMultiplyDivideParams
{
   p_input1,
   p_input2,
   p_operation
};

AI_SHADER_NODE_EXPORT_METHODS(MayaMultiplyDivideMtd);

static const char* enum_operation[] = { "nop", "multiply", "divide", "power", NULL };

node_parameters
{
   AiParameterRGB("input1", 0, 0, 0);
   AiParameterRGB("input2", 0, 0, 0);
   AiParameterENUM("operation", 0, enum_operation);
}

node_initialize
{
}

node_finish
{
}

shader_evaluate
{
   AtColor input1 = AiShaderEvalParamRGB(p_input1);
   AtColor input2 = AiShaderEvalParamRGB(p_input2);

   switch(AiShaderEvalParamInt(p_operation))
   {
   case 0: // No operation (output = input1)
      sg->out.RGB = input1;
      break;
   case 1: // Multiply (output = input1 * input2)
      AiColorMult(sg->out.RGB, input1, input2);
      break;
   case 2: // Divide (output = input1 / input2)
      sg->out.RGB.r = (input2.r == 0) ? input1.r : (input1.r / input2.r);
      sg->out.RGB.g = (input2.g == 0) ? input1.g : (input1.g / input2.g);
      sg->out.RGB.b = (input2.b == 0) ? input1.b : (input1.b / input2.b);
      break;
   case 3: // Power (output = input1 ^ input2)
      sg->out.RGB.r = powf(input1.r, input2.r);
      sg->out.RGB.g = powf(input1.g, input2.g);
      sg->out.RGB.b = powf(input1.b, input2.b);
      break;
   default:
      sg->out.RGB = AI_RGB_BLACK;
   }
}
