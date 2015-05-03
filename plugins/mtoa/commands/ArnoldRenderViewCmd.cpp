
#include "ArnoldRenderViewCmd.h"
#include "scene/MayaScene.h"

#include <ai_universe.h>

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnDagNode.h>
#include <maya/MRenderView.h>
#include <maya/MRenderUtil.h>

#include <vector>

#include "../render/display/renderview.h"


MSyntax CArnoldRenderViewCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("cam", "camera", MSyntax::kSelectionItem);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);
   syntax.addFlag("m", "mode", MSyntax::kString);
   syntax.addFlag("r", "region", MSyntax::kUnsigned, MSyntax::kUnsigned, MSyntax::kUnsigned, MSyntax::kUnsigned);
   syntax.addFlag("ar", "refresh", MSyntax::kBoolean);

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
   if (mode == "render")
   {
      MSelectionList sel;
      args.getFlagArgument("camera", 0, sel);
      MDagPath camera;
      status = sel.getDagPath(0, camera);

      startRenderView(camera, width, height);

      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      renderSession->InterruptRender();

      if (!CMayaScene::IsActive())
      {
         MGlobal::displayError("Error rendering Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      renderSession->SetResolution(width, height);
      // Set the render session camera.
      renderSession->SetCamera(camera);

      if (is_region)
         renderSession->SetRegion(region[0], region[1], region[2], region[3]);

      bool auto_refresh = args.isFlagSet("refresh") ? args.flagArgumentBool("refresh", 0) : true;

      CMayaScene::GetArnoldSession()->allowUpdates(auto_refresh);
      // Start off the render.
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
   CMayaScene::End();
   
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   CMayaScene::ExecuteScript(renderGlobals.preMel);
   CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

   CMayaScene::Begin(MTOA_SESSION_RENDERVIEW);
   CMayaScene::GetArnoldSession()->SetExportCamera(camera);


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

   // Set resolution and camera as passed in.
   CMayaScene::GetRenderSession()->SetResolution(width, height);
   CMayaScene::GetRenderSession()->SetCamera(camera);
  
   CMayaScene::GetRenderSession()->StartRenderView();


}
