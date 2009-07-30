
#include "ArnoldSkyShader.h"
#include "ShaderUtils.h"

#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldSkyShaderNode::id(0x00072004);

MObject CArnoldSkyShaderNode::s_colorR;
MObject CArnoldSkyShaderNode::s_colorG;
MObject CArnoldSkyShaderNode::s_colorB;
MObject CArnoldSkyShaderNode::s_color;
MObject CArnoldSkyShaderNode::s_emission_colorR;
MObject CArnoldSkyShaderNode::s_emission_colorG;
MObject CArnoldSkyShaderNode::s_emission_colorB;
MObject CArnoldSkyShaderNode::s_emission_color;
MObject CArnoldSkyShaderNode::s_emission_intensity;
MObject CArnoldSkyShaderNode::s_intensity;
MObject CArnoldSkyShaderNode::s_opaque_alpha;
MObject CArnoldSkyShaderNode::s_visible;
MObject CArnoldSkyShaderNode::s_separate_colors;
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
   MFnNumericAttribute nAttr;

   MAKE_COLOR(s_color, "color", "c", 1, 1, 1);
   MAKE_INPUT(nAttr, s_color);

   MAKE_COLOR(s_emission_color, "emission_color", "ec", 1, 1, 1);
   MAKE_INPUT(nAttr, s_emission_color);

   s_emission_intensity = nAttr.create("emission_intensity", "ei", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_emission_intensity);

   s_intensity = nAttr.create("intensity", "i", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_intensity);

   s_opaque_alpha = nAttr.create("opaque_alpha", "oa", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_opaque_alpha);

   s_visible = nAttr.create("visible", "vis", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_visible);

   s_separate_colors = nAttr.create("separate_colors", "sep", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_separate_colors);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_color, s_OUT_color);
   attributeAffects(s_emission_color, s_OUT_color);
   attributeAffects(s_emission_intensity, s_OUT_color);
   attributeAffects(s_intensity, s_OUT_color);
   attributeAffects(s_opaque_alpha, s_OUT_color);
   attributeAffects(s_visible, s_OUT_color);

   return MS::kSuccess;
}
