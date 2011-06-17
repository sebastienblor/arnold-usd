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

MString CArnoldExportAssCmd::GetAssName(const MString& customName,
                                        const MCommonRenderSettingsData& renderGlobals,
                                        double frameNumber,
                                        const MString &sceneName,
                                        const MString &cameraName,
                                        const MString &fileFormat,
                                        const MObject layer,
                                        const bool createDirectory,
                                        const bool isSequence,
                                        const bool subFrames,
                                        MStatus *ReturnStatus) const
{
   MStatus status;
   MString assFileName = customName;
   // Current Maya file and directory
   MFileObject sceneFile;
   sceneFile.overrideResolvedFullName(sceneName);
   MString sceneDir = sceneFile.resolvedPath();
   MString sceneFileName = sceneFile.resolvedName();
   // Strip Maya scene extension if present
   unsigned int nscn = sceneFileName.numChars();
   if (nscn > 3)
   {
      MString ext = sceneFileName.substringW(nscn-3, nscn);
      if (ext == ".ma" || ext == ".mb")
      {
         sceneFileName = sceneFileName.substringW(0, nscn-4);
      }
   }
   // TODO: since .ass is a registered Maya file extension (through the translator),
   // we can output ass files in their own registered project subdir.
   // It's the default Maya behavior a relative path / filename is specified.
   // Problem is it won't be affected by Render command argument redirecting the output file.
   // Thus in bath mode we usually want to use the absolute path.
   // Current behavior is use the custom file name if one is explicitely passed
   // (ie direct call to arnoldExportAss command)
   // If no name is passed (ie call by render command) then :
   // Use render globals file name, but output in projects' ass subdirectory if not in batch mode
   // Use render globals file name and absolute path if in batch mode
   if (customName.numChars())
   {
      if (isSequence)
      {
         // TODO: some of maya tools support fractionnal frame numbers
         char frameExt[64];
         if (subFrames)
         {
            int fullFrame = (int) floor(frameNumber);
            int subFrame = (int) floor((frameNumber - fullFrame) * 1000);
            sprintf(frameExt, ".%04d.%03d", fullFrame, subFrame);
         }
         else
         {
            sprintf(frameExt, ".%04d", (int) frameNumber);
         }

         assFileName = customName + frameExt;
      }
      else
         assFileName = customName;
   }
   else
   {
      MCommonRenderSettingsData::MpathType pathType;
      if (IsBatch())
      {
         pathType = MCommonRenderSettingsData::kFullPathImage;
      }
      else
      {
         pathType = MCommonRenderSettingsData::kRelativePath;
      }
      assFileName = renderGlobals.getImageName(pathType,
                                               frameNumber,
                                               sceneFileName,
                                               cameraName,
                                               fileFormat,
                                               layer,
                                               createDirectory,
                                               &status);
   }
   // Add desired extension if not present
   MString ext = MString(".") + fileFormat;
   unsigned int next = ext.length();
   unsigned int nchars = assFileName.numChars();
   if (nchars <= next || assFileName.substringW(nchars-next, nchars) != ext)
   {
      assFileName += ext;
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

   if (NULL != ReturnStatus) *ReturnStatus = status;
   return assFileName;
}

// FIXME: will probably get removed when we have proper bounding box format support
MStatus CArnoldExportAssCmd::WriteAsstoc(const MString& filename, const AtBBox& bBox)
{
   MString bboxcomment = "bounds ";
   bboxcomment += bBox.min.x;
   bboxcomment += " ";
   bboxcomment += bBox.min.y;
   bboxcomment += " ";
   bboxcomment += bBox.min.z;
   bboxcomment += " ";
   bboxcomment += bBox.max.x;
   bboxcomment += " ";
   bboxcomment += bBox.max.y;
   bboxcomment += " ";
   bboxcomment += bBox.max.z;

   FILE * bboxfile;
   bboxfile = fopen(filename.asChar(), "w");
   if (bboxfile != NULL) {
      fwrite(bboxcomment.asChar() , 1 , bboxcomment.length(), bboxfile);
      fclose(bboxfile);

      return MStatus::kSuccess;
   }
   else
   {
      return MStatus::kFailure;
   }
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
   ExportOptions exportOptions;
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

         curfilename = GetAssName(customFileName,
                                  renderGlobals,
                                  curframe,
                                  sceneName,
                                  cameraName,
                                  "ass",
                                  renderLayer,
                                  1,
                                  1,
                                  subFrames, &status);
         tocfilename = GetAssName(customFileName,
                                  renderGlobals,
                                  curframe,
                                  sceneName,
                                  cameraName,
                                  "asstoc",
                                  renderLayer,
                                  1,
                                  1,
                                  subFrames, &status);

         renderSession->Translate(exportOptions);
         if (cameraName != "")
            renderSession->SetCamera(cameraName);

         if (writeBox)
            WriteAsstoc(tocfilename, renderSession->GetBoundingBox());

         renderSession->DoExport(curfilename);
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

      curfilename = GetAssName(customFileName,
                               renderGlobals,
                               fileFrameNumber,
                               sceneName,
                               cameraName,
                               "ass",
                               renderLayer,
                               1,
                               0,
                               0, &status);
      tocfilename = GetAssName(customFileName,
                               renderGlobals,
                               fileFrameNumber,
                               sceneName,
                               cameraName,
                               "asstoc",
                               renderLayer,
                               1,
                               0,
                               0, &status);

      renderSession->Translate(exportOptions);
      if (cameraName != "")
         renderSession->SetCamera(cameraName);

      if (writeBox)
         WriteAsstoc(tocfilename, renderSession->GetBoundingBox());

      renderSession->DoExport(curfilename);
      renderSession->Finish();
      renderSession->ExecuteScript(renderGlobals.postRenderMel);

      appendToResult(curfilename);
   }

   return status;
}
