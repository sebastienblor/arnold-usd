#include "ArnoldViewportRendererOptionsCmd.h"
#include "session/ArnoldRenderViewSession.h"
#include "session/SessionManager.h"
#include "viewport2/ArnoldViewOverride.h"

#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>


MSyntax CArnoldViewportRendererOptionsCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("m", "mode", MSyntax::kString);
   syntax.addFlag("opt", "option", MSyntax::kString, MSyntax::kString);
   syntax.addFlag("get", "getoption", MSyntax::kString);
   syntax.addFlag("aov", "currentaov", MSyntax::kLong);
   syntax.addFlag("st", "status", MSyntax::kString);
   syntax.addFlag("hud", "toggleHUD", MSyntax::kBoolean);
   syntax.makeFlagQueryWithFullArgs("option", true);
   syntax.enableQuery( true );
   return syntax;
}

static bool s_wasVisible = false;

MStatus CArnoldViewportRendererOptionsCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList);

   MString mode = (args.isFlagSet("mode")) ? args.flagArgumentString("mode", 0) : "open";

   CArnoldRenderViewSession *session = (CArnoldRenderViewSession *)CSessionManager::FindActiveSession(CArnoldRenderViewSession::GetViewportSessionId());   
   bool sessionExisted (session != nullptr);
   if (session == nullptr)
   {
      session = new CArnoldRenderViewSession(true);
      CSessionManager::AddActiveSession(CArnoldRenderViewSession::GetViewportSessionId(), session);
   }
   
   CRenderViewMtoA &renderView = session->GetRenderView();

   if (args.isQuery())
   {
      // For option / getoption, we don't need to keep the session alive, so we'll delete it before we return
      if (args.isFlagSet("option"))
      {
         MString option;
         status = args.getFlagArgument("option", 0, option);
         setResult(session->GetRenderViewOption(option));
         if (!sessionExisted)
            CSessionManager::DeleteActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
         return status;
      }
   }
   else
   {
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
         MGlobal::executeCommand("workspaceControl -q -vis \"ArnoldViewportRendererOptions\"", workspaceVisible);
         bool isVisible = (workspaceVisible > 0);

         if (isVisible == s_wasVisible) 
            return MS::kSuccess;

         if (isVisible)
            mode = "open"; // turning visibility ON, just open ARV
         else
            mode = "close"; // closing ARV
      }
      if (mode == "visChanged_cb")
      {
         MGlobal::executeCommandOnIdle("arnoldViewport -mode visChanged");
         return MS::kSuccess;
      }
         
      // For option / getoption, we don't need to keep the session alive, so we'll delete it before we return
      if (args.isFlagSet("option"))
      {
         MString option = args.flagArgumentString("option", 0);
         MString value = args.flagArgumentString("option", 1);
         session->SetRenderViewOption(option, value);
         if (!sessionExisted)
            CSessionManager::DeleteActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
         return MS::kSuccess;
      }
      
      if (args.isFlagSet("getoption"))
      {
         MString option = args.flagArgumentString("get", 0);
         setResult(session->GetRenderViewOption(option));
         if (!sessionExisted)
            CSessionManager::DeleteActiveSession(CArnoldRenderViewSession::GetViewportSessionId());
         return MS::kSuccess;
      }

      if (mode == "open")
      {
         s_wasVisible = true;
         renderView.SetViewportRendering(true);
         renderView.OpenMtoAViewportRendererOptions();
         if (!sessionExisted)
            renderView.RequestFullSceneUpdate();
         renderView.CloseOptionsWindow();
      
      } else if (mode == "close")
      {  
         s_wasVisible = false;        
         renderView.CloseOptionsWindow();
      }

      if (args.isFlagSet("toggleHUD"))
      {
         bool toggleHUD = args.flagArgumentBool("toggleHUD", 0);
         MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer(); 
         ArnoldViewOverride *renderOverride = NULL;
         if (renderer)
         {
            renderOverride = (ArnoldViewOverride *) renderer->findRenderOverride( "arnoldViewOverride" );
         }
         if (!renderOverride)
            return MStatus::kFailure;

         renderOverride->getHUDRenderer()->setUserUIDrawables(toggleHUD);
      }
   }

   return MS::kSuccess;
}
