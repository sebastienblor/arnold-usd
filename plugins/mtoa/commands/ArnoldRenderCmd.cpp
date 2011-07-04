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
   CRenderSession* renderSession = CRenderSession::GetInstance();
   MArgDatabase args(syntax(), argList);
   MDagPath dagPath;

   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   const bool batch = args.isFlagSet("batch") ? true : false;
   renderSession->SetBatch(batch);

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
   CExportOptions exportOptions;
   exportOptions.SetExportMode(MTOA_EXPORT_ALL);
   exportOptions.GetExportFilter()->unselected = !renderGlobals.renderAll;

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
      if (exportOptions.GetExportFilter()->unselected)
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
         exportOptions.SetExportFrame(framerender);
         renderSession->Translate(exportOptions);
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

         if (cameras.length() > 1)
         {
            renderSession->SetMultiCameraRender(true);
         }

         for (unsigned int arrayIter = 0; (arrayIter < cameras.length()); arrayIter++)
         {
            // It is ok to set the camera here, because if exportOptions.camera is unset
            // all the cameras are exported during Translate (above)
            renderSession->SetCamera(cameras[arrayIter]);

            if (renderSession->DoBatchRender() != AI_SUCCESS)
            {
               renderSession->Finish();
               MGlobal::displayError("[mtoa] Failed batch render");
               return MS::kFailure;
            }
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

      MSelectionList sel;
      args.getFlagArgument("camera", 0, sel);
      MDagPath camera;
      if (MStatus::kSuccess == sel.getDagPath(0, camera)) exportOptions.SetExportCamera(camera);

      renderSession->ExecuteScript(renderGlobals.preRenderMel);

      renderSession->Finish(); // In case we're already rendering (e.g. IPR).
      renderSession->Translate(exportOptions); // Translate the scene from Maya.
      renderSession->SetResolution(width, height);
      renderSession->DoInteractiveRender(); // Start the render.
      renderSession->Finish(); // Clean up.

      renderSession->ExecuteScript(renderGlobals.postRenderMel);
   }

   return status;
}
