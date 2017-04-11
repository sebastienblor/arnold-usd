#include "Extension.h"
#include "PxUtils.h"
#include "common/DynLibrary.h"
#include "nodes/ArnoldNodeIDs.h"

#include "translators/NodeTranslatorImpl.h"
#include "ExtensionImpl.h"
#define MNoVersionString
#define MNoPluginEntry

#include <string>

#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>
#include <ai_nodes.h>
#include "ai_node_entry.h"
#include "ExtensionsManager.h"

#include <maya/MSceneMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MTypes.h>
#include <maya/MFnPlugin.h>
#include <maya/MFileObject.h>
#include <maya/MNodeClass.h>

// --------- CExtension -------------//

unsigned int CExtensionImpl::s_autoNodeId(ARNOLD_NODEID_AUTOGEN);
LoadedArnoldPluginsSet CExtensionImpl::s_allLoadedArnoldPlugins;

CExtension::CExtension(const MString &file)
{
   m_impl = new CExtensionImpl;
   this->m_impl->setFile(file);
   m_impl->m_registered = false;
   m_impl->m_deferred = false;
   m_impl->m_library = NULL;
}

unsigned int CExtension::RegisteredNodesCount() const {return m_impl->m_registeredMayaNodes.size();}
unsigned int CExtension::TranslatedNodesCount() const {return m_impl->m_registeredTranslators.size();}
MString CExtension::GetExtensionName() const {return m_impl->m_extensionName;}
MString CExtension::GetExtensionFile() const {return m_impl->m_extensionFile;}
MString CExtension::GetApiVersion() const {return m_impl->m_apiVersion;}
bool CExtension::IsRegistered() const {return m_impl->m_registered;}
bool CExtension::IsDeferred() const {return m_impl->m_deferred;}


/// Specify that this extenions needs a particular Maya Plugin
void CExtension::Requires(const MString &plugin)
{
   std::string plugin_str(plugin.asChar());
   m_impl->m_requiredMayaPlugins.insert(plugin_str);
}

/// Returns the list of required Maya plugins
MStringArray CExtension::Required()
{
   MStringArray result;
   RequiredMayaPluginsSet::iterator pluginIt;
   for (pluginIt = m_impl->m_requiredMayaPlugins.begin();
       pluginIt != m_impl->m_requiredMayaPlugins.end();
       pluginIt++)
   {
      result.append(pluginIt->c_str());
   }

   return result;
}

/// Count total number of translators
unsigned int CExtension::TranslatorCount() const
{
   unsigned int sum = 0;
   MayaNodeToTranslatorsOldMap::const_iterator it;
   for (it = m_impl->m_registeredTranslators.begin();
        it != m_impl->m_registeredTranslators.end();
        it++)
   {
      sum += it->second.size();
   }
   return sum;
}

/// Load an Arnold plugin.
///
/// Loads the Arnold plugin and registers a Maya nodes and translators for
/// the new Arnold nodes it define, if applicable.
///
/// @param file   The Arnold plugin file name
/// @param path   The search path to use
/// @return       The resolved file name plugin was loaded from
///
MString CExtension::LoadArnoldPlugin(const MString &file,
                                     const MString &path,
                                     MStatus *returnStatus)
{
   MStatus status;

   unsigned int nchars = file.numChars();
   MString libext = MString(LIBEXT);
   unsigned int next = libext.numChars();
   MString searchFile = file;
   if (nchars < next || libext != file.substringW(nchars-next, nchars-1))
   {
      searchFile += libext;
   }
   MString resolved;
   resolved = m_impl->FindFileInPath(searchFile, path, &status);
   if (MStatus::kSuccess == status && resolved.numChars() > 0)
   {
      AiMsgDebug("[mtoa] [%s] Found Arnold plugin file %s as %s.", m_impl->m_extensionName.asChar(), file.asChar(), resolved.asChar());
      status = m_impl->NewArnoldPlugin(resolved);
      if (MStatus::kSuccess == status)
      {
         // TODO: add error handling when solid angle adds a status result
         AiLoadPlugins(resolved.asChar());
         // Register plugin nodes and translators
         status = RegisterPluginNodesAndTranslators(resolved);
      }
   }
   else
   {
      if (path.numChars())
         AiMsgError("[mtoa] [%s] Could not find %s in search path %s", m_impl->m_extensionName.asChar(), file.asChar(), path.asChar());
      else
         AiMsgError("[mtoa] [%s] Could not find %s", m_impl->m_extensionName.asChar(), file.asChar());
   }

   if (NULL != returnStatus) *returnStatus = status;
   return resolved;
}

/// Get the list of all Arnold plugins this extension has loaded.
///
/// @return The array of resolved file name for each plugin
///
MStringArray CExtension::GetOwnLoadedArnoldPlugins()
{
   MStringArray result;
   LoadedArnoldPluginsSet::iterator pluginIt;
   for (pluginIt = m_impl->m_ownLoadedArnoldPlugins.begin();
       pluginIt != m_impl->m_ownLoadedArnoldPlugins.end();
       pluginIt++)
   {
      result.append(pluginIt->c_str());
   }

   return result;
}

/// Manually register a new Maya node.
///
/// @see Maya's MFnPlugin.registerNode(...)
///
/// @return MStatus::kSuccess if the node is registered successfully, else MStatus::kFailure
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
                         m_impl->m_extensionName,
                         m_impl->m_extensionFile,
                         creatorFunction,
                         initFunction,
                         type,
                         classification);

   return m_impl->NewMayaNode(mayaNode);
}

/// Manually register a translator for the given Maya node type.
///
/// Can be manually called in an extension when we need a specific translator
/// for a Maya node.
/// No metadata will be read, everything has to be specified explicitely.
///
/// @param mayaTypeName The Maya node type this translator should handle
///
/// @return MStatus::kSuccess if the node is registered successfully, else MStatus::kFailure
///
MStatus CExtension::RegisterTranslator(const MString &mayaTypeName,
                           const MString &translatorName,
                           TCreatorFunction creatorFunction,
                           TNodeInitFunction nodeInitFunction)
{
   CPxMayaNode mayaNode(mayaTypeName,
                        0,
                        m_impl->m_extensionName,
                        m_impl->m_extensionFile);

   MString transName;
   if (translatorName.numChars() != 0)
      transName = translatorName;
   else
      transName = m_impl->m_extensionName;
   CPxTranslator translator(transName,
                            m_impl->m_extensionName,
                            m_impl->m_extensionFile,
                            creatorFunction,
                            nodeInitFunction);

   return m_impl->NewTranslator(translator, mayaNode);
}


MStatus CExtension::RegisterAOV(const MString &mayaTypeName, const MString &aovName, int dataType, const MString &aovAttr)
{
   CPxMayaNode mayaNode(mayaTypeName);
   MayaNodeToTranslatorsOldMap::iterator nodeIt;
   nodeIt = m_impl->m_registeredTranslators.find(mayaNode);
   if (nodeIt != m_impl->m_registeredTranslators.end())
   {
      mayaNode = nodeIt->first;
      mayaNode.RegisterAOV(aovName, dataType, aovAttr);
      return MS::kSuccess;
   }
   return MS::kFailure;
}

// ------------- protected --------------- //

MStatus CExtensionImpl::setFile(const MString &file)
{
   MFileObject extensionFile;
   extensionFile.overrideResolvedFullName(file);
   m_extensionFile = extensionFile.resolvedFullName();
   MString name = extensionFile.resolvedName();
   // Strip extension if found
   unsigned int nchars = name.numChars();
   MString libext = MString(LIBEXT);
   unsigned int next = libext.numChars();
   if (nchars > next + 1)
   {
      MString ext = name.substringW(nchars-next, nchars-1);
      if (ext == libext)
      {
         name = name.substringW(0, nchars-next-1);
      }
   }
   // TODO some checking ?
   m_extensionName = name;
   return MStatus::kSuccess;
}

/// Unload all Arnold plugins this extensions has loaded
MStatus CExtensionImpl::UnloadArnoldPlugins()
{
   AiMsgDebug("[mtoa] [%s] Unloading all Arnold plugins", m_extensionName.asChar());

   MStatus status = MStatus::kSuccess;

   std::vector<std::string> unloaded;
   LoadedArnoldPluginsSet::iterator pluginIt;
   pluginIt = m_ownLoadedArnoldPlugins.end();
   while (pluginIt != m_ownLoadedArnoldPlugins.begin())
   {
      pluginIt--;
      std::string plugin(*pluginIt);
      MStatus pstatus = DoUnloadArnoldPlugin(plugin.c_str());
      if (MStatus::kSuccess == pstatus)
      {
         unloaded.push_back(plugin);
      }
      else
      {
         status = pstatus;
      }
   }
   // Delete the unloaded plugins from the list
   std::vector<std::string>::iterator it;
   for (it=unloaded.begin(); it!=unloaded.end(); it++)
   {
      MStatus pstatus = DeleteArnoldPlugin(it->c_str());
      if (MStatus::kSuccess != pstatus) status = pstatus;
   }
   return status;
}

/// Unload an Arnold plugin.
///
/// Remove from Arnold all node entries created by the specified plugin.
///
/// @param resolved  the resolved filename of an Arnold plugin
/// @return          MStatus::kSuccess or MStatus::kFailure
///
MStatus CExtensionImpl::UnloadArnoldPlugin(const MString &resolved)
{
   MStatus status = DoUnloadArnoldPlugin(resolved);
   if (MStatus::kSuccess == status)
   {
      status = DeleteArnoldPlugin(resolved);
   }

   return status;
}

MStatus CExtensionImpl::DoUnloadArnoldPlugin(const MString &resolved)
{
   AiMsgDebug("[mtoa] [%s] Unloading Arnold plugin %s", m_extensionName.asChar(), resolved.asChar());
   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
      MString nentryFile = AiNodeEntryGetFilename(nentry);
      const char* nentryFileChar = AiNodeEntryGetFilename(nentry);
      if (nentryFileChar != 0)
         nentryFile = nentryFileChar;
      else
         nentryFile = "";
      if (nentryFile == resolved)
      {
         const char *arnoldNodeName = AiNodeEntryGetName(nentry);
         // remove from arnold
         AiNodeEntryUninstall(arnoldNodeName);
         AiMsgDebug("[mtoa] [%s] Uninstalled Arnold node %s", m_extensionName.asChar(), arnoldNodeName);
         // TODO: unregister as well?
         // DeregisterNode(arnoldNodeName);
      }
   }

   // No real success or failure info returned from Arnold currently
   return MStatus::kSuccess;
}

/// Stores the given Arnold plugin file name in the list of plugins loaded by this extension.
///
/// Will fail and return MS::kFailure if that plugin is already stored.
///
/// @param file      the resolved filename of an Arnold plugin
///
MStatus CExtensionImpl::NewArnoldPlugin(const MString &file)
{
   std::string file_str(file.asChar());
   // std::string extensionPath(m_impl->m_extensionFile.asChar())

   LoadedArnoldPluginsSet::iterator pluginIt;
   pluginIt = s_allLoadedArnoldPlugins.find(file_str);
   if (s_allLoadedArnoldPlugins.end() != pluginIt)
   {
      // Already loaded (possibly by another extension)
      AiMsgError("[mtoa] [%s] Arnold plugin already loaded: %s", m_extensionName.asChar(), file.asChar());
      return MStatus::kFailure;
   }
   else
   {
      AiMsgDebug("[mtoa] [%s] Loads Arnold plugin: %s", m_extensionName.asChar(), file.asChar());
      CExtensionImpl::s_allLoadedArnoldPlugins.insert(file_str);
      m_ownLoadedArnoldPlugins.insert(file_str);
      return MStatus::kSuccess;
   }
}

/// Removes the given Arnold plugin file name from plugins list.
///
/// Plugin will be erased from own plugin list and global list
// of loaded Arnold plugins.
///
/// @param file  the resolved filename of an Arnold plugin
///
MStatus CExtensionImpl::DeleteArnoldPlugin(const MString &file)
{
   std::string file_str(file.asChar());
   // std::string extensionPath(m_impl->m_extensionFile.asChar())

   LoadedArnoldPluginsSet::iterator pluginIt;
   pluginIt = s_allLoadedArnoldPlugins.find(file_str);
   if (s_allLoadedArnoldPlugins.end() == pluginIt)
   {
      AiMsgError("[mtoa] [%s] Arnold plugin not loaded: %s", m_extensionName.asChar(), file.asChar());
      return MStatus::kFailure;
   }
   else
   {
      s_allLoadedArnoldPlugins.erase(file_str);
      m_ownLoadedArnoldPlugins.erase(file_str);
      return MStatus::kSuccess;
   }
}

/// Register corresponding Maya nodes for Arnold nodes provided by the given Arnold plugin.
///
/// Will only handle Arnold nodes that are marked as provided by this plugin.
///
/// @param plugin  the resolved file name of the Arnold plugin
///
MStatus CExtension::RegisterPluginNodesAndTranslators(const MString &plugin)
{
   MStatus status(MStatus::kSuccess);

   MString pluginName = (plugin.numChars()==0) ? "built-in nodes" : MString("plugin ") + plugin;
   AiMsgDebug("[mtoa] [%s] Generating new Maya nodes and translators for Arnold %s.", m_impl->m_extensionName.asChar(), pluginName.asChar());

   unsigned int prevNewNodes = RegisteredNodesCount();
   unsigned int prevTrsNodes = TranslatedNodesCount();
   unsigned int prevTrsCount = TranslatorCount();

   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL /*& ~AI_NODE_SHAPE*/);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
      if (AI_NODE_SHAPE == AiNodeEntryGetType(nentry))
      { 
         if (plugin.numChars() == 0) // builtin
            continue; 
         bool createProcedural;
         if (!(AiMetaDataGetBool(nentry, NULL, "maya.procedural", &createProcedural) && createProcedural))
            continue;

      }
      MString nodeName = AiNodeEntryGetName(nentry);
      /*if (nodeName == "driver_deepexr")
         continue;*/
      MString nodeFile;
      const char* nodeFileChar = AiNodeEntryGetFilename(nentry);
      if (nodeFileChar != 0)
         nodeFile = nodeFileChar;
      else
         nodeFile = "";
      if (nodeFile == plugin)
      {
         // If the Arnold node is marked as a node that should be ignored
         bool hide;
         if (AiMetaDataGetBool(nentry, NULL, "maya.hide", &hide) && hide)
         {
            AiMsgDebug("[mtoa] [%s] [node %s] Marked as hidden.", m_impl->m_extensionName.asChar(), nodeName.asChar());
            continue;
         }

         CPxArnoldNode arnoldNode(nentry);
         CPxMayaNode mayaNode("", // name gets filled by metadata
                              0,  // id gets filled by metadata
                              m_impl->m_extensionName,
                              m_impl->m_extensionFile);
         mayaNode.ReadMetaData(nentry);

         CPxTranslator translator("",
                                  m_impl->m_extensionName,
                                  m_impl->m_extensionFile);
         translator.ReadMetaData(nentry, MFnPlugin::isNodeRegistered(mayaNode.name));

         // Each arnold node may be processed in several ways:
         // - generate a new Maya node and a translator
         //     required:
         //       - that the node is a type that MtoA knows how to auto-translate
         //     optional:
         //       - a unique "maya.id" for the node to be created
         //       - "maya.name"
         // - generate a translator for an existing maya node
         //     required:
         //       - "maya.name" that matches an existing Maya node
         //
         // AiMsgDebug("[mtoa] [%s] Arnold node %s is provided by %s and will be processed for node registration", m_impl->m_extensionName.asChar(), nodeName, nodeFile);
         MStatus nodeStatus;
         nodeStatus = m_impl->RegisterNode(mayaNode, arnoldNode);
         // Only report hard failures, ignore kNotImplemented
         if (MStatus::kSuccess != nodeStatus
               && MStatus::kNotImplemented != nodeStatus)
         {
            status = nodeStatus;
         }
         MStatus translatorStatus;
         translatorStatus = m_impl->RegisterTranslator(translator, mayaNode, arnoldNode);
         // Only report hard failures, ignore kNotImplemented
         if (MStatus::kSuccess != translatorStatus
               && MStatus::kNotImplemented != translatorStatus)
         {
            status = nodeStatus;
         }
         static MString synColorNodeName("color_manager_syncolor");

         // Warning for Arnold nodes that are from plugins and not translated. (Ignoring syncolor)
         if (m_impl->m_extensionName != BUILTIN && arnoldNode.name != synColorNodeName && MStatus::kNotImplemented == translatorStatus)
         {
            AiMsgWarning("[mtoa] [%s] [node %s] There was not enough metadata information to automatically register a translator for that node, ignored.",
               m_impl->m_extensionName.asChar(), arnoldNode.name.asChar());
         }
      }
   }
   AiNodeEntryIteratorDestroy(nodeIter);

   // Info
   unsigned int newNodes = RegisteredNodesCount() - prevNewNodes;
   unsigned int trsNodes = TranslatedNodesCount() - prevTrsNodes;
   unsigned int trsCount = TranslatorCount() - prevTrsCount;

   AiMsgInfo("[mtoa] [%s] Generated %i new Maya nodes for Arnold %s.",
         m_impl->m_extensionName.asChar(), newNodes, pluginName.asChar());
   AiMsgInfo("[mtoa] [%s] Generated %i translators for %i Maya nodes (%i new and %i existing) for Arnold %s.",
         m_impl->m_extensionName.asChar(), trsCount, trsNodes, newNodes, trsNodes - newNodes, pluginName.asChar());

   return status;
}

/*
/// Register translators for Arnold nodes provided by the given Arnold Plugin.
///
/// Will only handle Arnold nodes that are marked as provided by this plugin.
///
/// @param plugin  the resolved absolute path to an Arnold plugin
///
MStatus CExtension::RegisterPluginTranslators(const MString &plugin)
{
   MStatus status(MStatus::kSuccess);

   // Arnold api doc says AiNodeEntryGetFilename returns <buit-in> for
   // built-in nodes, but it seems to return an empty string.
   if (plugin.numChars() == 0)
      AiMsgDebug("[mtoa] [%s] Registering translators for built-in nodes.", m_impl->m_extensionName.asChar());
   else
      AiMsgDebug("[mtoa] [%s] Registering translators for nodes provided by Arnold plugin %s.", m_impl->m_extensionName.asChar(), plugin.asChar());

   // FIXME: use map instead
   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
      const char* nodeName = AiNodeEntryGetName(nentry);
      const char* nodeFile = AiNodeEntryGetFilename(nentry);
      if (strcmp(nodeFile, plugin.asChar()) == 0)
      {
         // If the Arnold node is marked as a node that should be ignored
         bool ignore;
         if (AiMetaDataGetBool(nentry, NULL, "maya.ignore", &ignore) && ignore)
         {
            AiMsgDebug("[mtoa] [%s] [node %s] Marked as ignored.", m_impl->m_extensionName.asChar(), nodeName);
            continue;
         }
         // AiMsgDebug("[mtoa] [%s] Arnold node %s is provided by %s and will be processed for translator registration", m_impl->m_extensionName.asChar(), nodeName, nodeFile);
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
   unsigned int newNodes = RegisteredNodesCount();
   unsigned int trsNodes = TranslatedNodesCount();
   AiMsgInfo("[mtoa] [%s] Registered a total of %i translators for %i Maya nodes (%i new and %i existing).",
         m_impl->m_extensionName.asChar(), TranslatorCount(), trsNodes, newNodes, trsNodes - newNodes);

   return status;
}
*/

/// Register a Maya node for the given Arnold node.
///
/// Certain optional node-level metadata can be used to control how the
/// node factory processes the node's registration:
///  -# "maya.name" - the name for the generated or corresponding maya node
///  -# "maya.id" - the maya node id to use for the generated or corresponding maya node
///  -# "maya.class" - classification string (defaults to "shader/surface")
///
/// See CBaseAttrHelper for parameter-level metadata for controlling attribute creation
///
/// @param arnoldNodeEntry  arnold AtNodeEntry from which to generate the new Maya node
///
/// @return MStatus::kSuccess if the node is registered successfully, else MStatus::kFailure
///

MStatus CExtensionImpl::RegisterNode(CPxMayaNode &mayaNode,
                                 const CPxArnoldNode &arnoldNode)
{
   // We're either creating a new node and mapping it (associating it with an exiting node),
   // or mapping to an existing one,
   // or ignoring it alltogether if not enough information is present
   MStatus status;
   if (MFnPlugin::isNodeRegistered(mayaNode.name))
   {
      // we're mapping to an existing maya node
      status = MapMayaNode(mayaNode, arnoldNode);
   }
   else if (NULL != mayaNode.creator)
   {
      // we're creating (and mapping)
      status = NewMappedMayaNode(mayaNode, arnoldNode);
   }
   else
   {
      // No error or warning message, because there are many Arnold nodes that are not meant to be associated
      status = MStatus::kNotImplemented;
      AiMsgDebug("[mtoa] [%s] [node %s] Not enough metadata information to automatically associate an existing or register a new Maya node, ignored.", m_extensionName.asChar(), arnoldNode.name.asChar());
   }

   return status;
}

/// Register a translator for the given Arnold node entry.
///
/// Certain optional node-level metadata can be used to control how the
/// node factory processes the node's registration:
///  -# "maya.name" - the name for the generated or corresponding maya node
///  -# "maya.id" - the maya node id to use for the generated or corresponding maya node
///  -# "maya.translator" - the name to use for the translator
/// If no creator function is provided, will use default translator
/// for each Arnold exported node types (camera, light, shader, shape)
///
/// @param arnoldNodeEntry Arnold node name the translator should produce
///
/// @return MStatus::kSuccess if the translator is registered successfully, else MStatus::kFailure
///
MStatus CExtensionImpl::RegisterTranslator(const CPxTranslator &translator,
                                       CPxMayaNode &mayaNode,
                                       const CPxArnoldNode &arnoldNode)
{
   MStatus status;


   if (NULL == translator.creator || mayaNode.IsNull())
   {
      status = MStatus::kNotImplemented;
      AiMsgDebug("[mtoa] [%s] [node %s] Not enough metadata information to automatically register a translator, ignored.",
            m_extensionName.asChar(), arnoldNode.name.asChar());
   }
   else
   {
      // Try to add the new translator to the registered lists
      status = NewTranslator(translator, mayaNode);
   }

   return status;
}

/// Register a new Maya node and map it to an Arnold node.
MStatus CExtensionImpl::NewMappedMayaNode(CPxMayaNode mayaNode,
                                      const CPxArnoldNode &arnoldNode)
{
   MStatus status;

   if ((NULL == mayaNode.creator) || arnoldNode.IsNull())
   {
      AiMsgError("[mtoa] [%s] Not enough information to register a new Maya node for an existing Arnold node.", m_extensionName.asChar());
      return MStatus::kFailure;
   }
   // If we are going to create a node, check that we can,
   // it doesnt' exist and we have all required information or use defaults
   if (mayaNode.name == "")
   {
      mayaNode.SetName(toMayaStyle(MString("ai_")+arnoldNode.name));
      
      AiMsgWarning("[mtoa] [%s] [node %s] Using auto generated associated Maya type name %s.",
            mayaNode.provider.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());
   }
   if (!MFnPlugin::isNodeRegistered(mayaNode.name))
   {
      // If we don't have a valid nodeId, we'll generate a temporary one.
      // There is no guarantee that it won't collide with some node id another instance of MtoA.
      if (mayaNode.id.id() == 0)
      {
         mayaNode.id = MTypeId(s_autoNodeId++);
         AiMsgWarning("[mtoa] [%s] [node %s] Assigning temporary node id %i to associated Maya type name %s.",
               mayaNode.provider.asChar(), arnoldNode.name.asChar(), mayaNode.id.id(), mayaNode.name.asChar());
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
      AiMsgWarning("[mtoa] [%s] [node %s] Maya node %s is already an existing Maya node. Ignored",
            mayaNode.provider.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());
      return MStatus::kFailure;
   }
   if (MStatus::kSuccess == status)
   {
      // Now associate it with the Arnold node
      status = MapMayaNode(mayaNode, arnoldNode);
   }

   return status;
}

/// Register a new Maya node not corresponding to any Arnold node.
///
/// Typically helper nodes that don't need to be directly translated.
///
MStatus CExtensionImpl::NewMayaNode(const CPxMayaNode &mayaNode)
{
   // Need all necessary creation information
   if ((mayaNode.name == "") || (mayaNode.id.id() == 0) || (NULL == mayaNode.creator))
   {
      AiMsgError("[mtoa] [%s] Not enough information to register a new Maya node.", m_extensionName.asChar());
      return MStatus::kFailure;
   }
   if (MFnPlugin::isNodeRegistered(mayaNode.name))
   {
      AiMsgError("[mtoa] [%s] Failed to register Maya node %s: already exists.",
            mayaNode.provider.asChar(), mayaNode.name.asChar());
      return MStatus::kFailure;
   }
   // Extensions can freely override their own declarations, for conflicting declarations
   // between different extensions, it is handled by the the extensions manager
   std::pair<MayaNodesSet::iterator, bool> ret;
   ret = m_registeredMayaNodes.insert(mayaNode);
   if (false == ret.second)
   {
      AiMsgDebug("[mtoa] [%s] Overriding it's own registration of Maya node %s.",
            mayaNode.provider.asChar(), mayaNode.name.asChar());
      m_registeredMayaNodes.erase(ret.first);
      m_registeredMayaNodes.insert(mayaNode);
   }
   else
   {
      AiMsgDebug("[mtoa] [%s] Registered new Maya node %s.",
            mayaNode.provider.asChar(), mayaNode.name.asChar());
   }

   return MStatus::kSuccess;
}

/// Associate an existing Maya node to an Arnold node.
MStatus CExtensionImpl::MapMayaNode(const CPxMayaNode &mayaNode,
                                const CPxArnoldNode &arnoldNode)
{
   MStatus status;

   if (arnoldNode.IsNull())
   {
      AiMsgError("[mtoa] [%s] Not enough information to map an existing Maya node to an Arnold node.", m_extensionName.asChar());
      return MStatus::kFailure;
   }

   ArnoldNodeToMayaNodeMap::iterator it;
   std::pair<ArnoldNodeToMayaNodeMap::iterator, ArnoldNodeToMayaNodeMap::iterator> range;
   range = m_arnoldToMayaNodes.equal_range(arnoldNode);
   for (it = range.first; it != range.second; it++)
   {
      if ((*it).second == mayaNode)
      {
         AiMsgDebug("[mtoa] [%s] [node %s] Overriding it's own association of Maya node %s.",
            mayaNode.provider.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());
         (*it).second = mayaNode;
         // TODO: clean translators if we did a override?
         // AiMsgWarning("[mtoa] [%s] [node %s] Failed to associate existing Maya node %s.", m_impl->m_extensionName.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());

         return MStatus::kSuccess;
      }
   }
   // Not found, add it
   it = m_arnoldToMayaNodes.insert(range.first, std::make_pair(arnoldNode, mayaNode));
   // Add an empty translator set for that node
   MayaNodeToTranslatorsOldMap::iterator nodeIt;
   nodeIt = m_registeredTranslators.find(mayaNode);
   if (nodeIt == m_registeredTranslators.end())
   {
      m_registeredTranslators[mayaNode] = TranslatorsSet();
   }

   AiMsgDebug("[mtoa] [%s] [node %s] Is associated with existing Maya node %s.", m_extensionName.asChar(), arnoldNode.name.asChar(), mayaNode.name.asChar());
   return MStatus::kSuccess;
}

/// Store a new translator proxy in list.
MStatus CExtensionImpl::NewTranslator(const CPxTranslator &translator,
                                  const CPxMayaNode &mayaNode)
{
   if (NULL == translator.creator || mayaNode.IsNull())
   {
      AiMsgError("[mtoa] [%s] Not enough information to register a new translator.", m_extensionName.asChar());
      return MStatus::kFailure;
   }

   MStatus status;
   // Test translator creation and update proxy
   CPxTranslator trsProxy(translator);
   CNodeTranslator* trs = NULL;
   TCreatorFunction creatorFunction = translator.creator;
   trs = (CNodeTranslator*)creatorFunction();
   if (NULL != trs)
   {
      trs->CreateImplementation();
      if (trsProxy.name == "") trsProxy.SetName(trs->m_impl->m_abstract.name);
      if (trsProxy.arnold == "") trsProxy.arnold = trs->m_impl->m_abstract.arnold;
      // if (trsProxy.maya == "") trsProxy.maya = trs->m_abstract.maya;
      if (trsProxy.provider == "") trsProxy.provider = trs->m_impl->m_abstract.provider;

      delete trs;
   }
   else
   {
      AiMsgError("[mtoa] [%s] [node %s] Cannot create translator %s for Maya node %s: badly declared creator function?",
            m_extensionName.asChar(), trsProxy.arnold.asChar(), trsProxy.name.asChar(), mayaNode.name.asChar());
      return MStatus::kFailure;
   }
   TranslatorsSet nodeTranslators;
   MayaNodeToTranslatorsOldMap::iterator nodeIt;
   nodeIt = m_registeredTranslators.find(mayaNode);
   if (nodeIt != m_registeredTranslators.end())
   {
      nodeTranslators = nodeIt->second;
   }
   // Add to this node's translators
   std::pair<TranslatorsSet::iterator, bool> ret;
   unsigned int nbPrevTrs = nodeTranslators.size();
   ret = nodeTranslators.insert(trsProxy);
   if (true == ret.second)
   {
      // if (nodeTranslators.begin() == ret.first)
      if (0 == nbPrevTrs)
      {
         // First translator to be created 
         AiMsgDebug("[mtoa] [%s] [node %s] Registered the translator %s for associated Maya node %s.",
               trsProxy.provider.asChar(), trsProxy.arnold.asChar(), trsProxy.name.asChar(), mayaNode.name.asChar());
      }
      else
      {
         // There was already at least one
         AiMsgDebug("[mtoa] [%s] [node %s] Registered an alternative translator %s for associated Maya node %s.",
               trsProxy.provider.asChar(), trsProxy.arnold.asChar(), trsProxy.name.asChar(), mayaNode.name.asChar());
      }
   }
   else
   {
      // We can only override our own translators (if using the same name),
      nodeTranslators.erase(ret.first);
      nodeTranslators.insert(trsProxy);
      AiMsgDebug("[mtoa] [%s] [node %s] Replaced translator %s for associated Maya node %s.",
            trsProxy.provider.asChar(), trsProxy.arnold.asChar(), trsProxy.name.asChar(), mayaNode.name.asChar());
      
   }
   m_registeredTranslators[mayaNode] = nodeTranslators;
   status = MStatus::kSuccess;
   // Got no case where it would happen at the moment, but error checking should be done here
   if (MStatus::kSuccess != status)
   {
      AiMsgWarning("[mtoa] [%s] [node %s] Failed to register translator %s for Maya node %s.",
            m_extensionName.asChar(), trsProxy.arnold.asChar(), trsProxy.name.asChar(), mayaNode.name.asChar());
   }
   else
   {
      MString defaultTranslator = CExtensionsManager::GetDefaultTranslator(mayaNode.name);
      if (defaultTranslator == "")
         CExtensionsManager::SetDefaultTranslator(mayaNode.name, translator.name);
   }
   return status;
}

/// Find a registered Maya node by the Maya node type name.
const CPxMayaNode* CExtensionImpl::FindRegisteredMayaNode(const CPxMayaNode &mayaNode)
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

/// Find a std::set of all registered translators for a Maya node.
const TranslatorsSet* CExtensionImpl::FindRegisteredTranslators(const CPxMayaNode &mayaNode)
{
   MayaNodeToTranslatorsOldMap::iterator nodeIt;
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
MString CExtensionImpl::FindFileInPath(const MString &file,
                                   const MString &path,
                                   MStatus *returnStatus)
{
   MStatus status = MStatus::kNotFound;
   MString resolved = "";

   MFileObject fileObject;
   fileObject.setRawName(file);
   fileObject.setRawPath(path.expandEnvironmentVariablesAndTilde());
   unsigned int nbSearchPath = fileObject.pathCount();
   for (unsigned int i=0; i<nbSearchPath; i++)
   {
      if (fileObject.exists(i))
      {
         resolved = fileObject.ithFullName(i); 
         status = MStatus::kSuccess;
         break;
      }
   }
   if (MStatus::kSuccess != status)
   {
      resolved = "";
   }

   if (NULL != returnStatus) *returnStatus = status;
   return resolved;
}

/// Find all libraries (dll, so) in the given search path.
///
/// @param path all paths to search for libraries, ; or : separated
MStringArray CExtensionImpl::FindLibraries(const MString &path,
                                       MStatus *returnStatus)
{
   MStatus status = MStatus::kNotFound;
   MStringArray files;

   MString resolvedPathList = path.expandEnvironmentVariablesAndTilde();
   MStringArray pluginPaths;
   resolvedPathList.split(PATHSEP, pluginPaths);
   for (unsigned int i=0; i<pluginPaths.length(); ++i)
   {
      MString dir = pluginPaths[i];
      DIR *dp;
      struct dirent *dirp;
      if ((dp  = opendir(dir.asChar())) == NULL)
      {
         // TODO: print more explicit error message than just errno
         AiMsgWarning("[mtoa] Error opening %s.", dir.asChar());
         status = MStatus::kFailure;
         continue;
      }
      while ((dirp = readdir(dp)) != NULL)
      {
         MString entry = dirp->d_name;
         unsigned int nchars = entry.numChars();
         MString libext = MString(LIBEXT);
         unsigned int next = libext.numChars();
         if (nchars > next)
         {
            MString ext = entry.substringW(nchars-next, nchars-1);
            if (entry.substringW(0,0) != "." && ext == libext)
            {
               std::string filePath = (dir + DIRSEP + entry).asChar();
               std::replace(filePath.begin(), filePath.end(), '\\','/');
               files.append(filePath.c_str());
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
/// @param path  the resolved file name of Arnold plugin
bool CExtension::IsArnoldPluginLoaded(const MString &file)
{
   MFileObject fo;
   fo.setRawFullName(file);
   MString rawName = fo.rawName();
   for (LoadedArnoldPluginsSet::iterator it = CExtensionImpl::s_allLoadedArnoldPlugins.begin() ; it != CExtensionImpl::s_allLoadedArnoldPlugins.end(); ++it)
   {
      MFileObject fo2;
      fo2.setRawFullName((*it).c_str());
      if (rawName == fo2.rawName())
         return true;
   }
   return false;
}

/// Returns the list of all loaded Arnold plugins
MStringArray CExtension::GetAllLoadedArnoldPlugins()
{
   MStringArray result;
   LoadedArnoldPluginsSet::iterator pluginIt;
   for (pluginIt = CExtensionImpl::s_allLoadedArnoldPlugins.begin();
       pluginIt != CExtensionImpl::s_allLoadedArnoldPlugins.end();
       pluginIt++)
   {
      result.append(pluginIt->c_str());
   }

   return result;
}






