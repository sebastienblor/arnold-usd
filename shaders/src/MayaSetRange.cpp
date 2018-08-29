#include <ai.h>

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
   AiParameterVec("value", 0, 0, 0);
   AiParameterVec("min", 0, 0, 0);
   AiParameterVec("max", 0, 0, 0);
   AiParameterVec("oldMin", 0, 0, 0);
   AiParameterVec("oldMax", 0, 0, 0);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "setRange");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52524e47);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
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

   sg->out.VEC().x = (oldMax.x == oldMin.x) ? 0.0f : ((input.x - oldMin.x) / (oldMax.x - oldMin.x)) * (max.x - min.x) + min.x;
   sg->out.VEC().y = (oldMax.y == oldMin.y) ? 0.0f : ((input.y - oldMin.y) / (oldMax.y - oldMin.y)) * (max.y - min.y) + min.y;
   sg->out.VEC().z = (oldMax.z == oldMin.z) ? 0.0f : ((input.z - oldMin.z) / (oldMax.z - oldMin.z)) * (max.z - min.z) + min.z;
}
