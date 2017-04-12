#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyRel3FloatsOverrideMtd)

node_parameters
{
   AiParameterRGB("original", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("multiply", 0.0f, 0.0f, 0.0f);
   AiParameterRGB("offset", 0.0f, 0.0f, 0.0f);
   AiParameterBool("enabled", false);
   AiMetaDataSetStr(nentry, NULL, "maya.name", "applyRel3FloatsOverride");
}

IMPLEMENT_APPLY_REL_OVERRIDE_SHADER(RGB, AiShaderEvalParamRGB)
