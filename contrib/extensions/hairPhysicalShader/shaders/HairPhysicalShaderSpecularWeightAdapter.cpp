#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(HairPhysicalShaderSpecularWeightAdapterMtd);

enum HairPhysicalShaderSpecularWeightAdapterParams
{
    p_intensity = 0,
    p_longitudinalWidth
};

node_parameters
{
    AiParameterFLT("intensity", 0.55f);
    AiParameterFLT("longitudinalWidth", 3.5f);

    AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
    // Specular weight
    //      hairPhysicalShader: Unit-height Gaussian. Integral is not 1.0
    //      aiHair:           : Energy conservation
    // We keep the overall intensity by multiplying a scale factor.
    const float intensity         = AiShaderEvalParamFlt(p_intensity);
    const float longitudinalWidth = AiShaderEvalParamFlt(p_longitudinalWidth);
    sg->out.FLT = intensity * longitudinalWidth * 0.1f;
}
