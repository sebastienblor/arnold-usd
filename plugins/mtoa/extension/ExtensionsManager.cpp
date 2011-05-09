
#include "ExtensionsManager.h"
#include "AbMayaNode.h"

#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MFileObject.h>
#include <maya/MPlugArray.h>


// CExtensionsManager


// void initializePlugin(CExtension& plugin)
// in an extension library
// TODO : do we need void uninitializePlugin(CExtension& plugin) ?

typedef void (*ExtensionInitFunction)(CExtension&);


MayaNodesSet CExtensionsManager::s_registeredMayaNodes;
MayaNodeToTranslatorsMap CExtensionsManager::s_registeredTranslators;
MObject CExtensionsManager::s_plugin;
ExtensionsList CExtensionsManager::s_extensions;

/// The Maya plugin it's used in (MtoA)
void CExtensionsManager::SetMayaPlugin(MObject plugin)
{
   s_plugin = plugin;
}

/// Returns a pointer to the extension handling built-in nodes
CExtension* CExtensionsManager::GetBuiltin(MStatus *returnStatus)
{
   MStatus status;
   CExtension* builtin = NULL;
   builtin = GetExtension(BUILTIN);
   if (NULL == builtin)
   {
      builtin = NewExtension(BUILTIN);
      status = builtin->RegisterAllNodes("");
      status = builtin->RegisterAllTranslators("");
   }
   if (NULL != returnStatus) *returnStatus = status;
   return builtin;
}
/// Load an Arnold plugin.

/// Loads the Arnold plugin and registers a Maya node for each Arnold node
/// contained within it, if applicable.
///
///
/// @param path  the absolute path to an Arnold plugin
///
CExtension* CExtensionsManager::LoadArnoldPlugin(const MString &file,
                                                 const MString &path,
                                                 MStatus *returnStatus)
{
   MStatus status;
   MString resolved = CExtension::FindFileInPath(file, path, &status);
   CExtension* pluginExtension = NULL;
   if (MStatus::kSuccess == status)
   {
      // Create a CExtension to generate corresponding Maya nodes
      pluginExtension = NewExtension(resolved);
      if (NULL != pluginExtension)
      {
         // Extension loads the Arnold Plugin and will register new Maya nodes
         pluginExtension->LoadArnoldPlugin(MString(file), path, &status);
         if (MStatus::kSuccess == status)
         {
            // Register plugin nodes and translators
            status = pluginExtension->RegisterAllNodes(resolved);
            status = pluginExtension->RegisterAllTranslators(resolved);
            // Do not register new nodes with Maya immediatly to allow modification
            // status = RegisterExtension(pluginExtension);
         } else {
            // Remove empty extension if plugin load failed
            DeleteExtension(pluginExtension);
         }
      }
   }

   if (NULL != returnStatus) *returnStatus = status;
   return pluginExtension;
}

/// Load all Arnold plugins on the plugin path
///
MStatus CExtensionsManager::LoadArnoldPlugins(const MString &path)
{
   MStatus status = MStatus::kNotFound;

   MStringArray plugins;
   plugins = CExtension::FindLibraries(path, &status);
   for (unsigned int i=0; i<plugins.length(); ++i)
   {
      MString resolved = plugins[i];
      if (resolved.numChars() > 0)
      {
         LoadArnoldPlugin(resolved, "", &status);
      }
   }

   return status;
}


/// Load an MtoA extension.
///
CExtension* CExtensionsManager::LoadExtension(const MString &file,
                                              const MString &path,
                                              MStatus *returnStatus)
{
   MStatus status;
   MString resolved = CExtension::FindFileInPath(file, path, &status);
   CExtension* extension = NULL;
   if (MStatus::kSuccess == status)
   {
      // Create a CExtension to represent loaded extension
      // TODO : store Library handle for unload
      extension = NewExtension(resolved);
      while (NULL != extension)
      {
         AiMsgInfo("[xma] Loading extension %s(%s)...", extension->GetName().asChar(), extension->GetFile().asChar());
         void *pluginLib = LibraryLoad(extension->GetFile().asChar());
         if (pluginLib == NULL)
         {
            AiMsgError("[xma] Error loading extension library: %s.", LibraryLastError());
            DeleteExtension(extension);
            status = MStatus::kFailure;
         }
         void* initializer = LibrarySymbol(pluginLib, "initializePlugin");
         if (initializer == NULL)
         {
            AiMsgError("[xma] Error initializing extension library: %s.", LibraryLastError());
            DeleteExtension(extension);
            status = MStatus::kFailure;
         }
         ExtensionInitFunction* initFunc = (ExtensionInitFunction*)(&initializer);
         // Do the init
         (*initFunc)(*extension);
         AiMsgInfo("[xma] Successfully loaded extension library %s.", extension->GetFile().asChar());
         // Do not register now to allow to add calls (registerNode, etc) on it before
         // status = RegisterExtension(extension);
      }
   }

   if (NULL != returnStatus) *returnStatus = status;
   return extension;
}

/// Load all MtoA extensions on the extension path.
///
MStatus CExtensionsManager::LoadExtensions(const MString &path)
{
   MStatus status = MStatus::kNotFound;

   MStringArray extensions;
   extensions = CExtension::FindLibraries(path, &status);
   for (unsigned int i=0; i<extensions.length(); ++i)
   {
      MString resolved = extensions[i];
      if (resolved.numChars() > 0)
      {
         LoadExtension(resolved, "", &status);
         MString cmd = "import mtoa.api.extensions;mtoa.api.extensions.loadExtensionUI('" + resolved + "')";
         CHECK_MSTATUS(MGlobal::executePythonCommand(cmd));
      }
   }

   return status;
}

/// Unload an extension

/// Remove from the Arnold universe all nodes created by the specified extension and
/// deregister any Maya nodes generated for them.
/// Deregister all translators created by this extension
/// Restore what it may have overriden

MStatus CExtensionsManager::UnloadExtension(CExtension* extension)
{
   MStatus status = MStatus::kSuccess;

   if (extension->IsRegistered())
   {
      status = DeregisterExtension(extension);
   }
   if (MStatus::kSuccess == status)
   {
      // TODO unload all Arnold Plugins loaded from library
      // then unload library
      // void LibraryUnload(void *handle)
      // status =
      // Delete stored reference
      DeleteExtension(extension);
   }

   return status;
}

/// Unload all Extensions
///
MStatus CExtensionsManager::UnloadExtensions()
{
   MStatus status = MStatus::kSuccess;

   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      MStatus extStatus = UnloadExtension(&(*extIt));
      if (MStatus::kSuccess != extStatus) status = extStatus;
   }

   return status;
}

MStatus CExtensionsManager::RegisterExtension(CExtension* extension)
{
   MStatus status = MStatus::kSuccess;

   if (extension->m_registered)
   {
      AiMsgError("[xma] Already registered extension %s.", extension->GetFile().asChar());
      return MStatus::kFailure;
   }
   // We need to update all global maps with this extension's maps
   // and register the new Maya nodes provided by this extension with Maya

   // Add all Maya nodes registered by this extension to the global set
   AiMsgInfo("[xma] Registering new Maya nodes from extension %s(%s).",
         extension->GetName().asChar(), extension->GetFile().asChar());
   const CPxMayaNode* mayaNode;
   const CPxMayaNode* existingMayaNode;
   MayaNodesSet::iterator rnodeIt;
   for (rnodeIt = extension->m_registeredMayaNodes.begin();
         rnodeIt != extension->m_registeredMayaNodes.end();
         rnodeIt++)
   {
      mayaNode = &(*rnodeIt);
      existingMayaNode = FindRegisteredMayaNode(*mayaNode);
      if (NULL != existingMayaNode)
      {
         // TODO : allow node overriding?
         AiMsgError("[%s] Tried to override Maya node %s, already registered by %s(%s).)",
               mayaNode->provider.asChar(), existingMayaNode->name.asChar(),
               existingMayaNode->provider.asChar(), existingMayaNode->file.asChar());
         status = MStatus::kFailure;
         continue;
      }
      MStatus nodeStatus = RegisterMayaNode(*mayaNode);
      // Only add successfully registered nodes to the list
      if (MStatus::kSuccess == nodeStatus)
      {
         std::pair<MayaNodesSet::iterator, bool> ret;
         ret = s_registeredMayaNodes.insert(*mayaNode);
      }
      // Update return status
      if (MStatus::kSuccess != nodeStatus)
      {
         status = nodeStatus;
      }
   }

   // TODO : Update the MayaNodeToArnoldNodeMap and ArnoldNodeToMayaNodeMap
   // for already successfully registered nodes only.

   // Add translators
   // TODO currently translators can be overriden, including builting ones
   // do we want that or only allow to add new ones, or only to specialise
   // translators (overriding for subclasses of a node class only)
   AiMsgInfo("[xma] Registering new translators from extension %s(%s).",
         extension->GetName().asChar(), extension->GetFile().asChar());
   MayaNodeToTranslatorsMap::iterator tnodeIt;
   for (tnodeIt = extension->m_registeredTranslators.begin();
         tnodeIt != extension->m_registeredTranslators.end();
         tnodeIt++)
   {
      mayaNode = &tnodeIt->first;
      TranslatorsSet &newTrans = tnodeIt->second;
      TranslatorsSet* oldTrans = FindRegisteredTranslators(*mayaNode);
      if (NULL == oldTrans)
      {
         s_registeredTranslators[*mayaNode] = TranslatorsSet();
         oldTrans = &s_registeredTranslators[*mayaNode];
      }

      TranslatorsSet::iterator trsIt;
      for (trsIt = newTrans.begin();
            trsIt != newTrans.end();
            trsIt++)
      {
         const CPxTranslator &translator = *trsIt;
         std::pair<TranslatorsSet::iterator, bool> ret;
         ret = oldTrans->insert(translator);
         if (true == ret.second)
         {
            AiMsgInfo("[%s] Added translator %s on Maya node %s.",
                  translator.provider.asChar(), translator.name.asChar(), mayaNode->name.asChar());
         }
         else
         {
            CPxTranslator &oldTranslator = *ret.first;
            AiMsgInfo("[%s] Replaced translator %s by translator %s on Maya node %s, previously defined by extension %s(%s).",
                  translator.provider.asChar(), oldTranslator.name.asChar(), translator.name.asChar(),
                  mayaNode->name.asChar(), oldTranslator.provider.asChar(), oldTranslator.file.asChar());
            oldTranslator = translator;
            // TODO : remove old additionnal attributes before we add new ones ?
         }
         if (NULL != translator.initialize)
         {
            // TODO : reimplement the plugin deferred mechanism
            translator.initialize(mayaNode->name);
         }
      }
      // Add arnoldTranslator attribute if more than one translator
      if (oldTrans->size() > 1) {
         CExtensionAttrHelper helper(mayaNode->name);
         CAttrData data;
         data.defaultValue.STR = "";
         data.name = "arnoldTranslator";
         data.shortName = "arntr";
         helper.MakeInputString(data);
      }
   }

   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("[xma] Registered extension %s(%s).", extension->GetName().asChar(), extension->GetFile().asChar());
      extension->m_registered = true;
   }
   return status;
}

MStatus CExtensionsManager::RegisterExtensions()
{
   MStatus status = MStatus::kSuccess;

   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (!extIt->IsRegistered())
      {
         MStatus extStatus = RegisterExtension(&(*extIt));
         if (MStatus::kSuccess != extStatus) status = extStatus;
      }
   }

   return status;
}

MStatus CExtensionsManager::DeregisterExtension(CExtension* extension)
{
   MStatus status = MStatus::kSuccess;

   // TODO : iterate on nodes and translators and deregister then remove
   // TODO : restoring overrides. Complex!

   // First remove nodes from shader library
   /*
   MString str;
   str = MString("[mtoa] Removing nodes for shader library: ") + m_shaderFile;
   MGlobal::displayInfo(str);
   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_SHADER);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);

      const char* nentryFile = AiNodeEntryGetFilename(nentry);
      if (strcmp(nentryFile, m_shaderFile) == 0)
      {
         const char *arnoldNodeName = AiNodeEntryGetName(nentry);
         // remove from maya
         UnregisterMayaNode(arnoldNodeName);
         // remove from arnold
         AiNodeUninstall(arnoldNodeName);
      }
   }
   for (unsigned int i=0; i < s_shaderLibs.size(); ++i)
   {
      if (strcmp(s_shaderLibs[i]->GetFilename(), m_shaderFile))
      {
         delete s_shaderLibs[i];
         s_shaderLibs.erase(s_shaderLibs.begin() + i);
         break;
      }
   }
   m_loaded = false;
   */

   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("[xma] Deregistered extension %s(%s).", extension->GetName().asChar(), extension->GetFile().asChar());
      extension->m_registered = false;
   }
   return status;
}

MStatus CExtensionsManager::DeregisterExtensions()
{
   MStatus status = MStatus::kSuccess;

   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (extIt->IsRegistered())
      {
         MStatus extStatus = RegisterExtension(&(*extIt));
         if (MStatus::kSuccess != extStatus) status = extStatus;
      }
   }

   return status;
}

/// Get a instancied CDagTranslator translator for the given node (MObject)
/// using the value of the "arnoldTranslator" attribut
/// to provide an optional name to choose between multiple translators
CDagTranslator* CExtensionsManager::GetTranslator(const MDagPath &dagPath)
{
   MObject node = dagPath.node();
   return (CDagTranslator*)GetTranslator(node);
}

/// Get a instancied CNodeTranslator translator for the given node (MObject)
/// using the value of the "arnoldTranslator" attribut
/// to provide an optional name to choose between multiple translators
CNodeTranslator* CExtensionsManager::GetTranslator(const MObject &object)
{
   MFnDependencyNode depFn(object);
   MString typeName = depFn.typeName();
   MTypeId typeId = depFn.typeId();
   // MString transName = BUILTIN;
   MString transName = "";
   MStatus status;
   MPlug plug = depFn.findPlug("arnoldTranslator", &status);
   if (MStatus::kSuccess == status) transName = plug.asString();

   return (CNodeTranslator*) GetTranslator(typeName, transName);
}

/// Get a instancied translator for the given type,
/// with an optional name to choose between multiple translators
CNodeTranslator* CExtensionsManager::GetTranslator(const MString &typeName,
                                       const MString &translatorName)
{
   CNodeTranslator* translator = NULL;
   // No need for typeId, we're indexed on typeName
   // and with Maya 2012 it can be retrieved from typeName anyway
   CPxMayaNode mayaNode(typeName, MTypeId());
   TranslatorsSet *allTranslators;
   allTranslators = FindRegisteredTranslators(mayaNode);
   if (NULL == allTranslators || allTranslators->empty())
   {
      AiMsgDebug("[maya %s] No registered translators, it won't be exported.", mayaNode.name.asChar());
      return NULL;
   }
   const CPxTranslator* foundTrs = NULL;
   CPxTranslator searchTrs(translatorName);
   if (translatorName != "")
   {
      foundTrs = FindRegisteredTranslator(mayaNode, searchTrs);
      if (NULL == foundTrs)
      {
         searchTrs.name = "";
         foundTrs = FindRegisteredTranslator(mayaNode, searchTrs);
         AiMsgDebug("[maya %s] Found no translator named %s, defaulting on translator %s, provided by %s(%s).",
               mayaNode.name.asChar(), translatorName.asChar(), foundTrs->name.asChar(),
               foundTrs->provider.asChar(), foundTrs->file.asChar());
      }
   }
   else
   {
      foundTrs = FindRegisteredTranslator(mayaNode, searchTrs);
      AiMsgDebug("[maya %s] Using translator %s, provided by %s(%s).",
         mayaNode.name.asChar(), foundTrs->name.asChar(),
         foundTrs->provider.asChar(), foundTrs->file.asChar());
   }

   // Instanciate a new translator using proxy info
   if (NULL != foundTrs->creator)
   {
      TCreatorFunction creatorFunction = foundTrs->creator;
      translator = (CNodeTranslator*)creatorFunction();
      // This customize the prototype instance of the translator
      // with the information found in the translator class proxy
      // TODO : use abstract CAbTranslator instead
      translator->m_arnoldNodeName = foundTrs->arnold;
      translator->m_mayaNodeName = typeName;
      translator->m_translatorName = foundTrs->name;
      translator->m_providerName = foundTrs->provider;
   }
   else
   {
      AiMsgError("[maya %s] Translator %s has no creator method, incorrectly registered by %s.",
         mayaNode.name.asChar(), foundTrs->name.asChar(), foundTrs->provider.asChar());
   }

   return translator;
}

MStringArray CExtensionsManager::GetTranslatorNames(const MString &typeName,
                                       const MString &provider)
{
   MStringArray result;

   CPxMayaNode mayaNode(typeName);
   TranslatorsSet *allTranslators;
   allTranslators = FindRegisteredTranslators(mayaNode);
   if (NULL != allTranslators)
   {
      bool noTest = (provider.numChars() == 0);
      TranslatorsSet::iterator trsIt;
      for (trsIt = allTranslators->begin();
            trsIt != allTranslators->end();
            trsIt++)
      {
         const CPxTranslator &translator = *trsIt;
         if (noTest || provider == translator.provider)
         {
            result.append(translator.name);
         }
      }
   }
   return result;
}

CExtension* CExtensionsManager::GetExtension(const MString &extensionFile)
{
   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (extIt->GetFile() == extensionFile) return &(*extIt);
   }
   return NULL;
}

// -------------- protected ----------- //

CExtension* CExtensionsManager::NewExtension(const MString &extensionFile)
{
   CExtension* result;

   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (extIt->GetFile() == extensionFile) break;
   }
   if (s_extensions.end() == extIt)
   {
      CExtension extension = CExtension(extensionFile);
      extIt = s_extensions.insert(s_extensions.end(), extension);
      if (s_extensions.end() != extIt)
      {
         result = &(*extIt);
      }
      else
      {
         AiMsgError("[xma] Could not create extension entry for: %s", extensionFile.asChar());
         result = NULL;
      }
   }
   else
   {
      AiMsgError("[xma] Extension already managed: %s", extensionFile.asChar());
      result = NULL;
   }

   return result;
}

MStatus CExtensionsManager::DeleteExtension(CExtension* &extension)
{
   MStatus status;

   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (extIt->GetFile() == extension->GetFile()) break;
   }
   if (s_extensions.end() != extIt)
   {
      s_extensions.erase(extIt);
      extension = NULL;
      status = MStatus::kSuccess;
   }
   else
   {
      AiMsgError("[xma] Extension not managed: %s", extension->GetFile().asChar());
      status = MStatus::kFailure;
   }

   return status;
}

MStatus CExtensionsManager::RegisterMayaNode(const CPxMayaNode &mayaNode)
{
   MStatus status;

   if (MFnPlugin::isNodeRegistered(mayaNode.name))
   {
      AiMsgError("[xma] Cannot register node %s provided by %s, it already exists",
            mayaNode.name.asChar(), mayaNode.provider.asChar());
      return MStatus::kFailure;
   }
   // Construct the abstract to store in the class static s_abstract member,
   // if a pointer to one was provided in the proxy for that Maya node class.
   CAbMayaNode abstract(mayaNode);
   if (NULL != mayaNode.abstract) *mayaNode.abstract = abstract;
   const MString *classificationPtr = (mayaNode.classification == "") ? NULL : &mayaNode.classification;
   status = MFnPlugin(s_plugin).registerNode(mayaNode.name, mayaNode.id,
         mayaNode.creator, mayaNode.initialize,
         mayaNode.type, classificationPtr);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("[xma] Registered node %s provided by %s",
            mayaNode.name.asChar(), mayaNode.provider.asChar());
   }
   else
   {
      AiMsgError("[xma] Failed to register node %s provided by %s",
            mayaNode.name.asChar(), mayaNode.provider.asChar());
   }

   return status;
}

/// Unregister the maya node generated by the given arnold node and remove
/// all mappings associated with the arnold node.
///
MStatus CExtensionsManager::DeregisterMayaNode(const CPxMayaNode &mayaNode)
{
   MStatus status;

   if (!MFnPlugin::isNodeRegistered(mayaNode.name))
   {
      AiMsgWarning("[xma] Node %s not registered with Maya", mayaNode.name.asChar());
      return MStatus::kSuccess;
   }
   status = MFnPlugin(s_plugin).deregisterNode(mayaNode.id);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("[xma] Successfully deregistered node %s provided by %s",
            mayaNode.name.asChar(), mayaNode.provider.asChar());
   }
   else
   {
      AiMsgError("[xma] Failed to deregister node %s provided by %s",
            mayaNode.name.asChar(), mayaNode.provider.asChar());
   }
   return status;
}

/// Find a registering Maya node proxy.
const CPxMayaNode* CExtensionsManager::FindRegisteredMayaNode(const CPxMayaNode &mayaNode)
{
   MayaNodesSet::iterator nodeIt;
   nodeIt = s_registeredMayaNodes.find(mayaNode);
   if (nodeIt != s_registeredMayaNodes.end())
   {
      return &(*nodeIt);
   }
   else
   {
      return NULL;
   }
}

/// Try to find a given translator proxy in all registered translators
/// for the given Maya node.
///
/// Returns default translator for that Maya node if no translator (name)
/// is specified.
const CPxTranslator* CExtensionsManager::FindRegisteredTranslator(const CPxMayaNode &mayaNode,
                                             const CPxTranslator &translator)
{
   TranslatorsSet *allTranslators;
   allTranslators = FindRegisteredTranslators(mayaNode);
   if (NULL == allTranslators || allTranslators->empty()) return NULL;

   const CPxTranslator* result = NULL;
   if (translator.name == "")
   {
      // No specific translator requested, return last
      // TODO : actually check s_extensions to use the last loaded translator?
      result = &(*--allTranslators->end());
   }
   else
   {
      TranslatorsSet::iterator trsIt;
      trsIt = allTranslators->find(translator);
      if (allTranslators->end() != trsIt) result = &(*trsIt);
   }

   return result;
}

/// Find the std::set of the proxies for all registered translators for a Maya node.
TranslatorsSet* CExtensionsManager::FindRegisteredTranslators(const CPxMayaNode &mayaNode)
{
   MayaNodeToTranslatorsMap::iterator nodeIt;
   nodeIt = s_registeredTranslators.find(mayaNode);
   if (nodeIt != s_registeredTranslators.end())
   {
      return &nodeIt->second;
   }
   else
   {
      return NULL;
   }
}
