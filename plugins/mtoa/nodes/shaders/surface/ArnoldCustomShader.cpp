
#include "ArnoldCustomShader.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MRenderUtil.h>

#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>

#include <ai_node_entry.h>
#include <ai_params.h>
#include <ai_metadata.h>

MString CArnoldCustomShaderNode::s_shaderName;

MObjectArray CArnoldCustomShaderNode::s_PlugsAffecting;

std::vector<CStaticAttrHelper> CArnoldCustomShaderNode::s_nodeHelpers;

void CArnoldCustomShaderNode::postConstructor()
{
   setMPSafe(false);
}

MStatus CArnoldCustomShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldCustomShaderNode::creator()
{
   return new CArnoldCustomShaderNode();
}

MStatus CArnoldCustomShaderNode::initialize()
{
   MFnAttribute fnAttr;
   MFnNumericAttribute nAttr;
   MFnEnumAttribute eAttr;
   MFnTypedAttribute tAttr;
   MFnMatrixAttribute mAttr;
   MFnMessageAttribute msgAttr;

   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(s_shaderName.asChar());

   CStaticAttrHelper helper(CArnoldCustomShaderNode::addAttribute, nodeEntry);

   // outputs
   bool outputExists = helper.MakeOutput();

   if (outputExists)
   {
      // TODO: determine when it is appropriate to make outTransparency.
      // currently maya crashes when it does not exist
      MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
      MAKE_OUTPUT(nAttr, s_OUT_transparency);
   }

   // bump
   AtBoolean doBump;
   if (AiMetaDataGetBool(nodeEntry, NULL, "maya.supports_bump", &doBump) && doBump)
   {
      MObject attrib = nAttr.createPoint("normalCamera", "n");
      nAttr.setKeyable(true);
      nAttr.setStorable(true);
      nAttr.setReadable(true);
      nAttr.setWritable(true);
      addAttribute(attrib);
   }

   // inputs
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      // skip the special "name" parameter
      if (strcmp(paramName, "name") != 0)
         helper.MakeInput(paramName);
   }
   AiParamIteratorDestroy(nodeParam);


   s_nodeHelpers.push_back(helper);
   return MS::kSuccess;
}
