
#include "nodes/ArnoldFilterNode.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MPlugArray.h>

MTypeId CArnoldFilterNode::id(ARNOLD_NODEID_FILTER);

MObject CArnoldFilterNode::s_filter;

void* CArnoldFilterNode::creator()
{
   return new CArnoldFilterNode();
}

MStatus CArnoldFilterNode::initialize()
{
   MFnTypedAttribute tAttr;
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;
   MFnMessageAttribute mAttr;
   MFnStringData sData;

   s_filter = tAttr.create("aiTranslator", "ai_translator", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_filter);

   return MStatus::kSuccess;
}
