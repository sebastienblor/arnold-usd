#include <ai.h>
#include "MayaUtils.h"

namespace
{

// a modified version of AiArrayInterpolateFlt, from arnold core
AtVector AiArrayInterpolateVec(const AtArray *array, float time, int idx)
{
   AtVector vector;

   //AiAssert(array->type == AI_TYPE_VECTOR);

   if (array->nkeys == 1 || time <= 0)
      return ((AtVector *)array->data)[idx];

   if (time >= 1)
      return ((AtVector *)array->data)[(array->nkeys - 1) * array->nelements + idx];

   if (array->nkeys == 2)
   {
      vector = AiV3Lerp(time,
         ((AtVector *)array->data)[idx],
         ((AtVector *)array->data)[array->nelements + idx]);
   }
   else
   {
      float t_nkm1 = time * (array->nkeys-1);
      int   lokey  = FLOOR(t_nkm1);
      int   hikey  = lokey + 1;
      float t      = t_nkm1 - lokey;

      //AiAssert(lokey >= 0 && lokey < array->nkeys);
      //AiAssert(hikey >= 0 && hikey < array->nkeys);

      vector = AiV3Lerp(t,
         ((AtVector *)array->data)[lokey * array->nelements + idx],
         ((AtVector *)array->data)[hikey * array->nelements + idx] );
   }
   return vector;
}


enum AnimVectorParams
{
   p_values
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimVectorMtd);

node_parameters
{
   AiParameterARRAY("values", AiArray(0, 0, AI_TYPE_VECTOR));

   AiMetaDataSetBool(mds, NULL, "maya.hide", TRUE);
}

shader_evaluate
{
   sg->out.VEC = AiArrayInterpolateVec(AiShaderEvalParamArray(p_values), sg->time, 0);
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
