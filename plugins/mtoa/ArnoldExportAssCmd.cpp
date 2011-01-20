
#include "ArnoldExportAssCmd.h"
#include "render/RenderSession.h"
#include "scene/MayaScene.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderUtil.h>

MSyntax CArnoldExportAssCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("s", "selected");
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.addFlag("cam", "camera", MSyntax::kString);
   syntax.addFlag("sf", "startFrame", MSyntax::kLong);
   syntax.addFlag("ef", "endFrame", MSyntax::kLong);
   syntax.addFlag("fs", "frameStep", MSyntax::kLong);
   return syntax;
}

MString CArnoldExportAssCmd::GetCameraName()
{
   MString cameraName("");
   if (MGlobal::mayaState() == MGlobal::kInteractive)
   {
      MDagPath cameraPath;
      M3dView::active3dView().getCamera(cameraPath);

      MFnDagNode cameraNode(cameraPath.node());
      cameraName = cameraNode.partialPathName();
   }
   else
   {
      MStringArray renderableCameras;

      MGlobal::executePythonCommand("import maya");
      MGlobal::executePythonCommand("import maya.cmds");
      MGlobal::executePythonCommand("filter(lambda x: maya.cmds.getAttr(x+\".renderable\") == 1, maya.cmds.ls(type=\"camera\"))", renderableCameras);

      if (renderableCameras.length() > 0)
      {
         if (renderableCameras.length() > 1)
         {
           MGlobal::displayWarning("More than one renderable camera, use first one. (use the -cam/-camera option to override)");
         }
         cameraName = renderableCameras[0];
      }
    }
    return cameraName;
}

MStatus CArnoldExportAssCmd::doIt(const MArgList& argList)
{
   MStatus status;
   CRenderSession* renderSession = CRenderSession::GetInstance();
   // Just incase we're rendering with IPR.
   MGlobal::executeCommand("stopIprRendering renderView;");
   renderSession->Finish();

   MSyntax syntax = newSyntax();
   MArgDatabase argDB(syntax, argList, &status);

   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] ERROR: Cannot export to .ass while rendering.");
      return MS::kFailure;
   }

   // We don't need renderview_display so we need to set Batch mode on.
   renderSession->SetBatch(true);
   MString customFileName = "";
   MString cameraName = "";

   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);
   
   // Custom filename
   if (argDB.isFlagSet("filename"))
   {
      argDB.getFlagArgument("filename", 0, customFileName);
   }
   
   if (argDB.isFlagSet("camera"))
   {
      argDB.getFlagArgument("camera", 0, cameraName);
   }

   if (argDB.isFlagSet("selected"))
   {
      // Translate only selected objects
      renderSession->ExecuteScript(renderGlobals.preRenderMel);
      renderSession->Translate(MTOA_EXPORT_SELECTED);
      renderSession->DoExport(customFileName);
      renderSession->Finish();
      renderSession->ExecuteScript(renderGlobals.postRenderMel);
   }
   else
   {
      // Export the entire maya scene

      // If camera name is not set, default to active view camera in interactive mode
      // or the first found renderable camera in batch mode
      if (cameraName == "")
      {
         cameraName = GetCameraName();
      }

      int startframe = 1;
      int endframe = 0;
      int framestep = 1;

      if (argDB.isFlagSet("startFrame"))
      {
         argDB.getFlagArgument("startFrame", 0, startframe);
      }

      if (argDB.isFlagSet("endFrame"))
      {
         argDB.getFlagArgument("endFrame", 0, endframe);
      }

      if (argDB.isFlagSet("frameStep"))
      {
         argDB.getFlagArgument("frameStep", 0, framestep);
      }

      if (startframe <= endframe)
      {
         // customFileName is a prefix, need to add frame and extension
         // -> might want to check if extension or frame is already set
         MString curfilename;
         char frameext[64];

         for (int curframe = startframe; curframe <= endframe; curframe += framestep)
         {
            MGlobal::viewFrame((double)curframe);
            renderSession->ExecuteScript(renderGlobals.preRenderMel);

            sprintf(frameext, ".%04d.ass", curframe);
            curfilename = customFileName;
            curfilename += frameext;

            renderSession->Translate();
            if (cameraName != "") renderSession->SetCamera(cameraName);
            renderSession->DoExport(curfilename);
            renderSession->Finish();
            renderSession->ExecuteScript(renderGlobals.postRenderMel);
         }

      }
      else
      {
         renderSession->ExecuteScript(renderGlobals.preRenderMel);
         renderSession->Translate();
         if (cameraName != "") renderSession->SetCamera(cameraName);
         renderSession->DoExport(customFileName);
         renderSession->Finish();
         renderSession->ExecuteScript(renderGlobals.postRenderMel);
      }
   }

   return status;
}
