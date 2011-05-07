#include "nodes/ShaderUtils.h"
#include "nodes/shaders/surface/ArnoldCustomShader.h"
#include "nodes/shaders/light/ArnoldSkyDomeLightShader.h"
#include "scene/Lights.h"
#include "scene/Geometry.h"
#include "Extension.h"


#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MSceneMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MTypes.h>
#include <maya/MFnPlugin.h>
#include <maya/MFileObject.h>
#if MAYA_API_VERSION < 201200
   #include "utils/MNodeClass.h"
#else
   #include <maya/MNodeClass.h>
#endif

// --------- CExtension -------------//


MCallbackId CExtension::s_pluginLoadedCallbackId;
MString CExtension::s_swatchRenderer(ARNOLD_SWATCH);
unsigned int CExtension::s_autoNodeId(ARNOLD_NODEID_AUTOGEN);
LoadedArnoldPluginsSet CExtension::s_allLoadedArnoldPlugins;

CExtension::CExtension(const MString &path)
{
   MFileObject extensionFile;
   extensionFile.setRawFullName(path);
   m_extensionFile = extensionFile.rawFullName();
   m_extensionName = extensionFile.rawName();
   m_loaded = false;
   m_registered = false;
}

/// Load an Arnold plugin.

/// Loads the Arnold plugin and registers a Maya node for each Arnold node
/// contained within it, if applicable.
///
///
/// @param path  the absolute path to an Arnold plugin
///
MStatus CExtension::LoadArnoldPlugin(const MString &path)
{
#ifdef _WIN32
   char buffer[MAX_PATH];
   GetFullPathName(path.asChar(), MAX_PATH, buffer, NULL);
   MString pluginPath(buffer);
#else
   MString pluginPath(path);
#endif
   MFileObject pluginFile;
   pluginFile.setRawFullName(pluginPath);
   MString resolvedFull = pluginFile.resolvedFullName();

   if (MStatus::kSuccess == DoRegisterArnoldPlugin(resolvedFull))
   {
      // TODO: add error handling when solid angle adds a status result
      AiLoadPlugin(resolvedFull.asChar());
      return RegisterNodes(resolvedFull);
   }
   else
   {
      AiMsgWarning("Arnold plugin already loaded: %s", resolvedFull);
      return MStatus::kSuccess;
   }
}

/// Get the list of all Arnold plugins this extension has loaded
MStringArray CExtension::GetOwnLoadedArnoldPlugins()
{
   MStringArray result;
   LoadedArnoldPluginsSet::iterator pluginIt;
   for (pluginIt = m_ownLoadedArnoldPlugins.begin();
       pluginIt != m_ownLoadedArnoldPlugins.end();
       pluginIt++)
   {
      result.append(pluginIt->c_str());
   }

   return result;
}

/// Register corresponding Maya nodes for Arnold nodes provided by the given Arnold Plugin.
///
/// @param arnoldNodeName arnold AtNodeEntry from which to generate the new Maya node
///
/// @param path  the resolved absolute path to an Arnold plugin
///
MStatus CExtension::RegisterNodes(const MString &pluginPath)
{
   MStatus status(MStatus::kSuccess);

   AiMsgInfo("Extension %s generating nodes for Arnold plugin %s", m_extensionName.asChar(), pluginPath.asChar());

   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
      const char* nentryFile = AiNodeEntryGetFilename(nentry);
      if (strcmp(nentryFile, pluginPath.asChar()) == 0)
      {
         MStatus nodeStatus;
         nodeStatus = RegisterNode(nentry);
         if (MStatus::kSuccess == nodeStatus)
         {
            nodeStatus = RegisterTranslator(nentry);
         }
         if (MStatus::kSuccess != nodeStatus) status = nodeStatus;
      }
   }

   return status;
}

/// Register a Maya node for the given Arnold node
///
/// Certain optional node-level metadata can be used to control how the
/// node factory processes the node's registration:
///  -# "maya.name" - the name for the generated or corresponding maya node
///  -# "maya.id" - the maya node id to use for the generated or corresponding maya node
///  -# "maya.class" - classification string (defaults to "shader/surface")
///  -# "maya.hide" - skip registration altogether
///
/// See CBaseAttrHelper for parameter-level metadata for controlling attribute creation
///
/// @param arnoldNodeName arnold AtNodeEntry from which to generate the new Maya node
///
/// @return MStatus::kSuccess if the node is registered successfully, else MStatus::kFailure
///
MStatus CExtension::RegisterNode(const MString &arnoldNodeName,
                                     const MString &mayaTypeName,
                                     const MTypeId &mayaTypeId,
                                     MCreatorFunction creatorFunction,
                                     MInitializeFunction initFunction,
                                     MPxNode::Type type,
                                     const MString *classification)
{
   const AtNodeEntry* arnoldNodeEntry = AiNodeEntryLookUp(arnoldNodeName.asChar());
   if (arnoldNodeEntry==NULL)
   {
      AiMsgError("Arnold node %s doesn't exist", arnoldNodeName);
      return MStatus::kFailure;
   }
   else
   {
      return RegisterNode(arnoldNodeEntry,
                          mayaTypeName,
                          mayaTypeId,
                          creatorFunction,
                          initFunction,
                          type,
                          classification);
   }
}

/// Register a Maya node for the given Arnold node
///
/// Certain optional node-level metadata can be used to control how the
/// node factory processes the node's registration:
///  -# "maya.name" - the name for the generated or corresponding maya node
///  -# "maya.id" - the maya node id to use for the generated or corresponding maya node
///  -# "maya.class" - classification string (defaults to "shader/surface")
///  -# "maya.hide" - skip registration altogether
///
/// See CBaseAttrHelper for parameter-level metadata for controlling attribute creation
///
/// @param arnoldNodeEntry  arnold AtNodeEntry from which to generate the new Maya node
///
/// @return MStatus::kSuccess if the node is registered successfully, else MStatus::kFailure
///
MStatus CExtension::RegisterNode(const AtNodeEntry* arnoldNodeEntry,
                                     const MString &mayaTypeName,
                                     const MTypeId &mayaTypeId,
                                     MCreatorFunction creatorFunction,
                                     MInitializeFunction initFunction,
                                     MPxNode::Type type,
                                     const MString *classification)
{
   // should the node be ignored?
   // TODO : let caller check this
   AtBoolean hide;
   if (AiMetaDataGetBool(arnoldNodeEntry, NULL, "maya.hide", &hide) && hide)
      return MStatus::kSuccess;

   MString arnoldNodeName(AiNodeEntryGetName(arnoldNodeEntry));
   CPxMayaNode mayaNode(mayaTypeName,
                        mayaTypeId,
                        m_extensionFile,
                        creatorFunction,
                        initFunction,
                        type,
                        classification);
   // default translator
   // CPxTranslator translator("default", m_extensionFile);
   // If Maya node type name and ids were not specified
   if (mayaNode.name.numChars() == 0)
   {
      // map to an existing maya node?
      // then just add it to the list, manager won't creat it if existing
      // or no creator function is provided
      // get maya type name from metadata
      MString mayaNodeName;
      const char* mayaNodeNameMtd;
      if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.name", &mayaNodeNameMtd))
      {
         mayaNodeName = MString(mayaNodeNameMtd);
      }
      else
      {
         mayaNodeName = toMayaStyle(MString("ai_")+arnoldNodeName);
      }
      mayaNode.name = mayaNodeName;
   }
   if (mayaNode.id.id() == 0)
   {
      // get maye typeId from metadata
      AtInt nodeId;
      if (!AiMetaDataGetInt(arnoldNodeEntry, NULL, "maya.id", &nodeId))
      {
         nodeId = s_autoNodeId;
         // TODO: print hex nodeId
         AiMsgWarning("Assigning temporary node id %s to %s", nodeId, arnoldNodeName);
         s_autoNodeId++;
      }
      mayaNode.id = MTypeId(nodeId);
   }
   // Class methods to use to create the Maya node
   // TODO : use some map to make less hardcoded (BuiltinMayaNodes or BuiltinMayaTranslators)
   // we could also let mayaNode.type to MPxNode::kLast et let manager
   // handle this, but having access to builting classes for subclassing
   // might be useful to extensions anyway.
   if (NULL == mayaNode.creator)
   {
      AtInt arnoldNodeType = AiNodeEntryGetType(arnoldNodeEntry);
      const char* arnoldNodeTypeName = AiNodeEntryGetTypeName(arnoldNodeEntry);
      if ("light" == arnoldNodeTypeName)
      {
         // TODO : define a base light class
         // mayaNode.creator = CArnoldSkyDomeLightShaderNode::creator;
         // mayaNode.initialize = CArnoldSkyDomeLightShaderNode::initialize;
         mayaNode.type = MPxNode::kLocatorNode;
      }
      else if ("shader" == arnoldNodeTypeName)
      {
         mayaNode.creator = CArnoldCustomShaderNode::creator;
         mayaNode.initialize = CArnoldCustomShaderNode::initialize;
         // When creating
         // CArnoldCustomShaderNode::s_shaderName = arnoldNodeName;
         // CArnoldCustomShaderNode::s_shaderClass = shaderClass;
         mayaNode.type = MPxNode::kDependNode;
      }
      else if ("shape" == arnoldNodeTypeName)
      {
         // TODO : can be expanded to allow base custom shape too
         // can easily add this to CPxMayaNode
         // MCreatorFunction     uiCreatorFunction,
      }
      // No default strategy to create the rest, but creator and initialize will be passed
      // explicitely then
   }
   // Maya type the node will be derived from
   // TODO : use some map to make less hardcoded (BuiltinMayaNodes or BuiltinMayaTranslators)
   // we could also let mayaNode.type to MPxNode::kLast et let manager
   // handle this, but having access to builting classes for subclassing
   // might be useful to extensions anyway.
   if (MPxNode::kLast == mayaNode.type)
   {
      // Get node base (MPxNode::Type) if not specified
      MPxNode::Type nodeType = MPxNode::kDependNode;
      const char* nodeTypeMtd;
      if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.type", &nodeTypeMtd))
      {
         if ("kLocatorNode" == nodeTypeMtd)
         {
            nodeType = MPxNode::kLocatorNode;
         }
         else if ("kDependNode" == nodeTypeMtd)
         {
            nodeType = MPxNode::kDependNode;
         }
         else
         {
            AiMsgError("Unknwown Maya type %s for node %s", nodeTypeMtd, arnoldNodeName.asChar());
            return MStatus::kFailure;
         }
      }
      mayaNode.type = nodeType;
   }
   // classification string
   if (NULL == mayaNode.classification)
   {
      // classification metadata
      const char* classificationMtd;
      if (!AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.classification", &classificationMtd))
         classificationMtd = CLASSIFY_SHADER.asChar();
      // should we use swatch to preview this node
      AtBoolean doSwatch;
      if (!AiMetaDataGetBool(arnoldNodeEntry, NULL, "maya.swatch", &doSwatch))
         doSwatch = true;
      MString classification("");
      if ("" != classificationMtd)
      {
         classification = MString(classificationMtd);
         classification += MString(":") + ARNOLD_CLASSIFY(classification);
         if (doSwatch)
         {
            classification += MString(":swatch/") + s_swatchRenderer;
         }
      }
      if (classification.numChars())
      {
         mayaNode.classification = new MString(classification);
      }
   }

   // Add the node proxy to the lists
   return DoRegisterMayaNode(mayaNode, arnoldNodeName);
   // Register a default translator for this node type
   // default translator
   /*
   CPxTranslator translator(
         const MString &translatorName,
         m_extensionFile,
         arnoldNodeName,
         TCreatorFunction creatorFunction,
         TInitializeFunction nodeInitFunction)
   */
}

// To create a Maya node without an arnold counterpart
MStatus CExtension::RegisterNode(const MString &mayaTypeName,
                                     const MTypeId &mayaTypeId,
                                     MCreatorFunction creatorFunction,
                                     MInitializeFunction initFunction,
                                     MPxNode::Type type,
                                     const MString *classification)
{
   CPxMayaNode mayaNode(mayaTypeName,
                        mayaTypeId,
                        m_extensionFile,
                        creatorFunction,
                        initFunction,
                        type,
                        classification);
   return DoRegisterMayaNode(mayaNode);
}

// Old method for compatibility, it doesn't allow
// access to metadatas
// TODO : depreciate it
MStatus CExtension::RegisterTranslator(const MString &mayaTypeName,
                           const MTypeId &mayaTypeId,
                           const MString translatorName,
                           TCreatorFunction creatorFunction,
                           TNodeInitFunction nodeInitFunction)
{
   CPxMayaNode mayaNode(mayaTypeName, mayaTypeId, m_extensionFile);
   CPxTranslator translator(translatorName, m_extensionFile, "", creatorFunction, nodeInitFunction);

   return DoRegisterTranslator(translator, mayaNode);
}

// Register translator
MStatus CExtension::RegisterTranslator(const MString &arnoldNodeName,
                                       const MString &mayaTypeName,
                                       const MString translatorName,
                                       TCreatorFunction creatorFunction,
                                       TNodeInitFunction nodeInitFunction)
{
   const AtNodeEntry* arnoldNodeEntry = AiNodeEntryLookUp(arnoldNodeName.asChar());
   if (arnoldNodeEntry==NULL)
   {
      AiMsgError("Arnold node %s does not exist", arnoldNodeName.asChar());
     return MStatus::kFailure;
   }
   else
   {
      return RegisterTranslator(arnoldNodeEntry,
                                mayaTypeName,
                                translatorName,
                                creatorFunction,
                                nodeInitFunction);
   }
}

// Register translator
MStatus CExtension::RegisterTranslator(const AtNodeEntry* arnoldNodeEntry,
                                       const MString &mayaTypeName,
                                       const MString translatorName,
                                       TCreatorFunction creatorFunction,
                                       TNodeInitFunction nodeInitFunction)
{
   CPxMayaNode mayaNode(mayaTypeName, MTypeId(0), m_extensionFile);
   CPxTranslator translator(translatorName, m_extensionFile, arnoldNodeEntry, creatorFunction, nodeInitFunction);

   // If no explicit translator was specified, choose a default one using
   // Arnold node type and metadata
   if (NULL == translator.creator)
   {
      const char* arnoldNodeTypeName;
      if (!AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.translator", &arnoldNodeTypeName))
         arnoldNodeTypeName = AiNodeEntryGetTypeName(arnoldNodeEntry);
      if ("light" == arnoldNodeTypeName)
      {
         // TODO : define a non virtual CLightTranslator
         translator.creator = CPointLightTranslator::creator;
         translator.initialize = CPointLightTranslator::NodeInitializer;
      }
      else if ("shader" == arnoldNodeTypeName)
      {
         // TODO : define a non virtual CNodeTranslator
         translator.creator = CAutoTranslator::creator;
         // translator.initialize = CAutoTranslator::NodeInitializer;
      }
      else if ("shape" == arnoldNodeTypeName)
      {
         // TODO : define a non virtual CShapeTranslator or Geo
         translator.creator = CMeshTranslator::creator;
         translator.initialize = CMeshTranslator::NodeInitializer;
      } else {
         // Some default generic translator (CNodeTranslator) ?
      }
      // No default strategy to create the rest, but creator and initialize will be passed
      // explicitely then
   }
   // TODO : use classification to refine default translator choice?
   // classification string
   if (NULL == translator.creator)
   {
      // classification metadata
      const char* classificationMtd;
      if (!AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.classification", &classificationMtd))
         classificationMtd = CLASSIFY_SHADER.asChar();
      MString classification = MString(classificationMtd);
      // choose a translator
   }

   return DoRegisterTranslator(translator, mayaNode);
}

// ------------- protected --------------- //

/// Register the given Arnold plugin in the list of
/// plugins loaded by this extension.
///
///
/// @param path  the absolute path to an Arnold plugin
///
MStatus CExtension::DoRegisterArnoldPlugin(const MString &path)
{
   std::string path_str(path.asChar());
   // std::string extensionPath(m_extensionFile.asChar())

   LoadedArnoldPluginsSet::iterator pluginIt;
   pluginIt = s_allLoadedArnoldPlugins.find(path_str);
   if (pluginIt != s_allLoadedArnoldPlugins.end())
   {
      return MStatus::kFailure;
   }
   else
   {
      s_allLoadedArnoldPlugins.insert(path_str);
      m_ownLoadedArnoldPlugins.insert(path_str);
      return MStatus::kSuccess;
   }
}

// internal use
// Adds the CPxMayaNode to the list that will be handled by the extension manager
MStatus CExtension::DoRegisterMayaNode(const CPxMayaNode &mayaNode,
                                       const CPxArnoldNode &arnoldNode)
{
   if (arnoldNode.isNull())
   {
      AiMsgInfo("Extension %s registering Maya node %s", mayaNode.provider.asChar(), mayaNode.name.asChar());
   }
   else
   {
      AiMsgInfo("Extension %s associating Maya node %s to Arnold node %s", mayaNode.provider.asChar(), mayaNode.name.asChar(), arnoldNode.name.asChar());
   }

   // TODO: Allow override instead of failing?
   const CPxMayaNode *existingMayaNode = NULL;
   if (arnoldNode.isNull())
   {
      existingMayaNode = FindAssociatedMayaNode(arnoldNode);
   }
   else
   {
      existingMayaNode = FindRegisteredMayaNode(mayaNode);
   }
   if (NULL == existingMayaNode)
   {
      if (!arnoldNode.isNull())
      {
         // If it's associated to an Arnold node, store the info
         m_arnoldToMayaNodes[arnoldNode] = mayaNode;
         AiMsgInfo("Extension %s associated Maya node %s to Arnold node %s", mayaNode.provider.asChar(), mayaNode.name.asChar(), arnoldNode.name.asChar());
      }
      if (!MFnPlugin::isNodeRegistered(mayaNode.name))
      {
         // If it's a new node that MtoA will need to register with Maya, store it
         m_registeredMayaNodes.insert(mayaNode);

         AiMsgInfo("Extension %s registered Maya node %s", mayaNode.provider.asChar(), mayaNode.name.asChar());
      }
      // Add a translators empty entry until some are possibly registered for that node
      m_registeredTranslators[mayaNode] = TranslatorsSet();

      return MStatus::kSuccess;
   }
   else
   {
      if (arnoldNode.isNull())
      {
         AiMsgError("Extension %s already registered Maya node %s", mayaNode.provider.asChar(), mayaNode.name.asChar());
      }
      else
      {
         AiMsgError("Extension %s already associated Maya node %s to Arnold node %s", mayaNode.provider.asChar(), mayaNode.name.asChar(), arnoldNode.name.asChar());
      }
      return MStatus::kFailure;
   }
}

// internal use
MStatus CExtension::DoRegisterTranslator(const CPxTranslator &translator,
                                         const CPxMayaNode &mayaNode)
{
   if (NULL == translator.creator)
   {
      AiMsgError("Can't register translator %s without a creator method", translator.name.asChar());
      return MStatus::kFailure;
   }
   MStatus status;
   MayaNodeToTranslatorsMap::iterator nodeIt;
   nodeIt = m_registeredTranslators.find(mayaNode);
   if (nodeIt != m_registeredTranslators.end())
   {
      TranslatorsSet* nodeTranslators;
      nodeTranslators = &nodeIt->second;
      // Add to this node's translators
      std::pair<TranslatorsSet::iterator, bool> ret;
      ret = nodeTranslators->insert(translator);
      if (true == ret.second)
      {
         status = MStatus::kSuccess;
      }
      else
      {
         // We can only override our own translators (if using the same name),
         // extensions adding to other extensions translators will be checked by
         // CExtensionsManager when loading all extensions in sequence.
         *ret.first = translator;
         // AiMsgWarning("Overriding translator %s for Maya node %s), translatorName, mayaNode);
         status = MStatus::kSuccess;
      }
   }
   else
   {
      // If the node exists, or we registered it, we can just add the translator
      if (MFnPlugin::isNodeRegistered(mayaNode.name)
         || (NULL != FindRegisteredMayaNode(mayaNode)))
      {
         TranslatorsSet nodeTranslators = TranslatorsSet();
         nodeTranslators.insert(translator);
         m_registeredTranslators[mayaNode] = nodeTranslators;
      }
      else
      {
         // TODO: reimplement the deferred provided by plugin thing
         /*
         if (strlen(providedByPlugin) != 0)
         {
            // can't add the callback if the node type is unknown
            // make the callback when the plugin is loaded
            CMayaPluginData pluginData;
            pluginData.mayaNode = mayaNode;
            pluginData.translatorName = translatorName;
            pluginData.nodeClassInitializer = nodeClassInitializer;
            m_mayaPluginData[providedByPlugin].push_back(pluginData);
         }
         */
      }
   }

   // Caller will have to do that
   // translator.nodeInitializer(mayaNode);
   // Caller will do this when collecting registered translators
   // create the "arnoldTranslator" attribute
   /*
   CExtensionAttrHelper helper(mayaNode);
   CAttrData data;
   data.defaultValue.STR = "";
   data.name = "arnoldTranslator";
   data.shortName = "arntr";
   helper.MakeInputString(data);
   */

   return status;
}

// Find a registering Maya node
const CPxMayaNode* CExtension::FindRegisteredMayaNode(const CPxMayaNode &mayaNode)
{
   MayaNodesSet::iterator nodeIt;
   nodeIt = m_registeredMayaNodes.find(mayaNode);
   if (nodeIt != m_registeredMayaNodes.end())
   {
      return &(*nodeIt);
   }
   else
   {
      return NULL;
   }
}

// Find a registered Maya node corresponding the the given Arnold node
const CPxMayaNode* CExtension::FindAssociatedMayaNode(const CPxArnoldNode &arnoldNode)
{
   ArnoldNodeToMayaNodeMap::iterator nodeIt;
   nodeIt = m_arnoldToMayaNodes.find(arnoldNode);
   if (nodeIt != m_arnoldToMayaNodes.end())
   {
      return &nodeIt->second;
   }
   else
   {
      return NULL;
   }
}

// Find registered translators for a Maya node
const TranslatorsSet* CExtension::FindRegisteredTranslators(const CPxMayaNode &mayaNode)
{
   MayaNodeToTranslatorsMap::iterator nodeIt;
   nodeIt = m_registeredTranslators.find(mayaNode);
   if (nodeIt != m_registeredTranslators.end())
   {
      return &nodeIt->second;
   }
   else
   {
      return NULL;
   }
}

/// Checks if the given Arnold plugin has been loaded (by any extension).
///
///
/// @param path  the absolute path to an Arnold plugin
///
bool CExtension::IsArnoldPluginLoaded(const MString &path)
{
   std::string path_str(path.asChar());
   LoadedArnoldPluginsSet::iterator pluginIt;
   pluginIt = s_allLoadedArnoldPlugins.find(path_str);
   if (pluginIt != s_allLoadedArnoldPlugins.end())
   {
      return true;
   }
   return false;
}

MStringArray CExtension::GetAllLoadedArnoldPlugins()
{
   MStringArray result;
   LoadedArnoldPluginsSet::iterator pluginIt;
   for (pluginIt = s_allLoadedArnoldPlugins.begin();
       pluginIt != s_allLoadedArnoldPlugins.end();
       pluginIt++)
   {
      result.append(pluginIt->c_str());
   }

   return result;
}

// TODO:  Old stuff, to be reworked

/*

/// Initialize all translators that are waiting for a particular plugin to load
void CExtension::InitializePendingTranslators(MString& pluginName)
{
   // get all the nodes created by this plugin with class initializers
   std::vector<CMayaPluginData>& nodes = m_mayaPluginData[pluginName.asChar()];
   for (unsigned int i=0; i < nodes.size(); i++)
      nodes[i].nodeClassInitializer(nodes[i].mayaNode);

   // TODO: find out if extension attributes persist even if the plugin has been unloaded and then reloaded
   m_mayaPluginData[pluginName.asChar()].clear();
}


/// Called when a plugin is loaded to ensure that each translator that requires
/// node initialization gets a callback installed
void CExtension::MayaPluginLoadedCallback(const MStringArray &strs, void *clientData)
{
   // 0 = pluginPath, 1 = pluginName
   MString pluginName = strs[1];

   // start up the arnold universe so that attribute helpers can query arnold nodes
   bool active = false;
   if (AiUniverseIsActive())
      active = true;
   else
      AiBegin();

   for (unsigned int i=0; i < s_extensions.size(); i++)
      s_extensions[i]->InitializePendingTranslators(pluginName);

   if (!active)
      AiEnd();
}

/// Installs the plugin-loaded callback
void CExtension::CreateCallbacks()
{
   MStatus status;
   // create callbacks
   s_pluginLoadedCallbackId = MSceneMessage::addStringArrayCallback(MSceneMessage::kAfterPluginLoad, CExtension::MayaPluginLoadedCallback, NULL, &status);
   CHECK_MSTATUS(status);
}

void CExtension::RemoveCallbacks()
{
   // delete callbacks
   if (s_pluginLoadedCallbackId != 0)
      MMessage::removeCallback(s_pluginLoadedCallbackId);

   // const MStatus status = MNodeMessage::removeCallbacks(s_mayaCallbackIDs);
   // CHECK_MSTATUS(status);
   // if (status == MS::kSuccess)
   //    s_mayaCallbackIDs.clear();

}

*/




