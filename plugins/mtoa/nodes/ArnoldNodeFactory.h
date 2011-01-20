//-------------------------------------------------------------------------- 
// ArnoldNodeFactory class definition.
//-------------------------------------------------------------------------- 
#ifndef ARNOLD_PLUGIN_FACTORY_H
#define ARNOLD_PLUGIN_FACTORY_H
#define MNoVersionString
#define MNoPluginEntry

#include "nodes/ArnoldNodeIds.h"
#include "maya_scene/NodeTranslator.h"
#include "maya_scene/MayaScene.h"
#include "nodes/ArnoldNodeHelper.h"
#include "common/DynLibrary.h"

#include <ai_nodes.h>

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include <maya/MDGMessage.h>

#include <string>
#include <map>
#include <vector>


struct CMayaNodeData
{
   std::string arnoldNodeName;
   int nodeId;
   MCallbackId callbackId;
};

// key: maya node name
typedef std::map<std::string, CMayaNodeData> MayaNodeDataMap;
// key: arnold node name
typedef std::map<std::string, std::string> ArnoldNodeToMayaNode;
// key: arnold plugin file path
typedef std::map<std::string, std::vector<std::string> > ArnoldPluginData;
// key: maya node name
typedef std::map<std::string, std::vector<CAttrData> > DynamicAttrMap;


int FindLibraries(MString searchPath, MStringArray &files);

class DLLEXPORT CExtension
{
public:
   void RegisterTranslator(int typeId, CreatorFunction creator);
   void RegisterDagTranslator(int typeId, CreatorFunction creator);
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

   void RegisterMayaNode(AtNodeEntry* arnoldNode);
   static void MapToMayaNode(const char* arnoldNodeName, const char* mayaCounterpart, int typeId);
   void UnregisterMayaNode(const char* arnoldNodeName);
   void RegisterAllNodes();
   void UnregisterAllNodes();

   bool LoadExtension(const char* extensionFile);
   void LoadExtensions();

   int getNumPlugins();

   static void RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   static void RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator);

   static void NodeCreatedCallback(MObject &node, void *clientData);
   static void AddDynamicAttr(const char* mayaNode, CAttrData data);
public:
   static MayaNodeDataMap s_factoryNodes;

private:
   bool  m_loadOk;
   MFnPlugin m_plugin;
   static int s_autoNodeId;
   static ArnoldNodeToMayaNode s_arnoldToMayaNodes;
   static ArnoldPluginData s_arnoldPlugins;
   static DynamicAttrMap s_dynamicAttributes;
};

//-------------------------------------------------------------------------- 
inline CArnoldNodeFactory::CArnoldNodeFactory(MObject plugin)
   :  m_loadOk(false)
   ,  m_plugin(plugin)
{
}

//-------------------------------------------------------------------------- 
inline CArnoldNodeFactory::~CArnoldNodeFactory()
{
}

#endif  // ARNOLD_PLUGIN_FACTORY_H
