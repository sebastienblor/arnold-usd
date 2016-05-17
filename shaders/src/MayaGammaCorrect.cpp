#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaGammaCorrectMtd);

namespace
{

enum MayaGammaCorrectParams
{
   p_value,
   p_gamma
};

};

node_parameters
{
   AiParameterVec("value", 0, 0, 0);
   AiParameterVec("gamma", 1, 1, 1);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "gammaCorrect");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x5247414d);
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
   AtVector value = AiShaderEvalParamVec(p_value);
   AtVector gamma = AiShaderEvalParamVec(p_gamma);

   if (value.x < 0.f) value.x = 0.f;
   if (value.y < 0.f) value.y = 0.f;
   if (value.z < 0.f) value.z = 0.f;

   if (gamma.x < AI_EPSILON) gamma.x = (float) AI_EPSILON; 
   if (gamma.y < AI_EPSILON) gamma.y = (float) AI_EPSILON; 
   if (gamma.z < AI_EPSILON) gamma.z = (float) AI_EPSILON; 

   sg->out.RGB().r = powf(value.x, 1.f / gamma.x);
   sg->out.RGB().g = powf(value.y, 1.f / gamma.y);
   sg->out.RGB().b = powf(value.z, 1.f / gamma.z);

}
