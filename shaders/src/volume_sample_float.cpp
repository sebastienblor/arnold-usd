// $Id: volume_sample_float.cpp 1090 2014-11-14 12:50:32Z kik $

#include <ai.h>
#include <algorithm>
#include <string>

#ifndef SHADER_PREFIX
#define SHADER_PREFIX ""
#endif

AI_SHADER_NODE_EXPORT_METHODS(VolumeSampleFloatMethods)

namespace {

inline float bias(float x, float inv_bias)
{
    return x / (((inv_bias - 2.0f) * (1.0f - x)) + 1.0f);
}

inline float gain(float x, float inv_gain, float inv_one_minus_gain)
{
    if (x < 0.5f)
        return bias(x * 2.0f, inv_gain) * 0.5f;
    else
        return bias(x * 2.0f - 1.0f, inv_one_minus_gain) * 0.5f + 0.5f;
}

enum Params
{
    p_channel,
    p_position_offset,
    p_interpolation,
    p_input_min,
    p_input_max,
    p_contrast,
    p_contrast_pivot,
    p_bias,
    p_gain,
    p_output_min,
    p_output_max,
    p_clamp_min,
    p_clamp_max,
};

/// interpolation labels
const char* interpolation_labels[] = { "closest", "trilinear", "tricubic", NULL };

enum InputFrom
{
    INPUT_FROM_NONE,
    INPUT_FROM_EVALUATE,
    INPUT_FROM_CACHE,
};

struct ShaderData
{
    AtString channel;
    AtVector position_offset;
    bool position_offset_is_linked;
    InputFrom position_offset_from;
    int interpolation;
    float input_min;
    float input_max;
    float inv_input_range;
    float contrast;
    float contrast_pivot;
    float bias;
    float inv_bias;
    float gain;
    float inv_gain;
    float inv_one_minus_gain;
    float output_min;
    float output_max;
    float output_range;
    bool clamp_min;
    bool clamp_max;
};

} // namespace

node_parameters
{
    AiParameterStr("channel", "");
    AiParameterVec("position_offset", 0, 0, 0);
    AiParameterEnum("interpolation", AI_VOLUME_INTERP_TRILINEAR, interpolation_labels);
    AiParameterFlt("input_min", 0.0f);
    AiParameterFlt("input_max", 1.0f);
    AiParameterFlt("contrast", 1.0f);
    AiParameterFlt("contrast_pivot", 0.5f);
    AiParameterFlt("bias", 0.5f);
    AiParameterFlt("gain", 0.5f);
    AiParameterFlt("output_min", 0.0f);
    AiParameterFlt("output_max", 1.0f);
    AiParameterBool("clamp_min", false);
    AiParameterBool("clamp_max", false);

    AiMetaDataSetBool(nentry, "channel", "linkable", false);
    AiMetaDataSetBool(nentry, "interpolation", "linkable", false);
    AiMetaDataSetBool(nentry, "input_min", "linkable", false);
    AiMetaDataSetBool(nentry, "input_max", "linkable", false);
    AiMetaDataSetBool(nentry, "contrast", "linkable", false);
    AiMetaDataSetBool(nentry, "contrast_pivot", "linkable", false);
    AiMetaDataSetBool(nentry, "bias", "linkable", false);
    AiMetaDataSetBool(nentry, "gain", "linkable", false);
    AiMetaDataSetBool(nentry, "output_min", "linkable", false);
    AiMetaDataSetBool(nentry, "output_max", "linkable", false);
    AiMetaDataSetBool(nentry, "clamp_min", "linkable", false);
    AiMetaDataSetBool(nentry, "clamp_max", "linkable", false);
}

node_initialize
{
    AiNodeSetLocalData(node, new ShaderData);
}

node_update
{
   ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

   // channel
   data->channel = AiNodeGetStr(node, "channel");

   // position offset
   data->position_offset = AiNodeGetVec(node, "position_offset");

   if (AiNodeIsLinked(node, "position_offset"))
       data->position_offset_from = INPUT_FROM_EVALUATE;
   else if (data->position_offset != AI_V3_ZERO)
       data->position_offset_from = INPUT_FROM_CACHE;
   else
       data->position_offset_from = INPUT_FROM_NONE;

   // interpolation
   data->interpolation = AiNodeGetInt(node, "interpolation");

   // adjust
   data->input_min = AiNodeGetFlt(node, "input_min");
   data->input_max = AiNodeGetFlt(node, "input_max");
   data->contrast = AiNodeGetFlt(node, "contrast");
   data->contrast_pivot = AiNodeGetFlt(node, "contrast_pivot");
   data->bias = AiNodeGetFlt(node, "bias");
   data->gain = AiNodeGetFlt(node, "gain");
   data->output_min = AiNodeGetFlt(node, "output_min");
   data->output_max = AiNodeGetFlt(node, "output_max");
   data->clamp_min = AiNodeGetBool(node, "clamp_min");
   data->clamp_max = AiNodeGetBool(node, "clamp_max");

   // cache
   data->inv_input_range = 1.0f / (data->input_max - data->input_min);
   data->output_range = data->output_max - data->output_min;
   data->inv_bias = 1.0f / data->bias;
   data->inv_gain = 1.0f / data->gain;
   data->inv_one_minus_gain = 1.0f / (1.0f - data->gain);
}

shader_evaluate
{
   ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

   // sampling position offset
   AtVector Po_orig;

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

   // the values storing the result of AiVolumeSampleFlt() need to be zeroed
   // or NaNs might occur in optimized builds (htoa#374)
   float value = 0.0f;

   if (!data->channel.empty())
      AiVolumeSampleFlt(data->channel, data->interpolation, &value);
   else
       value = 0.0f;

   // adjust
   value = (value - data->input_min) * data->inv_input_range;
   value = (value - data->contrast_pivot) * data->contrast + data->contrast_pivot;
   if (data->bias != 0.5f) value = bias(value, data->inv_bias);
   if (data->gain != 0.5f) value = gain(value, data->inv_gain, data->inv_one_minus_gain);
   value = value * data->output_range + data->output_min;
   if (data->clamp_min) value = std::max(value, data->output_min);
   if (data->clamp_max) value = std::min(value, data->output_max);

   sg->out.FLT() = value;

   // restore sampling position
   if (data->position_offset_from != INPUT_FROM_NONE)
       sg->Po = Po_orig;
}

node_finish
{
   ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
   delete data;
}

void registerVolumeSampleFloat(AtNodeLib *node)
{
    node->methods = VolumeSampleFloatMethods;
    node->output_type = AI_TYPE_FLOAT;
    node->name = SHADER_PREFIX "volume_sample_float";
    node->node_type = AI_NODE_SHADER;
}
