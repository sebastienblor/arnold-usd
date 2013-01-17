#include "FluidTranslator.h"

#include <maya/MFnFluid.h>
#include <maya/MRampAttribute.h>
#include <maya/MColor.h>
#include <maya/MPlugArray.h>

void CFluidTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper("fluidShape");
   
   CAttrData data;
   data.defaultValue.FLT = 0.f;
   data.name = "aiStepSize";
   data.shortName = "ai_step_size";
   helper.MakeInputFloat(data);
   
   data.defaultValue.FLT = 0.f;
   data.name = "aiPhaseFunc";
   data.shortName = "aiPhaseFunc";
   data.hasMin = true;
   data.min.FLT = -1.f;
   data.hasMax = true;
   data.max.FLT = 1.f;
   helper.MakeInputFloat(data);
   
   data.hasMin = false;
   data.hasMax = false;
   
   data.name = "aiNoiseAffectColor";
   data.shortName = "ai_noise_affect_color";
   data.defaultValue.BOOL = false;
   helper.MakeInputBoolean(data);
   
   data.name = "aiNoiseAffectIncand";
   data.shortName = "ai_noise_affect_incand";
   helper.MakeInputBoolean(data);
   
   data.name = "aiNoiseAffectOpacity";
   data.shortName = "ai_noise_affect_opacity";
   helper.MakeInputBoolean(data);
   
   data.name = "aiVolumeNoise";
   data.shortName = "ai_volume_noise";
   helper.MakeInputNode(data);   
}

AtNode* CFluidTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("box");
}

AtArray* GetArray(AtNode* node, const char* paramName, unsigned int size, int type)
{
   AtArray* arr = AiNodeGetArray(node, paramName);
   if (arr == 0)
   {
      arr = AiArrayAllocate(size, 1, type);
      AiNodeSetArray(node, paramName, arr);
   }
   else if (arr->nelements != size)
   {
      arr = AiArrayAllocate(size, 1, type);
      AiNodeSetArray(node, paramName, arr);
   }
   return arr;
}

void ExportFloatGrid(AtNode* fluid, float* values, const char* paramName, unsigned int numVoxels)
{
   if (values == 0)
      return;
   AtArray* array = GetArray(fluid, paramName, numVoxels, AI_TYPE_FLOAT);
   for (unsigned int i = 0; i < numVoxels; ++i)
   {
      const float cVoxel = values[i];
      if (cVoxel < AI_EPSILON)
         AiArraySetFlt(array, i, 0.f);
      else
         AiArraySetFlt(array, i, cVoxel);
   }
}

void ExportFloatGradient(MPlug plug, AtNode* node, const char* paramName, int samplingResolution)
{
   MRampAttribute ramp(plug);
   AtArray* array = GetArray(node, paramName, samplingResolution, AI_TYPE_FLOAT);
   for (int i = 0; i < samplingResolution; ++i)
   {
      float v;
      ramp.getValueAtPosition((float)i / (float)(samplingResolution - 1), v);
      AiArraySetFlt(array, i, v);
   }
}

void ExportRGBGradient(MPlug plug, AtNode* node, const char* paramName, int samplingResolution)
{
   MRampAttribute ramp(plug);
   AtArray* array = GetArray(node, paramName, samplingResolution, AI_TYPE_RGB);
   for (int i = 0; i < samplingResolution; ++i)
   {
      MColor v;
      ramp.getColorAtPosition((float)i / (float)(samplingResolution - 1), v);
      AtRGB rgb = {(float)v.r, (float)v.g, (float)v.b};
      AiArraySetRGB(array, i, rgb);
   }
}

void CFluidTranslator::Export(AtNode* fluid)
{
   MFnFluid mayaFluid(GetMayaObject());
   MFnDependencyNode mayaFluidNode(GetMayaObject());
   
   ExportMatrix(fluid, 0);
   
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

   AiNodeSetPnt(fluid, "min", -0.5f * (float)xDim, -0.5f * (float)yDim, -0.5f * (float)zDim);
   AiNodeSetPnt(fluid, "max", 0.5f * (float)xDim, 0.5f * (float)yDim, 0.5f * (float)zDim);
   
   plug = FindMayaPlug("transparency");
   if (!plug.isNull())
      AiNodeSetRGB(fluid_shader, "transparency", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("aiPhaseFunc");
   if (!plug.isNull())
      AiNodeSetFlt(fluid_shader, "phase_func", plug.asFloat());
   
   
   plug = FindMayaPlug("aiVolumeNoise");
   if (!plug.isNull())
   {
      MPlugArray volumeNoisePlug;
      plug.connectedTo(volumeNoisePlug, true, false);
      if (volumeNoisePlug.length() > 0)
      {
         AtNode* volumeNoise = ExportRootShader(volumeNoisePlug[0]);
         AiNodeSetPtr(fluid_shader, "volume_noise", volumeNoise);
      }
   }

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
   
   ProcessParameter(fluid_shader, "color_texture", AI_TYPE_BOOLEAN, "colorTexture");
   ProcessParameter(fluid_shader, "incand_texture", AI_TYPE_BOOLEAN, "incandTexture");
   ProcessParameter(fluid_shader, "opacity_texture", AI_TYPE_BOOLEAN, "opacityTexture");
   
   ProcessParameter(fluid_shader, "texture_type", AI_TYPE_INT, "textureType");
   
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
   
   ProcessParameter(fluid_shader, "noise_affect_color", AI_TYPE_BOOLEAN, "aiNoiseAffectColor");
   ProcessParameter(fluid_shader, "noise_affect_incand", AI_TYPE_BOOLEAN, "aiNoiseAffectIncand");
   ProcessParameter(fluid_shader, "noise_affect_opacity", AI_TYPE_BOOLEAN, "aiNoiseAffectOpacity");
   
   AiNodeSetArray(fluid_shader, "matrix", AiArrayCopy(AiNodeGetArray(fluid, "matrix")));
   
   ProcessParameter(fluid, "self_shadows", AI_TYPE_BOOLEAN, "selfShadowing");
   ProcessParameter(fluid_shader, "shadow_opacity", AI_TYPE_FLOAT, "shadowOpacity");
   
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
         AtArray* array = GetArray(fluid_shader, "velocity", numVoxels, AI_TYPE_VECTOR);
         for (unsigned int i = 0; i < numVoxels; ++i)
         {
            AtVector cVector = {x[i], y[i], z[i]};
            cVector.x = cVector.x < AI_EPSILON ? 0.f : cVector.x;
            cVector.y = cVector.y < AI_EPSILON ? 0.f : cVector.y;
            cVector.z = cVector.z < AI_EPSILON ? 0.f : cVector.z;
            AiArraySetVec(array, i, cVector);
         }
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
         AtArray* array = GetArray(fluid_shader, "colors", numVoxels, AI_TYPE_RGB);
         for (unsigned int i = 0; i < numVoxels; ++i)
         {
            AtColor cColor = {r[i], g[i], b[i]};
            cColor.r = cColor.r < AI_EPSILON ? 0.f : cColor.r;
            cColor.g = cColor.g < AI_EPSILON ? 0.f : cColor.g;
            cColor.b = cColor.b < AI_EPSILON ? 0.f : cColor.b;
            AiArraySetRGB(array, i, cColor);
         }
      }
   }
}

void CFluidTranslator::ExportMotion(AtNode* fluid, unsigned int step)
{
   ExportMatrix(fluid, step);
   
   AtNode* fluid_shader = (AtNode*)AiNodeGetPtr(fluid, "shader");
   
   if (fluid_shader != 0)
   {
      AiNodeSetArray(fluid_shader, "matrix", AiArrayCopy(AiNodeGetArray(fluid, "matrix")));
   }
}
