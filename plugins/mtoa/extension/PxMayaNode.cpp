#include "PxMayaNode.h"
#if MAYA_API_VERSION >= 201200
   #include <maya/MNodeClass.h>
#endif

#include "attributes/AttrHelper.h"
#include "attributes/Metadata.h"
#include "nodes/MayaNodeIDs.h"
#include "nodes/ShaderUtils.h"
#include "nodes/shader/ArnoldShaderNode.h"
#include "nodes/shader/ArnoldStandardNode.h"

#include <ai_metadata.h>

// A Maya node class proxy
CPxMayaNode::CPxMayaNode(const MString &typeName,
                         const MTypeId &typeId,
                         const MString &providerName,
                         const MString &providerFile,
                         MCreatorFunction creatorFunction,
                         MInitializeFunction initFunction,
                         MPxNode::Type typeNode,
                         const MString &classif)
{
   name = typeName;
#if MAYA_API_VERSION < 201200
   id = typeId;
#else
   if (typeId.id() != 0)
      id = typeId;
   else
      id = MNodeClass(typeName).typeId();
#endif
   arnold = "";
   provider = providerName;
   file = providerFile;
   creator = creatorFunction;
   initialize = initFunction;
   abstract = NULL;
   type = typeNode;
   classification = classif;
}

/// Read metadata for this Maya node from an arnold node
MStatus CPxMayaNode::ReadMetaData(const AtNodeEntry* arnoldNodeEntry)
{
   const char* node = AiNodeEntryGetName(arnoldNodeEntry);
   arnold = node;
   const char* ext = provider.asChar();

   // int arnoldNodeType = AiNodeEntryGetType(arnoldNodeEntry);
   MString arnoldNodeTypeName = AiNodeEntryGetTypeName(arnoldNodeEntry);
   // If Maya node type name and ids were not specified
   if (name.numChars() == 0)
   {
      // get maya type name from metadata
      const char* mayaNodeNameMtd;
      if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.name", &mayaNodeNameMtd))
      {
         name = MString(mayaNodeNameMtd);
      }
      else if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.counterpart", &mayaNodeNameMtd))
      {
         AiMsgWarning("[mtoa] [%s] [node %s] The use of the maya.counterpart metadata is deprecated, use maya.name instead.",
               ext, node);
         name = MString(mayaNodeNameMtd);
      }
      else if (arnoldNodeTypeName == "camera")
      {
         name = "camera";
      }
      else if (arnoldNodeTypeName == "driver")
      {
         name = "aiAOVDriver";
      }
      else if (arnoldNodeTypeName == "filter")
      {
         name = "aiAOVFilter";
      }
      else
      {
         name = toMayaStyle(MString("ai_") + node);
      }
   }
   if (id.id() == 0)
   {
      // get maye typeId from metadata
      int nodeId;
      if (AiMetaDataGetInt(arnoldNodeEntry, NULL, "maya.id", &nodeId))
      {
         id = MTypeId(nodeId);
      }
      else if (AiMetaDataGetInt(arnoldNodeEntry, NULL, "maya.counterpart_id", &nodeId))
      {
         AiMsgWarning("[mtoa] [%s] [node %s] The use of the maya.counterpart_id metadata is deprecated, use maya.id instead.",
               ext, node);
         id = MTypeId(nodeId);
      }
      else if (arnoldNodeTypeName == "camera")
      {
         id = MTypeId(MAYA_NODEID_CAMERA);
      }
   }
   // If we didn't specify a maya node type to use as a base for this plugin node
   if (MPxNode::kLast == type)
   {
      const char* nodeTypeMtd;
      if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.type", &nodeTypeMtd))
      {
         if (strcmp(nodeTypeMtd, "kLocatorNode") == 0)
         {
            type = MPxNode::kLocatorNode;
         }
         else if (strcmp(nodeTypeMtd, "kDependNode") == 0)
         {
            type = MPxNode::kDependNode;
         }
         else
         {
            AiMsgWarning("[mtoa] [%s] [node %s] Unknown Maya type %s in maya.type metadata.",
                  ext, node, nodeTypeMtd);
         }
      }
   }

   // AOVs
   CBaseAttrHelper helper(arnoldNodeEntry);
   AtParamIterator* paramIt = AiNodeEntryGetParamIterator(arnoldNodeEntry);
   while (!AiParamIteratorFinished(paramIt))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(paramIt);
      const char* paramName = AiParamGetName(paramEntry);
      // skip the special "name" parameter
      if (strcmp(paramName, "name") != 0)
      {
         bool hide = false;
         if (!AiMetaDataGetBool(arnoldNodeEntry, paramName, "maya.hide", &hide) || !hide)
         {
            CAttrData attrData;
            helper.GetAttrData(paramName, attrData);

            // AOVs
            int aovType;
            if (AiMetaDataGetInt(arnoldNodeEntry, paramName, "aov.type", &aovType))
            {
               // assert that we're a string parameter
               if (AiParamGetType(paramEntry) != AI_TYPE_STRING)
               {
                  AiMsgError("[%s] [node %s] %s: AOV parameters must be of type string",
                             ext, node, paramName);
                  continue;
               }
               // it's an aov parameter
               AtParamValue defaultValue = MAiParamGetDefault(arnoldNodeEntry, paramEntry);
               RegisterAOV(defaultValue.STR, aovType, attrData.shortName);
            }
         }
      }
   }
   AiParamIteratorDestroy(paramIt);

   // Class methods to use to create the Maya node, if none were specified
   // TODO : use some map to make less hardcoded (BuiltinMayaNodes or BuiltinMayaTranslators)
   // we could also let type to MPxNode::kLast et let manager
   // handle this, but having access to builting classes for subclassing
   // might be useful to extensions anyway.
   if (NULL == creator)
   {
      if (arnoldNodeTypeName == "light")
      {
         // TODO : define a base light class
         // creator = CArnoldSkyDomeLightShaderNode::creator;
         // initialize = CArnoldSkyDomeLightShaderNode::initialize;
         type = MPxNode::kLocatorNode;
      }
      else if (arnoldNodeTypeName == "shader")
      {
         if(id == 0x00115D01) //aiStandard node
         {
            creator = CArnoldStandardNode::creator;
            initialize = CArnoldStandardNode::initialize;
            abstract = &CArnoldStandardNode::s_abstract;
         }
         else
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
         }
         type = MPxNode::kDependNode;
      }
      else if (arnoldNodeTypeName == "shape")
      {
         // TODO : can be expanded to allow base custom shape too
         // can easily add this to CPxMayaNode
         // MCreatorFunction     uiCreatorFunction,
      }
      // No default strategy to create the rest
   }
   // classification string if none is stored
   if (classification.numChars() == 0)
   {
      // classification metadata
      const char* classificationMtd;
      if (!AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.classification", &classificationMtd))
      {
         classificationMtd = CLASSIFY_SHADER.asChar();
      }
      
      if(strcmp("light/filter", classificationMtd) == 0)
      {
         const char* lights;
         if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.lights", &lights))
         {
            MString cmd = "from mtoa.utils import addLightFilterClassification;addLightFilterClassification('" + MString(lights) + "','"+ MString(name) +"')";
            CHECK_MSTATUS(MGlobal::executePythonCommand(cmd));
         }
      }
      // should we use swatch to preview this node
      bool doSwatch;
      if (!AiMetaDataGetBool(arnoldNodeEntry, NULL, "maya.swatch", &doSwatch))
         doSwatch = true;
      if (strlen(classificationMtd))
      {
         classification = MString(classificationMtd);
         classification += MString(":") + ARNOLD_CLASSIFY(classification);
         if (doSwatch) classification += MString(":swatch/") + ARNOLD_SWATCH;
      }
   }

   return MStatus::kSuccess;
}

void CPxMayaNode::RegisterAOV(const MString &aovName,
                             int dataType,
                             const MString &aovAttr)
{
   // there can only be one type per AOV
   // TODO: assert that this aov has not already been set with a different type
   CAOVData data;
   data.attribute = aovAttr;
   data.name = aovName;
   data.type = dataType;
   AiMsgInfo("[mtoa] [%s] [node %s] Registered AOV \"%s\"",
             provider.asChar(), name.asChar(), aovName.asChar());
   m_aovs.push_back(data);
}
