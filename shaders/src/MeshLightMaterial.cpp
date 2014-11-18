#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MeshLightMaterialMtd);

enum MeshLightMaterialParams{
   p_color = 0,
   p_color_multiplier,
   p_aov_meshlight_beauty
};

node_parameters
{
   AiParameterRGB("color", 0.f, 0.f, 0.f);
   AiParameterRGB("color_multiplier", 0.0f, 0.0f, 0.0f);
   AiParameterStr("aov_meshlight_beauty", "meshlight_beauty");
   
   AiMetaDataSetBool(mds, "color", "always_linear", true); // no gamma correction
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiMeshLightMaterial");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D1B);
   //AiMetaDataSetBool(mds, NULL, "maya.hide", true);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/surface");
}

node_initialize
{

}

node_update
{
   AiAOVRegister(AiNodeGetStr(node, "aov_meshlight_beauty"), AI_TYPE_RGB, AI_AOV_BLEND_NONE);
}

node_finish
{
   
}

shader_evaluate
{
   if (sg->Rt & (AI_RAY_DIFFUSE | AI_RAY_GLOSSY | AI_RAY_SHADOW))
   {
      sg->out.RGBA = AI_RGBA_BLACK;   
      return;
   }
   
   if (AiV3Dot(sg->Ng, sg->Ngf) < 0)
   {
      sg->out.RGBA = AI_RGBA_BLACK;   
      return;
   }

   sg->out.RGB = AiShaderEvalParamRGB(p_color) * AiShaderEvalParamRGB(p_color_multiplier);
   sg->out.RGBA.a = 1.f;

   if (sg->Rt & AI_RAY_CAMERA)
      AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_meshlight_beauty), sg->out.RGB);
}
