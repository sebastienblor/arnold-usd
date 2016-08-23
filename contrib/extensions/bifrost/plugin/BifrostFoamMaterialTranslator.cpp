#include "extension/Extension.h"
#include "utils/time.h"
#include "scene/MayaScene.h"

#include <maya/MFileObject.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>

#include "BifrostFoamMaterialTranslator.h"
#include "session/SessionOptions.h"



#include <string>
#include <vector>
#include <iostream>

void CBfFoamMaterialTranslator::ExportFloatGradient(MPlug plug, AtNode* node, const char* paramName)
{
   MIntArray plugArrayIndices;
   plug.getExistingArrayAttributeIndices(plugArrayIndices);
   unsigned int numElements = plugArrayIndices.length();
   AtArray* positions = AiArrayAllocate(numElements, 1, AI_TYPE_FLOAT);
   AtArray* values = AiArrayAllocate(numElements, 1, AI_TYPE_FLOAT);
   AtArray* interps = AiArrayAllocate(numElements, 1, AI_TYPE_INT);
   const MString positions_name = MString(paramName) + MString("_Position");
   const MString values_name = MString(paramName) + MString("_FloatValue");
   const MString interps_name = MString(paramName) + MString("_Interp");   
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

void CBfFoamMaterialTranslator::ExportRGBGradient(MPlug plug, AtNode* node, const char* paramName)
{
   MIntArray plugArrayIndices;
   plug.getExistingArrayAttributeIndices(plugArrayIndices);
   unsigned int numElements = plugArrayIndices.length();
   AtArray* positions = AiArrayAllocate(numElements, 1, AI_TYPE_FLOAT);
   AtArray* values = AiArrayAllocate(numElements, 1, AI_TYPE_RGB);
   AtArray* interps = AiArrayAllocate(numElements, 1, AI_TYPE_INT);
   const MString positions_name = MString(paramName) + MString("_Position");
   const MString values_name = MString(paramName) + MString("_Color");
   const MString interps_name = MString(paramName) + MString("_Interp");   
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
         AtNode* connectedColor = ExportConnectedNode(conns[0]);
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



AtNode* CBfFoamMaterialTranslator::CreateArnoldNodes()
{   
   return AddArnoldNode("bifrostFoamMaterial");
}

void CBfFoamMaterialTranslator::Export(AtNode* instance)
{
   MFnDependencyNode bfShaderNode(GetMayaObject());
   MPlug diffuseColor = bfShaderNode.findPlug("diffuseColor");
   
   AiNodeSetRGB(instance, "diffuseColor", diffuseColor.child(0).asFloat(), diffuseColor.child(1).asFloat(), diffuseColor.child(2).asFloat());
   AiNodeSetFlt(instance, "diffuseColorRemapInputMin", bfShaderNode.findPlug("diffuseColorRemapInputMin").asFloat());
   AiNodeSetFlt(instance, "diffuseColorRemapInputMax", bfShaderNode.findPlug("diffuseColorRemapInputMax").asFloat());
   AiNodeSetStr(instance, "diffuseColorRemapChannel", bfShaderNode.findPlug("diffuseColorRemapChannel").asString().asChar());
   ExportRGBGradient(bfShaderNode.findPlug("diffuseColorRemap"), instance, "diffuseColorRemap");
   ExportFloatGradient(bfShaderNode.findPlug("diffuseColorValueRemap"), instance, "diffuseColorValueRemap");

   MPlug scatterColor = bfShaderNode.findPlug("scatterColor");
   
   AiNodeSetBool(instance, "scatterUseDiffuseColor", bfShaderNode.findPlug("scatterUseDiffuseColor").asBool());
   AiNodeSetRGB(instance, "scatterColor", scatterColor.child(0).asFloat(), scatterColor.child(1).asFloat(), scatterColor.child(2).asFloat());
   AiNodeSetFlt(instance, "scatterWeight", bfShaderNode.findPlug("scatterWeight").asFloat());
   AiNodeSetFlt(instance, "scatterWeightRemapInputMin", bfShaderNode.findPlug("scatterWeightRemapInputMin").asFloat());
   AiNodeSetFlt(instance, "scatterWeightRemapInputMax", bfShaderNode.findPlug("scatterWeightRemapInputMax").asFloat());
   AiNodeSetStr(instance, "scatterWeightRemapChannel", bfShaderNode.findPlug("scatterWeightRemapChannel").asString().asChar());
   ExportFloatGradient(bfShaderNode.findPlug("scatterWeightRemap"), instance, "scatterWeightRemap");

   AiNodeSetBool(instance, "reflectionUseDiffuseColor", bfShaderNode.findPlug("reflectionUseDiffuseColor").asBool());
   MPlug reflectionColor = bfShaderNode.findPlug("reflectionColor");
   
   AiNodeSetRGB(instance, "reflectionColor", reflectionColor.child(0).asFloat(), reflectionColor.child(1).asFloat(), reflectionColor.child(2).asFloat());
   AiNodeSetFlt(instance, "reflectionWeight", bfShaderNode.findPlug("reflectionWeight").asFloat());
   AiNodeSetStr(instance, "reflectionWeightRemapChannel", bfShaderNode.findPlug("reflectionWeightRemapChannel").asString().asChar());
   AiNodeSetFlt(instance, "reflectionColorRemapInputMin", bfShaderNode.findPlug("reflectionColorRemapInputMin").asFloat());
   AiNodeSetFlt(instance, "reflectionColorRemapInputMax", bfShaderNode.findPlug("reflectionColorRemapInputMax").asFloat());
   ExportFloatGradient(bfShaderNode.findPlug("reflectionWeightRemap"), instance, "reflectionWeightRemap");

   MPlug transparencyColor = bfShaderNode.findPlug("transparencyColor");
   
   AiNodeSetRGB(instance, "transparencyColor", transparencyColor.child(0).asFloat(), transparencyColor.child(1).asFloat(), transparencyColor.child(2).asFloat());
   AiNodeSetFlt(instance, "transparencyWeight", bfShaderNode.findPlug("transparencyWeight").asFloat());
   AiNodeSetFlt(instance, "transparencyColorRemapInputMin", bfShaderNode.findPlug("transparencyColorRemapInputMin").asFloat());
   AiNodeSetFlt(instance, "transparencyColorRemapInputMax", bfShaderNode.findPlug("transparencyColorRemapInputMax").asFloat());
   ExportFloatGradient(bfShaderNode.findPlug("transparencyWeightRemap"), instance, "transparencyWeightRemap");
}

