#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyRelIntOverrideMtd)

node_parameters
{
   AiParameterFlt("original", 0);
   AiParameterFlt("multiply", 0);
   AiParameterFlt("offset", 0);
   AiParameterBool("enabled", false);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

IMPLEMENT_APPLY_REL_OVERRIDE_SHADER(INT, AiShaderEvalParamInt)
