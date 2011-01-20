//-------------------------------------------------------------------------- 
// ArnoldNodeFactory class implementation.
//-------------------------------------------------------------------------- 
#include "ArnoldNodeFactory.h"
#include "nodes/shaders/surface/ArnoldCustomShader.h"

#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>

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

void CExtension::RegisterTranslator(int typeId, CreatorFunction creator)
{
   CMayaScene::RegisterTranslator(typeId, creator);
}

void CExtension::RegisterDagTranslator(int typeId, CreatorFunction creator)
{
   CMayaScene::RegisterDagTranslator(typeId, creator);
}

// CArnoldNodeFactory

MayaNodeDataMap CArnoldNodeFactory::s_factoryNodes;
ArnoldNodeToMayaNode CArnoldNodeFactory::s_arnoldToMayaNodes;
ArnoldPluginData CArnoldNodeFactory::s_arnoldPlugins;
DynamicAttrMap CArnoldNodeFactory::s_dynamicAttributes;

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
void CArnoldNodeFactory::MapToMayaNode(const char* arnoldNodeName, const char* mayaCounterpart, int typeId)
{
   s_arnoldToMayaNodes[arnoldNodeName] = mayaCounterpart;
   RegisterTranslator(mayaCounterpart, typeId, CAutoTranslator::creator);
   s_factoryNodes[mayaCounterpart].arnoldNodeName = arnoldNodeName;
}

void CArnoldNodeFactory::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   if (!MFnPlugin::isNodeRegistered(mayaNode))
   {
      MGlobal::displayWarning(MString("[mtoa]: cannot register ") + mayaNode + ". the node type does not exist");
      return;
   }

   // TODO: add attribute compatibility test?
   CMayaNodeData   data;
   data.arnoldNodeName = "";
   data.nodeId = typeId;
   data.callbackId = 0;

   // add a callback for creating arnold attributes
   MStatus status;
   MCallbackId id = MDGMessage::addNodeAddedCallback(CArnoldNodeFactory::NodeCreatedCallback, mayaNode, NULL, &status);
   CHECK_MSTATUS(status);
   data.callbackId = id;

   s_factoryNodes[mayaNode] = data;
   CMayaScene::s_dagTranslators[typeId] = creator;
}

void CArnoldNodeFactory::RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   if (!MFnPlugin::isNodeRegistered(mayaNode))
   {
      MGlobal::displayWarning(MString("[mtoa]: cannot register ") + mayaNode + ". the node type does not exist");
      return;
   }

   // TODO: add attribute compatibility test?
   CMayaNodeData   data;
   data.arnoldNodeName = "";
   data.nodeId = typeId;
   data.callbackId = 0;

   // add a callback for creating arnold attributes
   MStatus status;
   MCallbackId id = MDGMessage::addNodeAddedCallback(CArnoldNodeFactory::NodeCreatedCallback, mayaNode, NULL, &status);
   CHECK_MSTATUS(status);
   data.callbackId = id;

   s_factoryNodes[mayaNode] = data;
   CMayaScene::s_dependTranslators[typeId] = creator;
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
void CArnoldNodeFactory::RegisterMayaNode(AtNodeEntry* arnoldNode)
{
   MStatus status;
   const char* arnoldNodeName = AiNodeEntryGetName(arnoldNode);

   // should the node be ignored?
   AtBoolean hide;
   if (AiMetaDataGetBool(arnoldNode, NULL, "maya.hide", &hide) && hide)
      return;

   // map to an existing maya node?
   const char* mayaCounterpart;
   int mayaCounterpartId;
   if (AiMetaDataGetStr(arnoldNode, NULL, "maya.counterpart", &mayaCounterpart) && AiMetaDataGetInt(arnoldNode, NULL, "maya.counterpart_id", &mayaCounterpartId))
      return MapToMayaNode(arnoldNodeName, mayaCounterpart, mayaCounterpartId);

   // remap node name?
   const char* mayaNodeName;
   if (!AiMetaDataGetStr(arnoldNode, NULL, "maya.name", &mayaNodeName))
      mayaNodeName = arnoldNodeName;

   // get nodeID
   AtInt nodeId;
   if (!AiMetaDataGetInt(arnoldNode, NULL, "maya.id", &nodeId))
   {
      nodeId = s_autoNodeId;
      // TODO: print hex nodeId
      MGlobal::displayWarning(MString("[mtoa]: Assigning temporary node id ") + nodeId + " to " + arnoldNodeName);
      s_autoNodeId++;
   }
   MGlobal::displayInfo(MString("[mtoa] INFO: Loading shader: ") + arnoldNodeName);

   // classification string
   const char* tmp;
   if (!AiMetaDataGetStr(arnoldNode, NULL, "maya.class", &tmp))
      tmp = "shader/surface";
   const MString shaderClass(tmp);

   // Create a custom named shader node type
   CArnoldCustomShaderNode::s_shaderName = arnoldNodeName;

   // Register the node and its parameters
   status = m_plugin.registerNode(mayaNodeName, nodeId, CArnoldCustomShaderNode::creator, CArnoldCustomShaderNode::initialize, MPxNode::kDependNode, &shaderClass);
   CHECK_MSTATUS(status);

   if (status == MStatus::kSuccess)
   {
      MapToMayaNode(arnoldNodeName, mayaNodeName,  nodeId);
   }
   else
   {
      MGlobal::displayError(MString("[mtoa]: Failed to create node ") + mayaNodeName);
   }
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
   for (it = s_arnoldPlugins.begin(); it!=s_arnoldPlugins.end(); it++)
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
   
   void* initializer = LibrarySymbol(pluginLib, "_Z16initializePluginR10CExtension");

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
#if defined(_LINUX) || defined(_DARWIN)
   // re-open mtoa.so so it's symbols are global. When Maya loads the plugin it seems to be loading it with RTLD_LOCAL
   // TODO: better error checking
   MStatus status;
   MString pluginPath = m_plugin.loadPath(&status);
   CHECK_MSTATUS(status);
   pluginPath += "/mtoa.so";
   cout << "mtoa path is " << pluginPath << endl;
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
}

void CArnoldNodeFactory::NodeCreatedCallback(MObject &node, void *clientData)
{
   MFnDependencyNode fnNode(node);
   std::vector<CAttrData> attrs = s_dynamicAttributes[fnNode.typeName().asChar()];
   CDynamicAttrHelper* helper = new CDynamicAttrHelper(node);
   for (unsigned int i=0; i < attrs.size(); i++)
   {
      CAttrData data = attrs[i];
      helper->MakeInput(data.name.asChar(), data);
   }
   delete helper;
}

void CArnoldNodeFactory::AddDynamicAttr(const char* mayaNode, CAttrData attrData)
{
   s_dynamicAttributes[mayaNode].push_back(attrData);
}
