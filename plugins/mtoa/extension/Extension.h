#ifndef EXTENSION_H
#define EXTENSION_H

// TODO : mode/Translator.h"
#include "extension/PxUtils.h"

#include "common/DynLibrary.h"
#include "scene/NodeTranslator.h"

#include <maya/MTypeId.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxNode.h>

#include <ai_nodes.h>
#include <ai.h>


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

class DLLEXPORT CExtension
{
   friend class CExtensionsManager;

public:
   CExtension(const MString &extensionFile);
   virtual ~CExtension() {}
   MString GetName() const {return m_extensionName;}
   MString GetFile() const {return m_extensionFile;}
   bool IsLoaded() const {return m_loaded;}
   bool IsRegistered() const {return m_registered;}

   // Arnold Plugin loading
   MStatus LoadArnoldPlugin(const MString &path);
   MStringArray GetOwnLoadedArnoldPlugins();

   // Register Maya nodes for all Arnold nodes declared with
   // the given plugin, using metadata info
   MStatus RegisterNodes(const MString &path);
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
                        MPxNode::Type type=MPxNode::kLast,
                        const MString *classification=NULL);
   MStatus RegisterNode(const AtNodeEntry* arnoldNodeEntry,
                        const MString &mayaTypeName = "",
                        const MTypeId &mayaTypeId = MTypeId(0),
                        MCreatorFunction creatorFunction=NULL,
                        MInitializeFunction initFunction=NULL,
                        MPxNode::Type type=MPxNode::kLast,
                        const MString *classification=NULL);
   // Can be called directly to register Maya nodes that
   // do not correspond to Arnold nodes
   // TODO : RegisterTransform, RegisterShape ?
   MStatus RegisterNode(const MString &mayaTypeName,
                        const MTypeId &mayaTypeId,
                        MCreatorFunction creatorFunction,
                        MInitializeFunction initFunction,
                        MPxNode::Type type=MPxNode::kDependNode,
                        const MString *classification=NULL);
   // TODO : deferred MStatus RegisterNode(const MString &typeName, const MTypeId &typeId, const MString &pluginProvider);

   // Old signature for compatibility
   MStatus RegisterTranslator(const MString &mayaTypeName,
                              const MTypeId &mayaTypeId,
                              const MString translatorName,
                              TCreatorFunction creatorFunction,
                              TNodeInitFunction nodeInitFunction=NULL);
   // Register a translator for a given Arnold node and Maya node type
   MStatus RegisterTranslator(const MString &arnoldNodeName,
                              const MString &mayaTypeName="",
                              const MString translatorName="",
                              TCreatorFunction creatorFunction=NULL,
                              TNodeInitFunction nodeInitFunction=NULL);
   // Register a translator for a given Arnold node entry and Maya node type
   MStatus RegisterTranslator(const AtNodeEntry* arnoldNodeEntry,
                              const MString &mayaTypeName="",
                              const MString translatorName="",
                              TCreatorFunction creatorFunction=NULL,
                              TNodeInitFunction nodeInitFunction=NULL);

protected :
   MStatus DoRegisterArnoldPlugin(const MString &path);
   MStatus DoRegisterMayaNode(const CPxMayaNode &mayaNode,
                              const CPxArnoldNode &arnoldNode=CPxArnoldNode());
   MStatus DoRegisterTranslator(const CPxTranslator &translator,
                                const CPxMayaNode &mayaNode);

   const CPxMayaNode* FindRegisteredMayaNode(const CPxMayaNode &mayaNode);
   const CPxMayaNode* FindAssociatedMayaNode(const CPxArnoldNode &arnoldNode);
   const TranslatorsSet* FindRegisteredTranslators(const CPxMayaNode &mayaNode);

   static bool IsArnoldPluginLoaded(const MString &path);
   static MStringArray GetAllLoadedArnoldPlugins();

   void InitializePendingTranslators(MString& pluginName);
   void MayaPluginLoadedCallback(const MStringArray &strs, void *clientData);
   void CreateCallbacks();
   void RemoveCallbacks();

protected:
   MString m_extensionFile;
   MString m_extensionName;
   bool m_loaded;
   bool m_registered;
   // LoadedArnoldPluginsSet m_ownArnoldPlugins;
   // only the new maya nodes registered by this Extension
   MayaNodesSet m_registeredMayaNodes;
   // new maya nodes as well as existing associated to arnold nodes
   ArnoldNodeToMayaNodeMap m_arnoldToMayaNodes;
   // translators registered by this extension, indexed by Maya node
   MayaNodeToTranslatorsMap m_registeredTranslators;
   // Arnold Plugins loaded by this extension
   LoadedArnoldPluginsSet m_ownLoadedArnoldPlugins;

   // PluginDataMap m_mayaPluginData;
   static MString s_swatchRenderer;
   static unsigned int s_autoNodeId;
   static MCallbackId s_pluginLoadedCallbackId;

   static LoadedArnoldPluginsSet s_allLoadedArnoldPlugins;
};

#endif // EXTENSION_H
