#include "ArnoldRenderCmd.h"
#include "render/OutputDriver.h"
#include "session/ArnoldRenderSession.h"
#include "session/SessionManager.h"
#include "utils/FileUtils.h"

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
#include <set>

static const std::string s_arnoldRenderSessionId("arnoldRender");

MSyntax CArnoldRenderCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("b", "batch", MSyntax::kNoArg);
   syntax.addFlag("p", "port", MSyntax::kUnsigned);
   syntax.addFlag("cam", "camera", MSyntax::kSelectionItem);
   syntax.addFlag("w", "width", MSyntax::kUnsigned);
   syntax.addFlag("h", "height", MSyntax::kUnsigned);
   syntax.addFlag("ofn", "origFileName", MSyntax::kString);
   syntax.addFlag("seq", "frameSequence", MSyntax::kString);
   syntax.addFlag("srv", "saveToRenderView", MSyntax::kString);

   return syntax;
}


enum RenderType
{
   MTOA_RENDER_INTERACTIVE,
   MTOA_RENDER_EXPORTASS,
   MTOA_RENDER_EXPORTASS_AND_KICK
};


MStatus CArnoldRenderCmd::doIt(const MArgList& argList)
{

   MStatus status;
   MArgDatabase args(syntax(), argList);
   MDagPath dagPath;
   
   const bool batch = args.isFlagSet("batch") ? true : false;
   const bool sequence = args.isFlagSet("frameSequence") ? true : false;
   const bool multiframe = batch || sequence;

   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   // Ticket #2377 : 
   //Commenting this for now, as we don't know if we really want ARV to be used for sequence rendering
   
   //if (sequence && !batch)
   //{
      // Sequence interactive rendering
      // RenderSession will try to render it with the Arnold RenderView.
      // If it can't it will return false, and we'll keep using Maya's native one
   //   if (CMayaScene::GetRenderSession()->RenderSequence()) return MS::kSuccess;
   //}

   // Rendered camera
   MString camera = "";
   if (!args.isFlagSet("camera"))
   {
      // no camera set on single frame mode, abort
      if (!multiframe) return MS::kFailure;
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

   CArnoldRenderSession *session = (CArnoldRenderSession *)CSessionManager::FindActiveSession(s_arnoldRenderSessionId);
   if (session == nullptr)
   {
      session = new CArnoldRenderSession();
      CSessionManager::AddActiveSession(s_arnoldRenderSessionId, session);
   }// else : FIXME what to do if the session already exists ??
   else
   {
      session->Clear();
   }

   CSessionOptions &sessionOptions = session->GetOptions();
   MFnDependencyNode fnArnoldRenderOptions(sessionOptions.GetArnoldRenderOptions());
  
   bool outputAssBoundingBox = sessionOptions.outputAssBoundingBox();
   bool expandProcedurals = sessionOptions.expandProcedurals();
   bool useBinaryEncoding = sessionOptions.useBinaryEncoding();
   bool forceTranslateShadingEngines = sessionOptions.forceTranslateShadingEngines();
   sessionOptions.SetProgressive(false); // no progressive renders for this command (MTOA-909)
   bool exportAllShadingGroups = sessionOptions.GetExportAllShadingGroups();
   bool exportFullPaths = sessionOptions.GetExportFullPath();

   // FIXME these 2 parameters are not part of the session options, what should we do with them ?
   MString kickRenderFlags = fnArnoldRenderOptions.findPlug("kickRenderFlags", true).asString();
   short renderType = fnArnoldRenderOptions.findPlug("renderType", true).asShort();
      
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
      if (!useBinaryEncoding)
      {
         cmdStr += " -asciiAss";
      }
      if (renderType == MTOA_RENDER_EXPORTASS)
      {         
         if (outputAssBoundingBox)
            cmdStr += " -bb";
      }
      if (expandProcedurals)
         cmdStr += " -ep";
      if (forceTranslateShadingEngines)
      {
         cmdStr += " -forceTranslateShadingEngines";
      }
      if (exportAllShadingGroups)
         cmdStr += " -shg";

      if (exportFullPaths)
         cmdStr += " -fp";

      if (renderGlobals.isAnimated())
      {     
         if (!((renderType == MTOA_RENDER_EXPORTASS_AND_KICK) && !batch))
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
      }
      if (camera != "")
      {
         cmdStr += " -cam " + camera;
      }

      MGlobal::displayInfo("[mtoa] Executing Maya command " + cmdStr);
      MStringArray assFileNames;
      status = MGlobal::executeCommand(cmdStr, assFileNames);
      unsigned int nfiles = assFileNames.length();
      
      
      MString mtoaBinPath = "";
      MString mtoaShaderPath = "";
      const char* mtoaEnvVar = getenv("MTOA_PATH");
      if (mtoaEnvVar)
      {
         mtoaBinPath = mtoaEnvVar;
         mtoaBinPath += "bin/";

         mtoaShaderPath = mtoaEnvVar;
         mtoaShaderPath += "shaders";
      }
      // FIXME eventually use these paths in kickCmd !!
      
      
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
               const unsigned int numAssfiles = assFileNames.length();
               kickCmd = "";
               for (unsigned int i = 0; i < numAssfiles; ++i)
                  kickCmd += "kick -dw -dp \"" + assFileNames[i] + "\";";
            }
            else
            {
               kickRenderFlags += " -dp"; // no progressive for final frame rendering
               
#ifdef _WIN32
               kickCmd = "Start kick " + kickRenderFlags + " \"" + assFileNames[0] + "\"";
#else
               kickCmd = "kick " + kickRenderFlags + " \"" + assFileNames[0] + "\" &";
#endif
            }
            // NOTE: must be blocking when in batch mode, non blocking when in interractive mode

            MGlobal::displayInfo("[mtoa] Calling external command " + kickCmd);
            int kickRet = system(kickCmd.asChar());            
            if (kickRet)
            {
               MString msg = "[mtoa] Kick return code : ";
               msg += kickRet;
            }

            // TODO : use pykick and MGlobal::executePythonCommandOnIdle to display feedback?
         }
      }
      else
      {
         MGlobal::displayError("[mtoa] Failed to export scene to ass");
      }

      return status;
   }

   // Get render view panel
   MString renderViewPanelName = "";
   if (!batch)
   {
      MStringArray allPanelNames;
      MGlobal::executeCommand("getPanel -scriptType renderWindowPanel", allPanelNames);
      if (allPanelNames.length() > 0)
         renderViewPanelName = allPanelNames[0];
   }
   // Note: Maya seems to internally calls the preRender preLayerRender scripts
   //       as well as the postRender and postLayerRender ones
   
   // Check if in multiframe mode
   if (multiframe)
   {
      // TODO: This really needs to go. We're translating the whole scene for a couple of
      // render options.

      // If in batch rendering mode, check if a port was given to use for communication
      int port = batch && args.isFlagSet("port") ? args.flagArgumentInt("port", 0) : -1;

      double startframe;
      double endframe;
      double byframestep;

      if (renderGlobals.isAnimated())
      {
         startframe = renderGlobals.frameStart.as(MTime::uiUnit());
         endframe = renderGlobals.frameEnd.as(MTime::uiUnit());
         byframestep = renderGlobals.frameBy;
      }
      else
      {
         // FIXME which one should it be?
         // startframe = renderGlobals.frameStart.as(MTime::uiUnit());
         startframe = MAnimControl::currentTime().as(MTime::uiUnit());
         endframe = startframe;
         byframestep = 1;
      }

      // Find cameras to render
      MDagPathArray cameras;
      if (args.isFlagSet("camera"))
      {
         MSelectionList sel;
         args.getFlagArgument("camera", 0, sel);
         MDagPath dagPath;
         if (sel.getDagPath(0, dagPath) != MStatus::kSuccess)
         {
            AiMsgError("[mtoa] Could not get path to camera");
            return MStatus::kFailure;
         }
         cameras.append(dagPath);
      }
      else
      {
         MItDag dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);
         // get all renderable cameras
         for (dagIterCameras.reset(); (!dagIterCameras.isDone()); dagIterCameras.next())
         {
            if (!dagIterCameras.getPath(dagPath))
            {
               AiMsgError("[mtoa] Could not get path for DAG iterator");
               return MStatus::kFailure;
            }

            MFnDependencyNode camDag(dagIterCameras.currentItem());
            if (camDag.findPlug("renderable", true).asBool())
            {
               cameras.append(dagPath);
            }
         }
      }

      // Check if any camera should be saved to render view
      MString saveToRenderView = "";
      if (renderViewPanelName.length() > 0 && args.isFlagSet("saveToRenderView"))
      {
         saveToRenderView = args.flagArgumentString("saveToRenderView", 0);
      }

      std::set<double> frameSet;

      if (args.isFlagSet("seq"))
      {
         MString seq;
         args.getFlagArgument("seq", 0, seq);
         if (seq.length() > 0)
         {
            MStringArray seqArr;
            if (seq.index(';') == -1)
               seq.split(' ', seqArr);
            else
               seq.split(';', seqArr);
            for (unsigned int i = 0; i < seqArr.length(); ++i)
            {
               MString elem = seqArr[i];
               const int id = elem.indexW(MString(".."));
               if (id == -1) // just one frame
                  frameSet.insert(elem.asDouble());
               else if (id > 0)
               {
                  const int id2 = elem.index(':');
                  const double startFrame = elem.substring(0, id - 1).asDouble();
                  const double endFrame = elem.substring(id + 2, (id2 == -1) ? (elem.length() - 1) : (id2 - 1)).asDouble();
                  const double step = (id2 == -1) ? 1.0 : elem.substring(id2 + 1, elem.length() - 1).asDouble();
                  for (double frame = startFrame; frame <= endFrame; frame += step)
                     frameSet.insert(frame);
               }
            }
         }
      }

      if (frameSet.size() == 0)
      {
         for (double framerender = startframe; framerender <= endframe; framerender += byframestep)
            frameSet.insert(framerender);
      }

      size_t frameIndex = 1, frameCount = frameSet.size();
      // Get Layer display name
      MObject layerObj = MFnRenderLayer::currentLayer();
      MString layerDisplayName = MFnDependencyNode(layerObj).name();
      int scriptExists = 0;

      MGlobal::executeCommand("exists renderLayerDisplayName", scriptExists);
      if (scriptExists)
         MGlobal::executeCommand("renderLayerDisplayName " + layerDisplayName, layerDisplayName);

      for (std::set<double>::const_iterator frameIt = frameSet.begin(); frameIt != frameSet.end(); ++frameIt)
      {
         const double framerender = *frameIt;
         MGlobal::viewFrame(framerender);
         MGlobal::executeCommand(renderGlobals.preRenderMel);

         session->Clear(); // clear the session and re-export the scene from scratch
         session->SetBatch(batch); // batch means that we're not displaying the render
         sessionOptions.ClearCamera(); // Clear the previously set camera before updating everything in the session options.
         // Update the session options so that everything is up-to-date (frame, log file, etc...) #MTOA-740
         sessionOptions.Update();
         sessionOptions.SetSupportStereoCameras(true);

         session->Export(selectedPtr);
         AtUniverse *universe = session->GetUniverse();
         
         if (renderViewPanelName.length() > 0)
            session->GetOptions().SetRenderViewPanelName(renderViewPanelName);

         for (unsigned int arrayIter = 0; (arrayIter < cameras.length()); arrayIter++)
         {
            const MDagPath& cameraDagPath = cameras[arrayIter];

            // It is ok to set the camera here, because if camera is no set at export time,
            // all the cameras are exported during the export.
            session->SetExportCamera(cameraDagPath);
            sessionOptions.SetResolution(width, height);
            
            if (batch)
            {
               // Skipt file render if it already exists
               if(renderGlobals.skipExistingFrames)
               {
                  MStringArray imageFilenames = session->GetActiveImageFilenames();
                  if(imageFilenames.length() > 0 && fileExists(imageFilenames[0].asChar()))
                  {
                        MGlobal::displayInfo("[mtoa] Skipping existing image: " + imageFilenames[0]);
                        continue;
                  }
               }
               session->DisplayProgress(true);

               if (!session->BatchRender())
               {
                  MGlobal::displayError("[mtoa] Failed batch render");
                  CSessionManager::DeleteActiveSession(s_arnoldRenderSessionId);
                  return MS::kFailure;
               }
            }
            else
            {
               MString msg = "[mtoa] Rendering Frame: ";
               msg +=  (int)framerender;
               msg +=  " (" ;
               msg += (int)frameIndex;
               msg += "/";
               msg += (int)frameCount;
               msg += ")  Camera: ";
               msg +=  cameraDagPath.partialPathName() ;
               msg += "  Layer: ";
               msg += layerDisplayName;

               MGlobal::displayInfo(msg);
               MStringArray imageFilenames = session->GetActiveImageFilenames();
               
               // Skipt file render if it already exists
               if(renderGlobals.skipExistingFrames)
               {
                  if(imageFilenames.length() > 0 && fileExists(imageFilenames[0].asChar()))
                  {
                        MGlobal::displayInfo("[mtoa] Skipping existing image: " + imageFilenames[0]);
                        continue;
                  }
               }

               for (size_t i = 0; i < imageFilenames.length(); ++i)
                  MGlobal::displayInfo("\t" + imageFilenames[i]);

               if (!session->Render())
               {
                  return MS::kFailure;
               }
               
               // Save the image to render view if requested
               if (saveToRenderView == "all" || saveToRenderView == MFnDependencyNode(cameraDagPath.transform()).name()) {
                  MGlobal::executeCommand("renderWindowMenuCommand(\"keepImageInRenderView\", \"" + renderViewPanelName + "\")");
               }
            }
         }

         MGlobal::executeCommand(renderGlobals.postRenderMel);
         ++frameIndex;
      }
   }

   // or single frame interactive mode
   else
   {
      MSelectionList sel;
      args.getFlagArgument("camera", 0, sel);
      MDagPath camera;
      // FIXME: at scene open the animation bar in Maya maybe be off sync,
      // ie it shows 0 but currentTime -q returns 1. Render is correct as it's indeed
      // done for frame 1
      // MGlobal::viewFrame(currentFrame);

      MGlobal::executeCommand(renderGlobals.preRenderMel);

      if (MStatus::kSuccess == sel.getDagPath(0, camera)) session->SetExportCamera(camera, false);
      sessionOptions.SetResolution(width, height);

      session->Export(selectedPtr);
      
      // And render view panel
      if (renderViewPanelName.length() > 0)
         sessionOptions.SetRenderViewPanelName(renderViewPanelName);

      // Start the render.
      if (!session->Render())
         return MS::kFailure;

      MGlobal::executeCommand(renderGlobals.postRenderMel, false, true);


      // Workaround for overriding the render view caption that Maya
      // sets after rendering is finished. Set the last caption again 
      // deferred to override Maya's caption.
      const MString& captionCmd = session->GetLastRenderViewCaptionCommand();
      if (captionCmd != "")
      {
         MGlobal::executeCommandOnIdle(captionCmd, false);
      }

      // DEBUG_MEMORY;
   }

   CSessionManager::DeleteActiveSession(s_arnoldRenderSessionId);

   return status;
}
