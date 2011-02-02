
#include "ArnoldLightDecayShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldLightDecayShaderNode::id(ARNOLD_NODEID_LIGHT_DECAY);

MObject CArnoldLightDecayShaderNode::s_decay_clamp;
MObject CArnoldLightDecayShaderNode::s_decay_radius;
MObject CArnoldLightDecayShaderNode::s_decay_type;
MObject CArnoldLightDecayShaderNode::s_far_end;
MObject CArnoldLightDecayShaderNode::s_far_start;
MObject CArnoldLightDecayShaderNode::s_near_end;
MObject CArnoldLightDecayShaderNode::s_near_start;
MObject CArnoldLightDecayShaderNode::s_use_far_atten;
MObject CArnoldLightDecayShaderNode::s_use_near_atten;

MObject CArnoldLightDecayShaderNode::s_OUT_colorR;
MObject CArnoldLightDecayShaderNode::s_OUT_colorG;
MObject CArnoldLightDecayShaderNode::s_OUT_colorB;
MObject CArnoldLightDecayShaderNode::s_OUT_color;
MObject CArnoldLightDecayShaderNode::s_OUT_transparencyR;
MObject CArnoldLightDecayShaderNode::s_OUT_transparencyG;
MObject CArnoldLightDecayShaderNode::s_OUT_transparencyB;
MObject CArnoldLightDecayShaderNode::s_OUT_transparency;

MStatus CArnoldLightDecayShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldLightDecayShaderNode::creator()
{
   return new CArnoldLightDecayShaderNode();
}

MStatus CArnoldLightDecayShaderNode::initialize()
{
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;

   s_decay_clamp = nAttr.create("decay_clamp", "dclamp", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_decay_clamp);

   s_decay_radius = nAttr.create("decay_radius", "drad", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_decay_radius);

   MAKE_ENUM(s_decay_type, "decay_type", "dtype", 0, "light_decay", "decay_type");

   s_far_end = nAttr.create("far_end", "fend", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_far_end);

   s_far_start = nAttr.create("far_start", "fst", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_far_start);

   s_near_end = nAttr.create("near_end", "nend", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_near_end);

   s_near_start = nAttr.create("near_start", "nst", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_near_start);

   s_use_near_atten = nAttr.create("use_near_atten", "natt", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_use_near_atten);

   s_use_far_atten = nAttr.create("use_far_atten", "fatt", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_use_far_atten);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_decay_clamp, s_OUT_color);
   attributeAffects(s_decay_radius, s_OUT_color);
   attributeAffects(s_decay_type, s_OUT_color);
   attributeAffects(s_far_end, s_OUT_color);
   attributeAffects(s_far_start, s_OUT_color);
   attributeAffects(s_near_end, s_OUT_color);
   attributeAffects(s_near_start, s_OUT_color);
   attributeAffects(s_use_far_atten, s_OUT_color);
   attributeAffects(s_use_near_atten, s_OUT_color);

   return MS::kSuccess;
}
