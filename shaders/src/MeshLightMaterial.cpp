#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MeshLightMaterialMtd);

enum MeshLightMaterialParams{
   p_color = 0,
   p_color_multiplier
};

node_parameters
{
   AiParameterRGB("color", 0.f, 0.f, 0.f);
   AiParameterRGB("color_multiplier", 0.0f, 0.0f, 0.0f);
   
   AiMetaDataSetBool(mds, "color", "always_linear", true); // no gamma correction
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiMeshLightMaterial");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D1B);
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
   if (sg->Rt & (AI_RAY_DIFFUSE | AI_RAY_GLOSSY | AI_RAY_SHADOW))
   {
      sg->out.RGBA = AI_RGBA_BLACK;   
      return;
   }
   
   sg->out.RGB = AiShaderEvalParamRGB(p_color) * AiShaderEvalParamRGB(p_color_multiplier);
   sg->out.RGBA.a = 1.f;
}
