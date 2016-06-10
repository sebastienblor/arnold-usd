// $Id: volume_sample_rgb.cpp 1183 2015-02-03 13:31:16Z sjannuz $

#include "color_utils.h"
#include <ai.h>
#include <algorithm>
#include <string>

#ifndef SHADER_PREFIX
#define SHADER_PREFIX ""
#endif

AI_SHADER_NODE_EXPORT_METHODS(VolumeSampleRgbMethods)

namespace {

inline void AiColorGamma(AtRGB *color, float gamma)
{
   if (gamma == 1.0f)
      return;
   gamma = 1.0f / gamma;
   color->r = powf(color->r, gamma);
   color->g = powf(color->g, gamma);
   color->b = powf(color->b, gamma);
}

enum Params
{
    p_channel,
    p_position_offset,
    p_interpolation,
    p_gamma,
    p_hue_shift,
    p_saturation,
    p_contrast,
    p_contrast_pivot,
    p_exposure,
    p_multiply,
    p_add,
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
    float gamma;
    float hue_shift;
    float saturation;
    float contrast;
    float contrast_pivot;
    float exposure;
    float multiply;
    float add;
};

} // namespace

node_parameters
{
    AiParameterStr("channel", "");
    AiParameterVec("position_offset", 0, 0, 0);
    AiParameterEnum("interpolation", AI_VOLUME_INTERP_TRILINEAR, interpolation_labels);
    AiParameterFlt("gamma", 1.0f);
    AiParameterFlt("hue_shift", 0.0f);
    AiParameterFlt("saturation", 1.0f);
    AiParameterFlt("contrast", 1.0f);
    AiParameterFlt("contrast_pivot", 0.18f); // perceptual mid-gray
    AiParameterFlt("exposure", 0.0f);
    AiParameterFlt("multiply", 1.0f);
    AiParameterFlt("add", 0.0f);

    AiMetaDataSetBool(nentry, "channel", "linkable", false);
    AiMetaDataSetBool(nentry, "interpolation", "linkable", false);
    AiMetaDataSetBool(nentry, "gamma", "linkable", false);
    AiMetaDataSetBool(nentry, "hue_shift", "linkable", false);
    AiMetaDataSetBool(nentry, "saturation", "linkable", false);
    AiMetaDataSetBool(nentry, "contrast", "linkable", false);
    AiMetaDataSetBool(nentry, "contrast_pivot", "linkable", false);
    AiMetaDataSetBool(nentry, "exposure", "linkable", false);
    AiMetaDataSetBool(nentry, "multiply", "linkable", false);
    AiMetaDataSetBool(nentry, "add", "linkable", false);
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

   // color correction
   data->gamma = AiNodeGetFlt(node, "gamma");
   data->hue_shift = AiNodeGetFlt(node, "hue_shift");
   data->saturation = AiNodeGetFlt(node, "saturation");
   data->contrast = AiNodeGetFlt(node, "contrast");
   data->contrast_pivot = AiNodeGetFlt(node, "contrast_pivot");
   data->exposure = AiNodeGetFlt(node, "exposure");
   data->multiply = AiNodeGetFlt(node, "multiply");
   data->add = AiNodeGetFlt(node, "add");
}

shader_evaluate
{
   ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

   if (data->multiply == 0.0f)
   {
       sg->out.RGB() = data->add;
       return;
   }

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

   // the values storing the result of AiVolumeSampleRGB() need to be zeroed
   // or NaNs will occur in optimized builds (htoa#374)
   AtRGB color = AI_RGB_BLACK;

   if (!data->channel.empty())
       AiVolumeSampleRGB(data->channel, data->interpolation, &color);
   else
       color = AI_RGB_BLACK;

   // color correction
   if (data->gamma != 1.0f)
       AiColorGamma(&color, data->gamma);

   if (data->hue_shift != 0.0f || data->saturation != 1.0f)
   {
       AtRGB hsl = convertFromRGB(color, COLOR_SPACE_HSL);

       hsl.r += data->hue_shift;
       hsl.r = hsl.r - floorf(hsl.r); // keep hue in [0, 1]

       hsl.g *= data->saturation;
       color = convertToRGB(hsl, COLOR_SPACE_HSL);
   }

   if (data->contrast != 1.0f)
       color = (color - data->contrast_pivot) * data->contrast + data->contrast_pivot;

   if (data->exposure != 0.0f)
       color *= powf(2.0f, data->exposure);

   if (data->multiply != 1.0f)
       color *= data->multiply;

   if (data->add != 0.0f)
       color += data->add;

   sg->out.RGB() = color;

   // restore sampling position
   if (data->position_offset_from != INPUT_FROM_NONE)
       sg->Po = Po_orig;
}

node_finish
{
   ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
   delete data;
}

void registerVolumeSampleRgb(AtNodeLib *node)
{
    node->methods = VolumeSampleRgbMethods;
    node->output_type = AI_TYPE_RGB;
    node->name = SHADER_PREFIX "volume_sample_rgb";
    node->node_type = AI_NODE_SHADER;
}
