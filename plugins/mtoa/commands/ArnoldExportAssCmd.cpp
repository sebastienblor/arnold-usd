#include "utils/Version.h"
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
#include <maya/MBoundingBox.h>

#include <math.h>

MSyntax CArnoldExportAssCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("b", "batch", MSyntax::kNoArg);
   syntax.addFlag("s", "selected");
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.addFlag("cam", "camera", MSyntax::kString);
   syntax.addFlag("sf", "startFrame", MSyntax::kDouble);
   syntax.addFlag("ef", "endFrame", MSyntax::kDouble);
   syntax.addFlag("fs", "frameStep", MSyntax::kDouble);
   syntax.addFlag("o", "options", MSyntax::kString);
   syntax.addFlag("c", "compressed");
   syntax.addFlag("bb", "boundingBox");
   syntax.addFlag("a", "asciiAss");
   syntax.addFlag("m", "mask", MSyntax::kUnsigned);
   syntax.addFlag("ll", "lightLinks", MSyntax::kUnsigned);
   syntax.addFlag("sl", "shadowLinks", MSyntax::kUnsigned);
   syntax.addFlag("ep", "expandProcedurals");
   syntax.addFlag("fsh", "forceTranslateShadingEngines");
   syntax.addFlag("fp", "fullPath");
   syntax.addFlag("pr", "exportPrefix", MSyntax::kString);
   syntax.addFlag("shg", "exportAllShadingGroups");


   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}

// Return all renderable cameras
int CArnoldExportAssCmd::GetRenderCameras(MDagPathArray &cameras)
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
         cameras.append(cameraPath);
      }
      dagIter.next();
   }

   int size = cameras.length();
   if (size > 1)
      MGlobal::displayWarning("More than one renderable camera. (use the -cam/-camera option to override)");
   else if (!size)
      MGlobal::displayWarning("Did not find a renderable camera. (use the -cam/-camera option to specify one)");
   return size;
}

void CArnoldExportAssCmd::UpdateStandinsBoundingBoxes()
{
   MItDag it(MItDag::kDepthFirst, MFn::kPluginShape);

   while (!it.isDone())
   {
      MDagPath dgShape;
      it.getPath(dgShape);

      MFnDependencyNode dpNode(dgShape.node());
      MString nodeType = dpNode.typeName();
      if (nodeType == "aiStandIn")
      {
         MFnDagNode dagShape(dgShape);
         MBoundingBox boundingBox = dagShape.boundingBox();
         MPoint bbMin = boundingBox.min();
         MPoint bbMax = boundingBox.max();
      }
      it.next();
   }
}

// FIXME: that should be a method on CMayaScene so we can share it between commands
MStatus CArnoldExportAssCmd::doIt(const MArgList& argList)
{
   MStatus status;

   // Initialize command syntax and get flags
   MSyntax syntax = newSyntax();
   // we must use an MArgParser because MArgList is not python compatible, and we
   // use this result to set syntax.useSelectionAsDefault() prior to creating the MArgDatabase.
   MArgParser args(syntax, argList, &status);
   bool exportSelected = args.isFlagSet("selected");
   MArgDatabase argDB(syntax, argList, &status);

   // We force "selected" mode when objects are passed explicitly
   MSelectionList sList;
   MStringArray sListStrings;
   argDB.getObjects(sListStrings);
   const unsigned int sListStringsLength = sListStrings.length();
   if (exportSelected)
   {
      if (sListStringsLength > 0)
      {
         for (unsigned int i = 0; i < sListStringsLength; ++i)
            sList.add(sListStrings[i]);
      }
      else
         MGlobal::getActiveSelectionList(sList);
   }
   
   if ((exportSelected == true) && (sList.length() == 0))
   {
      status = MStatus::kInvalidParameter;
      MGlobal::displayError("arnoldExportAss -selected needs an active selection or a list of objects");
      return status;
   }

   MString customFileName = "";
   MDagPath camera;
   MString cameraName = "";
   MString optionsName = "";
   MString assExtension = "ass";
   MString exportPrefix = "";

   bool writeBox                     = false;
   bool createDirectory              = true;
   bool isSequence                   = false;
   bool subFrames                    = false;
   bool compressed                   = false;
   bool asciiAss                     = true;
   bool expandProcedurals            = false;
   bool forceTranslateShadingEngines = false;
   bool fullPath                     = false;
   int mask        = -1;
   int lightLinks  = -1;
   int shadowLinks = -1;
   double startframe, endframe, framestep;

   // Batch mode
   bool batch = argDB.isFlagSet("batch") ? true : false;
   // Custom filename
   if (argDB.isFlagSet("filename"))
   {
      // if custom file name is provided
      argDB.getFlagArgument("filename", 0, customFileName);
      // Strip the .ass, .gz or .ass.gz extension if present
      unsigned int nchars = customFileName.numChars();
      if  (nchars > 7 && customFileName.substringW(nchars-7, nchars) == ".ass.gz")
      {
         customFileName = customFileName.substringW(0, nchars-8);
      }
      else if (nchars > 4 && customFileName.substringW(nchars-4, nchars) == ".ass")
      {
         customFileName = customFileName.substringW(0, nchars-5);
      }
      else if (nchars > 3 && customFileName.substringW(nchars-3, nchars) == ".gz")
      {
         customFileName = customFileName.substringW(0, nchars-4);
      }
   }
   // Rendered render layer
   // TODO: add this to flags
   MObject renderLayer = MFnRenderLayer::currentLayer();
   // Custom camera
   if (argDB.isFlagSet("camera"))
   {
      MString camName;
      argDB.getFlagArgument("camera", 0, camName);
      MSelectionList sel;
      sel.add(camName);
      MStatus status;
      status = sel.getDagPath(0, camera);
      if (status != MStatus::kSuccess)
      {
         MGlobal::displayError("[mtoa] Invalid camera \"" + camName + "\": " + status.errorString());
         return MS::kFailure;
      }
   }
   // Output bounding box
   if (argDB.isFlagSet("boundingBox"))
   {
      writeBox  = true;
   }
   // Specific objects mask to write
   if (argDB.isFlagSet("mask"))
   {
      argDB.getFlagArgument("mask", 0, mask);
   }
   if (argDB.isFlagSet("lightLinks"))
   {
      argDB.getFlagArgument("lightLinks", 0, lightLinks);
   }
   if (argDB.isFlagSet("shadowLinks"))
   {
      argDB.getFlagArgument("shadowLinks", 0, shadowLinks);
   }
   // Custom render options
   if (argDB.isFlagSet("options"))
   {
      argDB.getFlagArgument("options", 0, optionsName);
      // TODO: Set the arnoldRenderOptions node to use
   }
   // Start frame
   if (argDB.isFlagSet("startFrame"))
   {
      argDB.getFlagArgument("startFrame", 0, startframe);
   }
   else
   {
      startframe = MAnimControl::currentTime().as(MTime::uiUnit());
   }
   // End frame
   if (argDB.isFlagSet("endFrame"))
   {
      isSequence = true;
      argDB.getFlagArgument("endFrame", 0, endframe);
   }
   // Frame step
   if (argDB.isFlagSet("frameStep"))
   {
      argDB.getFlagArgument("frameStep", 0, framestep);
   }
   else
   {
      framestep = 1;
   }
   // If it's a sequence
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
   // Compressed
   if (argDB.isFlagSet("compressed"))
   {
      compressed = true;
      assExtension = "ass.gz";
   }
   // AsciiAss
   if (argDB.isFlagSet("asciiAss"))
   {
      asciiAss = false;
   }
   // Expand Procedurals
   
   if (argDB.isFlagSet("expandProcedurals"))
   {
      expandProcedurals = true;
   }

   if (argDB.isFlagSet("forceTranslateShadingEngines"))
   {
      forceTranslateShadingEngines = true;
   }
   if (argDB.isFlagSet("fullPath"))
   {
      fullPath = true;
   }
   if (argDB.isFlagSet("exportPrefix"))
   {
      argDB.getFlagArgument("exportPrefix", 0, exportPrefix);
   }

   bool exportAllShadingGroups = (argDB.isFlagSet("exportAllShadingGroups"));
      
   // Get Maya scene information
   MString sceneName = MFileIO::currentFile();
   // If camera name is not set, default to active view camera in interactive mode
   // or the first found renderable camera in batch mode
   // FIXME if you're exporting in selected mode to reuse in a standing
   // you probably don't want a camera anyway, remove this search and warnings in that case?

   MDagPathArray cameras;

   if (camera.isValid())
      cameras.append(camera);
   else if (mask == -1 || (mask & AI_NODE_CAMERA))
      GetRenderCameras(cameras);
   else
      cameras.append(MDagPath());
        

   // FIXME use the passed renderGlobals or options intead?
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   // Just incase we're rendering with IPR.
   MStringArray panelName;
   MGlobal::executeCommand("getPanel -scriptType renderWindowPanel", panelName);
   if (panelName.length() > 0)
      MGlobal::executeCommand("stopIprRendering " + panelName[0]);
   CMayaScene::End();
   // Cannot export while a render is active
   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] Cannot export to .ass while rendering");
      return MS::kFailure;
   }

   MString curfilename;
   MString tocfilename;
   
   MComputation computation;
   computation.beginComputation();

   // customFileName is a prefix, need to add frame and extension
   for (double curframe = startframe; curframe <= endframe; curframe += framestep)
   {
      if (computation.isInterruptRequested())
         break;
      MGlobal::viewFrame(curframe);
      UpdateStandinsBoundingBoxes(); // Need to update the bounding box attribute of standins, because in batch export it cannot be recomputed and it's needed for correct and optimal rendering
      CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

      // Setup CMayaScene for MTOA_SESSION_ASS mode
      // FIXME : do we really have to reset everything?
      CMayaScene::Begin(MTOA_SESSION_ASS);
      CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      // Not filtering out of render layer
      // FIXME this is the only piece of code that relies on the export filter.
      // In a future ABI-breaking version, we should find another way to support this
      if (exportSelected)
      {
         arnoldSession->SetExportFilterMask(arnoldSession->GetExportFilterMask() & ~MTOA_FILTER_LAYER);
      }
      else
      {
         arnoldSession->SetExportFilterMask(arnoldSession->GetExportFilterMask());
      }
      arnoldSession->SetExportFrame(curframe);
      // Set mask for nodes to export or use Arnold Render Globals if not passed
      if (mask != -1)
      {
         renderSession->SetOutputAssMask(mask);
      }
      renderSession->SetExpandProcedurals(expandProcedurals);
      // Set light linking mode or use Arnold Render Globals if not passed
      if (lightLinks != -1)
      {
         arnoldSession->SetLightLinkMode(ArnoldLightLinkMode(lightLinks));
      }
      // Set shadow linking mode or use Arnold Render Globals if not passed
      if (shadowLinks != -1)
      {
         arnoldSession->SetShadowLinkMode(ArnoldShadowLinkMode(shadowLinks));
      }
      renderSession->SetForceTranslateShadingEngines(forceTranslateShadingEngines);
      
      arnoldSession->SetExportFullPath(fullPath);
      arnoldSession->SetExportPrefix(exportPrefix);
            
      arnoldSession->SetExportAllShadingGroups(exportAllShadingGroups);

      MFnDependencyNode fnCam;

      // Export the scene or the selection
      if (exportSelected)
      {
         // We can't precompute sList since it won't survive moving from frame to frame with viewFrame
         sList.clear();
         if (sListStringsLength > 0)
         {
            for (unsigned int i = 0; i < sListStringsLength; ++i)
               sList.add(sListStrings[i]);
         }
         else
            MGlobal::getActiveSelectionList(sList);
         CMayaScene::Export(&sList);
      }
      else
      {
         CMayaScene::Export();
      }


      for (unsigned int arrayIter = 0; (arrayIter < cameras.length()); arrayIter++)
      {
         // It is ok to set the camera here, because if camera is no set at export time,
         // all the cameras are exported during the export.
         if (cameras[arrayIter].isValid())
         {
            arnoldSession->SetExportCamera(cameras[arrayIter]);
            fnCam.setObject(cameras[arrayIter].transform());
            cameraName = fnCam.name();
         } else
         {
            cameraName = "";
         }
         // getting ass file name
         curfilename = renderSession->GetAssName(customFileName,
                                                 renderGlobals,
                                                 curframe,
                                                 sceneName,
                                                 cameraName,
                                                 assExtension,
                                                 renderLayer,
                                                 createDirectory,
                                                 isSequence,
                                                 subFrames,
                                                 batch, &status);

         appendToResult(curfilename);

         renderSession->SetUseBinaryEncoding(asciiAss);
         renderSession->DoAssWrite(curfilename, compressed, writeBox);
      }
      CMayaScene::End();

      CMayaScene::ExecuteScript(renderGlobals.postRenderMel);
   }
   computation.endComputation();


   return status;
}
