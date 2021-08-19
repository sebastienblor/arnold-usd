#include "utils/Version.h"
#include "ArnoldExportAssCmd.h"
#include "session/ArnoldSession.h"
#include "session/ArnoldExportSession.h"
#include "session/SessionManager.h"

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
#include <maya/MComputation.h>

#include <math.h>

static const std::string s_exportAssSessionId("ArnoldExportAss");
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
   bool binaryAss                     = true;
   int mask        = -1;
   int lightLinks  = -1;
   int shadowLinks = -1;
   double startframe, endframe, framestep;
   bool mergeFrames = false;

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
      else if (nchars > 4 && customFileName.substringW(nchars-4, nchars) == ".usd")
      {
         // For USD files, we don't want to add the frame number to the filename
         customFileName = customFileName.substringW(0, nchars-5);
         assExtension = "usd";
         mergeFrames = true;
      } else if (nchars > 5 && customFileName.substringW(nchars-5, nchars) == ".usda")
      {
         customFileName = customFileName.substringW(0, nchars-6);
         assExtension = "usda";
         mergeFrames = true;
      } else if (nchars > 5 && customFileName.substringW(nchars-5, nchars) == ".usdc")
      {
         customFileName = customFileName.substringW(0, nchars-6);
         assExtension = "usdc";
         mergeFrames = true;
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


   //CArnoldSession *session = CSessionManager::FindActiveSession(exportSessionName);
   // FIXME check what happens if an existing session is there
   CArnoldExportSession * session = new CArnoldExportSession();

   if (!CSessionManager::AddActiveSession(s_exportAssSessionId, session))
   {
      delete session;
      return MS::kFailure;
   }
   
   CSessionOptions &options = session->GetOptions();

   // Custom render options node
   if (argDB.isFlagSet("options"))
   {
      argDB.getFlagArgument("options", 0, optionsName);
      if (optionsName.length() > 0)
      {
         MSelectionList list;
         MObject optNode;
         if (list.add(optionsName) == MS::kSuccess && list.length() > 0)
         {
            list.getDependNode(0,optNode);
            if (MFnDependencyNode(optNode).typeName() != "aiOptions")
            {
               AiMsgError("[mtoa] Options node needs to be of type 'aiOptions'");
            } else 
               options.SetArnoldRenderOptions(optNode);
         }
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
      if (mask >= 0)
         options.SetOutputAssMask(mask);
   }
   if (argDB.isFlagSet("lightLinks"))
   {
      argDB.getFlagArgument("lightLinks", 0, lightLinks);
      options.SetLightLinkMode(ArnoldLightLinkMode(lightLinks));
   }
   if (argDB.isFlagSet("shadowLinks"))
   {
      argDB.getFlagArgument("shadowLinks", 0, shadowLinks);
      options.SetShadowLinkMode(ArnoldShadowLinkMode(shadowLinks));
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
   
   if (argDB.isFlagSet("asciiAss"))
   {
      binaryAss = false;
   }
   options.SetUseBinaryEncoding(binaryAss);

   if (argDB.isFlagSet("expandProcedurals"))
   {
      options.SetExpandProcedurals(true);      
   }

   if (argDB.isFlagSet("forceTranslateShadingEngines"))
   {
      options.SetForceTranslateShadingEngines(true);
   }
   if (argDB.isFlagSet("fullPath"))
   {
      options.SetExportFullPath(true);
   }
   if (argDB.isFlagSet("exportPrefix"))
   {
      argDB.getFlagArgument("exportPrefix", 0, exportPrefix);
      options.SetExportPrefix(exportPrefix);
   }

   if (argDB.isFlagSet("exportAllShadingGroups"))
   {      
      options.SetExportAllShadingGroups(true);
   }

   
   // Get Maya scene information
   MString sceneName = MFileIO::currentFile();


   // now set the session options

   // If camera name is not set, default to active view camera in interactive mode
   // or the first found renderable camera in batch mode
   // FIXME if you're exporting in selected mode to reuse in a standin
   // you probably don't want a camera anyway, remove this search and warnings in that case?

   MDagPathArray cameras;

   if (camera.isValid())
      cameras.append(camera);
   else if (mask == -1 || (mask & AI_NODE_CAMERA))
   {
      cameras = session->GetRenderCameras();
      int size = cameras.length();
      if (size > 1)
         MGlobal::displayWarning("More than one renderable camera. (use the -cam/-camera option to override)");
      else if (!size)
         MGlobal::displayWarning("Did not find a renderable camera. (use the -cam/-camera option to specify one)");
   }
   else
      cameras.append(MDagPath());
        

   // FIXME use the passed renderGlobals or options intead?
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);


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
      
      if (renderGlobals.preRenderMel.length() > 0)
         MGlobal::executeCommand(renderGlobals.preRenderMel);

      // FIXME shouldn't have to do that, it should be in ArnoldSession
      options.SetExportFrame(curframe);

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
         
         session->Export(&sList);
      }
      else
      {
         session->Export();
      }


      for (unsigned int arrayIter = 0; (arrayIter < cameras.length()); arrayIter++)
      {
         // It is ok to set the camera here, because if camera is no set at export time,
         // all the cameras are exported during the export.
         if (cameras[arrayIter].isValid())
         {
            session->SetExportCamera(cameras[arrayIter]);
            fnCam.setObject(cameras[arrayIter].transform());
            cameraName = fnCam.name();
         } else
         {
            cameraName = "";
         }
         // getting ass file name
         
         curfilename = session->GetExportFilename(customFileName,
                                                 renderGlobals,
                                                 curframe,
                                                 sceneName,
                                                 cameraName,
                                                 assExtension,
                                                 renderLayer,
                                                 createDirectory,
                                                 isSequence && !mergeFrames,
                                                 subFrames,
                                                 batch, &status);

         appendToResult(curfilename);
         session->WriteScene(curfilename, compressed, writeBox, mergeFrames && (curframe != startframe));
      }
      session->Clear();

      if (renderGlobals.postRenderMel.length() > 0)
         MGlobal::executeCommand(renderGlobals.postRenderMel);
   }
   computation.endComputation();
   CSessionManager::DeleteActiveSession(s_exportAssSessionId);
   return status;
}
