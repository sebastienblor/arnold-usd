//#include "extension/Extension.h"

//#include "utils/time.h"

#include <maya/MFileObject.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>

#include "BifrostAeroMaterialTranslator.h"



#include <string>
#include <vector>
#include <iostream>

void CBfAeroMaterialTranslator::ExportFloatGradient(MPlug plug, AtNode* node, const char* paramName)
{
   MIntArray plugArrayIndices;
   plug.getExistingArrayAttributeIndices(plugArrayIndices);
   unsigned int numElements = plugArrayIndices.length();
   AtArray* positions = AiArrayAllocate(numElements, 1, AI_TYPE_FLOAT);
   AtArray* values = AiArrayAllocate(numElements, 1, AI_TYPE_FLOAT);
   AtArray* interps = AiArrayAllocate(numElements, 1, AI_TYPE_INT);
   const MString positions_name = MString(paramName) + MString("_position");
   const MString values_name = MString(paramName) + MString("_floatValue");
   const MString interps_name = MString(paramName) + MString("_interp");   
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

void CBfAeroMaterialTranslator::ExportRGBGradient(MPlug plug, AtNode* node, const char* paramName)
{
   MIntArray plugArrayIndices;
   plug.getExistingArrayAttributeIndices(plugArrayIndices);
   unsigned int numElements = plugArrayIndices.length();
   AtArray* positions = AiArrayAllocate(numElements, 1, AI_TYPE_FLOAT);
   AtArray* values = AiArrayAllocate(numElements, 1, AI_TYPE_RGB);
   AtArray* interps = AiArrayAllocate(numElements, 1, AI_TYPE_INT);
   const MString positions_name = MString(paramName) + MString("_position");
   const MString values_name = MString(paramName) + MString("_color");
   const MString interps_name = MString(paramName) + MString("_interp");   
   // check for the existing links, and unlink them
   // this is required to be able to change the connections in ipr
   AtArray* valuesOld = AiNodeGetArray(node, values_name.asChar());
   unsigned nelements = AiArrayGetNumElements(valuesOld);
   for (AtUInt32 i = 0; i < nelements; i++)
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
         AtRGB color = AtRGB(colorPlug.child(0).asFloat(),
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



AtNode* CBfAeroMaterialTranslator::CreateArnoldNodes()
{   
   return AddArnoldNode("bifrostAeroMaterial");
}

void CBfAeroMaterialTranslator::Export(AtNode* instance)
{
   MFnDependencyNode bfShaderNode(GetMayaObject());



///////////////////////////////////
   AiNodeSetFlt(instance, "densityScale", bfShaderNode.findPlug("densityScale").asFloat());
   AiNodeSetFlt(instance, "densityThreshold", bfShaderNode.findPlug("densityThreshold").asFloat());
   MPlug emissionColor = bfShaderNode.findPlug("emissionColor");
   AiNodeSetRGB(instance, "emissionColor", emissionColor.child(0).asFloat(), emissionColor.child(1).asFloat(), emissionColor.child(2).asFloat());
   AiNodeSetFlt(instance, "emissionIntensity", bfShaderNode.findPlug("emissionIntensity").asFloat());
   AiNodeSetFlt(instance, "emissionOffset", bfShaderNode.findPlug("emissionOffset").asFloat());

   MPlug absorbColor = bfShaderNode.findPlug("absorptionColor");
   AiNodeSetRGB(instance, "absorptionColor", absorbColor.child(0).asFloat(), absorbColor.child(1).asFloat(), absorbColor.child(2).asFloat());

   AiNodeSetFlt(instance, "absorptionIntensity", bfShaderNode.findPlug("absorptionIntensity").asFloat());
   AiNodeSetFlt(instance, "absorptionOffset", bfShaderNode.findPlug("absorptionOffset").asFloat());
   AiNodeSetFlt(instance, "shadowOpacityScale", bfShaderNode.findPlug("shadowOpacityScale").asFloat());
   
   MPlug scatteringColor = bfShaderNode.findPlug("scatteringColor");
   AiNodeSetRGB(instance, "scatteringColor", scatteringColor.child(0).asFloat(), 
      scatteringColor.child(1).asFloat(), scatteringColor.child(2).asFloat());
   AiNodeSetFlt(instance, "scatteringIntensity", bfShaderNode.findPlug("scatteringIntensity").asFloat());
   AiNodeSetFlt(instance, "scatteringOffset", bfShaderNode.findPlug("scatteringOffset").asFloat());
   AiNodeSetFlt(instance, "scatteringDensityCutoff", bfShaderNode.findPlug("scatteringDensityCutoff").asFloat());
   AiNodeSetFlt(instance, "scatteringDirectionality", bfShaderNode.findPlug("scatteringDirectionality").asFloat());

   AiNodeSetFlt(instance, "aiStepSize", bfShaderNode.findPlug("aiStepSize").asFloat());
   AiNodeSetInt(instance, "aiMaxSteps", bfShaderNode.findPlug("aiMaxSteps").asInt());
   AiNodeSetBool(instance, "aiShadowing", bfShaderNode.findPlug("aiShadowing").asBool());

   AiNodeSetFlt(instance, "aiShadowingStepSize", bfShaderNode.findPlug("aiShadowingStepSize").asFloat());
   AiNodeSetInt(instance, "aiShadowingMaxSteps", bfShaderNode.findPlug("aiShadowingMaxSteps").asInt());

   AiNodeSetStr(instance, "densityRemapChannel", bfShaderNode.findPlug("densityRemapChannel").asString().asChar());
   ExportFloatGradient(bfShaderNode.findPlug("densityRemap"), instance, "densityRemap");
   AiNodeSetFlt(instance, "densityRemapInputMin", bfShaderNode.findPlug("densityRemapInputMin").asFloat());
   AiNodeSetFlt(instance, "densityRemapInputMax", bfShaderNode.findPlug("densityRemapInputMax").asFloat());


   AiNodeSetStr(instance, "emissionColorRemapChannel", bfShaderNode.findPlug("emissionColorRemapChannel").asString().asChar());
   ExportRGBGradient(bfShaderNode.findPlug("emissionColorRemap"), instance, "emissionColorRemap");
   AiNodeSetFlt(instance, "emissionColorRemapInputMin", bfShaderNode.findPlug("emissionColorRemapInputMin").asFloat());
   AiNodeSetFlt(instance, "emissionColorRemapInputMax", bfShaderNode.findPlug("emissionColorRemapInputMax").asFloat());

  
}


void CBfAeroMaterialTranslator::NodeInitializer(CAbTranslator context)
{

   CExtensionAttrHelper helper(context.maya, "standard");

   CAttrData data;

   data.name = "aiStepSize";
   data.shortName = "ai_step_size";
   data.hasSoftMin = true;
   data.softMin.FLT() = 0.f;
   data.hasSoftMax = true;
   data.softMax.FLT() = 1.f;
   data.hasMin = true;
   data.min.FLT() = 0.f;
   data.hasMax = false;
   data.defaultValue.FLT() = 0.1f;
   helper.MakeInputFloat(data);


   data.name = "aiMaxSteps";
   data.shortName = "ai_max_steps";
   data.hasMin = true;
   data.min.INT() = 1;
   data.hasMax = false;
   data.hasSoftMin = true;
   data.softMin.INT() = 1;
   data.hasSoftMax = true;
   data.softMax.INT() = 1000000;
   data.defaultValue.INT() = 1000;
   helper.MakeInputInt(data);


   data.name = "aiShadowing";
   data.shortName = "ai_shadowing";
   data.defaultValue.BOOL() = true;
   helper.MakeInputBoolean(data);


   data.name = "aiShadowingStepSize";
   data.shortName = "ai_shadowing_step_size";
   data.hasSoftMin = true;
   data.softMin.FLT() = 0.f;
   data.hasSoftMax = true;
   data.softMax.FLT() = 5.f;
   data.hasMin = true;
   data.min.FLT() = 0.f;
   data.hasMax = false;
   data.defaultValue.FLT() = 1.1f;
   helper.MakeInputFloat(data);


   data.name = "aiShadowingMaxSteps";
   data.shortName = "ai_shadowing_max_steps";
   data.hasMin = true;
   data.min.INT() = 1;
   data.hasMax = false;
   data.hasSoftMin = true;
   data.softMin.INT() = 1;
   data.hasSoftMax = true;
   data.softMax.INT() = 1000000;
   data.defaultValue.INT() = 100;
   helper.MakeInputInt(data);
}
