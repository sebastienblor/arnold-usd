#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyAbsFloatOverrideMtd)

node_parameters
{
   AiParameterFlt("original", 0.0f);
   AiParameterFlt("value", 0.0f);
   AiParameterBool("enabled", false);
   AiMetaDataSetStr(mds, NULL, "maya.name", "applyAbsFloatOverride");
}

IMPLEMENT_APPLY_ABS_OVERRIDE_SHADER(FLT, AiShaderEvalParamFlt)
