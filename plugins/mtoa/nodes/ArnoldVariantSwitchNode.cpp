
#include "nodes/ArnoldVariantSwitchNode.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMessageAttribute.h>

MTypeId CArnoldVariantSwitchNode::id(ARNOLD_NODEID_OPERATOR_VARIANT_SWITCH);

MObject CArnoldVariantSwitchNode::s_enabled;
MObject CArnoldVariantSwitchNode::s_index;

MObject CArnoldVariantSwitchNode::s_varients;
MObject CArnoldVariantSwitchNode::s_name;
MObject CArnoldVariantSwitchNode::s_inputs;


void* CArnoldVariantSwitchNode::creator()
{
   return new CArnoldVariantSwitchNode();
}

MStatus CArnoldVariantSwitchNode::initialize()
{
   MFnTypedAttribute tAttr;
   // MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnMessageAttribute mAttr;
   MFnStringData sData;
   MFnCompoundAttribute cmpAttr;

   s_enabled = nAttr.create("enabled", "swen", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_enabled);

   s_index = nAttr.create("index", "idx", MFnNumericData::kInt);
   addAttribute(s_index);

   s_varients = cmpAttr.create("variants", "vars");
   cmpAttr.setArray(true);
   cmpAttr.setIndexMatters(false); // allow -nextAvailable

   s_name = tAttr.create("variantName", "name", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   cmpAttr.addChild(s_name);

   s_inputs = mAttr.create("inputs", "ipts");
   mAttr.setKeyable(false);
   mAttr.setArray(true);
   cmpAttr.addChild(s_inputs);

   cmpAttr.setKeyable(false);

   addAttribute(s_varients);

   return MStatus::kSuccess;
}
