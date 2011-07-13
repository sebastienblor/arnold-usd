
#include "nodes/ArnoldAOVNode.h"
#include "nodes/ArnoldNodeIDs.h"

#include <ai_universe.h>
#include <ai_params.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>

MTypeId CArnoldAOVNode::id(ARNOLD_NODEID_AOV);

MObject CArnoldAOVNode::s_aovs;
MObject CArnoldAOVNode::s_name;
MObject CArnoldAOVNode::s_enabled;
MObject CArnoldAOVNode::s_type;
MObject CArnoldAOVNode::s_prefix;
MObject CArnoldAOVNode::s_mode;

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

   s_mode = eAttr.create("mode", "m");
   eAttr.setKeyable(false);
   eAttr.setDefault(true);
   eAttr.addField("disabled", 0);
   eAttr.addField("enabled", 1);
   eAttr.addField("batch_only", 2);
   addAttribute(s_mode);

   s_enabled = nAttr.create("enabled", "aoven", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_enabled);

   s_name = tAttr.create("name", "aovn", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_name);

   s_type = eAttr.create("type", "aovt", AI_TYPE_RGBA);
   // add all valid AOV types
   eAttr.addField("int",     AI_TYPE_INT);
   eAttr.addField("bool",    AI_TYPE_BOOLEAN);
   eAttr.addField("float",   AI_TYPE_FLOAT);
   eAttr.addField("rgb",     AI_TYPE_RGB);
   eAttr.addField("rgba",    AI_TYPE_RGBA);
   eAttr.addField("vector",  AI_TYPE_VECTOR);
   eAttr.addField("point",   AI_TYPE_POINT);
   eAttr.addField("point2",  AI_TYPE_POINT2);
   eAttr.addField("pointer", AI_TYPE_POINTER);
   eAttr.setKeyable(false);
   addAttribute(s_type);

   s_prefix = tAttr.create("prefix", "aovpre", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_prefix);

   s_aovs = cAttr.create("aovs", "aovs");
   cAttr.setKeyable(false);
   cAttr.setArray(true);
   cAttr.addChild(s_enabled);
   cAttr.addChild(s_name);
   cAttr.addChild(s_type);
   cAttr.addChild(s_prefix);
   addAttribute(s_aovs);

   return MStatus::kSuccess;
}
