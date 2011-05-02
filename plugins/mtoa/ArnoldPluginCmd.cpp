#include "ArnoldPluginCmd.h"
#include "scene/TranslatorRegistry.h"

#include <maya/MArgDatabase.h>

MSyntax CArnoldPluginCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("lst", "listTranslators", MSyntax::kSelectionItem);
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
         result = CTranslatorRegistry::GetTranslatorNames(dagPath);
      }
      else
      {
         MObject obj;
         status = sel.getDependNode(0, obj);
         if (status == MS::kSuccess)
         {
            result = CTranslatorRegistry::GetTranslatorNames(obj);
         }
      }
      setResult(result);
   }
   return MS::kSuccess;
}
