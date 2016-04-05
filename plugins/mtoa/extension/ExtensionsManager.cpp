
#include "ExtensionsManager.h"
#include "common/DynLibrary.h"
#include "AbMayaNode.h"
#include "AbTranslator.h"
#include "utils/Version.h"

#include "utils/Universe.h"

#include "nodes/ArnoldNodeIDs.h"

#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MFileObject.h>
#include <maya/MPlugArray.h>
#include <maya/MSceneMessage.h>


// CExtensionsManager


// void initializePlugin(CExtension& plugin)
// in an extension library
// TODO : do we need void uninitializePlugin(CExtension& plugin) ?

typedef void (*ExtensionInitFunction)(CExtension&);
typedef void (*ExtensionDeinitFunction)(CExtension&);

MayaNodesSet CExtensionsManager::s_registeredMayaNodes;
MayaNodeToTranslatorsMap CExtensionsManager::s_registeredTranslators;
DefaultTranslatorMap CExtensionsManager::s_defaultTranslators;
MObject CExtensionsManager::s_plugin;
ExtensionsList CExtensionsManager::s_extensions;
MCallbackId CExtensionsManager::s_pluginLoadedCallbackId = 0;

/// The Maya plugin it's used in (MtoA)
void CExtensionsManager::SetMayaPlugin(const MObject& plugin)
{
   s_plugin = plugin;
}

const MObject& CExtensionsManager::GetMayaPlugin()
{
   return s_plugin;
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
      status = builtin->RegisterPluginNodesAndTranslators("");
   }
   if (NULL != returnStatus) *returnStatus = status;
   return builtin;
}
/// Load an Arnold plugin.
///
/// Loads the Arnold plugin and registers a Maya node for each Arnold node
/// contained within it, if applicable.

///
/// @param path   the absolute path to an Arnold plugin
/// @param path   the absolute path to an Arnold plugin
/// @return       A pointer to the extension if loading was successful, else NULL
CExtension* CExtensionsManager::LoadArnoldPlugin(const MString &file,
                                                 const MString &path,
                                                 MStatus *returnStatus)
{
   MStatus status;
   // Create a CExtension to handle plugin loading and generate corresponding Maya nodes
   CExtension* pluginExtension = NULL;
   pluginExtension = NewExtension(file);
   if (NULL != pluginExtension)
   {
      // Extension loads the Arnold Plugin and will register new Maya nodes
      MString resolved;
      resolved = pluginExtension->LoadArnoldPlugin(file, path, &status);
      if (MStatus::kSuccess == status)
      {
         status = pluginExtension->setFile(resolved);
         // Do not register new nodes with Maya immediatly to allow modification
         // status = RegisterExtension(pluginExtension);
      } else {
         // Remove empty extension if plugin load failed
         DeleteExtension(pluginExtension);
      }
   }
   else
   {
      AiMsgError("[mtoa] There is already an extension of name %s", file.asChar());
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
         if (!CExtension::IsArnoldPluginLoaded(resolved))
         {
            MStatus plugStatus;
            LoadArnoldPlugin(resolved, "", &plugStatus);
            if (MStatus::kSuccess != plugStatus) status = plugStatus;
         }
         else
         {
            AiMsgDebug("[mtoa.extensions]  Arnold plugin %s already loaded, ignored.", resolved.asChar());
         }
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
   unsigned int nchars = file.numChars();
   MString libext = MString(LIBEXT);
   unsigned int next = libext.numChars();
   MString searchFile = file;
   if (nchars < next || libext != file.substringW(nchars-next, nchars-1))
   {
      searchFile += libext;
   }
   MString resolved = CExtension::FindFileInPath(searchFile, path, &status);
   CExtension* extension = NULL;

   if (MStatus::kSuccess == status)
   {
      AiMsgDebug("[mtoa.extensions]  Found extension file %s as %s.", file.asChar(), resolved.asChar());
      // Create a CExtension to represent loaded extension
      // TODO : store Library handle for unload
      while (NULL == extension)
      {
         extension = NewExtension(resolved);
         AiMsgInfo("[mtoa] Loading extension %s(%s).", extension->GetExtensionName().asChar(), extension->GetExtensionFile().asChar());

         void *pluginLib = LibraryLoad(extension->GetExtensionFile().asChar());
         if (pluginLib == NULL)
         {
            AiMsgError("[mtoa] Error loading extension library: %s.", LibraryLastError());
            DeleteExtension(extension);
            status = MStatus::kFailure;
            break;
         }
         extension->m_library = pluginLib;
         void* initializer = LibrarySymbol(pluginLib, "initializeExtension");
         if (initializer == NULL)
         {
            AiMsgError("[mtoa] Error initializing extension library: %s.", LibraryLastError());
            LibraryUnload(pluginLib);
            DeleteExtension(extension);
            status = MStatus::kFailure;
            break;
         }
         const char* (*apiVersionFunction)() = (const char* (*)())LibrarySymbol(pluginLib, "getAPIVersion");
         if (apiVersionFunction != NULL)
            extension->m_apiVersion = apiVersionFunction();
         else
            extension->m_apiVersion = MString("unknown");
         const ExtensionInitFunction &initFunc = (ExtensionInitFunction)(initializer);
         // ExtensionInitFunction * initFunc = (ExtensionInitFunction*)(&initializer);
         // Do the init
         (initFunc)(*extension);
         // (*initFunc)(*extension);
         // TODO MStatus returning initializeExtension?
         if (MStatus::kSuccess == status)
         {
            // Info
            unsigned int newNodes = extension->RegisteredNodesCount();
            unsigned int trsNodes = extension->TranslatedNodesCount();
            unsigned int trsCount = extension->TranslatorCount();

            AiMsgInfo("[mtoa] [%s] Declares a total of %i new Maya nodes.",
                  extension->GetExtensionName().asChar(), newNodes);
            AiMsgInfo("[mtoa] [%s] Declares a total of %i translators for %i Maya nodes (%i new and %i existing).",
                  extension->GetExtensionName().asChar(), trsCount, trsNodes, newNodes, trsNodes - newNodes);

            AiMsgInfo("[mtoa] Successfully loaded extension library %s(%s).",
                  extension->GetExtensionName().asChar(), extension->GetExtensionFile().asChar());
         }
         else
         {
            AiMsgError("[mtoa] Call to initializeExtension failed on extension library %s(%s).",
                  extension->GetExtensionName().asChar(), extension->GetExtensionFile().asChar());
            // TODO : deinitialize and unload ?
         }
         // Do not register now to allow to add calls (registerNode, etc) on it before
         // status = RegisterExtension(extension);
      }
   }
   else
   {
      if (path.numChars())
         AiMsgError("[mtoa] Could not find %s in search path %s", file.asChar(), path.asChar());
      else
         AiMsgError("[mtoa] Could not find %s", file.asChar());
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
         if (NULL == GetExtension(resolved))
         {
            MStatus extStatus;
            LoadExtension(resolved, "", &extStatus);
            if (MStatus::kSuccess != extStatus) status = extStatus;
         }
         else
         {
            AiMsgDebug("[mtoa.extensions]  MtoA extension %s already loaded, ignored.", resolved.asChar());
         }
      }
   }

   return status;
}

/// Unload an extension
MStatus CExtensionsManager::UnloadExtension(CExtension* extension)
{
   MStatus status = DoUnloadExtension(extension);
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("[mtoa] Unloaded extension %s(%s).",
         extension->GetExtensionName().asChar(), extension->GetExtensionFile().asChar());
      // Remove from list
      status = DeleteExtension(extension);
   } 
   return status;
}

/// Remove from the Arnold universe all nodes created by the specified extension and
/// deregister any Maya nodes generated for them.
/// Deregister all translators created by this extension
/// Restore what it may have overriden
MStatus CExtensionsManager::DoUnloadExtension(CExtension* extension)
{
   MStatus status = MStatus::kSuccess;

   MString extensionName = extension->GetExtensionName();
   MString extensionFile = extension->GetExtensionFile();
   AiMsgDebug("[mtoa] Unloading extension %s(%s).",
               extensionName.asChar(), extensionFile.asChar());

   if (extension->IsRegistered())
   {
      status = DeregisterExtension(extension);
   }
   if (MStatus::kSuccess == status)
   {
      // Unload all Arnold Plugins loaded from extension
      status = extension->UnloadArnoldPlugins();
      // Unload the extension library
      void* pluginLib = extension->m_library;
      if (pluginLib != NULL)
      {
         void* deinitializer = LibrarySymbol(extension->m_library, "deinitializeExtension");
         if (deinitializer != NULL)
         {
            const ExtensionDeinitFunction &deinitFunc = (ExtensionDeinitFunction)(deinitializer);
            // Do the deinit
            (deinitFunc)(*extension);
            // TODO MStatus returning deinitializeExtension?
            if (MStatus::kSuccess != status)
            {
               AiMsgError("[mtoa] Call to deinitializeExtension failed on extension library %s(%s).",
                           extensionName.asChar(), extensionFile.asChar());
            }
         }
         LibraryUnload(pluginLib);
      }
   }
   else
   {
      AiMsgError("[mtoa] Error deregistering extension library: %s(%s).",
                  extensionName.asChar(), extensionFile.asChar());
   }

   return status;
}

/// Unload all Extensions
///
MStatus CExtensionsManager::UnloadExtensions()
{
   MStatus status = MStatus::kSuccess;

   std::vector<CExtension*> unloaded;
   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   extIt = s_extensions.end();
   while (extIt != s_extensions.begin())
   {
      --extIt;
      CExtension *extension = &(*extIt);
      MStatus extStatus = DoUnloadExtension(extension);
      if (MStatus::kSuccess == extStatus)
      {
         unloaded.push_back(extension);
      }
      else
      {
         status = extStatus;
      }
   }
   // Delete the unloaded extensions from the list
   std::vector<CExtension*>::iterator it;
   for (it=unloaded.begin(); it!=unloaded.end(); it++)
   {
      MStatus extStatus = DeleteExtension(*it);
      if (MStatus::kSuccess != extStatus) status = extStatus;
   }

   CExtension::s_autoNodeId = ARNOLD_NODEID_AUTOGEN;

   return status;
}

MStatus CExtensionsManager::RegisterExtension(CExtension* extension)
{
   MString extName = extension->GetExtensionName();
   MString extFile = extension->GetExtensionFile().asChar();

   if (extension->IsRegistered())
   {
      AiMsgError("[mtoa] Already registered extension %s, provided by %s.",
            extName.asChar(), extFile.asChar());
      return MStatus::kFailure;
   }

   MStatus status = MStatus::kSuccess;
   // If this extension depends on a Maya plugin being loaded, defer
   // extension registration
   MStringArray requiredPlugins;
   requiredPlugins = extension->Required();
   for (unsigned int i=0; i<requiredPlugins.length(); i++)
   {
      MString pluginName = requiredPlugins[i];
      MObject plugin = MFnPlugin::findPlugin(pluginName);
      if (plugin.isNull())
      {
         AiMsgWarning("[mtoa] Extension %s(%s) requires Maya plugin %s, registering will be deferred until plugin is loaded.",
               extName.asChar(), extFile.asChar(), pluginName.asChar());
         extension->m_deferred = true;
         return MStatus::kNotFound;
      }
   }

   unsigned int regNewNodes = 0;
   unsigned int regTrsNodes = 0;
   unsigned int regTrsCount = 0;

   // We need to update all global maps with this extension's maps
   // and register the new Maya nodes provided by this extension with Maya

   // Add all Maya nodes registered by this extension to the global set
   AiMsgDebug("[mtoa.extensions] [%s] Registering new Maya nodes provided by %s.",
         extName.asChar(), extFile.asChar());
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
         AiMsgError("[mtoa] [%s] Tried to replace Maya node %s, provided by %s(%s) with Maya node %s, provided by %s(%s).",
               extName.asChar(),
               mayaNode->name.asChar(), mayaNode->provider.asChar(), mayaNode->file.asChar(),
               existingMayaNode->name.asChar(), existingMayaNode->provider.asChar(), existingMayaNode->file.asChar());
         status = MStatus::kFailure;
         continue;
      }
      MStatus nodeStatus = RegisterMayaNode(*mayaNode);
      // Only add successfully registered nodes to the list
      if (MStatus::kSuccess == nodeStatus)
      {
         std::pair<MayaNodesSet::iterator, bool> ret;
         ret = s_registeredMayaNodes.insert(*mayaNode);
         regNewNodes++;
      }
      else
      {
         // FIXME: whether or not to create a maya node is determined in CExtension::RegisterNode, depending
         // on if a node with the specified name already exists. For deferred extensions
         // this may occur before the required maya plugin is loaded, and therefore the counterpart node will
         // not exist and the extension will erroneously mark the node for creation.
         // A temporary fix is to allow this node creation error to slip by for deferred extensions
         if (extension->IsDeferred())
            nodeStatus = MS::kSuccess;
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
   AiMsgDebug("[mtoa] [%s] Registering new translators provided by %s.",
         extName.asChar(), extFile.asChar());
   MayaNodeToTranslatorsMap::iterator tnodeIt;
   for (tnodeIt = extension->m_registeredTranslators.begin();
         tnodeIt != extension->m_registeredTranslators.end();
         tnodeIt++)
   {
      mayaNode = &tnodeIt->first;
      TranslatorsSet &newTrans = tnodeIt->second;

      if (newTrans.size() > 0)
      {
         TranslatorsSet* oldTrans = FindRegisteredTranslators(*mayaNode);
         if (NULL == oldTrans)
         {
            s_registeredTranslators[*mayaNode] = TranslatorsSet();
            oldTrans = &s_registeredTranslators[*mayaNode];
            regTrsNodes++;
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
               AiMsgDebug("[mtoa] [%s] [maya %s] Added translator %s to arnold %s, provided by %s(%s).",
                     extName.asChar(),
                     mayaNode->name.asChar(),
                     translator.name.asChar(),
                     translator.arnold.asChar(),
                     translator.provider.asChar(), translator.file.asChar());
            }
            else
            {
               AiMsgDebug("[mtoa] [%s] [maya %s] Replaced translator %s to arnold %s, provided by %s(%s) with translator %s to arnold %s, provided by %s(%s).",
                     extName.asChar(),
                     mayaNode->name.asChar(),
                     translator.name.asChar(),
                     translator.arnold.asChar(),
                     translator.provider.asChar(), translator.file.asChar(),
                     ret.first->name.asChar(),
                     ret.first->arnold.asChar(),
                     ret.first->provider.asChar(), ret.first->file.asChar());
               oldTrans->erase(ret.first);
               oldTrans->insert(translator);
               // TODO : remove old additionnal attributes before we add new ones ?
            }
            if (NULL != translator.initialize)
            {
               MString mayaName = mayaNode->name;
               AiMsgDebug("[mtoa] [%s] [maya %s] Calling initialize function for translator %s to arnold %s, provided by %s(%s).",
                     extName.asChar(),   
                     mayaNode->name.asChar(),
                     translator.name.asChar(),
                     translator.arnold.asChar(),
                     translator.provider.asChar(), translator.file.asChar());
               translator.initialize(CAbTranslator(translator.name, translator.arnold, mayaNode->name, translator.provider));
            }
            regTrsCount++;
         }
         // Add aiTranslator if more than one translator
         if (oldTrans->size() > 1) {
            CExtensionAttrHelper helper(mayaNode->name);
            if (helper.GetMayaNodeTypeId() != MTypeId(MFn::kInvalid))
            {
               AiMsgDebug("[mtoa] [%s] [maya %s] Multiple translators, adding \"aiTranslator\" attribute to Maya node",
                  extName.asChar(), mayaNode->name.asChar());
               CAttrData data;
               data.name = "aiTranslator";
               data.shortName = "ai_translator";
               helper.MakeInputString(data);
            } 
            else
            {
               AiMsgDebug("[mtoa] [%s] [maya %s] Multiple translators, but inexistant Maya node type",
                  extName.asChar(), mayaNode->name.asChar());
            }
         }
      }
   }

   // Info
   unsigned int declNewNodes = extension->RegisteredNodesCount();
   // unsigned int declTrsNodes = extension->TranslatedNodesCount();
   unsigned int declTrsCount = extension->TranslatorCount();

   if (regNewNodes == declNewNodes)
   {
      AiMsgDebug("[mtoa] [%s] Successfully registered %i out of %i new Maya nodes it declares in %s.",
         extName.asChar(), regNewNodes, declNewNodes, extFile.asChar());
   }
   else
   {
      AiMsgWarning("[mtoa] [%s] Only managed to register %i out of %i new Maya nodes it declares in %s.",
         extName.asChar(), regNewNodes, declNewNodes, extFile.asChar());
   }
   if (regTrsCount == declTrsCount)
   {
      AiMsgDebug("[mtoa] [%s] Successfully registered %i out of %i translators it declares in %s.",
         extName.asChar(), regTrsCount, declTrsCount, extFile.asChar());
   }
   else
   {
      AiMsgWarning("[mtoa] [%s] Only managed to register %i out of %i translators it declares in %s.",
         extName.asChar(), regTrsCount, declTrsCount, extFile.asChar());
   }

   // Final status
   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("[mtoa] Registered extension %s provided by %s.", extName.asChar(), extFile.asChar());

      extension->m_registered = true;
      extension->m_deferred = false;
      // Load associated scripts
      MString cmd = "import mtoa.api.extensions;mtoa.api.extensions.loadExtensionUI('" + extension->GetExtensionFile() + "')";
      CHECK_MSTATUS(MGlobal::executePythonCommand(cmd));
   }
   else
   {
      AiMsgError("[mtoa] Could not register extension %s provided by %s.", extName.asChar(), extFile.asChar());
   }

   return status;
}

void CExtensionsManager::SetDefaultTranslator(const MString& mayaTypeName, const MString& translatorName)
{
   s_defaultTranslators[mayaTypeName.asChar()] = translatorName;
}

MString CExtensionsManager::GetDefaultTranslator(const MString& nodeName)
{
   DefaultTranslatorMap::const_iterator it = s_defaultTranslators.find(nodeName.asChar());
   if (it == s_defaultTranslators.end())
      return "";
   else
      return it->second;
}

MStatus CExtensionsManager::RegisterExtensions()
{
   MStatus status = MStatus::kSuccess;

   // s_extensions is a std::list of extensions (ordered in load order)
   AiMsgInfo("[mtoa] Registering %i loaded extensions:", (int)s_extensions.size());
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (!extIt->IsRegistered() && !extIt->IsDeferred())
      {
         MStatus extStatus = RegisterExtension(&(*extIt));
         // Only in case of failure (ignore kNotFound for deferred extensions)
         if (MStatus::kFailure == extStatus) status = extStatus;
      }
   }

   return status;
}

MStatus CExtensionsManager::DeregisterExtension(CExtension* extension)
{
   if (!extension->IsRegistered())
   {
      if (extension->IsDeferred())
      {
         extension->m_deferred = false;
         return MStatus::kSuccess;
      }
      else
      {
         AiMsgError("[mtoa] This extension is not registered or deferred: %s(%s).",
            extension->GetExtensionName().asChar(), extension->GetExtensionFile().asChar());
         return MStatus::kFailure;
      }
   }

   MStatus status = MStatus::kSuccess;

   // Unregister all Maya nodes this extensions has registered
   AiMsgDebug("[mtoa] Deregistering new Maya nodes provided by %s(%s).",
         extension->GetExtensionName().asChar(), extension->GetExtensionFile().asChar());
   MayaNodesSet::iterator rnodeIt;
   rnodeIt = s_registeredMayaNodes.begin();
   while (rnodeIt != s_registeredMayaNodes.end())
   {
      MayaNodesSet::iterator node = rnodeIt++;
      if (node->provider == extension->GetExtensionName()
            &&( (node->file == extension->GetExtensionFile()) || (node->file == extension->GetExtensionName())) )
      {
         MStatus nodeStatus = DeregisterMayaNode(*node);
         if (MStatus::kSuccess == nodeStatus)
         {
            s_registeredMayaNodes.erase(node);
         }
         // Update return status
         if (MStatus::kSuccess != nodeStatus)
         {
            status = nodeStatus;
         }
      }
   }

   // remove translators from the list
   AiMsgDebug("[mtoa.extensions]  Deregistering translators provided by %s(%s).",
         extension->GetExtensionName().asChar(), extension->GetExtensionFile().asChar());
   MayaNodeToTranslatorsMap::iterator tnodeIt;
   for (tnodeIt = extension->m_registeredTranslators.begin();
         tnodeIt != extension->m_registeredTranslators.end();
         tnodeIt++)
   {
      // mayaNode = &tnodeIt->first;
      TranslatorsSet &existingTrans = tnodeIt->second;
      TranslatorsSet::iterator trsIt;
      trsIt = existingTrans.begin();
      while (trsIt != existingTrans.end())
      {
         TranslatorsSet::iterator trs = trsIt++;
         if (trs->provider == extension->GetExtensionName()
               && trs->file == extension->GetExtensionFile())
         {
            existingTrans.erase(trs);
         }
         // TODO : remove old additionnal attributes before we add new ones ?
         // TODO : restore overloaded translators
      }
   }

   if (MStatus::kSuccess == status)
   {
      AiMsgInfo("[mtoa] Deregistered extension %s(%s).", extension->GetExtensionName().asChar(), extension->GetExtensionFile().asChar());
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
      if (extIt->IsRegistered() || extIt->IsDeferred())
      {
         MStatus extStatus = DeregisterExtension(&(*extIt));
         if (MStatus::kSuccess != extStatus) status = extStatus;
      }
   }

   return status;
}

/// Get a instancied CDagTranslator translator for the given node (MObject)
/// using the value of the "aiTranslator" attribut
/// to provide an optional name to choose between multiple translators
CDagTranslator* CExtensionsManager::GetTranslator(const MDagPath &dagPath)
{
   MObject node = dagPath.node();
   return (CDagTranslator*)GetTranslator(node);
}

/// Get a instancied CNodeTranslator translator for the given node (MObject)
/// using the value of the "aiTranslator" attribut
/// to provide an optional name to choose between multiple translators
CNodeTranslator* CExtensionsManager::GetTranslator(const MObject &object)
{
   MFnDependencyNode depFn(object);
   MString typeName = depFn.typeName();
   MTypeId typeId = depFn.typeId();
   // MString transName = BUILTIN;
   MString transName = "";
   if (typeName != "objectSet")
   {
      MStatus status;
      MPlug plug = depFn.findPlug("aiTranslator", &status);
      if (status == MStatus::kSuccess)
      {
         MObject attr = plug.attribute();
         if (attr.hasFn(MFn::kEnumAttribute))
         {
            MFnEnumAttribute fnAttr(attr, &status);
            transName = fnAttr.fieldName(plug.asInt());
         }
         else
         {
            transName = plug.asString();
         }
      }
   }

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
      // AiMsgDebug("[maya %s] No registered translator, it won't be exported.", mayaNode.name.asChar());
      return NULL;
   }
   const CPxTranslator* foundTrs = NULL;
   CPxTranslator searchTrs(translatorName);
   if (translatorName != "")
   {
      foundTrs = FindRegisteredTranslator(mayaNode, searchTrs);
      if (NULL == foundTrs)
      {
         AiMsgWarning("[mtoa.extensions]  %s Found no translator named \"%s\"",
               mayaNode.name.asChar(), translatorName.asChar());
         return NULL;
      }
   }
   else
   {
      foundTrs = FindRegisteredTranslator(mayaNode, searchTrs);
      AiMsgDebug("[mtoa.extensions]  %s Using translator %s, provided by %s(%s).",
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
      translator->m_abstract = CAbTranslator(foundTrs->name, foundTrs->arnold, mayaNode.name, foundTrs->provider);
   }
   else
   {
      AiMsgError("[mtoa] [maya %s] Translator %s has no creator method, incorrectly registered by %s(%s).",
         mayaNode.name.asChar(), foundTrs->name.asChar(),
         foundTrs->provider.asChar(), foundTrs->file.asChar());
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
         if (noTest || provider == trsIt->provider)
         {
            result.append(trsIt->name);
            result.append(trsIt->arnold);
         }
      }
   }
   return result;
}
/*
void CExtensionsManager::GetAOVs(MStringArray& result)
{
   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      std::map<std::string, int>::iterator it;
      for (it = extIt->m_aovTypes.begin(); it != extIt->m_aovTypes.end(); ++it)
      {
         result.append(it->first.c_str());
      }
   }
}

void CExtensionsManager::GetNodeAOVs(const MString &mayaType, MStringArray& result)
{
   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      std::vector<CAOVData> dataList = extIt->m_aovAttrs[mayaType.asChar()];
      std::vector<CAOVData>::iterator it;
      for (it = dataList.begin(); it != dataList.end(); ++it)
      {
         result.append(it->name);
         result.append(it->attribute);
      }
   }
}

void CExtensionsManager::GetNodeTypesWithAOVs(MStringArray& result)
{
   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      std::map<std::string, std::vector<CAOVData> >::iterator it;
      for (it = extIt->m_aovAttrs.begin(); it!=extIt->m_aovAttrs.end(); ++it)
      {
         result.append(it->first.c_str());
      }
   }
}
*/

void CExtensionsManager::GetAOVs(MStringArray& result)
{
   MayaNodeToTranslatorsMap::iterator transIt;
   for (transIt = s_registeredTranslators.begin();
         transIt != s_registeredTranslators.end();
         transIt++)
   {
      for (unsigned int i = 0; i < transIt->first.m_aovs.size(); ++i)
      {
         result.append(transIt->first.m_aovs[i].name);
      }
   }
}

void CExtensionsManager::GetNodeAOVs(const MString &mayaTypeName, MStringArray& result)
{
   CPxMayaNode mayaType(mayaTypeName);
   MayaNodeToTranslatorsMap::iterator transIt = s_registeredTranslators.find(mayaType);
   if (transIt == s_registeredTranslators.end())
      return;

   mayaType = transIt->first;

   for (unsigned int i = 0; i < mayaType.m_aovs.size(); ++i)
   {
      result.append(mayaType.m_aovs[i].name);
      result.append(mayaType.m_aovs[i].attribute);
      MString type = AiParamGetTypeName(mayaType.m_aovs[i].type);
      result.append(type.toLowerCase());
   }
}

void CExtensionsManager::GetNodeTypesWithAOVs(MStringArray& result)
{
   MayaNodeToTranslatorsMap::iterator transIt;
   for (transIt = s_registeredTranslators.begin();
         transIt != s_registeredTranslators.end();
         transIt++)
   {
      if (transIt->first.m_aovs.size())
      {
         result.append(transIt->first.name);
      }
   }
}

CExtension* CExtensionsManager::GetExtension(const MString &extensionFile)
{
   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (extIt->GetExtensionFile() == extensionFile) return &(*extIt);
   }
   return NULL;
}

CExtension* CExtensionsManager::GetExtensionByName(const MString &extensionName)
{
   // s_extensions is a std::list of extensions (ordered in load order)
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (extIt->GetExtensionName() == extensionName) return &(*extIt);
   }
   return NULL;
}

/// Called when a Maya plugin is loaded to register deferred extensions
void CExtensionsManager::MayaPluginLoadedCallback(const MStringArray &strs, void *clientData)
{
   // 0 = pluginPath, 1 = pluginName
   MString pluginName = strs[1];
   std::string plugin_str(pluginName.asChar());
   // start up the arnold universe so that attribute helpers can query arnold nodes
   bool AiUniverseCreated = ArnoldUniverseBegin();
   ExtensionsList::iterator extIt;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (extIt->IsDeferred()
            && extIt->m_requiredMayaPlugins.find(plugin_str)
            != extIt->m_requiredMayaPlugins.end())
      {
         RegisterExtension(&(*extIt));
      }
   }
   if (AiUniverseCreated) ArnoldUniverseEnd();
}

/// Installs the plugin-loaded callback
MCallbackId CExtensionsManager::CreatePluginLoadedCallback()
{
   MStatus status;
   // create callback
   s_pluginLoadedCallbackId = MSceneMessage::addStringArrayCallback(MSceneMessage::kAfterPluginLoad, CExtensionsManager::MayaPluginLoadedCallback, NULL, &status);
   CHECK_MSTATUS(status);
   return s_pluginLoadedCallbackId;
}

MStatus CExtensionsManager::RemovePluginLoadedCallback()
{
   MStatus status;
   // delete callbacks
   if (s_pluginLoadedCallbackId != 0)
   {
      status = MMessage::removeCallback(s_pluginLoadedCallbackId);
      if (MStatus::kSuccess == status) s_pluginLoadedCallbackId = 0;
   }
   return status;
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
      if (extIt->GetExtensionFile() == extensionFile) break;
   }
   if (s_extensions.end() == extIt)
   {
      CExtension extension = CExtension(extensionFile);
      s_extensions.push_back(extension);
      result = &s_extensions.back();
   }
   else
   {
      AiMsgError("[mtoa] Extension already managed: %s", extensionFile.asChar());
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
      if (extIt->GetExtensionFile() == extension->GetExtensionFile()) break;
   }
   if (s_extensions.end() != extIt)
   {
      s_extensions.erase(extIt);
      extension = NULL;
      status = MStatus::kSuccess;
   }
   else
   {
      AiMsgError("[mtoa] Extension not managed: %s(%s)",
         extension->GetExtensionName().asChar(),extension->GetExtensionFile().asChar());
      status = MStatus::kFailure;
   }

   return status;
}

MStatus CExtensionsManager::RegisterMayaNode(const CPxMayaNode &mayaNode)
{
   MStatus status;

   if (MFnPlugin::isNodeRegistered(mayaNode.name))
   {
      AiMsgError("[mtoa] Cannot register already existing Maya node %s, provided by %s(%s).",
            mayaNode.name.asChar(),
            mayaNode.provider.asChar(), mayaNode.file.asChar());
      return MStatus::kFailure;
   }
   // Construct the abstract to store in the MPx class's static s_abstract member,
   // if a pointer to one was provided in the proxy for that Maya node class.
   CAbMayaNode abstract(mayaNode.name, mayaNode.arnold, mayaNode.classification, mayaNode.provider);

   if (NULL != mayaNode.abstract) *mayaNode.abstract = abstract;
   const MString *classificationPtr = (mayaNode.classification == "") ? NULL : &mayaNode.classification;
   status = MFnPlugin(s_plugin, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION).registerNode(
         mayaNode.name, mayaNode.id,
         mayaNode.creator, mayaNode.initialize,
         mayaNode.type, classificationPtr );
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      AiMsgDebug("[mtoa] Registered Maya node %s provided by %s(%s).",
            mayaNode.name.asChar(),
            mayaNode.provider.asChar(), mayaNode.file.asChar());
   }
   else
   {
      AiMsgError("[mtoa] Failed to register node %s provided by %s(%s).",
            mayaNode.name.asChar(),
            mayaNode.provider.asChar(), mayaNode.file.asChar());
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
      AiMsgWarning("[mtoa] Node %s does not exist in Maya", mayaNode.name.asChar());
      return MStatus::kSuccess;
   }
   status = MFnPlugin(s_plugin).deregisterNode(mayaNode.id);
   if (MStatus::kSuccess == status)
   {
      AiMsgDebug("[mtoa.extensions]  Successfully deregistered node %s provided by %s(%s).",
            mayaNode.name.asChar(),
            mayaNode.provider.asChar(), mayaNode.file.asChar());
   }
   else
   {
      AiMsgError("[mtoa] Failed to deregister node %s provided by %s(%s).",
            mayaNode.name.asChar(),
            mayaNode.provider.asChar(), mayaNode.file.asChar());
   }
   return status;
}

/// Return whether a maya node was generated by an mtoa extension
const bool CExtensionsManager::IsRegisteredMayaNode(const MString &mayaNodeType)
{
   const CPxMayaNode mayaNode(mayaNodeType);
   MayaNodesSet::iterator nodeIt;
   nodeIt = s_registeredMayaNodes.find(mayaNode);
   return nodeIt != s_registeredMayaNodes.end();
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
      TranslatorsSet::iterator it = allTranslators->find(GetDefaultTranslator(mayaNode.name));
      if (it == allTranslators->end())
         result = &(*--allTranslators->end());
      else
         result = &(*it);
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

MStringArray CExtensionsManager::ListLoadedExtensions()
{
   MStringArray ret;
   for (std::list<CExtension>::iterator it = s_extensions.begin(); it != s_extensions.end(); ++it)
      ret.append((*it).GetExtensionName());
   return ret;
}
