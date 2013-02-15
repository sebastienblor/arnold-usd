#include <ai.h>


AI_SHADER_NODE_EXPORT_METHODS(SimpleLightFilterMtd);

enum simpleLightFilterParams {
    p_intensity
};

node_parameters
{
    AiParameterFlt("Intensity", 0.0f);
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
    float intensity = AiShaderEvalParamFlt(p_intensity);
    sg->Liu = sg->Liu*intensity;
}
