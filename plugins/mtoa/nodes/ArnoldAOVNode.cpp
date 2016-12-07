
#include "nodes/ArnoldAOVNode.h"
#include "nodes/ArnoldNodeIDs.h"

#include <ai_universe.h>
#include <ai_params.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMessageAttribute.h>

MTypeId CArnoldAOVNode::id(ARNOLD_NODEID_AOV);

MObject CArnoldAOVNode::s_name;
MObject CArnoldAOVNode::s_enabled;
MObject CArnoldAOVNode::s_type;
MObject CArnoldAOVNode::s_prefix;
MObject CArnoldAOVNode::s_defaultValue;
MObject CArnoldAOVNode::s_imageFormat;
MObject CArnoldAOVNode::s_filterType;

MObject CArnoldAOVNode::s_outputs;
MObject CArnoldAOVNode::s_driver;
MObject CArnoldAOVNode::s_filter;


void* CArnoldAOVNode::creator()
{
   return new CArnoldAOVNode();
}

MStatus CArnoldAOVNode::initialize()
{
   MFnTypedAttribute tAttr;
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnMessageAttribute mAttr;
   MFnStringData sData;
   MFnCompoundAttribute cmpAttr;

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
   eAttr.addField("vector2",  AI_TYPE_VECTOR2);
   eAttr.addField("pointer", AI_TYPE_POINTER);
   eAttr.setKeyable(false);
   addAttribute(s_type);

   s_defaultValue = mAttr.create("defaultValue", "dftv");
   mAttr.setKeyable(false);
   addAttribute(s_defaultValue);

   s_prefix = tAttr.create("prefix", "aovpre", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_prefix);

   // TODO: remove after transitioning to new driver/filter nodes
   s_imageFormat = tAttr.create("imageFormat", "img", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_imageFormat);

   // TODO: remove after transitioning to new driver/filter nodes
   s_filterType = tAttr.create("filterType", "fltr", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_filterType);

   s_outputs = cmpAttr.create("outputs", "out");
   cmpAttr.setArray(true);
   cmpAttr.setIndexMatters(false); // allow -nextAvailable

   s_driver = mAttr.create("driver", "drvr");
   mAttr.setKeyable(false);
   cmpAttr.addChild(s_driver);

   s_filter = mAttr.create("filter", "ftr");
   mAttr.setKeyable(false);
   cmpAttr.addChild(s_filter);

   cmpAttr.setKeyable(false);

   addAttribute(s_outputs);

   return MStatus::kSuccess;
}
