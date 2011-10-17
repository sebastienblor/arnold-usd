#include "utils/Universe.h"
#include "utils/MtoaLog.h"
#include "RenderSession.h"
#include "RenderOptions.h"
#include "OutputDriver.h"
#include "scene/MayaScene.h"
#include "translators/NodeTranslator.h"
#include "extension/Extension.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_plugins.h>
#include <ai_render.h>
#include <ai_threads.h>
#include <ai_universe.h>
#include <ai_ray.h>


#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDagNode.h>
#include <maya/MComputation.h>
#include <maya/MEventMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MTimerMessage.h>
#include <maya/MMessage.h> // for MCallbackId
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h>
#include <maya/MStatus.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MImage.h>
#include <maya/MFileObject.h>

#include <maya/M3dView.h>
#include <maya/MRenderView.h>

#include <cstdio>

extern AtNodeMethods* mtoa_driver_mtd;

namespace
{
   MString VerifyFileName(MString fileName, bool compressed)
   {
      unsigned int len = fileName.length();

      if (!compressed)
      {
         if ((len < 4) || (fileName.substring(len - 4, len - 1).toLowerCase() != ".ass"))
            fileName += ".ass";
      }
      else
      {
         if ((len < 7) || (fileName.substring(len - 7, len - 1).toLowerCase() != ".ass.gz"))
         {
            if ((len < 4) || (fileName.substring(len - 4, len - 1).toLowerCase() == ".ass"))
               fileName += ".gz";
            else if ((len < 3) || (fileName.substring(len - 3, len - 1).toLowerCase() == ".gz"))
               fileName = fileName.substring(0, len - 4) + ".ass.gz";
            else
               fileName += ".ass.gz";
         }
      }

      return fileName;
   }
}

// This will update the render view if needed.
// It's called from a maya idle event callback.
// This means it's called a *lot*.
void CRenderSession::RefreshRenderView(float, float, void *)
{
   // This will make the render view show any tiles.
   RefreshRenderViewBBox();
}

void CRenderSession::TransferTilesToRenderView(void*)
{
   // Send the tiles to the render view. The false argument
   // tells it not to display them just yet.
   ProcessUpdateMessage(false);
}

// This is the code for the render thread. This version is used for IPR
// to run the AiRender() process outside of the main thread.
// This is *static*.
unsigned int CRenderSession::RenderThread(AtVoid* data)
{
   CRenderOptions * render_options = static_cast< CRenderOptions * >(data);
   // set progressive start point on AA
   const AtInt num_aa_samples = AiNodeGetInt(AiUniverseGetOptions(), "AA_samples");
   const AtInt sminInit = render_options->progressiveInitialLevel();
   AtInt init_progressive_samples = render_options->isProgressive() ? sminInit : num_aa_samples;

   // Get rid of any previous renders tiles that have not yet
   // been displayed.
   InitializeDisplayUpdateQueue();

   AtULong ai_status(AI_SUCCESS);
   for (int i = init_progressive_samples; i <= num_aa_samples; i++)
   {
      AtInt sampling = i ;
      if (sampling >= 0) sampling = num_aa_samples;

      AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", sampling);
      // Begin a render!
      ai_status = AiRender(AI_RENDER_MODE_CAMERA);
      if (ai_status != AI_SUCCESS) break;
      if (sampling > 0) break;
   }

   // Put this back after we're done interating through.
   AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", num_aa_samples);
   
   DisplayUpdateQueueRenderFinished();

   return 0;
}

MStatus CRenderSession::Begin(CRenderOptions* options)
{
   if (AiUniverseIsActive())
   {
      AiMsgWarning("[mtoa] There can only be one RenderSession active.");
      AiRenderAbort();
      InterruptRender();
      ArnoldUniverseEnd();
   }

   // Begin the Arnold universe, read metadata file and load plugins
   ArnoldUniverseBegin();
   m_is_active = AiUniverseIsActive() ? true : false;
   if (m_is_active)
   {
      m_renderOptions = *options;
      m_renderOptions.SetupLog();
      InstallNodes();
      return MStatus::kSuccess;
   }
   else
   {
      AiMsgError("[mtoa] Could not initialize the Arnold universe in CRenderSession.Begin(CRenderOptions* options)");
      return MStatus::kFailure;
   }
}

MStatus CRenderSession::End()
{
   MStatus status = MStatus::kSuccess;

   if (!AiUniverseIsActive())
   {
      AiMsgWarning("[mtoa] No active Arnold Universe present.");
   }
   else
   {
      AiRenderAbort();
      InterruptRender();
      ArnoldUniverseEnd();
   }
   m_is_active = false;
   // Restore "out of rendering" logging
   MtoaSetupLogging();

   return status;
}

AtBBox CRenderSession::GetBoundingBox()
{
   AtBBox bbox;
   if (AiUniverseIsActive())
   {
      // FIXME: we need to start a render to have it actually initialize the bounding box
      // (in free mode, does nothing but setting the scene up for future ray requests)
      AiRender(AI_RENDER_MODE_FREE);
      bbox = AiUniverseGetSceneBounds();
   }
   else
   {
      AiMsgError("[mtoa] RenderSession is not active.");
   }

   return bbox;
}

// FIXME: will probably get removed when we have proper bounding box format support
MStatus CRenderSession::WriteAsstoc(const MString& filename, const AtBBox& bBox)
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

void CRenderSession::InterruptRender()
{
   assert(AiUniverseIsActive());

   if (AiRendering()) AiRenderInterrupt();

   // Stop the Idle update.
   ClearIdleRenderViewCallback();

   // Wait for the thread to clear.
   if (m_render_thread != NULL)
   {
      AiThreadWait(m_render_thread);
      AiThreadClose(m_render_thread);
      m_render_thread = NULL;		// Until this is handled by AiThreadClose? Had issues where it tried to close an already closed thread
   }
}

void CRenderSession::SetResolution(const int width, const int height)
{
   if (width != -1) m_renderOptions.SetWidth(width);
   if (height != -1) m_renderOptions.SetHeight(height);
}

void CRenderSession::SetRegion(const AtUInt left, const AtUInt right,
                                const AtUInt bottom, const AtUInt top)
{
   m_renderOptions.SetRegion(left, right, bottom, top);
}

void CRenderSession::SetProgressive(const bool is_progressive)
{
   m_renderOptions.SetProgressive(is_progressive);
}

MDagPath CRenderSession::GetCamera() const
{
   return m_renderOptions.GetCamera();
}

/// Export the passed camera node and set options.camera
void CRenderSession::SetCamera(MDagPath cameraNode)
{
   assert(AiUniverseIsActive());

   cameraNode.extendToShape();
   m_renderOptions.SetCamera(cameraNode);

   /*
   // FIXME: this would best be handled by a kind of translator post-process hook.

   // check visibility for all image planes.
   MDagPath dagPath;
   MItDag   dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);

   // for all cameras
   for (dagIterCameras.reset(); (!dagIterCameras.isDone()); dagIterCameras.next())
   {
      if (!dagIterCameras.getPath(dagPath))
      {
         AiMsgError("[mtoa] Could not get path for DAG iterator");
         return;
      }
      bool isRenderingCamera = false;
      if (dagPath == cameraNode)
         isRenderingCamera = true;

      // check all of it's imageplanes
      MPlugArray connectedPlugs;
      MPlug      imagePlanePlug;
      MPlug      imagePlaneNodePlug;
      MFnDagNode fnDagNode(dagPath);
      imagePlanePlug = fnDagNode.findPlug("imagePlane");
      if (imagePlanePlug.numConnectedElements() > 0)
      {
         for(AtUInt ips = 0; (ips < imagePlanePlug.numElements()); ips++)
         {
            MStatus status;
            imagePlaneNodePlug = imagePlanePlug.elementByPhysicalIndex(ips);
            imagePlaneNodePlug.connectedTo(connectedPlugs, true, false, &status);
            MObject resNode = connectedPlugs[0].node(&status);
            if (status)
            {
               // get the dependency node of the image plane
               MFnDependencyNode fnRes(resNode);
               MString imagePlaneName(fnDagNode.partialPathName());
               imagePlaneName += "_IP_";
               imagePlaneName += ips;
               bool displayOnlyIfCurrent = fnRes.findPlug("displayOnlyIfCurrent", &status).asBool();
               AtNode* imagePlane = AiNodeLookUpByName(imagePlaneName.asChar());
               AtInt visibility = 0;
               AiMsgDebug("[mtoa] Using camera %s to output image %s.", cameraNode.partialPathName().asChar(), m_renderOptions.GetImageFilename().asChar());

               if ((displayOnlyIfCurrent && isRenderingCamera) || (!displayOnlyIfCurrent))
               {
                  visibility = AI_RAY_CAMERA;
               }
               if ((displayOnlyIfCurrent && !isRenderingCamera))
               {
                  visibility = 0;
               }
               AiNodeSetInt(imagePlane, "visibility", visibility);
            }
         }
      }
   }*/
}

void CRenderSession::DoInteractiveRender()
{
   assert(AiUniverseIsActive());

   MComputation comp;
   comp.beginComputation();

   PrepareRenderView();

   // Start the render thread.
   m_render_thread = AiThreadCreate(CRenderSession::RenderThread,
                                    &m_renderOptions,
                                    AI_PRIORITY_LOW);

   // Not sure I like this call here, but it guarantees the queue
   // is clear before the process is called.
   ClearDisplayUpdateQueue();
   // This returns when the render is done or if someone
   // has hit escape.
   ProcessDisplayUpdateQueueWithInterupt(comp);

   comp.endComputation();
}


AtULong CRenderSession::DoBatchRender()
{
   return AiRender(AI_RENDER_MODE_CAMERA);
}

MString CRenderSession::GetAssName(const MString& customName,
                                        const MCommonRenderSettingsData& renderGlobals,
                                        double frameNumber,
                                        const MString &sceneName,
                                        const MString &cameraName,
                                        const MString &fileFormat,
                                        const MObject layer,
                                        const bool createDirectory,
                                        const bool isSequence,
                                        const bool subFrames,
                                        const bool isBatch,
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
      if (isBatch)
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

void CRenderSession::DoAssWrite(MString customFileName, const bool compressed)
{
   assert(AiUniverseIsActive());

   MString fileName;

   // if no custom fileName is given, use the default one in the environment variable
   if (customFileName.length() > 0)
   {
      fileName = VerifyFileName(customFileName.asChar(), compressed);
   }
   else
   {
      fileName = VerifyFileName(m_renderOptions.outputAssFile().expandEnvironmentVariablesAndTilde(), compressed);
   }

   if (fileName.length() == 0)
   {
      AiMsgError("[mtoa] File name must be set before exporting .ass file");
   }
   else
   {
      AiMsgInfo("[mtoa] Exporting Maya scene to file \"%s\"", fileName.asChar());

      // FIXME : problem this is actually double filtering files
      // (Once at export to AiUniverse and once at file write from it)
      AiASSWrite(fileName.asChar(), m_renderOptions.outputAssMask(), false);
   }
}

MStatus CRenderSession::PrepareRenderView(bool addIdleRenderViewUpdate)
{
   MStatus status(MS::kSuccess);
   
   // We need to set the current camera in renderView,
   // so the buttons render from the camera you want.
   MDagPath camera = GetCamera();
   MRenderView::setCurrentCamera(camera);

   if (m_renderOptions.useRenderRegion())
   {
      status = MRenderView::startRegionRender(m_renderOptions.width(),
                                                m_renderOptions.height(),
                                                m_renderOptions.minX(),
                                                m_renderOptions.maxX(),
                                                m_renderOptions.minY(),
                                                m_renderOptions.maxY(),
                                                !m_renderOptions.clearBeforeRender(),
                                                true);
   }
   else
   {
      status = MRenderView::startRender(m_renderOptions.width(),
                                        m_renderOptions.height(),
                                        !m_renderOptions.clearBeforeRender(),
                                        true);
   }

   if (MStatus::kSuccess != status)
   {
      MGlobal::displayError("Render view is not able to render.");
      return status;
   }

   ClearIdleRenderViewCallback();
   if (addIdleRenderViewUpdate) AddIdleRenderViewCallback();

   return status;
}

void CRenderSession::DoIPRRender()
{
   assert(AiUniverseIsActive());

   if (!m_paused_ipr)
   {
      PrepareRenderView(true); // Install callbacks.

      // Start the render thread.
      m_render_thread = AiThreadCreate(CRenderSession::RenderThread,
                                       &m_renderOptions,
                                       AI_PRIORITY_LOW);
   }
}

void CRenderSession::FinishedIPRTuning()
{
   ClearIdleRenderViewCallback();
}

void CRenderSession::PauseIPR()
{
   assert(AiUniverseIsActive());

   InterruptRender();
   ClearIdleRenderViewCallback();
   m_paused_ipr = true;
}

void CRenderSession::UnPauseIPR()
{
   assert(AiUniverseIsActive());

   m_paused_ipr = false;
   CMayaScene::UpdateIPR();
   DoIPRRender();
}

AtUInt64 CRenderSession::GetUsedMemory()
{
   return AiMsgUtilGetUsedMemory() / 1024 / 1024;
}


void CRenderSession::AddIdleRenderViewCallback()
{
   MStatus status;
   if (m_idle_cb == 0)
   {
      m_idle_cb = MEventMessage::addEventCallback("idle",
                                                   CRenderSession::TransferTilesToRenderView,
                                                   NULL,
                                                   &status);
   }

   if (m_timer_cb == 0)
   {
      // This is called at ~24 times a second.
      m_timer_cb = MTimerMessage::addTimerCallback( 1.0f / 8.0f,
                                                    CRenderSession::RefreshRenderView,
                                                    NULL,
                                                    &status);
   }
}

void CRenderSession::ClearIdleRenderViewCallback()
{
   // Don't clear the callback if we're in the middle of a render.
   if (m_idle_cb != 0 || m_timer_cb != 0)
   {
      MRenderView::endRender();
      if (m_idle_cb != 0)
      {
         MMessage::removeCallback(m_idle_cb);
         m_idle_cb = 0;
      }
   
      if (m_timer_cb != 0)
      {
         MMessage::removeCallback(m_timer_cb);
         m_timer_cb = 0;
      }
   }
}

void CRenderSession::DoSwatchRender(const AtInt resolution)
{
   assert(AiUniverseIsActive());

   // Use the render view output driver. It will *not* be displayed
   // in the render view, we're just using the Arnold Node.
   // See DisplayUpdateQueueToMImage() for how we get the image.
   AtNode * const render_view = AiNode("renderview_display");
   AiNodeSetStr(render_view, "name", "swatch_renderview_display");

   MObject optNode = m_renderOptions.GetArnoldRenderOptions();
   AtFloat gamma =  optNode != MObject::kNullObj ? MFnDependencyNode(optNode).findPlug("display_gamma").asFloat() : 2.2f;
   AiNodeSetFlt(render_view, "gamma", gamma);

   AtNode * const filter = AiNode("gaussian_filter");
   AiNodeSetFlt(filter, "width", 2.0f);

   AtNode * const options     = AiUniverseGetOptions();

   // Create the single output line. No AOVs or anything.
   AtArray* outputs  = AiArrayAllocate(1, 1, AI_TYPE_STRING);
   AtChar   str[1024];
   sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(render_view));
   AiArraySetStr(outputs, 0, str);
   AiNodeSetArray(options, "outputs", outputs);

   // Most options should be read from an ass so just need to set the res and
   // guess a reasonable bucket size.
   AiNodeSetInt(options, "xres", resolution);
   AiNodeSetInt(options, "yres", resolution);
   AiNodeSetInt(options, "bucket_size", resolution/4);

   // Start the render thread.
   m_render_thread = AiThreadCreate(CRenderSession::RenderThread,
                                    &m_renderOptions,
                                    AI_PRIORITY_LOW);
}

bool CRenderSession::GetSwatchImage(MImage & image)
{
   assert(AiUniverseIsActive());

   if (CMayaScene::GetSessionMode() != MTOA_SESSION_SWATCH
         || NULL == m_render_thread)
   {
      return false;
   }
   
   // Wait for the swatch to finish.
   AiThreadWait(m_render_thread);

   // Store the image in the passed in MImage reference.
   return DisplayUpdateQueueToMImage(image);
}

