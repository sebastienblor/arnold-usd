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
MObject CArnoldCurvesCollector::s_exportRefPoints;
MObject CArnoldCurvesCollector::s_minPixelWidth;

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
   nAttr.addToCategory("arnold");
   addAttribute(s_width);


   s_sampleRate = nAttr.create("aiSampleRate", "aiSampleRate", MFnNumericData::kInt, 5);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   nAttr.addToCategory("arnold");
   addAttribute(s_sampleRate);

   s_curveShader = nAttr.create("aiCurveShader", "aiCurveShader", MFnNumericData::k3Float, 0.f);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   nAttr.setConnectable(true);
   nAttr.setKeyable(true);

   addAttribute(s_curveShader);

   s_exportRefPoints = nAttr.create("aiExportRefPoints", "aiExportRefPoints", MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(s_exportRefPoints);

   s_minPixelWidth = nAttr.create("aiMinPixelWidth", "aiMinPixelWidth", MFnNumericData::kFloat, 0.f);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(s_minPixelWidth);
/*
   FIXME still missing the mode, do I have to re-create the enum manually here ?

   s_mode = eAttr.create("mode", "mode", 0);
   eAttr.addField("Bounding Box", DM_BOUNDING_BOX);
   eAttr.addField("Per Object Bounding Box", DM_PER_OBJECT_BOUNDING_BOX);
   eAttr.addField("Polywire", DM_POLYWIRE);
   eAttr.addField("Wireframe", DM_WIREFRAME);
   eAttr.addField("Point Cloud", DM_POINT_CLOUD);
   eAttr.addField("Shaded Polywire", DM_SHADED_POLYWIRE);
   eAttr.addField("Shaded", DM_SHADED);
   addAttribute(s_mode);
*/
   /*data.name = "widthProfile";
   data.shortName = "wdthP";
   helper.MakeInputCurveRamp(data);*/
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
