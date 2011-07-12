#include "ArnoldExportAssCmd.h"
#include "scene/MayaScene.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>
#include <ai_bbox.h>

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderUtil.h>
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MAnimControl.h>

#include <math.h>

MSyntax CArnoldExportAssCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("b", "batch", MSyntax::kNoArg);
   syntax.addFlag("s", "selected");
   syntax.addFlag("bb", "boundingBox");
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.addFlag("cam", "camera", MSyntax::kSelectionItem);
   syntax.addFlag("sf", "startFrame", MSyntax::kDouble);
   syntax.addFlag("ef", "endFrame", MSyntax::kDouble);
   syntax.addFlag("fs", "frameStep", MSyntax::kDouble);
   syntax.addFlag("o", "options", MSyntax::kString);
   return syntax;
}

MDagPath CArnoldExportAssCmd::GetCamera()
{
   MDagPath camera;
   if (MGlobal::mayaState() == MGlobal::kInteractive)
      M3dView::active3dView().getCamera(camera);

   if (!camera.isValid())
   {
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
            if (!camera.isValid())
            {
               camera = cameraPath;
            }
            else
            {
               MGlobal::displayWarning("More than one renderable camera, using first one. (use the -cam/-camera option to override)");
               break;
            }
         }
         dagIter.next();
      }
   }
   if (!camera.isValid())
      MGlobal::displayWarning("Did not find a renderable camera. (use the -cam/-camera option to specify one)");

   return camera;
}

// FIXME: that should be a method on CMayaScene so we can share it between commands
MStatus CArnoldExportAssCmd::doIt(const MArgList& argList)
{
   MStatus status;

   // Initialize command syntax and get flags
   MSyntax syntax = newSyntax();
   MArgDatabase argDB(syntax, argList, &status);
   MString customFileName = "";
   MDagPath camera;
   MString cameraName = "";
   MString optionsName = "";
   bool exportSelected = false;
   bool writeBox = false;
   bool createDirectory = true;

   // Batch mode
   const bool batch = argDB.isFlagSet("batch") ? true : false;
   SetBatch(batch);
   // Custom filename
   if (argDB.isFlagSet("filename"))
   {
      // if custom file name is provided
      argDB.getFlagArgument("filename", 0, customFileName);
      // Strip the .ass extension if present
      unsigned int nchars = customFileName.numChars();
      if (nchars > 4 && customFileName.substringW(nchars-4, nchars) == ".ass")
      {
         customFileName = customFileName.substringW(0, nchars-5);
      }
   }
   // Rendered render layer
   // TODO: add this to flags
   MObject renderLayer = MFnRenderLayer::currentLayer();
   // Custom camera
   if (argDB.isFlagSet("camera"))
   {
      MSelectionList sel;
      argDB.getFlagArgument("camera", 0, sel);
      MStatus status;
      status = sel.getDagPath(0, camera);
   }
   // Only selected
   if (argDB.isFlagSet("selected"))
   {
      exportSelected = true;
   }
   // Output bounding box
   if (argDB.isFlagSet("boundingBox"))
   {
      writeBox  = true;
   }
   // Custom render options
   if (argDB.isFlagSet("options"))
   {
      argDB.getFlagArgument("options", 0, optionsName);
      // TODO: Set the arnoldRenderOptions node to use
   }

   double startframe, endframe, framestep;
   bool isSequence = false;
   bool subFrames = false;

   if (argDB.isFlagSet("startFrame"))
   {
      // isSequence = true;
      argDB.getFlagArgument("startFrame", 0, startframe);
   }
   else
   {
      startframe = MAnimControl::currentTime().as(MTime::uiUnit());
   }
   if (argDB.isFlagSet("endFrame"))
   {
      isSequence = true;
      argDB.getFlagArgument("endFrame", 0, endframe);
   }
   else
   {
      isSequence = false;
   }
   if (argDB.isFlagSet("frameStep"))
   {
      argDB.getFlagArgument("frameStep", 0, framestep);
   }
   else
   {
      framestep = 1;
   }

   if (isSequence)
   {
      subFrames = (fabs(framestep - floor(framestep)) >= AI_EPSILON || fabs(startframe - floor(startframe)) >= AI_EPSILON);
      // To avoid endless loops in corner cases
      if (fabs(framestep) < AI_EPSILON) framestep = 1;
      if ((endframe - startframe) * framestep < 0) framestep = -framestep;
   }
   else
   {
      endframe = startframe;
      framestep = 1;
   }

   // Get Maya scene information
   MString sceneName = MFileIO::currentFile();
   // If camera name is not set, default to active view camera in interactive mode
   // or the first found renderable camera in batch mode
   // FIXME if you're exporting in selected mode to reuse in a standing
   // you probably don't want a camera anyway, remove this search and warnings in that case?
   if (!camera.isValid()) camera = GetCamera();
   cameraName = camera.partialPathName();

   // FIXME use the passed renderGlobals or options intead?
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   // Just incase we're rendering with IPR.
   MGlobal::executeCommand("stopIprRendering renderView;");
   CMayaScene::End();
   // Cannot export while a render is active
   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] Cannot export to .ass while rendering");
      return MS::kFailure;
   }

   MString curfilename;
   MString tocfilename;

   // customFileName is a prefix, need to add frame and extension
   for (double curframe = startframe; curframe <= endframe; curframe += framestep)
   {
      MGlobal::viewFrame(curframe);
      CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

      // Setup CMayaScene for MTOA_SESSION_ASS mode
      // FIXME : do we really have to reset everything?
      CMayaScene::Begin(MTOA_SESSION_ASS);
      CArnoldSession* exportSession = CMayaScene::GetExportSession();
      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      // Not filtering out of render layer
      exportSession->SetExportFilter(exportSession->GetExportFilter() & ~MTOA_FILTER_LAYER);
      exportSession->SetExportCamera(camera);
      exportSession->SetExportFrame(curframe);

      curfilename = renderSession->GetAssName(customFileName,
                                              renderGlobals,
                                              curframe,
                                              sceneName,
                                              cameraName,
                                              "ass",
                                              renderLayer,
                                              createDirectory,
                                              isSequence,
                                              subFrames,
                                              IsBatch(), &status);
      tocfilename = renderSession->GetAssName(customFileName,
                                              renderGlobals,
                                              curframe,
                                              sceneName,
                                              cameraName,
                                              "asstoc",
                                              renderLayer,
                                              createDirectory,
                                              isSequence,
                                              subFrames,
                                              IsBatch(), &status);

      CMayaScene::Export();
      // TODO: package all of this in a method
      if (writeBox) AiNodeSetBool(AiUniverseGetOptions(), "preserve_scene_data", true);
      renderSession->DoAssWrite(curfilename);
      if (writeBox) renderSession->WriteAsstoc(tocfilename, renderSession->GetBoundingBox());

      CMayaScene::End();

      CMayaScene::ExecuteScript(renderGlobals.postRenderMel);

      appendToResult(curfilename);
   }


   return status;
}
