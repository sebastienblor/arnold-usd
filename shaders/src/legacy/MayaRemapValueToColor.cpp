#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaRemapValueToColorMtd);

namespace
{

enum MayaRemapValueToColorParams
{
   p_input,
   p_input_min,
   p_input_max,
   p_key_pos,
   p_key_val,
   p_key_interp,
   p_output_min,
   p_output_max
};

};

node_parameters
{
   AiParameterFlt("input", 0.0f);
   AiParameterFlt("inputMin", 0.0f);
   AiParameterFlt("inputMax", 1.0f);
   AiParameterArray("positions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AtArray *vdef = AiArrayAllocate(2, 1, AI_TYPE_RGB);
   AiArraySetRGB(vdef, 0, AI_RGB_BLACK);
   AiArraySetRGB(vdef, 1, AI_RGB_WHITE);
   AiParameterArray("values", vdef);
   AiParameterArray("interpolations", AiArray(2, 1, AI_TYPE_INT, 1, 1));;
   AiParameterFlt("outputMin", 0.0f);
   AiParameterFlt("outputMax", 1.0f);

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
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
   float input = AiShaderEvalParamFlt(p_input);
   float imin = AiShaderEvalParamFlt(p_input_min);
   float imax = AiShaderEvalParamFlt(p_input_max);

   AtRGB output;
   float omin = AiShaderEvalParamFlt(p_output_min);
   float omax = AiShaderEvalParamFlt(p_output_max);

   AtArray *pos = AiShaderEvalParamArray(p_key_pos);
   AtArray *val = AiShaderEvalParamArray(p_key_val);
   AtArray *interp = AiShaderEvalParamArray(p_key_interp);

   unsigned int* shuffle = (unsigned int*)AiShaderGlobalsQuickAlloc(sg, AiArrayGetNumElements(pos) * sizeof(unsigned int));
   SortFloatIndexArray(pos, shuffle);
   
   input = MapValue(input, imin, imax);

   InterpolateShuffle(pos, val, interp, input, output, shuffle);

   sg->out.RGB().r = UnmapValue(output.r, omin, omax);
   sg->out.RGB().g = UnmapValue(output.g, omin, omax);
   sg->out.RGB().b = UnmapValue(output.b, omin, omax);
}
