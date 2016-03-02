/*
#ifndef _WIN64
#include "../render/display/renderview.h"
#endif
*/
#include "ArnoldRenderViewCmd.h"
#include "scene/MayaScene.h"
#include "render/MaterialView.h"

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



// Return all renderable cameras
static int GetRenderCameras(MDagPathArray &cameras)
{

   M3dView view;
   MDagPath activeCameraPath;
   MStatus viewStatus;
   view = M3dView::active3dView(&viewStatus);
   if (viewStatus == MS::kSuccess && view.getCamera(activeCameraPath) == MS::kSuccess)
   {
      cameras.append(activeCameraPath);
      return 1;
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
      {
         cameras.append(cameraPath);
      }
      dagIter.next();
   }

   int size = cameras.length();
   if (size > 1)
      MGlobal::displayWarning("More than one renderable camera. (use the -cam/-camera option to override)");
   else if (!size)
      MGlobal::displayWarning("Did not find a renderable camera. (use the -cam/-camera option to specify one)");
   return size;
}

MSyntax CArnoldRenderViewCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("cam", "camera", MSyntax::kSelectionItem);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);
   syntax.addFlag("m", "mode", MSyntax::kString);
   syntax.addFlag("r", "region", MSyntax::kUnsigned, MSyntax::kUnsigned, MSyntax::kUnsigned, MSyntax::kUnsigned);

   return syntax;
}

MStatus CArnoldRenderViewCmd::doIt(const MArgList& argList)
{
   // IPR only works in interactive GUI mode
   //if (!MRenderView::doesRenderEditorExist())
   //   return MS::kFailure;
   MStatus status;
   MArgDatabase args(syntax(), argList);

   // Get argument to "-mode" flag
   MString mode = (args.isFlagSet("mode")) ? args.flagArgumentString("mode", 0) : "render";
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
      if (CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW))
      {
         // A render view session has already been started
         // let's pop-up the window, and eventually re-render
         CMayaScene::GetRenderSession()->StartRenderView();
         return MS::kSuccess;
      }

      // Make sure no material view session is active
      CMaterialView::SuspendRenderer();

      MDagPathArray cameras;
      if (args.isFlagSet("camera"))
      {
         MSelectionList sel;
         args.getFlagArgument("camera", 0, sel);
         MDagPath camera;
         status = sel.getDagPath(0, camera);

         if (camera.isValid())
         {
            cameras.append(camera);
         }
      }      
      GetRenderCameras(cameras);      
      startRenderView(cameras[0], width, height);

      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      renderSession->InterruptRender();

      if (!CMayaScene::IsActive())
      {
         MGlobal::displayError("Error rendering Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      renderSession->SetResolution(width, height);
      // Set the render session camera.
      renderSession->SetCamera(cameras[0]);

      if (is_region)
         renderSession->SetRegion(region[0], region[1], region[2], region[3]);

      // Start off the render.
      // Unless we are in "open" mode
      if (mode == "render") renderSession->RunRenderView();
   }
   else if (mode == "stop")
   {
      if (!CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW))
      {
         MGlobal::displayError("Error stopping Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      // abort the rendering
      renderSession->InterruptRender();

      CMayaScene::ExecuteScript(renderGlobals.postRenderMel);
      CMayaScene::ExecuteScript(renderGlobals.postMel);

      // Resume material view session
      CMaterialView::ResumeRenderer();
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

   // SetExportCamera mus be called AFTER CMayaScene::Export
   CMayaScene::GetArnoldSession()->SetExportCamera(camera);

   // Set resolution and camera as passed in.
   CMayaScene::GetRenderSession()->SetResolution(width, height);
   CMayaScene::GetRenderSession()->SetCamera(camera);

   CMayaScene::GetRenderSession()->StartRenderView();


}
