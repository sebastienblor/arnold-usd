
#include "ArnoldMaterialXShaderNode.h"
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


MTypeId CArnoldMaterialXShaderNode::id(ARNOLD_NODEID_SHADER_MATERIALX);

MObject CArnoldMaterialXShaderNode::s_OUT_colorR;
MObject CArnoldMaterialXShaderNode::s_OUT_colorG;
MObject CArnoldMaterialXShaderNode::s_OUT_colorB;
MObject CArnoldMaterialXShaderNode::s_OUT_color;
MObject CArnoldMaterialXShaderNode::s_OUT_transparencyR;
MObject CArnoldMaterialXShaderNode::s_OUT_transparencyG;
MObject CArnoldMaterialXShaderNode::s_OUT_transparencyB;
MObject CArnoldMaterialXShaderNode::s_OUT_transparency;

MObject CArnoldMaterialXShaderNode::s_normal_camera;

MObject CArnoldMaterialXShaderNode::s_mtlxFilePath;


CAbMayaNode CArnoldMaterialXShaderNode::s_abstract;

MObjectArray CArnoldMaterialXShaderNode::s_PlugsAffecting;
CStaticAttrHelper* CArnoldMaterialXShaderNode::s_nodeHelper = NULL;


CArnoldMaterialXShaderNode::~CArnoldMaterialXShaderNode()
{
   
}

void CArnoldMaterialXShaderNode::postConstructor()
{
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   setMPSafe(false);
   // Copy the abstract so that it can accessed on instances
   // (this replicates behaviour of CArnoldShaderNode)
   m_abstract = s_abstract;
}

MStatus CArnoldMaterialXShaderNode::compute(const MPlug& /*plug*/, MDataBlock& /*data*/)
{
	return MS::kSuccess;
}

void* CArnoldMaterialXShaderNode::creator()
{
   return new CArnoldMaterialXShaderNode();
}

MStatus CArnoldMaterialXShaderNode::initialize()
{
   MFnAttribute fnAttr;
   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;
   

   MAKE_COLOR(s_OUT_color, "outColor", "ocl", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);
   
   s_mtlxFilePath = tAttr.create("materialXFilePath", "mtlxpath", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   tAttr.setUsedAsFilename(true);
   addAttribute(s_mtlxFilePath);

//    s_texturePath = tAttr.create("texturePath", "texPth", MFnData::kString);
//    tAttr.setHidden(false);
//    tAttr.setStorable(true);
//    tAttr.setUsedAsFilename(true);
//    addAttribute(s_texturePath);

//    s_uvScale = nAttr.create("uvScale", "uvscl", MFnNumericData::kFloat);
//    nAttr.setStorable(true);
//    nAttr.setHidden(false);
//    nAttr.setReadable(true);
//    nAttr.setDefault(1.0f);
//    addAttribute(s_uvScale);
 
   return MS::kSuccess;
}
