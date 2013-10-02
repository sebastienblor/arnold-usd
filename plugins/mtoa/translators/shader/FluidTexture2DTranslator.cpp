#include "FluidTexture2DTranslator.h"

#include <maya/MFnFluid.h>

AtNode* CFluidTexture2DTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaFluidTexture2D");
}

inline void ExportFloatGrid(AtNode* fluid, float* values, const char* paramName, unsigned int numVoxels)
{
   if (values == 0)
      return;
   AtArray* array = AiArrayAllocate(numVoxels, 1, AI_TYPE_FLOAT);
   for (unsigned int i = 0; i < numVoxels; ++i)
   {
      const float cVoxel = values[i];
      if (cVoxel < AI_EPSILON)
         AiArraySetFlt(array, i, 0.f);
      else
         AiArraySetFlt(array, i, cVoxel);
   }
   AiNodeSetArray(fluid, paramName, array);
}

inline void SetContentsGradientMode(AtNode* node, const char* parameter, MFnFluid::FluidGradient fluidGradient)
{
   if (fluidGradient == MFnFluid::kConstant)
      AiNodeSetStr(node, parameter, "Constant");
   else if (fluidGradient == MFnFluid::kXGradient)
      AiNodeSetStr(node, parameter, "X Gradient");
   else if (fluidGradient == MFnFluid::kYGradient)
      AiNodeSetStr(node, parameter, "Y Gradient");
   else if (fluidGradient == MFnFluid::kZGradient)
      AiNodeSetStr(node, parameter, "Z Gradient");
   else if (fluidGradient == MFnFluid::kNegXGradient)
      AiNodeSetStr(node, parameter, "-X Gradient");
   else if (fluidGradient == MFnFluid::kNegYGradient)
      AiNodeSetStr(node, parameter, "-Y Gradient");
   else if (fluidGradient == MFnFluid::kNegZGradient)
      AiNodeSetStr(node, parameter, "-Z Gradient");      
   else if (fluidGradient == MFnFluid::kCenterGradient)
      AiNodeSetStr(node, parameter, "Center Gradient");
};

void CFluidTexture2DTranslator::Export(AtNode* fluid2d)
{
   MFnFluid mayaFluid(GetMayaObject());
   MFnDependencyNode mayaFluidNode(GetMayaObject());

   unsigned int xRes, yRes;

   mayaFluid.getResolution(xRes, yRes);

   AiNodeSetInt(fluid2d, "xres", xRes);
   AiNodeSetInt(fluid2d, "yres", yRes);

   MFnFluid::FluidMethod fluidMethod;
   MFnFluid::FluidGradient fluidGradient;

   const unsigned int numVoxels = xRes * yRes;

   ExportFloatGrid(fluid2d, mayaFluid.falloff(), "falloff", numVoxels);
   
   mayaFluid.getDensityMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid2d, "density_method", "Gradient");
      SetContentsGradientMode(fluid2d, "density_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid2d, "density_method", "Grid");

   if (fluidMethod != MFnFluid::kZero)
      ExportFloatGrid(fluid2d, mayaFluid.density(), "density", numVoxels);
   
   mayaFluid.getFuelMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid2d, "fuel_method", "Gradient");
      SetContentsGradientMode(fluid2d, "fuel_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid2d, "fuel_method", "Grid");
   
   if (fluidMethod != MFnFluid::kZero)
      ExportFloatGrid(fluid2d, mayaFluid.fuel(), "fuel", numVoxels);
   
   mayaFluid.getTemperatureMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid2d, "temperature_method", "Gradient");
      SetContentsGradientMode(fluid2d, "temperature_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid2d, "temperature_method", "Grid");
   
   if (fluidMethod != MFnFluid::kZero)
      ExportFloatGrid(fluid2d, mayaFluid.temperature(), "temperature", numVoxels);
}
