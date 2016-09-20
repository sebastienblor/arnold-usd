#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyAbs2FloatsOverrideMtd)

node_parameters
{
   AiParameterPnt2("original", 0.0f, 0.0f);
   AiParameterPnt2("value", 0.0f, 0.0f);
   AiParameterBool("enabled", false);
   AiMetaDataSetStr(mds, NULL, "maya.name", "applyAbs2FloatsOverride");
}

IMPLEMENT_APPLY_ABS_OVERRIDE_SHADER(PNT2, AiShaderEvalParamPnt2)
