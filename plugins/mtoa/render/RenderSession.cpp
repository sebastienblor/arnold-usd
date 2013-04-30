#include "platform/Platform.h"
#include "utils/Universe.h"
#include "utils/MtoaLog.h"
#include "utils/MayaUtils.h"
#include "RenderSession.h"
#include "RenderOptions.h"
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
#include <maya/MEventMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MTimerMessage.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h>
#include <maya/MStatus.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MImage.h>
#include <maya/MFileObject.h>
#include <maya/M3dView.h>
#include <maya/MAtomic.h>

#include <cstdio>
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern AtNodeMethods* mtoa_driver_mtd;

MComputation*                       CRenderSession::s_comp = NULL;
MCallbackId                         CRenderSession::s_idle_cb = NULL;
CRenderSession::RenderCallbackType  CRenderSession::m_renderCallback = NULL;
MCallbackId                         CRenderSession::m_render_cb = NULL;

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

MStatus CRenderSession::Begin(const CRenderOptions &options)
{
   if (AiUniverseIsActive())
   {
      AiMsgWarning("[mtoa] There can only be one RenderSession active.");
      InterruptRender();
      if (options.m_forceTextureCacheFlushAfterRender)
      {
         ArnoldUniverseEndAndFlush(AI_CACHE_TEXTURE);
      }
      else
      {
         ArnoldUniverseEnd();
      }
   }

   // Begin the Arnold universe, read metadata file and load plugins
   if (ArnoldUniverseOnlyBegin())
   {
      //Set the user defined log file
      m_renderOptions = options;
      m_renderOptions.SetupLog();
      ArnoldUniverseLoadPluginsAndMetadata();
   }
   
   m_is_active = AiUniverseIsActive() ? true : false;
   if (m_is_active)
   {
      InstallNodes();
      AiCritSecInit(&m_render_lock);
      return MStatus::kSuccess;
   }
   else
   {
      AiMsgError("[mtoa] Could not initialize the Arnold universe in CRenderSession.Begin(CRenderOptions* options)");
      return MStatus::kFailure;
   }
}

void CRenderSession::SetRendering(bool renderState)
{
   MAtomic::set(&m_rendering, renderState ? 1 : 0);
}

bool CRenderSession::IsRendering()
{
   return MAtomic::compareAndSwap(&m_rendering, 1, 1);
}

void CRenderSession::SetCallback(RenderCallbackType callback)
{
   m_renderCallback = callback;
}

void CRenderSession::ClearCallbackId()
{
   m_renderCallback = NULL;
   m_render_cb = 0;
   if(s_comp != NULL)
   {
      s_comp->endComputation();
      delete s_comp;
      s_comp = NULL;
   }
}

MCallbackId CRenderSession::GetCallbackId()
{
   return m_render_cb;
}

   

MStatus CRenderSession::End()
{
   MStatus status = MStatus::kSuccess;

   if (IsRendering())
      // IsRendering check prevents thread lock when CMayaScene::End is called
      // from InteractiveRenderThread
      InterruptRender();

   if (!AiUniverseIsActive())
   {
      AiMsgWarning("[mtoa] No active Arnold Universe present.");
   }
   else
   {
      if (m_renderOptions.m_forceTextureCacheFlushAfterRender)
      {
         ArnoldUniverseEndAndFlush(AI_CACHE_TEXTURE);
      }
      else
      {
         ArnoldUniverseEnd();
      }
   }
   m_is_active = false;
   AiCritSecClose(&m_render_lock);
   m_render_lock = NULL;
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

/// This static function runs on the main thread and checks for render interrupts (pressing the Esc key) and
///  process the method provided to CRenderSession::SetCallback() in the driver.
void CRenderSession::InteractiveRenderCallback(void *data)
{
   if (s_comp != NULL && s_comp->isInterruptRequested() && AiRendering())
   {
      s_comp->endComputation();
      delete s_comp;
      s_comp = NULL;
      // This causes AiRender to break, after which the CMayaScene::End()
      // which clears this callback.
      AiRenderInterrupt();
      // Which callback is more useful: AiRenderAbort or AiRenderInterrupt?
      // AiRenderAbort will draw uncomplete buckets while AiRenderInterrupt will not.
      // AiRenderAbort();
   }
   
   if (m_render_cb == 0 && m_renderCallback != NULL)
   {
      if(s_comp != NULL)
      {
         s_comp->endComputation();
         delete s_comp;
      }
      s_comp = new MComputation();
      s_comp->beginComputation();
      m_render_cb = MTimerMessage::addTimerCallback(0.01,
                                                    m_renderCallback,
                                                    NULL);
      s_comp->endComputation();
   }
      
   return;
}

void CRenderSession::InterruptRender()
{
   if (IsRendering() && AiRendering())
   {
      AiRenderInterrupt();
   }

   // Wait for the thread to clear.
   if (m_render_thread != NULL)
   {
      AiThreadWait(m_render_thread);
      AiThreadClose(m_render_thread);
      m_render_thread = NULL;	
   }
}

void CRenderSession::SetResolution(const int width, const int height)
{
   if (width != -1) m_renderOptions.SetWidth(width);
   if (height != -1) m_renderOptions.SetHeight(height);
}

void CRenderSession::SetRegion(const unsigned int left, const unsigned int right,
                                const unsigned int bottom, const unsigned int top)
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

MString CRenderSession::GetRenderViewPanelName() const
{
   return m_renderOptions.GetRenderViewPanelName();
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
         for(unsigned int ips = 0; (ips < imagePlanePlug.numElements()); ips++)
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
               int visibility = 0;
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

void CRenderSession::SetRenderViewPanelName(const MString &panel)
{
   m_renderOptions.SetRenderViewPanelName(panel);
}

unsigned int CRenderSession::ProgressiveRenderThread(void* data)
{
   CRenderSession * renderSession = static_cast< CRenderSession * >(data);
   // set progressive start point on AA
   const int num_aa_samples = AiNodeGetInt(AiUniverseGetOptions(), "AA_samples");
   const int progressive_start = MIN(num_aa_samples, renderSession->m_renderOptions.progressiveInitialLevel());
   const int steps = (progressive_start < 0) ? abs(progressive_start) + 1 : 1;
   int ai_status(AI_SUCCESS);
   renderSession->SetRendering(true);
   int sampling, i;
   for (sampling = progressive_start, i=1; sampling <= num_aa_samples; ++sampling, ++i)
   {
      if (sampling >= 0)
         sampling = num_aa_samples;

      AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", sampling);
      // Begin a render!
      AiMsgInfo("[mtoa] Beginning progressive sampling at %d AA (step %d of %d)", sampling, i, steps);
      ai_status = AiRender(AI_RENDER_MODE_CAMERA);

      if (ai_status != AI_SUCCESS) break;
      if (sampling > 0) break;
   }
   // Put this back after we're done interating through.
   AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", num_aa_samples);
   renderSession->SetRendering(false);

   return ai_status;
}

unsigned int CRenderSession::InteractiveRenderThread(void* data)
{
   CRenderSession * renderSession = static_cast< CRenderSession * >(data);

   if (renderSession->m_renderOptions.isProgressive())
      ProgressiveRenderThread(data);
      
   else
   {
      renderSession->SetRendering(true);
      AiRender(AI_RENDER_MODE_CAMERA);
      renderSession->SetRendering(false);
   }
   // get the post-MEL before ending the MayaScene
   MString postMel = renderSession->m_postRenderMel;
   renderSession->m_postRenderMel = "";

   CMayaScene::End();

   // don't echo, and do on idle
   CMayaScene::ExecuteScript(postMel, false, true);
   return 0;
}

void CRenderSession::DoInteractiveRender(const MString& postRenderMel)
{
   assert(AiUniverseIsActive());

   // Interrupt existing render and close rendering thread if any
   InterruptRender();

   AddIdleRenderViewCallback(postRenderMel);
   
   m_render_thread = AiThreadCreate(CRenderSession::InteractiveRenderThread,
                                    this,
                                    AI_PRIORITY_LOW);
   // DEBUG_MEMORY;
}


int CRenderSession::DoBatchRender()
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

   MString filename =  m_renderOptions.outputAssFile().expandEnvironmentVariablesAndTilde();

   MString path = "";
   if (customName.numChars())
      path = customName;
   else if (filename.numChars())
      path = filename;

   MString assFileName = getFileName(MCommonRenderSettingsData::kFullPathImage,
                                      frameNumber,
                                      sceneFileName,
                                      cameraName,
                                      fileFormat,
                                      layer, "",
                                      createDirectory,
                                      "ASS",
                                      path,
                                      &isSequence);

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
      AiASSWrite(fileName.asChar(), m_renderOptions.outputAssMask(), m_renderOptions.expandProcedurals());
   }
}

void CRenderSession::DoIPRRender()
{
   assert(AiUniverseIsActive());

   if (!m_paused_ipr)
   {
      // Interrupt existing render if any
      InterruptRender();

      // DEBUG_MEMORY;
      AddIdleRenderViewCallback("");

      // Start the render thread.
      m_render_thread = AiThreadCreate(CRenderSession::ProgressiveRenderThread,
                                       this,
                                       AI_PRIORITY_LOW);


   }
}

void CRenderSession::StopIPR()
{
   assert(AiUniverseIsActive());

   InterruptRender();
}

void CRenderSession::PauseIPR()
{
   assert(AiUniverseIsActive());

   InterruptRender();
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

/// Replace the idle event installed by AddIdleRenderViewCallback with one that
/// actually does the interrupt checking.
void CRenderSession::DoAddIdleRenderViewCallback(void* data)
{
   CRenderSession * renderSession = static_cast< CRenderSession * >(data);

   MMessage::removeCallback(renderSession->s_idle_cb);
   MStatus status;

   renderSession->s_idle_cb = MEventMessage::addEventCallback("idle",
                                                CRenderSession::InteractiveRenderCallback,
                                                (void*)data,
                                                &status);
}

// there is a very strange bug with Maya where MComputation will lock up the GUI if
// it is created somewhere below a MEL script that returns a value.  We sidestep this problem
// by using this idle event callback to setup the MComputation. The callback runs only once.
void CRenderSession::AddIdleRenderViewCallback(const MString& postRenderMel)
{
   MStatus status;
   m_postRenderMel = postRenderMel;
   if(s_idle_cb == 0)
   {
      s_idle_cb = MEventMessage::addEventCallback("idle",
                                                CRenderSession::DoAddIdleRenderViewCallback,
                                                this,
                                                &status);
   }
}

void CRenderSession::ClearIdleRenderViewCallback()
{
   // Don't clear the callback if we're in the middle of a render.
   if (s_idle_cb != 0)
   {
      MMessage::removeCallback(s_idle_cb);
      s_idle_cb = 0;
   }
}

void CRenderSession::DoSwatchRender(MImage & image, const int resolution)
{
   assert(AiUniverseIsActive());

   // Use the render view output driver. It will *not* be displayed
   // in the render view, we're just using the Arnold Node.
   // See DisplayUpdateQueueToMImage() for how we get the image.
   AtNode * const render_view = AiNode("renderview_display");
   AiNodeSetStr(render_view, "name", "swatch_renderview_display");

   AiNodeSetPtr(render_view, "swatch", image.floatPixels());

   MObject optNode = m_renderOptions.GetArnoldRenderOptions();
   float gamma =  optNode != MObject::kNullObj ? MFnDependencyNode(optNode).findPlug("display_gamma").asFloat() : 2.2f;
   AiNodeSetFlt(render_view, "gamma", gamma);

   AtNode * const filter = AiNode("gaussian_filter");
   AiNodeSetStr(filter, "name", "swatch_renderview_filter");
   AiNodeSetFlt(filter, "width", 2.0f);

   AtNode * const options     = AiUniverseGetOptions();

   // Create the single output line. No AOVs or anything.
   AtArray* outputs  = AiArrayAllocate(1, 1, AI_TYPE_STRING);
   char   str[1024];
   sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(render_view));
   AiArraySetStr(outputs, 0, str);
   AiNodeSetArray(options, "outputs", outputs);

   // Most options should be read from an ass so just need to set the res and
   // guess a reasonable bucket size.
   AiNodeSetInt(options, "xres", resolution);
   AiNodeSetInt(options, "yres", resolution);
   AiNodeSetInt(options, "bucket_size", resolution/4);

   // Close existing render if any
   InterruptRender();

   // Start the render on the current thread.
   AiRender(AI_RENDER_MODE_CAMERA);
}

