
#include "platform/Platform.h"
#include "utils/Universe.h"
#include "utils/MtoaLog.h"
#include "utils/MayaUtils.h"
#include "RenderSession.h"
#include "RenderOptions.h"
#include "scene/MayaScene.h"
#include "translators/NodeTranslator.h"
#include "translators/options/OptionsTranslator.h"
#include "extension/Extension.h"

#include "display/renderview_mtoa.h"

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

#ifdef _LINUX
#include <unistd.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "display/renderview_mtoa.h"
static CRenderViewMtoA  *s_renderView = NULL;



extern AtNodeMethods* mtoa_driver_mtd;

MComputation*                       CRenderSession::s_comp = NULL;
MCallbackId                         CRenderSession::s_idle_cb = 0;
CRenderSession::RenderCallbackType  CRenderSession::m_renderCallback = NULL;
CCritSec                            CRenderSession::m_render_lock;

namespace{
   // TODO : remove this in the future
   // this is ugly, but we cannot
   // do it otherwise because we cannot
   // change the API
   static MString IPRRefinementStarted("");
   static MString IPRRefinementFinished("");
   static MString IPRStepStarted("");
   static MString IPRStepFinished("");
   static volatile int s_renderingFinished;
}

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

      // load plugins from the render options' shader_searchpath (#1391)
      AiLoadPlugins(m_renderOptions.GetShaderSearchPath().asChar());
   }
   
   m_is_active = AiUniverseIsActive() ? true : false;
   if (m_is_active)
   {
      InstallNodes();
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
   return MAtomic::compareAndSwap(&m_rendering, 1, 1) == 1;
}

// Sadly neither maya nor arnold have builtin 
// atomic operations for 64 bit integers
// so I need to use locks

void CRenderSession::SetCallback(RenderCallbackType callback)
{
   CCritSec::CScopedLock sc(m_render_lock);
   m_renderCallback = callback;
}

void CRenderSession::ClearCallback()
{
   CCritSec::CScopedLock sc(m_render_lock);
   m_renderCallback = NULL;
}

CRenderSession::RenderCallbackType CRenderSession::GetCallback()
{
   CCritSec::CScopedLock sc(m_render_lock);
   return m_renderCallback;
}  

MStatus CRenderSession::End()
{
   MStatus status = MStatus::kSuccess;

   if (IsRendering())
   {
      // IsRendering check prevents thread lock when CMayaScene::End is called
      // from InteractiveRenderThread
      InterruptRender();

      if (s_renderView)
      {
         s_renderView->CloseRenderView();
      } 

   }
   
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

   
   // Restore "out of rendering" logging
   MtoaSetupLogging();
   return status;
}
void CRenderSession::DeleteRenderView()
{

   if (s_renderView != NULL)
   {
      delete s_renderView;
      s_renderView = NULL;
   }

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
void CRenderSession::InteractiveRenderCallback(float elapsedTime, float lastTime, void *data)
{
   if (CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW) && data != 0)
   {

//    do I even need this ??   
//      ((CRenderSession*)data)->UpdateRenderView();
      return;
   }

   const bool rendering = AiRendering();
   if (rendering)
   {
      if (s_comp != 0)
      {
         if (s_comp->isInterruptRequested())
            AiRenderInterrupt();
         // This causes AiRender to break, after which the CMayaScene::End()
         // which clears this callback.      
         // Which callback is more useful: AiRenderAbort or AiRenderInterrupt?
         // AiRenderAbort will draw uncomplete buckets while AiRenderInterrupt will not.
         // AiRenderAbort();
      }
      else if (!CMayaScene::IsActive(MTOA_SESSION_IPR))
      {
         s_comp = new MComputation();
         s_comp->beginComputation();
         s_comp->endComputation();
      }
   }
   
   
   CCritSec::CScopedLock sc(m_render_lock);
   if (m_renderCallback != 0)
   {
      if (s_comp == 0 && !CMayaScene::IsActive(MTOA_SESSION_IPR))
      {
         s_comp = new MComputation();
         s_comp->beginComputation();
         s_comp->endComputation();
      }
      m_renderCallback();
   }
   else if (!rendering && s_comp != 0)
   {
      s_comp->endComputation();
      delete s_comp;
      s_comp = 0;
   }
}

void CRenderSession::InterruptRender(bool waitFinished)
{
   if (s_renderView != NULL) 
   {
      s_renderView->InterruptRender();
   }
   if (IsRendering() && AiRendering()) AiRenderInterrupt();
      
   if (waitFinished)
   {
#ifdef _WIN64
      while(AiRendering()) Sleep(1);
#else
      while(AiRendering()) usleep(1000);
#endif

   }
   // Wait for the thread to clear.
   if (m_render_thread != 0)
   {
      AiThreadWait(m_render_thread);
      AiThreadClose(m_render_thread);
      m_render_thread = 0;	
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
}

void CRenderSession::SetRenderViewPanelName(const MString &panel)
{
   m_renderOptions.SetRenderViewPanelName(panel);
}



unsigned int CRenderSession::ProgressiveRenderThread(void* data)
{
   CRenderSession * renderSession = static_cast< CRenderSession * >(data);
   // set progressive start point on AA
   int num_aa_samples = AiNodeGetInt(AiUniverseGetOptions(), "AA_samples");
   if (num_aa_samples == 0)
      num_aa_samples = 1;
   const int progressive_start = renderSession->m_renderOptions.isProgressive() ? 
                                 MIN(num_aa_samples, renderSession->m_renderOptions.progressiveInitialLevel())
                                 : num_aa_samples;
   int ai_status(AI_SUCCESS);
   CMayaScene::ExecuteScript(IPRRefinementStarted, false, true);
   renderSession->SetRendering(true);
   for (int sampling = progressive_start; sampling <= num_aa_samples; ++sampling)
   {
      if (sampling == 0)
         continue;
      else if (sampling > 1)
         sampling = num_aa_samples;

      AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", sampling);
      // Begin a render!
      AiMsgInfo("[mtoa] Beginning progressive sampling at %d AA of %d AA", sampling, num_aa_samples);
      CMayaScene::ExecuteScript(IPRStepStarted, false, true);
      ai_status = AiRender(AI_RENDER_MODE_CAMERA);
      CMayaScene::ExecuteScript(IPRStepFinished, false, true);

      if (ai_status != AI_SUCCESS) break;
   }
   // Put this back after we're done interating through.
   AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", num_aa_samples);
   renderSession->SetRendering(false);
   CMayaScene::ExecuteScript(IPRRefinementFinished, false, true);

   return ai_status;
}



unsigned int CRenderSession::InteractiveRenderThread(void* data)
{
   MAtomic::set(&s_renderingFinished, 0);
   CRenderSession * renderSession = static_cast< CRenderSession * >(data);

   if (renderSession->m_renderOptions.isProgressive())
      ProgressiveRenderThread(data);      
   else
   {
      renderSession->SetRendering(true);
      AiRender(AI_RENDER_MODE_CAMERA);
      renderSession->SetRendering(false);
   }
   
   // don't echo, and do on idle
   
   MAtomic::set(&s_renderingFinished, 1);
   return 0;
}

void CRenderSession::DoInteractiveRender(const MString& postRenderMel)
{
   assert(AiUniverseIsActive());

   // Interrupt existing render and close rendering thread if any
   InterruptRender();

   //

   MAtomic::set(&s_renderingFinished, 0);
   m_render_thread = AiThreadCreate(CRenderSession::InteractiveRenderThread,
                                    this,
                                    AI_PRIORITY_LOW);
   // DEBUG_MEMORY;
   // Block until the render finishes
   s_comp = new MComputation();
   s_comp->beginComputation();
   while (MAtomic::compareAndSwap(&s_renderingFinished, 1, 1) != 1)
   {
      if (s_comp->isInterruptRequested())
         AiRenderInterrupt();
      CCritSec::CScopedLock sc(m_render_lock);
      if (m_renderCallback != 0)
         m_renderCallback();
#ifdef WIN32
      Sleep(0);
#else
      sleep(0);
#endif
   }
   s_comp->endComputation();
   delete s_comp;
   s_comp = 0;

   if (m_render_thread != 0)
   {
      AiThreadWait(m_render_thread);
      AiThreadClose(m_render_thread);
      m_render_thread = 0;
   } 

   CMayaScene::End();

   CMayaScene::ExecuteScript(postRenderMel, false, true);   
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
      MString ext = sceneFileName.substringW(nscn-3, nscn-1);
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
      AiASSWrite(fileName.asChar(), m_renderOptions.outputAssMask(), m_renderOptions.expandProcedurals(), m_renderOptions.useBinaryEncoding());
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

      MStatus status;
      // TODO : move this to a shared class later
      // we have to query this here because
      // we are not allowed to change the API for the next release
      MFnDependencyNode optionsNode(CMayaScene::GetSceneArnoldRenderOptionsNode(), &status);
      if (status)
      {
         IPRRefinementStarted = optionsNode.findPlug("IPRRefinementStarted").asString();
         IPRRefinementFinished = optionsNode.findPlug("IPRRefinementFinished").asString();
         IPRStepStarted = optionsNode.findPlug("IPRStepStarted").asString();
         IPRStepFinished = optionsNode.findPlug("IPRStepFinished").asString();
      }
      else
      {
         IPRRefinementStarted = "";
         IPRRefinementFinished = "";
         IPRStepStarted = "";
         IPRStepFinished = "";
      }

      // Start the render thread.
      m_render_thread = AiThreadCreate(CRenderSession::ProgressiveRenderThread,
                                       this,
                                       AI_PRIORITY_LOW);


   }
}


void CRenderSession::RunRenderView()
{
   InterruptRender(); // clear the previous thread  
   SetRendering(true);
   s_renderView->Render();
}

void CRenderSession::StartRenderView()
{
   if (s_renderView == NULL)
   {
      s_renderView = new CRenderViewMtoA;
   }
   s_renderView->OpenMtoARenderView(m_renderOptions.width(), m_renderOptions.height());
   s_renderView->SetFrame((float)CMayaScene::GetArnoldSession()->GetExportFrame());

}

void CRenderSession::UpdateRenderView()
{  
   if(s_renderView != NULL) // for now always return true
   {
      // This will tell the render View that the scene has changed
      // it will decide whether to re-render or not
      s_renderView->SceneChanged();
   }

}

void CRenderSession::ObjectNameChanged(MObject& node, const MString& str)
{
   if (!CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW)) return;

   // in renderView mode, we must advert the renderview that an object name has changed
   if (s_renderView != NULL)
   {
      MFnDependencyNode fnNode(node);
      const char *newName = fnNode.name().asChar();
      const char *oldName = str.asChar();
      s_renderView->ObjectNameChanged(newName, oldName);
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
   MMessage::removeCallback(s_idle_cb);
   s_idle_cb = 0;
   MStatus status;

   s_idle_cb = MTimerMessage::addTimerCallback(0.01f,
                                               CRenderSession::InteractiveRenderCallback,
                                               0,
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
                                                  0,
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
   AtNode* render_view = AiNode("renderview_display");
   AiNodeSetStr(render_view, "name", "swatch_renderview_display");

   AiNodeSetPtr(render_view, "swatch", image.floatPixels());

   MObject optNode = m_renderOptions.GetArnoldRenderOptions();
#ifdef MTOA_ENABLE_GAMMA
   float gamma =  optNode != MObject::kNullObj ? MFnDependencyNode(optNode).findPlug("display_gamma").asFloat() : 2.2f;
#else
   float gamma = 1.f;
#endif

   AiNodeSetFlt(render_view, "gamma", gamma);

   AtNode* filter = AiNode("gaussian_filter");
   AiNodeSetStr(filter, "name", "swatch_renderview_filter");
   AiNodeSetFlt(filter, "width", 2.0f);

   AtNode* options     = AiUniverseGetOptions();

   COptionsTranslator::AddProjectFoldersToSearchPaths(options);
   AiNodeDeclare(options, "is_swatch", "constant BOOL");
   AiNodeSetBool(options, "is_swatch", true);
   AiNodeSetStr(options, "pin_threads", "off");
   AiNodeSetInt(options, "threads", 4);

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
   AiNodeSetInt(options, "GI_sss_samples", 4);


   // Close existing render if any
   InterruptRender();

   // Start the render on the current thread.
   AiRender(AI_RENDER_MODE_CAMERA);
}
void CRenderSession::SetRenderViewOption(const MString &option, const MString &value)
{
   if (s_renderView == NULL)
   {
      s_renderView = new CRenderViewMtoA;
   }
   s_renderView->SetOption(option.asChar(), value.asChar());
}


