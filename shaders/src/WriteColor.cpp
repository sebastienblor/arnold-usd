#include <ai.h>

#define ARNOLD_NODEID_WRITECOLOR                0x00115D10

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
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_WRITECOLOR);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterRGBA("beauty", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGBA("input", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterSTR("aov_name", "");
}

shader_evaluate
{
   sg->out.RGBA = AiShaderEvalParamRGBA(p_beauty);

   if (sg->Rt & AI_RAY_CAMERA)
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
