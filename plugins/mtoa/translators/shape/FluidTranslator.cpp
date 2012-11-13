#include "FluidTranslator.h"

#include <maya/MFnFluid.h>
#include <maya/MRampAttribute.h>
#include <maya/MColor.h>

void CFluidTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper("fluidShape");
   
   CAttrData data;
   data.defaultValue.FLT = 0.f;
   data.name = "aiStepSize";
   data.shortName = "ai_step_size";
   helper.MakeInputFloat(data);
   
   data.defaultValue.FLT = 1.f;
   data.name = "aiShadowDensity";
   data.shortName = "ai_shadow_density";
   helper.MakeInputFloat(data);
}

AtNode* CFluidTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("box");
}

void CleanArray(AtNode* node, const char* paramName)
{
   AtArray* arr = AiNodeGetArray(node, paramName);
   if (arr->nelements > 0)
      AiArrayDestroy(arr);
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
   CleanArray(fluid, paramName);
   AiNodeSetArray(fluid, paramName, array);
}

void ExportFloatGradient(MPlug plug, AtNode* node, const char* paramName, int samplingResolution)
{
   MRampAttribute ramp(plug);
   AtArray* array = AiArrayAllocate(samplingResolution, 1, AI_TYPE_FLOAT);
   for (int i = 0; i < samplingResolution; ++i)
   {
      float v;
      ramp.getValueAtPosition((float)i / (float)(samplingResolution - 1), v);
      AiArraySetFlt(array, i, v);
   }
   CleanArray(node, paramName);
   AiNodeSetArray(node, paramName, array);
}

void ExportRGBGradient(MPlug plug, AtNode* node, const char* paramName, int samplingResolution)
{
   MRampAttribute ramp(plug);
   AtArray* array = AiArrayAllocate(samplingResolution, 1, AI_TYPE_RGB);
   for (int i = 0; i < samplingResolution; ++i)
   {
      MColor v;
      ramp.getColorAtPosition((float)i / (float)(samplingResolution - 1), v);
      AtRGB rgb = {(float)v.r, (float)v.g, (float)v.b};
      AiArraySetRGB(array, i, rgb);
   }
   CleanArray(node, paramName);
   AiNodeSetArray(node, paramName, array);
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
   
   float stepSize = 0.1f;
   MPlug plug = mayaFluidNode.findPlug("aiStepSize");
   if (!plug.isNull())
      stepSize = plug.asFloat();
   AiNodeSetFlt(fluid, "step_size", stepSize);
   float shadowDensity = 1.f;
   
   AtNode* fluid_shader = AiNode("mayaFluid"); // replace with a proper shader later
   AiNodeSetPtr(fluid, "shader", fluid_shader);
   
   plug = mayaFluidNode.findPlug("aiShadowDensity");
   if (!plug.isNull())
      shadowDensity = plug.asFloat();
   
   plug = mayaFluidNode.findPlug("transparency");
   if (!plug.isNull())
      AiNodeSetRGB(fluid_shader, "transparency", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   AiNodeSetFlt(fluid_shader, "shadow_density", shadowDensity);
   
   AiNodeSetArray(fluid_shader, "matrix", AiArrayCopy(AiNodeGetArray(fluid, "matrix")));
   AiNodeSetFlt(fluid_shader, "step_size", stepSize);
   ExportRGBGradient(mayaFluidNode.findPlug("color"), fluid_shader, "color_gradient", 1024);
   AiNodeSetInt(fluid_shader, "color_gradient_type", mayaFluidNode.findPlug("colorInput").asShort());
   AiNodeSetFlt(fluid_shader, "color_gradient_input_bias", mayaFluidNode.findPlug("colorInputBias").asFloat());
   
   ExportRGBGradient(mayaFluidNode.findPlug("incandescence"), fluid_shader, "incandescence_gradient", 1024);
   AiNodeSetInt(fluid_shader, "incandescence_gradient_type", mayaFluidNode.findPlug("incandescenceInput").asShort());
   AiNodeSetFlt(fluid_shader, "incandescence_gradient_input_bias", mayaFluidNode.findPlug("incandescenceInputBias").asFloat());
   
   ExportFloatGradient(mayaFluidNode.findPlug("opacity"), fluid_shader, "opacity_gradient", 1024);   
   AiNodeSetInt(fluid_shader, "opacity_gradient_type", mayaFluidNode.findPlug("opacityInput").asShort());
   AiNodeSetFlt(fluid_shader, "opacity_gradient_input_bias", mayaFluidNode.findPlug("opacityInputBias").asFloat());
   
   AiNodeSetInt(fluid_shader, "xres", xRes);
   AiNodeSetInt(fluid_shader, "yres", yRes);
   AiNodeSetInt(fluid_shader, "zres", zRes);
   
   AiNodeSetFlt(fluid_shader, "xdim", (float)xDim);
   AiNodeSetFlt(fluid_shader, "ydim", (float)yDim);
   AiNodeSetFlt(fluid_shader, "zdim", (float)zDim);   
   
   // first getting a simple color information from the color gradient
   
   MRampAttribute colorRamp(mayaFluidNode.findPlug("color"));
   MColor color;
   colorRamp.getColorAtPosition(0.f, color);
   
   AiNodeSetRGB(fluid_shader, "color", (float)color.r, (float)color.g, (float)color.b);
   
   const unsigned int numVoxels = xRes * yRes * zRes;
   
   // support for gradient mode
   
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
   
   ExportFloatGrid(fluid_shader, mayaFluid.pressure(), "pressure", numVoxels);
   
   mayaFluid.getVelocityMode(fluidMethod, fluidGradient);
   
   if (fluidMethod != MFnFluid::kZero)
   {
      float* x; float* y; float* z;
      mayaFluid.getVelocity(x, y, z);
      if (x != 0 && y != 0 && z != 0)
      {
         AtArray* array = AiArrayAllocate(numVoxels, 1, AI_TYPE_VECTOR);
         for (unsigned int i = 0; i < numVoxels; ++i)
         {
            AtVector cVector = {x[i], y[i], z[i]};
            cVector.x = cVector.x < AI_EPSILON ? 0.f : cVector.x;
            cVector.y = cVector.y < AI_EPSILON ? 0.f : cVector.y;
            cVector.z = cVector.z < AI_EPSILON ? 0.f : cVector.z;
            AiArraySetVec(array, i, cVector);
         }
         AiNodeSetArray(fluid_shader, "velocity", array);
      }
   }
   
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
         AiNodeSetArray(fluid_shader, "colors", array);
      }
   }
}
