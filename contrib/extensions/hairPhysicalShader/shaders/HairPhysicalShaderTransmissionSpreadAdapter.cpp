#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(HairPhysicalShaderTransmissionSpreadAdapterMtd);

enum HairPhysicalShaderTransmissionSpreadAdapterParams
{
    p_longitudinalWidthTT = 0,
    p_azimuthalWidthTT
};

node_parameters
{
    AiParameterFLT("longitudinalWidthTT", 10.0f);
    AiParameterFLT("azimuthalWidthTT", 10.0f);

    AiMetaDataSetBool(mds, "azimuthalWidthTT", "linkable", false);
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
    // Transmission spread
    //      hairPhysicalShader: longitudinalShiftTT, longitudinalWidthTT and azimuthalWidthTT
    //                          Shift along hair, Width in two directions
    //      aiHair:           : transmission_spread
    //                          longitudinalShiftTT = -2
    //                          longitudinalWidthTT = 4 * spread
    //                          azimuthalWidthTT    = 10 * spread
    //
    // Observation shows that Arnold's shift is roughly -4..
    // Translator will prompt a warning when the width ratio is not 5:2
    const float longitudinalWidthTT = AiShaderEvalParamFlt(p_longitudinalWidthTT);
    const float azimuthalWidthTT    = AiShaderEvalParamFlt(p_azimuthalWidthTT);
    const float spread              = (longitudinalWidthTT * 5.0f + azimuthalWidthTT * 2.0f) / 40.0f;
    sg->out.FLT = spread;
}
