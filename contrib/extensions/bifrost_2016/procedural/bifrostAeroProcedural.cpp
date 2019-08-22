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

   float shutter_length;
   float inv_fps;

};



AI_VOLUME_NODE_EXPORT_METHODS(BifrostAeroVolumeMtd);

node_parameters
{
   AiMetaDataSetBool(nentry, AtString(""), AtString("parallel_init"), true);
}

volume_cleanup
 {
   if (data == NULL) return false;
     
   BfVolumeUserData *volData = (BfVolumeUserData*)data->private_info; 
   if (volData)
   {
      if (volData->object_ref) delete volData->object_ref;
      delete volData;
   }
   data->private_info = NULL;

   return true;
}
volume_update
{
   VolumeCleanup(node, data);
   return VolumeCreate(node, data);
}
// FIXME Arnold5 what should we return here? This function wasn't defined before
volume_gradient
{
   *gradient = AI_V3_ZERO;
   return false;
}

volume_create
{
   BfVolumeUserData *volData = new BfVolumeUserData;
   volData->object_ref = 0;
   memset(volData->smoke_samplers, 0, AI_MAX_THREADS * sizeof(void*));
   memset(volData->density_samplers, 0, AI_MAX_THREADS * sizeof(void*));
   memset(volData->temperature_samplers, 0, AI_MAX_THREADS * sizeof(void*));
   memset(volData->velocity_samplers, 0, AI_MAX_THREADS * sizeof(void*));
   
   std::string object_name = AiNodeLookUpUserParameter(node, "object_name") ? AiNodeGetStr(node, "object_name").c_str() : "";
   std::string file_name = AiNodeLookUpUserParameter(node, "file_name") ? AiNodeGetStr(node, "file_name").c_str() : "";
   if (true || object_name != volData->object_name || file_name != volData->file) // in case we change the frame ?
   {
    // need to update objet_ref
      if (volData->object_ref) delete volData->object_ref;

      volData->object_ref = new BifrostObjectUserData(object_name, file_name);
      volData->object_name = object_name;
      volData->file = file_name;
   }


   volData->bbox.min = AiNodeGetVec(node, "min");
   volData->bbox.max = AiNodeGetVec(node, "max");
   
   volData->smoke_channel = volData->object_ref->findVoxelChannel("smoke");
   volData->density_channel = volData->object_ref->findVoxelChannel("density");
   volData->temperature_channel = volData->object_ref->findVoxelChannel("temperature");
   volData->velocity_channel = volData->object_ref->findVoxelChannel("velocity");


   cleanVoxelSamplers(volData->smoke_samplers);
   cleanVoxelSamplers(volData->density_samplers);
   cleanVoxelSamplers(volData->temperature_samplers);
   cleanVoxelSamplers(volData->velocity_samplers);
   
   data->bbox = volData->bbox;

   AtNode *shader = (AtNode*)AiNodeGetPtr(node, "shader");
   

   if(shader)
   {
       volData->step_size = AiNodeGetFlt(shader, "aiStepSize");
       volData->max_steps = AiNodeGetInt(shader, "aiMaxSteps");
       volData->shadowing = AiNodeGetBool(shader, "aiShadowing");
       volData->shadowing_step_size = AiNodeGetFlt(shader, "aiShadowingStepSize");
       volData->shadowing_max_steps = AiNodeGetInt(shader, "aiShadowingMaxSteps");


   } else
   {
       volData->step_size = 0.1f;
       volData->max_steps = 1000;
       volData->shadowing = true;
       volData->shadowing_step_size = 0.1f;
       volData->shadowing_max_steps = 1000;
   }

   data->auto_step_size = volData->step_size;
   data->private_info = volData;
   volData->inv_fps = AiNodeLookUpUserParameter(node, "inv_fps") ? AiNodeGetFlt(node, "inv_fps") : 1.f; 


   volData->shutter_length = AiNodeLookUpUserParameter(node, "shutter_length") ? AiNodeGetFlt(node, "shutter_length") : 1.f; 

   //AiAddMemUsage((AtInt64)grid->memoryUsage(), "BifrostAero volume plugin data");


   return true;
}

volume_sample
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
      value->FLT() =thread_sampler->sample<float>(pos);

      
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
     value->FLT() =thread_sampler->sample<float>(pos);
     
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
     value->FLT() =thread_sampler->sample<float>(pos);
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

     // velocity is expressed in seconds, need to convert to frames
     value->RGB() = AtRGB(col[0], col[1], col[2]);

     value->RGB() *= volData->inv_fps * volData->shutter_length;
     
     return true;
   }
   return false;
}

volume_ray_extents
{
   //if (!data->private_info) return;
   BfVolumeUserData *volData = (BfVolumeUserData*)data->private_info;
   if (volData == 0) return;


   float tmin, tmax;

   float step = volData->step_size;
   tmin = t0;
   tmax = t1;
   int i = 0;
   for (float t = tmin; t < tmax && i < volData->max_steps; t+= step, i++)
   {
      AiVolumeAddIntersection(info, t, AiMin(t+step, tmax));
   }
}


} // namespace


// DSO hook
#ifdef __cplusplus
extern "C"
{
#endif


node_loader
{
   if (i>0)
      return false;

   node->methods      = BifrostAeroVolumeMtd;
   node->output_type  = AI_TYPE_NONE;
   node->name         = "bifrostAero";
   node->node_type    = AI_NODE_SHAPE_VOLUME;
   strcpy(node->version, AI_VERSION);

   return true;
}


#ifdef __cplusplus
}
#endif
