#include "FluidTranslator.h"

#include <maya/MFnFluid.h>
#include <maya/MRampAttribute.h>
#include <maya/MColor.h>

void CFluidTranslator::NodeInitializer(CAbTranslator context)
{
   
}

AtNode* CFluidTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("box");
}

void ExportFloatGrid(AtNode* fluid, float* values, const char* paramName, unsigned int numVoxels)
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
   AiNodeDeclare(fluid, paramName, "constant ARRAY FLOAT");
   AiNodeSetArray(fluid, paramName, array);
}

void CFluidTranslator::Export(AtNode* fluid)
{
   MFnFluid mayaFluid(GetMayaObject());
   MFnDependencyNode mayaFluidNode(GetMayaObject());
   
   ExportMatrix(fluid, 0);
   
   unsigned int xRes, yRes, zRes;
   double xDim, yDim, zDim;
   
   mayaFluid.getResolution(xRes, yRes, zRes);
   mayaFluid.getDimensions(xDim, yDim, zDim);

   AiNodeSetPnt(fluid, "min", -0.5f * (float)xDim, -0.5f * (float)yDim, -0.5f * (float)zDim);
   AiNodeSetPnt(fluid, "max", 0.5f * (float)xDim, 0.5f * (float)yDim, 0.5f * (float)zDim);
   
   AiNodeSetFlt(fluid, "step_size", 0.1f);
   
   AtNode* fluid_shader = AiNode("mayaFluid"); // replace with a proper shader later
   AiNodeSetPtr(fluid, "shader", fluid_shader);
   
   // first getting a simple color information from the color gradient
   
   MRampAttribute colorRamp(mayaFluidNode.findPlug("color"));
   MColor color;
   colorRamp.getColorAtPosition(0.f, color);
   
   AiNodeSetRGB(fluid_shader, "color", (float)color.r, (float)color.g, (float)color.b);
   
   const unsigned int numVoxels = xRes * yRes * zRes;
   
   MFnFluid::FluidMethod fluidMethod;
   MFnFluid::FluidGradient fluidGradient;
   
   mayaFluid.getDensityMode(fluidMethod, fluidGradient);
   
   if (fluidMethod != MFnFluid::kZero)
      ExportFloatGrid(fluid_shader, mayaFluid.density(), "density", numVoxels);
   
   mayaFluid.getFuelMode(fluidMethod, fluidGradient);
   
   if (fluidMethod != MFnFluid::kZero)
      ExportFloatGrid(fluid_shader, mayaFluid.fuel(), "fuel", numVoxels);
   
   mayaFluid.getTemperatureMode(fluidMethod, fluidGradient);
   
   if (fluidMethod != MFnFluid::kZero)
      ExportFloatGrid(fluid_shader, mayaFluid.temperature(), "temperature", numVoxels);
   
   MFnFluid::ColorMethod colorMethod;
   
   mayaFluid.getColorMode(colorMethod);
   
   if (colorMethod != MFnFluid::kUseShadingColor)
   {
      float* r; float* g; float* b;
      mayaFluid.getColors(r, g, b);
      if (r != 0 && g != 0 && b != 0)
      {
         AtArray* array = AiArrayAllocate(numVoxels, 1, AI_TYPE_RGB);
         for (unsigned int i = 0; i < numVoxels; ++i)
         {
            AtColor cColor = {r[i], g[i], b[i]};
            cColor.r = cColor.r < AI_EPSILON ? 0.f : cColor.r;
            cColor.g = cColor.g < AI_EPSILON ? 0.f : cColor.g;
            cColor.b = cColor.b < AI_EPSILON ? 0.f : cColor.b;
            AiArraySetRGB(array, i, cColor);
         }
         AiNodeDeclare(fluid_shader, "colors", "constant ARRAY RGB");
         AiNodeSetArray(fluid_shader, "colors", array);
      }
   }
}
