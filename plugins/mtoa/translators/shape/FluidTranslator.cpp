#include "FluidTranslator.h"

#include <maya/MFnFluid.h>
#include <maya/MRampAttribute.h>
#include <maya/MColor.h>
#include <maya/MPlugArray.h>

void CFluidTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper("fluidShape");
   
   CAttrData data;
   data.defaultValue.FLT = 0.1f;
   data.hasMin = true;
   data.hasSoftMax = true;
   data.min.FLT = 0.f;
   data.softMax.FLT = 2.f;
   data.name = "aiStepSize";
   data.shortName = "ai_step_size";
   helper.MakeInputFloat(data);
   
   data.hasSoftMax = false;
   
   data.defaultValue.FLT = 0.f;
   data.hasMax = true;
   data.name = "aiPhaseFunc";
   data.shortName = "aiPhaseFunc";
   
   data.min.FLT = -1.f;
   data.max.FLT = 1.f;
   helper.MakeInputFloat(data);
   
   data.hasMin = false;
   data.hasMax = false;
   
   data.name = "aiOverrideTextures";
   data.shortName = "ai_override_textures";
   data.defaultValue.BOOL = false;
   helper.MakeInputBoolean(data);
   
   data.name = "aiTextureAffectColor";
   data.shortName = "ai_texture_affect_color";   
   helper.MakeInputBoolean(data);
   
   data.name = "aiTextureAffectIncand";
   data.shortName = "ai_texture_affect_incand";
   helper.MakeInputBoolean(data);
   
   data.name = "aiTextureAffectOpacity";
   data.shortName = "ai_texture_affect_opacity";
   helper.MakeInputBoolean(data);
   
   data.name = "aiVolumeTexture";
   data.shortName = "ai_volume_texture";
   helper.MakeInputRGB(data);   
   
   MStringArray strArr;
   strArr.append("Fixed");
   strArr.append("Grid");
   data.enums = strArr;
   data.defaultValue.INT = 0;
   data.name = "aiTextureCoordinateMethod";
   data.shortName = "ai_texture_coordinate_method";
   helper.MakeInputEnum(data);
   
   strArr.clear();
   strArr.append("Closest");
   strArr.append("Linear");
   strArr.append("Cubic");
   data.enums = strArr;
   data.defaultValue.INT = 1;
   data.name = "aiFilterType";
   data.shortName = "ai_filter_type";
   helper.MakeInputEnum(data);

   MakeArnoldVisibilityFlags(helper);
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
   AiNodeSetArray(fluid, paramName, array);
}

void CFluidTranslator::ExportFloatGradient(MPlug plug, AtNode* node, const char* paramName)
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

void CFluidTranslator::ExportRGBGradient(MPlug plug, AtNode* node, const char* paramName)
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
   for (AtUInt32 i = 0; i < valuesOld->nelements; i++)
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
         AtNode* connectedColor = ExportNode(conns[0]);
         MString attributeName = values_name + MString("[");
         attributeName += i;
         attributeName += "]";
         AiNodeLink(connectedColor, attributeName.asChar(), node);
      }
      else
      {
         AtRGB color = {colorPlug.child(0).asFloat(),
                        colorPlug.child(1).asFloat(),
                        colorPlug.child(2).asFloat()};
         AiArraySetRGB(values, i, color);
      }
      AiArraySetInt(interps, i, plugElement.child(2).asInt());
   }
   AiNodeSetArray(node, positions_name.asChar(), positions);
   AiNodeSetArray(node, values_name.asChar(), values);
   AiNodeSetArray(node, interps_name.asChar(), interps);
}

enum GradientType{
   GT_CONSTANT = 0,
   GT_X_GRADIENT,
   GT_Y_GRADIENT,
   GT_Z_GRADIENT,
   GT_CENTER_GRADIENT,
   GT_DENSITY,
   GT_TEMPERATURE,
   GT_FUEL,
   GT_PRESSURE,
   GT_SPEED,
   GT_DENSITY_AND_FUEL
};

void SetContentsGradientMode(AtNode* node, const char* parameter, MFnFluid::FluidGradient fluidGradient)
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

void CFluidTranslator::Export(AtNode* fluid)
{
   MFnFluid mayaFluid(GetMayaObject());
   MFnDependencyNode mayaFluidNode(GetMayaObject());
   
   ExportMatrix(fluid, 0);
   AiNodeSetInt(fluid, "visibility", ComputeVisibility());
   ExportTraceSets(fluid, FindMayaPlug("aiTraceSets"));
   ProcessParameter(fluid, "receive_shadows", AI_TYPE_BOOLEAN, "receiveShadows");
   
   AtNode* fluid_shader = (AtNode*)AiNodeGetPtr(fluid, "shader");
   MString shader_name = MString(AiNodeGetStr(fluid, "name")) + MString("_shader");

   if (MString(AiNodeGetName(fluid_shader)) != shader_name)
   {
      fluid_shader = AiNode("mayaFluid"); // replace with a proper shader later
      AiNodeSetPtr(fluid, "shader", fluid_shader);
      AiNodeSetStr(fluid_shader, "name", shader_name.asChar());
   }
   
   float stepSize = 0.1f;
   MPlug plug = FindMayaPlug("aiStepSize");
   if (!plug.isNull())
      stepSize = plug.asFloat();
   AiNodeSetFlt(fluid, "step_size", stepSize);
   
   unsigned int xRes, yRes, zRes;
   double xDim, yDim, zDim;
   
   mayaFluid.getResolution(xRes, yRes, zRes);
   mayaFluid.getDimensions(xDim, yDim, zDim);
   plug = mayaFluid.findPlug("dynamicOffset");
   float dynOffX = plug.child(0).asFloat();
   float dynOffY = plug.child(1).asFloat();
   float dynOffZ = plug.child(2).asFloat();
   
   const AtVector mn = {-0.5f * (float)xDim + dynOffX, -0.5f * (float)yDim + dynOffY, -0.5f * (float)zDim + dynOffZ};
   const AtVector mx = {0.5f * (float)xDim + dynOffX, 0.5f * (float)yDim + dynOffY, 0.5f * (float)zDim + dynOffZ};

   AiNodeSetPnt(fluid, "min", mn.x, mn.y, mn.z);
   AiNodeSetPnt(fluid, "max", mx.x, mx.y, mx.z);
   
   plug = FindMayaPlug("transparency");
   if (!plug.isNull())
      AiNodeSetRGB(fluid_shader, "transparency", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("aiPhaseFunc");
   if (!plug.isNull())
      AiNodeSetFlt(fluid_shader, "phase_func", plug.asFloat());
   
   MFnFluid::CoordinateMethod coordinateMode;
   
   mayaFluid.getCoordinateMode(coordinateMode);
   
   bool exportCoordinates = false;
   
   if (FindMayaPlug("aiOverrideTextures").asBool())   
   {
      plug = FindMayaPlug("aiVolumeTexture");
      if (!plug.isNull())
      {
         MPlugArray volumeNoisePlug;
         plug.connectedTo(volumeNoisePlug, true, false);
         if (volumeNoisePlug.length() > 0)
         {
            AtNode* volumeNoise = ExportNode(volumeNoisePlug[0]);
            AiNodeSetPtr(fluid_shader, "volume_texture", volumeNoise);
         }
      }
      
      ProcessParameter(fluid_shader, "texture_affect_color", AI_TYPE_BOOLEAN, "aiTextureAffectColor");
      ProcessParameter(fluid_shader, "texture_affect_incand", AI_TYPE_BOOLEAN, "aiTextureAffectIncand");
      ProcessParameter(fluid_shader, "texture_affect_opacity", AI_TYPE_BOOLEAN, "aiTextureAffectOpacity");
      ProcessParameter(fluid_shader, "coordinate_method", AI_TYPE_INT, "aiTextureCoordinateMethod");
      const int coordinateMethod = AiNodeGetInt(fluid_shader, "coordinate_method");
      if ((coordinateMethod == 1) && (coordinateMode == MFnFluid::kGrid))
         exportCoordinates = true;
   }
   else
   {
      ProcessParameter(fluid_shader, "color_texture", AI_TYPE_BOOLEAN, "colorTexture");
      ProcessParameter(fluid_shader, "incand_texture", AI_TYPE_BOOLEAN, "incandTexture");
      ProcessParameter(fluid_shader, "opacity_texture", AI_TYPE_BOOLEAN, "opacityTexture");

      ProcessParameter(fluid_shader, "texture_type", AI_TYPE_INT, "textureType");
      ProcessParameter(fluid_shader, "coordinate_method", AI_TYPE_INT, "coordinateMethod");
      const int coordinateMethod = AiNodeGetInt(fluid_shader, "coordinate_method");
      if ((coordinateMethod == 1) && (coordinateMode == MFnFluid::kGrid))
         exportCoordinates = true;

      ProcessParameter(fluid_shader, "color_tex_gain", AI_TYPE_FLOAT, "colorTexGain");
      ProcessParameter(fluid_shader, "incand_tex_gain", AI_TYPE_FLOAT, "incandTexGain");
      ProcessParameter(fluid_shader, "opacity_tex_gain", AI_TYPE_FLOAT, "opacityTexGain");

      ProcessParameter(fluid_shader, "threshold", AI_TYPE_FLOAT, "threshold");
      ProcessParameter(fluid_shader, "amplitude", AI_TYPE_FLOAT, "amplitude");
      ProcessParameter(fluid_shader, "ratio", AI_TYPE_FLOAT, "ratio");
      ProcessParameter(fluid_shader, "frequency_ratio", AI_TYPE_FLOAT, "frequencyRatio");
      ProcessParameter(fluid_shader, "depth_max", AI_TYPE_INT, "depthMax");

      ProcessParameter(fluid_shader, "invert_texture", AI_TYPE_BOOLEAN, "invertTexture");
      ProcessParameter(fluid_shader, "inflection", AI_TYPE_BOOLEAN, "inflection");

      ProcessParameter(fluid_shader, "texture_time", AI_TYPE_FLOAT, "textureTime");
      ProcessParameter(fluid_shader, "zoom_factor", AI_TYPE_FLOAT, "zoomFactor");
      ProcessParameter(fluid_shader, "frequency", AI_TYPE_FLOAT, "frequency");

      ProcessParameter(fluid_shader, "texture_origin_x", AI_TYPE_FLOAT, "textureOriginX");
      ProcessParameter(fluid_shader, "texture_origin_y", AI_TYPE_FLOAT, "textureOriginY");
      ProcessParameter(fluid_shader, "texture_origin_z", AI_TYPE_FLOAT, "textureOriginZ");

      ProcessParameter(fluid_shader, "texture_scale", AI_TYPE_VECTOR, "textureScale");
      ProcessParameter(fluid_shader, "texture_rotate", AI_TYPE_VECTOR, "textureRotate");

      ProcessParameter(fluid_shader, "implode", AI_TYPE_FLOAT, "implode");
      ProcessParameter(fluid_shader, "implode_center", AI_TYPE_VECTOR, "implodeCenter");
      
      ProcessParameter(fluid_shader, "billow_density", AI_TYPE_FLOAT, "billowDensity");
      ProcessParameter(fluid_shader, "spottyness", AI_TYPE_FLOAT, "spottyness");
      ProcessParameter(fluid_shader, "size_rand", AI_TYPE_FLOAT, "sizeRand");
      ProcessParameter(fluid_shader, "randomness", AI_TYPE_FLOAT, "randomness");
      ProcessParameter(fluid_shader, "billow_falloff", AI_TYPE_INT, "falloff");
      
      ProcessParameter(fluid_shader, "num_waves", AI_TYPE_INT, "numWaves");
   }
   
   bool exportDensity = false;
   bool exportFuel = false;
   bool exportTemperature = false;
   bool exportPressure = false;
   bool exportVelocity = false;
   bool exportColors = false;

   const int colorGradientType = mayaFluidNode.findPlug("colorInput").asShort();
   if (colorGradientType == GT_DENSITY)
      exportDensity = true;
   else if (colorGradientType == GT_TEMPERATURE)
      exportTemperature = true;
   else if (colorGradientType == GT_FUEL)
      exportFuel = true;
   else if (colorGradientType == GT_PRESSURE)
      exportFuel = true;
   else if (colorGradientType == GT_SPEED)
      exportVelocity = true;
   else if (colorGradientType == GT_DENSITY_AND_FUEL)
   {
      exportDensity = true;
      exportFuel = true;
   }

   ExportRGBGradient(mayaFluidNode.findPlug("color"), fluid_shader, "color_gradient");
   AiNodeSetInt(fluid_shader, "color_gradient_type", colorGradientType);
   AiNodeSetFlt(fluid_shader, "color_gradient_input_bias", mayaFluidNode.findPlug("colorInputBias").asFloat());
   
   const int incandescenceGradientType = mayaFluidNode.findPlug("incandescenceInput").asShort();
   if (incandescenceGradientType == GT_DENSITY)
      exportDensity = true;
   else if (incandescenceGradientType == GT_TEMPERATURE)
      exportTemperature = true;
   else if (incandescenceGradientType == GT_FUEL)
      exportFuel = true;
   else if (incandescenceGradientType == GT_PRESSURE)
      exportFuel = true;
   else if (incandescenceGradientType == GT_SPEED)
      exportVelocity = true;
   else if (incandescenceGradientType == GT_DENSITY_AND_FUEL)
   {
      exportDensity = true;
      exportFuel = true;
   }
   ExportRGBGradient(mayaFluidNode.findPlug("incandescence"), fluid_shader, "incandescence_gradient");
   AiNodeSetInt(fluid_shader, "incandescence_gradient_type", incandescenceGradientType);
   AiNodeSetFlt(fluid_shader, "incandescence_gradient_input_bias", mayaFluidNode.findPlug("incandescenceInputBias").asFloat());   
   
   const int opacityGradientType = mayaFluidNode.findPlug("opacityInput").asShort();
   if (opacityGradientType == GT_DENSITY)
      exportDensity = true;
   else if (opacityGradientType == GT_TEMPERATURE)
      exportTemperature = true;
   else if (opacityGradientType == GT_FUEL)
      exportFuel = true;
   else if (opacityGradientType == GT_PRESSURE)
      exportFuel = true;
   else if (opacityGradientType == GT_SPEED)
      exportVelocity = true;
   else if (opacityGradientType == GT_DENSITY_AND_FUEL)
   {
      exportDensity = true;
      exportFuel = true;
   }
   ExportFloatGradient(mayaFluidNode.findPlug("opacity"), fluid_shader, "opacity_gradient");
   AiNodeSetInt(fluid_shader, "opacity_gradient_type", opacityGradientType);
   AiNodeSetFlt(fluid_shader, "opacity_gradient_input_bias", mayaFluidNode.findPlug("opacityInputBias").asFloat());
   
   AiNodeSetInt(fluid_shader, "xres", xRes);
   AiNodeSetInt(fluid_shader, "yres", yRes);
   AiNodeSetInt(fluid_shader, "zres", zRes);
   
   AiNodeSetVec(fluid_shader, "min", mn.x, mn.y, mn.z);
   AiNodeSetVec(fluid_shader, "max", mx.x, mx.y, mx.z);
   
   ProcessParameter(fluid, "self_shadows", AI_TYPE_BOOLEAN, "selfShadowing");
   ProcessParameter(fluid_shader, "shadow_opacity", AI_TYPE_FLOAT, "shadowOpacity");

   ProcessParameter(fluid_shader, "dropoff_shape", AI_TYPE_INT, "dropoffShape");
   ProcessParameter(fluid_shader, "edge_dropoff", AI_TYPE_FLOAT, "edgeDropoff");
   ProcessParameter(fluid_shader, "velocity_scale", AI_TYPE_VECTOR, "velocityScale");
   
   // first getting a simple color information from the color gradient
   ProcessParameter(fluid_shader, "filter_type", AI_TYPE_INT, "aiFilterType");
   
   const unsigned int numVoxels = xRes * yRes * zRes;
   
   if (AiNodeGetInt(fluid_shader, "dropoff_shape") == 11)
   {
      ExportFloatGrid(fluid_shader, mayaFluid.falloff(), "falloff", numVoxels);
   }
   
   // support for gradient mode  

   MFnFluid::FluidMethod fluidMethod;
   MFnFluid::FluidGradient fluidGradient;
   
   mayaFluid.getDensityMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid_shader, "density_method", "Gradient");
      SetContentsGradientMode(fluid_shader, "density_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid_shader, "density_method", "Grid");

   if (exportDensity && (fluidMethod != MFnFluid::kZero))
      ExportFloatGrid(fluid_shader, mayaFluid.density(), "density", numVoxels);
   
   mayaFluid.getFuelMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid_shader, "fuel_method", "Gradient");
      SetContentsGradientMode(fluid_shader, "fuel_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid_shader, "fuel_method", "Grid");
   
   if (exportFuel && (fluidMethod != MFnFluid::kZero))
      ExportFloatGrid(fluid_shader, mayaFluid.fuel(), "fuel", numVoxels);
   
   mayaFluid.getTemperatureMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid_shader, "temperature_method", "Gradient");
      SetContentsGradientMode(fluid_shader, "temperature_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid_shader, "temperature_method", "Grid");
   
   if (exportTemperature && (fluidMethod != MFnFluid::kZero))
      ExportFloatGrid(fluid_shader, mayaFluid.temperature(), "temperature", numVoxels);
   
   if (exportPressure)
      ExportFloatGrid(fluid_shader, mayaFluid.pressure(), "pressure", numVoxels);
   
   mayaFluid.getVelocityMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid_shader, "velocity_method", "Gradient");
      SetContentsGradientMode(fluid_shader, "velocity_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid_shader, "velocity_method", "Grid");
   
   if ((fluidMethod != MFnFluid::kZero) && exportVelocity)
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
   
   if (exportCoordinates)
   {
      float* u; float* v; float* w;
      mayaFluid.getCoordinates(u, v, w);
      if (u != 0 && v != 0 && w != 0)
      {
         AtArray* array = AiArrayAllocate(numVoxels, 1, AI_TYPE_VECTOR);
         for (unsigned int i = 0; i < numVoxels; ++i)
         {
            AtVector cCoord = {u[i], v[i], w[i]};
            AiArraySetVec(array, i, cCoord);
         }
         AiNodeSetArray(fluid_shader, "coordinates", array);
      }
   }
   
   MFnFluid::ColorMethod colorMethod;
   
   mayaFluid.getColorMode(colorMethod);
   
   if ((colorMethod != MFnFluid::kUseShadingColor) && exportColors)
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

void CFluidTranslator::ExportMotion(AtNode* fluid, unsigned int step)
{
   ExportMatrix(fluid, step);
}
