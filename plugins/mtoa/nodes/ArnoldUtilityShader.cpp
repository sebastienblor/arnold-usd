
#include "ArnoldUtilityShader.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldUtilityShaderNode::id(0x00072001);

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

#define MAKE_INPUT(attr, name) \
   attr.setKeyable(true); \
   attr.setStorable(true);	\
   attr.setReadable(true);	\
   attr.setWritable(true); \
   addAttribute(name)

#define MAKE_OUTPUT(attr, name) \
   attr.setKeyable(false); \
   attr.setStorable(false); \
   attr.setReadable(true); \
   attr.setWritable(false); \
   addAttribute(name)

#define MAKE_COLOR(attrib, name, shortname) \
   attrib##R = nAttr.create(name##"R", shortname##"r", MFnNumericData::kFloat, 1);\
   attrib##G = nAttr.create(name##"G", shortname##"g", MFnNumericData::kFloat, 1);\
   attrib##B = nAttr.create(name##"B", shortname##"b", MFnNumericData::kFloat, 1);\
   attrib = nAttr.create(name, shortname, attrib##R, attrib##G, attrib##B);\
   nAttr.setUsedAsColor(true);\
   nAttr.setDefault(1, 1, 1);\
   addAttribute(attrib##R);\
   addAttribute(attrib##G);\
   addAttribute(attrib##B);

MStatus CArnoldUtilityShaderNode::initialize()
{
   MFnNumericAttribute  nAttr;
   MFnEnumAttribute eAttr;

   MAKE_COLOR(s_color, "color", "col");
   MAKE_INPUT(nAttr, s_color);

   s_color_mode = eAttr.create("color_mode", "cm", 0);
   eAttr.addField("color", 0);
   eAttr.addField("ng", 1);
   eAttr.addField("n", 2);
   eAttr.addField("bary", 3);
   eAttr.addField("uv", 4);
   eAttr.addField("dpdu", 5);
   eAttr.addField("dpdv", 6);
   eAttr.addField("p", 7);
   eAttr.addField("prims", 8);
   eAttr.addField("wire", 9);
   eAttr.addField("polywire", 10);
   eAttr.addField("obj", 11);
   eAttr.addField("edgelength", 12);
   eAttr.addField("floatgrid", 13);
   addAttribute(s_color_mode);

   s_shade_mode = eAttr.create("shade_mode", "sm", 0);
   eAttr.addField("ndoteye", 0);
   eAttr.addField("lambert", 1);
   eAttr.addField("flat", 2);
   addAttribute(s_shade_mode);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc");
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot");
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_color, s_OUT_color);
   attributeAffects(s_color_mode, s_OUT_color);
   attributeAffects(s_shade_mode, s_OUT_color);

   return MS::kSuccess;
}
