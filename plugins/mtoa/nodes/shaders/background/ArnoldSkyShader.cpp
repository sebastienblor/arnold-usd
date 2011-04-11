#include "ArnoldSkyShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "scene/NodeTranslator.h"

#include <ai_ray.h>
#include <ai_shader_util.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MStringArray.h>

MTypeId CArnoldSkyShaderNode::id(ARNOLD_NODEID_SKY);

CStaticAttrHelper CArnoldSkyShaderNode::s_attributes(CArnoldSkyShaderNode::addAttribute);

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

   MFnNumericAttribute nAttr;

   s_attributes.SetNode("sky");
   s_attributes.MakeInput("intensity");
   // FIXME: visibleInReflection, visibleInRefraction seem to be on the locator already but they are false by default
   CDagTranslator::MakeArnoldVisibilityFlags(s_attributes);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "ocl", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   return MS::kSuccess;
}

