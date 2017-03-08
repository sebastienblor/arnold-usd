
#include "ArnoldShaderNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "attributes/Metadata.h"
#include "render/AOV.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>
#include <maya/MFloatVector.h>

#include <ai_node_entry.h>
#include <ai_params.h>
#include <ai_metadata.h>

#ifdef ENABLE_COLOR_MANAGEMENT
#include <maya/MColorManagementUtilities.h>
#endif


CAbMayaNode CArnoldShaderNode::s_abstract;

MObjectArray CArnoldShaderNode::s_PlugsAffecting;

std::vector<CStaticAttrHelper> CArnoldShaderNode::s_nodeHelpers;

void CArnoldShaderNode::postConstructor()
{
   // TODO: use a metadata to define this
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   // No compute anyway
   setMPSafe(false);
   // Copy the abstract so that it can accessed on instances
   // (and saved before a new register overwrites it)
   m_abstract = s_abstract;

   // should we do another Node for aiImage instead ?

// the function connectDependencyNodeToColorManagement doesn't seem to exist before 2017
#if MAYA_API_VERSION >= 201700
   if (typeName() == "aiImage")
   {
      MObject obj = thisMObject();
      MColorManagementUtilities::connectDependencyNodeToColorManagement(obj);
   }
#endif
}

MStatus CArnoldShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldShaderNode::creator()
{
   return new CArnoldShaderNode();
}

MStatus CArnoldShaderNode::initialize()
{
   MFnAttribute fnAttr;
   MFnNumericAttribute nAttr;

   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

   MString maya = s_abstract.name;
   MString arnold = s_abstract.arnold;
   MString classification = s_abstract.classification;
   MString provider = s_abstract.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

   AiMsgDebug("[mtoa] Initializing ArnoldCustomShader as Maya node %s, from Arnold node %s metadata, provided by %s",
         maya.asChar(), arnold.asChar(), provider.asChar());
   CStaticAttrHelper helper(CArnoldShaderNode::addAttribute, nodeEntry);

   // outputs
   MObject outputAttr = helper.MakeOutput();
   bool outputExists = (outputAttr != MObject::kNullObj);
   if (outputExists)
   {
      // TODO: determine when it is appropriate to make outTransparency.
      // currently maya crashes when it does not exist
      MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
      MAKE_OUTPUT(nAttr, s_OUT_transparency);
   }

   // bump
   bool doBump = false;
   MStringArray classParts;
   MStringArray classes;
   classification.split(':', classParts);
   for (unsigned int i=0; i < classParts.length() && doBump == false; ++i)
   {
      classes.clear();
      classParts[i].split('/', classes);
      if (classes.length())
      {
         if (classes[0] == MString("drawdb")) // skip drawdb classification fragments
            continue;
         for (unsigned int j = 0; (j < classes.length()) && (doBump == false); ++j)
         {
            if (classes[j] == "surface")
            {
               doBump = true;
            }
         }
      }
   }
   if (doBump)
   {
      MObject attrib = nAttr.createPoint("normalCamera", "n");
      nAttr.setKeyable(true);
      nAttr.setStorable(true);
      nAttr.setReadable(true);
      nAttr.setWritable(true);
      addAttribute(attrib);

      if (outputExists)
      {
         // If it is a surface, make default outColor gray
         attributeAffects(attrib, outputAttr);
         MFnNumericAttribute nAttrTmp(outputAttr);
         nAttrTmp.setDefault(float(0.5), float(0.5), float(0.5));
      }

      // Make default hardware color gray
      MObject hwColor = nAttr.createColor("hardwareColor", "hwc");
      nAttr.setKeyable(true);
      nAttr.setStorable(true);
      nAttr.setReadable(true);
      nAttr.setWritable(true);
      nAttr.setDefault(0.5f, 0.5f, 0.5f);
      addAttribute(hwColor);
   }

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
            if (outputExists)
               attributeAffects(attr, outputAttr);
            /*
            // AOVs
            int aovType;
            if (AiMetaDataGetInt(nodeEntry, paramName, "aov.type", &aovType))
            {
               // assert that we're a string parameter
               if (AiParamGetType(paramEntry) != AI_TYPE_STRING)
               {
                  AiMsgError("[mtoa] AOV parameters must be of type string");
                  continue;
               }
               bool hide = false;
               if (!AiMetaDataGetBool(nodeEntry, paramName, "aov.hide", &hide) || !hide)
               {
                  // it's an aov parameter

                  //char aovName[128];
                  //if (!MAiMetaDataGetStr(nodeEntry, paramName, "aov.label", aovName))
                  //   strcpy(aovName, paramName);
                  AtParamValue defaultValue = MAiParamGetDefault(nodeEntry, paramEntry);
                  CExtensionsManager::GetExtension(provider)->RegisterAOV(maya, defaultValue.STR, aovType, attrData.name);
                  hasAOVs = true;
               }
            }*/
         }
      }
   }
   AiParamIteratorDestroy(nodeParam);


   s_nodeHelpers.push_back(helper);

   return MS::kSuccess;
}
