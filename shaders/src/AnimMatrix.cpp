#include <ai.h>
#include "MayaUtils.h"

namespace
{

enum AnimMatrixParams
{
   p_values
};

struct animMatrixData{
   float shutter_start;
   float inv_shutter_length;
};

};

AI_SHADER_NODE_EXPORT_METHODS(AnimMatrixMtd);

node_parameters
{
   AiParameterArray("values", AiArray(0, 0, AI_TYPE_MATRIX));

   AiMetaDataSetStr(nentry, NULL, "_synonym", "anim_matrix");
   AiMetaDataSetStr(nentry, NULL, "maya.name", "aiAnimMatrix");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
}

shader_evaluate
{
   animMatrixData *data = (animMatrixData*)AiNodeGetLocalData(node);
  
   AtMatrix mtx = AiArrayInterpolateMtx(AiShaderEvalParamArray(p_values), (sg->time - data->shutter_start) * data->inv_shutter_length, 0);
   sg->out.pMTX() = new AtMatrix(mtx);
}

node_initialize
{
   AiNodeSetLocalData(node, new animMatrixData());   
}

node_update
{
   animMatrixData *data = (animMatrixData*)AiNodeGetLocalData(node);
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
   delete (animMatrixData*)AiNodeGetLocalData(node);
}
