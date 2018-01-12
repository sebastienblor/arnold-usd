
#include "nodes/ArnoldOperatorNode.h"
#include "nodes/ArnoldNodeIDs.h"
#include "extension/ExtensionsManager.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MPlugArray.h>

CAbMayaNode CArnoldOperatorNode::s_abstract;
std::vector<CStaticAttrHelper> CArnoldOperatorNode::s_nodeHelpers;

void* CArnoldOperatorNode::creator()
{
   return new CArnoldOperatorNode();
}

void CArnoldOperatorNode::postConstructor()
{
   // TODO: use a metadata to define this
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   // No compute anyway
   setMPSafe(false);
   // Copy the abstract so that it can accessed on instances
   // (and saved before a new register overwrites it)
   m_abstract = s_abstract;
}

MStatus CArnoldOperatorNode::initialize()
{
   MFnAttribute fnAttr;
   MFnNumericAttribute nAttr;

   MString maya = s_abstract.name;
   MString arnold = s_abstract.arnold;
   MString classification = s_abstract.classification;
   MString provider = s_abstract.provider;
   // to add them to the list of existing operators in the arnold menu
   CExtensionsManager::AddOperator(maya);

   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

   CStaticAttrHelper helper(CArnoldOperatorNode::addAttribute, nodeEntry);
   MObject outputAttr = helper.MakeOutput();
   bool outputExists = (outputAttr != MObject::kNullObj);

   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      std::string paramNameStr(paramName);
      // skip the special "name" parameter
      if (paramNameStr != "name")
      {
         bool hide = false;
         if (!AiMetaDataGetBool(nodeEntry, paramName, "maya.hide", &hide) || !hide)
         {
            CAttrData attrData;
            helper.GetAttrData(paramName, attrData);
            if (paramNameStr == "inputs")
               attrData.type = AI_TYPE_NODE;

            MObject attr = helper.MakeInput(attrData);
            if (outputExists)
            {
               attributeAffects(attr, outputAttr);
            }
         }
      }
   }
   AiParamIteratorDestroy(nodeParam);
   s_nodeHelpers.push_back(helper);

   return MS::kSuccess;
}

