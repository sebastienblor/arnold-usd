#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyAbs3FloatsOverrideMtd)

node_parameters
{
   AiParameterRGB("original", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("value", 0.0f, 0.0f, 0.0f);
   AiParameterBool("enabled", false);
   AiMetaDataSetStr(nentry, NULL, "maya.name", "applyAbs3FloatsOverride");
}

IMPLEMENT_APPLY_ABS_OVERRIDE_SHADER(RGB, AiShaderEvalParamRGB)
