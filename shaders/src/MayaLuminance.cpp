
#include <ai_metadata.h>
#include <ai_nodes.h>
#include <ai_shaderglobals.h>
#include <ai_shaders.h>
#include <ai_shader_parameval.h>

namespace
{

enum MayaLuminanceParams
{
   p_value
};

};

AI_SHADER_NODE_EXPORT_METHODS(MayaLuminanceMtd);

node_parameters
{
   AiParameterRGB("value", 0, 0, 0);

   AiMetaDataSetStr(mds, NULL, "maya.counterpart", "luminance");
   AiMetaDataSetInt(mds, NULL, "maya.counterpart_id", 0x524c554d);
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
   AtColor input = AiShaderEvalParamRGB(p_value);
   sg->out.FLT = input.r * 0.3f + input.g * 0.59f + input.b * 0.11f;
}
