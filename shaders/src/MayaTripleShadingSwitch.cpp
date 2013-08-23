#include <ai.h>


AI_SHADER_NODE_EXPORT_METHODS(MayaTripleShadingSwitchMtd);

node_parameters
{
	AiParameterRGB("default", 0.8f, 0.8f, 0.8f);
}

enum MayaTripleShadingSwitchParams{
   p_default = 0,
};

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
   sg->out.RGB = AiShaderEvalParamRGB(p_default);
}
