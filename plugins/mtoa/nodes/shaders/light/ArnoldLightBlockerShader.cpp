
#include "ArnoldLightBlockerShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldLightBlockerShaderNode::id(ARNOLD_NODEID_LIGHT_BLOCKER);

MObject CArnoldLightBlockerShaderNode::s_density;
MObject CArnoldLightBlockerShaderNode::s_geometry_matrix;
MObject CArnoldLightBlockerShaderNode::s_geometry_type;
MObject CArnoldLightBlockerShaderNode::s_height_edge;
MObject CArnoldLightBlockerShaderNode::s_ramp;
MObject CArnoldLightBlockerShaderNode::s_ramp_axis;
MObject CArnoldLightBlockerShaderNode::s_roundness;
MObject CArnoldLightBlockerShaderNode::s_width_edge;

MObject CArnoldLightBlockerShaderNode::s_OUT_colorR;
MObject CArnoldLightBlockerShaderNode::s_OUT_colorG;
MObject CArnoldLightBlockerShaderNode::s_OUT_colorB;
MObject CArnoldLightBlockerShaderNode::s_OUT_color;
MObject CArnoldLightBlockerShaderNode::s_OUT_transparencyR;
MObject CArnoldLightBlockerShaderNode::s_OUT_transparencyG;
MObject CArnoldLightBlockerShaderNode::s_OUT_transparencyB;
MObject CArnoldLightBlockerShaderNode::s_OUT_transparency;

MStatus CArnoldLightBlockerShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldLightBlockerShaderNode::creator()
{
   return new CArnoldLightBlockerShaderNode();
}

MStatus CArnoldLightBlockerShaderNode::initialize()
{
   MFnEnumAttribute eAttr;
   MFnMatrixAttribute mAttr;
   MFnNumericAttribute nAttr;

   s_density = nAttr.create("density", "d", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_density);

   s_geometry_matrix = mAttr.create("geometry_matrix", "gmat", MFnMatrixAttribute::kFloat);
   MAKE_INPUT(mAttr, s_geometry_matrix);

   MAKE_ENUM(s_geometry_type, "geometry_type", "gtype", 0, "light_blocker", "geometry_type");

   s_height_edge = nAttr.create("height_edge", "hedge", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_height_edge);

   s_width_edge = nAttr.create("width_edge", "wedge", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_width_edge);

   s_ramp = nAttr.create("ramp", "ra", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_ramp);

   MAKE_ENUM(s_ramp_axis, "ramp_axis", "rax", 0, "light_blocker", "ramp_axis");

   s_roundness = nAttr.create("roundness", "ro", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_roundness);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_density, s_OUT_color);
   attributeAffects(s_geometry_matrix, s_OUT_color);
   attributeAffects(s_geometry_type, s_OUT_color);
   attributeAffects(s_height_edge, s_OUT_color);
   attributeAffects(s_ramp, s_OUT_color);
   attributeAffects(s_ramp_axis, s_OUT_color);
   attributeAffects(s_roundness, s_OUT_color);
   attributeAffects(s_width_edge, s_OUT_color);

   return MS::kSuccess;
}
