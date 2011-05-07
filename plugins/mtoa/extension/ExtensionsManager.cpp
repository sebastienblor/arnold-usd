
#include "ExtensionsManager.h"


#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MFileObject.h>
#include <maya/MPlugArray.h>

#define BUILTIN "<built-in>"

#ifdef _WIN32
#include <platform/win32/dirent.h>
#define PATHSEP ';'
#define DIRSEP "/"
#define LIBEXT MString(".dll")
#else
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>

#define PATHSEP ':'
#define DIRSEP "/"
#ifdef _LINUX
#define LIBEXT MString(".so")
#endif
#ifdef _DARWIN
#define LIBEXT MString(".dylib")
#endif
#endif

/// @param searchPath  a path to search for libraries, optionally containing
/// separators (: on unix, ; on windows) and environment variables
int FindLibraries(MString searchPath, MStringArray &files)
{
   MString resolvedPathList = searchPath.expandFilePath();
   MStringArray pluginPaths;
   resolvedPathList.split(PATHSEP, pluginPaths);
   for (unsigned int i=0; i<pluginPaths.length(); ++i)
   {
      MString dir = pluginPaths[i];
      DIR *dp;
      struct dirent *dirp;
      if ((dp  = opendir(dir.asChar())) == NULL)
      {
         // TODO: print better error message
         cerr << "Error(" << errno << ") opening " << dir << endl;
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
            }
         }
      }
      closedir(dp);
   }
   return 0;
}

// CExtensionsManager

// std::vector<CExtensionsManager*> CExtensionsManager::s_shaderLibs;

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
CExtension* CExtensionsManager::GetBuiltin()
{
   CExtension* result;
   result = GetExtension(BUILTIN);
   if (NULL == result)
   {
      result = NewExtension(BUILTIN);
      result->m_loaded = true;
   }

   return result;
}
/// Load an Arnold plugin.

/// Loads the Arnold plugin and registers a Maya node for each Arnold node
/// contained within it, if applicable.
///
///
/// @param path  the absolute path to an Arnold plugin
///
CExtension* CExtensionsManager::LoadArnoldPlugin(const MString &path)
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
   MString resolvedPath = pluginFile.resolvedFullName();
   CExtension* pluginExtension;
   // Create a CExtension to generate corresponding Maya nodes
   pluginExtension = NewExtension(resolvedPath);
   if (NULL != pluginExtension)
   {
      // Extension loads the Arnold Plugin and will register new Maya nodes
      pluginExtension->LoadArnoldPlugin(resolvedPath);
      // Register new nodes with Maya
      RegisterExtension(pluginExtension);
   }
   return pluginExtension;
}

/// Load all Arnold plugins on the plugin path
///
MStatus CExtensionsManager::LoadArnoldPlugins(const MString &directory)
{
   MStatus status = MStatus::kSuccess;

   // MString resolvedPathList = MString("$ARNOLD_PLUGIN_PATH").expandEnvironmentVariablesAndTilde();
   MStringArray plugins;
   FindLibraries(directory, plugins);
   for (unsigned int i=0; i<plugins.length(); ++i)
   {
      MString path = plugins[i];
      if (path.numChars() > 0)
      {
         cerr << "Loading plugin:" << path.asChar() << endl;
         CExtension* pluginExtension;
         pluginExtension = LoadArnoldPlugin(path);
         if (NULL == pluginExtension)
         {
            status = MStatus::kFailure;
         }
      }
   }

   return status;
}


/// Load an MtoA extension.
///
CExtension* CExtensionsManager::LoadExtension(const MString &path)
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
   MString resolvedPath = pluginFile.resolvedFullName();
   CExtension* extension;
   extension = NewExtension(resolvedPath);
   if (NULL != extension)
   {
      AiMsgInfo("Loading extension %s from %s", extension->GetName().asChar(), extension->GetFile().asChar());

      void *pluginLib = LibraryLoad(extension->GetFile().asChar());
      if (pluginLib == NULL)
      {
         AiMsgError("Error loading extension library: %s", LibraryLastError());
         return NULL;
      }

      void* initializer = LibrarySymbol(pluginLib, "initializePlugin");
      if (initializer == NULL)
      {
         AiMsgError("Error initializing extension library: %s", LibraryLastError());
         return NULL;
      }
      ExtensionInitFunction* initFunc = (ExtensionInitFunction*)(&initializer);
      // Do the init
      (*initFunc)(*extension);
      AiMsgInfo("Loaded extension library %s", extension->GetFile().asChar());

      // Do not register now to allow to add calls (registerNode, etc) on it before?
      // RegisterExtension(extension);
   }

   return extension;
}

/// Load all MtoA extensions on the extension path.
///
MStatus CExtensionsManager::LoadExtensions(const MString &directory)
{
   MStatus status = MStatus::kSuccess;

   // TODO: replace this with python code
   MStringArray extensions;
   FindLibraries(directory, extensions);
   for (unsigned int i=0; i<extensions.length(); ++i)
   {
      MString path = extensions[i];
      if (path.numChars() > 0)
      {
         cerr << "Loading extension:" << path.asChar() << endl;
         LoadExtension(path);
         MString cmd = "import mtoa.api.extensions;mtoa.api.extensions.loadExtensionUI('" + path + "')";
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

MStatus CExtensionsManager::UnloadExtension(const CExtension* extension)
{
   MStatus status;

   DeregisterExtension(extension);
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

   return status;
}

/// Unload all Extensions
///
MStatus CExtensionsManager::UnloadExtensions()
{
   MStatus status;
   /*
   for (unsigned int i=0; i < s_shaderLibs.size(); ++i)
   {
      s_shaderLibs[i]->UnloadPlugin();
   }
   */
   return status;
}

MStatus CExtensionsManager::RegisterExtension(CExtension* extension)
{
   MStatus status = MStatus::kSuccess;

   if (extension->IsRegistered())
   {
      AiMsgError("Already registered extension %s", extension->GetFile().asChar());
      return MStatus::kFailure;
   }
   // We need to update all global maps with this extension's maps
   // and register the new Maya nodes provided by this extension with Maya

   // Add all Maya nodes registered by this extension to the global set
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
         AiMsgError("Extension %s trying to override Maya node %s already registered by %s", mayaNode->provider.asChar(), existingMayaNode->name.asChar(), existingMayaNode->provider.asChar());
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
         s_registeredTranslators[*mayaNode] = newTrans;
      }
      else
      {
         TranslatorsSet::iterator trsIt;
         for (trsIt = newTrans.begin();
               trsIt != newTrans.end();
               trsIt++)
         {
            const CPxTranslator &translator = *trsIt;
            std::pair<TranslatorsSet::iterator, bool> ret;
            ret = oldTrans->insert(translator);
            if (false == ret.second)
            {
               CPxTranslator &oldTranslator = *ret.first;
               AiMsgInfo("Extension %s overriding tranlator %s previously defined by extension %s", translator.provider.asChar(), oldTranslator.name.asChar(), oldTranslator.provider.asChar());
               oldTranslator = translator;
            }
         }
      }
   }

   return status;
}


MStatus CExtensionsManager::DeregisterExtension(const CExtension* extension)
{
   MStatus status = MStatus::kSuccess;

   // TODO : iterate on nodes and translators and deregister then remove

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
      AiMsgDebug("There are no registered translators for Maya node %s, it won't be exported", mayaNode.name);
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
         AiMsgDebug("Found no translator named %s for Maya node %s, defaulting on translator %s",
               translatorName.asChar(), mayaNode.name.asChar(), foundTrs->name.asChar());
      }
   }
   else
   {
      foundTrs = FindRegisteredTranslator(mayaNode, searchTrs);
      AiMsgDebug("Using translator %s for Maya node %s", foundTrs->name.asChar(), mayaNode.name.asChar());
   }

   // Instanciate a new translator using proxy info
   if (NULL != foundTrs->creator)
   {
      TCreatorFunction creatorFunction = foundTrs->creator;
      translator = (CNodeTranslator*)creatorFunction();
      // This customize the prototype instance of the translator
      // with the information found in the translator class proxy
      translator->m_arnoldNodeName = foundTrs->arnold;
      translator->m_mayaNodeName = typeName;
      translator->m_translatorName = foundTrs->name;
      translator->m_providerName = foundTrs->provider;
   }
   else
   {
      AiMsgError("Translator %s was not correctly registered, no creator method", foundTrs->name.asChar());
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
   result = GetExtension(extensionFile);
   if (NULL == result)
   {
      CExtension extension = CExtension(extensionFile);
      ExtensionsList::iterator extIt;
      extIt = s_extensions.insert(s_extensions.end(), extension);
      if (s_extensions.end() != extIt)
      {
         result = &(*extIt);
      }
      else
      {
         AiMsgError("Could not create extension entry for: %s", extensionFile.asChar());
         result = NULL;
      }
   }
   else
   {
      AiMsgError("Extension already managed: %s", extensionFile.asChar());
      result = NULL;
   }

   return result;
}

MStatus CExtensionsManager::RegisterMayaNode(const CPxMayaNode &mayaNode)
{
   MStatus status;

   if (MFnPlugin::isNodeRegistered(mayaNode.name))
   {
      AiMsgError("Extension %s could not register node %s, already registered in Maya", mayaNode.provider.asChar(), mayaNode.name.asChar());
      return MStatus::kFailure;
   }
   status = MFnPlugin(s_plugin).registerNode(mayaNode.name, mayaNode.id,
         mayaNode.creator, mayaNode.initialize,
         mayaNode.type, mayaNode.classification);
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Extension %s registered node %s", mayaNode.provider.asChar(), mayaNode.name.asChar());
   }
   else
   {
      AiMsgError("Extension %s failed to register node %s", mayaNode.provider.asChar(), mayaNode.name.asChar());
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
      AiMsgWarning("Node %s not registered with Maya", mayaNode.name.asChar());
      return MStatus::kSuccess;
   }
   status = MFnPlugin(s_plugin).deregisterNode(mayaNode.id);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("Successfully deregistered node %s provided by extension %s", mayaNode.name.asChar(), mayaNode.provider.asChar());
   }
   else
   {
      AiMsgError("Failed to deregister node %s provided by extension %s", mayaNode.name.asChar(), mayaNode.provider.asChar());
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
