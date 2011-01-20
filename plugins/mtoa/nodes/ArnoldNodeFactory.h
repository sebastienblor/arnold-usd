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

#include <ai_nodes.h>

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>

#include <string>
#include <map>
#include <vector>

struct CMayaNodeData
{
   std::string mayaNodeName;
   int nodeId;
};

typedef std::map<std::string, std::string> MayaNodeToArnoldNode;
typedef std::map<std::string, CMayaNodeData> ArnoldNodeToMayaData;
typedef std::map<std::string, std::vector<std::string> > ArnoldPluginData;



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
   void MapToMayaNode(const char* arnoldNodeName, const char* mayaCounterpart, int nodeId);
   void UnregisterMayaNode(const char* arnoldNodeName);
   void RegisterAllNodes();
   void UnregisterAllNodes();
   int getNumPlugins();

   static void RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   static void RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator);

public:
   static MayaNodeToArnoldNode s_factoryNodes;

private:
   bool  m_loadOk;
   MFnPlugin m_plugin;
   static int s_autoNodeId;
   static ArnoldNodeToMayaData s_arnoldToMayaNodes;
   static ArnoldPluginData s_arnoldPlugins;
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
