#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum AnimFloatParams
{
   p_values
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimFloatMtd);

node_parameters
{
   AiParameterArray("values", AiArray(0, 0, AI_TYPE_FLOAT));

   AiMetaDataSetStr(nentry, NULL, "_synonym", "anim_float");
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiAnimFloat");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

shader_evaluate
{
   sg->out.FLT() = AiArrayInterpolateFlt(AiShaderEvalParamArray(p_values), sg->time, 0);
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
