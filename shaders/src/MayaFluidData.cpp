#include "MayaFluidData.h"

const char* contentsGradientTypeEnums[] = {"Constant", "X Gradient", "Y Gradient", "Z Gradient",
                                           "-X Gradient", "-Y Gradient", "-Z Gradient",
                                           "Center Gradient", 0};

const char* contentsMethodEnums[] = {"Grid", "Gradient", 0};

template <typename T>
void ReadArray(AtArray* array, int cm, int cmg, int numVoxels, ArrayDescription<T>& arrayDesc)
{
   arrayDesc.release();

   if (cm == CSM_GRID)
   {
      if ((int)array->nelements == numVoxels)
      {
         arrayDesc.single = false;
         arrayDesc.data = (T*)AiMalloc(sizeof(T) * numVoxels);
         for (int i = 0; i < numVoxels; ++i)
            arrayDesc.data[i] = ReadFromArray<T>(array, i);
      }
      else if (array->nelements == 1) // only one value
      {
         arrayDesc.single = true;
         arrayDesc.data = (T*)AiMalloc(sizeof(T));
         *arrayDesc.data = ReadFromArray<T>(array, 0);
      }
      else
      {
         arrayDesc.single = false;
         arrayDesc.data = 0;
      }
      arrayDesc.isGradient = false;
   }
   else
   {
      arrayDesc.isGradient = true;
      arrayDesc.gradientType = cmg;
   }
}

CMayaFluidData::CMayaFluidData(AtNode* node)
{
   xres = AiNodeGetInt(node, "xres");
   yres = AiNodeGetInt(node, "yres");
   zres = AiNodeGetInt(node, "zres");

   const int numVoxels = xres * yres * zres;
   
   if (numVoxels == 0)
      return;

   dmin = AiNodeGetVec(node, "min");
   dmax = AiNodeGetVec(node, "max");
   dmax = dmax - dmin;
   dmax.x = 1.f / dmax.x;
   dmax.y = 1.f / dmax.y;
   dmax.z = 1.f / dmax.z;

   ReadArray(AiNodeGetArray(node, "density"), AiNodeGetInt(node, "density_method"), AiNodeGetInt(node, "density_gradient"), numVoxels, density);
   ReadArray(AiNodeGetArray(node, "fuel"), AiNodeGetInt(node, "fuel_method"), AiNodeGetInt(node, "fuel_gradient"), numVoxels, fuel);
   ReadArray(AiNodeGetArray(node, "temperature"), AiNodeGetInt(node, "temperature_method"), AiNodeGetInt(node, "temperature_gradient"), numVoxels, temperature);
   ReadArray(AiNodeGetArray(node, "pressure"), CSM_GRID, CG_CONSTANT, numVoxels, pressure);
   ReadArray(AiNodeGetArray(node, "velocity"), AiNodeGetInt(node, "velocity_method"), AiNodeGetInt(node, "velocity_gradient"), numVoxels, velocity);
   ReadArray(AiNodeGetArray(node, "colors"), CSM_GRID, CG_CONSTANT, numVoxels, colors);
   ReadArray(AiNodeGetArray(node, "coordinates"), CSM_GRID, CG_CONSTANT, numVoxels, coordinates);
   ReadArray(AiNodeGetArray(node, "falloff"), CSM_GRID, CG_CONSTANT, numVoxels, falloff);
}

CMayaFluidData::~CMayaFluidData()
{
   density.release();
   fuel.release();
   temperature.release();
   pressure.release();
   velocity.release();
   colors.release();
   coordinates.release();
}

void CMayaFluidData::InitializeShaderParameters(AtList* params)
{
	AiParameterInt("xres", 0);
   AiParameterInt("yres", 0);
   AiParameterInt("zres", 0);
   
   AiParameterVec("min", 0.f, 0.f, 0.f);
   AiParameterVec("max", 0.f, 0.f, 0.f);
   
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
   
   AiParameterArray("coordinates", AiArrayAllocate(0, 1, AI_TYPE_VECTOR));
   
   AiParameterArray("falloff", AiArrayAllocate(0, 1, AI_TYPE_FLOAT));
}

AI_SHADER_NODE_EXPORT_METHODS(MayaFluidDataMtd);

node_parameters
{
   CMayaFluidData::InitializeShaderParameters(params);
}

node_initialize
{
   AiNodeSetLocalData(node, 0);
}

node_update
{
   CMayaFluidData* data = (CMayaFluidData*)AiNodeGetLocalData(node);
   if (data != 0)
      delete data;
   data = new CMayaFluidData(node);
   AiNodeSetLocalData(node, data);
}

node_finish
{
   CMayaFluidData* data = (CMayaFluidData*)AiNodeGetLocalData(node);
   if (data != 0)
      delete data;
}

shader_evaluate
{

}
