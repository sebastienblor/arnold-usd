#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(HairPhysicalShaderTransmissionWeightAdapterMtd);

enum HairPhysicalShaderTransmissionWeightAdapterParams
{
    p_intensityTT = 0,
    p_longitudinalWidthTT,
    p_azimuthalWidthTT
};

node_parameters
{
    AiParameterFlt("intensityTT", 0.15f);
    AiParameterFlt("longitudinalWidthTT", 10.0f);
    AiParameterFlt("azimuthalWidthTT", 10.0f);

    AiMetaDataSetBool(nentry, "azimuthalWidthTT", "linkable", false);
    AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
    AiMetaDataSetBool(nentry, NULL, "deprecated", true);
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
    const float intensityTT         = AiShaderEvalParamFlt(p_intensityTT);
    const float longitudinalWidthTT = AiShaderEvalParamFlt(p_longitudinalWidthTT);
    const float azimuthalWidthTT    = AiShaderEvalParamFlt(p_azimuthalWidthTT);
    const float spread              = (longitudinalWidthTT * 5.0f + azimuthalWidthTT * 2.0f) / 40.0f;
    sg->out.FLT() = intensityTT * spread * 0.1f;
}
