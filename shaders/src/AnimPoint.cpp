#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum AnimPointParams
{
   p_values
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimPointMtd);

node_parameters
{
   AiParameterArray("values", AiArray(0, 0, AI_TYPE_VECTOR));

   AiMetaDataSetStr(nentry, NULL, "_synonym", "anim_point");
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiAnimPoint");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

shader_evaluate
{
   sg->out.VEC() = AiArrayInterpolateVec(AiShaderEvalParamArray(p_values), sg->time, 0);
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
