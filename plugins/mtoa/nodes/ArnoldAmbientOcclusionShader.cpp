
#include "ArnoldAmbientOcclusionShader.h"
#include "ShaderUtils.h"

#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldAmbientOcclusionShaderNode::id(0x00072002);

MObject CArnoldAmbientOcclusionShaderNode::s_blackR;
MObject CArnoldAmbientOcclusionShaderNode::s_blackG;
MObject CArnoldAmbientOcclusionShaderNode::s_blackB;
MObject CArnoldAmbientOcclusionShaderNode::s_black;
MObject CArnoldAmbientOcclusionShaderNode::s_falloff;
MObject CArnoldAmbientOcclusionShaderNode::s_far_clip;
MObject CArnoldAmbientOcclusionShaderNode::s_near_clip;
MObject CArnoldAmbientOcclusionShaderNode::s_opacityR;
MObject CArnoldAmbientOcclusionShaderNode::s_opacityG;
MObject CArnoldAmbientOcclusionShaderNode::s_opacityB;
MObject CArnoldAmbientOcclusionShaderNode::s_opacity;
MObject CArnoldAmbientOcclusionShaderNode::s_samples;
MObject CArnoldAmbientOcclusionShaderNode::s_spread;
MObject CArnoldAmbientOcclusionShaderNode::s_whiteR;
MObject CArnoldAmbientOcclusionShaderNode::s_whiteG;
MObject CArnoldAmbientOcclusionShaderNode::s_whiteB;
MObject CArnoldAmbientOcclusionShaderNode::s_white;

MObject CArnoldAmbientOcclusionShaderNode::s_OUT_colorR;
MObject CArnoldAmbientOcclusionShaderNode::s_OUT_colorG;
MObject CArnoldAmbientOcclusionShaderNode::s_OUT_colorB;
MObject CArnoldAmbientOcclusionShaderNode::s_OUT_color;
MObject CArnoldAmbientOcclusionShaderNode::s_OUT_transparencyR;
MObject CArnoldAmbientOcclusionShaderNode::s_OUT_transparencyG;
MObject CArnoldAmbientOcclusionShaderNode::s_OUT_transparencyB;
MObject CArnoldAmbientOcclusionShaderNode::s_OUT_transparency;

MStatus CArnoldAmbientOcclusionShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldAmbientOcclusionShaderNode::creator()
{
   return new CArnoldAmbientOcclusionShaderNode();
}

MStatus CArnoldAmbientOcclusionShaderNode::initialize()
{
   MFnNumericAttribute  nAttr;

   MAKE_COLOR(s_black, "black", "bk", 0, 0, 0);
   MAKE_INPUT(nAttr, s_black);

   s_falloff = nAttr.create("falloff", "fo", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(100);
   MAKE_INPUT(nAttr, s_falloff);

   s_far_clip = nAttr.create("far_clip", "fc", MFnNumericData::kFloat, 100);
   nAttr.setMin(0);
   nAttr.setMax(100000);
   MAKE_INPUT(nAttr, s_far_clip);

   s_near_clip = nAttr.create("near_clip", "nc", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(100000);
   MAKE_INPUT(nAttr, s_near_clip);

   MAKE_COLOR(s_opacity, "opacity", "op", 1, 1, 1);
   MAKE_INPUT(nAttr, s_opacity);

   s_samples = nAttr.create("samples", "sm", MFnNumericData::kInt, 3);
   nAttr.setMin(0);
   nAttr.setMax(100);
   MAKE_INPUT(nAttr, s_samples);

   s_spread = nAttr.create("spread", "sp", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_spread);

   MAKE_COLOR(s_white, "white", "wt", 1, 1, 1);
   MAKE_INPUT(nAttr, s_white);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_black, s_OUT_color);
   attributeAffects(s_falloff, s_OUT_color);
   attributeAffects(s_far_clip, s_OUT_color);
   attributeAffects(s_near_clip, s_OUT_color);
   attributeAffects(s_opacity, s_OUT_color);
   attributeAffects(s_samples, s_OUT_color);
   attributeAffects(s_spread, s_OUT_color);
   attributeAffects(s_white, s_OUT_color);

   return MS::kSuccess;
}
