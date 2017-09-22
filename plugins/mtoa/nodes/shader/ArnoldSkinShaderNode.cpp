#include "ArnoldSkinShaderNode.h"
#include "nodes/ArnoldNodeIDs.h"
#include "nodes/ShaderUtils.h"

#include <maya/MFnNumericAttribute.h>

const MTypeId CArnoldSkinShaderNode::id(ARNOLD_NODEID_SKIN_SSS);
CAbMayaNode CArnoldSkinShaderNode::s_abstract;
CStaticAttrHelper* CArnoldSkinShaderNode::s_nodeHelper;

MObject CArnoldSkinShaderNode::s_OUT_color;
MObject CArnoldSkinShaderNode::s_OUT_transparencyR;
MObject CArnoldSkinShaderNode::s_OUT_transparencyG;
MObject CArnoldSkinShaderNode::s_OUT_transparencyB;
MObject CArnoldSkinShaderNode::s_OUT_transparency;
MObject CArnoldSkinShaderNode::s_normal_camera;

void CArnoldSkinShaderNode::postConstructor()
{
    setExistWithoutInConnections(true);
    setExistWithoutOutConnections(true);
    setMPSafe(false);
    // Copy the abstract so that it can accessed on instances
    // (this replicates behaviour of CArnoldShaderNode)
    m_abstract = s_abstract;
}

void* CArnoldSkinShaderNode::creator()
{
    return new CArnoldSkinShaderNode();
}

MStatus CArnoldSkinShaderNode::initialize()
{
    MFnNumericAttribute nAttr;

    MString maya = s_abstract.name;
    MString arnold = s_abstract.arnold;
    MString classification = s_abstract.classification;
    MString provider = s_abstract.provider;
    const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

    CStaticAttrHelper helper(CArnoldSkinShaderNode::addAttribute, nodeEntry);

    s_normal_camera = nAttr.createPoint( "normalCamera", "n" );
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
    nAttr.setDefault(1.0f, 1.0f, 1.0f);
    nAttr.setHidden(true) ;
    addAttribute(s_normal_camera);
    attributeAffects(s_normal_camera, s_OUT_color);

    MObject tempObject = nAttr.create("aiEnableMatte", "ai_enable_matte", MFnNumericData::kBoolean, 0);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
    addAttribute(tempObject);
    attributeAffects(tempObject, s_OUT_color);

    tempObject = nAttr.createColor("aiMatteColor", "ai_matte_color");
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
    nAttr.setDefault(0.);
    addAttribute(tempObject);
    attributeAffects(tempObject, s_OUT_color);

    tempObject = nAttr.create("aiMatteColorA", "ai_matte_color_a", MFnNumericData::kFloat);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
    nAttr.setDefault(0.);
    nAttr.setMin(0.);
    nAttr.setMax(1.);
    addAttribute(tempObject);
    attributeAffects(tempObject, s_OUT_color);

    // outputs
    s_OUT_color = helper.MakeOutput();

    MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
    MAKE_OUTPUT(nAttr, s_OUT_transparency);

    // inputs
    AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
    while (!AiParamIteratorFinished(nodeParam))
    {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      // skip the special "name" parameter
      if (strcmp(paramName, "name") != 0)
      {
         bool hide = false;
         if (!AiMetaDataGetBool(nodeEntry, paramName, "maya.hide", &hide) || !hide)
         {
            CAttrData attrData;
            helper.GetAttrData(paramName, attrData);
            MObject attr = helper.MakeInput(attrData);
            attributeAffects(attr, s_OUT_color);
         }
      }
    }
    AiParamIteratorDestroy(nodeParam);


    s_nodeHelper = &helper;

    return MS::kSuccess;
}
