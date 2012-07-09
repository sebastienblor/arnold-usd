#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(SolidColorMtd);

enum SolidColorParams{
   p_color = 0
};

node_parameters
{
   AiParameterRGB("color", 0.f, 0.f, 0.f);
   
   AiMetaDataSetBool(mds, "color", "always_linear", true); // no gamma correction
   AiMetaDataSetStr(mds, NULL, "maya.name", "aiSolidColor");
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
   sg->out.RGBA.a = 1.f;
   sg->out.RGB = AiShaderEvalParamRGB(p_color);   
}
