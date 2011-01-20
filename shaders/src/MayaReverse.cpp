
#include <ai_metadata.h>
#include <ai_nodes.h>
#include <ai_shaderglobals.h>
#include <ai_shaders.h>
#include <ai_shader_parameval.h>

namespace
{

enum MayaReverseParams
{
   p_input
};

};

AI_SHADER_NODE_EXPORT_METHODS(MayaReverseMtd);

node_parameters
{
   AiParameterRGB("input", 0, 0, 0);

   AiMetaDataSetStr(mds, NULL, "maya.counterpart", "reverse");
   AiMetaDataSetInt(mds, NULL, "maya.counterpart_id", 0x52525653);
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

   AiColorSub(sg->out.RGB, AI_RGB_WHITE, input);
}
