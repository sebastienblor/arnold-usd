#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MyShader1Mtd);

namespace
{

enum MyShader1Params { p_color };

};

node_parameters
{
   AiParameterRGB("color1", 0.0f, 0.0f, 0.0f);
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
   AtColor color = AiShaderEvalParamRGB(p_color);
   sg->out.RGB = color;
}