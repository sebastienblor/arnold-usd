#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyRelFloatOverrideMtd)

node_parameters
{
   AiParameterFlt("original", 0.0f);
   AiParameterFlt("multiply", 0.0f);
   AiParameterFlt("offset", 0.0f);
   AiParameterBool("enabled", false);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

IMPLEMENT_APPLY_REL_OVERRIDE_SHADER(FLT, AiShaderEvalParamFlt)
