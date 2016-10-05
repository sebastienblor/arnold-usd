#pragma once

#include "extension/PxUtils.h"
#include "translators/NodeTranslator.h"
#include "translators/DagTranslator.h"
#include "utils/Version.h"

#include <maya/MTypeId.h>
#include <maya/MPxNode.h>

#include <algorithm>

#define BUILTIN "<built-in>"



// To track loaded Arnold plugins
class PluginStore: public std::vector<std::string>
{
public:
   iterator find(const std::string& item)
   {
      return std::find(begin(), end(), item);
   }

   void insert(const std::string& item)
   {
      push_back(item);
   }
   void erase(const std::string& item)
   {
      iterator it = find(item);
      std::vector<std::string>::erase(it);
   }
};


typedef PluginStore LoadedArnoldPluginsSet;
// To track required  Maya plugins
typedef AmSet<std::string> RequiredMayaPluginsSet;

// class CExtension

/// Class to represent and manipulate Arnold extensions.
///
/// This class is used in the initializeExtensions and deinitializeExtension functions
/// of a MtoA extension to respectively register and deregister the extension services
/// (nodes, translators) with MtoA and Maya.RT CExtension
///
class CExtensionImpl
{
public:
   MStatus setFile(const MString &file);

   MStatus UnloadArnoldPlugins();
   MStatus UnloadArnoldPlugin(const MString &resolved);
   MStatus DoUnloadArnoldPlugin(const MString &resolved);
   MStatus NewArnoldPlugin(const MString &file);
   MStatus DeleteArnoldPlugin(const MString &file);

   // Register the Maya node for a givem Arnold node, using the node metadata
   MStatus RegisterNode(CPxMayaNode &mayaNode,
                        const CPxArnoldNode &arnoldNode);

   // Register a translator from the metadata associated with the Arnold node name or entry
   MStatus RegisterTranslator(const CPxTranslator &translator,
                              CPxMayaNode &mayaNode,
                              const CPxArnoldNode &arnoldNode);

   MStatus NewMappedMayaNode(CPxMayaNode mayaNode,
                             const CPxArnoldNode &arnoldNode);
   MStatus NewMayaNode(const CPxMayaNode &mayaNode);
   MStatus MapMayaNode(const CPxMayaNode &mayaNode,
                       const CPxArnoldNode &arnoldNode);
   MStatus NewTranslator(const CPxTranslator &translator,
                         const CPxMayaNode &mayaNode);

   const CPxMayaNode* FindRegisteredMayaNode(const CPxMayaNode &mayaNode);
   const TranslatorsSet* FindRegisteredTranslators(const CPxMayaNode &mayaNode);

   static MString FindFileInPath(const MString &file,
                                 const MString &path,
                                 MStatus *returnStatus=NULL);
   static MStringArray FindLibraries(const MString &path,
                                     MStatus *returnStatus=NULL);


   MString m_extensionFile;
   MString m_extensionName;
   MString m_apiVersion;
   void* m_library;
   bool m_registered;
   bool m_deferred;
   // LoadedArnoldPluginsSet ownArnoldPlugins;
   // only the new maya nodes registered by this Extension
   MayaNodesSet m_registeredMayaNodes;
   // new maya nodes as well as existing associated to arnold nodes
   // FIXME can probably remove it
   ArnoldNodeToMayaNodeMap m_arnoldToMayaNodes;
   // translators registered by this extension, indexed by Maya node
   MayaNodeToTranslatorsMap m_registeredTranslators;
   // Arnold plugins loaded by this extension
   LoadedArnoldPluginsSet m_ownLoadedArnoldPlugins;
   // Maya plugins required by this extension
   RequiredMayaPluginsSet m_requiredMayaPlugins;

   // Static info for all extensions
   static unsigned int s_autoNodeId;
   static LoadedArnoldPluginsSet s_allLoadedArnoldPlugins;
};
