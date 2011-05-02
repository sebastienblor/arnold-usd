#include "ArnoldPluginCmd.h"
#include "scene/Extension.h"

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
      MDagPath dagPath;
      status = sel.getDagPath(0, dagPath);
      if (status == MS::kSuccess)
      {
         result = CExtension::GetAllTranslatorNames(dagPath);
      }
      else
      {
         MObject obj;
         status = sel.getDependNode(0, obj);
         if (status == MS::kSuccess)
         {
            result = CExtension::GetAllTranslatorNames(obj);
         }
      }
      setResult(result);
   }
   else if (args.isFlagSet("loadExtension", 0))
   {
      MString extPath = args.flagArgumentString("loadExtension", 0);
      CExtension::Load(extPath.asChar());
#if MAYA_API_VERSION < 201200
      // versions of maya before extension attributes need to manually have their
      // dynamic attributes updated
      MNodeClass::InitializeExistingNodes();
#endif
   }
   else if (args.isFlagSet("unloadExtension", 0))
   {
      MString extPath = args.flagArgumentString("unloadExtension", 0);
      CExtension::Unload(extPath.asChar());
   }
   return MS::kSuccess;
}
