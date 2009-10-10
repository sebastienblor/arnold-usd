
#include <ai_metadata.h>
#include <ai_nodes.h>
#include <ai_shaderglobals.h>
#include <ai_shaders.h>
#include <ai_shader_parameval.h>

enum MayaClampParams {
   p_value,
   p_gamma
};

AI_SHADER_NODE_EXPORT_METHODS(MayaGammaCorrectMtd);

node_parameters
{
   AiParameterRGB("value", 0, 0, 0);
   AiParameterRGB("gamma", 1, 1, 1);
   AiMetaDataSetBool(mds, "gamma", "always_linear", TRUE); // no gamma correction
}

node_initialize
{
}

node_finish
{
}

shader_evaluate
{
   AtColor value = AiShaderEvalParamRGB(p_value);
   AtColor gamma = AiShaderEvalParamRGB(p_gamma);

   sg->out.RGB.r = (gamma.r == 0) ? value.r : pow(value.r, 1.0f / gamma.r);
   sg->out.RGB.g = (gamma.g == 0) ? value.g : pow(value.g, 1.0f / gamma.g);
   sg->out.RGB.b = (gamma.b == 0) ? value.b : pow(value.b, 1.0f / gamma.b);
}
