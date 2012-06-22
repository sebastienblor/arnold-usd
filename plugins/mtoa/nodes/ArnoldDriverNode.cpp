
#include "nodes/ArnoldDriverNode.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MPlugArray.h>

MTypeId CArnoldDriverNode::id(ARNOLD_NODEID_DRIVER);

MObject CArnoldDriverNode::s_split_aovs;
MObject CArnoldDriverNode::s_driver;
MObject CArnoldDriverNode::s_prefix;

void* CArnoldDriverNode::creator()
{
   return new CArnoldDriverNode();
}

MStatus CArnoldDriverNode::initialize()
{
   MFnTypedAttribute tAttr;
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnMessageAttribute mAttr;
   MFnStringData sData;

   s_split_aovs = nAttr.create("splitAOVs", "split", MFnNumericData::kBoolean, true);
   nAttr.setKeyable(false);
   addAttribute(s_split_aovs);

   s_driver = tAttr.create("aiTranslator", "ai_translator", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_driver);

   s_prefix = tAttr.create("prefix", "pre", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_prefix);

   return MStatus::kSuccess;
}
