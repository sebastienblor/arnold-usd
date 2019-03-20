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

void CFluidTexture2DTranslator::ExportFloatGradient(MPlug plug, AtNode* node, const char* paramName)
{
   MIntArray plugArrayIndices;
   plug.getExistingArrayAttributeIndices(plugArrayIndices);
   unsigned int numElements = plugArrayIndices.length();
   AtArray* positions = AiArrayAllocate(numElements, 1, AI_TYPE_FLOAT);
   AtArray* values = AiArrayAllocate(numElements, 1, AI_TYPE_FLOAT);
   AtArray* interps = AiArrayAllocate(numElements, 1, AI_TYPE_INT);
   const MString positions_name = MString(paramName) + MString("_positions");
   const MString values_name = MString(paramName) + MString("_values");
   const MString interps_name = MString(paramName) + MString("_interps");   
   for (unsigned int i = 0; i < numElements; ++i)
   {
      MPlug plugElement = plug.elementByLogicalIndex(plugArrayIndices[i]);
      AiArraySetFlt(positions, i, plugElement.child(0).asFloat());
      AiArraySetFlt(values, i, plugElement.child(1).asFloat());
      AiArraySetInt(interps, i, plugElement.child(2).asInt());
   }
   AiNodeSetArray(node, positions_name.asChar(), positions);
   AiNodeSetArray(node, values_name.asChar(), values);
   AiNodeSetArray(node, interps_name.asChar(), interps);
}

void CFluidTexture2DTranslator::ExportRGBGradient(MPlug plug, AtNode* node, const char* paramName)
{
   MIntArray plugArrayIndices;
   plug.getExistingArrayAttributeIndices(plugArrayIndices);
   unsigned int numElements = plugArrayIndices.length();
   AtArray* positions = AiArrayAllocate(numElements, 1, AI_TYPE_FLOAT);
   AtArray* values = AiArrayAllocate(numElements, 1, AI_TYPE_RGB);
   AtArray* interps = AiArrayAllocate(numElements, 1, AI_TYPE_INT);
   const MString positions_name = MString(paramName) + MString("_positions");
   const MString values_name = MString(paramName) + MString("_values");
   const MString interps_name = MString(paramName) + MString("_interps");   
   // check for the existing links, and unlink them
   // this is required to be able to change the connections in ipr
   AtArray* valuesOld = AiNodeGetArray(node, values_name.asChar());
   unsigned oldNelements = AiArrayGetNumElements(valuesOld);
   for (unsigned i = 0; i < oldNelements; i++)
   {
      MString attributeName = values_name + MString("[");
      attributeName += i;
      attributeName += "]";
      AtNode* linkedNode = AiNodeGetLink(node, attributeName.asChar());
      if (linkedNode != 0)
         AiNodeUnlink(node, attributeName.asChar());
   }
   for (unsigned int i = 0; i < numElements; ++i)
   {
      MPlug plugElement = plug.elementByLogicalIndex(plugArrayIndices[i]);
      AiArraySetFlt(positions, i, plugElement.child(0).asFloat());
      MPlug colorPlug = plugElement.child(1);
      MPlugArray conns;
      MStatus status;
      colorPlug.connectedTo(conns, true, false, &status);
      if (status && conns.length())
      {
         AtNode* connectedColor = ExportConnectedNode(conns[0]);
         MString attributeName = values_name + MString("[");
         attributeName += i;
         attributeName += "]";
         AiNodeLink(connectedColor, attributeName.asChar(), node);
      }
      else
      {
         AtRGB color (colorPlug.child(0).asFloat(),
                        colorPlug.child(1).asFloat(),
                        colorPlug.child(2).asFloat());
         AiArraySetRGB(values, i, color);
      }
      AiArraySetInt(interps, i, plugElement.child(2).asInt());
   }
   AiNodeSetArray(node, positions_name.asChar(), positions);
   AiNodeSetArray(node, values_name.asChar(), values);
   AiNodeSetArray(node, interps_name.asChar(), interps);
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

   const int colorGradientType = mayaFluidNode.findPlug("colorInput", true).asShort();

   ExportRGBGradient(mayaFluidNode.findPlug("color", true), fluid2d, "color_gradient");
   AiNodeSetInt(fluid2d, "color_gradient_type", colorGradientType);
   AiNodeSetFlt(fluid2d, "color_gradient_input_bias", mayaFluidNode.findPlug("colorInputBias", true).asFloat());
   
   const int incandescenceGradientType = mayaFluidNode.findPlug("incandescenceInput", true).asShort();
   ExportRGBGradient(mayaFluidNode.findPlug("incandescence", true), fluid2d, "incandescence_gradient");
   AiNodeSetInt(fluid2d, "incandescence_gradient_type", incandescenceGradientType);
   AiNodeSetFlt(fluid2d, "incandescence_gradient_input_bias", mayaFluidNode.findPlug("incandescenceInputBias", true).asFloat());   
   
   const int opacityGradientType = mayaFluidNode.findPlug("opacityInput", true).asShort();
   ExportFloatGradient(mayaFluidNode.findPlug("opacity", true), fluid2d, "opacity_gradient");
   AiNodeSetInt(fluid2d, "opacity_gradient_type", opacityGradientType);
   AiNodeSetFlt(fluid2d, "opacity_gradient_input_bias", mayaFluidNode.findPlug("opacityInputBias", true).asFloat());

   ProcessParameter(fluid2d, "dropoff_shape", AI_TYPE_INT, "dropoffShape");
   ProcessParameter(fluid2d, "edge_dropoff", AI_TYPE_FLOAT, "edgeDropoff");
   ProcessParameter(fluid2d, "velocity_scale", AI_TYPE_VECTOR, "velocityScale");

   MPlug renderInterpolatorPlug = FindMayaPlug("renderInterpolator");
   if (!renderInterpolatorPlug.isNull())
      AiNodeSetInt(fluid2d, "filter_type", renderInterpolatorPlug.asInt() == 3 ? 1 : 0);   

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

   mayaFluid.getVelocityMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid2d, "velocity_method", "Gradient");
      SetContentsGradientMode(fluid2d, "velocity_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid2d, "velocity_method", "Grid");
   
   if (fluidMethod != MFnFluid::kZero)
   {
      float* x; float* y; float* z;
      mayaFluid.getVelocity(x, y, z);
      if (x != 0 && y != 0 && z != 0)
      {
         AtArray* array = AiArrayAllocate(numVoxels, 1, AI_TYPE_VECTOR);
         for (unsigned int i = 0; i < numVoxels; ++i)
         {
            AtVector cVector(x[i], y[i], z[i]);
            cVector.x = cVector.x < AI_EPSILON ? 0.f : cVector.x;
            cVector.y = cVector.y < AI_EPSILON ? 0.f : cVector.y;
            cVector.z = cVector.z < AI_EPSILON ? 0.f : cVector.z;
            AiArraySetVec(array, i, cVector);
         }
         AiNodeSetArray(fluid2d, "velocity", array);
      }
   }
   
   float* u; float* v; float* w;
   mayaFluid.getCoordinates(u, v, w);
   if (u != 0 && v != 0)
   {
      AtArray* array = AiArrayAllocate(numVoxels, 1, AI_TYPE_VECTOR2);
      for (unsigned int i = 0; i < numVoxels; ++i)
      {
         AtVector2 cCoord(u[i], v[i]);
         AiArraySetVec2(array, i, cCoord);
      }
      AiNodeSetArray(fluid2d, "coordinates", array);
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
            AtRGB cColor (r[i], g[i], b[i]);
            cColor.r = cColor.r < AI_EPSILON ? 0.f : cColor.r;
            cColor.g = cColor.g < AI_EPSILON ? 0.f : cColor.g;
            cColor.b = cColor.b < AI_EPSILON ? 0.f : cColor.b;
            AiArraySetRGB(array, i, cColor);
         }
         AiNodeSetArray(fluid2d, "colors", array);
      }
   }
}
