
#include "ArnoldBackgroundImageShader.h"
#include "ShaderUtils.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

MTypeId CArnoldBackgroundImageShaderNode::id(0x00072003);

MObject CArnoldBackgroundImageShaderNode::s_filename;
MObject CArnoldBackgroundImageShaderNode::s_OUT_colorR;
MObject CArnoldBackgroundImageShaderNode::s_OUT_colorG;
MObject CArnoldBackgroundImageShaderNode::s_OUT_colorB;
MObject CArnoldBackgroundImageShaderNode::s_OUT_color;
MObject CArnoldBackgroundImageShaderNode::s_OUT_transparencyR;
MObject CArnoldBackgroundImageShaderNode::s_OUT_transparencyG;
MObject CArnoldBackgroundImageShaderNode::s_OUT_transparencyB;
MObject CArnoldBackgroundImageShaderNode::s_OUT_transparency;

MStatus CArnoldBackgroundImageShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldBackgroundImageShaderNode::creator()
{
   return new CArnoldBackgroundImageShaderNode();
}

MStatus CArnoldBackgroundImageShaderNode::initialize()
{
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;

   s_filename = tAttr.create("filename", "file", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setStorable(true);
   tAttr.setConnectable(false);
   addAttribute(s_filename);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_filename, s_OUT_color);

   return MS::kSuccess;
}
