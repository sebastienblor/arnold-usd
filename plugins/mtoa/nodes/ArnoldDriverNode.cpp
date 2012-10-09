
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

MObject CArnoldDriverNode::s_mergeAOVs;
MObject CArnoldDriverNode::s_driver;
MObject CArnoldDriverNode::s_prefix;
MObject CArnoldDriverNode::s_outputMode;

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

   s_mergeAOVs = nAttr.create("mergeAOVs", "merge_AOVs", MFnNumericData::kBoolean, false);
   nAttr.setKeyable(false);
   addAttribute(s_mergeAOVs);

   s_driver = tAttr.create("aiTranslator", "ai_translator", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_driver);

   s_prefix = tAttr.create("prefix", "pre", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_prefix);

   s_outputMode = eAttr.create("outputMode", "output_mode", 2);
   eAttr.addField("GUI Only", 0);
   eAttr.addField("Batch Only", 1);
   eAttr.addField("GUI and Batch", 2);
   tAttr.setKeyable(false);
   addAttribute(s_outputMode);

   return MStatus::kSuccess;
}
