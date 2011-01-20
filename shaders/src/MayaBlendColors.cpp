
#include <ai_metadata.h>
#include <ai_shaderglobals.h>
#include <ai_shader_parameval.h>
#include <ai_shaders.h>
#include <ai_nodes.h>

namespace
{

enum MayaBlendColorsParams
{
   p_blender,
   p_color1,
   p_color2
};

};

AI_SHADER_NODE_EXPORT_METHODS(MayaBlendColorsMtd);

node_parameters
{
   AiParameterFLT("blender", 0.5f);
   AiParameterRGB("color1", 1, 0, 0);
   AiParameterRGB("color2", 0, 0, 1);

   AiMetaDataSetStr(mds, NULL, "maya.counterpart", "blendColors");
   AiMetaDataSetInt(mds, NULL, "maya.counterpart_id", 0x52424c32);
}

shader_evaluate
{
   AtFloat blend  = AiShaderEvalParamFlt(p_blender);
   AtRGB   color1 = AiShaderEvalParamRGB(p_color1);
   AtRGB   color2 = AiShaderEvalParamRGB(p_color2);
   
   AiColorLerp(sg->out.RGB, blend, color2, color1);
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
