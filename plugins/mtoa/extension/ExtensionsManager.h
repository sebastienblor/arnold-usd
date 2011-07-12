#ifndef EXTENSIONSMANAGER_H
#define EXTENSIONSMANAGER_H

#define MNoVersionString
#define MNoPluginEntry

#include "extension/Extension.h"
#include "extension/PxUtils.h"

#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>
#include <maya/MDGMessage.h>
#include <maya/MMessage.h>


//--------------- CExtensionsManager -----------------------------------

// Loaded extensions, in load order
typedef std::list<CExtension> ExtensionsList;

/// Responsible for loading Arnold and MtoA plugins and creating new Maya nodes

/// When MtoA is loaded in Maya, the node factory is called to load all Arnold plugins found on ARNOLD_PLUGIN_PATH,
/// and generate Maya nodes from the Arnold shaders contained within them. Metadata can be added to
/// the Arnold node and its parameters to control how the Maya node and its attributes are generated, if at all.
class DLLEXPORT CExtensionsManager
{
public:
   static void SetMayaPlugin(MObject plugin);
   static CExtension* GetBuiltin(MStatus *returnStatus=NULL);

   static CExtension* LoadArnoldPlugin(const MString &file,
                                       const MString &path=PLUGIN_SEARCH,
                                       MStatus *returnStatus=NULL);
   static MStatus LoadArnoldPlugins(const MString &path=PLUGIN_SEARCH);
   static CExtension* LoadExtension(const MString &file,
                                    const MString &path=EXTENSION_SEARCH,
                                    MStatus *returnStatus=NULL);
   static MStatus LoadExtensions(const MString &path=EXTENSION_SEARCH);

   static MStatus UnloadExtension(CExtension* extension);
   static MStatus UnloadExtensions();

   static MStatus RegisterExtension(CExtension* extension);
   static MStatus RegisterExtensions();
   static MStatus DeregisterExtension(CExtension* extension);
   static MStatus DeregisterExtensions();

   static CDagTranslator*  GetTranslator(const MDagPath &dagPath);
   static CNodeTranslator* GetTranslator(const MObject &object);
   static CNodeTranslator* GetTranslator(const MString &typeName,
                                         const MString &translatorName="");

   static MStringArray GetTranslatorNames(const MString &typeName,
                                          const MString &provider="");

   static CExtension* GetExtension(const MString &extensionFile);

   static void MayaPluginLoadedCallback(const MStringArray &strs, void *clientData);
   static MCallbackId CreatePluginLoadedCallback();
   static MStatus RemovePluginLoadedCallback();

protected:
   static CExtension* NewExtension(const MString &extensionFile);
   static MStatus DeleteExtension(CExtension* &extension);

   static MStatus RegisterMayaNode(const CPxMayaNode &mayaNode);
   static MStatus DeregisterMayaNode(const CPxMayaNode &mayaNode);

   // static CExtension* FindExtension(const MString &extensionFile);
   static const CPxMayaNode* FindRegisteredMayaNode(const CPxMayaNode &mayaNode);
   static const CPxTranslator* FindRegisteredTranslator(const CPxMayaNode &mayaNode,
                                                        const CPxTranslator &translator=CPxTranslator());
   static TranslatorsSet* FindRegisteredTranslators(const CPxMayaNode &mayaNode);

private:
   CExtensionsManager() {}

   static MayaNodesSet s_registeredMayaNodes;
   static MayaNodeToTranslatorsMap s_registeredTranslators;

   static MObject s_plugin;
   static ExtensionsList s_extensions;

   static MCallbackId s_pluginLoadedCallbackId;
};


#endif  // EXTENSIONSMANAGER_H
