#include "MayaFluidData.h"

const char* contentsMethodEnums[] = {"Grid", "Gradient", 0};

const char* contentsGradientTypeEnums[] = {"Constant", "X Gradient", "Y Gradient", "Z Gradient",
                                           "-X Gradient", "-Y Gradient", "-Z Gradient",
                                           "Center Gradient", 0};

void InitializeFluidShaderParameters(AtList* params, bool is3d)
{
	AiParameterInt("xres", 0);
   AiParameterInt("yres", 0);

   if (is3d)
   {
      AiParameterInt("zres", 0);
   
      AiParameterVec("min", 0.f, 0.f, 0.f);
      AiParameterVec("max", 0.f, 0.f, 0.f);
   }   
   
   AiParameterEnum("density_method", CSM_GRADIENT, contentsMethodEnums);
   AiParameterEnum("density_gradient", CG_CONSTANT, contentsGradientTypeEnums);
   AiParameterArray("density", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));

   AiParameterEnum("fuel_method", CSM_GRADIENT, contentsMethodEnums);
   AiParameterEnum("fuel_gradient", CG_CONSTANT, contentsGradientTypeEnums);
   AiParameterArray("fuel", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));

   AiParameterEnum("temperature_method", CSM_GRADIENT, contentsMethodEnums);
   AiParameterEnum("temperature_gradient", CG_CONSTANT, contentsGradientTypeEnums);
   AiParameterArray("temperature", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));

   AiParameterArray("pressure", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));

   AiParameterEnum("velocity_method", CSM_GRADIENT, contentsMethodEnums);
   AiParameterEnum("velocity_gradient", CG_CONSTANT, contentsGradientTypeEnums);
   AiParameterArray("velocity", AiArrayAllocate(0, 1, AI_TYPE_VECTOR));
   
   AiParameterArray("colors", AiArrayAllocate(0, 1, AI_TYPE_RGB));
   
   if (is3d)
   {
      AiParameterArray("coordinates", AiArrayAllocate(0, 1, AI_TYPE_VECTOR));
   }
   else
   {
      AiParameterArray("coordinates", AiArrayAllocate(0, 1, AI_TYPE_POINT2));
   }
   
   AiParameterArray("falloff", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
}

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidDataMtd);

node_parameters
{
   InitializeFluidShaderParameters(params);
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

node_initialize
{
   AiNodeSetLocalData(node, 0);
}

node_update
{
   CMayaFluidData<true>* data = (CMayaFluidData<true>*)AiNodeGetLocalData(node);
   if (data != 0)
      delete data;
   data = new CMayaFluidData<true>(node);
   AiNodeSetLocalData(node, data);
}

node_finish
{
   CMayaFluidData<true>* data = (CMayaFluidData<true>*)AiNodeGetLocalData(node);
   if (data != 0)
      delete data;
}

shader_evaluate
{

}
