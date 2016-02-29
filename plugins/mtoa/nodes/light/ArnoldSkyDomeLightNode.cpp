#include "ArnoldSkyDomeLightNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <ai_ray.h>
#include <ai_shader_util.h>

#include <ai_metadata.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFloatVector.h>

MTypeId CArnoldSkyDomeLightNode::id(ARNOLD_NODEID_SKYDOME_LIGHT);

CStaticAttrHelper CArnoldSkyDomeLightNode::s_attributes(CArnoldSkyDomeLightNode::addAttribute);

// Inputs
MObject CArnoldSkyDomeLightNode::s_intensity;
// MObject CArnoldSkyDomeLightNode::s_exposure;
// MObject CArnoldSkyDomeLightNode::s_castShadows;
// MObject CArnoldSkyDomeLightNode::s_samples;
// MObject CArnoldSkyDomeLightNode::s_normalize;
MObject CArnoldSkyDomeLightNode::s_affectDiffuse;
MObject CArnoldSkyDomeLightNode::s_affectSpecular;
// Arnold outputs
MObject CArnoldSkyDomeLightNode::s_OUT_colorR;
MObject CArnoldSkyDomeLightNode::s_OUT_colorG;
MObject CArnoldSkyDomeLightNode::s_OUT_colorB;
MObject CArnoldSkyDomeLightNode::s_OUT_color;
MObject CArnoldSkyDomeLightNode::s_OUT_transparencyR;
MObject CArnoldSkyDomeLightNode::s_OUT_transparencyG;
MObject CArnoldSkyDomeLightNode::s_OUT_transparencyB;
MObject CArnoldSkyDomeLightNode::s_OUT_transparency;
// Maya specific intputs
MObject CArnoldSkyDomeLightNode::s_pointCamera;
MObject CArnoldSkyDomeLightNode::s_normalCamera;
// Maya specific Outputs
MObject CArnoldSkyDomeLightNode::aLightDirection;
MObject CArnoldSkyDomeLightNode::aLightIntensity;
MObject CArnoldSkyDomeLightNode::aLightAmbient;
MObject CArnoldSkyDomeLightNode::aLightDiffuse;
MObject CArnoldSkyDomeLightNode::aLightSpecular;
MObject CArnoldSkyDomeLightNode::aLightShadowFraction;
MObject CArnoldSkyDomeLightNode::aPreShadowIntensity;
MObject CArnoldSkyDomeLightNode::aLightBlindData;
MObject CArnoldSkyDomeLightNode::aLightData;

void* CArnoldSkyDomeLightNode::creator()
{
   return new CArnoldSkyDomeLightNode();
}

MStatus CArnoldSkyDomeLightNode::initialize()
{
   MPxNode::inheritAttributesFrom("SphereLocator");

   MFnNumericAttribute nAttr;
   MFnLightDataAttribute lAttr;

   s_attributes.SetNode("skydome_light");
   s_attributes.MakeInput("resolution");

   //s_attributes.MakeMatrixInput(s_matrix, "matrix");

   s_intensity = s_attributes.MakeInput("intensity");

   // Metadata must be present to get it as Maya attributes emitDiffuse and emitSpecular
   s_affectDiffuse = s_attributes.MakeInput("affect_diffuse");
   s_affectDiffuse = s_attributes.MakeInput("affect_specular");

   // Removed so they are added as dynamic and have same ai prefix as other lights will
   // s_castShadows = s_attributes.MakeInput("cast_shadows");
   // s_exposure = s_attributes.MakeInput("exposure");
   // s_samples = s_attributes.MakeInput("samples");
   // s_normalize = s_attributes.MakeInput("normalize");
   // s_attributes.MakeInput("bounce_factor");
   // s_attributes.MakeInput("bounces");

   // arrays
   // TODO: use metdata to rename this attribute to light_filters
   s_attributes.MakeInput("filters");
   s_attributes.MakeInput("time_samples");



   // MAYA SPECIFIC INPUTS
   s_pointCamera = nAttr.createPoint("pointCamera", "p");
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_pointCamera);

   s_normalCamera = nAttr.createPoint("normalCamera", "n");
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_normalCamera);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "ocl", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // MAYA SPECIFIC OUTPUTS

   aLightDirection = nAttr.createPoint("lightDirection", "ld");
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(-1.0f, 0.0f, 0.0f);

   aLightIntensity = nAttr.createColor("lightIntensity", "li");
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(1.0f, 0.5f, 0.2f);

   aLightAmbient = nAttr.create("lightAmbient", "la", MFnNumericData::kBoolean);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(true);

   aLightDiffuse = nAttr.create("lightDiffuse", "ldf", MFnNumericData::kBoolean);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(true);

   aLightSpecular = nAttr.create("lightSpecular", "ls", MFnNumericData::kBoolean);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(true);

   aLightShadowFraction = nAttr.create("lightShadowFraction", "lsf", MFnNumericData::kFloat);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(0.0f);

   aPreShadowIntensity = nAttr.create("preShadowIntensity", "psi", MFnNumericData::kFloat);
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   nAttr.setDefault(0.0f);

   aLightBlindData = nAttr.createAddr("lightBlindData", "lbld");
   nAttr.setStorable(false);
   nAttr.setHidden(true);
   nAttr.setReadable(true);
   nAttr.setWritable(false);

   aLightData = lAttr.create("lightData", "ltd",
                              aLightDirection, aLightIntensity,
                              aLightAmbient, aLightDiffuse, aLightSpecular,
                              aLightShadowFraction, aPreShadowIntensity,
                              aLightBlindData);
   nAttr.setReadable(true);
   nAttr.setWritable(false);
   lAttr.setStorable(false);
   lAttr.setHidden(true);
   lAttr.setDefault(-1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f,
                     true, true, true, 0.0f, 1.0f, NULL);

   addAttribute(aLightData);

   attributeAffects(s_pointCamera, aLightData);
   attributeAffects(s_normalCamera, aLightData);

   attributeAffects (aLightIntensity, aLightData);
   attributeAffects (aLightDirection, aLightData);
   attributeAffects (aLightAmbient, aLightData);
   attributeAffects (aLightDiffuse, aLightData);
   attributeAffects (aLightSpecular, aLightData);
   attributeAffects (aLightShadowFraction, aLightData);
   attributeAffects (aPreShadowIntensity, aLightData);
   attributeAffects (aLightBlindData, aLightData);
   attributeAffects (aLightData, aLightData);

   attributeAffects(s_color, aLightData);
   attributeAffects(s_intensity, aLightData);
   // attributeAffects(s_exposure, aLightData);
   attributeAffects(s_format, aLightData);
   // attributeAffects(s_castShadows, aLightData);
   // attributeAffects(s_samples, aLightData);
   // attributeAffects(s_normalize, aLightData);
   attributeAffects(s_affectDiffuse, aLightData);
   attributeAffects(s_affectSpecular, aLightData);

   return MS::kSuccess;
}

MStatus CArnoldSkyDomeLightNode::compute(const MPlug& plug, MDataBlock& block)
{
   if ((plug != aLightData) && (plug.parent() != aLightData))
      return MS::kUnknownParameter;

   MFloatVector resultColor;

   // Real user input
   MFloatVector& lightColor = block.inputValue(s_color).asFloatVector();
   float lightIntensity = block.inputValue(s_intensity).asFloat();

   // Components to build LightData
   // MFloatVector& pointCamera = block.inputValue(s_pointCamera).asFloatVector();
   MFloatVector& normalCamera = block.inputValue(s_normalCamera).asFloatVector();
   MFloatVector lightDirection = normalCamera;
   bool affectAmbient = false;
   bool affectDiffuse = true; // block.inputValue(s_affectDiffuse).asBool();
   bool affectSpecular = false; // block.inputValue(s_affectSpecular).asBool();

   // TODO: exposure
   resultColor = lightColor * lightIntensity;

   // set ouput color attribute
   MDataHandle outLightDataHandle = block.outputValue(aLightData);

   MFloatVector& outIntensity = outLightDataHandle.child(aLightIntensity).asFloatVector();
   outIntensity = resultColor;

   MFloatVector& outDirection = outLightDataHandle.child(aLightDirection).asFloatVector();
   outDirection = lightDirection;

   bool& outAmbient = outLightDataHandle.child(aLightAmbient).asBool();
   outAmbient = affectAmbient;
   bool& outDiffuse = outLightDataHandle.child(aLightDiffuse).asBool();
   outDiffuse = affectDiffuse;
   bool& outSpecular = outLightDataHandle.child(aLightSpecular).asBool();
   outSpecular = affectSpecular;

   float& outSFraction = outLightDataHandle.child(aLightShadowFraction).asFloat();
   outSFraction = 1.0f;

   float& outPSIntensity = outLightDataHandle.child(aPreShadowIntensity).asFloat();
   outPSIntensity = (resultColor[0] + resultColor[1] + resultColor[2]) / 3.0f;

   void*& outBlindData = outLightDataHandle.child(aLightBlindData).asAddr();
   outBlindData = NULL;

   outLightDataHandle.setClean();


    return MS::kSuccess;
}

void CArnoldSkyDomeLightNode::draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
   if ((view.objectDisplay() & M3dView::kDisplayLights) == 0) return;
   CSphereLocator::draw(view, DGpath, style, status);
}

#ifdef ENABLE_VP2
/* override */
MSelectionMask CArnoldSkyDomeLightNode::getShapeSelectionMask() const
//
// Description
//     This method is overriden to support interactive object selection in Viewport 2.0
//
// Returns
//
//    The selection mask of the shape
//
{
	MSelectionMask::SelectionType selType = MSelectionMask::kSelectMeshes;
    return MSelectionMask( selType );
}
#endif
