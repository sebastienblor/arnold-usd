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
   AiMetaDataSetStr(nentry, NULL, "_synonym", "writeColor");
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiWriteColor_rgba");
   AiMetaDataSetBool(nentry, NULL, "maya.swatch", false);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "aov.shader", true);

   AiParameterRGBA("passthrough", 0.0f, 0.0f, 0.0f, 1.0f);
   AiParameterRGB("aov_input", 0.0f, 0.0f, 0.0f);
   AiParameterStr("aov_name", "");
   AiParameterBool("blend_opacity", false);
}

shader_evaluate
{
   sg->out.RGBA() = AiShaderEvalParamRGBA(p_beauty);

   if (sg->Rt & AI_RAY_CAMERA)
   {
      const AtRGB input = AiShaderEvalParamRGB(p_input);
      AiAOVSetRGB(sg, AtString(AiShaderEvalParamStr(p_name)), input);
   }
}

node_initialize
{
}

node_update
{
   if (AiNodeGetBool(node, "blend_opacity"))
      AiAOVRegister(AiNodeGetStr(node, "aov_name"), AI_TYPE_RGB, AI_AOV_BLEND_OPACITY);
}

node_finish
{
}
