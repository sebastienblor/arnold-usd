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

   AiMetaDataSetStr(nentry, NULL, "maya.name", "reverse");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52525653);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
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

   sg->out.RGB() = AI_RGB_WHITE - input;
}
