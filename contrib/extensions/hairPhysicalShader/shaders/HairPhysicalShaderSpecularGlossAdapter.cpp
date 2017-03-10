#include <ai.h>
#include <algorithm>

AI_SHADER_NODE_EXPORT_METHODS(HairPhysicalShaderSpecularGlossAdapterMtd);

enum HairPhysicalShaderSpecularGlossAdapterParams
{
    p_longitudinalWidth = 0
};

node_parameters
{
    AiParameterFlt("longitudinalWidth", 3.5f);

    AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
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
    // Specular gloss
    //      hairPhysicalShader: Unit-height Gaussian exp(-0.5 * ((thetaH - shift) / width)^2)
    //      aiHair            : Cosine power sqrt(1 - dot(H,T)^2) ^ (0.5 * gloss)
    // It's difficult to map Guassian to Cosine power.. The relationship between gloss and width
    // is roughly: gloss = k / width^2.
    const float longitudinalWidth = AiShaderEvalParamFlt(p_longitudinalWidth);
    sg->out.FLT() = std::min(3000.0f / (longitudinalWidth * longitudinalWidth), 5000.0f);
}
