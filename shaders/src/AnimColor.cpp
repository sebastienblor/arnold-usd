#include <ai.h>
#include "MayaUtils.h"

namespace
{

// a modified version of AiArrayInterpolateFlt, from arnold core
AtRGBA AiArrayInterpolateRGBA(const AtArray *array, AtFloat time, AtInt idx)
{
   AtRGBA vector;

   //AiAssert(array->type == AI_TYPE_VECTOR);

   if (array->nkeys == 1 || time <= 0)
      return ((AtRGBA *)array->data)[idx];

   if (time >= 1)
      return ((AtRGBA *)array->data)[(array->nkeys - 1) * array->nelements + idx];

   if (array->nkeys == 2)
   {
      vector = AiRGBALerp(time,
         ((AtRGBA *)array->data)[idx],
         ((AtRGBA *)array->data)[array->nelements + idx]);
   }
   else
   {
      AtFloat t_nkm1 = time * (array->nkeys-1);
      AtInt   lokey  = FLOOR(t_nkm1);
      AtInt   hikey  = lokey + 1;
      AtFloat t      = t_nkm1 - lokey;

      //AiAssert(lokey >= 0 && lokey < array->nkeys);
      //AiAssert(hikey >= 0 && hikey < array->nkeys);

      vector = AiRGBALerp(t,
         ((AtRGBA *)array->data)[lokey * array->nelements + idx],
         ((AtRGBA *)array->data)[hikey * array->nelements + idx] );
   }
   return vector;
}


enum AnimColorParams
{
   p_values
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimColorMtd);

node_parameters
{
   AiParameterARRAY("values", AiArray(0, 0, AI_TYPE_RGBA));

   AiMetaDataSetBool(mds, NULL, "maya.hide", TRUE);
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
