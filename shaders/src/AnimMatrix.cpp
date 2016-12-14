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
   AiParameterArray("values", AiArray(0, 0, AI_TYPE_MATRIX));

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

shader_evaluate
{
   //AtMatrix *mtx = (AtMatrix*)AiShaderGlobalsQuickAlloc(sg, sizeof(AtMatrix));
   
   AtMatrix mtx = AiArrayInterpolateMtx(AiShaderEvalParamArray(p_values), sg->time, 0);
   sg->out.pMTX() = new AtMatrix(mtx);
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
