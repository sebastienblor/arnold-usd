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
   MString translatorName;
   NodeClassInitFunction nodeClassInitializer;
};

// plugin name to list of provided nodes needing callbacks
typedef std::map<std::string, std::vector<CMayaPluginData> > PluginDataMap;

typedef std::map<int, std::map<std::string, CreatorFunction> > NodeIdToTranslatorMap;

// Translator Registry

/// Stores registered translators and associates them with Maya nodes
///

class DLLEXPORT CTranslatorRegistry
{
public:
   static bool RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                        NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
   static bool RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                     NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
   static bool RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
   static bool RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
#if MAYA_API_VERSION >= 201200
   // in 2012 we can determine the node Id from the node name
   static bool RegisterDagTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator,
                                     NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   static bool RegisterDependTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator,
                                        NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   static bool RegisterDagTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator);
   static bool RegisterDependTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator);
#endif
   static CNodeTranslator* GetDependTranslator(MObject &object);
   static CDagTranslator* GetDagTranslator(MDagPath &dagPath);

   static MStringArray GetTranslatorNames(MObject &object);
   static MStringArray GetTranslatorNames(MDagPath &dagPath);

   static void NodeCreatedCallback(MObject &node, void *clientData);
   static void MayaPluginLoadedCallback(const MStringArray &strs, void *clientData);
   static void ManageCallback(const MCallbackId id);
   static void CreateCallbacks();
   static void RemoveCallbacks();

private:
   static void AddDagCreator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
   static void AddDependCreator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
   static bool RegisterTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   static bool RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
#if MAYA_API_VERSION >= 201200
   static int RegisterTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   static int RegisterTranslator(const char* mayaNode, CreatorFunction creator);
#endif

private:
   static NodeIdToTranslatorMap s_dagTranslators;
   static NodeIdToTranslatorMap s_dependTranslators;

   static PluginDataMap s_mayaPluginData;
   static MCallbackId s_pluginLoadedCallbackId;
   static MCallbackIdArray s_mayaCallbackIDs;
};


#endif // TRANSLATORREGISTRY_H
