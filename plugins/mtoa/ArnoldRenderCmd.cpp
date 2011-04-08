#include "ArnoldRenderCmd.h"
#include "render/RenderOptions.h"
#include "render/RenderSession.h"

#include <ai_msg.h>
#include <ai_universe.h>

#include <maya/M3dView.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderView.h>
#include <maya/MStringArray.h>
#include <maya/MItDag.h>
#include <maya/MPlug.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MRenderUtil.h>
#include <maya/MFileIO.h>

MSyntax CArnoldRenderCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("b", "batch", MSyntax::kNoArg);
   syntax.addFlag("cam", "camera", MSyntax::kString);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);

   return syntax;
}

MStatus CArnoldRenderCmd::doIt(const MArgList& argList)
{
   MStatus status;
   CRenderSession* renderSession = CRenderSession::GetInstance();
   MArgDatabase args(syntax(), argList);
   MDagPath dagPath;

   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   const bool batch = args.isFlagSet("batch") ? true : false;
   renderSession->SetBatch(batch);

   // no camera set on interactive mode, abort
   if (!args.isFlagSet("camera") && !batch)
   {
      return MS::kFailure;
   }
   // TODO: get the "selected" flag here
   ExportOptions exportOptions;
   exportOptions.mode = MTOA_EXPORT_ALL;
   exportOptions.filter.unselected = !renderGlobals.renderAll;

   // FIXME: just a fast hack, should rehaul CRenderOptions code
   // and share same proc for ArnoldRenderCmd and ArnoldExportAssCmd
   short renderType = 0;
   bool outputAssBoundingBox = false;
   MSelectionList list;
   MObject node;
   list.add("defaultArnoldRenderOptions");
   if (list.length() > 0)
   {
      list.getDependNode(0, node);
      MFnDependencyNode fnArnoldRenderOptions(node);
      renderType = fnArnoldRenderOptions.findPlug("renderType").asShort();
      outputAssBoundingBox = fnArnoldRenderOptions.findPlug("outputAssBoundingBox").asBool();
   }

   if (renderType != MTOA_RENDER_INTERACTIVE)
   {
      MString filename;
      filename = renderGlobals.name;
      if (filename != "")
      {
         MString curProject = MGlobal::executeCommandStringResult("workspace -q -o");
         if (curProject != "")
         {
            MString dirProject = MGlobal::executeCommandStringResult("workspace -q -rd "
                  + curProject);
            MString assDir = MGlobal::executeCommandStringResult(
                  "workspace -q -fileRuleEntry ArnoldSceneSource");
            filename = dirProject + "/" + assDir + "/" + filename + ".ass";
         }
         else
         {
            MString curDir = MGlobal::executeCommandStringResult("workspace -q -dir");
            filename = curDir + "/" + filename + ".ass";
         }
      }
      else
      {
         // If all else fails, use the current Maya scene + ass
         filename = MFileIO::currentFile() + ".ass";
      }

      MString cmdStr = "arnoldExportAss";
      cmdStr += " -f \"" + filename + "\"";

      if (exportOptions.filter.unselected)
      {
         cmdStr += " -s";
      }

      if (outputAssBoundingBox)
      {
         cmdStr += " -bb";
      }

      if (renderGlobals.isAnimated())
      {
         AtFloat startframe = static_cast<float> (renderGlobals.frameStart.as(MTime::uiUnit()));
         AtFloat endframe = static_cast<float> (renderGlobals.frameEnd.as(MTime::uiUnit()));
         AtFloat byframestep = renderGlobals.frameBy;
         cmdStr += " -sf ";
         cmdStr += startframe;
         cmdStr += " -ef ";
         cmdStr += endframe;
         cmdStr += " -fs ";
         cmdStr += byframestep;
      }

      MString camera = args.flagArgumentString("camera", 0);
      if (camera != "")
      {
         cmdStr += " -cam " + camera;
      }

      status = MGlobal::executeCommand(cmdStr);
      if (MStatus::kSuccess == status)
      {
         MGlobal::displayInfo("[mtoa] Exported scene to file " + filename);
         if (renderType == MTOA_RENDER_EXPORTASS_AND_KICK)
         {
#ifdef _WIN32
            MString cmd = "kick " + filename;
#else
            MString cmd = "kick " + filename + " &";
#endif
            int ret = system(cmd.asChar());

            MGlobal::displayInfo("[mtoa] Value returned by kick : " + ret);
         }
      }
      else
      {
         MGlobal::displayError("[mtoa] Failed to export scene to file " + filename);
      }

      return status;
   }

   // Note: Maya seems to internally calls the preRender preLayerRender scripts
   //       as well as the postRender and postLayerRender ones

   // Check if in batch mode
   if (batch)
   {
      // TODO: This really needs to go. We're translating the whole scene for a couple of
      // render options.

      AtFloat startframe;
      AtFloat endframe;
      AtFloat byframestep;

      if (renderGlobals.isAnimated())
      {
         startframe = static_cast<float> (renderGlobals.frameStart.as(MTime::uiUnit()));
         endframe = static_cast<float> (renderGlobals.frameEnd.as(MTime::uiUnit()));
         byframestep = renderGlobals.frameBy;
         // in case startFrame == endFrame, we
      }
      else
      {
         startframe = 0;
         endframe = 0;
         byframestep = 1;
      }

      //FIXME: in command line mode, seems that maya doesn't move to the first frame correctly.
      MGlobal::viewFrame((double) startframe);

      for (AtFloat framerender = startframe; framerender <= endframe; framerender += byframestep)
      {
         const CRenderOptions* renderOptions = renderSession->RenderOptions();
         if (renderOptions->isAnimated())
            MGlobal::viewFrame((double) framerender);
         renderSession->ExecuteScript(renderGlobals.preRenderMel);

         // FIXME: do we really need to reset options each time?
         renderSession->Translate(exportOptions);

         MStringArray cameras;
         MItDag dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);

         // get all renderable cameras
         for (dagIterCameras.reset(); (!dagIterCameras.isDone()); dagIterCameras.next())
         {
            if (!dagIterCameras.getPath(dagPath))
            {
               AiMsgError("[mtoa] ERROR: Could not get path for DAG iterator.");
               return status;
            }

            MFnDependencyNode camDag(dagIterCameras.item());
            if (camDag.findPlug("renderable").asBool())
            {
               MFnDagNode cameraNode(dagPath);
               cameras.append(cameraNode.name().asChar());
            }
         }

         if (cameras.length() > 1)
         {
            renderSession->SetMultiCameraRender(true);
         }

         for (unsigned int arrayIter = 0; (arrayIter < cameras.length()); arrayIter++)
         {
            renderSession->SetCamera(cameras[arrayIter]);
            renderSession->DoBatchRender();
         }

         renderSession->Finish();
         renderSession->ExecuteScript(renderGlobals.postRenderMel);
      }
   }

   // or interactive mode
   else
   {
      int width = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
      int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;
      MString camera = args.flagArgumentString("camera", 0);

      renderSession->ExecuteScript(renderGlobals.preRenderMel);

      renderSession->Finish(); // In case we're already rendering (e.g. IPR).
      renderSession->Translate(exportOptions); // Translate the scene from Maya.
      renderSession->SetCamera(camera);
      renderSession->SetResolution(width, height);
      renderSession->DoInteractiveRender(); // Start the render.
      renderSession->Finish(); // Clean up.

      renderSession->ExecuteScript(renderGlobals.postRenderMel);
   }

   return status;
}
