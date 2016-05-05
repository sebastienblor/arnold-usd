#include <ai.h>

#define ARNOLD_NODEID_WRITECOLOR                0x00115D10

namespace
{

   enum WriteColorParams
   {
      p_beauty,
      p_input,
      p_name,
      p_blend
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
   AiParameterStr("aov_name", "");
   AiParameterBool("blend", false);
}

shader_evaluate
{
   sg->out.RGBA() = AiShaderEvalParamRGBA(p_beauty);

   if (sg->Rt & AI_RAY_CAMERA)
   {
      const AtRGBA input = AiShaderEvalParamRGBA(p_input);
      AiAOVSetRGBA(sg, AtString(AiShaderEvalParamStr(p_name)), input);
   }
}

node_initialize
{
}

node_update
{
   if (AiNodeGetBool(node, "blend"))
      AiAOVRegister(AiNodeGetStr(node, "aov_name"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
}

node_finish
{
}
