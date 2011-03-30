#ifndef ARNOLDNODEFACTORY_H
#define ARNOLDNODEFACTORY_H

#define MNoVersionString
#define MNoPluginEntry

#include "nodes/ArnoldNodeIDs.h"
#include "nodes/MayaNodeIDs.h"
#include "scene/NodeTranslator.h"
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
   void RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   void RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   void RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin="");
   void RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin="");
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

public:
   static MayaNodeDataMap s_factoryNodes;

private:
   bool  m_loadOk;
   MFnPlugin m_plugin;
   static int s_autoNodeId;
   static ArnoldNodeToMayaNode s_arnoldToMayaNodes;
   static ArnoldPluginData s_arnoldPlugins;
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

// AutoTranslator
//
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
   AtNode* Export();
   void Update(AtNode* atNode);
private:
   const AtNodeEntry* m_nodeEntry;
};
#endif  // ARNOLDNODEFACTORY_H
