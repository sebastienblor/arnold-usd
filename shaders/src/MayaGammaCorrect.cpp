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
   AiParameterRGB("gamma", 1, 1, 1);
   AiMetaDataSetBool(mds, "gamma", "always_linear", TRUE); // no gamma correction

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
   AtColor gamma = AiShaderEvalParamRGB(p_gamma);

   if (value.r < AI_EPSILON)
      sg->out.RGB.r = 0.0f;
   else
      sg->out.RGB.r = (gamma.r == 0.0f || gamma.r == 1.0f) ? value.r : pow(value.r, 1.0f / gamma.r);

   if (value.g < AI_EPSILON)
      sg->out.RGB.g = 0.0f;
   else
      sg->out.RGB.g = (gamma.g == 0.0f || gamma.g == 1.0f) ? value.g : pow(value.g, 1.0f / gamma.g);

   if (value.b < AI_EPSILON)
      sg->out.RGB.b = 0.0f;
   else
      sg->out.RGB.b = (gamma.r == 0.0f || gamma.b == 1.0f) ? value.b : pow(value.b, 1.0f / gamma.b);

}
