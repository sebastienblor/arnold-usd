
#include "ArnoldUtility2Shader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

MTypeId CArnoldUtility2ShaderNode::id(ARNOLD_NODEID_UTILITY2);

MObject CArnoldUtility2ShaderNode::s_what;
MObject CArnoldUtility2ShaderNode::s_colorSetName;
MObject CArnoldUtility2ShaderNode::s_OUT_colorR;
MObject CArnoldUtility2ShaderNode::s_OUT_colorG;
MObject CArnoldUtility2ShaderNode::s_OUT_colorB;
MObject CArnoldUtility2ShaderNode::s_OUT_color;
MObject CArnoldUtility2ShaderNode::s_OUT_transparencyR;
MObject CArnoldUtility2ShaderNode::s_OUT_transparencyG;
MObject CArnoldUtility2ShaderNode::s_OUT_transparencyB;
MObject CArnoldUtility2ShaderNode::s_OUT_transparency;

MStatus CArnoldUtility2ShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldUtility2ShaderNode::creator()
{
   return new CArnoldUtility2ShaderNode();
}

MStatus CArnoldUtility2ShaderNode::initialize()
{
   MFnNumericAttribute  nAttr;
   MFnEnumAttribute eAttr;
   MFnTypedAttribute tAttr;

   //MAKE_ENUM(s_what, "what", "wht", 0, "utility2", "what");
   // The above doesn't seem to work... any idea?
   s_what = eAttr.create("what", "wht", 0);
   eAttr.addField("tangent", 0);
   eAttr.addField("bitangent", 1);
   eAttr.addField("color", 2);
   eAttr.setStorable(true);
   eAttr.setReadable(true);
   eAttr.setWritable(true);
   addAttribute(s_what);

   s_colorSetName = tAttr.create("colorSetName", "csn", MFnData::kString);
   MAKE_INPUT(tAttr, s_colorSetName);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_what, s_OUT_color);
   attributeAffects(s_colorSetName, s_OUT_color);

   return MS::kSuccess;
}
