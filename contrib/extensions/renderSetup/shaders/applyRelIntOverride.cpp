#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyRelIntOverrideMtd)

node_parameters
{
   AiParameterFlt("original", 0);
   AiParameterFlt("multiply", 0);
   AiParameterFlt("offset", 0);
   AiParameterBool("enabled", false);
   AiMetaDataSetStr(nentry, NULL, "maya.name", "applyRelIntOverride");
}

IMPLEMENT_APPLY_REL_OVERRIDE_SHADER(INT, AiShaderEvalParamInt)
