#include "ArnoldRenderCmd.h"
#include "scene/MayaScene.h"

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
#include <maya/MFileObject.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MAnimControl.h>
#include <maya/MDagPathArray.h>

#include <sstream>

MSyntax CArnoldRenderCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("b", "batch", MSyntax::kNoArg);
   syntax.addFlag("cam", "camera", MSyntax::kSelectionItem);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);

   return syntax;
}

MStatus CArnoldRenderCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList);
   MDagPath dagPath;

   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   const bool batch = args.isFlagSet("batch") ? true : false;

   // Rendered camera
   MString camera = "";
   if (!args.isFlagSet("camera"))
   {
      // no camera set on interactive mode, abort
      if (!batch) return MS::kFailure;
   }
   else
   {
      camera = args.flagArgumentString("camera", 0);
   }
   // TODO: get the "selected" flag here
   bool exportSelected = !renderGlobals.renderAll;
   MSelectionList selected;
   MSelectionList* selectedPtr = NULL;
   if (exportSelected)
   {
      MGlobal::getActiveSelectionList(selected);
      selectedPtr = &selected;
   }

   int width = args.isFlagSet("width") ? args.flagArgumentInt("width", 0) : -1;
   int height = args.isFlagSet("height") ? args.flagArgumentInt("height", 0) : -1;

   // FIXME: just a fast hack, should rehaul CRenderOptions code
   // and share same proc for ArnoldRenderCmd and ArnoldExportAssCmd
   // TODO : use MString CRenderOptions::VerifyFileName(MString fileName, bool compressed)
   // code to support compressed output filename too
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
      // FIXME: actual export code should be shared so we don't have to do this dirty call
      MString cmdStr = "arnoldExportAss";
      if (batch)
      {
         cmdStr += " -b";
      }
      if (exportSelected)
      {
         cmdStr += " -s";
      }
      if (outputAssBoundingBox)
      {
         cmdStr += " -bb";
      }
      if (renderGlobals.isAnimated())
      {
         float startframe = static_cast<float> (renderGlobals.frameStart.as(MTime::uiUnit()));
         float endframe = static_cast<float> (renderGlobals.frameEnd.as(MTime::uiUnit()));
         float byframestep = renderGlobals.frameBy;
         cmdStr += " -sf ";
         cmdStr += startframe;
         cmdStr += " -ef ";
         cmdStr += endframe;
         cmdStr += " -fs ";
         cmdStr += byframestep;
      }
      if (camera != "")
      {
         cmdStr += " -cam " + camera;
      }

      MGlobal::displayInfo("[mtoa] Executing Maya command " + cmdStr);
      MStringArray assFileNames;
      status = MGlobal::executeCommand(cmdStr, assFileNames);
      unsigned int nfiles = assFileNames.length();

      if (MStatus::kSuccess == status && nfiles)
      {
         MGlobal::displayInfo("[mtoa] Exported scene to file " + assFileNames[0]);
         if (renderType == MTOA_RENDER_EXPORTASS_AND_KICK)
         {
            // TODO: will only works for single frame, batch render should be used for multiple
            // TODO: might want to remove this as it's a testing implementation and call kick from
            // post render scripts
            MString kickCmd;
            if (batch)
            {
               kickCmd = "kick -dw -dp \"" + assFileNames[0] + "\"";
            }
            else
            {
#ifdef _WIN32
               kickCmd = "Start kick \"" + assFileNames[0] + "\"";
#else
               kickCmd = "kick \"" + assFileNames[0] + "\" &";
#endif
            }
            // NOTE: must be blocking when in batch mode, non blocking when in interractive mode

            MGlobal::displayInfo("[mtoa] Calling external command " + kickCmd);
            system(kickCmd.asChar());

            // TODO : use pykick and MGlobal::executePythonCommandOnIdle to display feedback?

            // int ret = system(kickCmd.asChar());
            // std::stringstream info;
            // info << "[mtoa] Value returned by kick : " << ret;
            // AiMsgInfo(info.str().c_str());
         }
      }
      else
      {
         MGlobal::displayError("[mtoa] Failed to export scene to ass");
      }

      return status;
   }

   // Note: Maya seems to internally calls the preRender preLayerRender scripts
   //       as well as the postRender and postLayerRender ones

   CMayaScene::End(); // In case we're already rendering (e.g. IPR).

   // Check if in batch mode
   if (batch)
   {
      // TODO: This really needs to go. We're translating the whole scene for a couple of
      // render options.

      double startframe;
      double endframe;
      double byframestep;

      if (renderGlobals.isAnimated())
      {
         startframe = renderGlobals.frameStart.as(MTime::uiUnit());
         endframe = renderGlobals.frameEnd.as(MTime::uiUnit());
         byframestep = renderGlobals.frameBy;

         MGlobal::viewFrame(startframe);
      }
      else
      {
         // FIXME which one should it be?
         // startframe = renderGlobals.frameStart.as(MTime::uiUnit());
         startframe = MAnimControl::currentTime().as(MTime::uiUnit());
         endframe = startframe;
         byframestep = 1;
      }

      MDagPathArray cameras;
      MItDag dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);
      // get all renderable cameras
      for (dagIterCameras.reset(); (!dagIterCameras.isDone()); dagIterCameras.next())
      {
         if (!dagIterCameras.getPath(dagPath))
         {
            AiMsgError("[mtoa] Could not get path for DAG iterator");
            return status;
         }

         MFnDependencyNode camDag(dagIterCameras.item());
         if (camDag.findPlug("renderable").asBool())
         {
            cameras.append(dagPath);
         }
      }

      for (double framerender = startframe; framerender <= endframe; framerender += byframestep)
      {
         MGlobal::viewFrame(framerender);
         CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

         // FIXME: do we really need to reset everything each time?
         CMayaScene::Begin(MTOA_SESSION_BATCH);
         CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
         CRenderSession* renderSession = CMayaScene::GetRenderSession();
         arnoldSession->SetExportFrame(framerender);

         CMayaScene::Export(selectedPtr);
         // Reset resolution and output since it's a new export, new options node
         renderSession->SetResolution(width, height);

         for (unsigned int arrayIter = 0; (arrayIter < cameras.length()); arrayIter++)
         {
            // It is ok to set the camera here, because if camera is no set at export time,
            // all the cameras are exported during the export.
            arnoldSession->SetExportCamera(cameras[arrayIter]);

            if (renderSession->DoBatchRender() != AI_SUCCESS)
            {
               CMayaScene::End();
               MGlobal::displayError("[mtoa] Failed batch render");
               return MS::kFailure;
            }
         }

         CMayaScene::ExecuteScript(renderGlobals.postRenderMel);
         CMayaScene::End();
      }
   }

   // or interactive mode
   else
   {
      MSelectionList sel;
      args.getFlagArgument("camera", 0, sel);
      MDagPath camera;
      double currentFrame = MAnimControl::currentTime().as(MTime::uiUnit());
      // FIXME: at scene open the animation bar in Maya maybe be off sync,
      // ie it shows 0 but currentTime -q returns 1. Render is correct as it's indeed
      // done for frame 1
      // MGlobal::viewFrame(currentFrame);

      CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

      // CMayaScene::ExportAndRenderFrame(MTOA_SESSION_RENDER, selected);

      CMayaScene::Begin(MTOA_SESSION_RENDER);

      CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
      CRenderSession* renderSession = CMayaScene::GetRenderSession();

      arnoldSession->SetExportFrame(currentFrame);
      if (MStatus::kSuccess == sel.getDagPath(0, camera)) arnoldSession->SetExportCamera(camera);
      CMayaScene::Export(selectedPtr);
      renderSession->SetResolution(width, height);
      // Set the render session camera.
      renderSession->SetCamera(camera);
      renderSession->DoInteractiveRender(); // Start the render.

      CMayaScene::ExecuteScript(renderGlobals.postRenderMel);
      CMayaScene::End(); // Clean up.
      // DEBUG_MEMORY;
   }

   return status;
}
