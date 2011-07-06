#ifndef EXTENSION_H
#define EXTENSION_H

#include "extension/PathUtils.h"
#include "extension/PxUtils.h"
#include "common/DynLibrary.h"
#include "translators/NodeTranslator.h"

#include <maya/MTypeId.h>
#include <maya/MPxNode.h>

#include <ai_nodes.h>
#include <ai.h>

#define BUILTIN "<built-in>"

class CAbMayaNode;

// To track loaded Arnold plugins
typedef std::set<std::string> LoadedArnoldPluginsSet;
// To track required  Maya plugins
typedef std::set<std::string> RequiredMayaPluginsSet;

// class CExtension

/// Class to represent and manipulate Arnold extensions.
///
/// This class is used in the initializeExtensions and deinitializeExtension functions
/// of a MtoA extension to respectively register and deregister the extension services
/// (nodes, translators) with MtoA and Maya.RT CExtension
///
class DLLEXPORT CExtension
{
   friend class CExtensionsManager;

public:
   CExtension(const MString &file);
   virtual ~CExtension() {}
   void Requires(const MString &plugin);
   MStringArray Required();
   MString GetName() const {return m_extensionName;}
   MString GetFile() const {return m_extensionFile;}
   unsigned int RegisteredNodesCount() const {return m_registeredMayaNodes.size();}
   unsigned int TranslatedNodesCount() const {return m_registeredTranslators.size();}
   unsigned int TranslatorCount() const;
   bool IsRegistered() const {return m_registered;}
   bool IsDeferred() const {return m_deferred;}

   // Arnold Plugin loading
   MString LoadArnoldPlugin(const MString &file,
                            const MString &path=PLUGIN_SEARCH,
                            MStatus *returnStatus=NULL);
   // Get list of Arnold plugins this extension loads
   MStringArray GetOwnLoadedArnoldPlugins();

   // Can be called directly to register new Maya nodes
   MStatus RegisterNode(const MString &mayaTypeName,
                        const MTypeId &mayaTypeId,
                        MCreatorFunction creatorFunction,
                        MInitializeFunction initFunction,
                        MPxNode::Type type=MPxNode::kDependNode,
                        const MString &classification="");

   // To register a translator for a given Maya node
   // gives no access to metadata (all info needs to be set explicitely)
   MStatus RegisterTranslator(const MString &mayaTypeName,
                              const MString &translatorName,
                              TCreatorFunction creatorFunction,
                              TNodeInitFunction nodeInitFunction=NULL);

   MStatus RegisterAOV(const MString &nodeType,
                       const MString &aovName,
                       AtInt dataType,
                       const MString &aovAttr);

protected :
   MStatus setFile(const MString &file);

   MStatus UnloadArnoldPlugins();
   MStatus UnloadArnoldPlugin(const MString &resolved);
   MStatus NewArnoldPlugin(const MString &file);
   MStatus DeleteArnoldPlugin(const MString &file);

   // Register Maya nodes for all Arnold nodes declared with
   // the given plugin, using metadata info
   MStatus RegisterPluginNodesAndTranslators(const MString &plugin="");

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

   static bool IsArnoldPluginLoaded(const MString &path);
   static MStringArray GetAllLoadedArnoldPlugins();

protected:
   MString m_extensionFile;
   MString m_extensionName;
   void* m_library;
   bool m_registered;
   bool m_deferred;
   // LoadedArnoldPluginsSet m_ownArnoldPlugins;
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

   // map from aov name to data type
   std::map<std::string, AtInt> m_aovTypes;
   // map from node type to a vector of aov data
   std::map<std::string, std::vector<CAOVData> > m_aovAttrs;

   // Static info for all extensions
   static unsigned int s_autoNodeId;
   static LoadedArnoldPluginsSet s_allLoadedArnoldPlugins;
};

#endif // EXTENSION_H
