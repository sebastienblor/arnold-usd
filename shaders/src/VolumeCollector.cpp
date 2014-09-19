// $Id: volume_collector.cpp 848 2014-07-17 12:31:05Z kik $

#include <ai.h>
#include <cassert>
#include <cstring>

AI_SHADER_NODE_EXPORT_METHODS(VolumeCollectorMtd);

#define ARNOLD_NODEID_VOLUME_COLLECTOR 0x00115D1F

namespace {

enum VolumeCollectorParams
{
    p_scattering,
    p_scattering_channel,
    p_scattering_color,
    p_scattering_intensity,
    p_anisotropy,
    p_attenuation_mode,
    p_attenuation_use_scattering,
    p_attenuation,
    p_attenuation_channel,
    p_attenuation_color,
    p_attenuation_intensity,
    p_emission,
    p_emission_channel,
    p_emission_color,
    p_emission_intensity,
    p_position_offset,
    p_interpolation,
};

static const char* attenuation_mode_labels[] = { "absorption", "extinction", NULL };
static const char* interpolation_labels[] = { "closest", "trilinear", "tricubic", NULL };

enum AttenuationMode
{
    ATTENUATION_MODE_ABSORPTION = 0,
    ATTENUATION_MODE_EXTINCTION = 1,
};

enum InputFrom
{
    INPUT_FROM_NONE,
    INPUT_FROM_EVALUATE,
    INPUT_FROM_CACHE,
    INPUT_FROM_CHANNEL,
    INPUT_FROM_SCATTERING,
};

struct ShaderData
{
    bool scattering_is_linked;
    bool scattering_color_is_linked;
    bool scattering_intensity_is_linked;
    bool anisotropy_is_linked;
    bool attenuation_is_linked;
    bool attenuation_color_is_linked;
    bool attenuation_intensity_is_linked;
    bool emission_is_linked;
    bool emission_color_is_linked;
    bool emission_intensity_is_linked;
    bool position_offset_is_linked;

    AtRGB scattering;
    const char* scattering_channel;
    AtRGB scattering_color;
    float scattering_intensity;
    float anisotropy;
    int attenuation_mode;
    bool attenuation_use_scattering;
    AtRGB attenuation;
    AtRGB attenuation_color;
    const char* attenuation_channel;
    float attenuation_intensity;
    AtRGB emission;
    const char* emission_channel;
    AtRGB emission_color;
    float emission_intensity;
    AtPoint position_offset;
    int interpolation;

    InputFrom scattering_from;
    InputFrom attenuation_from;
    InputFrom emission_from;
    InputFrom position_offset_from;
};

} // namespace

node_parameters
{
    AiMetaDataSetStr(mds, NULL, "maya.name", "aiVolumeCollector");
    AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_VOLUME_COLLECTOR);
    AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
    AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

    AiParameterRGB("scattering", 1.0f, 1.0f, 1.0f);
    AiParameterStr("scattering_channel", "");
    AiParameterRGB("scattering_color", 1.0f, 1.0f, 1.0f);
    AiParameterFlt("scattering_intensity", 1.0f);
    AiParameterFlt("anisotropy", 0);
    AiParameterEnum("attenuation_mode", ATTENUATION_MODE_ABSORPTION, attenuation_mode_labels);
    AiParameterBool("attenuation_use_scattering", false);
    AiParameterRGB("attenuation", 1.0f, 1.0f, 1.0f);
    AiParameterStr("attenuation_channel", "");
    AiParameterRGB("attenuation_color", 1.0f, 1.0f, 1.0f);
    AiParameterFlt("attenuation_intensity", 1.0f);
    AiParameterRGB("emission", 0.0f, 0.0f, 0.0f);
    AiParameterStr("emission_channel", "");
    AiParameterRGB("emission_color", 1.0f, 1.0f, 1.0f);
    AiParameterFlt("emission_intensity", 1.0f);
    AiParameterVec("position_offset", 0, 0, 0);
    AiParameterEnum("interpolation", AI_VOLUME_INTERP_TRILINEAR, interpolation_labels);

    AiMetaDataSetBool(mds, "interpolation"             , "linkable", false);
    AiMetaDataSetBool(mds, "scattering_channel"        , "linkable", false);
    AiMetaDataSetBool(mds, "attenuation_channel"       , "linkable", false);
    AiMetaDataSetBool(mds, "emission_channel"          , "linkable", false);
    AiMetaDataSetBool(mds, "attenuation_use_scattering", "linkable", false);
    AiMetaDataSetBool(mds, "attenuation_mode"          , "linkable", false);
}

node_initialize
{
    AiNodeSetLocalData(node, new ShaderData);
}

node_update
{
    ShaderData* data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

    // position offset
    data->position_offset = AiNodeGetVec(node, "position_offset");

    if (AiNodeIsLinked(node, "position_offset"))
        data->position_offset_from = INPUT_FROM_EVALUATE;
    else if (data->position_offset != AI_V3_ZERO)
        data->position_offset_from = INPUT_FROM_CACHE;
    else
        data->position_offset_from = INPUT_FROM_NONE;

    // get linked status
    data->scattering_is_linked            = AiNodeIsLinked(node, "scattering");
    data->scattering_color_is_linked      = AiNodeIsLinked(node, "scattering_color");
    data->scattering_intensity_is_linked  = AiNodeIsLinked(node, "scattering_intensity");
    data->anisotropy_is_linked            = AiNodeIsLinked(node, "anisotropy");

    data->attenuation_is_linked           = AiNodeIsLinked(node, "attenuation");
    data->attenuation_color_is_linked     = AiNodeIsLinked(node, "attenuation_color");
    data->attenuation_intensity_is_linked = AiNodeIsLinked(node, "attenuation_intensity");

    data->emission_is_linked              = AiNodeIsLinked(node, "emission");
    data->emission_color_is_linked        = AiNodeIsLinked(node, "emission_color");
    data->emission_intensity_is_linked    = AiNodeIsLinked(node, "emission_intensity");

    // cache parameter values
    data->interpolation = AiNodeGetInt(node, "interpolation");

    data->scattering                 = AiNodeGetRGB(node, "scattering");
    data->scattering_channel         = AiNodeGetStr(node, "scattering_channel");
    data->scattering_color           = AiNodeGetRGB(node, "scattering_color");
    data->scattering_intensity       = AiNodeGetFlt(node, "scattering_intensity");
    data->anisotropy                 = AiNodeGetFlt(node, "anisotropy");

    data->attenuation_mode           = AiNodeGetInt(node, "attenuation_mode");
    data->attenuation_use_scattering = AiNodeGetBool(node, "attenuation_use_scattering");
    data->attenuation                = AiNodeGetRGB(node, "attenuation");
    data->attenuation_channel        = AiNodeGetStr(node, "attenuation_channel");
    data->attenuation_color          = AiNodeGetRGB(node, "attenuation_color");
    data->attenuation_intensity      = AiNodeGetFlt(node, "attenuation_intensity");

    data->emission                   = AiNodeGetRGB(node, "emission");
    data->emission_channel           = AiNodeGetStr(node, "emission_channel");
    data->emission_color             = AiNodeGetRGB(node, "emission_color");
    data->emission_intensity         = AiNodeGetFlt(node, "emission_intensity");

    // input mode
    if (data->scattering_is_linked)
        data->scattering_from = INPUT_FROM_EVALUATE;
    else if (data->scattering_channel && data->scattering_channel[0] != 0)
        data->scattering_from = INPUT_FROM_CHANNEL;
    else
        data->scattering_from = INPUT_FROM_CACHE;

    if (data->attenuation_use_scattering)
        data->attenuation_from = INPUT_FROM_SCATTERING;
    else if (data->attenuation_is_linked)
        data->attenuation_from = INPUT_FROM_EVALUATE;
    else if (data->attenuation_channel && data->attenuation_channel[0] != 0)
        data->attenuation_from = strcmp(data->attenuation_channel, data->scattering_channel) ? INPUT_FROM_CHANNEL : INPUT_FROM_SCATTERING;
    else
        data->attenuation_from = INPUT_FROM_CACHE;

    if (data->emission_is_linked)
        data->emission_from = INPUT_FROM_EVALUATE;
    else if (data->emission_channel && data->emission_channel[0] != 0)
        data->emission_from = INPUT_FROM_CHANNEL;
    else
        data->emission_from = INPUT_FROM_CACHE;
}

node_finish
{
    ShaderData* data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
    delete data;
}

shader_evaluate
{
    ShaderData* data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

    // sampling position offset
    AtPoint Po_orig;

    switch (data->position_offset_from)
    {
    case INPUT_FROM_EVALUATE:
        Po_orig = sg->Po;
        sg->Po += AiShaderEvalParamVec(p_position_offset);
        break;

    case INPUT_FROM_CACHE:
        Po_orig = sg->Po;
        sg->Po += data->position_offset;
        break;

    default:
        Po_orig = AI_V3_ZERO;
        break;
    }

    // fetch inputs
    AtColor scattering, attenuation, emission;

    switch (data->scattering_from)
    {
    case INPUT_FROM_CHANNEL:  AiVolumeSampleRGB(data->scattering_channel, data->interpolation, &scattering); break;
    case INPUT_FROM_EVALUATE: scattering = AiShaderEvalParamRGB(p_scattering); break;
    case INPUT_FROM_CACHE:    scattering = data->scattering; break;
    default: assert("invalid value for data->scattering_from"); break;
    }

    switch (data->attenuation_from)
    {
    case INPUT_FROM_CHANNEL:    AiVolumeSampleRGB(data->attenuation_channel, data->interpolation, &attenuation); break;
    case INPUT_FROM_EVALUATE:   attenuation = AiShaderEvalParamRGB(p_attenuation); break;
    case INPUT_FROM_CACHE:      attenuation = data->attenuation; break;
    case INPUT_FROM_SCATTERING: attenuation = scattering; break;
    default: assert("invalid value for data->attenuation_from"); break;
    }

    switch (data->emission_from)
    {
    case INPUT_FROM_CHANNEL:  AiVolumeSampleRGB(data->emission_channel, data->interpolation, &emission); break;
    case INPUT_FROM_EVALUATE: emission = AiShaderEvalParamRGB(p_emission); break;
    case INPUT_FROM_CACHE:    emission = data->emission; break;
    default: assert("invalid value for data->emission_from"); break;
    }

    const AtRGB scattering_color      = data->scattering_color_is_linked      ? AiShaderEvalParamRGB(p_scattering_color)      : data->scattering_color;
    const float scattering_intensity  = data->scattering_intensity_is_linked  ? AiShaderEvalParamFlt(p_scattering_intensity)  : data->scattering_intensity;
    const float anisotropy            = data->anisotropy_is_linked            ? AiShaderEvalParamFlt(p_anisotropy)            : data->anisotropy;
    const AtRGB attenuation_color     = data->attenuation_color_is_linked     ? AiShaderEvalParamRGB(p_attenuation_color)     : data->attenuation_color;
    const float attenuation_intensity = data->attenuation_intensity_is_linked ? AiShaderEvalParamFlt(p_attenuation_intensity) : data->attenuation_intensity;
    const AtRGB emission_color        = data->emission_color_is_linked        ? AiShaderEvalParamRGB(p_emission_color)        : data->emission_color;
    const float emission_intensity    = data->emission_intensity_is_linked    ? AiShaderEvalParamFlt(p_emission_intensity)    : data->emission_intensity;

    // color and intensity
    scattering  *= scattering_color  * scattering_intensity;
    attenuation *= attenuation_color * attenuation_intensity;
    emission    *= emission_color    * emission_intensity;

    // clip volume properties
    AiColorClipToZero(scattering);
    AiColorClipToZero(attenuation);
    AiColorClipToZero(emission);

    // update volume shader globals
    AiShaderGlobalsSetVolumeScattering(sg, scattering, anisotropy);

    switch (data->attenuation_mode)
    {
    case ATTENUATION_MODE_ABSORPTION: AiShaderGlobalsSetVolumeAbsorption(sg, attenuation); break;
    case ATTENUATION_MODE_EXTINCTION: AiShaderGlobalsSetVolumeAttenuation(sg, attenuation); break;
    }

    if (!AiColorIsZero(emission))
       AiShaderGlobalsSetVolumeEmission(sg, emission);

    // restore sampling position
    if (data->position_offset_from != INPUT_FROM_NONE)
        sg->Po = Po_orig;
}

