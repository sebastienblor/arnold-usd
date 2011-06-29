#include "ArnoldExportAssCmd.h"
#include "render/RenderSession.h"

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

MSyntax CArnoldExportAssCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("b", "batch", MSyntax::kNoArg);
   syntax.addFlag("s", "selected");
   syntax.addFlag("bb", "boundingBox");
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.addFlag("cam", "camera", MSyntax::kString);
   syntax.addFlag("sf", "startFrame", MSyntax::kDouble);
   syntax.addFlag("ef", "endFrame", MSyntax::kDouble);
   syntax.addFlag("fs", "frameStep", MSyntax::kDouble);
   syntax.addFlag("o", "options", MSyntax::kString);
   return syntax;
}

MString CArnoldExportAssCmd::GetCameraName()
{
   MString cameraName("");
   if (MGlobal::mayaState() == MGlobal::kInteractive)
   {
      MDagPath cameraPath;
      M3dView::active3dView().getCamera(cameraPath);

      MFnDagNode cameraNode(cameraPath.node());
      cameraName = cameraNode.partialPathName();
   }
   if (cameraName == "")
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
            if (cameraName == "")
            {
               cameraName = cameraNode.partialPathName();
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
   if (cameraName == "")
      MGlobal::displayWarning("Did not find a renderable camera. (use the -cam/-camera option to specify one)");

   return cameraName;
}

MStatus CArnoldExportAssCmd::doIt(const MArgList& argList)
{
   MStatus status;

   // Initialize command syntax and get flags
   MSyntax syntax = newSyntax();
   MArgDatabase argDB(syntax, argList, &status);
   MString customFileName = "";
   MString cameraName = "";
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
      argDB.getFlagArgument("camera", 0, cameraName);
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
   if (cameraName == "")
   {
      cameraName = GetCameraName();
   }
   // Set the necessary options for scene export
   // Filtering
   CExportFilter exportFilter;
   exportFilter.unselected = exportSelected;
   if (exportSelected) exportFilter.notinlayer = false;
   // Current frame
   double fileFrameNumber = 0;
   MTime cT = MAnimControl::currentTime();
   fileFrameNumber = double(cT.value());
   // Build Export options in MTOA_EXPORT_FILE mode
   CExportOptions exportOptions;
   exportOptions.SetExportFrame(fileFrameNumber);
   exportOptions.SetExportMode(MTOA_EXPORT_FILE);
   exportOptions.SetExportFilter(exportFilter);

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

         curfilename = renderSession->GetAssName(customFileName,
                                                 renderGlobals,
                                                 curframe,
                                                 sceneName,
                                                 cameraName,
                                                 "ass",
                                                 renderLayer,
                                                 1,
                                                 1,
                                                 subFrames,
                                                 IsBatch(), &status);
         tocfilename = renderSession->GetAssName(customFileName,
                                                 renderGlobals,
                                                 curframe,
                                                 sceneName,
                                                 cameraName,
                                                 "asstoc",
                                                 renderLayer,
                                                 1,
                                                 1,
                                                 subFrames,
                                                 IsBatch(), &status);

         exportOptions.SetExportFrame(curframe);
         renderSession->Translate(exportOptions);
         if (cameraName != "")
            renderSession->SetCamera(cameraName);

         renderSession->DoExport(curfilename);
         renderSession->Finish();

         if (writeBox)
            renderSession->WriteAsstoc(tocfilename, renderSession->GetBoundingBox());
            renderSession->Finish();

         renderSession->ExecuteScript(renderGlobals.postRenderMel);

         appendToResult(curfilename);
      }
   }
   else
   {
      renderSession->ExecuteScript(renderGlobals.preRenderMel);

      curfilename = renderSession->GetAssName(customFileName,
                                              renderGlobals,
                                              fileFrameNumber,
                                              sceneName,
                                              cameraName,
                                              "ass",
                                              renderLayer,
                                              1,
                                              0,
                                              0,
                                              IsBatch(), &status);
      tocfilename = renderSession->GetAssName(customFileName,
                                              renderGlobals,
                                              fileFrameNumber,
                                              sceneName,
                                              cameraName,
                                              "asstoc",
                                              renderLayer,
                                              1,
                                              0,
                                              0,
                                              IsBatch(), &status);

      renderSession->Translate(exportOptions);
      if (cameraName != "")
         renderSession->SetCamera(cameraName);

      renderSession->DoExport(curfilename);
      renderSession->Finish();

      if (writeBox)
         renderSession->WriteAsstoc(tocfilename, renderSession->GetBoundingBox());
         renderSession->Finish();

      renderSession->ExecuteScript(renderGlobals.postRenderMel);

      appendToResult(curfilename);
   }

   return status;
}
