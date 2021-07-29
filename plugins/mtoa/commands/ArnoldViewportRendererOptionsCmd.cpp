#include "ArnoldViewportRendererOptionsCmd.h"
#include "session/ArnoldRenderViewSession.h"
#include "session/SessionManager.h"

#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>

static std::string s_arnoldViewportSession("arnoldViewport");


MSyntax CArnoldViewportRendererOptionsCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("m", "mode", MSyntax::kString);
   return syntax;
}

static bool s_wasVisible = false;

MStatus CArnoldViewportRendererOptionsCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList);

   MGlobal::executePythonCommand("import mtoa.core;mtoa.core.createOptions()");  
   MString mode = (args.isFlagSet("mode")) ? args.flagArgumentString("mode", 0) : "open";

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

   CArnoldRenderViewSession *session = (CArnoldRenderViewSession *)CSessionManager::FindActiveSession(s_arnoldViewportSession);   
   bool sessionExisted = (session != nullptr);
   if (session == nullptr)
   {
      s_wasVisible = true;
      
      session = new CArnoldRenderViewSession(true);
      CSessionManager::AddActiveSession(s_arnoldViewportSession, session);
      CRenderViewMtoA &renderView = session->GetRenderView();
      renderView.OpenMtoAViewportRendererOptions();
      renderView.RequestFullSceneUpdate();
      s_wasVisible = false;
      renderView.CloseOptionsWindow();
      CSessionManager::DeleteActiveSession(s_arnoldViewportSession);
      return MS::kSuccess;
   }
   
   CRenderViewMtoA &renderView = session->GetRenderView();
      
   if (mode == "open")
   {
      s_wasVisible = true;
      renderView.SetViewportRendering(true);
      renderView.OpenMtoAViewportRendererOptions();
      renderView.RequestFullSceneUpdate();
      
      if (!sessionExisted)
      {
         s_wasVisible = false;
         renderView.CloseOptionsWindow();
         //MGlobal::executeCommand("workspaceControl -edit -cl \"ArnoldViewportRendererOptions\"");      
      }

   } else if (mode == "close")
   {  
      s_wasVisible = false;        
      renderView.CloseOptionsWindow();
   }

   return MS::kSuccess;
}

