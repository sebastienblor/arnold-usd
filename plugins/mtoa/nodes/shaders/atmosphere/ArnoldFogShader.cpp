
#include "ArnoldFogShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldFogShaderNode::id(ARNOLD_NODEID_FOG);

MObject CArnoldFogShaderNode::s_colorR;
MObject CArnoldFogShaderNode::s_colorG;
MObject CArnoldFogShaderNode::s_colorB;
MObject CArnoldFogShaderNode::s_color;
MObject CArnoldFogShaderNode::s_distance;
MObject CArnoldFogShaderNode::s_ground_normalX;
MObject CArnoldFogShaderNode::s_ground_normalY;
MObject CArnoldFogShaderNode::s_ground_normalZ;
MObject CArnoldFogShaderNode::s_ground_normal;
MObject CArnoldFogShaderNode::s_ground_pointX;
MObject CArnoldFogShaderNode::s_ground_pointY;
MObject CArnoldFogShaderNode::s_ground_pointZ;
MObject CArnoldFogShaderNode::s_ground_point;
MObject CArnoldFogShaderNode::s_height;
MObject CArnoldFogShaderNode::s_OUT_colorR;
MObject CArnoldFogShaderNode::s_OUT_colorG;
MObject CArnoldFogShaderNode::s_OUT_colorB;
MObject CArnoldFogShaderNode::s_OUT_color;
MObject CArnoldFogShaderNode::s_OUT_transparencyR;
MObject CArnoldFogShaderNode::s_OUT_transparencyG;
MObject CArnoldFogShaderNode::s_OUT_transparencyB;
MObject CArnoldFogShaderNode::s_OUT_transparency;

MStatus CArnoldFogShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldFogShaderNode::creator()
{
   return new CArnoldFogShaderNode();
}

MStatus CArnoldFogShaderNode::initialize()
{
   MFnNumericAttribute nAttr;

   MAKE_COLOR(s_color, "color", "c", 1, 1, 1);
   MAKE_INPUT(nAttr, s_color);

   s_distance = nAttr.create("distance", "d", MFnNumericData::kFloat, 0.02f);
   nAttr.setMin(0);
   nAttr.setSoftMax(1000);
   MAKE_INPUT(nAttr, s_distance);

   MAKE_VECTOR(s_ground_normal, "ground_normal", "gn", 0, 1, 0);
   MAKE_INPUT(nAttr, s_ground_normal);

   MAKE_POINT(s_ground_point, "ground_point", "gp", 0, 0, 0);
   MAKE_INPUT(nAttr, s_ground_point);

   s_height = nAttr.create("height", "h", MFnNumericData::kFloat, 5);
   nAttr.setMin(0);
   nAttr.setSoftMax(1000);
   MAKE_INPUT(nAttr, s_height);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_color, s_OUT_color);
   attributeAffects(s_distance, s_OUT_color);
   attributeAffects(s_ground_normal, s_OUT_color);
   attributeAffects(s_ground_point, s_OUT_color);
   attributeAffects(s_height, s_OUT_color);

   return MS::kSuccess;
}
