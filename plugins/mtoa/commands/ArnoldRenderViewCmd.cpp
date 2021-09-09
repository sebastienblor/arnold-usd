/*
#ifndef _WIN64
#include "../render/display/renderview.h"
#endif
*/
#include "ArnoldRenderViewCmd.h"

#include "translators/DagTranslator.h"
#include "session/ArnoldRenderViewSession.h"
#include "session/SessionManager.h"

#include <ai_universe.h>

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnDagNode.h>
#include <maya/MRenderView.h>
#include <maya/MRenderUtil.h>
#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/M3dView.h>
#include <maya/MCommonRenderSettingsData.h>

#include <vector>

static bool s_wasVisible = false;
static const std::string s_renderViewSessionId("renderView");
MSyntax CArnoldRenderViewCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("cam", "camera", MSyntax::kSelectionItem);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);
   syntax.addFlag("m", "mode", MSyntax::kString);
   syntax.addFlag("r", "region", MSyntax::kUnsigned, MSyntax::kUnsigned, MSyntax::kUnsigned, MSyntax::kUnsigned);
   syntax.addFlag("opt", "option", MSyntax::kString, MSyntax::kString);
   syntax.addFlag("get", "getoption", MSyntax::kString);
   syntax.addFlag("st", "status", MSyntax::kString);
   return syntax;
}


MStatus CArnoldRenderViewCmd::doIt(const MArgList& argList)
{

#ifdef MTOA_DISABLE_RV
   MGlobal::displayError("Arnold RenderView has been disabled in this build");
   return MS::kFailure;
#endif
   // IPR only works in interactive GUI mode
   //if (!MRenderView::doesRenderEditorExist())
   //   return MS::kFailure;
   MStatus status;
   MArgDatabase args(syntax(), argList);

   MString mode = (args.isFlagSet("mode")) ? args.flagArgumentString("mode", 0) : "render";

   // When the workspace is closed, we used to call directly -mode "visChanged" below. 
   // But since Maya 2018 this isn't working properly and this callback is invoked too early. 
   // When we ask (see below) its visibility, it didn't return the correct value. So now we're first going though an "idle" callback
   // See #3518
   if (mode == "visChanged_cb")
   {
      MGlobal::executeCommandOnIdle("arnoldRenderView -mode visChanged");
      return MS::kSuccess;
   }

   if (mode == "visChanged")
   {
      // this signal is emitted by maya workspaces when the workspace visibility changes
      // However, we're receiving this signal a bit too often, so we must only treat it
      // if it actually changes the current visibility.
      // So we're storing a static boolean s_wasVisible to keep track of the changes

      int workspaceVisible = 0;
      // get the information of the workspace visibility. This command only works in Maya 2017 and above
      // but we're not supposed to get here for previous versions as the argument "visChanged" in only
      // invoked for workspaces
      MGlobal::executeCommand("workspaceControl -q -vis \"ArnoldRenderView\"", workspaceVisible);
      bool isVisible = (workspaceVisible > 0);

      if (isVisible == s_wasVisible) 
         return status; // nothing has changed, we can leave


      if (isVisible)
         mode = "open"; // turning visibility ON, just open ARV
      else
         mode = "close"; // closing ARV
   }


   CArnoldRenderViewSession *session = (CArnoldRenderViewSession *)CSessionManager::FindActiveSession(s_renderViewSessionId);
   bool sessionExisted = (session != nullptr);
   if (session == nullptr)
   {
      session = new CArnoldRenderViewSession();
      CSessionManager::AddActiveSession(s_renderViewSessionId, session);
   }

   if (mode == "close")
   {  
      s_wasVisible = false;      
      session->CloseRenderView();
      return MS::kSuccess;
   }
   
   if (args.isFlagSet("option"))
   {
      MString option = args.flagArgumentString("option", 0);
      MString value = args.flagArgumentString("option", 1);
      session->SetRenderViewOption(option, value);
      return MS::kSuccess;
   }
   if (args.isFlagSet("getoption"))
   {
      MString option = args.flagArgumentString("get", 0);
      setResult(session->GetRenderViewOption(option));
      return MS::kSuccess;
   }

/* this doesn't seem to be used anymore
   if (args.isFlagSet("status"))
   {
      MString statusLog = args.flagArgumentString("status", 0);
      session->SetStatus(statusLog);
      return MS::kSuccess;
   }
   */


   // Get argument to "-mode" flag
   
   int width  = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
   int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;
   if (width > 0 && height > 0)
      session->GetOptions().SetResolution(width, height);
   else
   {
      width = height = 1024; // FIXME default ARV size ??.
   }

   bool is_region = args.isFlagSet("region");
   int region[4];
   if (is_region)
   {
      for (int i = 0; i < 4; ++i)
         region[i] = args.flagArgumentInt("region", i);

      session->GetOptions().SetRegion(region[0], region[1], region[2], region[3]);
   }

   MSelectionList selected;
   /*
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);*/

   // What mode are we in?
   if (mode == "render" || mode == "open" || mode == "render_silent")
   {
      // Close any viewports renderers before opening the ARV. (? should we close anything now ?)
      // CRenderSession::CloseOtherViews("");

      s_wasVisible = true;

      if (sessionExisted)
      {
         // A render view session has already been started
         // let's pop-up the window, and eventually re-render
         session->OpenRenderView();

         if (mode == "render" || mode == "render_silent")
            session->SetRenderViewOption("Run IPR", "1");
         
         return MS::kSuccess;
      }

      if (mode == "open")
      {  
         // Just open the renderview
         session->SetCamerasList();
         session->OpenRenderView();
         // do we need the options here ? session->ExportOptions();
         
         session->SetRenderViewOption("Run IPR", "0");
         session->GetRenderView().RequestFullSceneUpdate();
         session->SetStatus(MString(""));
         return MS::kSuccess;
      }


      MDagPath renderCamera;

      // First check if a camera is specified
      if (args.isFlagSet("camera"))
      {
         MSelectionList sel;
         args.getFlagArgument("camera", 0, sel);
         MDagPath camera;
         status = sel.getDagPath(0, camera);

         if (camera.isValid())
            renderCamera = camera;
      }

      // if not, get the viewport camera, and otherwise the first one found
      if (!renderCamera.isValid())
      {
         MDagPathArray cameras = session->GetRenderCameras(true);
         if (cameras.length() > 0)
            renderCamera = cameras[0];
      }
      session->SetExportCamera(renderCamera, false);

      // FIXME do we want the preMel callbacks and should they be handled here ???
      MCommonRenderSettingsData renderGlobals;
      MRenderUtil::getCommonRenderSettings(renderGlobals);
      MGlobal::executeCommand(renderGlobals.preMel);
      MGlobal::executeCommand(renderGlobals.preRenderMel);

      if (!renderGlobals.renderAll)
      {
         MSelectionList selected;
         MGlobal::getActiveSelectionList(selected);
         session->Export(&selected);
      }
      else
      {
         session->Export();
      }
      
      // Need to set the camera's partial path name, as this is what is being displayed
      // in the renderview's menu #4360
      MString renderCameraName = renderCamera.partialPathName();
      
      // FIXME !! why were we testing this ??? if (setDefaultCamera && !wasViewportRendering)
      // Note that we need to call this *before* CRenderSession::StartRenderView
      // because ARV_options parameter will be parsed then, and a camera will
      // eventually be set. We want this other camera to have priority over the 
      // one we're setting here.
      session->SetRenderViewOption("Add Camera", renderCameraName);
      session->SetRenderViewOption("Camera", renderCameraName);
      
      if (mode == "render_silent")
         session->GetRenderView().SetUniverse(session->GetUniverse());
      else
         session->OpenRenderView();

      // FIXME : Ensure the previous render is stopped ???

      if (is_region)
      {
         MString regionStr;
         regionStr += region[0];
         regionStr += " ";
         regionStr += region[1];
         regionStr += " ";
         regionStr += region[2];
         regionStr += " ";
         regionStr += region[3];
         session->SetRenderViewOption("Crop Region", regionStr.asChar());
      }
      session->Render(mode != "render_silent");

   } else if (mode == "stop")
   {
      CSessionManager::DeleteActiveSession(s_renderViewSessionId);
      /*
      if (!CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW))
      {
         MGlobal::displayError("Error stopping Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      // abort the rendering
      renderSession->InterruptRender();

      CMayaScene::End();

      CMayaScene::ExecuteScript(renderGlobals.postRenderMel);
      CMayaScene::ExecuteScript(renderGlobals.postMel);

      */
   }
   else if (mode == "refresh")
   {
      session->RequestUpdate();

      /*
      if (!CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW))
      {
         MGlobal::displayError("Error refreshing Arnold Render View, Arnold Render session is not active.");
         return MS::kFailure;
      }

      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      if (is_region)
         renderSession->SetRegion(region[0], region[1], region[2], region[3]);

      renderSession->UpdateRenderView();
      // only consider argument "region", ignore camera/width/height, etc...
      */
   }  

   return status;
}
