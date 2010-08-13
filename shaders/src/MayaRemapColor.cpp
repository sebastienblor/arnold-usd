
#include "MayaUtils.h"
#include <ai.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

AI_SHADER_NODE_EXPORT_METHODS(MayaRemapColorMtd);

namespace
{

enum RemapParams
{
   p_input,
   p_input_min,
   p_input_max,
   p_key_red_pos,
   p_key_red_val,
   p_key_red_interp,
   p_key_green_pos,
   p_key_green_val,
   p_key_green_interp,
   p_key_blue_pos,
   p_key_blue_val,
   p_key_blue_interp,
   p_output_min,
   p_output_max
};

float map_value(float v, float vmin, float vmax)
{
   return ((v - vmin) / (vmax - vmin));
}

float unmap_value(float v, float vmin, float vmax)
{
   return (vmin + (vmax - vmin) * v);
}

};

node_parameters
{
   AiParameterRGB("input", 0.0f, 0.0f, 0.0f);
   AiParameterFLT("inputMin", 0.0f);
   AiParameterFLT("inputMax", 1.0f);
   AiParameterARRAY("rPositions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("rValues", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("rInterpolations", AiArray(2, 1, AI_TYPE_STRING, "linear", "linear"));
   AiParameterARRAY("gPositions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("gValues", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("gInterpolations", AiArray(2, 1, AI_TYPE_STRING, "linear", "linear"));
   AiParameterARRAY("bPositions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("bValues", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("bInterpolations", AiArray(2, 1, AI_TYPE_STRING, "linear", "linear"));
   AiParameterFLT("outputMin", 0.0f);
   AiParameterFLT("outputMax", 1.0f);
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
   AtRGB input = AiShaderEvalParamRGB(p_input);
   float imin = AiShaderEvalParamFlt(p_input_min);
   float imax = AiShaderEvalParamFlt(p_input_max);

   AtRGB output;
   float omin = AiShaderEvalParamFlt(p_output_min);
   float omax = AiShaderEvalParamFlt(p_output_max);

   input.r = map_value(input.r, imin, imax);
   input.g = map_value(input.g, imin, imax);
   input.b = map_value(input.b, imin, imax);

   Interpolate(AiShaderEvalParamArray(p_key_red_pos),
               AiShaderEvalParamArray(p_key_red_val),
               AiShaderEvalParamArray(p_key_red_interp),
               input.r,
               output.r);

   Interpolate(AiShaderEvalParamArray(p_key_green_pos),
               AiShaderEvalParamArray(p_key_green_val),
               AiShaderEvalParamArray(p_key_green_interp),
               input.g,
               output.g);

   Interpolate(AiShaderEvalParamArray(p_key_blue_pos),
               AiShaderEvalParamArray(p_key_blue_val),
               AiShaderEvalParamArray(p_key_blue_interp),
               input.b,
               output.b);

   sg->out.RGB.r = unmap_value(output.r, omin, omax);
   sg->out.RGB.g = unmap_value(output.g, omin, omax);
   sg->out.RGB.b = unmap_value(output.b, omin, omax);
}
