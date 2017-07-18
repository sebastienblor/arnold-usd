#include <ai.h>
#include "MayaUtils.h"
#include <vector>
#include <assert.h>

namespace
{

enum AnimMatrixParams
{
   p_values
};

struct animMatrixData{
   float shutter_start;
   float inv_shutter_length;
   std::vector<AtMatrix> thread_mtx;
   bool constant_mtx; // most of the time this matrix array is constant....
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

   if (sg->tid >= data->thread_mtx.size())
   {
      // Should never happen, unless the amount of threads ends up changing while render is in progress
      AiMsgError("[mtoa] invalid thread id in MtoaAnimShader");
      assert(0);
      sg->out.pMTX() = &data->thread_mtx[0];
      return;
   }

   AtMatrix &mtx = data->thread_mtx[sg->tid];
   mtx = (data->constant_mtx) ? AiArrayGetMtx(AiShaderEvalParamArray(p_values), 0) : 
               AiArrayInterpolateMtx(AiShaderEvalParamArray(p_values), (sg->time - data->shutter_start) * data->inv_shutter_length, 0);
   
   sg->out.pMTX() = &mtx;
}

node_initialize
{
   animMatrixData *data = new animMatrixData();
   AiNodeSetLocalData(node, data);
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
   data->thread_mtx.resize(AiMax(1, AiNodeGetInt(AiUniverseGetOptions(), "threads")));

   // most of the time the matrices will be constant here.
   // So I'd rather check if there is any animation to avoid
   // calling AiArrayInterpolateMtx uselessly
   if(AiNodeIsLinked(node, "values"))
      data->constant_mtx = false;
   else
   {
      AtArray *mtxArray = AiNodeGetArray(node, "values");
      unsigned int nvalues = AiArrayGetNumKeys(mtxArray) * AiArrayGetNumElements(mtxArray);

      AtMatrix firstMtx = AiArrayGetMtx(mtxArray, 0);
      data->constant_mtx = true;

      // in theory we have a single element and multiple keys in this array
      // but who knows....
      for (unsigned int i = 1; i < nvalues; ++i)
      {
         AtMatrix mtx = AiArrayGetMtx(mtxArray, i);
         if (!(mtx == firstMtx))
         {
            data->constant_mtx = false;
            break;
         }
      }
   }
}

node_finish
{
   delete (animMatrixData*)AiNodeGetLocalData(node);
}
