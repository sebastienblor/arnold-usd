
#include "ArnoldSkyShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <ai_ray.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

MTypeId CArnoldSkyShaderNode::id(ARNOLD_NODEID_SKY);

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
MObject CArnoldSkyShaderNode::s_casts_shadows;
MObject CArnoldSkyShaderNode::s_primary_visibility;
MObject CArnoldSkyShaderNode::s_visible_in_reflections;
MObject CArnoldSkyShaderNode::s_visible_in_refractions;
MObject CArnoldSkyShaderNode::s_diffuse_visibility;
MObject CArnoldSkyShaderNode::s_glossy_visibility;

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

   s_intensity = nAttr.create("intensity", "i", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_intensity);

   MAKE_ENUM(s_format, "format", "for", 1, "sky", "format");

   s_casts_shadows = nAttr.create("casts_shadows", "shd", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_casts_shadows);
   
   s_primary_visibility = nAttr.create("primary_visibility", "vis", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_primary_visibility);
   
   s_visible_in_reflections = nAttr.create("visible_in_reflections", "rfl", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_visible_in_reflections);
   
   s_visible_in_refractions = nAttr.create("visible_in_refractions", "rfr", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_visible_in_refractions);
   
   s_diffuse_visibility = nAttr.create("diffuse_visibility", "dvis", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_diffuse_visibility);
   
   s_glossy_visibility = nAttr.create("glossy_visibility", "gvis", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_glossy_visibility);

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
   attributeAffects(s_primary_visibility, s_OUT_color);
   attributeAffects(s_visible_in_reflections, s_OUT_color);
   attributeAffects(s_visible_in_refractions, s_OUT_color);
   attributeAffects(s_diffuse_visibility, s_OUT_color);
   attributeAffects(s_glossy_visibility, s_OUT_color);

   return MS::kSuccess;
}
