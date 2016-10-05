#include "ArnoldCurvesCollector.h"
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

MTypeId CArnoldCurvesCollector::id(ARNOLD_NODEID_CURVES_COLLECTOR);

CStaticAttrHelper CArnoldCurvesCollector::s_attributes(CArnoldCurvesCollector::addAttribute);

MObject CArnoldCurvesCollector::s_width;
MObject CArnoldCurvesCollector::s_sampleRate;
MObject CArnoldCurvesCollector::s_curveShader;
MObject CArnoldCurvesCollector::s_curveShaderR;
MObject CArnoldCurvesCollector::s_curveShaderG;
MObject CArnoldCurvesCollector::s_curveShaderB;
MObject CArnoldCurvesCollector::s_exportRefPoints;
MObject CArnoldCurvesCollector::s_minPixelWidth;
MObject CArnoldCurvesCollector::s_mode;


void* CArnoldCurvesCollector::creator()
{
   return new CArnoldCurvesCollector();
}

MStatus CArnoldCurvesCollector::initialize()
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
   nAttr.setChannelBox(true);
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
   addAttribute(s_mode);

   /*data.name = "widthProfile";
   data.shortName = "wdthP";
   helper.MakeInputCurveRamp(data);*/

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

   return MS::kSuccess;
}

void CArnoldCurvesCollector::postConstructor()
{
   // Call parent postConstructor as it is not done automatically as the parent constructor
   MPxLocatorNode::postConstructor();
   setMPSafe(true);
}

MStatus CArnoldCurvesCollector::compute(const MPlug& plug, MDataBlock& block)
{
   return MS::kUnknownParameter;
}
