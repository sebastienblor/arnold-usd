#ifndef TRANSLATORREGISTRY_H
#define TRANSLATORREGISTRY_H

#include "NodeTranslator.h"
#include "common/DynLibrary.h"

#include <ai.h>

#include <string>
#include <vector>
#include <map>

class CExtension;

typedef void (*pluginInitFunctionType)(CExtension&);


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

class DLLEXPORT CExtension
{
public:
   virtual ~CExtension()
   {}
   const char* GetFilename() {return m_extensionFile;}
   static CExtension* Load(const char* extensionFile);
   static CExtension* Init(const char* extensionFile);
   static bool Unload(const char* extensionFile);
   bool RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                        NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
   bool RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator,
                                     NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin="");
   bool RegisterDependTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
   bool RegisterDagTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
#if MAYA_API_VERSION >= 201200
   // in 2012 we can determine the node Id from the node name
   bool RegisterDagTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator,
                                     NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   bool RegisterDependTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator,
                                        NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   bool RegisterDagTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator);
   bool RegisterDependTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator);
#endif
   CNodeTranslator* GetDependTranslator(MObject &object);
   CDagTranslator* GetDagTranslator(MDagPath &dagPath);

   MStringArray GetTranslatorNames(MObject &object);
   MStringArray GetTranslatorNames(MDagPath &dagPath);
   MStringArray GetTranslatorNames(int typeId);

   static CNodeTranslator* FindDependTranslator(MObject &object);
   static CDagTranslator* FindDagTranslator(MDagPath &dagPath);
   static MStringArray GetAllTranslatorNames(MObject &object);
   static MStringArray GetAllTranslatorNames(MDagPath &dagPath);
   static MStringArray GetAllTranslatorNames(int typeId);
   static CExtension* FindExtension(const char* baseName);
   void InitializePendingTranslators(MString& pluginName);

   static void MayaPluginLoadedCallback(const MStringArray &strs, void *clientData);
   static void CreateCallbacks();
   static void RemoveCallbacks();

protected:
   CExtension(const char* extensionFile) :
      m_extensionFile(extensionFile)
   {}
   void AddDagCreator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
   void AddDependCreator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator);
   bool RegisterTranslator(const char* mayaNode, int typeId, const char* translatorName, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   bool RegisterTranslator(const char* mayaNode, int typeId, CreatorFunction creator);
#if MAYA_API_VERSION >= 201200
   int RegisterTranslator(const char* mayaNode, const char* translatorName, CreatorFunction creator, NodeClassInitFunction nodeClassInitializer, const char* providedByPlugin);
   int RegisterTranslator(const char* mayaNode, CreatorFunction creator);
#endif

protected:
   const char* m_extensionFile;
   NodeIdToTranslatorMap m_dagTranslators;
   NodeIdToTranslatorMap m_dependTranslators;

   PluginDataMap m_mayaPluginData;
   static std::vector<CExtension*> s_extensions;
   static MCallbackId s_pluginLoadedCallbackId;
};


#endif // TRANSLATORREGISTRY_H
