#include <ai.h>
#include "MayaUtils.h"

namespace
{

// a modified version of AiArrayInterpolateMtx, from arnold core
void AiArrayInterpolateMtx(const AtArray *array, float time, int idx, AtMatrix result)
{
   AtMatrix* data = (AtMatrix*) array->data;

   if (array->nkeys == 1 || time <= 0)
   {
     AiM4Copy(result, data[0]);
     return;
   }

   if (time >= 1)
   {
     AiM4Copy(result, data[(array->nkeys - 1) * array->nelements + idx]);
     return;
   }

   if (array->nkeys == 2)
   {
     AiM4Lerp(result, time, data[idx], data[array->nelements + idx]);
     return;
   }
   else
   {
     float t_nkm1 = time * (array->nkeys-1);
     int   lokey  = FLOOR(t_nkm1);
     int   hikey  = lokey + 1;
     float t      = t_nkm1 - lokey;

     AiM4Lerp(result, t, data[lokey * array->nelements + idx], data[hikey* array->nelements + idx]);
     return;
   }
}

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
