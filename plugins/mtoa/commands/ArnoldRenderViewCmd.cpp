/*
#ifndef _WIN64
#include "../render/display/renderview.h"
#endif
*/
#include "ArnoldRenderViewCmd.h"
#include "scene/MayaScene.h"
#include "translators/DagTranslator.h"

#include <ai_universe.h>

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnDagNode.h>
#include <maya/MRenderView.h>
#include <maya/MRenderUtil.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/M3dView.h>
#include <vector>

/*
#ifdef _WIN64
#include "../render/display/renderview.h"
#endif
*/
static bool s_wasVisible = false;
static bool s_arvExists = false;

// Return the default camera
static MDagPath GetDefaultCamera()
{
   M3dView view;
   MDagPath activeCameraPath;
   MStatus viewStatus;
   view = M3dView::active3dView(&viewStatus);
   if (viewStatus == MS::kSuccess && view.getCamera(activeCameraPath) == MS::kSuccess)
   {
      return activeCameraPath;
   }

   MItDag dagIter(MItDag::kDepthFirst, MFn::kCamera);
   MDagPath cameraPath;
   // MFnCamera cameraNode;
   MFnDagNode cameraNode;
   MPlug renderable;
   MStatus stat;
   while (!dagIter.isDone())
   {
      dagIter.getPath(cameraPath);
      cameraNode.setObject(cameraPath);
      renderable = cameraNode.findPlug("renderable", false, &stat);
      if (stat && renderable.asBool())
         return cameraPath;

      dagIter.next();
   }
   return MDagPath();

//   MGlobal::displayWarning("No renderable camera was found");
}

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

   MGlobal::executePythonCommand("import mtoa.core;mtoa.core.createOptions()");   

   MString mode = (args.isFlagSet("mode")) ? args.flagArgumentString("mode", 0) : "render";

   CRenderSession* renderSession = CMayaScene::GetRenderSession();

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

   if (mode == "close")
   {  
      s_wasVisible = false;        
      if (renderSession)
          renderSession->CloseRenderView();
      return MS::kSuccess;
   }
   
   if (args.isFlagSet("option"))
   {
      MString option = args.flagArgumentString("option", 0);
      MString value = args.flagArgumentString("option", 1);
      CRenderSession::SetRenderViewOption(option, value);
      return MS::kSuccess;
   }
   if (args.isFlagSet("getoption"))
   {
      MString option = args.flagArgumentString("get", 0);
      setResult(CRenderSession::GetRenderViewOption(option));
      return MS::kSuccess;
   }

   // Get argument to "-mode" flag
   
   int width  = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
   int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;
   bool is_region = args.isFlagSet("region");
   int region[4];
   if (is_region)
   {
      for (int i = 0; i < 4; ++i)
         region[i] = args.flagArgumentInt("region", i);
   }

   MSelectionList selected;
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);


   // What mode are we in?
   if (mode == "render" || mode == "open")
   {
       // Close any viewports renderers before opening the ARV.

       CRenderSession::CloseOtherViews("");

      s_wasVisible = true;

      if (CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW))
      {
         // A render view session has already been started
         // let's pop-up the window, and eventually re-render
         CMayaScene::GetRenderSession()->StartRenderView();
         
         if (mode == "render")
            CMayaScene::GetRenderSession()->SetRenderViewOption("Run IPR", "1");
         return MS::kSuccess;
      }
      if (mode == "open")
      {
         CMayaScene::End();
         CMayaScene::Begin(MTOA_SESSION_RENDERVIEW);
         CRenderSession *renderSession = CMayaScene::GetRenderSession();
         renderSession->UpdateRenderOptions();
         renderSession->RenderOptions()->UpdateImageDimensions();
         renderSession->FillRenderViewCameras();
         
         renderSession->StartRenderView();
         s_arvExists = true;
         renderSession->SetRenderViewOption("Run IPR", "0");
// (why were we calling this function ?) renderSession->SetRenderViewOption("Full IPR Update", "1");



		 renderSession->CloseRenderViewWithSession(false); // but DON't close ARV with CMayaScene::End()
         CMayaScene::End();
		 return MS::kSuccess;
      }
      MDagPath defaultCamera;

      // First check if a camera is specified
      if (args.isFlagSet("camera"))
      {
         MSelectionList sel;
         args.getFlagArgument("camera", 0, sel);
         MDagPath camera;
         status = sel.getDagPath(0, camera);

         if (camera.isValid())
            defaultCamera = camera;
      }

      // if not, get the viewport camera, and otherwise the first one found
      if (!defaultCamera.isValid())
         defaultCamera = GetDefaultCamera();
      
      startRenderView(defaultCamera, width, height);

      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      renderSession->InterruptRender();

      if (!CMayaScene::IsActive())
      {
         MGlobal::displayError("Error rendering Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      renderSession->SetResolution(width, height);

      if (is_region)
      {
         renderSession->SetRegion(region[0], region[2], region[1], region[3]);
         CRenderSession* renderSession = CMayaScene::GetRenderSession();
         MString regionStr;
         regionStr += region[0];
         regionStr += " ";
         regionStr += region[1];
         regionStr += " ";
         regionStr += region[2];
         regionStr += " ";
         regionStr += region[3];
         renderSession->SetRenderViewOption("Crop Region", regionStr.asChar());
      }

      // Start off the render.
      // Unless we are in "open" mode
      renderSession->RunRenderView();
   } else if (mode == "stop")
   {
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

   }
   else if (mode == "refresh")
   {
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
   }  
   
   return status;
}


void CArnoldRenderViewCmd::startRenderView(const MDagPath &camera, int width, int height)
{   
   // only set the default (view) camera when we run ARV
   // for the first time
   bool setDefaultCamera = !s_arvExists;

   bool wasViewportRendering = CRenderSession::IsViewportRendering();

   s_arvExists = true;
   CMayaScene::End();
   
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   CMayaScene::ExecuteScript(renderGlobals.preMel);
   CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

   CMayaScene::Begin(MTOA_SESSION_RENDERVIEW);
   
   if (!renderGlobals.renderAll)
   {
      MSelectionList selected;
      MGlobal::getActiveSelectionList(selected);
      CMayaScene::Export(&selected);
   }
   else
   {
      CMayaScene::Export();
   }
   CRenderSession *renderSession = CMayaScene::GetRenderSession();

   // Set the render session camera.
   renderSession->SetCamera(camera);
   MString renderCamera = CDagTranslator::GetArnoldNaming(camera);
   
   if (setDefaultCamera && !wasViewportRendering)
   {
      // Note that we need to call this *before* CRenderSession::StartRenderView
      // because ARV_options parameter will be parsed then, and a camera will
      // eventually be set. We want this other camera to have priority over the 
      // one we're setting here.
      renderSession->SetRenderViewOption("Camera", renderCamera);
   }
   
   // SetExportCamera mus be called AFTER CMayaScene::Export
   CMayaScene::GetArnoldSession()->SetExportCamera(camera);
   
   // Set resolution and camera as passed in.
   renderSession->SetResolution(width, height);
   CMayaScene::GetRenderSession()->StartRenderView();

   // If we were viewport-rendering, previous calls to SetRenderViewOption will land in the viewport
   // so we must set this now that we create ARV
   if (wasViewportRendering)
      renderSession->SetRenderViewOption("Camera", renderCamera);  


}
