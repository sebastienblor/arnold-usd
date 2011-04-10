#ifndef TRANSLATORREGISTRY_H
#define TRANSLATORREGISTRY_H

#include "NodeTranslator.h"

#include <ai.h>

#include <string>
#include <vector>
#include <map>

struct CMayaPluginData
{
   MString mayaNode;
   NodeClassInitFunction nodeClassInitializer;
};

// plugin name to list of provided nodes needing callbacks
typedef std::map<std::string, std::vector<CMayaPluginData> > PluginDataMap;

// Translator Registry

/// Stores registered translators and associates them with Maya nodes
///
class DLLEXPORT CTranslatorRegistry
{
public:
   static bool RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
   static bool RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
   static bool RegisterDependTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
   static bool RegisterDagTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
#if MAYA_API_VERSION >= 201200
   // in 2012 we can determine the node Id from the node name
   static bool RegisterDagTranslator(const char* mayaNode, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   static bool RegisterDependTranslator(const char* mayaNode, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   static bool RegisterDagTranslator(const char* mayaNode, CreatorFunction creator);
   static bool RegisterDependTranslator(const char* mayaNode, CreatorFunction creator);
#endif
   static CNodeTranslator* GetDependTranslator(int typeId);
   static CDagTranslator* GetDagTranslator(int typeId);

   static void NodeCreatedCallback(MObject &node, void *clientData);
   static void MayaPluginLoadedCallback(const MStringArray &strs, void *clientData);
   static void CreateCallbacks();
   static void RemoveCallbacks();

private:
   static bool RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   static bool RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
#if MAYA_API_VERSION >= 201200
   static int RegisterTranslator(const char* mayaNode, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   static int RegisterTranslator(const char* mayaNode, CreatorFunction creator);
#endif

private:
   static std::map<int, CreatorFunction>  s_dagTranslators;
   static std::map<int, CreatorFunction>  s_dependTranslators;

   static PluginDataMap s_mayaPluginData;
   static MCallbackId s_pluginLoadedCallbackId;
   static MCallbackIdArray s_mayaCallbackIDs;
};


#endif // TRANSLATORREGISTRY_H
