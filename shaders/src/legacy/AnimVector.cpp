#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum AnimVectorParams
{
   p_values
};

struct animVectorData{
   float shutter_start;
   float inv_shutter_length;
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimVectorMtd);

node_parameters
{
   AiParameterArray("values", AiArray(0, 0, AI_TYPE_VECTOR));
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
}

shader_evaluate
{
   animVectorData *data = (animVectorData*)AiNodeGetLocalData(node);
   sg->out.VEC() = AiArrayInterpolateVec(AiShaderEvalParamArray(p_values), (sg->time - data->shutter_start) * data->inv_shutter_length, 0);
}

node_initialize
{
   AiNodeSetLocalData(node, new animVectorData());   
}

node_update
{
   animVectorData *data = (animVectorData*)AiNodeGetLocalData(node);
   AtNode *camera = AiUniverseGetCamera();
   if (camera)
   {  
      data->shutter_start  = AiNodeGetFlt(camera, "shutter_start");
      float shutter_end  = AiNodeGetFlt(camera, "shutter_end");
      data->inv_shutter_length = 1.f / (AiMax(AI_EPSILON, shutter_end - data->shutter_start));
   } else
   {
      data->shutter_start = 0.f;
      data->inv_shutter_length = 1.f;
   }
}

node_finish
{
   delete (animVectorData*)AiNodeGetLocalData(node);
}
