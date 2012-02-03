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
   AiParameterARRAY("values", AiArray(0, 0, AI_TYPE_POINT));

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

shader_evaluate
{
   sg->out.PNT = AiArrayInterpolatePnt(AiShaderEvalParamArray(p_values), sg->time, 0);
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
