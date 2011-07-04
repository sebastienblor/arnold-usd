
#include "ArnoldIprCmd.h"
#include "render/RenderSession.h"

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
   syntax.addFlag("et", "elapsedTime", MSyntax::kString);
   syntax.addFlag("si", "samplingInfo", MSyntax::kString);

   return syntax;
}

MStatus CArnoldIprCmd::doIt(const MArgList& argList)
{

   // IPR only works in interactive GUI mode
   if (!MRenderView::doesRenderEditorExist())
      return MS::kFailure;

   CRenderSession* renderSession = CRenderSession::GetInstance();

   MStatus status;
   MArgDatabase args(syntax(), argList);

   // "-mode" flag is not set, so we simply return a bool with the rendering status
   if (!args.isFlagSet("mode"))
   {
      setResult(renderSession->IsActive());
      return MS::kSuccess;
   }

   // Get argument to "-mode" flag
   MString mode = args.flagArgumentString("mode", 0);
   int width  = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
   int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;

   // TODO: get the "selected" flag here
   CExportOptions exportOptions;
   exportOptions.mode = MTOA_EXPORT_IPR;

   // What mode are we in?
   if (mode == "start")
   {
      MSelectionList sel;
      args.getFlagArgument("camera", 0, sel);
      status = sel.getDagPath(0, exportOptions.camera);

      // Just incase we were rendering already.
      renderSession->Finish();

      MCommonRenderSettingsData renderGlobals;
      MRenderUtil::getCommonRenderSettings(renderGlobals);
      exportOptions.filter.unselected = !renderGlobals.renderAll;

      renderSession->ExecuteScript(renderGlobals.preMel);
      renderSession->ExecuteScript(renderGlobals.preRenderMel);

      // This will export the scene.
      renderSession->Translate(exportOptions);

      // Set resolution and camera as passed in.
      renderSession->SetResolution(width, height);

      // No need to call render as Maya sends us "unpause" next.

   }

   else if (mode == "stop")
   {
      renderSession->Finish();

      MCommonRenderSettingsData renderGlobals;
      MRenderUtil::getCommonRenderSettings(renderGlobals);
      renderSession->ExecuteScript(renderGlobals.postRenderMel);
      renderSession->ExecuteScript(renderGlobals.postMel);
   }

   else if (mode == "refresh")
   {
      // Close down Arnold, clearing out the old data.
      renderSession->Finish();
      // We save and restore the res instead of using the translated one because
      // the translated value is from the render globals. We may have been
      // passed in a different value to start with.
      const AtInt width = renderSession->RenderOptions()->width();
      const AtInt height = renderSession->RenderOptions()->height();
      // Same deal for the camera.
      exportOptions.camera = renderSession->RenderOptions()->GetCamera();
      // Set the export mode to IPR
      // FIXME: do we really need to reset options and do a full translation each time?
      // Re-translate.
      renderSession->Translate(exportOptions);
      // Restore the resolution and camera.
      renderSession->SetResolution(width, height);

      // Start off the render.
      renderSession->DoIPRRender();
   }

   else if ((mode == "region") || (mode == "render"))
   {
      renderSession->InterruptRender();

      if (!renderSession->IsActive())
      {
         MGlobal::displayError("Error rendering Arnold IPR, Arnold is not active.");
         return MS::kFailure;
      }

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
      }
      MSelectionList sel;
      args.getFlagArgument("camera", 0, sel);
      MDagPath camera;
      status = sel.getDagPath(0, camera);
      // Set the render session camera.
      renderSession->SetCamera(camera);

      // Start off the render.
      renderSession->DoIPRRender();
   }
   else if ((mode == "pause"))
   {
      renderSession->PauseIPR();
   }

   else if (mode == "unpause")
   {
      if (!renderSession->IsActive())
      {
         MGlobal::displayError("Error starting Arnold IPR");
         return MS::kFailure;
      }

      // Start off the render.
      renderSession->UnPauseIPR();
   }

   else if ((mode == "finishedIPR"))
   {
      // We might be rendering again by the time this is called.
      if (AiRendering()) return status;
         
      renderSession->FinishedIPRTuning();

      // Format a bit of info for the renderview.
      const AtUInt64 mem_used(renderSession->GetUsedMemory());

      MString rvInfo("renderWindowEditor -edit -pcaption (\"    (Arnold Renderer)\\n");
      rvInfo += "Memory: ";
      rvInfo += (unsigned int)mem_used;
      rvInfo += "Mb";

      if (args.isFlagSet("samplingInfo"))
      {
         const MString samplingInfo(args.flagArgumentString("samplingInfo", 0));
         rvInfo += "    Sampling: ";
         rvInfo += samplingInfo;
      }

      if (args.isFlagSet("elapsedTime"))
      {
         const MString elapsedTime(args.flagArgumentString("elapsedTime", 0));
         rvInfo += "    Render Time: ";
         rvInfo += elapsedTime;
      }

      rvInfo += "    Camera: ";
      rvInfo += renderSession->RenderOptions()->GetCameraName();
      rvInfo += "\") renderView;";

      MGlobal::executeCommandOnIdle(rvInfo, false);
   }

   return status;
}
