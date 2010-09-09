
#include "ArnoldBarndoorShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldBarndoorShaderNode::id(ARNOLD_NODEID_BARNDOOR);

MObject CArnoldBarndoorShaderNode::s_barndoor_bottom_edge;
MObject CArnoldBarndoorShaderNode::s_barndoor_bottom_left;
MObject CArnoldBarndoorShaderNode::s_barndoor_bottom_right;
MObject CArnoldBarndoorShaderNode::s_barndoor_top_edge;
MObject CArnoldBarndoorShaderNode::s_barndoor_top_left;
MObject CArnoldBarndoorShaderNode::s_barndoor_top_right;
MObject CArnoldBarndoorShaderNode::s_barndoor_left_edge;
MObject CArnoldBarndoorShaderNode::s_barndoor_left_bottom;
MObject CArnoldBarndoorShaderNode::s_barndoor_left_top;
MObject CArnoldBarndoorShaderNode::s_barndoor_right_edge;
MObject CArnoldBarndoorShaderNode::s_barndoor_right_bottom;
MObject CArnoldBarndoorShaderNode::s_barndoor_right_top;

MObject CArnoldBarndoorShaderNode::s_OUT_colorR;
MObject CArnoldBarndoorShaderNode::s_OUT_colorG;
MObject CArnoldBarndoorShaderNode::s_OUT_colorB;
MObject CArnoldBarndoorShaderNode::s_OUT_color;
MObject CArnoldBarndoorShaderNode::s_OUT_transparencyR;
MObject CArnoldBarndoorShaderNode::s_OUT_transparencyG;
MObject CArnoldBarndoorShaderNode::s_OUT_transparencyB;
MObject CArnoldBarndoorShaderNode::s_OUT_transparency;

MStatus CArnoldBarndoorShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldBarndoorShaderNode::creator()
{
   return new CArnoldBarndoorShaderNode();
}

MStatus CArnoldBarndoorShaderNode::initialize()
{
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;

   s_barndoor_bottom_edge = nAttr.create("barndoor_bottom_edge", "bbe", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_bottom_edge);

   s_barndoor_bottom_left = nAttr.create("barndoor_bottom_left", "bbl", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_bottom_left);

   s_barndoor_bottom_right = nAttr.create("barndoor_bottom_right", "bbr", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_bottom_right);

   s_barndoor_top_edge = nAttr.create("barndoor_top_edge", "bte", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_top_edge);

   s_barndoor_top_left = nAttr.create("barndoor_top_left", "btl", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_top_left);

   s_barndoor_top_right = nAttr.create("barndoor_top_right", "btr", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_top_right);

   s_barndoor_left_edge = nAttr.create("barndoor_left_edge", "ble", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_left_edge);

   s_barndoor_left_bottom = nAttr.create("barndoor_left_bottom", "blb", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_left_bottom);

   s_barndoor_left_top = nAttr.create("barndoor_left_top", "blt", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_left_top);

   s_barndoor_right_edge = nAttr.create("barndoor_right_edge", "bre", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_right_edge);

   s_barndoor_right_bottom = nAttr.create("barndoor_right_bottom", "brb", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_right_bottom);

   s_barndoor_right_top = nAttr.create("barndoor_right_top", "brt", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_barndoor_right_top);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_barndoor_bottom_edge, s_OUT_color);
   attributeAffects(s_barndoor_bottom_left, s_OUT_color);
   attributeAffects(s_barndoor_bottom_right, s_OUT_color);
   attributeAffects(s_barndoor_top_edge, s_OUT_color);
   attributeAffects(s_barndoor_top_left, s_OUT_color);
   attributeAffects(s_barndoor_top_right, s_OUT_color);
   attributeAffects(s_barndoor_left_edge, s_OUT_color);
   attributeAffects(s_barndoor_left_bottom, s_OUT_color);
   attributeAffects(s_barndoor_left_top, s_OUT_color);
   attributeAffects(s_barndoor_right_edge, s_OUT_color);
   attributeAffects(s_barndoor_right_bottom, s_OUT_color);
   attributeAffects(s_barndoor_right_top, s_OUT_color);

   return MS::kSuccess;
}
