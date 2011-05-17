#include "ArnoldPluginCmd.h"
#include "extension/ExtensionsManager.h"
#include "extension/Extension.h"

#include <maya/MArgDatabase.h>
#include <maya/MTypes.h>
#if MAYA_API_VERSION < 201200
#include "utils/MNodeClass.h"
#endif

MSyntax CArnoldPluginCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("lst", "listTranslators", MSyntax::kSelectionItem);
   syntax.addFlag("le", "loadExtension", MSyntax::kString);
   syntax.addFlag("ule", "unloadExtension", MSyntax::kString);
   return syntax;
}

MStatus CArnoldPluginCmd::doIt(const MArgList& argList)
{
   MArgDatabase args(syntax(), argList);
   if (args.isFlagSet("listTranslators"))
   {
      MSelectionList sel;
      args.getFlagArgument("listTranslators", 0, sel);
      MStatus status;
      MStringArray result;
      MObject obj;
      MDagPath dagPath;
      status = sel.getDagPath(0, dagPath);
      if (status == MS::kSuccess)
      {
         obj = dagPath.node();
      }
      else
      {
         status = sel.getDependNode(0, obj);
      }
      if (!obj.isNull())
      {
         MString typeName = MFnDependencyNode(obj).typeName();
         // Can also request only those from a specific extension
         // with GetTranslatorNames(typeName, provider)
         result = CExtensionsManager::GetTranslatorNames(typeName);
      }
      setResult(result);
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
   return MS::kSuccess;
}
