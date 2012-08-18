#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidMtd);

node_parameters
{
   AiParameterRGB("color", 1.f, 1.f, 1.f);
   
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiMayaFluid");
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
   AiMetaDataSetBool(mds, NULL, "maya.swatch", false);
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x00115D1C);
}

enum MayaFluidParams{
   p_color=0
};

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
   if (sg->Rt & AI_RAY_SHADOW)
   {
      sg->out_opacity = AI_RGB_WHITE;
      return;
   }
   sg->out.RGB = AiShaderEvalParamRGB(p_color);
}
