#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaRemapHsvMtd);

namespace
{

enum MayaRemapHsvParams
{
   p_input,
   p_input_min,
   p_input_max,
   p_key_hue_pos,
   p_key_hue_val,
   p_key_hue_interp,
   p_key_saturation_pos,
   p_key_saturation_val,
   p_key_saturation_interp,
   p_key_value_pos,
   p_key_value_val,
   p_key_value_interp,
   p_output_min,
   p_output_max
};

};

node_parameters
{
   AiParameterRGB("input", 0.0f, 0.0f, 0.0f);
   AiParameterFLT("inputMin", 0.0f);
   AiParameterFLT("inputMax", 1.0f);
   AiParameterARRAY("hPositions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("hValues", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("hInterpolations", AiArray(2, 1, AI_TYPE_STRING, "linear", "linear"));
   AiParameterARRAY("sPositions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("sValues", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("sInterpolations", AiArray(2, 1, AI_TYPE_STRING, "linear", "linear"));
   AiParameterARRAY("vPositions", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("vValues", AiArray(2, 1, AI_TYPE_FLOAT, 0.0f, 1.0f));
   AiParameterARRAY("vInterpolations", AiArray(2, 1, AI_TYPE_STRING, "linear", "linear"));
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
   float omin = AiShaderEvalParamFlt(p_output_min);
   float omax = AiShaderEvalParamFlt(p_output_max);

   AtVector inputHsv = RGBtoHSV(input);
   AtVector outputHsv;

   // Bring hue value in [0, 1] range
   inputHsv.x = inputHsv.x / 360.0f;

   inputHsv.x = MapValue(inputHsv.x, imin, imax);
   inputHsv.y = MapValue(inputHsv.y, imin, imax);
   inputHsv.z = MapValue(inputHsv.z, imin, imax);

   Interpolate(AiShaderEvalParamArray(p_key_hue_pos),
               AiShaderEvalParamArray(p_key_hue_val),
               AiShaderEvalParamArray(p_key_hue_interp),
               inputHsv.x,
               outputHsv.x);

   Interpolate(AiShaderEvalParamArray(p_key_saturation_pos),
               AiShaderEvalParamArray(p_key_saturation_val),
               AiShaderEvalParamArray(p_key_saturation_interp),
               inputHsv.y,
               outputHsv.y);

   Interpolate(AiShaderEvalParamArray(p_key_value_pos),
               AiShaderEvalParamArray(p_key_value_val),
               AiShaderEvalParamArray(p_key_value_interp),
               inputHsv.z,
               outputHsv.z);

   outputHsv.x = UnmapValue(outputHsv.x, omin, omax);
   outputHsv.y = UnmapValue(outputHsv.y, omin, omax);
   outputHsv.z = UnmapValue(outputHsv.z, omin, omax);

   // Set hue value back in [0, 360] range
   outputHsv.x = outputHsv.x * 360.0f;

   sg->out.RGB = HSVtoRGB(outputHsv);
}

