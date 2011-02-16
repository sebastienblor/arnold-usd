#ifndef ARNOLDNODEFACTORY_H
#define ARNOLDNODEFACTORY_H

#define MNoVersionString
#define MNoPluginEntry

#include "nodes/ArnoldNodeIDs.h"
#include "nodes/MayaNodeIDs.h"
#include "scene/NodeTranslator.h"
#include "scene/TranslatorRegistry.h"
#include "scene/MayaScene.h"
#include "utils/AttrHelper.h"
#include "common/DynLibrary.h"

#include <ai_nodes.h>
#include <ai.h>

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include <maya/MDGMessage.h>
#include <maya/MMessage.h>

#include <string>
#include <map>
#include <vector>


struct CMayaNodeData
{
   std::string arnoldNodeName;
   int nodeId;
   MCallbackId callbackId;
   CMayaNodeData() :  arnoldNodeName(""),
                      nodeId(0),
                      callbackId(0)
   {}
};

/// key: maya node name
typedef std::map<std::string, CMayaNodeData> MayaNodeDataMap;
/// key: arnold node name
typedef std::map<std::string, std::string> ArnoldNodeToMayaNode;
/// key: arnold plugin file path
typedef std::map<std::string, std::vector<std::string> > ArnoldPluginData;
/// key: maya node name
typedef std::map<std::string, std::vector<CAttrData> > DynamicAttrMap;

int FindLibraries(MString searchPath, MStringArray &files);

class DLLEXPORT CExtension
{
public:
   void RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   void RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   void RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
   void RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
};

typedef void (*pluginInitFunctionType)(CExtension&);

//--------------- CArnoldNodeFactory -----------------------------------

/// Responsible for loading Arnold and MtoA plugins and creating new Maya nodes

/// When MtoA is loaded in Maya, the node factory is called to load all Arnold plugins found on ARNOLD_PLUGIN_PATH,
/// and generate Maya nodes from the Arnold shaders contained within them. Metadata can be added to
/// the Arnold node and its parameters to control how the Maya node and its attributes are generated, if at all.

class CArnoldNodeFactory
{
public:
   CArnoldNodeFactory(MObject plugin);
   ~CArnoldNodeFactory();

   void LoadPlugin(const char* pluginFile);
   void UnloadPlugin(const char* pluginFile);
   void LoadPlugins();
   void UnloadPlugins();

   bool RegisterMayaNode(const AtNodeEntry* arnoldNodeEntry);
   bool RegisterMayaNode(const char* arnoldNodeName, const char* mayaNodeName, int nodeId, const char* shaderClass="");
   static bool MapToMayaNode(const char* arnoldNodeName, const char* mayaCounterpart, int typeId);
   void UnregisterMayaNode(const char* arnoldNodeName);
   void RegisterAllNodes();
   void UnregisterAllNodes();

   bool LoadExtension(const char* extensionFile);
   void LoadExtensions();
   void UnloadExtensions();

   static const char* GetArnoldNodeFromMayaNode(const MString& mayaShader);

private:
   static MayaNodeDataMap s_factoryNodes;
   MFnPlugin m_plugin;
   static int s_autoNodeId;
   static ArnoldNodeToMayaNode s_arnoldToMayaNodes;
   static ArnoldPluginData s_arnoldPlugins;
   static MCallbackId s_pluginLoadedCallbackId;
   void *m_pluginHandle;
};

inline CArnoldNodeFactory::CArnoldNodeFactory(MObject plugin)
   :  m_plugin(plugin)
   ,  m_pluginHandle(NULL)
{
}

inline CArnoldNodeFactory::~CArnoldNodeFactory()
{
}

//--------------- CAutoTranslator ------------------------------------------

/// A Translator class which can automatically export simple Maya nodes.

/// To perform an automatic export, the translator does the following:
///  -# gets the Arnold node entry that corresponds to the Maya node being export from CArnoldNodeFactory
///  -# loops through each parameter on the Arnold node entry
///  -# processes the equivalent attribute on the Maya node
///

class DLLEXPORT CAutoTranslator : public CNodeTranslator
{
public:
   CAutoTranslator() :
      CNodeTranslator()
   {}
   virtual void Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      // downconvert to MObject
      CNodeTranslator::Init(dagPath.node(), scene, outputAttr);
   }
   static void* creator()
   {
      return new CAutoTranslator();
   }
   const char* GetArnoldNodeType();
   void Export(AtNode* atNode);
};
#endif  // ARNOLDNODEFACTORY_H
