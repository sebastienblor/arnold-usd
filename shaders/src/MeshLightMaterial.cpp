#include <ai.h>

namespace MSTR
{
   static const AtString mesh_light_beauty("mesh_light_beauty");
}

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
   
   AiMetaDataSetBool(mds, "color", "always_linear", true); // no gamma correction
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiMeshLightMaterial");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D1B);
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
   AiMetaDataSetStr(mds, NULL, "maya.classification", "shader/surface");
}

node_initialize
{

}

node_update
{
   AiAOVRegister("mesh_light_beauty", AI_TYPE_RGB, AI_AOV_BLEND_NONE);
}

node_finish
{
   
}

shader_evaluate
{
   if (sg->Rt & (AI_RAY_DIFFUSE | AI_RAY_GLOSSY | AI_RAY_SHADOW))
   {
      sg->out.RGBA() = AI_RGBA_ZERO;   
      return;
   }
   
   if (AiV3Dot(sg->Ng, sg->Ngf) < 0)
   {
      sg->out.RGBA() = AI_RGBA_ZERO;
      return;
   }

   sg->out.RGB() = AiShaderEvalParamRGB(p_color) * AiShaderEvalParamRGB(p_color_multiplier);
   sg->out.RGBA().a = 1.f;

   if (sg->Rt & AI_RAY_CAMERA)
      AiAOVSetRGB(sg, MSTR::mesh_light_beauty, sg->out.RGB()); // we should check if this aov exists, could be done in node_update
}
