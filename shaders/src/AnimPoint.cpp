#include <ai.h>
#include "MayaUtils.h"

namespace
{

// a modified version of AiArrayInterpolateFlt, from arnold core
AtPoint AiArrayInterpolatePnt(const AtArray *array, AtFloat time, AtInt idx)
{
   AtPoint point;

   //AiAssert(array->type == AI_TYPE_POINT);

   if (array->nkeys == 1 || time <= 0)
      return ((AtPoint *)array->data)[idx];

   if (time >= 1)
      return ((AtPoint *)array->data)[(array->nkeys - 1) * array->nelements + idx];

   if (array->nkeys == 2)
   {
      point = AiV3Lerp(time,
         ((AtPoint *)array->data)[idx],
         ((AtPoint *)array->data)[array->nelements + idx]);
   }
   else
   {
      AtFloat t_nkm1 = time * (array->nkeys-1);
      AtInt   lokey  = FLOOR(t_nkm1);
      AtInt   hikey  = lokey + 1;
      AtFloat t      = t_nkm1 - lokey;

      point = AiV3Lerp(t,
         ((AtPoint *)array->data)[lokey * array->nelements + idx],
         ((AtPoint *)array->data)[hikey * array->nelements + idx] );
   }
   return point;
}

enum AnimPointParams
{
   p_values
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimPointMtd);

node_parameters
{
   AiParameterARRAY("values", AiArray(0, 0, AI_TYPE_POINT));

   AiMetaDataSetBool(mds, NULL, "maya.hide", TRUE);
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
