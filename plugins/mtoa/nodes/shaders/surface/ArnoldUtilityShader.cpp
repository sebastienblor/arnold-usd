
#include "ArnoldUtilityShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldUtilityShaderNode::id(ARNOLD_NODEID_UTILITY);

MObject CArnoldUtilityShaderNode::s_colorR;
MObject CArnoldUtilityShaderNode::s_colorG;
MObject CArnoldUtilityShaderNode::s_colorB;
MObject CArnoldUtilityShaderNode::s_color;
MObject CArnoldUtilityShaderNode::s_color_mode;
MObject CArnoldUtilityShaderNode::s_shade_mode;
MObject CArnoldUtilityShaderNode::s_OUT_colorR;
MObject CArnoldUtilityShaderNode::s_OUT_colorG;
MObject CArnoldUtilityShaderNode::s_OUT_colorB;
MObject CArnoldUtilityShaderNode::s_OUT_color;
MObject CArnoldUtilityShaderNode::s_OUT_transparencyR;
MObject CArnoldUtilityShaderNode::s_OUT_transparencyG;
MObject CArnoldUtilityShaderNode::s_OUT_transparencyB;
MObject CArnoldUtilityShaderNode::s_OUT_transparency;

MStatus CArnoldUtilityShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldUtilityShaderNode::creator()
{
   return new CArnoldUtilityShaderNode();
}

MStatus CArnoldUtilityShaderNode::initialize()
{
   MFnNumericAttribute  nAttr;
   MFnEnumAttribute eAttr;

   MAKE_COLOR(s_color, "color", "col", 1, 1, 1);
   MAKE_INPUT(nAttr, s_color);

   MAKE_ENUM(s_color_mode, "color_mode", "cm", 0, "utility", "color_mode");

   MAKE_ENUM(s_shade_mode, "shade_mode", "sm", 0, "utility", "shade_mode");

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_color, s_OUT_color);
   attributeAffects(s_color_mode, s_OUT_color);
   attributeAffects(s_shade_mode, s_OUT_color);

   return MS::kSuccess;
}
