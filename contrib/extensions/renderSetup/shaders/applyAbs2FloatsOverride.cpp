#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyAbs2FloatsOverrideMtd)

node_parameters
{
   AiParameterVec2("original", 0.0f, 0.0f);
   AiParameterVec2("value", 0.0f, 0.0f);
   AiParameterBool("enabled", false);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
}

IMPLEMENT_APPLY_ABS_OVERRIDE_SHADER(VEC2, AiShaderEvalParamVec2)