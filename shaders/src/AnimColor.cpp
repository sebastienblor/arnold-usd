#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum AnimColorParams
{
   p_values
};

struct animColorData{
   float shutter_start;
   float inv_shutter_length;
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimColorMtd);

node_parameters
{
   AiParameterArray("values", AiArray(0, 0, AI_TYPE_RGBA));

   AiMetaDataSetStr(nentry, NULL, "_synonym", "anim_color");
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiAnimColor");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

shader_evaluate
{
   animColorData *data = (animColorData*)AiNodeGetLocalData(node);
   sg->out.RGBA() = AiArrayInterpolateRGBA(AiShaderEvalParamArray(p_values), (sg->time - data->shutter_start) * data->inv_shutter_length, 0);
}

node_initialize
{
   AiNodeSetLocalData(node, new animColorData());   
}

node_update
{
   animColorData *data = (animColorData*)AiNodeGetLocalData(node);
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
   delete (animColorData*)AiNodeGetLocalData(node);
}
