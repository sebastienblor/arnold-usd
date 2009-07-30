
#include "ArnoldSky_HDRIShader.h"
#include "ShaderUtils.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

MTypeId CArnoldSky_HDRIShaderNode::id(0x00072005);

MObject CArnoldSky_HDRIShaderNode::s_HDRI_map;
MObject CArnoldSky_HDRIShaderNode::s_XX;
MObject CArnoldSky_HDRIShaderNode::s_XY;
MObject CArnoldSky_HDRIShaderNode::s_XZ;
MObject CArnoldSky_HDRIShaderNode::s_X;
MObject CArnoldSky_HDRIShaderNode::s_X_angle;
MObject CArnoldSky_HDRIShaderNode::s_YX;
MObject CArnoldSky_HDRIShaderNode::s_YY;
MObject CArnoldSky_HDRIShaderNode::s_YZ;
MObject CArnoldSky_HDRIShaderNode::s_Y;
MObject CArnoldSky_HDRIShaderNode::s_Y_angle;
MObject CArnoldSky_HDRIShaderNode::s_ZX;
MObject CArnoldSky_HDRIShaderNode::s_ZY;
MObject CArnoldSky_HDRIShaderNode::s_ZZ;
MObject CArnoldSky_HDRIShaderNode::s_Z;
MObject CArnoldSky_HDRIShaderNode::s_Z_angle;
MObject CArnoldSky_HDRIShaderNode::s_flip_X;
MObject CArnoldSky_HDRIShaderNode::s_flip_Y;
MObject CArnoldSky_HDRIShaderNode::s_flip_Z;
MObject CArnoldSky_HDRIShaderNode::s_format;
MObject CArnoldSky_HDRIShaderNode::s_multiplier;
MObject CArnoldSky_HDRIShaderNode::s_opaque_alpha;
MObject CArnoldSky_HDRIShaderNode::s_rgb_multiplierR;
MObject CArnoldSky_HDRIShaderNode::s_rgb_multiplierG;
MObject CArnoldSky_HDRIShaderNode::s_rgb_multiplierB;
MObject CArnoldSky_HDRIShaderNode::s_rgb_multiplier;
MObject CArnoldSky_HDRIShaderNode::s_visibility;
MObject CArnoldSky_HDRIShaderNode::s_OUT_colorR;
MObject CArnoldSky_HDRIShaderNode::s_OUT_colorG;
MObject CArnoldSky_HDRIShaderNode::s_OUT_colorB;
MObject CArnoldSky_HDRIShaderNode::s_OUT_color;
MObject CArnoldSky_HDRIShaderNode::s_OUT_transparencyR;
MObject CArnoldSky_HDRIShaderNode::s_OUT_transparencyG;
MObject CArnoldSky_HDRIShaderNode::s_OUT_transparencyB;
MObject CArnoldSky_HDRIShaderNode::s_OUT_transparency;

MStatus CArnoldSky_HDRIShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldSky_HDRIShaderNode::creator()
{
   return new CArnoldSky_HDRIShaderNode();
}

MStatus CArnoldSky_HDRIShaderNode::initialize()
{
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;

   s_HDRI_map = tAttr.create("HDRI_map", "map", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setStorable(true);
   tAttr.setConnectable(false);
   addAttribute(s_HDRI_map);

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

   s_flip_X = nAttr.create("flip_X", "fx", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_flip_X);

   s_flip_Y = nAttr.create("flip_Y", "fy", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_flip_Y);

   s_flip_Z = nAttr.create("flip_Z", "fz", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_flip_Z);

   s_format = eAttr.create("format", "for", 1);
   eAttr.addField("mirrored_ball", 0);
   eAttr.addField("angular", 1);
   eAttr.addField("latlong", 2);
   eAttr.addField("cubic", 3);
   addAttribute(s_format);

   s_multiplier = nAttr.create("multiplier", "m", MFnNumericData::kFloat, 1.0f);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_multiplier);

   s_opaque_alpha = nAttr.create("opaque_alpha", "oa", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_opaque_alpha);

   MAKE_COLOR(s_rgb_multiplier, "rgb_multiplier", "rgbm", 1.0f, 1.0f, 1.0f);
   MAKE_INPUT(nAttr, s_rgb_multiplier);

   s_visibility = nAttr.create("visibility", "vis", MFnNumericData::kInt, 65535);
   nAttr.setMin(0);
   nAttr.setMax(65535);
   MAKE_INPUT(nAttr, s_visibility);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_HDRI_map, s_OUT_color);
   attributeAffects(s_X, s_OUT_color);
   attributeAffects(s_Y, s_OUT_color);
   attributeAffects(s_Z, s_OUT_color);
   attributeAffects(s_X_angle, s_OUT_color);
   attributeAffects(s_Y_angle, s_OUT_color);
   attributeAffects(s_Z_angle, s_OUT_color);
   attributeAffects(s_flip_X, s_OUT_color);
   attributeAffects(s_flip_Y, s_OUT_color);
   attributeAffects(s_flip_Z, s_OUT_color);
   attributeAffects(s_format, s_OUT_color);
   attributeAffects(s_multiplier, s_OUT_color);
   attributeAffects(s_rgb_multiplier, s_OUT_color);
   attributeAffects(s_opaque_alpha, s_OUT_color);
   attributeAffects(s_visibility, s_OUT_color);

   return MS::kSuccess;
}
