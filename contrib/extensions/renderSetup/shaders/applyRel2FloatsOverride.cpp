#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyRel2FloatsOverrideMtd)

node_parameters
{
   AiParameterVec2("original", 0.0f, 0.0f);
   AiParameterVec2("multiply", 0.0f, 0.0f);
   AiParameterVec2("offset", 0.0f, 0.0f);
   AiParameterBool("enabled", false);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

IMPLEMENT_APPLY_REL_OVERRIDE_SHADER(VEC2, AiShaderEvalParamVec2)
