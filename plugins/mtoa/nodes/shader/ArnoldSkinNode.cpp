#include "ArnoldSkinNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <ai_types.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MRenderUtil.h>

#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>

MTypeId CArnoldSkinNode::id(ARNOLD_NODEID_SKIN);

MObject CArnoldSkinNode::s_diffuse_color;
MObject CArnoldSkinNode::s_diffuse_weight;
MObject CArnoldSkinNode::s_shallow_scatter_color;
MObject CArnoldSkinNode::s_shallow_scatter_weight;

MObject CArnoldSkinNode::s_OUT_color;
MObject CArnoldSkinNode::s_OUT_transparencyR;
MObject CArnoldSkinNode::s_OUT_transparencyG;
MObject CArnoldSkinNode::s_OUT_transparencyB;
MObject CArnoldSkinNode::s_OUT_transparency;

CAbMayaNode CArnoldSkinNode::s_abstract;

MObjectArray CArnoldSkinNode::s_PlugsAffecting;
CStaticAttrHelper* CArnoldSkinNode::s_nodeHelper = NULL;

void CArnoldSkinNode::postConstructor()
{
	setExistWithoutInConnections(true);
	setExistWithoutOutConnections(true);
	setMPSafe(false);
	// Copy the abstract so that it can accessed on instances
	// (this replicates behaviour of CArnoldShaderNode)
	m_abstract = s_abstract;
}

MStatus CArnoldSkinNode::compute(const MPlug& plug, MDataBlock& data)
{
	if ((plug == s_OUT_color) || (plug.parent() == s_OUT_color))
	{
		MFloatVector resultColor(0.0,0.0,0.0);

		MFloatVector& shallow_scatter_color  = data.inputValue( s_shallow_scatter_color ).asFloatVector();
		float shallow_scatter_weight = data.inputValue( s_shallow_scatter_weight ).asFloat();

		shallow_scatter_weight = 1.0f;

		MFloatVector& diffuse_color  = data.inputValue( s_diffuse_color ).asFloatVector();
		float diffuse_weight = data.inputValue( s_diffuse_weight ).asFloat();

		diffuse_weight = 0.0f;

		resultColor[0] = ( shallow_scatter_weight * shallow_scatter_color[0] ) + ( diffuse_weight  * diffuse_color[0] );
		resultColor[1] = ( shallow_scatter_weight * shallow_scatter_color[1] ) + ( diffuse_weight  * diffuse_color[1] );
		resultColor[2] = ( shallow_scatter_weight * shallow_scatter_color[2] ) + ( diffuse_weight  * diffuse_color[2] );

		// set ouput color attribute
		MDataHandle outColorHandle = data.outputValue( s_OUT_color );
		MFloatVector& outColor = outColorHandle.asFloatVector();
		outColor = resultColor;
		outColorHandle.setClean();

		return MS::kSuccess;
	}
	else if ((plug == s_OUT_transparency) || (plug.parent() == s_OUT_transparency))
	{
		//float& trFloat ( data.inputValue( s_Kt ).asFloat());
		float trFloat = 0.0f;
		//MFloatVector& opacity  = data.inputValue( s_opacity ).asFloatVector();
		MFloatVector opacity(1.0f, 1.0f, 1.0f);
		float opFloat0 = CLAMP(trFloat*opacity[0]/2.0f + (1 - opacity[0]), 0.0f, 1.0f);
		float opFloat1 = CLAMP(trFloat*opacity[1]/2.0f + (1 - opacity[1]), 0.0f, 1.0f);
		float opFloat2 = CLAMP(trFloat*opacity[2]/2.0f + (1 - opacity[2]), 0.0f, 1.0f);
		MFloatVector tr(opFloat0, opFloat1, opFloat2);
		// set ouput color attribute
		MDataHandle outTransHandle = data.outputValue( s_OUT_transparency );
		MFloatVector& outTrans = outTransHandle.asFloatVector();
		outTrans = tr;
		data.setClean( plug );
		return MS::kSuccess;
	}
	else        
		return MS::kUnknownParameter;
}

void* CArnoldSkinNode::creator()
{
	return new CArnoldSkinNode();
}

MStatus CArnoldSkinNode::initialize()
{
	MFnAttribute fnAttr;
	MFnNumericAttribute nAttr;

	MString maya = s_abstract.name;
	MString arnold = s_abstract.arnold;
	MString classification = s_abstract.classification;
	MString provider = s_abstract.provider;
	const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

	AiMsgDebug("[mtoa] Initializing ArnoldCustomShader as Maya node %s, from Arnold node %s metadata, provided by %s",
		maya.asChar(), arnold.asChar(), provider.asChar());
	CStaticAttrHelper helper(CArnoldSkinNode::addAttribute, nodeEntry);

	// outputs
	s_OUT_color = helper.MakeOutput();

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

	MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
	MAKE_OUTPUT(nAttr, s_OUT_transparency);

	// bump
	MObject attrib = nAttr.createPoint("normalCamera", "n");
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	addAttribute(attrib);

	// If it is a surface, make default outColor gray
	attributeAffects(attrib, s_OUT_color);
	MFnNumericAttribute nAttrTmp(s_OUT_color);
	nAttrTmp.setDefault(float(0.5), float(0.5), float(0.5));

	// Make default hardware color gray
	MObject hwColor = nAttr.createColor("hardwareColor", "hwc");
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	nAttr.setDefault(0.5f, 0.5f, 0.5f);
	addAttribute(hwColor);

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
				
				if (strcmp(paramName, "diffuse_color") == 0)
				{
					s_diffuse_color = attr;
				}
				else if (strcmp(paramName, "diffuse_weight") == 0)
				{
					s_diffuse_weight = attr;
				}
				else if (strcmp(paramName, "shallow_scatter_color") == 0)
				{
					s_shallow_scatter_color = attr;
				}
				else if (strcmp(paramName, "shallow_scatter_weight") == 0)
				{
					s_shallow_scatter_weight = attr;
				}
			}
		}
	}
	AiParamIteratorDestroy(nodeParam);


	s_nodeHelper = &helper;

	return MS::kSuccess;
}
