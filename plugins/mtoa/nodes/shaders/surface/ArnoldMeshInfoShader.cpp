
#include "ArnoldMeshInfoShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

MTypeId CArnoldMeshInfoShaderNode::id(ARNOLD_NODEID_MESH_INFO);

MObject CArnoldMeshInfoShaderNode::s_what;
MObject CArnoldMeshInfoShaderNode::s_colorSetName;
MObject CArnoldMeshInfoShaderNode::s_OUT_colorR;
MObject CArnoldMeshInfoShaderNode::s_OUT_colorG;
MObject CArnoldMeshInfoShaderNode::s_OUT_colorB;
MObject CArnoldMeshInfoShaderNode::s_OUT_color;
MObject CArnoldMeshInfoShaderNode::s_OUT_transparencyR;
MObject CArnoldMeshInfoShaderNode::s_OUT_transparencyG;
MObject CArnoldMeshInfoShaderNode::s_OUT_transparencyB;
MObject CArnoldMeshInfoShaderNode::s_OUT_transparency;

MStatus CArnoldMeshInfoShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldMeshInfoShaderNode::creator()
{
   return new CArnoldMeshInfoShaderNode();
}

MStatus CArnoldMeshInfoShaderNode::initialize()
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
