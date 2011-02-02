//-------------------------------------------------------------------------- 
// ArnoldNodeFactory class implementation.
//-------------------------------------------------------------------------- 
#include "ArnoldNodeFactory.h"
#include "nodes/shaders/surface/ArnoldCustomShader.h"
#include "nodes/Metadata.h"
#include "render/RenderSession.h"

#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MPlugArray.h>

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
   CTranslatorRegistry::RegisterDependTranslator(mayaNode, typeId, creator);
}

void CExtension::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   CTranslatorRegistry::RegisterDagTranslator(mayaNode, typeId, creator);
}

void CExtension::RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin)
{
   CTranslatorRegistry::RegisterDependTranslator(mayaNode, typeId, creator, nodeInitializer, providedByPlugin);
}

void CExtension::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeInitFunction nodeInitializer, const char* providedByPlugin)
{
   CTranslatorRegistry::RegisterDagTranslator(mayaNode, typeId, creator, nodeInitializer, providedByPlugin);
}

// CArnoldNodeFactory

MayaNodeDataMap CArnoldNodeFactory::s_factoryNodes;
ArnoldNodeToMayaNode CArnoldNodeFactory::s_arnoldToMayaNodes;
ArnoldPluginData CArnoldNodeFactory::s_arnoldPlugins;
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
#ifdef _WIN32
         char buffer[MAX_PATH];
         GetFullPathName(plugin.asChar(), MAX_PATH, buffer, NULL);
         plugin = buffer;
#endif // _WIN32
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
   if (CTranslatorRegistry::RegisterDependTranslator(mayaCounterpart, typeId, CAutoTranslator::creator))
   {
      s_factoryNodes[mayaCounterpart].arnoldNodeName = arnoldNodeName;
      s_factoryNodes[mayaCounterpart].nodeId = typeId;
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
//  5. "maya.hide" - skip registration altogether
//
// options 4 & 5 will result in no new maya node being created.
//
bool CArnoldNodeFactory::RegisterMayaNode(const AtNodeEntry* arnoldNode)
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
   MString shaderClass = "";
   char tmp[256];
   if (MAiMetaDataGetStr(arnoldNode, NULL, "maya.class", tmp))
      shaderClass = tmp;
   return RegisterMayaNode(arnoldNodeName, mayaNodeName, nodeId, shaderClass.asChar());
}

bool CArnoldNodeFactory::RegisterMayaNode(const char* arnoldNodeName, const char* mayaNodeName, int nodeId, const char* shaderClass)
{
   MString classification = "shader/surface:swatch/ArnoldRenderSwatch";
   if (strlen(shaderClass))
      classification += MString(":") + shaderClass;

   // Create a custom named shader node type
   CArnoldCustomShaderNode::s_shaderName = arnoldNodeName;

   // Register the node and its parameters
   MStatus status = m_plugin.registerNode(mayaNodeName, nodeId, CArnoldCustomShaderNode::creator, CArnoldCustomShaderNode::initialize, MPxNode::kDependNode, &classification);
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
#ifdef _WIN32
         char buffer[MAX_PATH];
         GetFullPathName(plugin.asChar(), MAX_PATH, buffer, NULL);
         plugin = buffer;
#endif // _WIN32
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


// AutoTranslator
//
AtNode* CAutoTranslator::Export()
{
   MString mayaShader = m_fnNode.typeName();
   std::string arnoldNode = CArnoldNodeFactory::s_factoryNodes[mayaShader.asChar()].arnoldNodeName;
   AtNode* shader = NULL;
   m_nodeEntry = AiNodeEntryLookUp(arnoldNode.c_str());

   // Make sure that the given type of node exists
   if (m_nodeEntry != NULL)
   {
      shader = AiNode(arnoldNode.c_str());

      AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
      Update(shader);
   }
   return shader;
}

void CAutoTranslator::Update(AtNode *shader)
{
   MPlug plug;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(m_nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (!strncmp(paramName, "aov_", 4))
      {
         CRenderSession *renderSession = CRenderSession::GetInstance();
         const CRenderOptions *renderOptions = renderSession->RenderOptions();

         // do not check type for now
         std::string aovName(paramName);
         aovName = aovName.substr(4);
         if (renderOptions->FindAOV(aovName.c_str()) != size_t(-1))
         {
            AiNodeSetStr(shader, paramName, aovName.c_str());
         }
         else
         {
            AiNodeSetStr(shader, paramName, "");
         }
      }
      else if (strcmp(paramName, "name"))
      {
         AtInt paramType = AiParamGetType(paramEntry);

         // attr name name remap
         const char* attrName;
         if (!AiMetaDataGetStr(m_nodeEntry, paramName, "maya.name", &attrName))
            attrName = paramName;

         plug = m_fnNode.findPlug(attrName);
         ProcessParameter(shader, plug, paramName, paramType);
      }
   }

   MPlugArray connections;

   plug = m_fnNode.findPlug("normalCamera");

   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      MString attrName = connections[0].partialName(false, false, false, false, false, true);

      AtNode* bump = m_scene->ExportShader(connections[0].node(), attrName);

      if (bump != NULL)
         AiNodeLink(bump, "@before", shader);
   }
}
