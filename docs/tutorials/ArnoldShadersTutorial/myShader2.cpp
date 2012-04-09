#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MyShader2Mtd);

namespace
{

enum MyShader2Params { p_color };

};

node_parameters
{
   AiParameterRGB("color2", 1.0f, 1.0f, 1.0f);
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