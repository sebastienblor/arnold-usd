#include "Extension.h"

#include "nodes/ArnoldNodeIDs.h"

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


MCallbackId CExtension::s_pluginLoadedCallbackId(0);
unsigned int CExtension::s_autoNodeId(ARNOLD_NODEID_AUTOGEN);
LoadedArnoldPluginsSet CExtension::s_allLoadedArnoldPlugins;

CExtension::CExtension(const MString &file)
{
   MFileObject extensionFile;
   extensionFile.overrideResolvedFullName(file);
   m_extensionFile = extensionFile.resolvedFullName();
   m_extensionName = extensionFile.resolvedName();
   m_registered = false;
}

/// Count total number of translators
unsigned int CExtension::TranslatorCount() const
{
   unsigned int sum = 0;
   MayaNodeToTranslatorsMap::const_iterator it;
   for (it = m_registeredTranslators.begin();
        it != m_registeredTranslators.end();
        it++)
   {
      sum += it->second.size();
   }
   return sum;
}

/// Load an Arnold plugin.

/// Loads the Arnold plugin and registers a Maya node for each Arnold node
/// contained within it, if applicable.
///
///
/// @param file  the Arnold plugin file name
/// @param path  the search path to use
///
MString CExtension::LoadArnoldPlugin(const MString &file,
                                     const MString &path,
                                     MStatus *returnStatus)
{
   MStatus status;
   MString resolved;
   resolved = FindFileInPath(MString(file), path);
   status = NewArnoldPlugin(resolved);
   if (MStatus::kSuccess == status)
   {
      // TODO: add error handling when solid angle adds a status result
      AiLoadPlugin(resolved.asChar());
      status = MStatus::kSuccess;
   }
   else
   {
      AiMsgWarning("[%s] Arnold plugin already loaded: %s.", m_extensionFile, resolved);
      status = MStatus::kFailure;
   }
   if (NULL != returnStatus) *returnStatus = status;

   return resolved;
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
/// @param plugin  the resolved absolute path to an Arnold plugin
///
MStatus CExtension::RegisterAllNodes(const MString &plugin)
{
   MStatus status(MStatus::kSuccess);

   // Arnold api doc says AiNodeEntryGetFilename returns <buit-in> for
   // built-in nodes, but it seems to return an empty string.
   if (plugin.numChars() == 0)
      AiMsgInfo("[%s] Generating Maya nodes for built-in nodes...", m_extensionName.asChar());
   else
      AiMsgInfo("[%s] Generating Maya nodes for %s...", m_extensionName.asChar(), plugin.asChar());

   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
      const char* nodeName = AiNodeEntryGetName(nentry);
      const char* nodeFile = AiNodeEntryGetFilename(nentry);
      if (strcmp(nodeFile, plugin.asChar()) == 0)
      {
         // AiMsgDebug("[%s] Arnold node %s is provided by %s and will be processed for node registration", m_extensionName.asChar(), nodeName, nodeFile);
         MStatus nodeStatus;
         nodeStatus = RegisterNode(nentry);
         // Only report hard failures, ignore kNotImplemented
         if (MStatus::kSuccess != nodeStatus
               && MStatus::kNotImplemented != nodeStatus)
         {
            status = nodeStatus;
         }
      }
   }
   AiNodeEntryIteratorDestroy(nodeIter);

   // Info
   AiMsgInfo("[%s] Translates %i Maya nodes, including %i new registered Maya nodes.",
         m_extensionName.asChar(), TranslatedCount(), NodesCount());

   return status;
}

/// Register translators for Arnold nodes provided by the given Arnold Plugin.
///
/// @param plugin  the resolved absolute path to an Arnold plugin
///
MStatus CExtension::RegisterAllTranslators(const MString &plugin)
{
   MStatus status(MStatus::kSuccess);

   // Arnold api doc says AiNodeEntryGetFilename returns <buit-in> for
   // built-in nodes, but it seems to return an empty string.
   if (plugin.numChars() == 0)
      AiMsgInfo("[%s] Generating translators for built-in nodes...", m_extensionName.asChar());
   else
      AiMsgInfo("[%s] Generating translators for %s...", m_extensionName.asChar(), plugin.asChar());

   // FIXME: use map instead
   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
      const char* nodeName = AiNodeEntryGetName(nentry);
      const char* nodeFile = AiNodeEntryGetFilename(nentry);
      if (strcmp(nodeFile, plugin.asChar()) == 0)
      {
         // AiMsgDebug("[%s] Arnold node %s is provided by %s and will be processed for translator registration", m_extensionName.asChar(), nodeName, nodeFile);
         MStatus nodeStatus;
         nodeStatus = RegisterTranslator(nentry);
         // Only report hard failures, ignore kNotImplemented
         if (MStatus::kSuccess != nodeStatus
               && MStatus::kNotImplemented != nodeStatus)
         {
            status = nodeStatus;
         }
      }
   }
   AiNodeEntryIteratorDestroy(nodeIter);

   // Info
   AiMsgInfo("[%s] Defines %i translators.", m_extensionName.asChar(), TranslatorCount());

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
                                     CAbMayaNode *abstractMember,
                                     MPxNode::Type type,
                                     const MString &classification)
{
   const AtNodeEntry* arnoldNodeEntry = AiNodeEntryLookUp(arnoldNodeName.asChar());
   if (arnoldNodeEntry==NULL)
   {
      AiMsgError("[%s] Arnold node %s does not exist", m_extensionName.asChar(), arnoldNodeName.asChar());
      return MStatus::kInvalidParameter;
   }
   else
   {
      return RegisterNode(arnoldNodeEntry,
                          mayaTypeName,
                          mayaTypeId,
                          creatorFunction,
                          initFunction,
                          abstractMember,
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
                                     CAbMayaNode *abstractMember,
                                     MPxNode::Type type,
                                     const MString &classification)
{
   MStatus status;
   CPxArnoldNode arnoldNode(arnoldNodeEntry);
   CPxMayaNode mayaNode(mayaTypeName,
                        mayaTypeId,
                        arnoldNodeEntry,
                        m_extensionName,
                        m_extensionFile,
                        creatorFunction,
                        initFunction,
                        abstractMember,
                        type,
                        classification);

   // If the Arnold node is marked as a node that should be ignored
   AtBoolean hide;
   if (AiMetaDataGetBool(arnoldNodeEntry, NULL, "maya.hide", &hide) && hide)
   {
      AiMsgDebug("[%s] [node %s] Marked as hidden from Maya, ignored.", m_extensionName.asChar(), arnoldNode.name.asChar());
      return MStatus::kSuccess;
   }
   // Read from metadata any information that was not explicitely passed
   status = mayaNode.ReadMetaData();

   // We're either creating a new node and mapping it,
   // or mapping an existing one,
   // or ignoring it alltogether if not enough information is present

   if (NULL != mayaNode.creator)
   {
      // Then we're creating (and maping), unless node already exists
      status = NewMappedMayaNode(mayaNode, arnoldNode);
   }
   else if (mayaNode.name != "")
   {
      // Then we're mapping
      status = MapMayaNode(mayaNode, arnoldNode);
   }
   else
   {
      // No error or warning message, because there are many Arnold nodes that are not meant to be associated
      status = MStatus::kNotImplemented;
      AiMsgDebug("[%s] [node %s] Not enough information to associate an existing or new Maya node, ignored.", m_extensionName.asChar(), arnoldNode.name.asChar());
   }

   return status;
}

/// To register a Maya node not corresponding to any Arnold node.
///
MStatus CExtension::RegisterNode(const MString &mayaTypeName,
                                     const MTypeId &mayaTypeId,
                                     MCreatorFunction creatorFunction,
                                     MInitializeFunction initFunction,
                                     MPxNode::Type type,
                                     const MString &classification)
{
   CPxMayaNode mayaNode(mayaTypeName,
                         mayaTypeId,
                         NULL,
                         m_extensionName,
                         m_extensionFile,
                         creatorFunction,
                         initFunction,
                         NULL,
                         type,
                         classification);

   return NewMayaNode(mayaNode);
}

/// Register translator
///
/// Keeping old method signature for compatibility, it doesn't allow access to metadata
MStatus CExtension::RegisterTranslator(const MString &mayaTypeName,
                           const MTypeId &mayaTypeId,
                           const MString &translatorName,
                           TCreatorFunction creatorFunction,
                           TNodeInitFunction nodeInitFunction)
{
   CPxMayaNode mayaNode(mayaTypeName,
                        mayaTypeId,
                        NULL,                     // No Arnold node name available
                        m_extensionName,
                        m_extensionFile);
   CPxTranslator translator(translatorName,
                            NULL,                 // No Arnold node name available
                            m_extensionName,
                            m_extensionFile,
                            creatorFunction,
                            nodeInitFunction);
   AiMsgWarning("[%s] RegisterTranslator(mayaTypeName, mayaTypeId, ...) depreciated, use RegisterTranslator(arnoldNodeName, ...).", m_extensionName.asChar());
   return NewTranslator(translator, mayaNode);
}

/// Register translator
MStatus CExtension::RegisterTranslator(const MString &arnoldNodeName,
                                       const MString &translatorName,
                                       TCreatorFunction creatorFunction,
                                       TNodeInitFunction nodeInitFunction)
{
   const AtNodeEntry* arnoldNodeEntry = AiNodeEntryLookUp(arnoldNodeName.asChar());
   if (arnoldNodeEntry==NULL)
   {
      AiMsgError("[%s] Arnold node %s does not exist, translator needs either a valid Arnold or Maya node",
         m_extensionName.asChar(), arnoldNodeName.asChar());
      return MStatus::kInvalidParameter;
   }
   else
   {
      return RegisterTranslator(arnoldNodeEntry,
                                translatorName,
                                creatorFunction,
                                nodeInitFunction);
   }
}

/// Register translator
MStatus CExtension::RegisterTranslator(const AtNodeEntry* arnoldNodeEntry,
                                       const MString &translatorName,
                                       TCreatorFunction creatorFunction,
                                       TNodeInitFunction nodeInitFunction)
{
   MStatus status;
   CPxArnoldNode arnoldNode(arnoldNodeEntry);
   CPxMayaNode mayaNode("",
                        0,
                        arnoldNodeEntry,
                        m_extensionName,
                        m_extensionFile);
   mayaNode.ReadMetaData();
   CPxTranslator translator(translatorName,
                            arnoldNodeEntry,
                            m_extensionName,
                            m_extensionFile,
                            creatorFunction,
                            nodeInitFunction);
   translator.ReadMetaData();

   if (NULL == translator.creator || mayaNode.IsNull())
   {
      status = MStatus::kNotImplemented;
      AiMsgDebug("[%s] [node %s] Not enough information to register a translator, ignored.",
            m_extensionName.asChar(), arnoldNode.name.asChar());
   }
   else
   {
      // Try to add the new translator to the registered lists
      status = NewTranslator(translator, mayaNode);
      if (MStatus::kSuccess == status)
         AiMsgDebug("[%s] [node %s] Registered translator %s for Maya node %s.",
               m_extensionName.asChar(), arnoldNode.name.asChar(), translator.name.asChar(), mayaNode.name.asChar());
      else
         AiMsgWarning("[%s] [node %s] Failed to register translator %s for Maya node %s.",
               m_extensionName.asChar(), arnoldNode.name.asChar(), translator.name.asChar(), mayaNode.name.asChar());
   }

   return status;
}

/// To specifiy either arnoldNodeName or mayaTypeName
///
/// Since it is used to add an addtionnal translator to a Maya node you need to
/// provide a translator name and a creator method.
MStatus CExtension::RegisterTranslator(const MString &arnoldNodeName,
                                       const MString &mayaTypeName,
                                       const MString &translatorName,
                                       TCreatorFunction creatorFunction,
                                       TNodeInitFunction nodeInitFunction)
{
   // Can have an empty Arnold node name / NULL aiNodeEntry if we don't plan to read metadatas
   // as long as we have a valid mayaTypeName instead
   const AtNodeEntry* arnoldNodeEntry = AiNodeEntryLookUp(arnoldNodeName.asChar());
   if (NULL == arnoldNodeEntry && mayaTypeName == "")
   {
      AiMsgError("[%s] Arnold node '%s' and Maya node '%s' do not exist, translator needs either a valid Arnold or Maya node.",
      m_extensionName.asChar(), arnoldNodeName.asChar(), mayaTypeName.asChar());
      return MStatus::kInvalidParameter;
   }
   else
   {
      const AtNodeEntry* arnoldNodeEntry = AiNodeEntryLookUp(arnoldNodeName.asChar());
      CPxTranslator translator(translatorName,
                               arnoldNodeEntry,
                               m_extensionName,
                               m_extensionFile,
                               creatorFunction,
                               nodeInitFunction);
      CPxMayaNode mayaNode(mayaTypeName,
                           0,
                           arnoldNodeEntry,
                           m_extensionName,
                           m_extensionFile);
      if (NULL != arnoldNodeEntry)
      {
         translator.ReadMetaData();
         mayaNode.ReadMetaData();
      }

      return NewTranslator(translator, mayaNode);
   }
}
// ------------- protected --------------- //


/// Stores the given Arnold plugin file name in the list of
/// plugins loaded by this extension, and updates global
/// list of loaded Arnold plugins.
///
/// Will fail and return MS::kFailure if that plugin is
/// already stored.
///
/// @param file  the resolved filename of an Arnold plugin
///
MStatus CExtension::NewArnoldPlugin(const MString &file)
{
   std::string file_str(file.asChar());
   // std::string extensionPath(m_extensionFile.asChar())

   LoadedArnoldPluginsSet::iterator pluginIt;
   pluginIt = s_allLoadedArnoldPlugins.find(file_str);
   if (s_allLoadedArnoldPlugins.end() != pluginIt)
   {
      // Already loaded (possibly by another extension)
      return MStatus::kFailure;
   }
   else
   {
      s_allLoadedArnoldPlugins.insert(file_str);
      m_ownLoadedArnoldPlugins.insert(file_str);
      return MStatus::kSuccess;
   }
}

/// Removes the given Arnold plugin file name from the list of plugins
/// loaded by this extension, and updates global list of loaded Arnold plugins.
///
/// Will fail and return MS::kFailure if that plugin is not already stored.
///
/// @param file  the resolved filename of an Arnold plugin
///
MStatus CExtension::DeleteArnoldPlugin(const MString &file)
{
   std::string file_str(file.asChar());
   // std::string extensionPath(m_extensionFile.asChar())

   LoadedArnoldPluginsSet::iterator pluginIt;
   pluginIt = s_allLoadedArnoldPlugins.find(file_str);
   if (s_allLoadedArnoldPlugins.end() == pluginIt)
   {
      return MStatus::kFailure;
   }
   else
   {
      s_allLoadedArnoldPlugins.erase(file_str);
      m_ownLoadedArnoldPlugins.erase(file_str);
      return MStatus::kSuccess;
   }
}

/// internal use
/// Register a new Maya node and map it to an Arnold node.
///
/// Store the information
MStatus CExtension::NewMappedMayaNode(CPxMayaNode mayaNode,
                                       const CPxArnoldNode &arnoldNode)
{
   MStatus status;

	if ((NULL == mayaNode.creator) || arnoldNode.IsNull())
   {
      AiMsgError("[%s] Not enough information to register a new Maya node for an existing Arnold node.", m_extensionName.asChar());
      return MStatus::kFailure;
   }
   // If we are going to create a node, check that we can,
	// it doesnt' exist and we have all required information or use defaults
   if (mayaNode.name == "")
   {
      mayaNode.name = toMayaStyle(MString("ai_")+arnoldNode.name);
      AiMsgWarning("[%s] [node %s] Using auto generated associated Maya type name %s.",
            mayaNode.provider.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());
   }
   if (!MFnPlugin::isNodeRegistered(mayaNode.name))
   {
      // If we don't have a valid nodeId, we'll generate a temporary one.
      // There is no guarantee that it won't collide with some node id another instance of MtoA.
      if (mayaNode.id.id() == 0)
      {
         mayaNode.id = MTypeId(s_autoNodeId++);
         AiMsgWarning("[%s] [node %s] Assigning temporary node id %i to associated Maya type name %s.",
               mayaNode.provider.asChar(), mayaNode.id.id(), arnoldNode.name.asChar(), mayaNode.name.asChar());
      }
      // Use kDependNode as default
      if (MPxNode::kLast == mayaNode.type)
      {
         mayaNode.type = MPxNode::kDependNode;
      }
      // Register it for creation
      status = NewMayaNode(mayaNode);
   }
   else
   {
      AiMsgDebug("[%s] [node %s] Maya node %s is already an existing Maya node, it will be associated.",
            mayaNode.provider.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());
      status = MStatus::kSuccess;
   }
   if (MStatus::kSuccess == status)
   {
      // Now associate it with the Arnold node
      status = MapMayaNode(mayaNode, arnoldNode);
   }

   return status;
}

// internal use
/// Register a new Maya node not corresponding to any Arnold node.
/// Typically helper nodes that don't need to be directly translated.
///
/// Store the information
MStatus CExtension::NewMayaNode(CPxMayaNode mayaNode)
{
   // Need all necessary creation information
   if ((mayaNode.name == "") || (mayaNode.id.id() == 0) || (NULL == mayaNode.creator))
   {
      AiMsgError("[%s] Not enough information to register a new Maya node.", m_extensionName.asChar());
      return MStatus::kFailure;
   }
   if (MFnPlugin::isNodeRegistered(mayaNode.name))
   {
      AiMsgError("[%s] Failed to register Maya node %s for creation: already exists.",
            mayaNode.provider.asChar(), mayaNode.name.asChar());
      return MStatus::kFailure;
   }
   // Extensions can freely override their own declarations, for conflicting declarations
   // between different extensions, it is handled by the the extensions manager
   std::pair<MayaNodesSet::iterator, bool> ret;
   ret = m_registeredMayaNodes.insert(mayaNode);
   if (false == ret.second)
   {
      AiMsgDebug("[%s] Overriding it's own registration of Maya node %s for creation.",
            mayaNode.provider.asChar(), mayaNode.name.asChar());
      *ret.first = mayaNode;
   }
   else
   {
      AiMsgDebug("[%s] Registered new Maya node %s for creation.",
            mayaNode.provider.asChar(), mayaNode.name.asChar());
   }

   return MStatus::kSuccess;
}

// Associate an existing Maya node to an Arnold node, and store the information
MStatus CExtension::MapMayaNode(const CPxMayaNode &mayaNode,
                                       const CPxArnoldNode &arnoldNode)
{
   MStatus status;

	if (mayaNode.IsNull() || arnoldNode.IsNull())
   {
      AiMsgError("[%s] Not enough information to map an existing Maya node to an Arnold node.", m_extensionName.asChar());
      return MStatus::kFailure;
   }

	ArnoldNodeToMayaNodeMap::iterator it;
	std::pair<ArnoldNodeToMayaNodeMap::iterator, ArnoldNodeToMayaNodeMap::iterator> range;
	range = m_arnoldToMayaNodes.equal_range(arnoldNode);
	for (it = range.first; it != range.second; it++)
	{
	   if ((*it).second == mayaNode)
	   {
	      AiMsgDebug("[%s] [node %s] Overriding it's own association of Maya node %s.",
            mayaNode.provider.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());
	      (*it).second = mayaNode;
	      // TODO: clean translators if we did a override?
	      // AiMsgWarning("[%s] [node %s] Failed to associate existing Maya node %s.", m_extensionName.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());

	      return MStatus::kSuccess;
	   }
	}
	// Not found, add it
	it = m_arnoldToMayaNodes.insert(range.first, std::make_pair(arnoldNode, mayaNode));
	// Add an empty translator set for that node
   MayaNodeToTranslatorsMap::iterator nodeIt;
   nodeIt = m_registeredTranslators.find(mayaNode);
   if (nodeIt == m_registeredTranslators.end())
   {
      m_registeredTranslators[mayaNode] = TranslatorsSet();
   }

   AiMsgDebug("[%s] [node %s] Associated Maya node %s.", m_extensionName.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());
	return MStatus::kSuccess;
}

// internal use
MStatus CExtension::NewTranslator(const CPxTranslator &translator,
                                         const CPxMayaNode &mayaNode)
{
   if (NULL == translator.creator || mayaNode.IsNull())
   {
      AiMsgError("[%s] Not enough information to register a new translator.", m_extensionName.asChar());
      return MStatus::kFailure;
   }

   MStatus status;
   TranslatorsSet nodeTranslators;
   MayaNodeToTranslatorsMap::iterator nodeIt;
   nodeIt = m_registeredTranslators.find(mayaNode);
   if (nodeIt != m_registeredTranslators.end())
   {
      nodeTranslators = nodeIt->second;
   }
   // Add to this node's translators
   std::pair<TranslatorsSet::iterator, bool> ret;
   ret = nodeTranslators.insert(translator);
   if (true == ret.second)
   {
      if (nodeTranslators.begin() == ret.first)
      {
         // First translator to be create 
         AiMsgDebug("[%s] [node %s] Registered the translator %s for associated Maya node %s.",
               translator.provider.asChar(), translator.arnold.asChar(), translator.name.asChar(), mayaNode.name.asChar());
      }
      else
      {
         // There was already at least one
         AiMsgDebug("[%s] [node %s] Registered an alternative translator %s for associated Maya node %s.",
               translator.provider.asChar(), translator.arnold.asChar(), translator.name.asChar(), mayaNode.name.asChar());
      }
   }
   else
   {
      // We can only override our own translators (if using the same name),
      *ret.first = translator;
      AiMsgDebug("[%s] [node %s] Replaced translator %s for associated Maya node %s.",
            translator.provider.asChar(), translator.arnold.asChar(), translator.name.asChar(), mayaNode.name.asChar());
      
   }
   m_registeredTranslators[mayaNode] = nodeTranslators;
   status = MStatus::kSuccess;
   return status;
}
   
/// Find a registering Maya node
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

/// Find registered translators for a Maya node
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

/// Find file in the given search path(s)
///
///
MString CExtension::FindFileInPath(const MString &file,
                                   const MString &path,
                                   MStatus *returnStatus)
{
   MStatus status = MStatus::kNotFound;
   MString resolved;

   MFileObject fileObject;
   fileObject.setRawName(file);
   fileObject.setRawPath(path);
   unsigned int nbSearchPath = fileObject.pathCount();
   for (unsigned int i=0; i<nbSearchPath; i++)
   {
      if (fileObject.exists(i))
      {
         resolved = fileObject.ithFullName(i); 
         AiMsgDebug("File %s found at %s.", file.asChar(), resolved.asChar());
         status = MStatus::kSuccess;
         break;
      }
   }
   if (MStatus::kSuccess != status)
   {
      AiMsgDebug("File %s not found in path %s.", file.asChar(), path.asChar());
      resolved = "";
   }

   if (NULL != returnStatus) *returnStatus = status;
   return resolved;
}

/// Find all libraries (dll, so) in the given search path.
///
/// @param searchPath  a path to search for libraries, optionally containing
/// separators (: on unix, ; on windows) and environment variables
MStringArray CExtension::FindLibraries(const MString &path,
                                       MStatus *returnStatus)
{
   MStatus status = MStatus::kNotFound;
   MStringArray files;

   MString resolvedPathList = path.expandFilePath();
   MStringArray pluginPaths;
   resolvedPathList.split(PATHSEP, pluginPaths);
   for (unsigned int i=0; i<pluginPaths.length(); ++i)
   {
      MString dir = pluginPaths[i];
      DIR *dp;
      struct dirent *dirp;
      if ((dp  = opendir(dir.asChar())) == NULL)
      {
         // TODO: print more explicit error message
         AiMsgError("Error(%i) opening %s.", errno, dir);
         status = MStatus::kFailure;
         continue;
      }
      while ((dirp = readdir(dp)) != NULL)
      {
         MString entry = dirp->d_name;
         unsigned int nchars = entry.numChars();
         if (nchars > LIBEXT.numChars())
         {
            MString ext = entry.substringW(nchars-LIBEXT.numChars(), nchars);
            if (entry.substringW(0,0) != "." && ext == LIBEXT)
            {
               files.append(dir + DIRSEP + entry);
               if (MStatus::kNotFound == status) status = MStatus::kSuccess;
            }
         }
      }
      closedir(dp);
   }

   if (NULL != returnStatus) *returnStatus = status;
   return files;
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






