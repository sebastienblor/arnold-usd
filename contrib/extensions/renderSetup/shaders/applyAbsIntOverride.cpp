#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyAbsIntOverrideMtd)

node_parameters
{
   AiParameterInt("original", 0);
   AiParameterInt("value", 0);
   AiParameterBool("enabled", false);
   AiMetaDataSetStr(nentry, NULL, "maya.name", "applyAbsIntOverride");
}

IMPLEMENT_APPLY_ABS_OVERRIDE_SHADER(INT, AiShaderEvalParamInt)
