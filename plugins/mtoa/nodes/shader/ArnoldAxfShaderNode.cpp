
#include "ArnoldAxfShaderNode.h"
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
#include "AxFtoA.h"

MTypeId CArnoldAxfShaderNode::id(ARNOLD_NODEID_SHADER_AXF);

MObject CArnoldAxfShaderNode::s_OUT_colorR;
MObject CArnoldAxfShaderNode::s_OUT_colorG;
MObject CArnoldAxfShaderNode::s_OUT_colorB;
MObject CArnoldAxfShaderNode::s_OUT_color;
MObject CArnoldAxfShaderNode::s_OUT_transparencyR;
MObject CArnoldAxfShaderNode::s_OUT_transparencyG;
MObject CArnoldAxfShaderNode::s_OUT_transparencyB;
MObject CArnoldAxfShaderNode::s_OUT_transparency;

MObject CArnoldAxfShaderNode::s_normal_camera;

MObject CArnoldAxfShaderNode::s_axfFilePath;
MObject CArnoldAxfShaderNode::s_texturePath;
MObject CArnoldAxfShaderNode::s_uvScale;

CAbMayaNode CArnoldAxfShaderNode::s_abstract;

MObjectArray CArnoldAxfShaderNode::s_PlugsAffecting;
CStaticAttrHelper* CArnoldAxfShaderNode::s_nodeHelper = NULL;


CArnoldAxfShaderNode::~CArnoldAxfShaderNode()
{
   AxFtoASessionEnd();
}

void CArnoldAxfShaderNode::postConstructor()
{
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   setMPSafe(false);
   // Copy the abstract so that it can accessed on instances
   // (this replicates behaviour of CArnoldShaderNode)
   m_abstract = s_abstract;
}

MStatus CArnoldAxfShaderNode::compute(const MPlug& /*plug*/, MDataBlock& /*data*/)
{
	return MS::kSuccess;
}

void* CArnoldAxfShaderNode::creator()
{
   return new CArnoldAxfShaderNode();
}

MStatus CArnoldAxfShaderNode::initialize()
{
   MFnAttribute fnAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;
   
   AxFtoASessionStart();

   MAKE_COLOR(s_OUT_color, "outColor", "ocl", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);
   
   s_axfFilePath = tAttr.create("axfFilePath", "axfFP", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   tAttr.setUsedAsFilename(true);
   addAttribute(s_axfFilePath);

   s_texturePath = tAttr.create("texturePath", "texPth", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   tAttr.setUsedAsFilename(true);
   addAttribute(s_texturePath);

   s_uvScale = nAttr.create("uvScale", "uvscl", MFnNumericData::kFloat);
   nAttr.setStorable(true);
   nAttr.setHidden(false);
   nAttr.setReadable(true);
   nAttr.setDefault(1.0f);
   addAttribute(s_uvScale);
 
   return MS::kSuccess;
}
