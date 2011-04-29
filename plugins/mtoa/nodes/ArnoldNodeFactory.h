#ifndef ARNOLDNODEFACTORY_H
#define ARNOLDNODEFACTORY_H

#define MNoVersionString
#define MNoPluginEntry

#include "ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "nodes/MayaNodeIDs.h"
#include "scene/NodeTranslator.h"
#include "scene/Extension.h"
#include "scene/MayaScene.h"
#include "utils/AttrHelper.h"
#include "scene/Extension.h"

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
   CMayaNodeData() :  arnoldNodeName(""),
                      nodeId(0)
   {}
};

/// key: maya node name. value: arnold node name and maya node id
typedef std::map<std::string, CMayaNodeData> MayaNodeDataMap;
/// key: arnold node name. value: maya node name
typedef std::map<std::string, std::string> ArnoldNodeToMayaNode;

int FindLibraries(MString searchPath, MStringArray &files);

//--------------- CArnoldNodeFactory -----------------------------------

/// Responsible for loading Arnold and MtoA plugins and creating new Maya nodes

/// When MtoA is loaded in Maya, the node factory is called to load all Arnold plugins found on ARNOLD_PLUGIN_PATH,
/// and generate Maya nodes from the Arnold shaders contained within them. Metadata can be added to
/// the Arnold node and its parameters to control how the Maya node and its attributes are generated, if at all.

class DLLEXPORT CArnoldNodeFactory
{
public:
   ~CArnoldNodeFactory();
   static void SetMayaPlugin(MObject plugin);

   static CArnoldNodeFactory* Init(CExtension* extension, const char* shaderFile);
   void LoadPlugin();
   void CreateNodes();
   void UnloadPlugin();
   static void UnloadPlugins();

   bool RegisterMayaNode(const AtNodeEntry* arnoldNodeEntry);
   bool RegisterMayaNode(const char* arnoldNodeName, const char* mayaNodeName, int nodeId, MString classification=ARNOLD_SHADER, MString swatchName=ARNOLD_SWATCH);
   bool MapToMayaNode(const char* arnoldNodeName, const char* mayaCounterpart, int typeId);
   void UnregisterMayaNode(const char* arnoldNodeName);

   const char* GetFilename() {return m_shaderFile;}
   static const char* GetArnoldNodeFromMayaNode(const MString& mayaShader);

protected:
   CArnoldNodeFactory(CExtension* extension, const char* shaderFile) :
      m_shaderFile(shaderFile),
      m_extension(extension),
      m_loaded(false)
   {}

private:
   const char* m_shaderFile;
   CExtension* m_extension;
   bool m_loaded;
   MayaNodeDataMap m_factoryNodes;
   ArnoldNodeToMayaNode m_arnoldToMayaNodes;

   static int s_autoNodeId;
   static MObject s_plugin;
   static std::vector<CArnoldNodeFactory*> s_shaderLibs;
};

inline CArnoldNodeFactory::~CArnoldNodeFactory()
{
}


class DLLEXPORT CLoader
{
public:
   CLoader(){};
   ~CLoader();
   static void LoadPlugins();
   static void UnloadPlugins();
   static bool LoadExtension(const char* extensionFile);
   static void LoadExtensions();
   static void UnloadExtensions();
private:
   static std::vector<CExtension> s_extensions;
   static std::vector<CArnoldNodeFactory> s_shaderLibs;
};

//--------------- CAutoTranslator ------------------------------------------

/// A Translator class which can automatically export simple Maya nodes.

/// To perform an automatic export, the translator does the following:
///  -# gets the Arnold node entry that corresponds to the Maya node being export from CArnoldNodeFactory
///  -# loops through each parameter on the Arnold node entry
///  -# processes the equivalent attribute on the Maya node
///

class CAutoTranslator
   :  public CNodeTranslator
{
public:
   virtual AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      return CNodeTranslator::Init(dagPath.node(), scene, outputAttr);
   }
   static void* creator()
   {
      return new CAutoTranslator();
   }
   AtNode* CreateArnoldNodes();
   void Export(AtNode* atNode);
};
#endif  // ARNOLDNODEFACTORY_H
