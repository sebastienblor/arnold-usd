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

NodeIdToTranslatorMap  CTranslatorRegistry::s_dagTranslators;
NodeIdToTranslatorMap  CTranslatorRegistry::s_dependTranslators;
PluginDataMap CTranslatorRegistry::s_mayaPluginData;
MCallbackId CTranslatorRegistry::s_pluginLoadedCallbackId;
MCallbackIdArray CTranslatorRegistry::s_mayaCallbackIDs;

// internal use
bool CTranslatorRegistry::RegisterTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                             NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
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
         pluginData.translatorName = translatorName;
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

// internal use
void CTranslatorRegistry::AddDagCreator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator)
{
   std::map<std::string, CreatorFunction> translators = s_dagTranslators[typeId];
   // if a translator already exists for this node ...
   if (translators.size())
   {
      if (translators.count(translatorName))
      {
         MGlobal::displayWarning(MString("[mtoa] Overriding translator \"") + translatorName + "\" for maya node \"" + mayaNode + "\"");
         // FIXME: do override
      }
      else
      {
         // create the "arnoldTranslator" attribute
         CExtensionAttrHelper helper(mayaNode); 
         CAttrData data;
         data.defaultValue.STR = "";
         data.name = "arnoldTranslator";
         data.shortName = "arntr";
         helper.MakeInputString(data);
      }
   }
   s_dagTranslators[typeId][translatorName] = creator;
}

// internal use
void CTranslatorRegistry::AddDependCreator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator)
{
   std::map<std::string, CreatorFunction> translators = s_dependTranslators[typeId];
   // if a translator already exists for this node ...
   if (translators.size())
   {
      if (translators.count(translatorName))
      {
         MGlobal::displayWarning(MString("[mtoa] Overriding translator \"") + translatorName + "\" for maya node \"" + mayaNode + "\"");
         // FIXME: do override
      }
      else
      {
         // create the "arnoldTranslator" attribute
         CExtensionAttrHelper helper(mayaNode);
         CAttrData data;
         data.defaultValue.STR = "";
         data.name = "arnoldTranslator";
         data.shortName = "arntr";
         helper.MakeInputString(data);
      }
   }
   s_dependTranslators[typeId][translatorName] = creator;
}

bool CTranslatorRegistry::RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                                NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   if (RegisterTranslator(mayaNode, typeId, translatorName, creator, nodeClassInitializer, providedByPlugin))
   {
      AddDagCreator(mayaNode, typeId, translatorName, creator);
      return true;
   }
   return false;
}

bool CTranslatorRegistry::RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                                   NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   if (RegisterTranslator(mayaNode, typeId, translatorName, creator, nodeClassInitializer, providedByPlugin))
   {
      AddDependCreator(mayaNode, typeId, translatorName, creator);
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

bool CTranslatorRegistry::RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator)
{
   if (RegisterTranslator(mayaNode, typeId, creator))
   {
      AddDagCreator(mayaNode, typeId, translatorName, creator);
      return true;
   }
   return false;
}

bool CTranslatorRegistry::RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator)
{
   if (RegisterTranslator(mayaNode, typeId, creator))
   {
      AddDependCreator(mayaNode, typeId, translatorName, creator);
      return true;
   }
   return false;
}


#if MAYA_API_VERSION >= 201200
// internal use
int CTranslatorRegistry::RegisterTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator,
                                            NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
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
         pluginData.translatorName = translatorName;
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

bool CTranslatorRegistry::RegisterDagTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator,
                                                NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   int typeId = RegisterTranslator(mayaNode, translatorName, creator, nodeClassInitializer, providedByPlugin);
   if (typeId)
   {
      AddDagCreator(mayaNode, typeId, translatorName, creator);
      return true;
   }
   return false;
}

bool CTranslatorRegistry::RegisterDependTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator,
                                                   NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin)
{
   int typeId = RegisterTranslator(mayaNode, translatorName, creator, nodeClassInitializer, providedByPlugin);
   if (typeId)
   {
      AddDependCreator(mayaNode, typeId, translatorName, creator);
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

bool CTranslatorRegistry::RegisterDagTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator)
{
   int typeId = RegisterTranslator(mayaNode, typeId, creator);
   if (typeId)
   {
      AddDagCreator(mayaNode, typeId, translatorName, creator);
      return true;
   }
   return false;
}

bool CTranslatorRegistry::RegisterDependTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator)
{
   int typeId = RegisterTranslator(mayaNode, typeId, creator);
   if (typeId)
   {
      AddDependCreator(mayaNode, typeId, translatorName, creator);
      return true;
   }
   return false;
}
#endif

/// Create a new CNodeTranslator for the passed Maya node
CNodeTranslator* CTranslatorRegistry::GetDependTranslator(MObject &object)
{
   MFnDependencyNode node(object);
   int typeId = node.typeId().id();
   NodeIdToTranslatorMap::iterator translatorIt = s_dependTranslators.find(typeId);
   if (translatorIt != s_dependTranslators.end())
   {
      std::map<std::string, CreatorFunction> subTypes = translatorIt->second;
      if (subTypes.size())
      {
         MStatus status;
         MPlug plug = node.findPlug("arnoldTranslator", &status);
         if (status != MS::kSuccess)
         {
            // use the last
            CreatorFunction func = (--subTypes.end())->second;
            return (CNodeTranslator*)func();
         }
         else
         {
            // find the translator with the given name
            MString transName = plug.asString();
            if (transName == "")
            {
               MGlobal::displayWarning(MString("[mtoa] node \"") + node.name() + ".arnoldTranslator\" set to empty string. Using last registered translator.");
               // use the last
               CreatorFunction func = (--subTypes.end())->second;
               return (CNodeTranslator*)func();
            }
            else
            {
               std::map<std::string, CreatorFunction>::iterator it = subTypes.find(transName.asChar());
               if (it != subTypes.end())
               {
                  CreatorFunction func = it->second;
                  return (CNodeTranslator*)func();
               }
            }
         }
      }
   }
   return NULL;
}

/// Create a new CDagTranslator for the passed Maya node
CDagTranslator* CTranslatorRegistry::GetDagTranslator(MDagPath &dagPath)
{
   MFnDependencyNode node(dagPath.node());
   int typeId = node.typeId().id();
   NodeIdToTranslatorMap::iterator translatorIt = s_dagTranslators.find(typeId);
   if (translatorIt != s_dagTranslators.end())
   {
      std::map<std::string, CreatorFunction> subTypes = translatorIt->second;
      if (subTypes.size())
      {
         MStatus status;
         MPlug plug = node.findPlug("arnoldTranslator", &status);
         if (status != MS::kSuccess)
         {
            // use the last
            CreatorFunction func = (--subTypes.end())->second;
            return (CDagTranslator*)func();
         }
         else
         {
            // find the translator with the given name
            MString transName = plug.asString();
            if (transName == "")
            {
               MGlobal::displayWarning(MString("[mtoa] node \"") + node.name() + ".arnoldTranslator\" set to empty string. Using last registered translator.");
               // use the last
               CreatorFunction func = (--subTypes.end())->second;
               return (CDagTranslator*)func();
            }
            else
            {
               std::map<std::string, CreatorFunction>::iterator it = subTypes.find(transName.asChar());
               if (it != subTypes.end())
               {
                  CreatorFunction func = it->second;
                  return (CDagTranslator*)func();
               }
            }
         }
      }
   }
   return NULL;
}

MStringArray CTranslatorRegistry::GetTranslatorNames(MObject &object)
{
   MFnDependencyNode node(object);
   int typeId = node.typeId().id();
   NodeIdToTranslatorMap::iterator translatorIt = s_dependTranslators.find(typeId);
   MStringArray result;
   if (translatorIt != s_dependTranslators.end())
   {
      std::map<std::string, CreatorFunction> subTypes = translatorIt->second;
      std::map<std::string, CreatorFunction>::iterator it;
      for (it = subTypes.begin(); it != subTypes.end(); ++it)
         result.append(it->first.c_str());
   }
   return result;
}

MStringArray CTranslatorRegistry::GetTranslatorNames(MDagPath &dagPath)
{
   MFnDependencyNode node(dagPath.node());
   int typeId = node.typeId().id();
   NodeIdToTranslatorMap::iterator translatorIt = s_dagTranslators.find(typeId);
   MStringArray result;
   if (translatorIt != s_dagTranslators.end())
   {
      std::map<std::string, CreatorFunction> subTypes = translatorIt->second;
      std::map<std::string, CreatorFunction>::iterator it;
      for (it = subTypes.begin(); it != subTypes.end(); ++it)
         result.append(it->first.c_str());
   }
   return result;
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

void CTranslatorRegistry::ManageCallback(const MCallbackId id)
{
   s_mayaCallbackIDs.append(id);
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
