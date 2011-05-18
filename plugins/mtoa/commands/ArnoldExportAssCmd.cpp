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

MSyntax CArnoldExportAssCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("s", "selected");
   syntax.addFlag("bb", "boundingBox");
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.addFlag("cam", "camera", MSyntax::kString);
   syntax.addFlag("sf", "startFrame", MSyntax::kLong);
   syntax.addFlag("ef", "endFrame", MSyntax::kLong);
   syntax.addFlag("fs", "frameStep", MSyntax::kLong);
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
   int startframe = 1; // TODO: use current frame if not set
   int endframe = 0;
   int framestep = 1;

   // Custom filename
   if (argDB.isFlagSet("filename"))
   {
      argDB.getFlagArgument("filename", 0, customFileName);
   }
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
   if (argDB.isFlagSet("selected"))
   {
      exportSelected = true;
   }
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

   MString tocfilename;
   // Export range of frames or single frame
   if (startframe <= endframe)
   {
      // customFileName is a prefix, need to add frame and extension
      // TODO: might want to check if extension or frame is already set
      MString curfilename;
      char frameext[64];

      for (int curframe = startframe; curframe <= endframe; curframe += framestep)
      {
         MGlobal::viewFrame((double)curframe);
         renderSession->ExecuteScript(renderGlobals.preRenderMel);

         sprintf(frameext, ".%04d.ass", curframe);
         curfilename = customFileName;
         curfilename += frameext;
         tocfilename = curfilename;
         tocfilename += ".asstoc";

         renderSession->Translate(exportOptions);
         if (cameraName != "")
            renderSession->SetCamera(cameraName);

         if (writeBox)
            WriteAsstoc(tocfilename, renderSession->GetBoundingBox());

         renderSession->DoExport(curfilename);
         renderSession->Finish();
         renderSession->ExecuteScript(renderGlobals.postRenderMel);
      }
   }
   else
   {
      renderSession->ExecuteScript(renderGlobals.preRenderMel);

      tocfilename = customFileName + ".asstoc";

      renderSession->Translate(exportOptions);
      if (cameraName != "")
         renderSession->SetCamera(cameraName);

      if (writeBox)
         WriteAsstoc(tocfilename, renderSession->GetBoundingBox());

      renderSession->DoExport(customFileName);
      renderSession->Finish();
      renderSession->ExecuteScript(renderGlobals.postRenderMel);
   }

   return status;
}
