#include <ai.h>
#include <string>
#include <cassert>
#include <cstring>

#ifndef SHADER_PREFIX
#define SHADER_PREFIX ""
#endif

AI_SHADER_NODE_EXPORT_METHODS(VolumeCollectorMtd);

#define ARNOLD_NODEID_VOLUME_COLLECTOR 0x00115D1F

namespace {

enum VolumeCollectorParams
{
    p_scattering_source,
    p_scattering,
    p_scattering_channel,
    p_scattering_color,
    p_scattering_intensity,
    p_anisotropy,
    p_attenuation_source,
    p_attenuation,
    p_attenuation_channel,
    p_attenuation_color,
    p_attenuation_intensity,
    p_attenuation_mode,
    p_emission_source,
    p_emission,
    p_emission_channel,
    p_emission_color,
    p_emission_intensity,
    p_position_offset,
    p_interpolation,
};

static const char* scattering_source_labels[] = { "parameter", "channel", NULL };
static const char* attenuation_source_labels[] = { "parameter", "channel", "scattering", NULL };
static const char* emission_source_labels[] = { "parameter", "channel", NULL };
static const char* attenuation_mode_labels[] = { "absorption", "extinction", NULL };
static const char* interpolation_labels[] = { "closest", "trilinear", "tricubic", NULL };

enum InputSource
{
    INPUT_SOURCE_PARAMETER  = 0,
    INPUT_SOURCE_CHANNEL    = 1,
    INPUT_SOURCE_SCATTERING = 2,
};

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
    AtString scattering_channel;
    AtRGB scattering_color;
    float scattering_intensity;
    float anisotropy;
    int attenuation_mode;
    AtRGB attenuation;
    AtRGB attenuation_color;
    AtString attenuation_channel;
    float attenuation_intensity;
    AtRGB emission;
    AtString emission_channel;
    AtRGB emission_color;
    float emission_intensity;
    AtVector position_offset;
    int interpolation;

    int scattering_source;
    int attenuation_source;
    int emission_source;

    InputFrom scattering_from;
    InputFrom attenuation_from;
    InputFrom emission_from;
    InputFrom position_offset_from;
};

} // namespace

node_parameters
{
    AiParameterEnum("scattering_source", INPUT_SOURCE_PARAMETER, scattering_source_labels);
    AiParameterRGB("scattering", 1.0f, 1.0f, 1.0f);
    AiParameterStr("scattering_channel", "");
    AiParameterRGB("scattering_color", 1.0f, 1.0f, 1.0f);
    AiParameterFlt("scattering_intensity", 1.0f);
    AiParameterFlt("anisotropy", 0);
    AiParameterEnum("attenuation_source", INPUT_SOURCE_PARAMETER, attenuation_source_labels);
    AiParameterRGB("attenuation", 1.0f, 1.0f, 1.0f);
    AiParameterStr("attenuation_channel", "");
    AiParameterRGB("attenuation_color", 1.0f, 1.0f, 1.0f);
    AiParameterFlt("attenuation_intensity", 1.0f);
    AiParameterEnum("attenuation_mode", ATTENUATION_MODE_ABSORPTION, attenuation_mode_labels);
    AiParameterEnum("emission_source", INPUT_SOURCE_PARAMETER, emission_source_labels);
    AiParameterRGB("emission", 0.0f, 0.0f, 0.0f);
    AiParameterStr("emission_channel", "");
    AiParameterRGB("emission_color", 1.0f, 1.0f, 1.0f);
    AiParameterFlt("emission_intensity", 1.0f);
    AiParameterVec("position_offset", 0, 0, 0);
    AiParameterEnum("interpolation", AI_VOLUME_INTERP_TRILINEAR, interpolation_labels);

    AiMetaDataSetBool(nentry, "scattering_source"         , "linkable", false);
    AiMetaDataSetBool(nentry, "attenuation_source"        , "linkable", false);
    AiMetaDataSetBool(nentry, "emission_source"           , "linkable", false);
    AiMetaDataSetBool(nentry, "interpolation"             , "linkable", false);
    AiMetaDataSetBool(nentry, "scattering_channel"        , "linkable", false);
    AiMetaDataSetBool(nentry, "attenuation_channel"       , "linkable", false);
    AiMetaDataSetBool(nentry, "emission_channel"          , "linkable", false);
    AiMetaDataSetBool(nentry, "attenuation_mode"          , "linkable", false);

    AiMetaDataSetBool(nentry, "scattering"       , "always_linear", true);
    AiMetaDataSetBool(nentry, "scattering_color" , "always_linear", true);
    AiMetaDataSetBool(nentry, "attenuation"      , "always_linear", true);
    AiMetaDataSetBool(nentry, "attenuation_color", "always_linear", true);
    AiMetaDataSetBool(nentry, "emission"         , "always_linear", true);
    AiMetaDataSetBool(nentry, "emission_color"   , "always_linear", true);
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

    data->scattering_source          = AiNodeGetInt(node, "scattering_source");
    data->scattering                 = AiNodeGetRGB(node, "scattering");
    data->scattering_channel         = AiNodeGetStr(node, AtString("scattering_channel"));
    data->scattering_color           = AiNodeGetRGB(node, "scattering_color");
    data->scattering_intensity       = AiNodeGetFlt(node, "scattering_intensity");
    data->anisotropy                 = AiNodeGetFlt(node, "anisotropy");

    data->attenuation_source         = AiNodeGetInt(node, "attenuation_source");
    data->attenuation                = AiNodeGetRGB(node, "attenuation");
    data->attenuation_channel        = AiNodeGetStr(node, AtString("attenuation_channel"));
    data->attenuation_color          = AiNodeGetRGB(node, "attenuation_color");
    data->attenuation_intensity      = AiNodeGetFlt(node, "attenuation_intensity");
    data->attenuation_mode           = AiNodeGetInt(node, "attenuation_mode");

    data->emission_source            = AiNodeGetInt(node, "emission_source");
    data->emission                   = AiNodeGetRGB(node, "emission");
    data->emission_channel           = AiNodeGetStr(node, AtString("emission_channel"));
    data->emission_color             = AiNodeGetRGB(node, "emission_color");
    data->emission_intensity         = AiNodeGetFlt(node, "emission_intensity");

    switch (data->scattering_source)
    {
    case INPUT_SOURCE_PARAMETER:
        data->scattering_from = data->scattering_is_linked ? INPUT_FROM_EVALUATE : INPUT_FROM_CACHE;
        break;

    case INPUT_SOURCE_CHANNEL:
        data->scattering_from = INPUT_FROM_CHANNEL;
        break;

    default:
        assert("invalid value for data->scattering_source"); break;
        break;
    }

    switch (data->attenuation_source)
    {
    case INPUT_SOURCE_PARAMETER:
        if (data->attenuation_is_linked)
        {
            if (AiNodeGetLink(node, "attenuation") == AiNodeGetLink(node, "scattering"))
                data->attenuation_from = INPUT_FROM_SCATTERING;
            else
                data->attenuation_from = INPUT_FROM_EVALUATE;
        }
        else
            data->attenuation_from = INPUT_FROM_CACHE;
        break;

    case INPUT_SOURCE_CHANNEL:
        if (data->scattering_channel == data->attenuation_channel)
            data->attenuation_from = INPUT_FROM_SCATTERING;
        else
            data->attenuation_from = INPUT_FROM_CHANNEL;
        break;

    case INPUT_SOURCE_SCATTERING:
        data->attenuation_from = INPUT_FROM_SCATTERING;
        break;

    default:
        assert("invalid value for data->attenuation_source"); break;
        break;
    }

    switch (data->emission_source)
    {
    case INPUT_SOURCE_PARAMETER:
        data->emission_from = data->emission_is_linked ? INPUT_FROM_EVALUATE : INPUT_FROM_CACHE;
        break;

    case INPUT_SOURCE_CHANNEL:
        data->emission_from = INPUT_FROM_CHANNEL;
        break;

    default:
        assert("invalid value for data->emission_source"); break;
        break;
    }

    // detect constant zero values for color and intensity
    if ((!data->scattering_intensity_is_linked && data->scattering_intensity == 0.0f) ||
        (!data->scattering_color_is_linked && data->scattering_color == AI_RGB_BLACK))
    {
        data->scattering_from = INPUT_FROM_CACHE;
        data->scattering = AI_RGB_BLACK;
    }

    if ((!data->attenuation_intensity_is_linked && data->attenuation_intensity == 0.0f) ||
        (!data->attenuation_color_is_linked && data->attenuation_color == AI_RGB_BLACK))
    {
        data->attenuation_from = INPUT_FROM_CACHE;
        data->attenuation = AI_RGB_BLACK;
    }

    if ((!data->emission_intensity_is_linked && data->emission_intensity == 0.0f) ||
        (!data->emission_color_is_linked && data->emission_color == AI_RGB_BLACK))
    {
        data->emission_from = INPUT_FROM_NONE;
        data->emission = AI_RGB_BLACK;
    }
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
    AtVector Po_orig = sg->Po;
    AtVector P_orig = sg->P;

    switch (data->position_offset_from)
    {
    case INPUT_FROM_EVALUATE:
    {
        sg->Po += AiShaderEvalParamVec(p_position_offset);
        sg->P = AiM4PointByMatrixMult(sg->M, sg->Po);
        break;
    }

    case INPUT_FROM_CACHE:
        sg->Po += data->position_offset;
        sg->P = AiM4PointByMatrixMult(sg->M, sg->Po);
        break;

    default:
        break;
    }

    // the values storing the result of AiVolumeSampleRGB() need to be zeroed
    // or NaNs will occur in optimized builds (htoa#374)
    AtRGB scattering  = AI_RGB_BLACK;
    AtRGB attenuation = AI_RGB_BLACK;

    AtRGB absorption_result = AI_RGB_BLACK;
    AtRGB scattering_result = AI_RGB_BLACK;
    AtRGB emission_result   = AI_RGB_BLACK;
    float anisotropy_result = 0.0f;

    // scattering
    if (!(sg->Rt & AI_RAY_SHADOW) || (data->attenuation_from == INPUT_FROM_SCATTERING) || (data->attenuation_mode == ATTENUATION_MODE_ABSORPTION))
    {
        switch (data->scattering_from)
        {
        case INPUT_FROM_CHANNEL:  AiVolumeSampleRGB(data->scattering_channel, data->interpolation, &scattering); break;
        case INPUT_FROM_EVALUATE: scattering = AiShaderEvalParamRGB(p_scattering); break;
        case INPUT_FROM_CACHE:    scattering = data->scattering; break;
        default: assert("invalid value for data->scattering_from"); break;
        }

       if (!(sg->Rt & AI_RAY_SHADOW) || (data->attenuation_mode == ATTENUATION_MODE_ABSORPTION))
       {
            // color, intensity, anisotropy and clipping
            const AtRGB scattering_color     = data->scattering_color_is_linked     ? AiShaderEvalParamRGB(p_scattering_color)     : data->scattering_color;
            const float scattering_intensity = data->scattering_intensity_is_linked ? AiShaderEvalParamFlt(p_scattering_intensity) : data->scattering_intensity;
            anisotropy_result                = data->anisotropy_is_linked           ? AiShaderEvalParamFlt(p_anisotropy)           : data->anisotropy;

            scattering_result = scattering * scattering_color * scattering_intensity;
            AiColorClipToZero(scattering_result);
       }
    }

    // attenuation
    switch (data->attenuation_from)
    {
    case INPUT_FROM_CHANNEL:    AiVolumeSampleRGB(data->attenuation_channel, data->interpolation, &attenuation); break;
    case INPUT_FROM_EVALUATE:   attenuation = AiShaderEvalParamRGB(p_attenuation); break;
    case INPUT_FROM_CACHE:      attenuation = data->attenuation; break;
    case INPUT_FROM_SCATTERING: attenuation = scattering; break;
    default: assert("invalid value for data->attenuation_from"); break;
    }

    // color, intensity and clipping
    const AtRGB attenuation_color     = data->attenuation_color_is_linked     ? AiShaderEvalParamRGB(p_attenuation_color)     : data->attenuation_color;
    const float attenuation_intensity = data->attenuation_intensity_is_linked ? AiShaderEvalParamFlt(p_attenuation_intensity) : data->attenuation_intensity;
    attenuation *= attenuation_color * attenuation_intensity;
    AiColorClipToZero(attenuation);

    // update volume shader globals
    switch (data->attenuation_mode)
    {
    case ATTENUATION_MODE_ABSORPTION: absorption_result = attenuation; break;
    case ATTENUATION_MODE_EXTINCTION: absorption_result = attenuation - scattering_result; break;
    }

    // emission
    if (!(sg->Rt & AI_RAY_SHADOW) && (data->emission_from != INPUT_FROM_NONE))
    {
        AtRGB emission = AI_RGB_BLACK;

        switch (data->emission_from)
        {
        case INPUT_FROM_CHANNEL:  AiVolumeSampleRGB(data->emission_channel, data->interpolation, &emission); break;
        case INPUT_FROM_EVALUATE: emission = AiShaderEvalParamRGB(p_emission); break;
        case INPUT_FROM_CACHE:    emission = data->emission; break;
        default: assert("invalid value for data->emission_from"); break;
        }

        // color, intensity and clipping
        const AtRGB emission_color     = data->emission_color_is_linked     ? AiShaderEvalParamRGB(p_emission_color)     : data->emission_color;
        const float emission_intensity = data->emission_intensity_is_linked ? AiShaderEvalParamFlt(p_emission_intensity) : data->emission_intensity;
        emission *= emission_color * emission_intensity;
        AiColorClipToZero(emission);

        // update volume shader globals
        if (!AiColorIsSmall(emission))
            emission_result = emission;
    }

    // create closure
    sg->out.CLOSURE() = AiClosureVolumeHenyeyGreenstein(sg, absorption_result, scattering_result, emission_result, anisotropy_result);

    // restore sampling position
    if (data->position_offset_from != INPUT_FROM_NONE)
    {
        sg->Po = Po_orig;
        sg->P = P_orig;
    }
}
