
#include "ArnoldSkyShader.h"
#include "ShaderUtils.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

MTypeId CArnoldSkyShaderNode::id(0x00072004);

MObject CArnoldSkyShaderNode::s_XX;
MObject CArnoldSkyShaderNode::s_XY;
MObject CArnoldSkyShaderNode::s_XZ;
MObject CArnoldSkyShaderNode::s_X;
MObject CArnoldSkyShaderNode::s_X_angle;
MObject CArnoldSkyShaderNode::s_YX;
MObject CArnoldSkyShaderNode::s_YY;
MObject CArnoldSkyShaderNode::s_YZ;
MObject CArnoldSkyShaderNode::s_Y;
MObject CArnoldSkyShaderNode::s_Y_angle;
MObject CArnoldSkyShaderNode::s_ZX;
MObject CArnoldSkyShaderNode::s_ZY;
MObject CArnoldSkyShaderNode::s_ZZ;
MObject CArnoldSkyShaderNode::s_Z;
MObject CArnoldSkyShaderNode::s_Z_angle;
MObject CArnoldSkyShaderNode::s_colorR;
MObject CArnoldSkyShaderNode::s_colorG;
MObject CArnoldSkyShaderNode::s_colorB;
MObject CArnoldSkyShaderNode::s_color;
MObject CArnoldSkyShaderNode::s_intensity;
MObject CArnoldSkyShaderNode::s_format;
MObject CArnoldSkyShaderNode::s_opaque_alpha;
MObject CArnoldSkyShaderNode::s_visible;

MObject CArnoldSkyShaderNode::s_OUT_colorR;
MObject CArnoldSkyShaderNode::s_OUT_colorG;
MObject CArnoldSkyShaderNode::s_OUT_colorB;
MObject CArnoldSkyShaderNode::s_OUT_color;
MObject CArnoldSkyShaderNode::s_OUT_transparencyR;
MObject CArnoldSkyShaderNode::s_OUT_transparencyG;
MObject CArnoldSkyShaderNode::s_OUT_transparencyB;
MObject CArnoldSkyShaderNode::s_OUT_transparency;

MStatus CArnoldSkyShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldSkyShaderNode::creator()
{
   return new CArnoldSkyShaderNode();
}

MStatus CArnoldSkyShaderNode::initialize()
{
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;

   MAKE_VECTOR(s_X, "X", "x", 1.0f, 0, 0);
   MAKE_INPUT(nAttr, s_X);

   MAKE_VECTOR(s_Y, "Y", "y", 0, 1.0f, 0);
   MAKE_INPUT(nAttr, s_Y);

   MAKE_VECTOR(s_Z, "Z", "z", 0, 0, 1.0f);
   MAKE_INPUT(nAttr, s_Z);

   s_X_angle = nAttr.create("X_angle", "xa", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(360);
   MAKE_INPUT(nAttr, s_X_angle);

   s_Y_angle = nAttr.create("Y_angle", "ya", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(360);
   MAKE_INPUT(nAttr, s_Y_angle);

   s_Z_angle = nAttr.create("Z_angle", "za", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(360);
   MAKE_INPUT(nAttr, s_Z_angle);

   MAKE_COLOR(s_color, "color", "c", 1, 1, 1);
   MAKE_INPUT(nAttr, s_color);

   s_intensity = nAttr.create("intensity", "i", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_intensity);

   s_format = eAttr.create("format", "for", 1);
   eAttr.addField("mirrored_ball", 0);
   eAttr.addField("angular", 1);
   eAttr.addField("latlong", 2);
   eAttr.addField("cubic", 3);
   addAttribute(s_format);

   s_opaque_alpha = nAttr.create("opaque_alpha", "oa", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_opaque_alpha);

   s_visible = nAttr.create("visible", "vis", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_visible);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_X, s_OUT_color);
   attributeAffects(s_Y, s_OUT_color);
   attributeAffects(s_Z, s_OUT_color);
   attributeAffects(s_X_angle, s_OUT_color);
   attributeAffects(s_Y_angle, s_OUT_color);
   attributeAffects(s_Z_angle, s_OUT_color);
   attributeAffects(s_color, s_OUT_color);
   attributeAffects(s_intensity, s_OUT_color);
   attributeAffects(s_format, s_OUT_color);
   attributeAffects(s_opaque_alpha, s_OUT_color);
   attributeAffects(s_visible, s_OUT_color);

   return MS::kSuccess;
}
