#include "ArnoldUserDataVectorNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "translators/DagTranslator.h"

// #include <ai_ray.h>
// #include <ai_shader_util.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MStringArray.h>


MTypeId CArnoldUserDataVectorNode::id(ARNOLD_NODEID_USER_DATA_VECTOR);

CStaticAttrHelper CArnoldUserDataVectorNode::s_attributes(CArnoldUserDataVectorNode::addAttribute);

MObject CArnoldUserDataVectorNode::s_OUT_ValueX;
MObject CArnoldUserDataVectorNode::s_OUT_ValueY;
MObject CArnoldUserDataVectorNode::s_OUT_ValueZ;
MObject CArnoldUserDataVectorNode::s_OUT_Value;
MObject CArnoldUserDataVectorNode::s_OUT_transparencyR;
MObject CArnoldUserDataVectorNode::s_OUT_transparencyG;
MObject CArnoldUserDataVectorNode::s_OUT_transparencyB;
MObject CArnoldUserDataVectorNode::s_OUT_transparency;

MObject CArnoldUserDataVectorNode::s_vectorAttrName;
MObject CArnoldUserDataVectorNode::s_defaultValue;
MObject CArnoldUserDataVectorNode::s_defaultValueX;
MObject CArnoldUserDataVectorNode::s_defaultValueY;
MObject CArnoldUserDataVectorNode::s_defaultValueZ;


void* CArnoldUserDataVectorNode::creator()
{
   return new CArnoldUserDataVectorNode();
}

MStatus CArnoldUserDataVectorNode::initialize()
{

   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;

  // OUTPUT ATTRIBUTES

    MAKE_VECTOR(s_OUT_Value, "outValue", "out", 0, 0, 0);
    MAKE_OUTPUT(nAttr, s_OUT_Value);

    MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
    MAKE_OUTPUT(nAttr, s_OUT_transparency);

   MAKE_VECTOR(s_defaultValue, "defaultValue", "defaultValue", 0.0, 0.0, 0.0);
   MAKE_INPUT(nAttr, s_defaultValue);
   
    s_vectorAttrName = tAttr.create("vectorAttrName", "vectorAttrName", MFnData::kString);
    tAttr.setStorable(true);
    tAttr.setReadable(true);
    tAttr.setWritable(true);
    CHECK_MSTATUS_AND_RETURN_IT(addAttribute(s_vectorAttrName));
    CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(s_vectorAttrName, s_OUT_Value));

   return MS::kSuccess;
}


