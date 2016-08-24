#include <ai.h>
#include "template.h"

AI_SHADER_NODE_EXPORT_METHODS(applyRel2FloatsOverrideMtd)

node_parameters
{
   AiParameterPnt2("original", 0.0f, 0.0f);
   AiParameterPnt2("multiply", 0.0f, 0.0f);
   AiParameterPnt2("offset", 0.0f, 0.0f);
   AiParameterBool("enabled", false);
   AiMetaDataSetStr(mds, NULL, "maya.name", "applyRel2FloatsOverride");
}

IMPLEMENT_APPLY_REL_OVERRIDE_SHADER(PNT2, AiShaderEvalParamPnt2)
