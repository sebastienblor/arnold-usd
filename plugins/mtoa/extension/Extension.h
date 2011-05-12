#ifndef EXTENSION_H
#define EXTENSION_H

// TODO : mode/Translator.h"
#include "extension/PathUtils.h"
#include "extension/PxUtils.h"
#include "common/DynLibrary.h"
#include "scene/NodeTranslator.h"

#include <maya/MTypeId.h>
#include <maya/MPxNode.h>

#include <ai_nodes.h>
#include <ai.h>

#define BUILTIN "<built-in>"


// class CExtension;

// typedef void (*extensionInitFunction)(CExtension&);


// struct CMayaPluginData
// {
//    MString mayaNode;
//  MString translatorName;
//   NodeClassInitFunction nodeClassInitializer;
// };

// plugin name to list of provided nodes needing callbacks
// typedef std::map<std::string, std::vector<CMayaPluginData> > PluginDataMap;

// To track loaded Arnold plugins
typedef std::set<std::string> LoadedArnoldPluginsSet;


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
   MString GetName() const {return m_extensionName;}
   MString GetFile() const {return m_extensionFile;}
   unsigned int RegisteredNodesCount() const {return m_registeredMayaNodes.size();}
   unsigned int TranslatedNodesCount() const {return m_registeredTranslators.size();}
   unsigned int TranslatorCount() const;
   bool IsRegistered() const {return m_registered;}

   // Arnold Plugin loading
   MString LoadArnoldPlugin(const MString &file,
                            const MString &path=PLUGIN_SEARCH,
                            MStatus *returnStatus=NULL);
   MStringArray GetOwnLoadedArnoldPlugins();

   // Register Maya nodes for all Arnold nodes declared with
   // the given plugin, using metadata info
   MStatus RegisterAllNodes(const MString &plugin="");
   // Register Maya nodes for all Arnold nodes declared with
   // the given plugin, using Arnold node type and metadata info
   MStatus RegisterAllTranslators(const MString &plugin="");

   // Register the Maya node for a givem Arnold node,
   // using the node metadata
   // Will use ArnoldDefaultNode or ArnoldDefaultDag
   // if no MCreatorFunction and MInitializeFunction
   // are provided
   MStatus RegisterNode(const MString &arnoldNodeName,
                        const MString &mayaTypeName = "",
                        const MTypeId &mayaTypeId = MTypeId(0),
                        MCreatorFunction creatorFunction=NULL,
                        MInitializeFunction initFunction=NULL,
                        CAbMayaNode *abstractMember=NULL,
                        MPxNode::Type type=MPxNode::kLast,
                        const MString &classification="");
   MStatus RegisterNode(const AtNodeEntry* arnoldNodeEntry,
                        const MString &mayaTypeName = "",
                        const MTypeId &mayaTypeId = MTypeId(0),
                        MCreatorFunction creatorFunction=NULL,
                        MInitializeFunction initFunction=NULL,
                        CAbMayaNode *abstractMember=NULL,
                        MPxNode::Type type=MPxNode::kLast,
                        const MString &classification="");
   // Can be called directly to register Maya nodes that
   // do not correspond to Arnold nodes
   // TODO : RegisterTransform, RegisterShape ?
   MStatus RegisterNode(const MString &mayaTypeName,
                        const MTypeId &mayaTypeId,
                        MCreatorFunction creatorFunction,
                        MInitializeFunction initFunction,
                        MPxNode::Type type=MPxNode::kDependNode,
                        const MString &classification="");
   // TODO : deferred MStatus RegisterNode(const MString &typeName, const MTypeId &typeId, const MString &pluginProvider);

   // To register a translator for a given Maya node
   // gives no access to metadata (all info needs to be set explicitely)
   MStatus RegisterTranslator(const MString &mayaTypeName,
                              const MTypeId &mayaTypeId,
                              const MString &translatorName,
                              TCreatorFunction creatorFunction,
                              TNodeInitFunction nodeInitFunction=NULL);
   // To register a translator from the metadata associated with the Arnold node name or entry
   MStatus RegisterTranslator(const MString &arnoldNodeName,
                              const MString &translatorName="",
                              TCreatorFunction creatorFunction=NULL,
                              TNodeInitFunction nodeInitFunction=NULL);
   MStatus RegisterTranslator(const AtNodeEntry* arnoldNodeEntry,
                              const MString &translatorName="",
                              TCreatorFunction creatorFunction=NULL,
                              TNodeInitFunction nodeInitFunction=NULL);
   // To manually register a translator for a given Arnold node and Maya type name,
   // since it is used to add an addtionnal translator to a Maya node you need to
   // provide a translator name and a creator method.
   MStatus RegisterTranslator(const MString &arnoldNodeName,
                              const MString &mayaTypeName,
                              const MString &translatorName,
                              TCreatorFunction creatorFunction,
                              TNodeInitFunction nodeInitFunction=NULL);


protected :
   MStatus setFile(const MString &file);

   MStatus NewArnoldPlugin(const MString &file);
   MStatus DeleteArnoldPlugin(const MString &file);
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

   // TODO : reimplement this?
   /*
   void InitializePendingTranslators(MString& pluginName);
   void MayaPluginLoadedCallback(const MStringArray &strs, void *clientData);
   void CreateCallbacks();
   void RemoveCallbacks();
   */

protected:
   MString m_extensionFile;
   MString m_extensionName;
   bool m_registered;
   // LoadedArnoldPluginsSet m_ownArnoldPlugins;
   // only the new maya nodes registered by this Extension
   MayaNodesSet m_registeredMayaNodes;
   // new maya nodes as well as existing associated to arnold nodes
   // FIXME can probably remove it
   ArnoldNodeToMayaNodeMap m_arnoldToMayaNodes;
   // translators registered by this extension, indexed by Maya node
   MayaNodeToTranslatorsMap m_registeredTranslators;
   // Arnold Plugins loaded by this extension
   LoadedArnoldPluginsSet m_ownLoadedArnoldPlugins;

   // PluginDataMap m_mayaPluginData;
   static unsigned int s_autoNodeId;
   static MCallbackId s_pluginLoadedCallbackId;

   static LoadedArnoldPluginsSet s_allLoadedArnoldPlugins;
};

#endif // EXTENSION_H
