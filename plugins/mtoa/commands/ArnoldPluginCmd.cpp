#include "ArnoldPluginCmd.h"
#include "extension/ExtensionsManager.h"
#include "attributes/AttrHelper.h"
#include "scene/MayaScene.h"
#include "utils/Universe.h"
#include "utils/BuildID.h"

#include <maya/MArgDatabase.h>
#include <maya/MTypes.h>

MSyntax CArnoldPluginCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("lsp", "listPlugins", MSyntax::kString);
   syntax.addFlag("lst", "listTranslators", MSyntax::kString);
   syntax.addFlag("le", "loadExtension", MSyntax::kString);
   syntax.addFlag("ule", "unloadExtension", MSyntax::kString);
   syntax.addFlag("gad", "getAttrData", MSyntax::kString);

   syntax.addFlag("aov", "listAOVs", MSyntax::kNoArg);
   syntax.addFlag("nt", "nodeType", MSyntax::kString);
   syntax.addFlag("n", "nodePlug", MSyntax::kSelectionItem);

   syntax.addFlag("lnt", "listAOVNodeTypes", MSyntax::kNoArg);
   syntax.addFlag("las", "listAOVShaders", MSyntax::kNoArg);
   syntax.addFlag("lcs", "listCustomShapes", MSyntax::kNoArg);
   syntax.addFlag("lop", "listOperators", MSyntax::kNoArg);
   syntax.addFlag("sdt", "setDefaultTranslator", MSyntax::kString, MSyntax::kString);

   syntax.addFlag("llx", "listLoadedExtensions", MSyntax::kNoArg);
   syntax.addFlag("gev", "getExtensionApiVersion", MSyntax::kString);
   syntax.addFlag("gbi", "getBuildID", MSyntax::kNoArg);
   syntax.addFlag("gcv", "getClmVersion", MSyntax::kNoArg);
   syntax.addFlag("gbd", "getBuildDate", MSyntax::kNoArg);


   return syntax;
}

MStatus CArnoldPluginCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList, &status);
   if (!status)
      return status;

   if (args.isFlagSet("listPlugins"))
   {
      MStringArray pluginNames;
      MString extPath = args.flagArgumentString("listPlugins", 0);
      CExtension* extension = CExtensionsManager::GetExtension(extPath);
      if (NULL != extension)
      {
         pluginNames = extension->GetOwnLoadedArnoldPlugins();
      }
      else
      {
         MGlobal::displayError(MString("Could not find extension ")+extPath);
      }
      // Can also request only those from a specific extension
      // with GetTranslatorNames(typeName, provider)
      setResult(pluginNames);
   }
   else if (args.isFlagSet("listTranslators"))
   {
      MString typeName = args.flagArgumentString("listTranslators", 0);
      // Can also request only those from a specific extension
      // with GetTranslatorNames(typeName, provider)
      setResult(CExtensionsManager::GetTranslatorNames(typeName));
   }
   else if (args.isFlagSet("loadExtension", 0))
   {
      MString extPath = args.flagArgumentString("loadExtension", 0);
      CExtensionsManager::LoadExtension(extPath);
   }
   else if (args.isFlagSet("unloadExtension", 0))
   {
      MString extPath = args.flagArgumentString("unloadExtension", 0);
      CExtension* extension = CExtensionsManager::GetExtension(extPath);
      if (NULL != extension)
      {
         CExtensionsManager::UnloadExtension(extension);
      }
      else
      {
         MGlobal::displayError(MString("Could not find extension ")+extPath);
      }
   }
   else if (args.isFlagSet("getAttrData", 0))
   {
      MString nodeName = args.flagArgumentString("getAttrData", 0);
      bool AiUniverseCreated = ArnoldUniverseBegin();
      MStringArray result;
      const AtNodeEntry* nodeEntry = AiNodeEntryLookUp(nodeName.asChar());
      if (nodeEntry == NULL)
      {
         MString err = "Unknown arnold node \"";
         err += nodeName + "\"";
         MGlobal::displayError( err );
         return MS::kInvalidParameter;
      }
      CBaseAttrHelper helper(nodeEntry);
      AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
      while (!AiParamIteratorFinished(nodeParam))
      {
         const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
         const char* paramName = AiParamGetName(paramEntry);
         if (!helper.IsHidden(paramName))
         {
            result.append(paramName);
            result.append(helper.GetMayaAttrName(paramName));
            AtString label("");
            AiMetaDataGetStr(nodeEntry, paramName, "label", &label);
            result.append(label.c_str());
            AtString desc("");
            AiMetaDataGetStr(nodeEntry, paramName, "desc", &desc);
            result.append(desc.c_str());
         }
      }
      AiParamIteratorDestroy(nodeParam);
      setResult(result);
      if (AiUniverseCreated) ArnoldUniverseEnd();
   }
   else if (args.isFlagSet("listAOVs"))
   {
      MStringArray result;
      if (args.isFlagSet("nodeType"))
      {
         MString nodeType;
         args.getFlagArgument("nodeType", 0, nodeType);
         CExtensionsManager::GetNodeAOVs(nodeType, result);
      }
      else if (args.isFlagSet("nodePlug"))
      {
         MSelectionList sel;
         args.getFlagArgument("nodePlug", 0, sel);
         MPlug plug;
         MStatus status = sel.getPlug(0, plug);
         CHECK_MSTATUS(status);
         //MGlobal::displayError(MString("Could not find extension "));

         AOVSet aovs;
         CMayaScene::Begin(MTOA_SESSION_SWATCH);
         CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
         arnoldSession->ExportOptions();
         arnoldSession->ExportNode(plug, true, -1, &status);

         CHECK_MSTATUS(status);
         CMayaScene::End();

         for (AOVSet::iterator it=aovs.begin(); it!=aovs.end(); ++it)
         {
            result.append(it->GetName());
         }
      }
      else
         CExtensionsManager::GetAOVs(result);
      setResult(result);
   }
   else if (args.isFlagSet("listAOVNodeTypes"))
   {
      MStringArray result;
      CExtensionsManager::GetNodeTypesWithAOVs(result);
      setResult(result);
   }
   else if (args.isFlagSet("listAOVShaders"))
   {
      MStringArray result;
      CExtensionsManager::GetAOVShaders(result);
      setResult(result);
   }
   else if (args.isFlagSet("listCustomShapes"))
   {
      MStringArray result;
      CExtensionsManager::GetCustomShapes(result);
      setResult(result);
   }
   else if (args.isFlagSet("listOperators"))
   {
      MStringArray result;
      CExtensionsManager::GetOperators(result);
      setResult(result);
   }
   else if(args.isFlagSet("setDefaultTranslator"))
   {
      MString mayaTypeName, translatorName;
      args.getFlagArgument("setDefaultTranslator", 0, mayaTypeName);
      args.getFlagArgument("setDefaultTranslator", 1, translatorName);
      CExtensionsManager::SetDefaultTranslator(mayaTypeName, translatorName);
   }
   else if(args.isFlagSet("listLoadedExtensions"))
   {
      MStringArray arr = CExtensionsManager::ListLoadedExtensions();
      setResult(arr);
   }
   else if(args.isFlagSet("getExtensionApiVersion"))
   {
      MString extensionName;
      args.getFlagArgument("getExtensionApiVersion", 0, extensionName);
      CExtension* extension = CExtensionsManager::GetExtensionByName(extensionName);
      if (extension != 0)
         setResult(extension->GetApiVersion());
   }
   else if(args.isFlagSet("getBuildID"))
   {
      setResult(MString(BUILD_ID));
   }
   else if(args.isFlagSet("getBuildDate"))
   {
      setResult(MString(__TIMESTAMP__));
   }
   else if(args.isFlagSet("getClmVersion"))
   {
#ifdef CLIC_V1
      setResult(MString("1"));
#endif
#ifdef CLIC_V2
      setResult(MString("2"));
#endif
   }

   // FIXME: error on unknown flag
   return MS::kSuccess;
}
