#include <ai.h>

#define ARNOLD_NODEID_WRITEFLOAT                0x00115D11

namespace
{

enum WriteFloatParams
{
   p_beauty,
   p_input,
   p_name
};


};

AI_SHADER_NODE_EXPORT_METHODS(WriteFloatMtd);

node_parameters
{
   // Node metadata
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiWriteFloat");
   AiMetaDataSetInt(mds, NULL, "maya.id", ARNOLD_NODEID_WRITEFLOAT);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/utility");
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);

   AiParameterRGBA("beauty", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterFlt("input", 0.0f);
   AiParameterStr("aov_name", "");
}

shader_evaluate
{
   sg->out.RGBA() = AiShaderEvalParamRGBA(p_beauty);
   if (sg->Rt & AI_RAY_CAMERA)
      AiAOVSetFlt(sg, AtString(AiShaderEvalParamStr(p_name)), AiShaderEvalParamFlt(p_input));
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
