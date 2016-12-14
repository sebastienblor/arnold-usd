#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaFlatClosureMtd);

namespace
{

enum MayaFlatClosureShaderParams
{
   p_outColor
};

};

node_parameters
{
   AiParameterRGB("color", 1.0f, 1.0f, 1.0f);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "MayaFlatShader");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52535983);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   AtRGB color = AiShaderEvalParamRGB(p_outColor);
   AtClosureList closures;
   closures.add(AiClosureEmission(sg, color));
   sg->out.CLOSURE() = closures;
}
