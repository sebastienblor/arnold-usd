
#include "ArnoldBSSRDFShader.h"
#include "ShaderUtils.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldBSSRDFShaderNode::id(0x00072009);

MObject CArnoldBSSRDFShaderNode::s_colorR;
MObject CArnoldBSSRDFShaderNode::s_colorG;
MObject CArnoldBSSRDFShaderNode::s_colorB;
MObject CArnoldBSSRDFShaderNode::s_color;
MObject CArnoldBSSRDFShaderNode::s_radiusR;
MObject CArnoldBSSRDFShaderNode::s_radiusG;
MObject CArnoldBSSRDFShaderNode::s_radiusB;
MObject CArnoldBSSRDFShaderNode::s_radius;

MObject CArnoldBSSRDFShaderNode::s_OUT_colorR;
MObject CArnoldBSSRDFShaderNode::s_OUT_colorG;
MObject CArnoldBSSRDFShaderNode::s_OUT_colorB;
MObject CArnoldBSSRDFShaderNode::s_OUT_color;
MObject CArnoldBSSRDFShaderNode::s_OUT_transparencyR;
MObject CArnoldBSSRDFShaderNode::s_OUT_transparencyG;
MObject CArnoldBSSRDFShaderNode::s_OUT_transparencyB;
MObject CArnoldBSSRDFShaderNode::s_OUT_transparency;

MStatus CArnoldBSSRDFShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldBSSRDFShaderNode::creator()
{
   return new CArnoldBSSRDFShaderNode();
}

MStatus CArnoldBSSRDFShaderNode::initialize()
{
   MFnNumericAttribute  nAttr;

   MAKE_COLOR(s_color, "color", "c", 1, 1, 1);
   MAKE_INPUT(nAttr, s_color);

   MAKE_COLOR(s_radius, "radius", "r", 0.1f, 0.1f, 0.1f);
   MAKE_INPUT(nAttr, s_radius);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_color, s_OUT_color);
   attributeAffects(s_radius, s_OUT_color);

   return MS::kSuccess;
}
