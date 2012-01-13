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
   AiParameterRGB("value", 0, 0, 0);
   AiParameterVEC("gamma", 1, 1, 1);

   AiMetaDataSetStr(mds, NULL, "maya.name", "gammaCorrect");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x5247414d);
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
   AtColor value = AiShaderEvalParamRGB(p_value);
   AtVector gamma = AiShaderEvalParamVec(p_gamma);

   if (value.r < 0.f) value.r = 0.f; 
   if (value.g < 0.f) value.g = 0.f; 
   if (value.b < 0.f) value.b = 0.f; 

   if (gamma.x < AI_EPSILON) gamma.x = (float) AI_EPSILON; 
   if (gamma.y < AI_EPSILON) gamma.y = (float) AI_EPSILON; 
   if (gamma.z < AI_EPSILON) gamma.z = (float) AI_EPSILON; 

   sg->out.RGB.r = powf(value.r, 1.f / gamma.x); 
   sg->out.RGB.g = powf(value.g, 1.f / gamma.y); 
   sg->out.RGB.b = powf(value.b, 1.f / gamma.z); 

}
