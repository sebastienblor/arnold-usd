
#include "ArnoldStandardHairNode.h"
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

MTypeId CArnoldStandardHairNode::id(ARNOLD_NODEID_STANDARD_HAIR);

MObject CArnoldStandardHairNode::s_OUT_color;
MObject CArnoldStandardHairNode::s_normal_camera;

CAbMayaNode CArnoldStandardHairNode::s_abstract;

MObjectArray CArnoldStandardHairNode::s_PlugsAffecting;
CStaticAttrHelper* CArnoldStandardHairNode::s_nodeHelper = NULL;

void CArnoldStandardHairNode::postConstructor()
{
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   setMPSafe(false);
   // Copy the abstract so that it can accessed on instances
   // (this replicates behaviour of CArnoldShaderNode)
   m_abstract = s_abstract;
}

MStatus CArnoldStandardHairNode::compute(const MPlug& /*plug*/, MDataBlock& /*data*/)
{
	return MS::kSuccess;
}

void* CArnoldStandardHairNode::creator()
{
   return new CArnoldStandardHairNode();
}

MStatus CArnoldStandardHairNode::initialize()
{
   MFnAttribute fnAttr;
   MFnNumericAttribute nAttr;

   MString maya = s_abstract.name;
   MString arnold = s_abstract.arnold;
   MString classification = s_abstract.classification;
   MString provider = s_abstract.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

   AiMsgDebug("[mtoa] Initializing ArnoldCustomShader as Maya node %s, from Arnold node %s metadata, provided by %s",
         maya.asChar(), arnold.asChar(), provider.asChar());
   CStaticAttrHelper helper(CArnoldStandardHairNode::addAttribute, nodeEntry);

   s_normal_camera = nAttr.createPoint( "normalCamera", "n" );
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setDefault(1.0f, 1.0f, 1.0f);
   nAttr.setHidden(true) ;
   addAttribute(s_normal_camera);
   attributeAffects(s_normal_camera, s_OUT_color);

   // dummy attribute to pass transparency to the VP2 API.
   MObject tempObject = nAttr.createColor("aiTransparency", "ai_transparency");
   nAttr.setStorable(false);
   nAttr.setReadable(true);
   nAttr.setWritable(true);
   nAttr.setHidden(true);
   nAttr.setDefault(0.);
   addAttribute(tempObject);
   
   // outputs
   s_OUT_color = helper.MakeOutput();
   
   // inputs
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      // skip the special "name" parameter
      if (strcmp(paramName, "name") != 0)
      {
         bool hide = false;
         if (!AiMetaDataGetBool(nodeEntry, paramName, "maya.hide", &hide) || !hide)
         {
            CAttrData attrData;
            helper.GetAttrData(paramName, attrData);
            MObject attr = helper.MakeInput(attrData);
            attributeAffects(attr, s_OUT_color);
         }
      }
   }
   AiParamIteratorDestroy(nodeParam);

   s_nodeHelper = &helper;

   return MS::kSuccess;
}
