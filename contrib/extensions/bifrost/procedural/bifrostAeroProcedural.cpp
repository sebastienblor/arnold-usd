#include <ai.h>
#include <algorithm>
#include <iostream>


#include "../common/bifrostObjectUserData.h"
//#include "bifrostHelpers.h"


#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_pointchannel.h>

#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_tiledataaccessor.h>
#include <bifrostapi/bifrost_voxelsampler.h>
#include <bifrostapi/bifrost_voxelchannel.h>


namespace
{
void cleanVoxelSamplers(Bifrost::API::VoxelSampler **smoke_samplers)
{
   for (int i = 0; i < AI_MAX_THREADS; ++i)
   {
      if (smoke_samplers[i]) 
      {
         delete smoke_samplers[i];
         smoke_samplers[i] = 0;
      }
   }
}
  
struct BfVolumeUserData {
   BifrostObjectUserData *object_ref;
   Bifrost::API::VoxelSampler *smoke_samplers[AI_MAX_THREADS];
   Bifrost::API::VoxelSampler *density_samplers[AI_MAX_THREADS];
   Bifrost::API::VoxelSampler *temperature_samplers[AI_MAX_THREADS];
   Bifrost::API::VoxelSampler *velocity_samplers[AI_MAX_THREADS];

   Bifrost::API::VoxelChannel smoke_channel;
   Bifrost::API::VoxelChannel density_channel;
   Bifrost::API::VoxelChannel temperature_channel;
   Bifrost::API::VoxelChannel velocity_channel;
   std::string object_name;
   std::string file;
   AtBBox bbox;
   float step_size;
   int max_steps;

   bool shadowing;
   float shadowing_step_size;
   int shadowing_max_steps;

};


bool BifrostAeroVolumePluginInit(void** user_ptr)
{
   BfVolumeUserData *data = new BfVolumeUserData;
   data->object_ref = 0;
   memset(data->smoke_samplers, 0, AI_MAX_THREADS * sizeof(void*));
   memset(data->density_samplers, 0, AI_MAX_THREADS * sizeof(void*));
   memset(data->temperature_samplers, 0, AI_MAX_THREADS * sizeof(void*));
   memset(data->velocity_samplers, 0, AI_MAX_THREADS * sizeof(void*));
   
   *user_ptr = data;
   return true;
}

bool BifrostAeroVolumePluginCleanup(void* user_ptr)
{
   BfVolumeUserData *data = (BfVolumeUserData*)user_ptr; 
   if (data)
   {
      if (data->object_ref) delete data->object_ref;
      delete data;
   }
   return true;
}

bool BifrostAeroVolumePluginCreateVolume(void* user_ptr,
                                  const char* user_config,
                                  const AtNode* node,
                                  AtVolumeData* out_data)
{
   BfVolumeUserData *data = (BfVolumeUserData*)user_ptr;
   

   std::string object_name = AiNodeLookUpUserParameter(node, "object_name") ? AiNodeGetStr(node, "object_name") : "";
   std::string file_name = AiNodeLookUpUserParameter(node, "file_name") ? AiNodeGetStr(node, "file_name") : "";
   if (true || object_name != data->object_name || file_name != data->file) // in case we change the frame ?
   {
    // need to update objet_ref
      if (data->object_ref) delete data->object_ref;

      data->object_ref = new BifrostObjectUserData(object_name, file_name);
      data->object_name = object_name;
      data->file = file_name;
   }


   data->bbox.min = AiNodeGetPnt(node, "min");
   data->bbox.max = AiNodeGetPnt(node, "max");
   
   data->smoke_channel = data->object_ref->findVoxelChannel("smoke");
   data->density_channel = data->object_ref->findVoxelChannel("density");
   data->temperature_channel = data->object_ref->findVoxelChannel("temperature");
   data->velocity_channel = data->object_ref->findVoxelChannel("velocity");


   cleanVoxelSamplers(data->smoke_samplers);
   cleanVoxelSamplers(data->density_samplers);
   cleanVoxelSamplers(data->temperature_samplers);
   cleanVoxelSamplers(data->velocity_samplers);
   
   out_data->bbox = data->bbox;

   AtNode *shader = (AtNode*)AiNodeGetPtr(node, "shader");
   
   if(shader)
   {
       data->step_size = AiNodeGetFlt(shader, "aiStepSize");
       data->max_steps = AiNodeGetInt(shader, "aiMaxSteps");
       data->shadowing = AiNodeGetBool(shader, "aiShadowing");
       data->shadowing_step_size = AiNodeGetFlt(shader, "aiShadowingStepSize");
       data->shadowing_max_steps = AiNodeGetInt(shader, "aiShadowingMaxSteps");


   } else
   {
       data->step_size = 0.1f;
       data->max_steps = 1000;
       data->shadowing = true;
       data->shadowing_step_size = 0.1f;
       data->shadowing_max_steps = 1000;
   }

   out_data->auto_step_size = data->step_size;
   out_data->private_info = user_ptr;
   //AiAddMemUsage((AtInt64)grid->memoryUsage(), "BifrostAero volume plugin data");


   return true;
}

bool BifrostAeroVolumePluginCleanupVolume(void* user_ptr, AtVolumeData* data, const AtNode* node)
{
   if (!user_ptr) return false;

     
  // AiAddMemUsage(-((AtInt64)grid->memoryUsage()), "BifrostAero volume plugin data");
  
   data->private_info = NULL;
   return true;
}

bool BifrostAeroVolumePluginSample(void* user_ptr,
                            const AtVolumeData* data,
                            const AtString channel,
                            const AtShaderGlobals* sg,
                            int interp,
                            AtParamValue *value,
                            AtByte *type)
{

   if (!data->private_info) return false;
   BfVolumeUserData *volData = (BfVolumeUserData*)data->private_info;

   amino::Math::vec3f pos;
   pos[0] = sg->P.x;
   pos[1] = sg->P.y;
   pos[2] = sg->P.z;

   if (!strcmp(channel, "smoke"))
   {
      Bifrost::API::VoxelSampler *thread_sampler = volData->smoke_samplers[sg->tid];
      if (thread_sampler == 0)
      {
         thread_sampler = volData->smoke_samplers[sg->tid] = new Bifrost::API::VoxelSampler(volData->smoke_channel.createSampler(
            Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace));
      }
      *type = AI_TYPE_FLOAT;
      value->FLT =thread_sampler->sample<float>(pos);

      
      return true;
   }
   if (!strcmp(channel, "density"))
   {
     Bifrost::API::VoxelSampler *thread_sampler = volData->density_samplers[sg->tid];
     if (thread_sampler == 0)
     {
        thread_sampler = volData->density_samplers[sg->tid] = new Bifrost::API::VoxelSampler(volData->density_channel.createSampler(
           Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace));
     }
     *type = AI_TYPE_FLOAT;
     value->FLT =thread_sampler->sample<float>(pos);
     
     return true;
   } 
   if (!strcmp(channel, "temperature"))
   {
     Bifrost::API::VoxelSampler *thread_sampler = volData->temperature_samplers[sg->tid];
     if (thread_sampler == 0)
     {
        thread_sampler = volData->temperature_samplers[sg->tid] = new Bifrost::API::VoxelSampler(volData->temperature_channel.createSampler(
           Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace));
     }
     *type = AI_TYPE_FLOAT;
     value->FLT =thread_sampler->sample<float>(pos);
     return true;
   }
   if (!strcmp(channel, "velocity"))
   {
     Bifrost::API::VoxelSampler *thread_sampler = volData->velocity_samplers[sg->tid];
     if (thread_sampler == 0)
     {
        thread_sampler = volData->velocity_samplers[sg->tid] = new Bifrost::API::VoxelSampler(volData->velocity_channel.createSampler(
           Bifrost::API::VoxelSamplerQBSplineType, Bifrost::API::WorldSpace));
     }
     *type = AI_TYPE_RGB;
     amino::Math::vec3f col = thread_sampler->sample<amino::Math::vec3f>(pos);
     value->RGB.r = col[0]; 
     value->RGB.g = col[1]; 
     value->RGB.b = col[2]; 
     return true;
   }
   return false;
}

void BifrostAeroVolumePluginRayExtents(void* user_ptr,
                                const AtVolumeData* data,
                                const AtVolumeIntersectionInfo* info,
                                AtByte tid,
                                float time,
                                const AtPoint* origin,
                                const AtVector* direction,
                                float t0,
                                float t1)
{

   //if (!data->private_info) return;
   BfVolumeUserData *volData = (BfVolumeUserData*)user_ptr;
   if (volData == 0) return;


   float tmin, tmax;

   float step = volData->step_size;
   tmin = t0;
   tmax = t1;
   int i = 0;
   for (float t = tmin; t < tmax && i < volData->max_steps; t+= step, i++)
   {
      AiVolumeAddIntersection(info, t, MIN(t+step, tmax));
   }
}


} // namespace


// DSO hook
#ifdef __cplusplus
extern "C"
{
#endif

AI_EXPORT_LIB bool VolumePluginLoader(AtVolumePluginVtable* vtable)
{
   vtable->Init           = BifrostAeroVolumePluginInit;
   vtable->Cleanup        = BifrostAeroVolumePluginCleanup;
   vtable->CreateVolume   = BifrostAeroVolumePluginCreateVolume;
   vtable->CleanupVolume  = BifrostAeroVolumePluginCleanupVolume;
   vtable->Sample         = BifrostAeroVolumePluginSample;
   vtable->RayExtents     = BifrostAeroVolumePluginRayExtents;
   strcpy(vtable->version, AI_VERSION);
   return true;
}

#ifdef __cplusplus
}
#endif
