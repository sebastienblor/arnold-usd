#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum AnimMatrixParams
{
   p_values
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimMatrixMtd);

node_parameters
{
   AiParameterARRAY("values", AiArray(0, 0, AI_TYPE_MATRIX));

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

shader_evaluate
{
   sg->out.pMTX = (AtMatrix*)AiShaderGlobalsQuickAlloc(sg, sizeof(AtMatrix));
   AiArrayInterpolateMtx(AiShaderEvalParamArray(p_values), sg->time, 0, *sg->out.pMTX);
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
