#include "Extension.h"

#include <maya/MSceneMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MTypes.h>
#include <maya/MFnPlugin.h>
#include <maya/MFileObject.h>
#if MAYA_API_VERSION < 201200
   #include "utils/MNodeClass.h"
#else
   #include <maya/MNodeClass.h>
#endif


// --------- CExtension -------------//

MCallbackId CExtension::s_pluginLoadedCallbackId;
std::vector<CExtension*> CExtension::s_extensions;

/// Load an MtoA extension.
///
/// @return true if the extension is loaded successfully, else false
///
CExtension* CExtension::Load(const char* extensionFile)
{
   AiMsgDebug("[mtoa] Loading extension %s", extensionFile);
   void *pluginLib = LibraryLoad(extensionFile);
   if (pluginLib == NULL)
   {
      MGlobal::displayError(MString("[mtoa] Error loading plugin: ") + LibraryLastError());
      return NULL;
   }

   void* initializer = LibrarySymbol(pluginLib, "initializePlugin");

   if (initializer == NULL)
   {
      MGlobal::displayError(MString("[mtoa] Error initializing plugin: ") + LibraryLastError());
      return NULL;
   }
   pluginInitFunctionType * initFunc = (pluginInitFunctionType*)(&initializer);

   CExtension* ext = new CExtension(extensionFile);
   s_extensions.insert(s_extensions.begin(), ext);
   (*initFunc)(*ext);
   MGlobal::displayInfo(MString("[mtoa] Loaded extension: ") + extensionFile);
   return ext;
}

/// Create and track a new CExtension for the passed file, but don't call any plugin initialization functions
CExtension* CExtension::Init(const char* extensionFile)
{
   CExtension* ext = new CExtension(extensionFile);
   s_extensions.insert(s_extensions.begin(), ext);
   return ext;
}

/// Unload an MtoA extension.
///
/// return true if the extension is loaded successfully, else false
///
bool CExtension::Unload(const char* extensionFile)
{
   for (unsigned int i=0; i < s_extensions.size(); i++)
   {
      if (strcmp(s_extensions[i]->GetFilename(), extensionFile) == 0)
      {
         delete s_extensions[i];
         s_extensions.erase(s_extensions.begin() + i);
         MGlobal::displayInfo(MString("[mtoa] Unloaded extension: ") + extensionFile);
         return true;
      }
   }
   return false;
}

// internal use
void CExtension::AddCreator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator)
{
   MStringArray translators = GetAllTranslatorNames(typeId);
   // if a translator already exists for this node ...
   if (translators.length())
   {
      bool found = false;
      for (unsigned int i=0; i < translators.length(); ++i)
      {
         if (translators[i] == translatorName)
         {
            found = true;
            break;
         }
      }
      if (found)
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
   m_translators[typeId][translatorName] = creator;
}

bool CExtension::RegisterTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
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
         m_mayaPluginData[providedByPlugin].push_back(pluginData);
      }
      else
      {
         MGlobal::displayWarning(MString("[mtoa]: cannot register ") + mayaNode + ". the node type does not exist. If the node is provided by a plugin, specify providedByPlugin when registering its translator");
         return false;
      }
   }
   else
   {
      AiMsgDebug("[mtoa] Registering translator \"%s\" for maya node \"%s\"", translatorName, mayaNode);
      // call the node class initializer
      nodeClassInitializer(mayaNode);
   }
   AddCreator(mayaNode, typeId, translatorName, creator);
   return true;
}

bool CExtension::RegisterTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator)
{
   AddCreator(mayaNode, typeId, translatorName, creator);
   return true;
}

#if MAYA_API_VERSION >= 201200

bool CExtension::RegisterTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator,
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
         m_mayaPluginData[providedByPlugin].push_back(pluginData);
      }
      else
      {
         MGlobal::displayWarning(MString("[mtoa] Cannot register ") + mayaNode + ". the node type does not exist. If the node is provided by a plugin, specify providedByPlugin when registering its translator");
         return false;
      }
   }
   else
   {
      // call the node class initializer
      nodeClassInitializer(mayaNode);
   }
   int typeId = MNodeClass(mayaNode).typeId().id();
   if (typeId)
   {
      AddCreator(mayaNode, typeId, translatorName, creator);
      return true;
   }
   return false;
}

bool CExtension::RegisterTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator)
{

   // need an existing node to query the typeId
   if (!MFnPlugin::isNodeRegistered(mayaNode))
   {
      MGlobal::displayWarning(MString("[mtoa] Cannot register ") + mayaNode + ". the node type does not exist. If the node is provided by a plugin, specify providedByPlugin when registering its translator");
      return false;
   }
   int typeId = MNodeClass(mayaNode).typeId().id();
   if (typeId)
   {
      AddCreator(mayaNode, typeId, translatorName, creator);
      return true;
   }
   return false;
}
#endif

/// Create a new CNodeTranslator for the passed Maya node
CNodeTranslator* CExtension::GetTranslator(MObject &object)
{
   MFnDependencyNode node(object);
   int typeId = node.typeId().id();
   NodeIdToTranslatorMap::iterator translatorIt = m_translators.find(typeId);
   std::map<std::string, CreatorFunction>::iterator subIt;
   CNodeTranslator* result = NULL;
   if (translatorIt != m_translators.end())
   {
      std::map<std::string, CreatorFunction>& subTypes = translatorIt->second;
      if (subTypes.size())
      {
         MStatus status;
         MPlug plug = node.findPlug("arnoldTranslator", &status);
         if (status != MS::kSuccess)
         {
            // use the last
            subIt = --subTypes.end();
            CreatorFunction func = subIt->second;
            result = (CNodeTranslator*)func();
         }
         else
         {
            // find the translator with the given name
            MString transName = plug.asString();
            if (transName == "")
            {
               MGlobal::displayWarning(MString("[mtoa] node \"") + node.name() + ".arnoldTranslator\" set to empty string. Using last registered translator.");
               // use the last
               subIt = --subTypes.end();
               CreatorFunction func = subIt->second;
               result = (CNodeTranslator*)func();
            }
            else
            {
               subIt = subTypes.find(transName.asChar());
               if (subIt != subTypes.end())
               {
                  CreatorFunction func = subIt->second;
                  result = (CNodeTranslator*)func();
               }
            }
         }
      }
   }
   if (result != NULL)
      result->SetTranslatorName(subIt->first.c_str());
   return result;
}

/// Create a new CDagTranslator for the passed Maya node
CDagTranslator* CExtension::GetTranslator(MDagPath &dagPath)
{
   MObject node = dagPath.node();
   return (CDagTranslator*)GetTranslator(node);
}

/// Search all extensions and create a CNodeTranslator for the passed Maya node
CNodeTranslator* CExtension::FindTranslator(MObject &object)
{
   for (unsigned int i=0; i < s_extensions.size(); i++)
   {
      CNodeTranslator* trans = s_extensions[i]->GetTranslator(object);
      if (trans != NULL)
         return trans;
   }
   return NULL;
}

/// Search all extensions and create a CDagTranslator for the passed Maya node
CDagTranslator* CExtension::FindTranslator(MDagPath &dagPath)
{
   MObject node = dagPath.node();
   return (CDagTranslator*)FindTranslator(node);
}

MStringArray CExtension::GetTranslatorNames(int typeId)
{
   MStringArray result;
   NodeIdToTranslatorMap::iterator translatorIt;
   std::map<std::string, CreatorFunction>::iterator it;
   translatorIt = m_translators.find(typeId);
   if (translatorIt != m_translators.end())
   {
      std::map<std::string, CreatorFunction>& subTypes = translatorIt->second;
      for (it = subTypes.begin(); it != subTypes.end(); ++it)
         result.append(it->first.c_str());
   }
   return result;
}

MStringArray CExtension::GetTranslatorNames(MObject &object)
{
   MFnDependencyNode node(object);
   return GetTranslatorNames(node.typeId().id());
}

MStringArray CExtension::GetTranslatorNames(MDagPath &dagPath)
{
   MObject node = dagPath.node();
   return GetTranslatorNames(node);
}

MStringArray CExtension::GetAllTranslatorNames(MObject &object)
{
   MStringArray result;
   for (unsigned int i=0; i < s_extensions.size(); i++)
   {
      MStringArray tmp = s_extensions[i]->GetTranslatorNames(object);
      for (unsigned int j=0; j < tmp.length(); j++)
         result.append(tmp[j]);
   }
   return result;
}

MStringArray CExtension::GetAllTranslatorNames(MDagPath &dagPath)
{
   MObject node = dagPath.node();
   return GetAllTranslatorNames(node);
}

MStringArray CExtension::GetAllTranslatorNames(int typeId)
{
   MStringArray result;
   for (unsigned int i=0; i < s_extensions.size(); i++)
   {
      MStringArray tmp = s_extensions[i]->GetTranslatorNames(typeId);
      for (unsigned int j=0; j < tmp.length(); j++)
         result.append(tmp[j]);
   }
   return result;
}

/// Find an extension based on its short name.
//
/// Given the name of an extension without the directory or file extension,
/// return a CExtension pointer, or NULL
CExtension* CExtension::FindExtension(const char* baseName)
{
   for (unsigned int i=0; i < s_extensions.size(); ++i)
   {
      MFileObject file;
      file.setRawFullName(s_extensions[i]->GetFilename());
      MString name = file.resolvedName();
      int pos = name.rindexW('.');
      if (pos > 1)
      {
         MString extName = name.substringW(0, pos-1);
         if (extName == baseName)
            return s_extensions[i];
      }
   }
   return NULL;
}

/// Initialize all translators that are waiting for a particular plugin to load
void CExtension::InitializePendingTranslators(MString& pluginName)
{
   // get all the nodes created by this plugin with class initializers
   std::vector<CMayaPluginData>& nodes = m_mayaPluginData[pluginName.asChar()];
   for (unsigned int i=0; i < nodes.size(); i++)
      nodes[i].nodeClassInitializer(nodes[i].mayaNode);

   // TODO: find out if extension attributes persist even if the plugin has been unloaded and then reloaded
   m_mayaPluginData[pluginName.asChar()].clear();
}


/// Called when a plugin is loaded to ensure that each translator that requires
/// node initialization gets a callback installed
void CExtension::MayaPluginLoadedCallback(const MStringArray &strs, void *clientData)
{
   // 0 = pluginPath, 1 = pluginName
   MString pluginName = strs[1];

   // start up the arnold universe so that attribute helpers can query arnold nodes
   bool active = false;
   if (AiUniverseIsActive())
      active = true;
   else
      AiBegin();

   for (unsigned int i=0; i < s_extensions.size(); i++)
      s_extensions[i]->InitializePendingTranslators(pluginName);

   if (!active)
      AiEnd();
}

/// Installs the plugin-loaded callback
void CExtension::CreateCallbacks()
{
   MStatus status;
   // create callbacks
   s_pluginLoadedCallbackId = MSceneMessage::addStringArrayCallback(MSceneMessage::kAfterPluginLoad, CExtension::MayaPluginLoadedCallback, NULL, &status);
   CHECK_MSTATUS(status);
}

void CExtension::RemoveCallbacks()
{
   // delete callbacks
   if (s_pluginLoadedCallbackId != 0)
      MMessage::removeCallback(s_pluginLoadedCallbackId);
   /*
   const MStatus status = MNodeMessage::removeCallbacks(s_mayaCallbackIDs);
   CHECK_MSTATUS(status);
   if (status == MS::kSuccess)
      s_mayaCallbackIDs.clear();
   */
}

