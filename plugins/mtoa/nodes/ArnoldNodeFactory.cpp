//-------------------------------------------------------------------------- 
// ArnoldNodeFactory class implementation.
//-------------------------------------------------------------------------- 
#include "ArnoldNodeFactory.h"
#include "nodes/shaders/surface/ArnoldCustomShader.h"
#include "nodes/Metadata.h"

#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MSceneMessage.h>

#ifdef _WIN32
   #include <platform/win32/dirent.h>
   #define PATHSEP ';'
   #define DIRSEP "\\"
   #define LIBEXT MString(".dll")
#else
   #include <sys/types.h>
   #include <dirent.h>
   #include <dlfcn.h>

   #define PATHSEP ':'
   #define DIRSEP "/"
   #ifdef _LINUX
     #define LIBEXT MString(".so")
   #endif
   #ifdef _DARWIN
     #define LIBEXT MString(".dylib")
   #endif
#endif
// @param searchPath  a path to search for libraries, optionally containing
// separators ( : on unix, ; on windows ) and environment variables
int FindLibraries(MString searchPath, MStringArray &files)
{
   MString resolvedPathList = searchPath.expandFilePath();
   MStringArray pluginPaths;
   resolvedPathList.split(PATHSEP, pluginPaths);
   for (unsigned int i=0; i<pluginPaths.length(); ++i)
   {
      MString dir = pluginPaths[i];
      DIR *dp;
      struct dirent *dirp;
      if((dp  = opendir(dir.asChar())) == NULL)
      {
         // TODO: print better error message
         cout << "Error(" << errno << ") opening " << dir << endl;
         continue;
      }
      while ((dirp = readdir(dp)) != NULL)
      {
         MString entry = dirp->d_name;
         unsigned int nchars = entry.numChars();
         if (nchars > LIBEXT.numChars())
         {
            MString ext = entry.substringW(nchars-LIBEXT.numChars(), nchars);
            if (entry.substringW(0,0) != "." && ext == LIBEXT)
            {
               files.append(dir + DIRSEP + entry);
            }
         }
      }
      closedir(dp);
   }
   return 0;
}

// CExtension

void CExtension::RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   CArnoldNodeFactory::RegisterDependTranslator(mayaNode, typeId, creator);
}

void CExtension::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   CArnoldNodeFactory::RegisterDagTranslator(mayaNode, typeId, creator);
}

void CExtension::RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin)
{
   CArnoldNodeFactory::RegisterDependTranslator(mayaNode, typeId, creator, nodeInitializer, providedByPlugin);
}

void CExtension::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin)
{
   CArnoldNodeFactory::RegisterDagTranslator(mayaNode, typeId, creator, nodeInitializer, providedByPlugin);
}

// CArnoldNodeFactory

MayaNodeDataMap CArnoldNodeFactory::s_factoryNodes;
ArnoldNodeToMayaNode CArnoldNodeFactory::s_arnoldToMayaNodes;
ArnoldPluginData CArnoldNodeFactory::s_arnoldPlugins;
PluginDataMap CArnoldNodeFactory::s_mayaPluginData;
MCallbackId CArnoldNodeFactory::s_pluginLoadedCallbackId = 0;

int CArnoldNodeFactory::s_autoNodeId(ARNOLD_NODEID_CUSTOM);

// Load an arnold plugin.

// Loads the arnold plugin and registers a maya node for each arnold node
// contained within it, if applicable.
//
// @param pluginFile  the absolute path to an arnold plugin
//
void CArnoldNodeFactory::LoadPlugin(const char* pluginFile)
{
   MString str;
   // TODO: add error handling when solid angle adds a status result
   AiLoadPlugin(pluginFile);
   /*
   bool err;
   err = AiLoadPlugin(pluginFile);
   if (err)
   {
      str = MString("[mtoa]: Could not load shader library: ") + pluginFile + MString(".");
      MGlobal::displayError(str);
   else
   */
   {
      str = MString("[mtoa]: Generating nodes for shader library: ") + pluginFile;
      MGlobal::displayInfo(str);
      AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_SHADER);
      while (!AiNodeEntryIteratorFinished(nodeIter))
      {
         AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);

         const char* nentryFile = AiNodeEntryGetFilename(nentry);
         if (strcmp(nentryFile, pluginFile) == 0)
         {
            RegisterMayaNode(nentry);
         }
      }
   }
}

// Unload an arnold plugin.

// Remove from the arnold universe all nodes created by the specified plugin and
// unregister any maya nodes generated for them.
//
// @param pluginFile  the absolute path to a previously loaded arnold plugin
//
void CArnoldNodeFactory::UnloadPlugin(const char* pluginFile)
{
   MString str;
   str = MString("[mtoa]: Removing nodes for shader library: ") + pluginFile;
   MGlobal::displayInfo(str);
   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_SHADER);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);

      const char* nentryFile = AiNodeEntryGetFilename(nentry);
      if (strcmp(nentryFile, pluginFile) == 0)
      {
         const char *arnoldNodeName = AiNodeEntryGetName(nentry);
         // remove from maya
         UnregisterMayaNode(arnoldNodeName);
         // remove from arnold
         AiNodeUninstall(arnoldNodeName);
      }
   }
}

// Load all plugins on the plugin path
//
void CArnoldNodeFactory::LoadPlugins()
{
   //MString resolvedPathList = MString("$MTOA_PLUGINS_PATH").expandEnvironmentVariablesAndTilde();
   MStringArray plugins;
   FindLibraries("$MTOA_PLUGINS_PATH", plugins);
   for (unsigned int i=0; i<plugins.length(); ++i)
   {
      MString plugin = plugins[i];
      if (plugin.length() > 0)
      {
         LoadPlugin(plugin.asChar());
      }
   }
}


// Associate an arnold node with an existing maya node.  During translation of
// the maya scene to ass format, the arnold node will be used wherever the maya
// node is encountered.
//
bool CArnoldNodeFactory::MapToMayaNode(const char* arnoldNodeName, const char* mayaCounterpart, int typeId)
{
   s_arnoldToMayaNodes[arnoldNodeName] = mayaCounterpart;
   if (RegisterDependTranslator(mayaCounterpart, typeId, CAutoTranslator::creator))
   {
      s_factoryNodes[mayaCounterpart].arnoldNodeName = arnoldNodeName;
      return true;
   }
   return false;
}

// called when a plugin is loaded to ensure that each translator that requires
// node initialization gets a callback installed
void CArnoldNodeFactory::MayaPluginLoadedCallback(const MStringArray &strs, void *clientData)
{
   // 0 = pluginPath, 1 = pluginName
   MString pluginName = strs[1];
   std::vector<CMayaPluginData> nodes = s_mayaPluginData[pluginName.asChar()];
   for (unsigned int i=0; i < nodes.size(); i++)
   {
      // add a callback for creating arnold attributes
      MStatus status;
      AiMsgInfo("[mtoa] adding callback for node %s", nodes[i].mayaNode.c_str());
      MCallbackId id = MDGMessage::addNodeAddedCallback(
            CArnoldNodeFactory::NodeCreatedCallback,
            nodes[i].mayaNode.c_str(),
            (void *)nodes[i].nodeInitializer,
            &status);
      CHECK_MSTATUS(status);
      s_factoryNodes[nodes[i].mayaNode].callbackId = id;
   }
   // callbacks only need to be added once
   // it appears that they remain in place even after the plugin is unloaded and reloaded
   s_mayaPluginData[pluginName.asChar()].clear();
}

// internal use
bool CArnoldNodeFactory::RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin)
{
   MStatus status;
   MCallbackId id = 0;

   if (!MFnPlugin::isNodeRegistered(mayaNode))
   {
      if (strlen(providedByPlugin) != 0)
      {
         // can't add the callback if the node type is unknown
         // make the callback when the plugin is loaded
         CMayaPluginData pluginData;
         pluginData.mayaNode = mayaNode;
         pluginData.nodeInitializer = nodeInitializer;
         s_mayaPluginData[providedByPlugin].push_back(pluginData);
      }
      else
      {
         MGlobal::displayWarning(MString("[mtoa]: cannot register ") + mayaNode + ". the node type does not exist. If the node is provided by a plugin, specify providedByPlugin when registering its translator");
         return false;
      }
   }
   else
   {
      // add a callback for creating arnold attributes
      id = MDGMessage::addNodeAddedCallback(CArnoldNodeFactory::NodeCreatedCallback, mayaNode, (void *)nodeInitializer, &status);
      CHECK_MSTATUS(status);
   }

   CMayaNodeData   data;
   data.arnoldNodeName = "";
   data.nodeId = typeId;
   data.callbackId = id;

   s_factoryNodes[mayaNode] = data;
   return true;
}

bool CArnoldNodeFactory::RegisterDagTranslator(const char* mayaNode,int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin)
{
   if (RegisterTranslator(mayaNode, typeId, creator, nodeInitializer, providedByPlugin))
   {
      CMayaScene::RegisterDagTranslator(typeId, creator);
      return true;
   }
   return false;
}

bool CArnoldNodeFactory::RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin)
{
   if (RegisterTranslator(mayaNode, typeId, creator, nodeInitializer, providedByPlugin))
   {
      CMayaScene::RegisterTranslator(typeId, creator);
      return true;
   }
   return false;
}

// internal use
bool CArnoldNodeFactory::RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   if (!MFnPlugin::isNodeRegistered(mayaNode))
   {
      MGlobal::displayWarning(MString("[mtoa]: cannot register ") + mayaNode + ". the node type does not exist. If the node is provided by a plugin, specify providedByPlugin when registering its translator");
      return false;
   }

   CMayaNodeData   data;
   data.arnoldNodeName = "";
   data.nodeId = typeId;
   data.callbackId = 0;
   return true;
}

bool CArnoldNodeFactory::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   if (RegisterTranslator(mayaNode, typeId, creator))
   {
      CMayaScene::RegisterDagTranslator(typeId, creator);
      return true;
   }
   return false;
}

bool CArnoldNodeFactory::RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   if (RegisterTranslator(mayaNode, typeId, creator))
   {
      CMayaScene::RegisterTranslator(typeId, creator);
      return true;
   }
   return false;
}

// Register a maya node for the given arnold node
//
// Certain optional metadata can be used to control how the
// node factory processes its registration:
//  1. "maya.name" - the name that should be used for the generated maya node
//  2. "maya.id" - the maya node id to use for the generated maya node
//  3. "maya.class" - classification string (defaults to "shader/surface")
//  4. "maya.counterpart" - the name of an existing maya node to
//     which this one should be mapped (see MapToMayaNode). no new node will
//     be generated.
//  5. "maya.ignore" - skip registration altogether
//
// options 4 & 5 will result in no new maya node being created.
//
bool CArnoldNodeFactory::RegisterMayaNode(AtNodeEntry* arnoldNode)
{
   MStatus status;
   const char* arnoldNodeName = AiNodeEntryGetName(arnoldNode);

   // should the node be ignored?
   AtBoolean hide;
   if (MAiMetaDataGetBool(arnoldNode, NULL, "maya.hide", &hide) && hide)
      return true;

   // map to an existing maya node?
   char mayaCounterpart[128];
   int mayaCounterpartId;
   if (MAiMetaDataGetStr(arnoldNode, NULL, "maya.counterpart", mayaCounterpart) && AiMetaDataGetInt(arnoldNode, NULL, "maya.counterpart_id", &mayaCounterpartId))
   {
      if (!MapToMayaNode(arnoldNodeName, mayaCounterpart, mayaCounterpartId))
      {
         MGlobal::displayError(MString("[mtoa]: Failed to create counter-part node ") + mayaCounterpart);
         return false;
      }
      return true;
   }
   // remap node name?
   char mayaNodeName[128];
   if (!MAiMetaDataGetStr(arnoldNode, NULL, "maya.name", mayaNodeName))
      strcpy(mayaNodeName, arnoldNodeName);

   // get nodeID
   AtInt nodeId;
   if (!MAiMetaDataGetInt(arnoldNode, NULL, "maya.id", &nodeId))
   {
      nodeId = s_autoNodeId;
      // TODO: print hex nodeId
      MGlobal::displayWarning(MString("[mtoa]: Assigning temporary node id ") + nodeId + " to " + arnoldNodeName);
      s_autoNodeId++;
   }
   MGlobal::displayInfo(MString("[mtoa] INFO: Loading shader: ") + arnoldNodeName);

   // classification string
   MString shaderClass = "shader/surface";
   char tmp[256];
   if (MAiMetaDataGetStr(arnoldNode, NULL, "maya.class", tmp))
      shaderClass = tmp;
   shaderClass += "/:swatch/ArnoldRenderSwatch";

   // Create a custom named shader node type
   CArnoldCustomShaderNode::s_shaderName = arnoldNodeName;

   // Register the node and its parameters
   status = m_plugin.registerNode(mayaNodeName, nodeId, CArnoldCustomShaderNode::creator, CArnoldCustomShaderNode::initialize, MPxNode::kDependNode, &shaderClass);
   CHECK_MSTATUS(status);

   if (status != MStatus::kSuccess || !MapToMayaNode(arnoldNodeName, mayaNodeName,  nodeId))
   {
      MGlobal::displayError(MString("[mtoa]: Failed to create node ") + mayaNodeName);
      return false;
   }
   return true;
}

// Unregister the maya node generated by the given arnold node and remove
// all mappings associated with the arnold node.
//
void CArnoldNodeFactory::UnregisterMayaNode(const char* arnoldNodeName)
{
   MStatus status;
   MayaNodeDataMap::iterator it;
   // use the arnold node name to look up the maya node data
   it = s_factoryNodes.find(s_arnoldToMayaNodes[arnoldNodeName]);
   if (it != s_factoryNodes.end() && it->second.nodeId > 0)
   {
      status = m_plugin.deregisterNode(it->second.nodeId);
      CHECK_MSTATUS(status);
      if (it->second.callbackId)
      {
         status = MDGMessage::removeCallback(it->second.callbackId);
         CHECK_MSTATUS(status);
      }
      s_arnoldToMayaNodes.erase(it->second.arnoldNodeName);
      s_factoryNodes.erase(it->first);
   }
}

void CArnoldNodeFactory::RegisterAllNodes()
{
   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_SHADER);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
      const char* nentryFile = AiNodeEntryGetFilename(nentry);
      if (strlen(nentryFile))
      {
         RegisterMayaNode(nentry);
      }
   }
}


void CArnoldNodeFactory::UnregisterAllNodes()
{
   MStatus status;
   MString str;
   ArnoldPluginData::iterator it;
   for (it = s_arnoldPlugins.begin(); it!=s_arnoldPlugins.end(); ++it)
   {
      std::vector<std::string> nodes = it->second;
      for (unsigned int i=0; i < nodes.size(); i++)
      {
         UnregisterMayaNode(nodes[i].c_str());
      }
   }
   s_arnoldPlugins.clear();
}


bool CArnoldNodeFactory::LoadExtension(const char* extensionFile)
{
   AiMsgDebug("loading extension %s", extensionFile);
   void *pluginLib = LibraryLoad(extensionFile);
   if (pluginLib == NULL)
   {
      MGlobal::displayError(MString("[mtoa] error loading plugin: ") + LibraryLastError());
      return false;
   }
   
   void* initializer = LibrarySymbol(pluginLib, "initializePlugin");

   if (initializer == NULL)
   {
      MGlobal::displayError(MString("[mtoa] error initializing plugin: ") + LibraryLastError());
      return false;
   }
   pluginInitFunctionType * initFunc = (pluginInitFunctionType*)(&initializer);
   
   CExtension plugin = CExtension();
   (*initFunc)(plugin);

   return true;
}

// Load all mtoa extensions on the extension path
//
void CArnoldNodeFactory::LoadExtensions()
{
   MStatus status;
#if defined(_LINUX) || defined(_DARWIN)
   // re-open mtoa.so so it's symbols are global. When Maya loads the plugin it seems to be loading it with RTLD_LOCAL
   // TODO: better error checking
   MString pluginPath = m_plugin.loadPath(&status);
   CHECK_MSTATUS(status);
   pluginPath += "/mtoa.so";
   m_pluginHandle = dlopen(pluginPath.asChar(), RTLD_LAZY | RTLD_GLOBAL );
#endif

   MStringArray plugins;
   FindLibraries("$MTOA_EXTENSIONS_PATH", plugins);
   for (unsigned int i=0; i<plugins.length(); ++i)
   {
      MString plugin = plugins[i];
      if (plugin.length() > 0)
      {
         LoadExtension(plugin.asChar());
         MString cmd = "import mtoa.api.extensions;mtoa.api.extensions.loadExtensionUI('" + plugin + "')";
         CHECK_MSTATUS(MGlobal::executePythonCommand(cmd));
      }
   }
   // create callbacks
   s_pluginLoadedCallbackId = MSceneMessage::addStringArrayCallback(MSceneMessage::kAfterPluginLoad, CArnoldNodeFactory::MayaPluginLoadedCallback, NULL, &status);
   CHECK_MSTATUS(status);
}

// Unload all mtoa extensions on the extension path
//
void CArnoldNodeFactory::UnloadExtensions()
{
#if defined(_LINUX) || defined(_DARWIN)
   if (m_pluginHandle != NULL)
   {
      MStatus status;
      MString pluginPath = m_plugin.loadPath(&status);
      CHECK_MSTATUS(status);
      dlclose(m_pluginHandle);
   }
#endif
   // delete callbacks
   if ( s_pluginLoadedCallbackId != 0 )
      MMessage::removeCallback( s_pluginLoadedCallbackId );
}

void CArnoldNodeFactory::NodeCreatedCallback(MObject &node, void *clientData)
{
   // CDynamicAttrHelper requires the universe to be initialized so that it can
   // query information from arnold nodes
   NodeInitFunction nodeInitializer = (NodeInitFunction)clientData;
   if (AiUniverseIsActive())
      nodeInitializer(node);
   else
   {
      AiBegin();
      nodeInitializer(node);
      AiEnd();
   }
}
