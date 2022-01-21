#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "PxMayaNode.h"
#include <maya/MNodeClass.h>

#include "attributes/AttrHelper.h"
#include "attributes/Metadata.h"
#include "nodes/MayaNodeIDs.h"
#include "nodes/ShaderUtils.h"
#include "nodes/shader/ArnoldShaderNode.h"
#include "nodes/shape/ArnoldProceduralNode.h"
#include "nodes/ArnoldOperatorNode.h"
#include "nodes/ArnoldImagerNode.h"
#include "nodes/shader/ArnoldSkinShaderNode.h"
#include "nodes/shader/ArnoldStandardNode.h"
#include "nodes/shader/ArnoldStandardSurfaceNode.h"
#include "nodes/shader/ArnoldStandardHairNode.h"
#include "nodes/ArnoldNodeIDs.h"
#include "utils/MtoaLog.h"
#include "utils/ConstantStrings.h"
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
   SetName(typeName);
   if (typeId.id() != 0)
      id = typeId;
   else
      id = MNodeClass(typeName).typeId();
   arnold = "";
   provider = providerName;
   file = providerFile;
   creator = creatorFunction;
   initialize = initFunction;
   abstract = NULL;
   type = typeNode;
   classification = classif;
   m_aovShader = false;
}

/// Read metadata for this Maya node from an arnold node
MStatus CPxMayaNode::ReadMetaData(const AtNodeEntry* arnoldNodeEntry)
{
   const char* node = AiNodeEntryGetName(arnoldNodeEntry);
   arnold = node;
   const char* ext = provider.asChar();

   // int arnoldNodeType = AiNodeEntryGetType(arnoldNodeEntry);
   const AtString arnoldNodeTypeName = AtString(AiNodeEntryGetTypeName(arnoldNodeEntry));
   // If Maya node type name and ids were not specified
   if (name.numChars() == 0)
   {
      // get maya type name from metadata
      AtString mayaNodeNameMtd;
      if (AiMetaDataGetStr(arnoldNodeEntry, AtString(), str::maya_name, &mayaNodeNameMtd))
      {
         SetName(MString(mayaNodeNameMtd));
      }
      else if (AiMetaDataGetStr(arnoldNodeEntry, AtString(), str::maya_counterpart, &mayaNodeNameMtd))
      {
         AiMsgWarning("[mtoa] [%s] [node %s] The use of the maya.counterpart metadata is deprecated, use maya.name instead.",
               ext, node);
         SetName(MString(mayaNodeNameMtd));
      }
      else if (arnoldNodeTypeName == str::camera)
      {
         SetName("camera");
      }
      else if (arnoldNodeTypeName == str::driver)
      {
         SetName("aiAOVDriver");
      }
      else if (arnoldNodeTypeName == str::filter)
      {
         SetName("aiAOVFilter");
      }
      else
      {
         SetName(toMayaStyle(MString("ai_") + node));
      }
   }
   if (id.id() == 0)
   {
      // get maye typeId from metadata
      int nodeId;
      if (AiMetaDataGetInt(arnoldNodeEntry, AtString(), str::maya_id, &nodeId))
      {
         id = MTypeId(nodeId);
      }
      else if (AiMetaDataGetInt(arnoldNodeEntry, AtString(), str::maya_counterpart_id, &nodeId))
      {
         AiMsgWarning("[mtoa] [%s] [node %s] The use of the maya.counterpart_id metadata is deprecated, use maya.id instead.",
               ext, node);
         id = MTypeId(nodeId);
      }
      else if (arnoldNodeTypeName == str::camera)
      {
         id = MTypeId(MAYA_NODEID_CAMERA);
      }
   }
   // If we didn't specify a maya node type to use as a base for this plugin node
   if (MPxNode::kLast == type)
   {
      AtString nodeTypeMtd;
      if (AiMetaDataGetStr(arnoldNodeEntry, AtString(), str::maya_type, &nodeTypeMtd))
      {
         if (nodeTypeMtd == str::kLocatorNode)
         {
            type = MPxNode::kLocatorNode;
         }
         else if (nodeTypeMtd == str::kDependNode)
         {
            type = MPxNode::kDependNode;
         }
         else
         {
            AiMsgWarning("[mtoa] [%s] [node %s] Unknown Maya type %s in maya.type metadata.",
                  ext, node, nodeTypeMtd.c_str());
         }
      }
   }

   // AOVs
   CBaseAttrHelper helper(arnoldNodeEntry);
   AtParamIterator* paramIt = AiNodeEntryGetParamIterator(arnoldNodeEntry);
   while (!AiParamIteratorFinished(paramIt))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(paramIt);
      const AtString paramName = AiParamGetName(paramEntry);
      // skip the special "name" parameter
      if (paramName != str::name)
      {
         bool hide = false;
         if (!AiMetaDataGetBool(arnoldNodeEntry, paramName, str::maya_hide, &hide) || !hide)
         {
            CAttrData attrData;
            helper.GetAttrData(paramName, attrData);

            // AOVs
            int aovType;
            if (AiMetaDataGetInt(arnoldNodeEntry, paramName, str::aov_type, &aovType))
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
               RegisterAOV(defaultValue.STR().c_str(), aovType, attrData.shortName);
            }
         }
      }
   }
   AiParamIteratorDestroy(paramIt);

   bool aovShader = m_aovShader = false;
   if (AiMetaDataGetBool(arnoldNodeEntry, AtString(), str::aov_shader, &aovShader))
      m_aovShader = true;
   

   // Class methods to use to create the Maya node, if none were specified
   // TODO : use some map to make less hardcoded (BuiltinMayaNodes or BuiltinMayaTranslators)
   // we could also let type to MPxNode::kLast et let manager
   // handle this, but having access to builting classes for subclassing
   // might be useful to extensions anyway.
   MString drawdbClassification = "";
   
   bool isAutoProcedural = false;

   AtString drawdbClassificationMtd;
   if (AiMetaDataGetStr(arnoldNodeEntry, AtString(), str::maya_drawdb, &drawdbClassificationMtd))
   {
      drawdbClassification = MString(":drawdb/") + MString(drawdbClassificationMtd);
   }
   if (NULL == creator)
   {      
      if (arnoldNodeTypeName == str::light)
      {
         // TODO : define a base light class
         // creator = CArnoldSkyDomeLightShaderNode::creator;
         // initialize = CArnoldSkyDomeLightShaderNode::initialize;
         type = MPxNode::kLocatorNode;
      }
      else if (arnoldNodeTypeName == str::shader)
      {
         if (id == ARNOLD_NODEID_STANDARD) //aiStandard node
         {
            creator    = CArnoldStandardNode::creator;
            initialize = CArnoldStandardNode::initialize;
            abstract   = &CArnoldStandardNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
               drawdbClassification = ":drawdb/shader/surface/arnold/standard";
         }
         else if (id == ARNOLD_NODEID_STANDARD_SURFACE) //aiStandardSurface node
         {
            creator    = CArnoldStandardSurfaceNode::creator;
            initialize = CArnoldStandardSurfaceNode::initialize;
            abstract   = &CArnoldStandardSurfaceNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
               drawdbClassification = ":drawdb/shader/surface/arnold/standard_surface";
         }
         else if (id == ARNOLD_NODEID_STANDARD_HAIR)
         {
            creator = CArnoldStandardHairNode::creator;
            initialize = CArnoldStandardHairNode::initialize;
            abstract = &CArnoldStandardHairNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
               drawdbClassification = ":drawdb/shader/surface/arnold/standard_hair";
         }
         else if (id == ARNOLD_NODEID_SKIN_SSS)
         {
            creator    = CArnoldSkinShaderNode::creator;
            initialize = CArnoldSkinShaderNode::initialize;
            abstract   = &CArnoldSkinShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
               drawdbClassification = ":drawdb/shader/surface/arnold/skin";
         }
         else if(id == ARNOLD_NODEID_IMAGE)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
               drawdbClassification = ":drawdb/shader/surface/arnold/image";
         }
         else if(id == ARNOLD_NODEID_SHADER_ABS)
         {
            creator    = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract   = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
                drawdbClassification = ":drawdb/shader/utility/math/arnold/abs";             
         }
         else if (id == ARNOLD_NODEID_SHADER_ADD)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
                drawdbClassification = ":drawdb/shader/utility/math/arnold/add";
         }
         else if (id == ARNOLD_NODEID_SHADER_ATAN)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
                drawdbClassification = ":drawdb/shader/utility/math/arnold/atan";
         }
         else if (id == ARNOLD_NODEID_SHADER_MIN)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
                drawdbClassification = ":drawdb/shader/utility/math/arnold/min";
         }
         else if (id == ARNOLD_NODEID_SHADER_MAX)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
                drawdbClassification = ":drawdb/shader/utility/math/arnold/max";
         }
         else if (id == ARNOLD_NODEID_SHADER_NEGATE)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
                drawdbClassification = ":drawdb/shader/utility/math/arnold/negate";
         }
         else if (id == ARNOLD_NODEID_SHADER_MULTIPLY)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
            drawdbClassification = ":drawdb/shader/utility/math/arnold/multiply";
         }
         else if (id == ARNOLD_NODEID_SHADER_DIVIDE)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/divide";
         }
         else if (id == ARNOLD_NODEID_SHADER_SUBTRACT)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/subtract";
         }
         else if (id == ARNOLD_NODEID_SHADER_POW)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/pow";
         }
         else if (id == ARNOLD_NODEID_SHADER_SQRT)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/sqrt";
         }
         else if (id == ARNOLD_NODEID_SHADER_DOT)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/dot";
         }
         else if (id == ARNOLD_NODEID_SHADER_LOG)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/log";
         }
         else if (id == ARNOLD_NODEID_SHADER_EXP)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/exp";
         }
         else if (id == ARNOLD_NODEID_SHADER_CROSS)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/cross";
         }
         else if (id == ARNOLD_NODEID_SHADER_LENGTH)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/length";
         }
         else if (id == ARNOLD_NODEID_SHADER_NORMALIZE)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/normalize";
         }
         else if (id == ARNOLD_NODEID_SHADER_MODULO)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/modulo";
         }
         else if (id == ARNOLD_NODEID_SHADER_FRACTION)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/fraction";
         }
         else if (id == ARNOLD_NODEID_SHADER_CLAMP)
         {
             creator = CArnoldShaderNode::creator;
             initialize = CArnoldShaderNode::initialize;
             abstract = &CArnoldShaderNode::s_abstract;
             if (drawdbClassification.numChars() == 0)
                 drawdbClassification = ":drawdb/shader/utility/math/arnold/clamp";
         }
        else if (id == ARNOLD_NODEID_SHADER_FLAT)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
               drawdbClassification = ":drawdb/shader/surface/flat";
         }
        /*else if (id == ARNOLD_NODEID_SHADER_MIX)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
               drawdbClassification = ":drawdb/shader/surface/mix";
         }*/
        else if (id == ARNOLD_NODEID_SHADER_ROUND_CORNERS)
         {
            creator = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
               drawdbClassification = ":drawdb/shader/surface/round_corners";
         }
         else
         {
            creator    = CArnoldShaderNode::creator;
            initialize = CArnoldShaderNode::initialize;
            abstract   = &CArnoldShaderNode::s_abstract;
            if (drawdbClassification.numChars() == 0)
               drawdbClassification = ":drawdb/shader/surface/arnold/genericShader";
         }
         type = MPxNode::kDependNode;
      }
      else if (arnoldNodeTypeName == str::shape)
      {
         if (AiMetaDataGetBool(arnoldNodeEntry, AtString(), str::maya_procedural, &isAutoProcedural) && isAutoProcedural)
         {
            creator    = CArnoldProceduralNode::creator;
            initialize = CArnoldProceduralNode::initialize;
            abstract   = &CArnoldProceduralNode::s_abstract;
            classification = MString("drawdb/subscene/arnold/procedural/") + MString(arnoldNodeTypeName.c_str());
         }
      } else if (arnoldNodeTypeName == str::_operator)
      {
         creator    = CArnoldOperatorNode::creator;
         initialize = CArnoldOperatorNode::initialize;
         abstract   = &CArnoldOperatorNode::s_abstract;

      } else if (arnoldNodeTypeName == str::driver)
      {
         // Special case for drivers
         AtString subtypeMtd;
         
         if (AiMetaDataGetStr(arnoldNodeEntry, AtString(), str::subtype, &subtypeMtd) && subtypeMtd == str::imager)
         {         
            creator    = CArnoldImagerNode::creator;
            initialize = CArnoldImagerNode::initialize;
            abstract   = &CArnoldImagerNode::s_abstract;
            SetName(toMayaStyle(MString("ai_") + node));
         }
      }
   }
   // classification string if none is stored
   if (classification.numChars() == 0)
   {
      // classification metadata
      AtString classificationMtd;
      if (!AiMetaDataGetStr(arnoldNodeEntry, AtString(), str::maya_classification, &classificationMtd))
      {
         classificationMtd = AtString(CLASSIFY_SHADER.asChar());
      }
      
      if(classificationMtd == str::light__filter)
      {
         AtString lights;
         if (AiMetaDataGetStr(arnoldNodeEntry, AtString(), str::maya_lights, &lights))
         {
            MString cmd = "from mtoa.lightFilters import addLightFilterClassification;addLightFilterClassification('" + MString(lights) + "','"+ MString(name) +"')";
            CHECK_MSTATUS(MGlobal::executePythonCommand(cmd));
         }
      }
      // should we use swatch to preview this node
      // bool doSwatch = (!isAutoProcedural); // we don't want swatches on procedurals by default
      // AiMetaDataGetBool(arnoldNodeEntry, NULL, "maya.swatch", &doSwatch); 
      // doSwatch = true;

      if (strlen(classificationMtd))
      {
         classification = MString(classificationMtd);
         classification += MString(":") + ARNOLD_CLASSIFY(classification);
         classification += MString(":swatch/") + ARNOLD_SWATCH;
      }
      if (drawdbClassification.numChars() != 0)
         classification += drawdbClassification;
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
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa] ["+provider+"] [node "+name+"] Registered AOV \""+aovName+"\"");
   
   m_aovs.push_back(data);
}
