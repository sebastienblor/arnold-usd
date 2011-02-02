#ifndef ARNOLDNODEFACTORY_H
#define ARNOLDNODEFACTORY_H

#define MNoVersionString
#define MNoPluginEntry

#include "nodes/ArnoldNodeIDs.h"
#include "scene/NodeTranslator.h"
#include "scene/MayaScene.h"
#include "nodes/ArnoldNodeHelper.h"
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
};

struct CMayaPluginData
{
   std::string mayaNode;
   NodeInitFunction nodeInitializer;
};

// key: maya node name
typedef std::map<std::string, CMayaNodeData> MayaNodeDataMap;
// key: arnold node name
typedef std::map<std::string, std::string> ArnoldNodeToMayaNode;
// key: arnold plugin file path
typedef std::map<std::string, std::vector<std::string> > ArnoldPluginData;
// key: maya node name
typedef std::map<std::string, std::vector<CAttrData> > DynamicAttrMap;
// plugin name to list of provided nodes needing callbacks
typedef std::map<std::string, std::vector<CMayaPluginData> > PluginDataMap;

int FindLibraries(MString searchPath, MStringArray &files);

class DLLEXPORT CExtension
{
public:
   void RegisterDependTranslator(const char* arnoldNodeName, int typeId, CreatorFunction creator);
   void RegisterDagTranslator(const char* arnoldNodeName, int typeId, CreatorFunction creator);
   void RegisterDependTranslator(const char* arnoldNodeName, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin="");
   void RegisterDagTranslator(const char* arnoldNodeName, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin="");
};

typedef void (*pluginInitFunctionType)(CExtension&);

class CArnoldNodeFactory
{
public:
   CArnoldNodeFactory(MObject plugin);
   ~CArnoldNodeFactory();

   void LoadPlugin(const char* pluginFile);
   void UnloadPlugin(const char* pluginFile);
   void LoadPlugins();
   void UnloadPlugins();

   bool RegisterMayaNode(const AtNodeEntry* arnoldNode);
   bool RegisterMayaNode(const char* arnoldNodeName, const char* mayaNodeName, int nodeId, const char* shaderClass="");
   static bool MapToMayaNode(const char* arnoldNodeName, const char* mayaCounterpart, int typeId);
   void UnregisterMayaNode(const char* arnoldNodeName);
   void RegisterAllNodes();
   void UnregisterAllNodes();

   bool LoadExtension(const char* extensionFile);
   void LoadExtensions();
   void UnloadExtensions();

   int getNumPlugins();

   static bool RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin="");
   static bool RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin="");
   static bool RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   static bool RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator);

   static void NodeCreatedCallback(MObject &node, void *clientData);
   static void MayaPluginLoadedCallback(const MStringArray &strs, void *clientData);
public:
   static MayaNodeDataMap s_factoryNodes;

private:
   static bool RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin);
   static bool RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator);

private:
   bool  m_loadOk;
   MFnPlugin m_plugin;
   static int s_autoNodeId;
   static ArnoldNodeToMayaNode s_arnoldToMayaNodes;
   static ArnoldPluginData s_arnoldPlugins;
   static PluginDataMap s_mayaPluginData;
   static MCallbackId s_pluginLoadedCallbackId;
   void *m_pluginHandle;
};

//-------------------------------------------------------------------------- 
inline CArnoldNodeFactory::CArnoldNodeFactory(MObject plugin)
   :  m_loadOk(false)
   ,  m_plugin(plugin)
   ,  m_pluginHandle(NULL)
{
}

//-------------------------------------------------------------------------- 
inline CArnoldNodeFactory::~CArnoldNodeFactory()
{
}

#endif  // ARNOLDNODEFACTORY_H
