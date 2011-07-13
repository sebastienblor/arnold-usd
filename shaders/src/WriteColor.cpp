#include <ai.h>

namespace
{

enum WriteColorParams
{
   p_beauty,
   p_input,
   p_name
};

};

AI_SHADER_NODE_EXPORT_METHODS(WriteColorMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiWriteColor");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D10);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/surface");

   AiParameterRGBA("beauty", 0, 0, 0, 0);
   AiParameterRGBA("input", 0, 0, 0, 0);
   AiParameterSTR("aov_name", "customColor");

   AiMetaDataSetInt(mds, "aov_name", "aov.type", AI_TYPE_RGBA);
}

shader_evaluate
{
   sg->out.RGBA = AiShaderEvalParamRGBA(p_beauty);

   AiAOVSetRGBA(sg, AiShaderEvalParamStr(p_name), AiShaderEvalParamRGBA(p_input));
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
