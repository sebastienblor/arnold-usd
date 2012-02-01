#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaRemapColorMtd);

namespace
{

enum MayaRemapColorParams
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

};

node_parameters
{
   AiParameterRGB("input", 0.0f, 0.0f, 0.0f);
   AiParameterFLT("inputMin", 0.0f);
   AiParameterFLT("inputMax", 1.0f);
   AiParameterARRAY("rPositions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("rValues", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("rInterpolations", AiArray(2, 1, AI_TYPE_INT, 1, 1));
   AiParameterARRAY("gPositions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("gValues", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("gInterpolations", AiArray(2, 1, AI_TYPE_INT, 1, 1));
   AiParameterARRAY("bPositions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("bValues", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("bInterpolations", AiArray(2, 1, AI_TYPE_INT, 1, 1));
   AiParameterFLT("outputMin", 0.0f);
   AiParameterFLT("outputMax", 1.0f);

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
   AtRGB input = AiShaderEvalParamRGB(p_input);
   float imin = AiShaderEvalParamFlt(p_input_min);
   float imax = AiShaderEvalParamFlt(p_input_max);

   AtRGB output;
   float omin = AiShaderEvalParamFlt(p_output_min);
   float omax = AiShaderEvalParamFlt(p_output_max);

   input.r = MapValue(input.r, imin, imax);
   input.g = MapValue(input.g, imin, imax);
   input.b = MapValue(input.b, imin, imax);
   
   AtArray* rPos = AiShaderEvalParamArray(p_key_red_pos);
   unsigned int* rShuffle = (unsigned int*)AiShaderGlobalsQuickAlloc(sg, rPos->nelements * sizeof(unsigned int));
   SortFloatIndexArray(rPos, rShuffle);
   
   AtArray* gPos = AiShaderEvalParamArray(p_key_green_pos);
   unsigned int* gShuffle = (unsigned int*)AiShaderGlobalsQuickAlloc(sg, gPos->nelements * sizeof(unsigned int));
   SortFloatIndexArray(gPos, gShuffle);
   
   AtArray* bPos = AiShaderEvalParamArray(p_key_blue_pos);
   unsigned int* bShuffle = (unsigned int*)AiShaderGlobalsQuickAlloc(sg, bPos->nelements * sizeof(unsigned int));
   SortFloatIndexArray(bPos, bShuffle);

   InterpolateShuffle(rPos,
                     AiShaderEvalParamArray(p_key_red_val),
                     AiShaderEvalParamArray(p_key_red_interp),
                     input.r,
                     output.r,
                     rShuffle);

   InterpolateShuffle(gPos,
                     AiShaderEvalParamArray(p_key_green_val),
                     AiShaderEvalParamArray(p_key_green_interp),
                     input.g,
                     output.g,
                     gShuffle);

   InterpolateShuffle(bPos,
                     AiShaderEvalParamArray(p_key_blue_val),
                     AiShaderEvalParamArray(p_key_blue_interp),
                     input.b,
                     output.b,
                     bShuffle);

   sg->out.RGB.r = UnmapValue(output.r, omin, omax);
   sg->out.RGB.g = UnmapValue(output.g, omin, omax);
   sg->out.RGB.b = UnmapValue(output.b, omin, omax);
}
