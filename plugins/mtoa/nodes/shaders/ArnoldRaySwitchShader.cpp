
#include "ArnoldRaySwitchShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldRaySwitchShaderNode::id(ARNOLD_NODEID_RAY_SWITCH);


MObject CArnoldRaySwitchShaderNode::s_IN_camera;
MObject CArnoldRaySwitchShaderNode::s_IN_cameraR;
MObject CArnoldRaySwitchShaderNode::s_IN_cameraG;
MObject CArnoldRaySwitchShaderNode::s_IN_cameraB;
MObject CArnoldRaySwitchShaderNode::s_IN_shadow;
MObject CArnoldRaySwitchShaderNode::s_IN_shadowR;
MObject CArnoldRaySwitchShaderNode::s_IN_shadowG;
MObject CArnoldRaySwitchShaderNode::s_IN_shadowB;
MObject CArnoldRaySwitchShaderNode::s_IN_reflection;
MObject CArnoldRaySwitchShaderNode::s_IN_reflectionR;
MObject CArnoldRaySwitchShaderNode::s_IN_reflectionG;
MObject CArnoldRaySwitchShaderNode::s_IN_reflectionB;
MObject CArnoldRaySwitchShaderNode::s_IN_refraction;
MObject CArnoldRaySwitchShaderNode::s_IN_refractionR;
MObject CArnoldRaySwitchShaderNode::s_IN_refractionG;
MObject CArnoldRaySwitchShaderNode::s_IN_refractionB;
MObject CArnoldRaySwitchShaderNode::s_IN_diffuse;
MObject CArnoldRaySwitchShaderNode::s_IN_diffuseR;
MObject CArnoldRaySwitchShaderNode::s_IN_diffuseG;
MObject CArnoldRaySwitchShaderNode::s_IN_diffuseB;
MObject CArnoldRaySwitchShaderNode::s_IN_glossy;
MObject CArnoldRaySwitchShaderNode::s_IN_glossyR;
MObject CArnoldRaySwitchShaderNode::s_IN_glossyG;
MObject CArnoldRaySwitchShaderNode::s_IN_glossyB;
MObject CArnoldRaySwitchShaderNode::s_OUT_colorR;
MObject CArnoldRaySwitchShaderNode::s_OUT_colorG;
MObject CArnoldRaySwitchShaderNode::s_OUT_colorB;
MObject CArnoldRaySwitchShaderNode::s_OUT_color;
MObject CArnoldRaySwitchShaderNode::s_OUT_transparencyR;
MObject CArnoldRaySwitchShaderNode::s_OUT_transparencyG;
MObject CArnoldRaySwitchShaderNode::s_OUT_transparencyB;
MObject CArnoldRaySwitchShaderNode::s_OUT_transparency;

MStatus CArnoldRaySwitchShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldRaySwitchShaderNode::creator()
{
   return new CArnoldRaySwitchShaderNode();
}

MStatus CArnoldRaySwitchShaderNode::initialize()
{
   MFnNumericAttribute  nAttr;

   // INPUT ATTRIBUTES

   MAKE_COLOR(s_IN_camera, "camera", "icam",  0, 0, 0);
   MAKE_INPUT(nAttr, s_IN_camera);

   MAKE_COLOR(s_IN_shadow, "shadow", "ishad", 0, 0, 0);
   MAKE_INPUT(nAttr, s_IN_shadow);

   MAKE_COLOR(s_IN_reflection, "reflection", "irfl", 0, 0, 0);
   MAKE_INPUT(nAttr, s_IN_reflection);

   MAKE_COLOR(s_IN_refraction, "refraction", "irfr", 0, 0, 0);
   MAKE_INPUT(nAttr, s_IN_refraction);

   MAKE_COLOR(s_IN_diffuse, "diffuse", "idiff", 0, 0, 0);   
   MAKE_INPUT(nAttr, s_IN_diffuse);

   MAKE_COLOR(s_IN_glossy, "glossy", "igls",  0, 0, 0);
   MAKE_INPUT(nAttr, s_IN_glossy);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   return MS::kSuccess;
}
