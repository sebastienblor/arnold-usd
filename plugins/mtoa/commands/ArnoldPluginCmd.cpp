#include "ArnoldPluginCmd.h"
#include "extension/ExtensionsManager.h"
#include "extension/Extension.h"
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
   return syntax;
}

MStatus CArnoldPluginCmd::doIt(const MArgList& argList)
{
   MArgDatabase args(syntax(), argList);
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
      MStringArray result;
      const AtNodeEntry* nodeEntry = AiNodeEntryLookUp(nodeName.asChar());
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
   }
   // FIXME: error on unknown flag
   return MS::kSuccess;
}
