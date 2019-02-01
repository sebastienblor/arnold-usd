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

   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   
   //AiMetaDataSetStr(nentry, NULL, "maya.name", "clamp");
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52434c33);
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
   AtRGB min   = AiShaderEvalParamRGB(p_min);
   AtRGB max   = AiShaderEvalParamRGB(p_max);

   sg->out.RGB().r = (input.r < min.r) ? min.r : ((input.r > max.r) ? max.r : input.r);
   sg->out.RGB().g = (input.g < min.g) ? min.g : ((input.g > max.g) ? max.g : input.g);
   sg->out.RGB().b = (input.b < min.b) ? min.b : ((input.b > max.b) ? max.b : input.b);
}
