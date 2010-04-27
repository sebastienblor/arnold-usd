
#include "ArnoldWireframeShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldWireframeShaderNode::id(ARNOLD_NODEID_WIREFRAME);

MObject CArnoldWireframeShaderNode::s_edge_type;
MObject CArnoldWireframeShaderNode::s_fill_colorR;
MObject CArnoldWireframeShaderNode::s_fill_colorG;
MObject CArnoldWireframeShaderNode::s_fill_colorB;
MObject CArnoldWireframeShaderNode::s_fill_color;
MObject CArnoldWireframeShaderNode::s_line_colorR;
MObject CArnoldWireframeShaderNode::s_line_colorG;
MObject CArnoldWireframeShaderNode::s_line_colorB;
MObject CArnoldWireframeShaderNode::s_line_color;
MObject CArnoldWireframeShaderNode::s_line_width;
MObject CArnoldWireframeShaderNode::s_raster_space;

MObject CArnoldWireframeShaderNode::s_OUT_colorR;
MObject CArnoldWireframeShaderNode::s_OUT_colorG;
MObject CArnoldWireframeShaderNode::s_OUT_colorB;
MObject CArnoldWireframeShaderNode::s_OUT_color;
MObject CArnoldWireframeShaderNode::s_OUT_transparencyR;
MObject CArnoldWireframeShaderNode::s_OUT_transparencyG;
MObject CArnoldWireframeShaderNode::s_OUT_transparencyB;
MObject CArnoldWireframeShaderNode::s_OUT_transparency;

MStatus CArnoldWireframeShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldWireframeShaderNode::creator()
{
   return new CArnoldWireframeShaderNode();
}

MStatus CArnoldWireframeShaderNode::initialize()
{
   MFnEnumAttribute eAttr;
   MFnNumericAttribute  nAttr;

   MAKE_ENUM(s_edge_type, "edge_type", "et", 0, "wireframe", "edge_type");

   MAKE_COLOR(s_fill_color, "fill_color", "fc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_fill_color);

   MAKE_COLOR(s_line_color, "line_color", "lc", 0, 0, 0);
   MAKE_INPUT(nAttr, s_line_color);

   s_line_width = nAttr.create("line_width", "lw", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_line_width);

   s_raster_space = nAttr.create("raster_space", "rs", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_raster_space);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_edge_type, s_OUT_color);
   attributeAffects(s_fill_color, s_OUT_color);
   attributeAffects(s_line_color, s_OUT_color);
   attributeAffects(s_line_width, s_OUT_color);
   attributeAffects(s_raster_space, s_OUT_color);

   return MS::kSuccess;
}
