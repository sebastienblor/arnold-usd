#include "ArnoldPluginCmd.h"
#include "extension/ExtensionsManager.h"
#include "attributes/AttrHelper.h"
#include "utils/Universe.h"

#include <maya/MArgDatabase.h>
#include <maya/MTypes.h>
#if MAYA_API_VERSION < 201200
#include "attributes/MNodeClass.h"
#endif

MSyntax CArnoldPluginCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("lst", "listTranslators", MSyntax::kString);
   syntax.addFlag("le", "loadExtension", MSyntax::kString);
   syntax.addFlag("ule", "unloadExtension", MSyntax::kString);
   syntax.addFlag("gad", "getAttrData", MSyntax::kString);

   syntax.addFlag("aov", "listAOVs", MSyntax::kNoArg);
   syntax.addFlag("nt", "nodeType", MSyntax::kString);
   syntax.addFlag("lnt", "listAOVNodeTypes", MSyntax::kNoArg);
   return syntax;
}

MStatus CArnoldPluginCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList, &status);
   if (!status)
      return status;

   if (args.isFlagSet("listTranslators"))
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
      status = ArnoldUniverseBegin();
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
            const char* label = "";
            AiMetaDataGetStr(nodeEntry, paramName, "label", &label);
            result.append(label);
            const char* desc = "";
            AiMetaDataGetStr(nodeEntry, paramName, "desc", &desc);
            result.append(desc);
         }
      }
      setResult(result);
      ArnoldUniverseEnd();
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

   // FIXME: error on unknown flag
   return MS::kSuccess;
}
