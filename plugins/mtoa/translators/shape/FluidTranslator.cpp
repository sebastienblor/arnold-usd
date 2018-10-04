#include "FluidTranslator.h"
#include <maya/MFnFluid.h>
#include <maya/MRampAttribute.h>
#include <maya/MColor.h>
#include <maya/MPlugArray.h>

bool CFluidTranslator::RequiresMotionData(){return IsMotionBlurEnabled(MTOA_MBLUR_OBJECT);}
void CFluidTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper("fluidShape");
   
   CAttrData data;
   data.defaultValue.FLT() = 0.1f;
   data.hasMin = true;
   data.hasSoftMax = true;
   data.min.FLT() = 0.f;
   data.softMax.FLT() = 2.f;
   data.name = "aiStepSize";
   data.shortName = "ai_step_size";
   data.channelBox = false;
   data.keyable = false;       

   helper.MakeInputFloat(data);
   
   data.hasSoftMax = false;
   
   data.defaultValue.FLT() = 0.f;
   data.hasMax = true;
   data.name = "aiPhaseFunc";
   data.shortName = "aiPhaseFunc";
   
   data.min.FLT() = -1.f;
   data.max.FLT() = 1.f;
   helper.MakeInputFloat(data);
   
   data.hasMin = false;
   data.hasMax = false;
   
   data.name = "aiOverrideTextures";
   data.shortName = "ai_override_textures";
   data.defaultValue.BOOL() = false;
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

   data.name = "aiEnableDeformationBlur";
   data.shortName = "ai_enable_deformation_blur";
   data.defaultValue.BOOL() = false;
   helper.MakeInputBoolean(data);
   
   data.name = "aiMotionVectorScale";
   data.shortName = "ai_motion_vector_scale";
   data.defaultValue.FLT() = 1.f;
   data.hasMin = true;
   data.hasSoftMax = true;
   data.min.FLT() = 0.f;
   data.softMax.FLT() = 1.f;
   helper.MakeInputFloat(data);

   data.name = "aiVolumeTexture";
   data.shortName = "ai_volume_texture";
   helper.MakeInputRGB(data);   
   
   MStringArray strArr;
   strArr.append("Fixed");
   strArr.append("Grid");
   data.enums = strArr;
   data.defaultValue.INT() = 0;
   data.name = "aiTextureCoordinateMethod";
   data.shortName = "ai_texture_coordinate_method";
   helper.MakeInputEnum(data);
   
   strArr.clear();
   strArr.append("Closest");
   strArr.append("Linear");
   strArr.append("Cubic");
   data.enums = strArr;
   data.defaultValue.INT() = 1;
   data.name = "aiFilterType";
   data.shortName = "ai_filter_type";
   helper.MakeInputEnum(data);

   MakeArnoldVisibilityFlags(helper);
}

AtNode* CFluidTranslator::CreateArnoldNodes()
{
   AddArnoldNode("mayaFluid", "shader");
   AddArnoldNode("MayaFluidData", "data");
   return AddArnoldNode("box");
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

void CFluidTranslator::Export(AtNode* fluid)
{
   MFnFluid mayaFluid(GetMayaObject());
   MFnDependencyNode mayaFluidNode(GetMayaObject());
   
   ExportMatrix(fluid);
   AiNodeSetByte(fluid, "visibility", ComputeVisibility());
   ExportTraceSets(fluid, FindMayaPlug("aiTraceSets"));
   ProcessParameter(fluid, "receive_shadows", AI_TYPE_BOOLEAN, "receiveShadows");
   
   AtNode* fluid_shader = GetArnoldNode("shader");
   AiNodeSetPtr(fluid, "shader", fluid_shader);

   AtNode* fluid_data = GetArnoldNode("data");
   if (AiNodeLookUpUserParameter(fluid, "mtoa_fluid_data") == 0)
      AiNodeDeclare(fluid, "mtoa_fluid_data", "constant NODE");
   AiNodeSetPtr(fluid, "mtoa_fluid_data", fluid_data);
   
   float stepSize = 0.1f;
   MPlug plug = FindMayaPlug("aiStepSize");
   if (!plug.isNull())
      stepSize = plug.asFloat();
   AiNodeSetFlt(fluid, "step_size", stepSize);
   
   plug = FindMayaPlug("transparency");
   if (!plug.isNull())
      AiNodeSetRGB(fluid_shader, "transparency", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("aiPhaseFunc");
   if (!plug.isNull())
      AiNodeSetFlt(fluid_shader, "phase_func", plug.asFloat());
   
   MFnFluid::CoordinateMethod coordinateMode;
   
   mayaFluid.getCoordinateMode(coordinateMode);
   
   if (FindMayaPlug("aiOverrideTextures").asBool())   
   {
      plug = FindMayaPlug("aiVolumeTexture");
      if (!plug.isNull())
      {
         MPlugArray volumeNoisePlug;
         plug.connectedTo(volumeNoisePlug, true, false);
         if (volumeNoisePlug.length() > 0)
         {
            AtNode* volumeNoise = ExportConnectedNode(volumeNoisePlug[0]);
            AiNodeSetPtr(fluid_shader, "volume_texture", volumeNoise);
         }
      }
      
      ProcessParameter(fluid_shader, "texture_affect_color", AI_TYPE_BOOLEAN, "aiTextureAffectColor");
      ProcessParameter(fluid_shader, "texture_affect_incand", AI_TYPE_BOOLEAN, "aiTextureAffectIncand");
      ProcessParameter(fluid_shader, "texture_affect_opacity", AI_TYPE_BOOLEAN, "aiTextureAffectOpacity");
      ProcessParameter(fluid_shader, "coordinate_method", AI_TYPE_INT, "aiTextureCoordinateMethod");
   }
   else
   {
      ProcessParameter(fluid_shader, "color_texture", AI_TYPE_BOOLEAN, "colorTexture");
      ProcessParameter(fluid_shader, "incand_texture", AI_TYPE_BOOLEAN, "incandTexture");
      ProcessParameter(fluid_shader, "opacity_texture", AI_TYPE_BOOLEAN, "opacityTexture");

      ProcessParameter(fluid_shader, "texture_type", AI_TYPE_INT, "textureType");
      ProcessParameter(fluid_shader, "coordinate_method", AI_TYPE_INT, "coordinateMethod");

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

   const int colorGradientType = mayaFluidNode.findPlug("colorInput").asShort();

   ExportRGBGradient(mayaFluidNode.findPlug("color"), fluid_shader, "color_gradient");
   AiNodeSetInt(fluid_shader, "color_gradient_type", colorGradientType);
   AiNodeSetFlt(fluid_shader, "color_gradient_input_bias", mayaFluidNode.findPlug("colorInputBias").asFloat());
   
   const int incandescenceGradientType = mayaFluidNode.findPlug("incandescenceInput").asShort();
   ExportRGBGradient(mayaFluidNode.findPlug("incandescence"), fluid_shader, "incandescence_gradient");
   AiNodeSetInt(fluid_shader, "incandescence_gradient_type", incandescenceGradientType);
   AiNodeSetFlt(fluid_shader, "incandescence_gradient_input_bias", mayaFluidNode.findPlug("incandescenceInputBias").asFloat());   
   
   const int opacityGradientType = mayaFluidNode.findPlug("opacityInput").asShort();
   ExportFloatGradient(mayaFluidNode.findPlug("opacity"), fluid_shader, "opacity_gradient");
   AiNodeSetInt(fluid_shader, "opacity_gradient_type", opacityGradientType);
   AiNodeSetFlt(fluid_shader, "opacity_gradient_input_bias", mayaFluidNode.findPlug("opacityInputBias").asFloat());
   
   ProcessParameter(fluid, "self_shadows", AI_TYPE_BOOLEAN, "selfShadowing");
   ProcessParameter(fluid_shader, "shadow_opacity", AI_TYPE_FLOAT, "shadowOpacity");

   ProcessParameter(fluid_shader, "dropoff_shape", AI_TYPE_INT, "dropoffShape");
   ProcessParameter(fluid_shader, "edge_dropoff", AI_TYPE_FLOAT, "edgeDropoff");
   ProcessParameter(fluid_shader, "velocity_scale", AI_TYPE_VECTOR, "velocityScale");
   ProcessParameter(fluid_shader, "motion_vector_scale", AI_TYPE_FLOAT, "aiMotionVectorScale");
   
   if (RequiresMotionData())
   {
      double motionStart, motionEnd;
      GetSessionOptions().GetMotionRange(motionStart, motionEnd);
      AiNodeSetFlt(fluid, "motion_start", (float)motionStart);
      AiNodeSetFlt(fluid, "motion_end", (float)motionEnd);
   }

   // first getting a simple color information from the color gradient
   ProcessParameter(fluid_shader, "filter_type", AI_TYPE_INT, "aiFilterType");
   if (RequiresMotionData())
   {
      // only if deformation motion blur is enabled, translate the parameter "enableDeformationBlur"

      ProcessParameter(fluid_shader, "enable_deformation_blur", AI_TYPE_BOOLEAN, "aiEnableDeformationBlur");
      float mv_scale = AiNodeGetFlt(fluid_shader, "motion_vector_scale");
      double motion_start = 0.;
      double motion_end = 1.;
      // multiply the scale by the shutter length (ignoring shutter offset here...)
      GetSessionOptions().GetMotionRange(motion_start, motion_end);
      mv_scale *= (float)(motion_end - motion_start);
      
      AiNodeSetFlt(fluid_shader, "motion_vector_scale", mv_scale);
   } else
   {
      AiNodeSetBool(fluid_shader, "enable_deformation_blur", false);
   }
   
   // support for gradient mode  
   // Exporting fluid data to a shader attached to the shape
   // at the moment this is the best way to store data
   // instead on the shader   

   unsigned int xRes, yRes, zRes;
   double xDim, yDim, zDim;
   
   mayaFluid.getResolution(xRes, yRes, zRes);
   mayaFluid.getDimensions(xDim, yDim, zDim);
   plug = mayaFluid.findPlug("dynamicOffset");
   float dynOffX = plug.child(0).asFloat();
   float dynOffY = plug.child(1).asFloat();
   float dynOffZ = plug.child(2).asFloat();
   
   const AtVector mn (-0.5f * (float)xDim + dynOffX, -0.5f * (float)yDim + dynOffY, -0.5f * (float)zDim + dynOffZ);
   const AtVector mx (0.5f * (float)xDim + dynOffX, 0.5f * (float)yDim + dynOffY, 0.5f * (float)zDim + dynOffZ);

   AiNodeSetVec(fluid, "min", mn.x, mn.y, mn.z);
   AiNodeSetVec(fluid, "max", mx.x, mx.y, mx.z);

   AiNodeSetInt(fluid_data, "xres", xRes);
   AiNodeSetInt(fluid_data, "yres", yRes);
   AiNodeSetInt(fluid_data, "zres", zRes);
   
   AiNodeSetVec(fluid_data, "min", mn.x, mn.y, mn.z);
   AiNodeSetVec(fluid_data, "max", mx.x, mx.y, mx.z);

   MFnFluid::FluidMethod fluidMethod;
   MFnFluid::FluidGradient fluidGradient;

   const unsigned int numVoxels = xRes * yRes * zRes;

   ExportFloatGrid(fluid_shader, mayaFluid.falloff(), "falloff", numVoxels);
   
   mayaFluid.getDensityMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid_data, "density_method", "Gradient");
      SetContentsGradientMode(fluid_data, "density_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid_data, "density_method", "Grid");

   if (fluidMethod != MFnFluid::kZero)
      ExportFloatGrid(fluid_data, mayaFluid.density(), "density", numVoxels);
   
   mayaFluid.getFuelMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid_data, "fuel_method", "Gradient");
      SetContentsGradientMode(fluid_data, "fuel_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid_data, "fuel_method", "Grid");
   
   if (fluidMethod != MFnFluid::kZero)
      ExportFloatGrid(fluid_data, mayaFluid.fuel(), "fuel", numVoxels);
   
   mayaFluid.getTemperatureMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid_data, "temperature_method", "Gradient");
      SetContentsGradientMode(fluid_data, "temperature_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid_data, "temperature_method", "Grid");
   
   if (fluidMethod != MFnFluid::kZero)
      ExportFloatGrid(fluid_data, mayaFluid.temperature(), "temperature", numVoxels);
   
   //ExportFloatGrid(fluid_shader, mayaFluid.pressure(), "pressure", numVoxels); this is not even used in the shader
   
   mayaFluid.getVelocityMode(fluidMethod, fluidGradient);

   if (fluidMethod == MFnFluid::kGradient)
   {
      AiNodeSetStr(fluid_data, "velocity_method", "Gradient");
      SetContentsGradientMode(fluid_data, "velocity_gradient", fluidGradient);
   }
   else
      AiNodeSetStr(fluid_data, "velocity_method", "Grid");
   
   if (fluidMethod != MFnFluid::kZero)
   {
      // Velocity arrays are different than the other ones, as they store data per-face.
      // X array size is thus (xres + 1) * yres * zres
      // Y array size is xres * (yres + 1) * zres
      // and Z array size is xres * yres * (zres + 1)

      int xGridSize[3], yGridSize[3], zGridSize[3];
      xGridSize[0] = xRes + 1;
      xGridSize[1] = yRes;
      xGridSize[2] = zRes;

      yGridSize[0] = xRes;
      yGridSize[1] = yRes + 1;
      yGridSize[2] = zRes;

      zGridSize[0] = xRes;
      zGridSize[1] = yRes;
      zGridSize[2] = zRes + 1;

      // precompute xRes * yRes (will be used to recover the voxels index)
      // for each of the 3 arrays
      int XYRes[3];
      XYRes[0] = xGridSize[0] * xGridSize[1];
      XYRes[1] = yGridSize[0] * yGridSize[1];
      XYRes[2] = zGridSize[0] * zGridSize[1];


      // for now we compute the average density at the center of the voxel, 
      // and provide that value to the fluidData.
      // But it would be more accurate to store the data as it is, 
      // and then in MayaFluid use the corresponding per-face interpolation

      int velocityGridSize[3];
      mayaFluid.velocityGridSizes(velocityGridSize[0], velocityGridSize[1], velocityGridSize[2]);

      float* xVel; float* yVel; float* zVel;
      mayaFluid.getVelocity(xVel, yVel, zVel);
      // velocities seem to be in world space, so we shouldn't have to convert them

      unsigned int i = 0;

      if (xVel != 0 && yVel != 0 && zVel != 0)
      {
         AtArray* velArray = AiArrayAllocate(numVoxels, 1, AI_TYPE_VECTOR);

         for (unsigned int z = 0; z < zRes; ++z)
         {
            for (unsigned int y = 0; y < yRes; ++y)
            {
               for (unsigned int x = 0; x < xRes; ++x, ++i)
               {
                  // computing average velocity for output voxel "i" (x, y, z)
                  AtVector cVector (0.f, 0.f, 0.f);
                   //compute this voxel's coordinates for each of the X, Y, Z arrays
                  int lowCoords[3];
                  lowCoords[0] = x + y * xGridSize[0] + z * XYRes[0]; 
                  lowCoords[1] = x + y * yGridSize[0] + z * XYRes[1];
                  lowCoords[2] = x + y * zGridSize[0] + z * XYRes[2];

                  // Now compute the average velocity at the center of the voxel for 3 components :

                  // next X element in X direction is index + 1
                  cVector.x = (xVel[lowCoords[0]] + xVel[lowCoords[0]+ 1]) * 0.5f; 
                  // next Y element in Y direction is index + xRes
                  cVector.y = (yVel[lowCoords[1]] + yVel[lowCoords[1]+ xRes]) * 0.5f; 
                  // next Z element in Z direction is index + xRes * yRes (= XYRes[2])
                  cVector.z = (zVel[lowCoords[2]] + zVel[lowCoords[2]+ XYRes[2]]) * 0.5f; 

                  AiArraySetVec(velArray, i, cVector);
               }     
            }
         }
         AiNodeSetArray(fluid_data, "velocity", velArray);
      }
   }
   
   float* u; float* v; float* w;
   mayaFluid.getCoordinates(u, v, w);
   if (u != 0 && v != 0 && w != 0)
   {
      AtArray* array = AiArrayAllocate(numVoxels, 1, AI_TYPE_VECTOR);
      for (unsigned int i = 0; i < numVoxels; ++i)
      {
         AtVector cCoord (u[i], v[i], w[i]);
         AiArraySetVec(array, i, cCoord);
      }
      AiNodeSetArray(fluid_data, "coordinates", array);
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
         AiNodeSetArray(fluid_data, "colors", array);
      }
   }
}

void CFluidTranslator::ExportMotion(AtNode* fluid)
{
   ExportMatrix(fluid);
}
