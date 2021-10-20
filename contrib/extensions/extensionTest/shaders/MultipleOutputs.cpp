#include <ai.h>
#include <string.h>

AI_SHADER_NODE_EXPORT_METHODS(MtoaTestMultipleOutputShaderMtd);

node_parameters
{
   AiOutputRGB("red_output");
   AiOutputRGB("blue_output");
}

node_initialize
{
}

shader_evaluate
{
    AtRGB red_color(1.0, 0.0, 0.0);
    AtRGB blue_color(0.0, 0.0, 1.0);
    switch (AiShaderGlobalsGetSelectedOutput(sg))
   {
      default:
      case 0:
         sg->out.RGB() = red_color; return;
      case 1:
         sg->out.RGB() = blue_color; return;
   }

}

node_update
{
}

node_finish
{
}

