#include "TranslatorRegistry.h"

#include <maya/MSceneMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MTypes.h>
#include <maya/MFnPlugin.h>
#if MAYA_API_VERSION < 201200
   #include "utils/MNodeClass.h"
#else
   #include <maya/MNodeClass.h>
#endif


// --------- CTranslatorRegistry -------------//

std::map<int, CreatorFunction>  CTranslatorRegistry::s_dagTranslators;
std::map<int, CreatorFunction>  CTranslatorRegistry::s_dependTranslators;
PluginDataMap CTranslatorRegistry::s_mayaPluginData;
MCallbackId CTranslatorRegistry::s_pluginLoadedCallbackId;
MCallbackIdArray CTranslatorRegistry::s_mayaCallbackIDs;

// internal use
bool CTranslatorRegistry::RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   MStatus status;

   if (!MFnPlugin::isNodeRegistered(mayaNode))
   {
      if (strlen(providedByPlugin) != 0)
      {
         // can't add the callback if the node type is unknown
         // make the callback when the plugin is loaded
         CMayaPluginData pluginData;
         pluginData.mayaNode = mayaNode;
         pluginData.nodeClassInitializer = nodeClassInitializer;
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
      // call the node class initializer
      nodeClassInitializer(mayaNode);
   }
   return true;
}

bool CTranslatorRegistry::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   if (RegisterTranslator(mayaNode, typeId, creator, nodeClassInitializer, providedByPlugin))
   {
      s_dagTranslators[typeId] = creator;
      return true;
   }
   return false;
}

bool CTranslatorRegistry::RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   if (RegisterTranslator(mayaNode, typeId, creator, nodeClassInitializer, providedByPlugin))
   {
      s_dependTranslators[typeId] = creator;
      return true;
   }
   return false;
}

// internal use
bool CTranslatorRegistry::RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   if (!MFnPlugin::isNodeRegistered(mayaNode))
   {
      MGlobal::displayWarning(MString("[mtoa] Cannot register ") + mayaNode + ". the node type does not exist. If the node is provided by a plugin, specify providedByPlugin when registering its translator");
      return false;
   }
   return true;
}

bool CTranslatorRegistry::RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   if (RegisterTranslator(mayaNode, typeId, creator))
   {
      s_dagTranslators[typeId] = creator;
      return true;
   }
   return false;
}

bool CTranslatorRegistry::RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator)
{
   if (RegisterTranslator(mayaNode, typeId, creator))
   {
      s_dependTranslators[typeId] = creator;
      return true;
   }
   return false;
}


#if MAYA_API_VERSION >= 201200
// internal use
int CTranslatorRegistry::RegisterTranslator(const char* mayaNode, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   MStatus status;
   if (!MFnPlugin::isNodeRegistered(mayaNode))
   {
      if (strlen(providedByPlugin) != 0)
      {
         // can't add the callback if the node type is unknown
         // make the callback when the plugin is loaded
         CMayaPluginData pluginData;
         pluginData.mayaNode = mayaNode;
         pluginData.nodeClassInitializer = nodeClassInitializer;
         s_mayaPluginData[providedByPlugin].push_back(pluginData);
      }
      else
      {
         MGlobal::displayWarning(MString("[mtoa] Cannot register ") + mayaNode + ". the node type does not exist. If the node is provided by a plugin, specify providedByPlugin when registering its translator");
         return 0;
      }
   }
   else
   {
      // call the node class initializer
      nodeClassInitializer(mayaNode);
   }
   return MNodeClass(mayaNode).typeId().id();
}

bool CTranslatorRegistry::RegisterDagTranslator(const char* mayaNode, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   int typeId = RegisterTranslator(mayaNode, creator, nodeClassInitializer, providedByPlugin);
   if (typeId)
   {
      s_dagTranslators[typeId] = creator;
      return true;
   }
   return false;
}

bool CTranslatorRegistry::RegisterDependTranslator(const char* mayaNode, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   int typeId = RegisterTranslator(mayaNode, creator, nodeClassInitializer, providedByPlugin);
   if (typeId)
   {
      s_dependTranslators[typeId] = creator;
      return true;
   }
   return false;
}

// internal use
int CTranslatorRegistry::RegisterTranslator(const char* mayaNode, CreatorFunction creator)
{
   if (!MFnPlugin::isNodeRegistered(mayaNode))
   {
      MGlobal::displayWarning(MString("[mtoa] Cannot register ") + mayaNode + ". the node type does not exist. If the node is provided by a plugin, specify providedByPlugin when registering its translator");
      return 0;
   }
   return MNodeClass(mayaNode).typeId().id();
}

bool CTranslatorRegistry::RegisterDagTranslator(const char* mayaNode, CreatorFunction creator)
{
   int typeId = RegisterTranslator(mayaNode, typeId, creator);
   if (typeId)
   {
      s_dagTranslators[typeId] = creator;
      return true;
   }
   return false;
}

bool CTranslatorRegistry::RegisterDependTranslator(const char* mayaNode, CreatorFunction creator)
{
   int typeId = RegisterTranslator(mayaNode, typeId, creator);
   if (typeId)
   {
      s_dependTranslators[typeId] = creator;
      return true;
   }
   return false;
}
#endif

CNodeTranslator* CTranslatorRegistry::GetDependTranslator(int typeId)
{
   std::map<int, CreatorFunction>::iterator translatorIt;
   translatorIt = s_dependTranslators.find(typeId);
   if (translatorIt != s_dependTranslators.end())
   {
      return (CNodeTranslator*)translatorIt->second();
   }
   return NULL;
}

CDagTranslator* CTranslatorRegistry::GetDagTranslator(int typeId)
{
   std::map<int, CreatorFunction>::iterator translatorIt;
   translatorIt = s_dagTranslators.find(typeId);
   if (translatorIt != s_dagTranslators.end())
   {
      return (CDagTranslator*)translatorIt->second();
   }
   return NULL;
}


/// Called when a plugin is loaded to ensure that each translator that requires
/// node initialization gets a callback installed
void CTranslatorRegistry::MayaPluginLoadedCallback(const MStringArray &strs, void *clientData)
{
   // 0 = pluginPath, 1 = pluginName
   MString pluginName = strs[1];
   // get all the nodes created by this plugin with class initializers
   std::vector<CMayaPluginData> nodes = s_mayaPluginData[pluginName.asChar()];

   // start up the arnold universe so that attribute helpers can query arnold nodes
   bool active = false;
   if (AiUniverseIsActive())
      active = true;
   else
      AiBegin();

   for (unsigned int i=0; i < nodes.size(); i++)
      nodes[i].nodeClassInitializer(nodes[i].mayaNode);

   if (!active)
      AiEnd();

   // TODO: find out if extension attributes persist even if the plugin has been unloaded and then reloaded
   s_mayaPluginData[pluginName.asChar()].clear();
}

/// Installs the plugin-loaded callback
void CTranslatorRegistry::CreateCallbacks()
{
   MStatus status;
   // create callbacks
   s_pluginLoadedCallbackId = MSceneMessage::addStringArrayCallback(MSceneMessage::kAfterPluginLoad, CTranslatorRegistry::MayaPluginLoadedCallback, NULL, &status);
   CHECK_MSTATUS(status);
}

void CTranslatorRegistry::RemoveCallbacks()
{
   // delete callbacks
   if (s_pluginLoadedCallbackId != 0)
      MMessage::removeCallback(s_pluginLoadedCallbackId);

   const MStatus status = MNodeMessage::removeCallbacks(s_mayaCallbackIDs);
   CHECK_MSTATUS(status);
   if (status == MS::kSuccess)
      s_mayaCallbackIDs.clear();
}
