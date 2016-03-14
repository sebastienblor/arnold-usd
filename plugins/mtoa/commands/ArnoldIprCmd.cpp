
#include "ArnoldIprCmd.h"
#include "scene/MayaScene.h"
#include "render/MaterialView.h"

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

MStatus CArnoldIprCmd::doIt(const MArgList& argList)
{

   // IPR only works in interactive GUI mode
   if (!MRenderView::doesRenderEditorExist())
      return MS::kFailure;

   MStatus status;
   MArgDatabase args(syntax(), argList);

   // "-mode" flag is not set, so we simply return a bool with the rendering status
   if (!args.isFlagSet("mode"))
   {
      // Note that we return false if the scene is active with a different type of render
      // (MTOA_SESSION_RENDER is currently the only other that runs on a separate thread).
      // This is intentional: returning true here for non-IPR render would result in a follow-up
      // call to `arnoldIpr -stop` which is specialized for IPR (specifically it calls post render
      // MEL scripts, which are handled in a different way by interrupted non-IPR renders).
      setResult(CMayaScene::IsActive(MTOA_SESSION_IPR));
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
      // Just incase we were rendering already.
      CMayaScene::End();

      CMayaScene::ExecuteScript(renderGlobals.preMel);
      CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

      CMayaScene::Begin(MTOA_SESSION_IPR);
      MSelectionList sel;
      args.getFlagArgument("camera", 0, sel);
      MDagPath camera;
      status = sel.getDagPath(0, camera);
      CMayaScene::GetArnoldSession()->SetExportCamera(camera);

      if (!renderGlobals.renderAll)
      {
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
      MStringArray allPanelNames;
      MGlobal::executeCommand("getPanel -scriptType renderWindowPanel", allPanelNames);
      if (allPanelNames.length()>0) CMayaScene::GetRenderSession()->SetRenderViewPanelName(allPanelNames[0]);

      // No need to call render as Maya sends us "unpause" next.

   }

   else if (mode == "stop")
   {
      if (!CMayaScene::IsActive(MTOA_SESSION_IPR))
      {
         MGlobal::displayError("Error stopping Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }

      CMayaScene::End();

      CMayaScene::ExecuteScript(renderGlobals.postRenderMel);
      CMayaScene::ExecuteScript(renderGlobals.postMel);
   }

   else if (mode == "refresh")
   {
      if (!CMayaScene::IsActive(MTOA_SESSION_IPR))
      {
         MGlobal::displayError("Error refreshing Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      // FIXME: why do all this resetting? shouldn't the camera and resolution still exist on the previous sessions?

      // We save and restore the res instead of using the translated one because
      // the translated value is from the render globals. We may have been
      // passed in a different value to start with.
      const int width = CMayaScene::GetRenderSession()->RenderOptions()->width();
      const int height = CMayaScene::GetRenderSession()->RenderOptions()->height();
      // Same deal for the camera.
      MDagPath camera = CMayaScene::GetRenderSession()->GetCamera();
      MString panel = CMayaScene::GetRenderSession()->GetRenderViewPanelName();
      AiMsgDebug ("[mtoa] IPR refresh using last rendered camera '%s' in panel '%s'",
                  camera.partialPathName().asChar(), panel.asChar());

      // End and restart a new render session re-using saved resolution and camera
      CMayaScene::End();
      CMayaScene::Begin(MTOA_SESSION_IPR);

      CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
      CRenderSession* renderSession = CMayaScene::GetRenderSession();

      arnoldSession->SetExportCamera(camera);
      if (!renderGlobals.renderAll)
      {
         MGlobal::getActiveSelectionList(selected);
         CMayaScene::Export(&selected);
      }
      else
      {
         CMayaScene::Export();
      }

      // Set resolution and camera as passed in.
      renderSession->SetResolution(width, height);
      renderSession->SetCamera(camera);
      renderSession->SetRenderViewPanelName(panel);

      // Start off the render.
      renderSession->DoIPRRender();
   }

   else if ((mode == "region") || (mode == "render"))
   {
      if (!CMayaScene::IsActive())
      {
         MGlobal::displayError("Error rendering Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }

      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      renderSession->InterruptRender();

      if (mode == "region")
      {
         unsigned int left(-1);
         unsigned int right(-1);
         unsigned int bottom(-1);
         unsigned int top(-1);

         status = MRenderView::getRenderRegion(left, right, bottom, top);
         if (status == MS::kSuccess)
         {
            renderSession->SetRegion(left, right, bottom, top);
         }
      }
      else
      {
         renderSession->SetResolution(width, height);
         MSelectionList sel;
         args.getFlagArgument("camera", 0, sel);
         MDagPath camera;
         status = sel.getDagPath(0, camera);
         // Set the render session camera.
         renderSession->SetCamera(camera);
         MStringArray allPanelNames;
         MGlobal::executeCommand("getPanel -scriptType renderWindowPanel", allPanelNames);
         if (allPanelNames.length() > 0) renderSession->SetRenderViewPanelName(allPanelNames[0]);
      }
      // Start off the render.
      renderSession->DoIPRRender();
   }
   else if ((mode == "pause"))
   {
      if (!CMayaScene::IsActive())
      {
         MGlobal::displayError("Error pausing Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      CMayaScene::GetRenderSession()->PauseIPR();
   }
   else if (mode == "unpause")
   {
      if (!CMayaScene::IsActive())
      {
         MGlobal::displayError("Error unpausing Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }
      // Start off the render.
      CMayaScene::GetRenderSession()->UnPauseIPR();
   }
   else if ((mode == "finishedIPR"))
   {
      if (!CMayaScene::IsActive())
      {
         MGlobal::displayError("Error finishing Arnold IPR, Arnold Render session is not active.");
         return MS::kFailure;
      }

      // We might be rendering again by the time this is called.
      if (AiRendering()) return status;

      CMayaScene::End();
   }
   
   return status;
}
