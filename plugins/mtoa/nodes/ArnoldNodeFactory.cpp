#include "ArnoldNodeFactory.h"
#include "nodes/shaders/surface/ArnoldCustomShader.h"
#include "render/RenderSession.h"

#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_msg.h>

#include <maya/MFileObject.h>
#include <maya/MPlugArray.h>

#ifdef _WIN32
   #include <platform/win32/dirent.h>
   #define PATHSEP ';'
   #define DIRSEP "/"
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

/// @param searchPath  a path to search for libraries, optionally containing
/// separators (: on unix, ; on windows) and environment variables
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
      if ((dp  = opendir(dir.asChar())) == NULL)
      {
         // TODO: print better error message
         cerr << "Error(" << errno << ") opening " << dir << endl;
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

// CArnoldNodeFactory

// Don't use ARNOLD_NODEID_CUSTOM as it's an actual node!
int CArnoldNodeFactory::s_autoNodeId(ARNOLD_NODEID_AUTOGEN);
std::vector<CArnoldNodeFactory*> CArnoldNodeFactory::s_shaderLibs;
MObject CArnoldNodeFactory::s_plugin;

void CArnoldNodeFactory::SetMayaPlugin(MObject plugin)
{
	s_plugin = plugin;
}


/// Load an Arnold plugin.

/// Loads the Arnold plugin and registers a Maya node for each Arnold node
/// contained within it, if applicable.
///
/// @param pluginFile  the absolute path to an Arnold plugin
///
void CArnoldNodeFactory::LoadPlugin()
{
   // TODO: add error handling when solid angle adds a status result
   if (m_loaded)
   {
      // TODO: print warning
      return;
   }
   AiLoadPlugin(m_shaderFile);
   /*
   bool err;
   err = AiLoadPlugin(pluginFile);
   if (err)
   {
      str = MString("[mtoa] Could not load shader library: ") + pluginFile;
      MGlobal::displayError(str);
   else
   */
   {
      CreateNodes();
      m_loaded = true;
      s_shaderLibs.push_back(this);
   }
}

/// Create and track a new CExtension for the passed file, but don't call any plugin initialization functions
CArnoldNodeFactory* CArnoldNodeFactory::Init(CExtension* extension, const char* shaderFile)
{
   CArnoldNodeFactory* lib = new CArnoldNodeFactory(extension, shaderFile);
   s_shaderLibs.insert(s_shaderLibs.begin(), lib);
   return lib;
}

/// Register new Maya nodes

/// Loop through shaders in this library and create maya nodes for accepted types
void CArnoldNodeFactory::CreateNodes()
{
   MString str;
   if (strlen(m_shaderFile))
      str = MString("[mtoa] Generating nodes for shader library: ") + m_shaderFile;
   else
      str = MString("[mtoa] Generating nodes for builtin shader library");

   MGlobal::displayInfo(str);
   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_SHADER);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);

      const char* nentryFile = AiNodeEntryGetFilename(nentry);
      if (strcmp(nentryFile, m_shaderFile) == 0)
      {
         RegisterMayaNode(nentry);
      }
   }
}

/// Unload an Arnold plugin.

/// Remove from the Arnold universe all nodes created by the specified plugin and
/// unregister any Maya nodes generated for them.
///
void CArnoldNodeFactory::UnloadPlugin()
{
   MString str;
   str = MString("[mtoa] Removing nodes for shader library: ") + m_shaderFile;
   MGlobal::displayInfo(str);
   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_SHADER);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);

      const char* nentryFile = AiNodeEntryGetFilename(nentry);
      if (strcmp(nentryFile, m_shaderFile) == 0)
      {
         const char *arnoldNodeName = AiNodeEntryGetName(nentry);
         // remove from maya
         UnregisterMayaNode(arnoldNodeName);
         // remove from arnold
         AiNodeUninstall(arnoldNodeName);
      }
   }
   for (unsigned int i=0; i < s_shaderLibs.size(); ++i)
   {
      if (strcmp(s_shaderLibs[i]->GetFilename(), m_shaderFile))
      {
         delete s_shaderLibs[i];
         s_shaderLibs.erase(s_shaderLibs.begin() + i);
         break;
      }
   }
   m_loaded = false;
}

/// Unload all Arnold plugins on the plugin path and deregister all Maya nodes
///
void CArnoldNodeFactory::UnloadPlugins()
{
   for (unsigned int i=0; i < s_shaderLibs.size(); ++i)
   {
      s_shaderLibs[i]->UnloadPlugin();
   }
}

/// Associate an Arnold node with an existing Maya node.  During translation of
/// the Maya scene the Arnold node will be used wherever the Maya
/// node is encountered.
///
bool CArnoldNodeFactory::MapToMayaNode(const char* arnoldNodeName, const char* mayaCounterpart, int typeId)
{
   m_arnoldToMayaNodes[arnoldNodeName] = mayaCounterpart;
   if (m_extension->RegisterDependTranslator(mayaCounterpart, typeId, "auto", CAutoTranslator::creator))
   {
      m_factoryNodes[mayaCounterpart].arnoldNodeName = arnoldNodeName;
      m_factoryNodes[mayaCounterpart].nodeId = typeId;
      return true;
   }
   return false;
}

/// Register a Maya node for the given Arnold node
///
/// Certain optional node-level metadata can be used to control how the
/// node factory processes the node's registration:
///  -# "maya.name" - the name that should be used for the generated maya node
///  -# "maya.id" - the maya node id to use for the generated maya node
///  -# "maya.class" - classification string (defaults to "shader/surface")
///  -# "maya.counterpart" - the name of an existing maya node to
///     which this one should be mapped (see CArnoldNodeFactory::MapToMayaNode). no new node will
///     be generated.
///  -# "maya.hide" - skip registration altogether
///
/// See CBaseAttrHelper for parameter-level metadata for controlling attribute creation
///
/// @param arnoldNodeEntry  arnold AtNodeEntry from which to generate the new Maya node
///
/// @return true if the node is registered successfully, else false
///
bool CArnoldNodeFactory::RegisterMayaNode(const AtNodeEntry* arnoldNodeEntry)
{
   MStatus status;
   const char* arnoldNodeName = AiNodeEntryGetName(arnoldNodeEntry);

   // should the node be ignored?
   AtBoolean hide;
   if (AiMetaDataGetBool(arnoldNodeEntry, NULL, "maya.hide", &hide) && hide)
      return true;

   // map to an existing maya node?
   const char* mayaCounterpart;
   int mayaCounterpartId;
   if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.counterpart", &mayaCounterpart) &&
         AiMetaDataGetInt(arnoldNodeEntry, NULL, "maya.counterpart_id", &mayaCounterpartId))
   {
      if (!MapToMayaNode(arnoldNodeName, mayaCounterpart, mayaCounterpartId))
      {
         MGlobal::displayError(MString("[mtoa] Failed to create counter-part node ") + MString(mayaCounterpart));
         return false;
      }
      return true;
   }
   // remap node name?
   const char* mayaNodeName;
   if (!AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.name", &mayaNodeName))
      mayaNodeName = arnoldNodeName;

   // get nodeID
   AtInt nodeId;
   if (!AiMetaDataGetInt(arnoldNodeEntry, NULL, "maya.id", &nodeId))
   {
      nodeId = s_autoNodeId;
      // TODO: print hex nodeId
      MGlobal::displayWarning(MString("[mtoa] Assigning temporary node id ") + nodeId + " to " + arnoldNodeName);
      s_autoNodeId++;
   }

   // classification string
   const char* classification;
   if (!AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.classification", &classification))
      classification = ARNOLD_SHADER.asChar();
   // swatch string, use "ArnoldRenderSwatch" to enable arnold rendered swatches
   const char* swatch;
   if (!AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.swatch", &swatch))
      swatch = ARNOLD_SWATCH.asChar();

   MGlobal::displayInfo(MString("[mtoa] INFO: Loading shader: ") + arnoldNodeName + " as " + mayaNodeName);

  return RegisterMayaNode(arnoldNodeName, mayaNodeName, nodeId, classification, swatch);
}

/// Register a Maya node for the given Arnold node
///
/// See CBaseAttrHelper for parameter-level metadata for controlling attribute creation
///
/// @param arnoldNodeName  arnold node entry name from which to generate the new Maya node
/// @param mayaNodeName    name to use for the new Maya node
/// @param nodeId          maya node Id
/// @param shaderClass     maya shader class string
///
/// @return true if the node is registered successfully, else false
///
bool CArnoldNodeFactory::RegisterMayaNode(const char* arnoldNodeName, const char* mayaNodeName,
                                          int nodeId, MString classification, MString swatchName)
{
   // classifications are a delicate matter in maya.
   // without the "shader/surface" classification, swatches won't render (fixable?) and
   // normalCamera won't be automatically treated as a bump map in the hypershade.
   MString shaderClass("");
   if (classification.numChars())
   {
      shaderClass = classification;
      if (swatchName.numChars())
      {
         shaderClass += MString(":swatch/") + swatchName;
      }
   }

      // TODO: handle case where shaderClass holds a colon-separated list of classifications
      // if (shaderClass == ARNOLD_SHADER_SURFACE)
      // order here matters: currently, we must add this for swatches to work
      // shaderClass = "shader/surface:" + shaderClass;
   CArnoldCustomShaderNode::s_shaderName = arnoldNodeName;
   CArnoldCustomShaderNode::s_shaderClass = shaderClass;

   // Register the node and its parameters
   // AiMsgDebug("[mtoa] registering Arnold node %s as Maya node %s of classification %s", arnoldNodeName, mayaNodeName, shaderClass.asChar());
   AiMsgInfo("[mtoa] registering Arnold node %s as Maya node %s of classification %s using swatch %s",
             arnoldNodeName, mayaNodeName, shaderClass.asChar(), swatchName.asChar());

   MStatus status = MFnPlugin(s_plugin).registerNode(mayaNodeName, nodeId, CArnoldCustomShaderNode::creator,
                                                     CArnoldCustomShaderNode::initialize,
                                                     MPxNode::kDependNode, &shaderClass);
   CHECK_MSTATUS(status);

   if (status != MStatus::kSuccess || !MapToMayaNode(arnoldNodeName, mayaNodeName,  nodeId))
   {
      MGlobal::displayError(MString("[mtoa] Failed to create node ") + mayaNodeName);
      return false;
   }
   return true;
}

/// Unregister the maya node generated by the given arnold node and remove
/// all mappings associated with the arnold node.
///
void CArnoldNodeFactory::UnregisterMayaNode(const char* arnoldNodeName)
{
   MStatus status;
   MayaNodeDataMap::iterator it;
   // use the arnold node name to look up the maya node data
   it = m_factoryNodes.find(m_arnoldToMayaNodes[arnoldNodeName]);
   if (it != m_factoryNodes.end() && it->second.nodeId > 0)
   {
      status = MFnPlugin(s_plugin).deregisterNode(it->second.nodeId);
      CHECK_MSTATUS(status);
      m_arnoldToMayaNodes.erase(it->second.arnoldNodeName);
      m_factoryNodes.erase(it->first);
   }
}


std::vector<CExtension> CLoader::s_extensions;
std::vector<CArnoldNodeFactory> CLoader::s_shaderLibs;

/// Load all Arnold plugins on the plugin path
///
void CLoader::LoadPlugins()
{
   //MString resolvedPathList = MString("$ARNOLD_PLUGIN_PATH").expandEnvironmentVariablesAndTilde();
   MStringArray plugins;
   FindLibraries("$ARNOLD_PLUGIN_PATH", plugins);
   for (unsigned int i=0; i<plugins.length(); ++i)
   {
      MString pluginFile = plugins[i];
      if (pluginFile.length() > 0)
      {
#ifdef _WIN32
         char buffer[MAX_PATH];
         GetFullPathName(pluginFile.asChar(), MAX_PATH, buffer, NULL);
         pluginFile = MString(buffer);
#endif // _WIN32
         // if the shader library matches the convention [extension]_shaders, associate
         // the shader library with [extension]
         CExtension* extension = NULL;
         MFileObject file;
         file.setRawFullName(pluginFile);
         MString name = file.resolvedName();
         int pos = name.rindexW('_');
         if (pos > 1)
         {
            MString extname = name.substringW(0, pos-1);
            extension = CExtension::FindExtension(extname.asChar());
         }
         if (extension == NULL)
            extension = CExtension::Init(pluginFile.asChar());

         CArnoldNodeFactory* shaderLib = CArnoldNodeFactory::Init(extension, pluginFile.asChar());
         shaderLib->LoadPlugin();
      }
   }
}

/// Load all MtoA extensions on the extension path.
///
void CLoader::LoadExtensions()
{
   // TODO: replace this with python code
   MStringArray plugins;
   FindLibraries("$MTOA_EXTENSIONS_PATH", plugins);
   for (unsigned int i=0; i<plugins.length(); ++i)
   {
      MString plugin = plugins[i];
      if (plugin.length() > 0)
      {
         cerr << "Loading:" << plugin.asChar() << endl;
         CExtension::Load(plugin.asChar());
         MString cmd = "import mtoa.api.extensions;mtoa.api.extensions.loadExtensionUI('" + plugin + "')";
         CHECK_MSTATUS(MGlobal::executePythonCommand(cmd));
      }
   }
}

/// Unload all MtoA extensions on the extension path
///
void CLoader::UnloadExtensions()
{

}

/// Return the name of the Arnold node entry that corresponds to the given Maya node type
///
/// @param mayaShader  name of a maya type previously registered with the node factory
///
/// @return name of the Arnold node entry
///
const char* CArnoldNodeFactory::GetArnoldNodeFromMayaNode(const MString& mayaShader)
{
   for (unsigned int i=0; i < s_shaderLibs.size(); i++)
   {
      MayaNodeDataMap& nodes = s_shaderLibs[i]->m_factoryNodes;
      // FIXME: use iterators so we don't do the check twice
      if (nodes.count(mayaShader.asChar()))
         return nodes[mayaShader.asChar()].arnoldNodeName.c_str();
   }
   return "";
}



// AutoTranslator
//
AtNode* CAutoTranslator::CreateArnoldNodes()
{
   MString mayaShader = GetFnNode().typeName();
   return AddArnoldNode(CArnoldNodeFactory::GetArnoldNodeFromMayaNode(mayaShader));
}

void CAutoTranslator::Export(AtNode *shader)
{
   MStatus status;
   MPlug plug;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(shader->base_node);
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
         if (!AiMetaDataGetStr(shader->base_node, paramName, "maya.name", &attrName))
            attrName = paramName;

         plug = GetFnNode().findPlug(attrName, &status);
         if (status == MS::kSuccess)
            ProcessParameter(shader, plug, paramName, paramType);
         else
            AiMsgWarning("[mtoa] Attribute %s.%s requested by translator does not exist", GetFnNode().name().asChar(), attrName);
      }
   }

   MPlugArray connections;

   plug = GetFnNode().findPlug("normalCamera");

   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      MString attrName = connections[0].partialName(false, false, false, false, false, true);

      AtNode* bump = ExportShader(connections[0].node(), attrName);

      if (bump != NULL)
         AiNodeLink(bump, "@before", shader);
   }
}
