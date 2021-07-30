
#include "ArnoldIprCmd.h"
#include "render/OutputDriver.h"
#include "session/ArnoldRenderSession.h"
#include "session/SessionManager.h"
#include "utils/MtoaLog.h"
#include <ai_universe.h>

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnDagNode.h>
#include <maya/MRenderView.h>
#include <maya/MRenderUtil.h>

#include <vector>


MSyntax CArnoldIprCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("n", "node", MSyntax::kString);
   syntax.addFlag("cam", "camera", MSyntax::kSelectionItem);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);
   syntax.addFlag("m", "mode", MSyntax::kString);

   return syntax;
}

static std::string s_arnoldIprSessionId("arnoldIPR");


MStatus CArnoldIprCmd::doIt(const MArgList& argList)
{
  MStatus status;
   // IPR only works in interactive GUI mode
   if (!MRenderView::doesRenderEditorExist())
      return MS::kFailure;
 
   MArgDatabase args(syntax(), argList);
   CArnoldRenderSession *session = (CArnoldRenderSession *)CSessionManager::FindActiveSession(s_arnoldIprSessionId);

   // "-mode" flag is not set, so we simply return a bool with the rendering status
   if (!args.isFlagSet("mode"))
   {
      // Note that we return false if the scene is active with a different type of render
      // (MTOA_SESSION_RENDER is currently the only other that runs on a separate thread).
      // This is intentional: returning true here for non-IPR render would result in a follow-up
      // call to `arnoldIpr -stop` which is specialized for IPR (specifically it calls post render
      // MEL scripts, which are handled in a different way by interrupted non-IPR renders).
      setResult(session != nullptr);
      return MS::kSuccess;
   }


   // Get argument to "-mode" flag
   MString mode = args.flagArgumentString("mode", 0);
   int width  = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
   int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;

   MSelectionList selected;
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   // What mode are we in?
   if (mode == "start")
   {
      if (session == nullptr)
      {
         session = new CArnoldRenderSession();
         CSessionManager::AddActiveSession(s_arnoldIprSessionId, session);
         session->AddUpdateCallbacks();
      }// else : FIXME what to do if the session already exists ??
      else
      {
         session->Clear();
      }
      CSessionOptions &sessionOptions = session->GetOptions();

      sessionOptions.SetExportFileDrivers(false);
      
      MGlobal::executeCommand(renderGlobals.preMel);
      MGlobal::executeCommand(renderGlobals.preRenderMel);

      MSelectionList sel;
      args.getFlagArgument("camera", 0, sel);
      MDagPath camera;
      status = sel.getDagPath(0, camera);
      session->SetExportCamera(camera, false);
      sessionOptions.SetResolution(width, height);

      if (!renderGlobals.renderAll)
      {
         MGlobal::getActiveSelectionList(selected);
         session->Export(&selected);
      }
      else
      {
         session->Export();
      }

      // Set resolution and camera as passed in.
      MStringArray allPanelNames;
      MGlobal::executeCommand("getPanel -scriptType renderWindowPanel", allPanelNames);
      if (allPanelNames.length()>0) sessionOptions.SetRenderViewPanelName(allPanelNames[0]);

      // No need to call render as Maya sends us "unpause" next.

   }

   else if (mode == "stop")
   {
      if (session == nullptr)
      {
         MGlobal::displayError("Error stopping Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      MGlobal::executeCommand(renderGlobals.postRenderMel);
      MGlobal::executeCommand(renderGlobals.postMel);
      // Workaround for overriding the render view caption that Maya
      // sets after rendering is finished. Set the last caption again 
      // deferred to override Maya's caption.
      const MString& captionCmd = session->GetLastRenderViewCaptionCommand();
      if (captionCmd != "")
      {
         MGlobal::executeCommandOnIdle(captionCmd, false);
      }
      CSessionManager::DeleteActiveSession(s_arnoldIprSessionId);

   }

   else if (mode == "refresh")
   {
      if (session == nullptr)
      {
         MGlobal::displayError("Error refreshing Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      session->UpdateFullScene();
      session->IPR();
   }

   else if ((mode == "region") || (mode == "render"))
   {
      if (session == nullptr)
      {
         MGlobal::displayError("Error rendering Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      CSessionOptions &sessionOptions = session->GetOptions();
      session->InterruptRender();

      if (mode == "region")
      {
         unsigned int left(-1);
         unsigned int right(-1);
         unsigned int bottom(-1);
         unsigned int top(-1);

         status = MRenderView::getRenderRegion(left, right, bottom, top);
         if (status == MS::kSuccess)
         {
            sessionOptions.SetRegion(left, right, bottom, top);
         }
      }
      else
      {
         sessionOptions.SetResolution(width, height);
         MSelectionList sel;
         args.getFlagArgument("camera", 0, sel);
         MDagPath camera;
         status = sel.getDagPath(0, camera);
         // Set the render session camera.
         session->SetExportCamera(camera);
         MStringArray allPanelNames;
         MGlobal::executeCommand("getPanel -scriptType renderWindowPanel", allPanelNames);
         if (allPanelNames.length() > 0) sessionOptions.SetRenderViewPanelName(allPanelNames[0]);
      }
      // Start off the render.
      session->IPR();
   }
   else if ((mode == "pause"))
   {
      if (session== nullptr)
      {
         MGlobal::displayError("Error pausing Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      session->PauseIPR();
   }
   else if (mode == "unpause")
   {
      if (session == nullptr)
      {
         MGlobal::displayError("Error unpausing Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      // Start off the render.
      session->UnPauseIPR();
   }
   else if ((mode == "finishedIPR"))
   {
      if (session == nullptr)
      {
         MGlobal::displayError("Error finishing Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      // We might be rendering again by the time this is called.
      if (session->IsRendering()) return status;

      CSessionManager::DeleteActiveSession(s_arnoldIprSessionId);
   }
   return status;
}
   