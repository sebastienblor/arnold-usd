
#include "ArnoldDisplacementNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MRenderUtil.h>

#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>

MTypeId CArnoldDisplacementNode::id(ARNOLD_NODEID_DISPLACEMENT);

MObject CArnoldDisplacementNode::s_height;
MObject CArnoldDisplacementNode::s_zero_value;
MObject CArnoldDisplacementNode::s_mapR;
MObject CArnoldDisplacementNode::s_mapG;
MObject CArnoldDisplacementNode::s_mapB;
MObject CArnoldDisplacementNode::s_map;
MObject CArnoldDisplacementNode::s_autobump;
MObject CArnoldDisplacementNode::s_vector_displacement;
MObject CArnoldDisplacementNode::s_vector_displacement_scale;
MObject CArnoldDisplacementNode::s_OUT_colorR;
MObject CArnoldDisplacementNode::s_OUT_colorG;
MObject CArnoldDisplacementNode::s_OUT_colorB;
MObject CArnoldDisplacementNode::s_OUT_color;
MObject CArnoldDisplacementNode::s_OUT_transparencyR;
MObject CArnoldDisplacementNode::s_OUT_transparencyG;
MObject CArnoldDisplacementNode::s_OUT_transparencyB;
MObject CArnoldDisplacementNode::s_OUT_transparency;
MObject CArnoldDisplacementNode::s_OUT_displacementR;
MObject CArnoldDisplacementNode::s_OUT_displacementG;
MObject CArnoldDisplacementNode::s_OUT_displacementB;
MObject CArnoldDisplacementNode::s_OUT_displacement;

MStatus CArnoldDisplacementNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldDisplacementNode::creator()
{
   return new CArnoldDisplacementNode();
}

MStatus CArnoldDisplacementNode::initialize()
{
   MFnNumericAttribute  nAttr;

   s_height = nAttr.create("disp_height", "hgt", MFnNumericData::kFloat, 1);
   nAttr.setMin(-100);
   nAttr.setMax(100);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   nAttr.setConnectable(false);
   MAKE_INPUT(nAttr, s_height);

   s_zero_value = nAttr.create("disp_zero_value", "zerov", MFnNumericData::kFloat, 0);
   nAttr.setMin(-100);
   nAttr.setMax(100);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(1);
   nAttr.setConnectable(false);
   MAKE_INPUT(nAttr, s_zero_value);

   MAKE_COLOR(s_map, "disp_map", "map", 1, 1, 1);
   MAKE_INPUT(nAttr, s_map);

   s_autobump = nAttr.create("autobump", "atbmp", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_autobump);

   s_vector_displacement = nAttr.create("vector_displacement", "vdisp", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_vector_displacement);

   s_vector_displacement_scale = nAttr.create("vector_displacement_scale", "vdispscale", MFnNumericData::k3Float, 1.0);
   MAKE_INPUT(nAttr, s_vector_displacement_scale);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   MAKE_COLOR(s_OUT_displacement, "displacement", "od", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_displacement);

   attributeAffects(s_map, s_OUT_color);
   attributeAffects(s_map, s_OUT_displacement);

   return MS::kSuccess;
}
