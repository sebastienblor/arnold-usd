#include <ai.h>

namespace
{

enum MayaClampParams
{
   p_min,
   p_max,
   p_input
};

};

AI_SHADER_NODE_EXPORT_METHODS(MayaClampMtd);

node_parameters
{
   AiParameterRGB("min", 0, 0, 0);
   AiParameterRGB("max", 1, 1, 1);
   AiParameterRGB("input", 0, 0, 0);

   AiMetaDataSetBool(nentry, "min", "always_linear", true);
   AiMetaDataSetBool(nentry, "max", "always_linear", true);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "clamp");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52434c33);
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
   AtColor input = AiShaderEvalParamRGB(p_input);
   AtColor min   = AiShaderEvalParamRGB(p_min);
   AtColor max   = AiShaderEvalParamRGB(p_max);

   sg->out.RGB().r = (input.r < min.r) ? min.r : ((input.r > max.r) ? max.r : input.r);
   sg->out.RGB().g = (input.g < min.g) ? min.g : ((input.g > max.g) ? max.g : input.g);
   sg->out.RGB().b = (input.b < min.b) ? min.b : ((input.b > max.b) ? max.b : input.b);
}
