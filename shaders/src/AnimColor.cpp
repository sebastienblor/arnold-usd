#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum AnimColorParams
{
   p_values
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimColorMtd);

node_parameters
{
   AiParameterARRAY("values", AiArray(0, 0, AI_TYPE_RGBA));

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

shader_evaluate
{
   sg->out.RGBA = AiArrayInterpolateRGBA(AiShaderEvalParamArray(p_values), sg->time, 0);
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
