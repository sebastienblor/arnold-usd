#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyAbsFloatOverrideMtd)

node_parameters
{
   AiParameterFlt("original", 0.0f);
   AiParameterFlt("value", 0.0f);
   AiParameterBool("enabled", false);
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
}

IMPLEMENT_APPLY_ABS_OVERRIDE_SHADER(FLT, AiShaderEvalParamFlt)
