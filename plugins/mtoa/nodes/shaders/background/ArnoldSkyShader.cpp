#include "ArnoldSkyShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <ai_ray.h>
#include <ai_shader_util.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MStringArray.h>

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
MObject CArnoldSkyShaderNode::s_intensity;

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

void* CArnoldSkyShaderNode::creator()
{
   return new CArnoldSkyShaderNode();
}

MStatus CArnoldSkyShaderNode::initialize()
{
   MPxNode::inheritAttributesFrom("SphereLocator");

   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;

   s_intensity = nAttr.create("intensity", "i", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_intensity);

   s_casts_shadows = nAttr.create("casts_shadows", "shd", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_casts_shadows);

   s_primary_visibility = nAttr.create("primary_visibility", "pvis", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_primary_visibility);

   s_visible_in_reflections = nAttr.create("visible_in_reflections", "rfl", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_visible_in_reflections);

   s_visible_in_refractions = nAttr.create("visible_in_refractions", "rfr", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_visible_in_refractions);

   s_diffuse_visibility = nAttr.create("diffuse_visibility", "dvis", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_diffuse_visibility);

   s_glossy_visibility = nAttr.create("glossy_visibility", "gvis", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   addAttribute(s_glossy_visibility);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "ocl", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   return MS::kSuccess;
}

