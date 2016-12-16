
#include "ArnoldStandardSurfaceNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"


#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MRenderUtil.h>

#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>

MTypeId CArnoldStandardSurfaceNode::id(ARNOLD_NODEID_STANDARD_SURFACE);

MObject CArnoldStandardSurfaceNode::s_Kd;
MObject CArnoldStandardSurfaceNode::s_Kd_color;
MObject CArnoldStandardSurfaceNode::s_emission;
MObject CArnoldStandardSurfaceNode::s_emission_color;
MObject CArnoldStandardSurfaceNode::s_Kt;
MObject CArnoldStandardSurfaceNode::s_opacity;
MObject CArnoldStandardSurfaceNode::s_OUT_color;
MObject CArnoldStandardSurfaceNode::s_OUT_transparencyR;
MObject CArnoldStandardSurfaceNode::s_OUT_transparencyG;
MObject CArnoldStandardSurfaceNode::s_OUT_transparencyB;
MObject CArnoldStandardSurfaceNode::s_OUT_transparency;
MObject CArnoldStandardSurfaceNode::s_normal_camera;


CAbMayaNode CArnoldStandardSurfaceNode::s_abstract;

MObjectArray CArnoldStandardSurfaceNode::s_PlugsAffecting;
CStaticAttrHelper* CArnoldStandardSurfaceNode::s_nodeHelper = NULL;

void CArnoldStandardSurfaceNode::postConstructor()
{
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   setMPSafe(false);
   // Copy the abstract so that it can accessed on instances
   // (this replicates behaviour of CArnoldShaderNode)
   m_abstract = s_abstract;
}

MStatus CArnoldStandardSurfaceNode::compute(const MPlug& plug, MDataBlock& data)
{
   if ((plug == s_OUT_color) || (plug.parent() == s_OUT_color))
   {
      MFloatVector resultColor(0.0,0.0,0.0);

      MFloatVector& surfaceColor  = data.inputValue( s_Kd_color ).asFloatVector();
      float difusse = data.inputValue( s_Kd ).asFloat();
      
      MFloatVector& emissionColor  = data.inputValue( s_emission_color ).asFloatVector();
      float emission = data.inputValue( s_emission ).asFloat();
    
      resultColor[0] = ( difusse * surfaceColor[0] ) + ( 2.0f * emission  * emissionColor[0] );
      resultColor[1] = ( difusse * surfaceColor[1] ) + ( 2.0f * emission  * emissionColor[1] );
      resultColor[2] = ( difusse * surfaceColor[2] ) + ( 2.0f * emission  * emissionColor[2] );

      // set ouput color attribute
      MDataHandle outColorHandle = data.outputValue( s_OUT_color );
      MFloatVector& outColor = outColorHandle.asFloatVector();
      outColor = resultColor;
      outColorHandle.setClean();
 
      return MS::kSuccess;
   }
   else if ((plug == s_OUT_transparency) || (plug.parent() == s_OUT_transparency))
   {
      float& trFloat ( data.inputValue( s_Kt ).asFloat());
      MFloatVector& opacity  = data.inputValue( s_opacity ).asFloatVector();
      float opFloat0 = AiClamp(trFloat*opacity[0]/2.0f + (1 - opacity[0]), 0.0f, 1.0f);
      float opFloat1 = AiClamp(trFloat*opacity[1]/2.0f + (1 - opacity[1]), 0.0f, 1.0f);
      float opFloat2 = AiClamp(trFloat*opacity[2]/2.0f + (1 - opacity[2]), 0.0f, 1.0f);
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

void* CArnoldStandardSurfaceNode::creator()
{
   return new CArnoldStandardSurfaceNode();
}

MStatus CArnoldStandardSurfaceNode::initialize()
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
   CStaticAttrHelper helper(CArnoldStandardSurfaceNode::addAttribute, nodeEntry);

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
            if (strcmp(paramName, "base") == 0)
            {
               s_Kd = attr;
            }
            else if (strcmp(paramName, "base_color") == 0)
            {
               s_Kd_color = attr;
            }
            else if (strcmp(paramName, "emission") == 0)
            {
               s_emission = attr;
            }
            else if (strcmp(paramName, "emission_color") == 0)
            {
               s_emission_color = attr;
            }
            else if (strcmp(paramName, "transparency") == 0)
            {
               s_Kt = attr;
               attributeAffects(attr, s_OUT_transparency);
            }
            else if (strcmp(paramName, "opacity") == 0)
            {
               s_opacity = attr;
               attributeAffects(attr, s_OUT_transparency);
            }
         }
      }
   }
   AiParamIteratorDestroy(nodeParam);


   s_nodeHelper = &helper;

   return MS::kSuccess;
}
