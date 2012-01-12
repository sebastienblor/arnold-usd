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

   if (value.r < AI_EPSILON)
      sg->out.RGB.r = 0.0f;
   else
      sg->out.RGB.r = (gamma.x == 0.0f || gamma.x == 1.0f) ? value.r : pow(value.r, 1.0f / gamma.x);

   if (value.g < AI_EPSILON)
      sg->out.RGB.g = 0.0f;
   else
      sg->out.RGB.g = (gamma.y == 0.0f || gamma.y == 1.0f) ? value.g : pow(value.g, 1.0f / gamma.y);

   if (value.b < AI_EPSILON)
      sg->out.RGB.b = 0.0f;
   else
      sg->out.RGB.b = (gamma.x == 0.0f || gamma.z == 1.0f) ? value.b : pow(value.b, 1.0f / gamma.z);

}
