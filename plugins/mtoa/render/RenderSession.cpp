#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif


#include "platform/Platform.h"
#include "utils/Universe.h"
#include "utils/MtoaLog.h"
#include "utils/MayaUtils.h"
#include "RenderSession.h"
#include "RenderOptions.h"
#include "scene/MayaScene.h"
#include "translators/NodeTranslator.h"
#include "translators/DagTranslator.h"
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
#include <maya/MBoundingBox.h>
#include <maya/MConditionMessage.h>


#include <cstdio>
#include <assert.h>

#ifndef _WIN64
#include <unistd.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "display/renderview_mtoa.h"
static CRenderViewMtoA  *s_renderView = NULL;

static bool s_closeRenderViewWithSession = false;
bool is_playblasting = false;


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

      // load plugins from the render options' plugin_searchpath (#1391)
      AiLoadPlugins(m_renderOptions.GetPluginSearchPath().asChar());
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
   s_closeRenderViewWithSession = false;
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

void CRenderSession::CloseOtherViews(const MString& destination)
{
#ifndef MTOA_DISABLE_RV

   bool viewFound = false;
   M3dView thisView;

   if (destination.length() > 0)
   {
      // we are opening a viewport render override
      // close the ARV if it exists.
      if (s_renderView)
      {
         s_renderView->CloseRenderView();
         // This is what tells RenderviewMtoA that we're doing viewport rendering
         // and therefore that we should call "refresh -r" when render changes
         s_renderView->SetViewportRendering(true);
      }
      viewFound = (M3dView::getM3dViewFromModelPanel(destination, thisView) == MStatus::kSuccess);
   } 

   // Close all but the destination render override if there is one
   // If the destination is an empty string it is the ARV.
   for (unsigned int i = 0, viewCount = M3dView::numberOf3dViews(); i < viewCount; ++i)
   {
      M3dView view;
      M3dView::get3dView(i, view);
      if (view.renderOverrideName() == "arnoldViewOverride" &&
               (!viewFound || view.widget() != thisView.widget()))
         view.setRenderOverrideName("");
   }
#endif
}

void CRenderSession::RenderViewPlayblast(bool state, void * data)
{
   if (state)
   {
      is_playblasting = true;
   }
   else 
   {
      is_playblasting = false;
   }
}

MStatus CRenderSession::End()
{
   MStatus status = MStatus::kSuccess;

   if (IsRendering())
   {
      // IsRendering check prevents thread lock when CMayaScene::End is called
      // from InteractiveRenderThread
      InterruptRender();
 #ifndef MTOA_DISABLE_RV
      if (s_renderView)
      {
         if (s_closeRenderViewWithSession)
            s_renderView->CloseRenderView();
         else
            s_renderView->DisableRendering();
      }


 #endif
   }
   
   if (AiUniverseIsActive())
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
#ifndef MTOA_DISABLE_RV
   if (s_renderView != NULL)
   {
      delete s_renderView;
      s_renderView = NULL;
   }
#endif
}

AtBBox CRenderSession::GetBoundingBox()
{
   // We're no longer asking Arnold to dump the bounding box, because it required to start 
   // a render in "free" mode, so that subdivision and displacement were applied.
   // This gave us an exact bounding box, and it used to be necessary when load_at_init = false.
   // Now that load_at_init was removed, this bounding box information is just used for viewport display,
   // so we don't need it to be perfectly accurate, and we don't want its computation to be expensive.
   // So I'm now changing this, so that bounding boxes are computed by what Maya returns us.

   const ObjectToTranslatorMap &processedTranslators = CMayaScene::GetArnoldSession()->GetProcessedTranslators();
   ObjectToTranslatorMap::const_iterator it = processedTranslators.begin();
   ObjectToTranslatorMap::const_iterator itEnd = processedTranslators.end();

   MBoundingBox globalBox = MBoundingBox(); // creates an empty bounding box 
   for (; it != itEnd; ++it)
   {
      CNodeTranslator *translator = it->second;
      if (translator == NULL) continue;

      AtNode *node = translator->GetArnoldNode();
      if (node == NULL) continue;

      if (AiNodeEntryGetType(AiNodeGetNodeEntry(node)) != AI_NODE_SHAPE) continue; // only consider shapes

      CDagTranslator *dagTranslator = static_cast<CDagTranslator*>(translator);
      const MDagPath &dagPath = dagTranslator->GetMayaDagPath();
      if (!dagPath.isValid()) continue;

      MStatus status;
      MFnDagNode fnNode(dagPath, &status);
      if (status != MS::kSuccess) continue;

      MBoundingBox box = fnNode.boundingBox (&status);
      if (status != MS::kSuccess) continue;

      box.transformUsing(dagPath.inclusiveMatrix());

      globalBox.expand(box);
   }
   AtVector boxmin((float)globalBox.min()[0], (float)globalBox.min()[1], (float)globalBox.min()[2]);
   AtVector boxmax((float)globalBox.max()[0], (float)globalBox.max()[1], (float)globalBox.max()[2]);
   return AtBBox(boxmin, boxmax);
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
            AiRenderInterrupt(AI_BLOCKING);
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
#ifndef MTOA_DISABLE_RV
   if (s_renderView != NULL) 
   {
      s_renderView->InterruptRender();
   }
#endif

   if (IsRendering() && AiRendering())
      AiRenderInterrupt(waitFinished ? AI_BLOCKING : AI_NON_BLOCKING);
      
   // Wait for the thread to clear.
   if (m_render_thread != 0)
   {
      AiThreadWait(m_render_thread);
      AiThreadClose(m_render_thread);
      m_render_thread = 0;	
   }
}

bool CRenderSession::IsRegionCropped()
{
#ifndef MTOA_DISABLE_RV
   if (s_renderView)
   {
      std::string isCropped(s_renderView->GetOption("Crop Region"));
      return (isCropped == "1");
   }
#endif   
   return false;

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
   CRenderViewMtoA::SetCameraName(cameraNode.partialPathName());
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
                                 AiMin(num_aa_samples, renderSession->m_renderOptions.progressiveInitialLevel())
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
      if (MtoaTranslationInfo())
      {
         MString log = "[mtoa] Beginning progressive sampling at ";
         log += sampling;
         log += " AA of ";
         log += num_aa_samples;
         MtoaDebugLog(log);
      }
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

int CRenderSession::DoInteractiveRender()
{
   assert(AiUniverseIsActive());

   // Interrupt existing render and close rendering thread if any
   InterruptRender();

   //

   MAtomic::set(&s_renderingFinished, 0);
   m_render_thread = AiThreadCreate(CRenderSession::InteractiveRenderThread,
                                    this,
                                    AI_PRIORITY_LOW);

   int status = AI_SUCCESS;
   // DEBUG_MEMORY;
   // Block until the render finishes
   s_comp = new MComputation();
   s_comp->beginComputation();
   while (MAtomic::compareAndSwap(&s_renderingFinished, 1, 1) != 1)
   {
      if (s_comp->isInterruptRequested())
      {
         AiRenderInterrupt();
         status = AI_INTERRUPT;
      }
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

   return status;
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

void CRenderSession::DoAssWrite(MString customFileName, const bool compressed, bool writeBox)
{
   assert(AiUniverseIsActive());

   MString fileName;
   AtNode *options = AiUniverseGetOptions();

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
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] Exporting Maya scene to file \""+ fileName +"\"");

      // FIXME this is not the ideal place to set this, but given how renderOptions/sessionOptions are currently 
      // assembled, this is the best place to do it (#2995)
      if ((RenderOptions()->outputAssMask() & AI_NODE_COLOR_MANAGER) == 0)
      {
         AiNodeSetPtr(options, "color_manager", NULL);
         // Loop over all shaders + drivers having a parameter "color_space"
         AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(AI_NODE_SHADER | AI_NODE_DRIVER);
         static AtString colorSpaceStr("color_space");
         while (!AiNodeIteratorFinished(nodeIter))
         {
            AtNode *node = AiNodeIteratorGetNext(nodeIter);
            if (node == NULL) continue;
            if(AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(node), colorSpaceStr))
            {
               AiNodeSetStr(node, "color_space", "");
            }
         }
         AiNodeIteratorDestroy(nodeIter);
      }

      // Now save the metadata
      AtMetadataStore *mds = AiMetadataStore();

      // we're still adding this as an option because it could be expensive on big scenes
      AtBBox bBox = AI_BBOX_ZERO;
      if (writeBox)
      {
         bBox = GetBoundingBox();
         MString boundsStr;
         boundsStr += bBox.min.x;
         boundsStr += " ";
         boundsStr += bBox.min.y;
         boundsStr += " ";
         boundsStr += bBox.min.z;
         boundsStr += " ";
         boundsStr += bBox.max.x;
         boundsStr += " ";
         boundsStr += bBox.max.y;
         boundsStr += " ";
         boundsStr += bBox.max.z;
         AiMetadataStoreSetStr(mds, AtString("bounds"), boundsStr.asChar());
      }

      if (AiNodeLookUpUserParameter(options, "frame"))
         AiMetadataStoreSetFlt(mds, AtString("frame"), AiNodeGetFlt(options, "frame"));

      if (AiNodeLookUpUserParameter(options, "fps"))
         AiMetadataStoreSetFlt(mds, AtString("fps"), AiNodeGetFlt(options, "fps"));

      if (AiNodeLookUpUserParameter(options, "render_layer"))
         AiMetadataStoreSetStr(mds, AtString("render_layer"), AiNodeGetStr(options, "render_layer"));

      MString currentUser;
      MGlobal::executePythonCommand("import getpass; getpass.getuser();", currentUser);
      if (currentUser.length() > 0)
         AiMetadataStoreSetStr(mds, AtString("user"), currentUser.asChar());



      MString sceneFileName;
      MGlobal::executeCommand("file -q -sn", sceneFileName);
      if(sceneFileName.length() > 0)
         AiMetadataStoreSetStr(mds, AtString("scene"), sceneFileName.asChar());


      // FIXME : problem this is actually double filtering files
      // (Once at export to AiUniverse and once at file write from it)
      AiASSWriteWithMetadata(fileName.asChar(), m_renderOptions.outputAssMask(), m_renderOptions.expandProcedurals(), m_renderOptions.useBinaryEncoding(), mds);
      AiMetadataStoreDestroy(mds);

      if (writeBox && getenv("MTOA_EXPORT_ASSTOC"))
      {
         int extPos = fileName.rindexW('.');
         if (extPos < 0)
            fileName += ".asstoc";
         else
            fileName = fileName.substringW(0, extPos) + MString("asstoc");

         WriteAsstoc(fileName, bBox);
      }

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
      MFnDependencyNode optionsNode(CMayaScene::GetSceneArnoldRenderOptionsNode(), &status);
      if (status)
      {
         IPRRefinementStarted = optionsNode.findPlug("IPRRefinementStarted", true).asString();
         IPRRefinementFinished = optionsNode.findPlug("IPRRefinementFinished", true).asString();
         IPRStepStarted = optionsNode.findPlug("IPRStepStarted", true).asString();
         IPRStepFinished = optionsNode.findPlug("IPRStepFinished", true).asString();
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

/*
void CRenderSession::RunInteractiveRenderer()
{
#ifndef MTOA_DISABLE_RV
   if (s_renderView == NULL)
      s_renderView = new CRenderViewMtoA;

   s_renderView->SetViewportRendering(true);
   InterruptRender(); // clear the previous thread  
   SetRendering(true);

   s_renderView->Render();
#endif
}*/


void CRenderSession::PostDisplay()
{
#ifndef MTOA_DISABLE_RV
   if(s_renderView)
      s_renderView->PostDisplay();
#endif
}
bool CRenderSession::HasRenderResults(AtBBox2 &box)
{
#ifndef MTOA_DISABLE_RV
   if (s_renderView)
      return s_renderView->HasRenderResults(box);
 #endif
   return false;
}

void CRenderSession::RunRenderView()
{
   InterruptRender(); // clear the previous thread  
   SetRendering(true);
#ifndef MTOA_DISABLE_RV
   s_renderView->Render();
#endif
}

void CRenderSession::StartRenderView()
{
#ifndef MTOA_DISABLE_RV 
   if (s_renderView == NULL)
   {
      s_renderView = new CRenderViewMtoA;
   }
   // This is what tells RenderviewMtoA that we're doing viewport rendering
   // and therefore that we should call "refresh -r" when render changes
   s_renderView->SetViewportRendering(false);
   
   s_renderView->OpenMtoARenderView(m_renderOptions.width(), m_renderOptions.height());

   CArnoldSession *session = CMayaScene::GetArnoldSession();
   if (session)
      s_renderView->SetFrame((float)session->GetExportFrame());
#endif   
}

bool CRenderSession::IsViewportRendering()
{
   return s_renderView && s_renderView->IsViewportRendering();

}
const AtRGBA *CRenderSession::GetDisplayedBuffer(unsigned int *w, unsigned int *h)
{
#ifndef MTOA_DISABLE_RV
   if(s_renderView)
      return s_renderView->GetDisplayedBuffer(w, h);
#endif
   return NULL;


}
void CRenderSession::OpenInteractiveRendererOptions()
{
#ifndef MTOA_DISABLE_RV
   if (s_renderView == NULL)
   {
      s_renderView = new CRenderViewMtoA;
   }
   s_renderView->SetViewportRendering(true);
   s_renderView->OpenMtoAViewportRendererOptions();
#endif
}

void CRenderSession::UpdateRenderView()
{  
#ifndef MTOA_DISABLE_RV
   if(s_renderView != NULL) // for now always return true
   {
      if (is_playblasting)
      {
         s_renderView->SetOption("Wait Render", "1");
         SetProgressive(false);
      }
      else
      {
         SetProgressive(true);
      }
      // This will tell the render View that the scene has changed
      // it will decide whether to re-render or not
      s_renderView->SceneChanged();
   }
#endif
}

void CRenderSession::CloseRenderView()
{  
#ifndef MTOA_DISABLE_RV
   if(s_renderView != NULL) // for now always return true
   {
      // This will tell the render View that the scene has changed
      // it will decide whether to re-render or not
      s_renderView->CloseRenderView();
   }
#endif
}

void CRenderSession::CloseOptionsWindow()
{
#ifndef MTOA_DISABLE_RV
    if (s_renderView != NULL) // for now always return true
    {
      // This will tell the render View that the scene has changed
      // it will decide whether to re-render or not
      s_renderView->CloseOptionsWindow();
    }
#endif
}

void CRenderSession::FillRenderViewCameras()
{

   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();
   MString camerasList;
      
   M3dView view;
   MDagPath activeCameraPath;
   MStatus viewStatus;
   view = M3dView::active3dView(&viewStatus);
   MString viewCam;
   //CRenderSession *renderSession = CMayaScene::GetRenderSession();

   if (viewStatus == MS::kSuccess && view.getCamera(activeCameraPath) == MS::kSuccess)
   {      
 //     if (renderSession)
 //        camerasList = viewCam = CDagTranslator::GetArnoldNaming(activeCameraPath);
 //     else
      // Now we always export the short name. We'll let ARV find the corresponding AtNode based on the user data dcc_name
      camerasList = viewCam = activeCameraPath.partialPathName();
   }
   
   MDagPath path;
   MItDag   dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);
   
   MFnDagNode cameraNode;

   // FIXME dummy exportFilter that is needed in FilteredStatus
   CMayaExportFilter exportFilter;

   for (; (!dagIterCameras.isDone()); dagIterCameras.next())
   {
      if (dagIterCameras.getPath(path))
      {
         // we're exporting the cameras if they're *either* accepted by the filter status
         // *or* if they're set as renderable
         // FIXME the filter status is useless here, it's always set to MTOA_FILTER_ALL
         if(arnoldSession->FilteredStatus(path, &exportFilter) != MTOA_EXPORT_ACCEPTED)
         {
            // this camera is hidden, check if it's renderable
            MStatus stat;
            MFnDagNode cameraNode(path);
            MPlug renderable = cameraNode.findPlug("renderable", false, &stat);
            bool isRenderable = (stat == MS::kSuccess) ? renderable.asBool() : false;
            // Force the export of default persp camera for ARV (#3655)
            if (cameraNode.name() == MString("perspShape"))
               isRenderable = true;


            if (!isRenderable)
               continue;
         }
         // we can't call GetArnoldNaming if there's no active session
         MString camName = path.partialPathName();
         if (camName == viewCam) continue; // we've already set this camera in the list

         if (camerasList.length() > 0)
            camerasList += ";";

         camerasList += camName;
      }
   }
   // giving ARV the list of cameras
   SetRenderViewOption("Cameras", camerasList);
}

void CRenderSession::ObjectNameChanged(MObject& node, const MString& str)
{
   if (!CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW)) return;

   // in renderView mode, we must advert the renderview that an object name has changed
#ifndef MTOA_DISABLE_RV
   if (s_renderView != NULL)
   {
      MFnDependencyNode fnNode(node);
      const char *newName = fnNode.name().asChar();
      const char *oldName = str.asChar();
      s_renderView->ObjectNameChanged(newName, oldName);
   }
#endif
}


void CRenderSession::PauseIPR()
{
   assert(AiUniverseIsActive());

   InterruptRender();
   m_paused_ipr = true;
}

bool CRenderSession::IsIPRPaused()
{
#ifndef MTOA_DISABLE_RV

   if (s_renderView)
   {
      std::string isIPRRunning(s_renderView->GetOption("Run IPR"));
      return (isIPRRunning == "0");
   }
#endif
    return true;
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
#ifndef MTOA_DISABLE_RV
   if (s_renderView == NULL)
   {
      s_renderView = new CRenderViewMtoA;
   }
   
   s_renderView->SetOption(option.asChar(), value.asChar());
#endif
}
MString CRenderSession::GetRenderViewOption(const MString &option)
{
#ifndef MTOA_DISABLE_RV
   if (s_renderView)
   {
      std::string res(s_renderView->GetOption(option.asChar()));
      return MString(res.c_str());
   }
#endif
   return MString("");
}
void CRenderSession::SetRenderViewStatusInfo(const MString &info)
{
   if (s_renderView)
      s_renderView->SetStatusInfo(info.asChar());
}
bool CRenderSession::RenderSequence()
{   
   if (s_renderView == NULL || !CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW)) return false;

   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);
   float startFrame;
   float endFrame;
   float frameStep;

   if (renderGlobals.isAnimated())
   {
      startFrame = (float)renderGlobals.frameStart.as(MTime::uiUnit());
      endFrame = (float)renderGlobals.frameEnd.as(MTime::uiUnit());
      frameStep = (float)renderGlobals.frameBy;
   }
   else
   {
      startFrame = (float)MAnimControl::currentTime().as(MTime::uiUnit());
      endFrame = startFrame;
      frameStep = 1;
   }

   StartRenderView();
   SetRendering(true);

#ifndef MTOA_DISABLE_RV
   s_renderView->RenderSequence(startFrame, endFrame, frameStep);
#endif
   return true;
}

void CRenderSession::UpdateRenderOptions()
{
   m_renderOptions.GetFromMaya(); 
   m_renderOptions.SetupLog();

#ifndef MTOA_DISABLE_RV
   if (s_renderView)
   {
      s_renderView->SetLogging(m_renderOptions.GetLogConsoleVerbosity(), m_renderOptions.GetLogFileVerbosity());
   }
#endif
}
void CRenderSession::CloseRenderViewWithSession(bool b)
{
	s_closeRenderViewWithSession = b;
}
