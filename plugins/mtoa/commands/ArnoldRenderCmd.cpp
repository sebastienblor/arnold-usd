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

#include <sstream>

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
      // Current Maya project
      MString curProject = MGlobal::executeCommandStringResult("workspace -q -o");
      // Current Maya file and directory
      MFileObject sceneFile;
      sceneFile.overrideResolvedFullName(MFileIO::currentFile());
      MString sceneDir = sceneFile.resolvedPath();
      MString sceneFileName = sceneFile.resolvedName();
      // filename specified in render settings
      MString assFileName = renderGlobals.name;
      // Use specified file name, default to Maya scene name
      if (assFileName.numChars() == 0)
      {
         assFileName = sceneFileName;
         // Strip the .mb or .ma extension if present
         unsigned int nchars = assFileName.numChars();
         if (nchars > 3)
         {
            MString ext = assFileName.substringW(nchars-3, nchars);
            if (ext == ".ma" || ext == ".mb")
            {
               assFileName = assFileName.substringW(0, nchars-4);
            }
         }
      }
      // Double checking
      if (assFileName.numChars() == 0) assFileName = "default";
      // Add .ass extension if not present
      unsigned int nchars = assFileName.numChars();
      if (nchars <= 4 || assFileName.substringW(nchars-4, nchars) != ".ass")
      {
         assFileName += ".ass";
      }
      // If we didn't have an absolute path specified for the file name, then
      // if we got an active, non default project, use the subdirectory registered for ass files
      // else use same directory as Maya file name
      MFileObject assFile;
      status = assFile.setRawFullName(assFileName);
      // If a relative path was specified, use project settings
      if (MStatus::kSuccess == status && assFile.expandedPath().numChars() == 0)
      {
         // Relative file name, check if we got an active project
         MString curProject = MGlobal::executeCommandStringResult("workspace -q -o");
         MString dirProject = "";
         MString assDir = "";
         if (curProject.numChars())
         {
            // If we got an active project, query the subdirectory registered for ass files
            dirProject = MGlobal::executeCommandStringResult("workspace -q -rd \"" + curProject + "\"");
            assDir = MGlobal::executeCommandStringResult("workspace -q -fileRuleEntry ArnoldSceneSource");
         }
         // Use current project ass files subdir, or if none found, use current maya scene dir
         if (dirProject.numChars() && assDir.numChars())
         {
            assFile.setRawPath(dirProject + "/" + assDir);
         }
         else
         {
            assFile.setRawPath(sceneDir);
         }
      }

      // Get expanded full name
      assFileName = assFile.resolvedFullName();

      // FIXME: actual export code should be shared so we don't have to do this dirty call
      MString cmdStr = "arnoldExportAss";
      cmdStr += " -f \"" + assFileName + "\"";

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

      AiMsgInfo("[mtoa] Executing Maya command %s", cmdStr.asChar());
      status = MGlobal::executeCommand(cmdStr);

      if (MStatus::kSuccess == status)
      {
         AiMsgInfo("[mtoa] Exported scene to file %s", assFileName.asChar());
         if (renderType == MTOA_RENDER_EXPORTASS_AND_KICK)
         {
#ifdef _WIN32
            MString kickCmd = "Start kick \"" + assFileName + "\"";
#else
            MString kickCmd = "kick \"" + assFileName + "\" &";
#endif
            // NOTE: must be non blocking!

            AiMsgInfo("[mtoa] Calling external command %s", kickCmd.asChar());
            system(kickCmd.asChar());

            // TODO : use pykick and MGlobal::executePythonCommandOnIdle to display feedback

            // int ret = system(kickCmd.asChar());
            // std::stringstream info;
            // info << "[mtoa] Value returned by kick : " << ret;
            // AiMsgInfo(info.str().c_str());
         }
      }
      else
      {
         AiMsgError("[mtoa] Failed to export scene to file %s", assFileName.asChar());
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
               AiMsgError("[mtoa] Could not get path for DAG iterator");
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
