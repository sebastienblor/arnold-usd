
#include <ai_metadata.h>
#include <ai_nodes.h>
#include <ai_shaderglobals.h>
#include <ai_shaders.h>
#include <ai_shader_parameval.h>

namespace
{

enum MayaSetRangeParams
{
   p_value,
   p_min,
   p_max,
   p_oldMin,
   p_oldMax
};

};

AI_SHADER_NODE_EXPORT_METHODS(MayaSetRangeMtd);

node_parameters
{
   AiParameterVEC("value", 0, 0, 0);
   AiParameterVEC("min", 0, 0, 0);
   AiParameterVEC("max", 0, 0, 0);
   AiParameterVEC("oldMin", 0, 0, 0);
   AiParameterVEC("oldMax", 0, 0, 0);

   AiMetaDataSetStr(mds, NULL, "maya::counterpart", "setRange");
   AiMetaDataSetInt(mds, NULL, "maya::counterpart_id", 0x52524e47);
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
   AtVector input    = AiShaderEvalParamVec(p_value);
   AtVector min      = AiShaderEvalParamVec(p_min);
   AtVector max      = AiShaderEvalParamVec(p_max);
   AtVector oldMin   = AiShaderEvalParamVec(p_oldMin);
   AtVector oldMax   = AiShaderEvalParamVec(p_oldMax);

   sg->out.VEC.x = (oldMax.x == oldMin.x) ? 0.0f : ((input.x - oldMin.x) / (oldMax.x - oldMin.x)) * (max.x - min.x) + min.x;
   sg->out.VEC.y = (oldMax.y == oldMin.y) ? 0.0f : ((input.y - oldMin.y) / (oldMax.y - oldMin.y)) * (max.y - min.y) + min.y;
   sg->out.VEC.z = (oldMax.z == oldMin.z) ? 0.0f : ((input.z - oldMin.z) / (oldMax.z - oldMin.z)) * (max.z - min.z) + min.z;
}
