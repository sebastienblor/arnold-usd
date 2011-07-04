#include "ArnoldExportAssCmd.h"
#include "render/RenderSession.h"
#include "scene/MayaScene.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>

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

MStatus CArnoldExportAssCmd::doIt(const MArgList& argList)
{
   MStatus status;

   // Initialize command syntax and get flags
   MSyntax syntax = newSyntax();
   MArgDatabase argDB(syntax, argList, &status);
   MString customFileName = "";
   CExportOptions exportOptions;
   MString optionsName = "";
   bool exportSelected = false;
   bool writeBox = false;
   double startframe = 1; // TODO: use current frame if not set
   double endframe = 0;
   double framestep = 1;

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
      status = sel.getDagPath(0, exportOptions.camera);
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
   if (argDB.isFlagSet("startFrame"))
   {
      argDB.getFlagArgument("startFrame", 0, startframe);
   }
   if (argDB.isFlagSet("endFrame"))
   {
      argDB.getFlagArgument("endFrame", 0, endframe);
   }
   if (argDB.isFlagSet("frameStep"))
   {
      argDB.getFlagArgument("frameStep", 0, framestep);
   }
   // Get Maya scene information
   MString sceneName = MFileIO::currentFile();
   // If camera name is not set, default to active view camera in interactive mode
   // or the first found renderable camera in batch mode
   // FIXME if you're exporting in selected mode to reuse in a standing
   // you probably don't want a camera anyway, remove this search and warnings in that case?
   if (!exportOptions.camera.isValid())
   {
      exportOptions.camera = GetCamera();
   }
   // Set the necessary options for scene export
   exportOptions.mode = MTOA_EXPORT_FILE;
   exportOptions.filter.unselected = exportSelected;
   if (exportOptions.filter.unselected)
   {
      exportOptions.filter.notinlayer = false;
   }
   // FIXME use the passed renderGlobals or options intead?
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   CRenderSession* renderSession = CRenderSession::GetInstance();
   // Just incase we're rendering with IPR.
   MGlobal::executeCommand("stopIprRendering renderView;");
   renderSession->Finish();
   // Cannot export while a render is active
   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] Cannot export to .ass while rendering");
      return MS::kFailure;
   }
   // We don't need renderview_display so we need to set Batch mode on.
   renderSession->SetBatch(true);

   MString curfilename;
   MString tocfilename;
   // Export range of frames or single frame
   if (startframe <= endframe && framestep > 0)
   {
      bool subFrames = ((framestep - floor(framestep)) >= 0.001);
      // customFileName is a prefix, need to add frame and extension
      // TODO: might want to check if extension or frame is already set
      for (double curframe = startframe; curframe <= endframe; curframe += framestep)
      {
         MGlobal::viewFrame(curframe);
         renderSession->ExecuteScript(renderGlobals.preRenderMel);

         curfilename = renderSession->GetMayaScene()->GetAssName(customFileName,
                                                                renderGlobals,
                                                                curframe,
                                                                sceneName,
                                                                exportOptions.camera.partialPathName(),
                                                                "ass",
                                                                renderLayer,
                                                                1,
                                                                1,
                                                                subFrames,
                                                                IsBatch(), &status);
         tocfilename = renderSession->GetMayaScene()->GetAssName(customFileName,
                                                                renderGlobals,
                                                                curframe,
                                                                sceneName,
                                                                exportOptions.camera.partialPathName(),
                                                                "asstoc",
                                                                renderLayer,
                                                                1,
                                                                1,
                                                                subFrames,
                                                                IsBatch(), &status);

         renderSession->Translate(exportOptions);

         renderSession->DoExport(curfilename);
         renderSession->Finish();

         if (writeBox)
            renderSession->GetMayaScene()->WriteAsstoc(tocfilename, renderSession->GetBoundingBox());
            renderSession->Finish();

         renderSession->ExecuteScript(renderGlobals.postRenderMel);

         appendToResult(curfilename);
      }
   }
   else
   {
      // Get current frame info
      double fileFrameNumber = 0;
      MTime cT = MAnimControl::currentTime();
      fileFrameNumber = double(cT.value());

      renderSession->ExecuteScript(renderGlobals.preRenderMel);

      curfilename = renderSession->GetMayaScene()->GetAssName(customFileName,
                                                             renderGlobals,
                                                             fileFrameNumber,
                                                             sceneName,
                                                             exportOptions.camera.partialPathName(),
                                                             "ass",
                                                             renderLayer,
                                                             1,
                                                             0,
                                                             0,
                                                             IsBatch(), &status);
      tocfilename = renderSession->GetMayaScene()->GetAssName(customFileName,
                                                             renderGlobals,
                                                             fileFrameNumber,
                                                             sceneName,
                                                             exportOptions.camera.partialPathName(),
                                                             "asstoc",
                                                             renderLayer,
                                                             1,
                                                             0,
                                                             0,
                                                             IsBatch(), &status);

      renderSession->Translate(exportOptions);
      renderSession->DoExport(curfilename);
      renderSession->Finish();

      if (writeBox)
         renderSession->GetMayaScene()->WriteAsstoc(tocfilename, renderSession->GetBoundingBox());
         renderSession->Finish();

      renderSession->ExecuteScript(renderGlobals.postRenderMel);

      appendToResult(curfilename);
   }

   return status;
}
