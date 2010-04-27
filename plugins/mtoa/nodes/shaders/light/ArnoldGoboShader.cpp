
#include "ArnoldGoboShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldGoboShaderNode::id(ARNOLD_NODEID_GOBO);

MObject CArnoldGoboShaderNode::s_density;
MObject CArnoldGoboShaderNode::s_filter_mode;
MObject CArnoldGoboShaderNode::s_offsetX;
MObject CArnoldGoboShaderNode::s_offsetY;
MObject CArnoldGoboShaderNode::s_offset;
MObject CArnoldGoboShaderNode::s_rotate;
MObject CArnoldGoboShaderNode::s_scale_s;
MObject CArnoldGoboShaderNode::s_scale_t;
MObject CArnoldGoboShaderNode::s_slidemapR;
MObject CArnoldGoboShaderNode::s_slidemapG;
MObject CArnoldGoboShaderNode::s_slidemapB;
MObject CArnoldGoboShaderNode::s_slidemap;
MObject CArnoldGoboShaderNode::s_wrap_s;
MObject CArnoldGoboShaderNode::s_wrap_t;

MObject CArnoldGoboShaderNode::s_OUT_colorR;
MObject CArnoldGoboShaderNode::s_OUT_colorG;
MObject CArnoldGoboShaderNode::s_OUT_colorB;
MObject CArnoldGoboShaderNode::s_OUT_color;
MObject CArnoldGoboShaderNode::s_OUT_transparencyR;
MObject CArnoldGoboShaderNode::s_OUT_transparencyG;
MObject CArnoldGoboShaderNode::s_OUT_transparencyB;
MObject CArnoldGoboShaderNode::s_OUT_transparency;

MStatus CArnoldGoboShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldGoboShaderNode::creator()
{
   return new CArnoldGoboShaderNode();
}

MStatus CArnoldGoboShaderNode::initialize()
{
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;

   s_density = nAttr.create("density", "d", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_density);

   MAKE_ENUM(s_filter_mode, "filter_mode", "fm", 0, "gobo", "filter_mode");

   MAKE_POINT2(s_offset, "offset", "off", 0, 0);
   MAKE_INPUT(nAttr, s_offset);

   s_rotate = nAttr.create("rotate", "rot", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(360);
   MAKE_INPUT(nAttr, s_rotate);

   s_scale_s = nAttr.create("scale_s", "scs", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_scale_s);

   s_scale_t = nAttr.create("scale_t", "sct", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1000);
   MAKE_INPUT(nAttr, s_scale_t);

   MAKE_COLOR(s_slidemap, "slidemap", "sl", 1, 1, 1);
   MAKE_INPUT(nAttr, s_slidemap);

   MAKE_ENUM(s_wrap_s, "wrap_s", "ws", 2, "gobo", "wrap_s");

   MAKE_ENUM(s_wrap_s, "wrap_t", "wt", 2, "gobo", "wrap_t");

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_density, s_OUT_color);
   attributeAffects(s_filter_mode, s_OUT_color);
   attributeAffects(s_offset, s_OUT_color);
   attributeAffects(s_rotate, s_OUT_color);
   attributeAffects(s_scale_s, s_OUT_color);
   attributeAffects(s_scale_t, s_OUT_color);
   attributeAffects(s_slidemap, s_OUT_color);
   attributeAffects(s_wrap_s, s_OUT_color);
   attributeAffects(s_wrap_t, s_OUT_color);

   return MS::kSuccess;
}
