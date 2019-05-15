#include "utils/Version.h"
#include "ArnoldExportToMaterialXCmd.h"
#include "scene/MayaScene.h"

#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnAttribute.h>
#include <maya/MStringArray.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>
#include <ai_bbox.h>
#include <ai_materialx.h>
#include <math.h>

MSyntax CArnoldExportToMaterialXCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.addFlag("l", "look",  MSyntax::kString);
   syntax.addFlag("p", "properties",  MSyntax::kString);
   syntax.addFlag("r", "relative",  MSyntax::kBoolean);
   syntax.addFlag("fp", "fullPath",  MSyntax::kBoolean);
   syntax.addFlag("s", "separator",  MSyntax::kString);
   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}



MStatus CArnoldExportToMaterialXCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MSyntax syntax = newSyntax();

   // we must use an MArgParser because MArgList is not python compatible, and we
   // use this result to set syntax.useSelectionAsDefault() prior to creating the MArgDatabase.
   MArgParser args(syntax, argList, &status);
   MArgDatabase argDB(syntax, argList, &status);

   MSelectionList selected; // list of selected objects to be exported

   MStringArray sListStrings;
   argDB.getObjects(sListStrings);
   const unsigned int sListStringsLength = sListStrings.length();
   
   if (sListStringsLength > 0)
   {
      for (unsigned int i = 0; i < sListStringsLength; ++i)
         selected.add(sListStrings[i]);
   }
   else
      MGlobal::getActiveSelectionList(selected);

   MGlobal::executePythonCommand("import mtoa.core;mtoa.core.createOptions()"); 

   // do we want to export the whole scene if nothing is selected ?
   if (selected.length() == 0)
   {
      AiMsgError("[mtoa] No geometry selected");
      return MS::kFailure;
   }
   
   if (!argDB.isFlagSet("filename"))
   {
      AiMsgError("[mtoa] Export to MaterialX : No filename specified");
      return MS::kFailure;
   }

   CMayaScene::End();
   // Cannot export while a render is active
   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] Cannot Export to MaterialX while rendering");
      return MS::kFailure;
   }

   MString filename = "";
   argDB.getFlagArgument("filename", 0, filename);

   MString lookName = "default";
   if (argDB.isFlagSet("look"))
      argDB.getFlagArgument("look", 0, lookName);

   MString properties = "";
   if (argDB.isFlagSet("properties"))
      argDB.getFlagArgument("properties", 0, properties);

   bool relative = true;
   if (argDB.isFlagSet("relative")) 
      argDB.getFlagArgument("relative", 0, relative);


   int prevFullPath = 1;
   MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.exportFullPaths\"", prevFullPath);
   bool restoreFullPath = false;
   if (argDB.isFlagSet("fullPath")) 
   {
      bool fullPath = (prevFullPath != 0);
      argDB.getFlagArgument("fullPath", 0, fullPath);
      if (fullPath != (prevFullPath!=0))
      {
         restoreFullPath = true;
         MString cmdStr = "setAttr \"defaultArnoldRenderOptions.exportFullPaths\" ";
         cmdStr += fullPath ? "1" : "0";
         MGlobal::executeCommand(cmdStr);
      }
   }

   int prevSeparator = 0;
   MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.exportSeparator\"", prevSeparator);
   bool restoreSeparator = false;

   if (argDB.isFlagSet("separator")) 
   {
      MString separator = "|";
      argDB.getFlagArgument("separator", 0, separator);
      int separatorVal = (separator == MString("/")) ? 1 : 0;
      if (separatorVal != prevSeparator)
      {
         restoreSeparator = true;
         MString cmdStr = "setAttr \"defaultArnoldRenderOptions.exportSeparator\" ";
         cmdStr += separatorVal;
         MGlobal::executeCommand(cmdStr);
      }
   }
   
   CMayaScene::Begin(MTOA_SESSION_ASS);
   CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   renderSession->SetForceTranslateShadingEngines(true);   
   arnoldSession->SetExportFilterMask(AI_NODE_ALL);
   
   CMayaScene::Export(&selected);
/*
   MString txt("Running Export to MaterialX with filename=");
   txt += filename;
   txt += MString(", lookName=");
   txt += lookName;
   txt += MString(", properties=");
   txt += properties;
   
   MGlobal::displayWarning(txt);
*/

   AiRender(AI_RENDER_MODE_FREE);
   AiRenderAbort();
   
   AiMaterialxWrite(NULL, filename.asChar(), lookName.asChar(), properties.asChar(), relative);

   CMayaScene::End();

   if (restoreFullPath)
   {
      MString cmdStr("setAttr \"defaultArnoldRenderOptions.exportFullPaths\" ");
      cmdStr += prevFullPath;
      MGlobal::executeCommand(cmdStr);
   }
   
   if (restoreSeparator)
   {
      MString cmdStr("setAttr \"defaultArnoldRenderOptions.exportSeparator\" ");
      cmdStr += prevSeparator;
      MGlobal::executeCommand(cmdStr);
   }

   return MS::kSuccess;
}


