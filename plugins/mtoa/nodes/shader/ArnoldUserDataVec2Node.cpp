#include "ArnoldUserDataVec2Node.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "translators/DagTranslator.h"

// #include <ai_ray.h>
// #include <ai_shader_util.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MStringArray.h>


MTypeId CArnoldUserDataVec2Node::id(ARNOLD_NODEID_USER_DATA_PNT2);

CStaticAttrHelper CArnoldUserDataVec2Node::s_attributes(CArnoldUserDataVec2Node::addAttribute);

MObject CArnoldUserDataVec2Node::s_OUT_ValueX;
MObject CArnoldUserDataVec2Node::s_OUT_ValueY;
MObject CArnoldUserDataVec2Node::s_OUT_Value;
MObject CArnoldUserDataVec2Node::s_OUT_transparencyR;
MObject CArnoldUserDataVec2Node::s_OUT_transparencyG;
MObject CArnoldUserDataVec2Node::s_OUT_transparencyB;
MObject CArnoldUserDataVec2Node::s_OUT_transparency;

MObject CArnoldUserDataVec2Node::s_vec2AttrName;
MObject CArnoldUserDataVec2Node::s_defaultValue;
MObject CArnoldUserDataVec2Node::s_defaultValueX;
MObject CArnoldUserDataVec2Node::s_defaultValueY;




void* CArnoldUserDataVec2Node::creator()
{
   return new CArnoldUserDataVec2Node();
}

MStatus CArnoldUserDataVec2Node::initialize()
{

   MFnNumericAttribute nAttr;
   MFnTypedAttribute tAttr;

  // OUTPUT ATTRIBUTES

    MAKE_VECTOR2(s_OUT_Value, "outValue", "out", 0, 0);
    MAKE_OUTPUT(nAttr, s_OUT_Value);

    MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
    MAKE_OUTPUT(nAttr, s_OUT_transparency);

   MAKE_VECTOR2(s_defaultValue, "defaultValue", "defaultValue", 0.0, 0.0);
   MAKE_INPUT(nAttr, s_defaultValue);
   
    s_vec2AttrName = tAttr.create("vec2AttrName", "vec2AttrName", MFnData::kString);
    tAttr.setStorable(true);
    tAttr.setReadable(true);
    tAttr.setWritable(true);
    CHECK_MSTATUS_AND_RETURN_IT(addAttribute(s_vec2AttrName));
    CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(s_vec2AttrName, s_OUT_Value));

   return MS::kSuccess;
}


