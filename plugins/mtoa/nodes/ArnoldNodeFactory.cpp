//-------------------------------------------------------------------------- 
// ArnoldNodeFactory class implementation.
//-------------------------------------------------------------------------- 
#include "ArnoldNodeFactory.h"
#include "nodes/shaders/surface/ArnoldCustomShader.h"
#include "maya_scene/MayaScene.h"

#include <ai_plugins.h>
#include <ai_universe.h>
#include <ai_metadata.h>

//-------------------------------------------------------------------------- 
MayaNodeToArnoldNode CArnoldNodeFactory::s_factoryNodes;
ArnoldNodeToMayaData CArnoldNodeFactory::s_arnoldToMayaNodes;
ArnoldPluginData CArnoldNodeFactory::s_arnoldPlugins;

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
      str = MString("[mtoa]: Generating nodes for shader library: ") + pluginFile + MString(".");
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
   str = MString("[mtoa]: Removing nodes for shader library: ") + pluginFile + MString(".");
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
   MString resolvedPathList = MString("$MTOA_PLUGINS_PATH").expandEnvironmentVariablesAndTilde();

   MStringArray pluginPaths;
#ifdef _WIN32
   resolvedPathList.split(';', pluginPaths);
#else
   resolvedPathList.split(':', pluginPaths);
#endif

   for (unsigned int i=0; i<pluginPaths.length(); ++i)
   {
      MString pluginPath = pluginPaths[i];
      MGlobal::displayInfo(MString("[mtoa]: Adding custom shader path: ") + pluginPath.asChar());
      if (pluginPath.length() > 0)
      {
         AiLoadPlugins(pluginPath.asChar());
      }
   }
   m_loadOk = true;

   AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_SHADER);
   while (!AiNodeEntryIteratorFinished(nodeIter))
   {
      AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
      const char* nentryFile = AiNodeEntryGetFilename(nentry);
      if (strlen(nentryFile))
      {
         s_arnoldPlugins[std::string(nentryFile)].push_back(std::string(AiNodeEntryGetName(nentry)));
      }
   }

   RegisterAllNodes();
}


// Associate an arnold node with an existing maya node.  During translation of
// the maya scene to ass format, the arnold node will be used wherever the maya
// node is encountered.
//
void CArnoldNodeFactory::MapToMayaNode(const char* arnoldNodeName, const char* mayaCounterpart, int nodeId)
{
   // TODO: add attribute compatibility test?
   s_factoryNodes[mayaCounterpart] = arnoldNodeName;

   CMayaNodeData   data;
   data.mayaNodeName = mayaCounterpart;
   data.nodeId = nodeId;
   s_arnoldToMayaNodes[arnoldNodeName] = data;
   CMayaScene::RegisterTranslator(nodeId, CAutoTranslator::creator);
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
      s_factoryNodes[mayaNodeName] = arnoldNodeName;

      CMayaNodeData data;
      data.mayaNodeName = mayaNodeName;
      data.nodeId = nodeId;
      s_arnoldToMayaNodes[arnoldNodeName] = data;
      CMayaScene::RegisterTranslator(nodeId, CAutoTranslator::creator);
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
   ArnoldNodeToMayaData::iterator it;
   it = s_arnoldToMayaNodes.find(arnoldNodeName);
   if (it != s_arnoldToMayaNodes.end() && it->second.nodeId > 0)
   {
      status = m_plugin.deregisterNode(it->second.nodeId);
      CHECK_MSTATUS(status);
      s_arnoldToMayaNodes.erase(it);
      s_factoryNodes.erase(it->second.mayaNodeName);
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
   int nodeId;
   MayaNodeToArnoldNode::iterator it;
   str = MString("[mtoa]: Unloading ") + s_factoryNodes.size() + " shaders";
   MGlobal::displayInfo(str);
   for (it = s_factoryNodes.begin(); it!=s_factoryNodes.end(); it++)
   {
      str = MString("[mtoa]: Unloading shader: ") + it->second.c_str();
      MGlobal::displayInfo(str);

      nodeId = s_arnoldToMayaNodes[it->second].nodeId;
      if (nodeId >= 0)
      {
         status = m_plugin.deregisterNode(nodeId);
         CHECK_MSTATUS(status);
      }
      else
      {
         status = MStatus::kSuccess;
      }
   }
   s_arnoldToMayaNodes.clear();
   s_factoryNodes.clear();
}
