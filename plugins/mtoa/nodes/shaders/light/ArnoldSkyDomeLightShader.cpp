#include "ArnoldSkyDomeLightShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIds.h"

#include <ai_ray.h>
#include <ai_shader_util.h>

#include <ai_metadata.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

MTypeId CArnoldSkyDomeLightShaderNode::id(ARNOLD_NODEID_SKYDOME_LIGHT);

CStaticAttrHelper CArnoldSkyDomeLightShaderNode::s_attributes(CArnoldSkyDomeLightShaderNode::addAttribute);

MObject CArnoldSkyDomeLightShaderNode::s_OUT_colorR;
MObject CArnoldSkyDomeLightShaderNode::s_OUT_colorG;
MObject CArnoldSkyDomeLightShaderNode::s_OUT_colorB;
MObject CArnoldSkyDomeLightShaderNode::s_OUT_color;
MObject CArnoldSkyDomeLightShaderNode::s_OUT_transparencyR;
MObject CArnoldSkyDomeLightShaderNode::s_OUT_transparencyG;
MObject CArnoldSkyDomeLightShaderNode::s_OUT_transparencyB;
MObject CArnoldSkyDomeLightShaderNode::s_OUT_transparency;

void* CArnoldSkyDomeLightShaderNode::creator()
{
   return new CArnoldSkyDomeLightShaderNode();
}

MStatus CArnoldSkyDomeLightShaderNode::initialize()
{
   MPxNode::inheritAttributesFrom("SphereLocator");

   MFnNumericAttribute nAttr;

   s_attributes.SetNode("skydome_light");
   s_attributes.MakeInput("resolution");
   s_attributes.MakeInput("intensity");
   //helper->MakeMatrixInput(s_matrix, "matrix");
   s_attributes.MakeInput("exposure");
   s_attributes.MakeInput("cast_shadows");
   s_attributes.MakeInput("shadow_density");
   s_attributes.MakeInput("shadow_color");
   s_attributes.MakeInput("samples");
   s_attributes.MakeInput("normalize");
   s_attributes.MakeInput("affect_diffuse");
   s_attributes.MakeInput("affect_specular");
   s_attributes.MakeInput("bounces");
   s_attributes.MakeInput("bounce_factor");

   // arrays
   // TODO: use metdata to rename this attribute to light_filters
   s_attributes.MakeInput("filters");
   s_attributes.MakeInput("time_samples");

   s_attributes.MakeInput("sss_samples");
   s_attributes.MakeInput("mis");

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "ocl", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   return MS::kSuccess;
}

