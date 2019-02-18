#include "ArnoldUserDataBoolNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "translators/DagTranslator.h"

// #include <ai_ray.h>
// #include <ai_shader_util.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MStringArray.h>


MTypeId CArnoldUserDataBoolNode::id(ARNOLD_NODEID_USER_DATA_BOOL);

CStaticAttrHelper CArnoldUserDataBoolNode::s_attributes(CArnoldUserDataBoolNode::addAttribute);

MObject CArnoldUserDataBoolNode::s_OUT_Value;

MObject CArnoldUserDataBoolNode::s_boolAttrName;
MObject CArnoldUserDataBoolNode::s_defaultValue;

void* CArnoldUserDataBoolNode::creator()
{
   return new CArnoldUserDataBoolNode();
}

MStatus CArnoldUserDataBoolNode::initialize()
{

   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;

   // OUTPUT ATTRIBUTES
   s_OUT_Value = nAttr.create("outValue", "out", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   MAKE_OUTPUT(nAttr, s_OUT_Value);

   s_defaultValue = nAttr.create("defaultValue", "defaultValue", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_defaultValue);
   
    s_boolAttrName = tAttr.create("boolAttrName", "boolAttrName", MFnData::kString);
    tAttr.setStorable(true);
    tAttr.setReadable(true);
    tAttr.setWritable(true);
    CHECK_MSTATUS_AND_RETURN_IT(addAttribute(s_boolAttrName));
    CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(s_boolAttrName, s_OUT_Value));

   return MS::kSuccess;
}


