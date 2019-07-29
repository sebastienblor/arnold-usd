#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "ExtensionsManager.h"
#include "ExtensionImpl.h"

#include "common/DynLibrary.h"
#include "AbMayaNode.h"
#include "AbTranslator.h"
#include "utils/Version.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "utils/Universe.h"

#include "nodes/ArnoldNodeIDs.h"
#include "nodes/shape/ArnoldProceduralNode.h"
#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MFileObject.h>
#include <maya/MPlugArray.h>
#include <maya/MSceneMessage.h>
#include <maya/MDGModifier.h>


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
OperatorsMap CExtensionsManager::s_operators;
CustomShapesMap CExtensionsManager::s_customShapes;

static unordered_set<std::string>  s_deferredExtensions;

static unordered_set<std::string>  s_mayaExtensionClasses;
static unordered_set<std::string>  s_extensionAttributes;

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
                                                 MStatus *returnStatus,
                                                 bool registerOnly)
{
   MStatus status;

   if (file == "mtoa")
   { // nodes created by MtoA have this file name
      if(returnStatus)
         *returnStatus = MS::kSuccess;
      return NULL;
   }

   // Create a CExtension to handle plugin loading and generate corresponding Maya nodes
   CExtension* pluginExtension = NULL;
   pluginExtension = NewExtension(file);
   if (NULL != pluginExtension)
   {
      std::string fileStr(file.asChar());
      std::replace(fileStr.begin(), fileStr.end(), '\\', '/');

      // Extension loads the Arnold Plugin and will register new Maya nodes
      MString resolved(fileStr.c_str());

      if (registerOnly)
         status = pluginExtension->RegisterPluginNodesAndTranslators(file);
      else
         resolved = pluginExtension->LoadArnoldPlugin(file, path, &status);

      if (MStatus::kSuccess == status)
      {
         status = pluginExtension->m_impl->setFile(resolved);
         // Do not register new nodes with Maya immediatly to allow modification
         // status = RegisterExtension(pluginExtension);
      } else {
         // Remove empty extension if plugin load failed
         DeleteExtension(pluginExtension);
      }
   }
   else
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] There is already an extension of name " + file);
   }

   if (NULL != returnStatus) *returnStatus = status;
   return pluginExtension;
}

/// Load all Arnold plugins on the plugin path
///
MStatus CExtensionsManager::LoadArnoldPlugins(const MString &path)
{
   MStatus status = MStatus::kSuccess;

   // Let Arnold search for plugins in path
   AiLoadPlugins(path.expandEnvironmentVariablesAndTilde().asChar());

   // Register any new node entries
   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_ALL);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
      MString filename = ArnoldGetEntryFile(nentry);

      // skip builtins and already registered
      if (filename.length() > 0 && !CExtension::IsArnoldPluginLoaded(filename))
      {
         MStatus pluginStatus;
         LoadArnoldPlugin(filename, path, &pluginStatus, true);
         if (MStatus::kSuccess != pluginStatus && MStatus::kNotFound != pluginStatus) status = pluginStatus;
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
   MString resolved = CExtensionImpl::FindFileInPath(searchFile, path, &status);
   CExtension* extension = NULL;

   if (MStatus::kSuccess == status)
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.ext]  Found extension file "+file+" as "+resolved);

      // Create a CExtension to represent loaded extension
      // TODO : store Library handle for unload
      while (NULL == extension)
      {
         extension = NewExtension(resolved);
         if (MtoaTranslationInfo())
            MtoaDebugLog("[mtoa] Loading extension "+ extension->GetExtensionName() +"("+extension->GetExtensionFile()+").");

         void *pluginLib = LibraryLoad(extension->GetExtensionFile().asChar());
         if (pluginLib == NULL)
         {
            MString msg = MString("[mtoa] Error loading extension library: ")+ MString(LibraryLastError());
            AiMsgWarning(msg.asChar());

            DeleteExtension(extension);
            status = MStatus::kFailure;
            break;
         }
         extension->m_impl->m_library = pluginLib;
         void* initializer = LibrarySymbol(pluginLib, "initializeExtension");
         if (initializer == NULL)
         {            
            MString msg = MString("[mtoa] Error initializing extension library: ") + MString(LibraryLastError());
            AiMsgWarning(msg.asChar());

            LibraryUnload(pluginLib);
            DeleteExtension(extension);
            status = MStatus::kFailure;
            break;
         }
         const char* (*apiVersionFunction)() = (const char* (*)())LibrarySymbol(pluginLib, "getAPIVersion");
         if (apiVersionFunction != NULL)
            extension->m_impl->m_apiVersion = apiVersionFunction();
         else
            extension->m_impl->m_apiVersion = MString("unknown");
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

            if (MtoaTranslationInfo())
            {
               MString log = "[mtoa] ["+ extension->GetExtensionName()+" declares a total of ";
               log += newNodes;
               log += " new Maya nodes.";
               MtoaDebugLog(log);

               log = "[mtoa] [" + extension->GetExtensionName() + " declares a total of ";
               log += trsCount;
               log += " translators for ";
               log += trsNodes;
               log += " Maya nodes (";
               log += newNodes;
               log += " new and ";
               log += (trsNodes - newNodes);
               log += " existing).";
               MtoaDebugLog(log);

               MtoaDebugLog("[mtoa] Successfully loaded extension library " + extension->GetExtensionName() + "("+extension->GetExtensionFile() +")");
            }
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
   MStatus status = MStatus::kSuccess;

   MStringArray extensions;
   extensions = CExtensionImpl::FindLibraries(path, &status);
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
            if (MtoaTranslationInfo())
               MtoaDebugLog("[mtoa.ext]  MtoA extension "+resolved+" already loaded, ignored.");
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
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] Unloaded extension "+extension->GetExtensionName()+"("+extension->GetExtensionFile()+").");         
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
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa] Unloading extension "+extensionName+"("+extensionFile+").");

   if (extension->IsRegistered())
   {
      status = DeregisterExtension(extension);
   }
   if (MStatus::kSuccess == status)
   {
      // Unload all Arnold Plugins loaded from extension
      status = extension->m_impl->UnloadArnoldPlugins();
      // Unload the extension library
      void* pluginLib = extension->m_impl->m_library;
      if (pluginLib != NULL)
      {
         void* deinitializer = LibrarySymbol(extension->m_impl->m_library, "deinitializeExtension");
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

   CExtensionImpl::s_autoNodeId = ARNOLD_NODEID_AUTOGEN;

   return status;
}

MStatus CExtensionsManager::RegisterExtension(CExtension* extension)
{
   MString extName = extension->GetExtensionName();
   MString extFile = extension->GetExtensionFile().asChar();

   if (extension->IsRegistered())
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] Already registered extension "+extName+", provided by "+extFile);            
      return MStatus::kNotFound;
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
         AiMsgInfo("[mtoa] Extension %s(%s) requires Maya plugin %s, registering will be deferred until plugin is loaded.",
               extName.asChar(), extFile.asChar(), pluginName.asChar());
         extension->m_impl->m_deferred = true;
         return MStatus::kNotFound;
      }
   }

   unsigned int regNewNodes = 0;
   unsigned int regTrsNodes = 0;
   unsigned int regTrsCount = 0;

   // We need to update all global maps with this extension's maps
   // and register the new Maya nodes provided by this extension with Maya

   // Add all Maya nodes registered by this extension to the global set
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.ext] ["+extName+"] Registering new Maya nodes provided by "+extFile);

   const CPxMayaNode* mayaNode;
   const CPxMayaNode* existingMayaNode;
   MayaNodesSet::iterator rnodeIt;
   for (rnodeIt = extension->m_impl->m_registeredMayaNodes.begin();
         rnodeIt != extension->m_impl->m_registeredMayaNodes.end();
         rnodeIt++)
   {
      mayaNode = &(*rnodeIt);
      existingMayaNode = FindRegisteredMayaNode(*mayaNode);
      if (NULL != existingMayaNode)
      {
         // TODO : allow node overriding?
         if (MtoaTranslationInfo())
            MtoaDebugLog("[mtoa] ["+extName+"] Tried to replace Maya node "+mayaNode->name+", provided by "+mayaNode->provider+"("+mayaNode->file+") with Maya node "+existingMayaNode->name+", provided by "+existingMayaNode->provider+"("+existingMayaNode->file+").");
                
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
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa] ["+extName+"] Registering new translators provided by "+extFile);

   MayaNodeToTranslatorsOldMap::iterator tnodeIt;
   for (tnodeIt = extension->m_impl->m_registeredTranslators.begin();
         tnodeIt != extension->m_impl->m_registeredTranslators.end();
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
         } else {
            // search for the original registered CPxMayaNode
            MayaNodeToTranslatorsMap::iterator prevIt = s_registeredTranslators.find(*mayaNode);
            if (prevIt != s_registeredTranslators.end())
            {
               CPxMayaNode *prevMayaNode = (CPxMayaNode*)(&(prevIt->first));
               // For each of these fields in CPxMayaNode that was left to its default value
               // we want to set the new value.
               // This situation might happen when an extension was first loaded for a given Maya node
               // and then an arnold shader was registered with the same maya.name (#3252).
               // In that case there will be two different CExtension classes, but as they both are for the
               // same Maya node type, the shader's data will be ignored, and the second CPxMayaNode will be discarded.
               // TODO: verify that we're not missing other information about the CPxMayaNode
               if (!prevMayaNode->m_aovShader)
                  prevMayaNode->m_aovShader = mayaNode->m_aovShader;

               if (prevMayaNode->m_aovs.empty())
                  prevMayaNode->m_aovs = mayaNode->m_aovs;

               if (prevMayaNode->arnold.length() == 0)
                  prevMayaNode->arnold = mayaNode->arnold;

               if (prevMayaNode->classification.length() == 0)
                  prevMayaNode->classification = mayaNode->classification;
            }

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
               if (MtoaTranslationInfo())
                  MtoaDebugLog("[mtoa] ["+extName+"] [maya "+mayaNode->name+"] Added translator "+translator.name+" to arnold "+translator.arnold+", provided by "+translator.provider+"("+translator.file+").");
            }
            else
            {
               if (MtoaTranslationInfo())
                  MtoaDebugLog("[mtoa] ["+extName+"] [maya "+mayaNode->name+"] Replaced translator "+translator.name+" to arnold "+translator.arnold+", provided by "+translator.provider+"("+translator.file+") with translator "+ret.first->name+" to arnold "+ret.first->arnold+", provided by "+ret.first->provider+"("+ret.first->file+").");

               oldTrans->erase(ret.first);
               oldTrans->insert(translator);
               // TODO : remove old additionnal attributes before we add new ones ?
            }
            if (NULL != translator.initialize)
            {
               MString mayaName = mayaNode->name;
               if (MtoaTranslationInfo())
                  MtoaDebugLog("[mtoa] ["+extName+"] [maya "+mayaNode->name+"] Calling initialize function for translator "+translator.name+" to arnold "+translator.arnold+", provided by "+translator.provider+"("+translator.file+").");
                     
               translator.initialize(CAbTranslator(translator.name, translator.arnold, mayaNode->name, translator.provider));
            }
            regTrsCount++;
         }
         // Add aiTranslator if more than one translator
         if (oldTrans->size() > 1) {
            CExtensionAttrHelper helper(mayaNode->name);
            if (helper.GetMayaNodeTypeId() != MTypeId(MFn::kInvalid))
            {
               if (MtoaTranslationInfo())
                  MtoaDebugLog("[mtoa] ["+extName+"] [maya "+mayaNode->name+"] Multiple translators, adding \"aiTranslator\" attribute to Maya node");
                  
               CAttrData data;
               data.name = "aiTranslator";
               data.shortName = "ai_translator";
               data.keyable = false;
               data.channelBox = false;
               helper.MakeInputString(data);
            } 
            else
            {
               if (MtoaTranslationInfo())
                  MtoaDebugLog("[mtoa] ["+extName+"] [maya "+mayaNode->name+"] Multiple translators, but inexistant Maya node type");
                  
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
      if (MtoaTranslationInfo())
      {
         MString log ="[mtoa] ["+ extName+"] Successfully registered ";
         log += regNewNodes;
         log += " out of ";
         log += declNewNodes;
         log += " new Maya nodes it declares in ";
         log += extFile;
         MtoaDebugLog(log);
      }
   }
   else
   {
      AiMsgWarning("[mtoa] [%s] Only managed to register %i out of %i new Maya nodes it declares in %s.",
         extName.asChar(), regNewNodes, declNewNodes, extFile.asChar());
   }
   if (regTrsCount == declTrsCount)
   {
      if (MtoaTranslationInfo())
      {
         MString log = "[mtoa] ["+extName+"] Successfully registered ";
         log += regTrsCount;
         log += " out of ";
         log += declTrsCount;
         log +=" translators it declares in ";
         log += extFile;
         MtoaDebugLog(log);
      }
   }
   else
   {
      AiMsgWarning("[mtoa] [%s] Only managed to register %i out of %i translators it declares in %s.",
         extName.asChar(), regTrsCount, declTrsCount, extFile.asChar());
   }

   // Final status
   if (MStatus::kSuccess == status)
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] Registered extension "+extName+" provided by "+extFile);

      extension->m_impl->m_registered = true;
      extension->m_impl->m_deferred = false;
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
   if (MtoaTranslationInfo())
   {
      MString log = "[mtoa] Registering ";
      log += (int)s_extensions.size();
      log += "  loaded extensions";
      MtoaDebugLog(log);
   }

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
         extension->m_impl->m_deferred = false;
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
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa] Deregistering new Maya nodes provided by "+extension->GetExtensionName()+"("+extension->GetExtensionFile()+").");

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
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.ext]  Deregistering translators provided by "+extension->GetExtensionName()+"("+extension->GetExtensionFile()+").");

   MayaNodeToTranslatorsOldMap::iterator tnodeIt;
   for (tnodeIt = extension->m_impl->m_registeredTranslators.begin();
         tnodeIt != extension->m_impl->m_registeredTranslators.end();
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
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] Deregistered extension "+extension->GetExtensionName()+"("+ extension->GetExtensionFile()+").");

      extension->m_impl->m_registered = false;
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
      MPlug plug = depFn.findPlug("aiTranslator", true, &status);
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
         if (translatorName != MString("maya"))
            AiMsgWarning("[mtoa.ext]  %s Found no translator named \"%s\"",
               mayaNode.name.asChar(), translatorName.asChar());
         return NULL;
      }
   }
   else
   {
      foundTrs = FindRegisteredTranslator(mayaNode, searchTrs);
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.ext]  "+mayaNode.name+" Using translator "+foundTrs->name+", provided by "+foundTrs->provider+"("+foundTrs->file+").");
         
   }

   // Instanciate a new translator using proxy info
   if (NULL != foundTrs->creator)
   {
      TCreatorFunction creatorFunction = foundTrs->creator;
      translator = (CNodeTranslator*)creatorFunction();
      translator->CreateImplementation();

      // This customize the prototype instance of the translator
      // with the information found in the translator class proxy
      translator->m_impl->m_abstract = CAbTranslator(foundTrs->name, foundTrs->arnold, mayaNode.name, foundTrs->provider);

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
      unordered_map<std::string, int>::iterator it;
      for (it = extIt->m_impl->aovTypes.begin(); it != extIt->m_impl->aovTypes.end(); ++it)
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
      std::vector<CAOVData> dataList = extIt->m_impl->aovAttrs[mayaType.asChar()];
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
      unordered_map<std::string, std::vector<CAOVData> >::iterator it;
      for (it = extIt->m_impl->aovAttrs.begin(); it!=extIt->m_impl->aovAttrs.end(); ++it)
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

void CExtensionsManager::GetAOVShaders(MStringArray& result)
{
   MayaNodeToTranslatorsMap::iterator transIt;
   for (transIt = s_registeredTranslators.begin();
         transIt != s_registeredTranslators.end();
         transIt++)
   {
      if (transIt->first.m_aovShader)
         result.append(transIt->first.name);
   }
}

void CExtensionsManager::AddCustomShape(const MString &shape)
{
   std::string shapeStr(shape.asChar());
   s_customShapes.insert(shapeStr);
}
void CExtensionsManager::GetCustomShapes(MStringArray& result)
{
   for (CustomShapesMap::const_iterator it = s_customShapes.begin(); it != s_customShapes.end(); ++it)
   {
      MString shapeName((*it).c_str());
      result.append(shapeName);
   }
}
void CExtensionsManager::AddOperator(const MString &op)
{
   std::string opStr(op.asChar());
   s_operators.insert(opStr);
}
void CExtensionsManager::GetOperators(MStringArray& result)
{
   for (OperatorsMap::const_iterator it = s_operators.begin(); it != s_operators.end(); ++it)
   {
      MString opName((*it).c_str());
      result.append(opName);
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

   ExtensionsList::iterator extIt;
   
   if (plugin_str == "mtoa")
   {
      // we're first called when MtoA finished loading.
      // Let's loop over the the extensions, and keep the deferred ones
      // in a list
      for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
      {         
         if (!extIt->IsDeferred())
            continue;

         const unordered_set<std::string> &extensionPlugins = extIt->m_impl->m_requiredMayaPlugins;
         for (unordered_set<std::string>::const_iterator plIt = extensionPlugins.begin(); plIt != extensionPlugins.end(); ++plIt)
         {
            s_deferredExtensions.insert(*plIt);
         }
         
      }
      return;
   }

   if (s_deferredExtensions.empty())
      return;

   // a new plugin has been loaded, we must check if it is in the list 
   // of deferred extensions
   if (s_deferredExtensions.find(plugin_str) == s_deferredExtensions.end())
      return;

   // the plugin that was just loaded is in our list, we need to loop over all our 
   // extensions and see which one needs to be registered

   bool universeCreated = false;
   for (extIt = s_extensions.begin();
         extIt != s_extensions.end();
         extIt++)
   {
      if (extIt->IsDeferred()
            && extIt->m_impl->m_requiredMayaPlugins.find(plugin_str)
            != extIt->m_impl->m_requiredMayaPlugins.end())
      {
         if (!universeCreated)
            universeCreated = ArnoldUniverseBegin();

         RegisterExtension(&(*extIt));
      }
   }
   if (universeCreated) ArnoldUniverseEnd();

   // remove this plugin from our list now that it was registered
   s_deferredExtensions.erase(plugin_str);
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
      // #2946 hiding excessive messages when extensions are loaded multiple times
//      AiMsgInfo("[mtoa] Extension already managed: %s", extensionFile.asChar());
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
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] Cannot register already existing Maya node "+mayaNode.name+", provided by "+mayaNode.provider+"("+mayaNode.file+").");
            
      return MStatus::kFailure;
   }
   // Construct the abstract to store in the MPx class's static s_abstract member,
   // if a pointer to one was provided in the proxy for that Maya node class.
   CAbMayaNode abstract(mayaNode.name, mayaNode.arnold, mayaNode.classification, mayaNode.provider);

   if (NULL != mayaNode.abstract) *mayaNode.abstract = abstract;
   const MString *classificationPtr = (mayaNode.classification == "") ? NULL : &mayaNode.classification;

   // FIXME find a better way to do this (add flag in mayaNode ?)
   if (mayaNode.creator == CArnoldProceduralNode::creator)
   {
      status = MFnPlugin(s_plugin, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION).registerShape(mayaNode.name, mayaNode.id,
            mayaNode.creator, mayaNode.initialize, CArnoldProceduralNodeUI::creator,
            classificationPtr );
      
   } else
   {
      status = MFnPlugin(s_plugin, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION).registerNode(
            mayaNode.name, mayaNode.id,
            mayaNode.creator, mayaNode.initialize,
            mayaNode.type, classificationPtr );
   }
   CHECK_MSTATUS(status);
   if (MStatus::kSuccess == status)
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] Registered Maya node "+mayaNode.name+" provided by "+mayaNode.provider +"("+mayaNode.file+").");
            
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
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.ext]  Successfully deregistered node "+mayaNode.name+" provided by "+mayaNode.provider+"("+mayaNode.file+").");
            
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
   TranslatorsSet *allTranslators = FindRegisteredTranslators(mayaNode);
   if (NULL == allTranslators || allTranslators->empty()) return NULL;

   const CPxTranslator* result = NULL;
   if (translator.name == "")
   {
      // No specific translator requested, return last
      // TODO : actually check s_extensions to use the last loaded translator?
      TranslatorsSet::iterator it = allTranslators->find(GetDefaultTranslator(mayaNode.name));
      if (it == allTranslators->end())
      {
         // need to get the last element. This code is ugly, but is there
         // a better way to get the last element of an unordered_set ?
         int setSize = (allTranslators->size() - 1);
         it = allTranslators->begin();
         for (int j = 0; j < setSize; ++j)
            it++;
         
      }

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


/** Register this attribute extension for a given maya class
 if the extension attribute was properly registered we want to link it to the MtoA plugin.
 However we must only do it for "parent" attributes as per maya guidelines, which is why we're testing parent().isNull().
 Unfortunately, maya is returning success even if this attribute was already added to the MNodeClass, and 
 linking the same attribute twice ends up crashing maya. So we're storing here a map of all class + attributes
 previously stored, in order to avoid doing it twice.
 Another situation we need to consider, is about classes inheriting from other class that already linked the same attribute.
 The only case in MtoA where this happens is with the stereoRigCamera, where the parameters are already loaded for 
 the base "camera" class. So we do an exception for that class
 **/
MStatus CExtensionsManager::RegisterExtensionAttribute(const MNodeClass &nodeClass, const MObject &attrib)
{
   MString nodeType = nodeClass.typeName();
   std::string nodeTypeStr(nodeType.asChar());
   // all parameters w're adding for "stereoRigCamera" have already been added for "camera"
   if (nodeType == "stereoRigCamera")
      return MS::kSuccess;
   
   s_mayaExtensionClasses.insert(nodeTypeStr); // make sure this class is listed

   MFnAttribute fnAttr(attrib);
   fnAttr.addToCategory("arnold");
   MString attrName = fnAttr.name();

   // key for class + attr
   std::string registerAttr(nodeTypeStr);
   registerAttr += ".";
   registerAttr += attrName.asChar();

   // This attribute was already added, we can leave
   if (s_extensionAttributes.find(registerAttr) != s_extensionAttributes.end())
      return MS::kSuccess;
   
   MDGModifier dgMod;
   MStatus status = dgMod.addExtensionAttribute(nodeClass, attrib);
   
   if (status == MStatus::kSuccess && !s_plugin.isNull() && fnAttr.parent().isNull())
      status = dgMod.linkExtensionAttributeToPlugin(s_plugin, attrib);   

   if (status == MStatus::kSuccess)
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.attr] Added extension attribute "+nodeType+"."+ attrName);

      status = dgMod.doIt();
      s_extensionAttributes.insert(registerAttr);

   } else
      AiMsgError("[mtoa.attr] Unable to create extension attribute %s.%s", nodeType.asChar(), attrName.asChar());
   
   return status;
}

MStatus CExtensionsManager::UnregisterExtensionAttributes(const MObject &plugin)
{
   unordered_set<std::string>::iterator classIt = s_mayaExtensionClasses.begin();
   for (; classIt != s_mayaExtensionClasses.end(); ++classIt)
   {
      MString className = (*classIt).c_str();
      MNodeClass nodeClass(className);
      MObjectArray attributes;
      nodeClass.getAttributes(attributes);
      for (unsigned int i = 0; i < attributes.length(); ++i)
      {
         MFnAttribute fnAttr(attributes[i]);
         std::string registerAttr(className.asChar());
         registerAttr += ".";
         registerAttr += fnAttr.name().asChar();
         if (s_extensionAttributes.find(registerAttr) == s_extensionAttributes.end())
            continue; // this attribute wasn't registered by MtoA, it's a maya native attribute

         MDGModifier dgMod;
         // First unlink the extension attribute
         dgMod.unlinkExtensionAttributeFromPlugin(plugin, attributes[i]);
         // Then remove the extension attribute from the nodeClass
         dgMod.removeExtensionAttribute(nodeClass, attributes[i]);
         dgMod.doIt();
      }
   }
   return MS::kSuccess; // when should I return failure ?
}