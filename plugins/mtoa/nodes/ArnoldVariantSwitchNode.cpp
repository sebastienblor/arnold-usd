
#include "nodes/ArnoldVariantSwitchNode.h"
#include "nodes/ArnoldNodeIDs.h"
#include "extension/ExtensionsManager.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMessageAttribute.h>

MTypeId CArnoldVariantSwitchNode::id(ARNOLD_NODEID_OPERATOR_VARIANT_SWITCH);

MObject CArnoldVariantSwitchNode::s_enable;
MObject CArnoldVariantSwitchNode::s_index;

MObject CArnoldVariantSwitchNode::s_variants;
MObject CArnoldVariantSwitchNode::s_name;
MObject CArnoldVariantSwitchNode::s_inputs;

MObject CArnoldVariantSwitchNode::s_out;


void* CArnoldVariantSwitchNode::creator()
{
   return new CArnoldVariantSwitchNode();
}

MStatus CArnoldVariantSwitchNode::initialize()
{
   CExtensionsManager::AddOperator("aiVariantSwitch");

   MFnTypedAttribute tAttr;
   // MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnMessageAttribute mAttr;
   MFnStringData sData;
   MFnCompoundAttribute cmpAttr;

   s_enable = nAttr.create("enable", "swen", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_enable);

   s_index = nAttr.create("index", "idx", MFnNumericData::kInt);
   addAttribute(s_index);

   s_variants = cmpAttr.create("variants", "vars");
   cmpAttr.setArray(true);
   cmpAttr.setIndexMatters(false); // allow -nextAvailable

   s_name = tAttr.create("variantName", "name", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   cmpAttr.addChild(s_name);

   s_inputs = mAttr.create("inputs", "ipts");
   mAttr.setArray(true);
   mAttr.setDisconnectBehavior(MFnAttribute::kDelete);
   mAttr.setKeyable(true);
   mAttr.setStorable(true);
   mAttr.setReadable(true);
   mAttr.setWritable(true);
   cmpAttr.addChild(s_inputs);

   cmpAttr.setKeyable(false);

   addAttribute(s_variants);

   // out message plug

   s_out = mAttr.create("out", "out");
   mAttr.setArray(false);
   mAttr.setKeyable(false);
   mAttr.setStorable(false);
   mAttr.setReadable(true);
   mAttr.setWritable(false);
   addAttribute(s_out);

   return MStatus::kSuccess;
}
