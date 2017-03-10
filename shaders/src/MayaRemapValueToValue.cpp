#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaRemapValueToValueMtd);

namespace
{

enum MayaRemapValueToValueParams
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
   AiParameterArray("values", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterArray("interpolations", AiArray(2, 1, AI_TYPE_INT, 1, 1));
   AiParameterFlt("outputMin", 0.0f);
   AiParameterFlt("outputMax", 1.0f);

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
   float input = AiShaderEvalParamFlt(p_input);
   float imin = AiShaderEvalParamFlt(p_input_min);
   float imax = AiShaderEvalParamFlt(p_input_max);
   float omin = AiShaderEvalParamFlt(p_output_min);
   float omax = AiShaderEvalParamFlt(p_output_max);
   AtArray *pos = AiShaderEvalParamArray(p_key_pos);
   AtArray *val = AiShaderEvalParamArray(p_key_val);
   AtArray *interp = AiShaderEvalParamArray(p_key_interp);
   
   unsigned int* shuffle = (unsigned int*)AiShaderGlobalsQuickAlloc(sg, AiArrayGetNumElements(pos) * sizeof(unsigned int));
   SortFloatIndexArray(pos, shuffle);

   float output = 0.0f;

   input = MapValue(input, imin, imax);

   InterpolateShuffle(pos, val, interp, input, output, shuffle);

   sg->out.FLT() = UnmapValue(output, omin, omax);
}
