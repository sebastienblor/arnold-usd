#include "ArnoldCurveCollector.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "translators/DagTranslator.h"
#include "translators/shape/ShapeTranslator.h"
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

MTypeId CArnoldCurveCollector::id(ARNOLD_NODEID_CURVES_COLLECTOR);

CStaticAttrHelper CArnoldCurveCollector::s_attributes(CArnoldCurveCollector::addAttribute);

MObject CArnoldCurveCollector::s_width;
MObject CArnoldCurveCollector::s_sampleRate;
MObject CArnoldCurveCollector::s_curveShader;
MObject CArnoldCurveCollector::s_curveShaderR;
MObject CArnoldCurveCollector::s_curveShaderG;
MObject CArnoldCurveCollector::s_curveShaderB;
MObject CArnoldCurveCollector::s_exportRefPoints;
MObject CArnoldCurveCollector::s_minPixelWidth;
MObject CArnoldCurveCollector::s_mode;
MObject CArnoldCurveCollector::s_widthProfile;


void* CArnoldCurveCollector::creator()
{
   return new CArnoldCurveCollector();
}

MStatus CArnoldCurveCollector::initialize()
{
   s_attributes.SetNode("curves");

   CDagTranslator::MakeArnoldVisibilityFlags(s_attributes);

   MFnNumericAttribute nAttr;
   MFnEnumAttribute eAttr;

   s_width = nAttr.create("aiCurveWidth", "aiCurveWidth", MFnNumericData::kFloat, 0.01f);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   nAttr.setConnectable(true);
   nAttr.setKeyable(true);
   nAttr.setSoftMin(0.);
   nAttr.setSoftMax(1.);
   nAttr.setMin(0.);
   nAttr.addToCategory("arnold");
   addAttribute(s_width);


   s_sampleRate = nAttr.create("aiSampleRate", "aiSampleRate", MFnNumericData::kInt, 5);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   nAttr.setSoftMin(1);
   nAttr.setSoftMax(20);
   nAttr.setMin(1);
   nAttr.addToCategory("arnold");
   addAttribute(s_sampleRate);

   MAKE_COLOR(s_curveShader, "aiCurveShader", "aiCurveShader", 0, 0, 0);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setInternal(true);
   nAttr.setChannelBox(false);
   addAttribute(s_curveShader);

   s_exportRefPoints = nAttr.create("aiExportRefPoints", "aiExportRefPoints", MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(s_exportRefPoints);

   s_minPixelWidth = nAttr.create("aiMinPixelWidth", "aiMinPixelWidth", MFnNumericData::kFloat, 0.f);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(s_minPixelWidth);

   s_mode = eAttr.create("aiMode", "aiMode", 0);
   eAttr.addField("ribbon", 0);
   eAttr.addField("thick", 1);
   eAttr.addField("oriented", 2);
   addAttribute(s_mode);

   MObject selfShadows = nAttr.create("aiSelfShadows", "aiSelfShadows", MFnNumericData::kBoolean, 1);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(selfShadows);

   MObject opaque = nAttr.create("aiOpaque", "aiOpaque", MFnNumericData::kBoolean, 1);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(opaque);
   
   CAttrData data;
   data.stringDefault = "";
   data.name = "aiTraceSets";
   data.shortName = "trace_sets";
   data.type = AI_TYPE_STRING;   
   s_attributes.MakeInput(data);
   
   data.stringDefault = "";
   data.name = "aiSssSetname";
   data.shortName = "ai_sss_setname";
   data.type = AI_TYPE_STRING;
   s_attributes.MakeInput(data);


   data.stringDefault = "";
   data.name = "aiUserOptions";
   data.shortName = "user_options";
   data.type = AI_TYPE_STRING;   
   s_attributes.MakeInput(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiExportHairIDs";
   data.shortName = "ai_export_hair_ids";
   s_attributes.MakeInputBoolean(data);


   MRampAttribute rAttr;
   s_widthProfile = rAttr.createCurveRamp("aiWidthProfile", "wdthP");
   addAttribute(s_widthProfile);

   return MS::kSuccess;
}

void CArnoldCurveCollector::postConstructor()
{   
   // Call parent postConstructor as it is not done automatically as the parent constructor
   MPxLocatorNode::postConstructor();
   setMPSafe(true);

   // Need to set the default Curve Profile ramp, otherwise it will 
   // appear empty. Here we choose to add two points at 0 and 1 position,
   // both with a value of 1
   MStatus status;
   MRampAttribute widthProfileAttr(thisMObject(), s_widthProfile, &status );
   if (status == MS::kSuccess)
   {
      static const float sDefaultWidthRampPositions[] = {0.0f, 1.f};
      static const float sDefaultWidthRampValues[] = {1.f, 1.f};
      static const int sDefaultWidthRampInterps[] = {3,3};

      widthProfileAttr.setRamp(MFloatArray(sDefaultWidthRampValues, 2), 
            MFloatArray(sDefaultWidthRampPositions, 2), 
            MIntArray(sDefaultWidthRampInterps, 2));
   }
}

MStatus CArnoldCurveCollector::compute(const MPlug& plug, MDataBlock& block)
{
   return MS::kUnknownParameter;
}
