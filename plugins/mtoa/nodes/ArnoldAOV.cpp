
#include "ArnoldAOV.h"
#include "nodes/ArnoldNodeIds.h"

#include <ai_universe.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>

MTypeId CArnoldAOVNode::id(ARNOLD_NODEID_AOV);

MObject CArnoldAOVNode::s_aovs;
MObject CArnoldAOVNode::s_name;
MObject CArnoldAOVNode::s_prefix;
MObject CArnoldAOVNode::s_batch_mode_only;

void* CArnoldAOVNode::creator()
{
   return new CArnoldAOVNode();
}

MStatus CArnoldAOVNode::initialize()
{
   MFnTypedAttribute tAttr;
   MFnEnumAttribute eAttr;
   MFnCompoundAttribute cAttr;
   MFnNumericAttribute nAttr;
   MFnStringData sData;

   s_batch_mode_only = nAttr.create("aov_batch_mode_only", "arniabmo", MFnNumericData::kBoolean);
   nAttr.setKeyable(false);
   nAttr.setDefault(true);
   addAttribute(s_batch_mode_only);
   
   s_name = tAttr.create("aov_name", "arnian", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_name);

   s_prefix = tAttr.create("aov_prefix", "arniap", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_prefix);

   s_aovs = cAttr.create("aovs", "arniaovs");
   cAttr.setKeyable(false);
   cAttr.setArray(true);
   cAttr.addChild(s_name);
   cAttr.addChild(s_prefix);
   addAttribute(s_aovs);

   return MStatus::kSuccess;
}
