#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaReverseMtd);

namespace
{

enum MayaReverseParams
{
   p_input
};

};

node_parameters
{
   AiParameterRGB("input", 0, 0, 0);

   AiMetaDataSetStr(mds, NULL, "maya.name", "reverse");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x52525653);
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

   sg->out.RGB() = AI_RGB_WHITE - input;
}
