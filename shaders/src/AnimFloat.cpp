#include <ai.h>
#include "MayaUtils.h"

namespace
{

// a modifed version of AiArrayInterpolateFlt, from arnold core
float AiArrayInterpolateFlt(const AtArray *array, float time, AtInt idx)
{
   //AiAssert(array->type == AI_TYPE_FLOAT);

   if (array->nkeys == 1 || time <= 0)
      return ((float *)array->data)[idx];

   if (time >= 1)
      return ((float *)array->data)[(array->nkeys - 1) * array->nelements + idx];

   if (array->nkeys == 2)
   {
      return LERP(time,
         ((float *)array->data)[idx],
         ((float *)array->data)[array->nelements + idx]);
   }
   else
   {
      float t_nkm1 = time * (array->nkeys-1);
      AtInt   lokey  = FLOOR(t_nkm1);
      AtInt   hikey  = lokey + 1;
      float t      = t_nkm1 - lokey;

      //AiAssert(lokey >= 0 && lokey < array->nkeys);
      //AiAssert(hikey >= 0 && hikey < array->nkeys);

      return LERP(t,
         ((float *)array->data)[lokey * array->nelements + idx],
         ((float *)array->data)[hikey * array->nelements + idx] );
   }
}

enum AnimFloatParams
{
   p_values
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimFloatMtd);

node_parameters
{
   AiParameterARRAY("values", AiArray(0, 0, AI_TYPE_FLOAT));

   AiMetaDataSetBool(mds, NULL, "maya.hide", TRUE);
}

shader_evaluate
{
   sg->out.FLT = AiArrayInterpolateFlt(AiShaderEvalParamArray(p_values), sg->time, 0);
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
