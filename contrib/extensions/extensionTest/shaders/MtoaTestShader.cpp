#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MtoaTestShaderMtd);

node_parameters
{
    AiParameterRGB("color", 1.0f, 1.0f, 1.0f);
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
    AtRGB color  = AiShaderEvalParamRGB(0);
   
    sg->out.RGB() = color;
    sg->out.RGBA().a = 1.f;
}
