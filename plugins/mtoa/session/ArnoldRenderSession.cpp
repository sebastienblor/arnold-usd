#include "ArnoldRenderSession.h"
#include <maya/MGlobal.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h>
#include <maya/M3dView.h>
#include <maya/MComputation.h>
#include <maya/MRenderView.h>
#include <maya/MTimerMessage.h>
#include "utils/MayaUtils.h"
#include "utils/MtoaLog.h"
#include <ai.h>
#include "utils/MtoAAdpPayloads.h"
#include "platform/Platform.h"
#ifndef _WIN64
#include <unistd.h>
#endif
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CArnoldRenderSession::CArnoldRenderSession() : 
                           CArnoldSession(),
                           m_renderCallback(nullptr),
                           m_driverNode(nullptr),
                           m_displayProgress(false),
                           m_pausedIPR(false),
                           m_IPRIdleCallbackId(0),
                           m_renderThread(nullptr),
                           m_idle_cb(0),
                           m_comp(0)
{
}
CArnoldRenderSession::~CArnoldRenderSession()
{
   if (m_idle_cb)
   {
      MMessage::removeCallback(m_idle_cb);
      m_idle_cb = 0;
   }

   if (m_renderThread)
   {    
      if (m_renderSession)
         AiRenderInterrupt(m_renderSession, AI_BLOCKING);
      AiThreadWait(m_renderThread);
      AiThreadClose(m_renderThread);
      m_renderThread = nullptr; 
   }

}

bool CArnoldRenderSession::BatchRender()
{
   AtRenderSession *renderSession = GetRenderSession();
   m_sessionOptions.SetupLog(renderSession);
   AiRenderSetHintStr(renderSession, AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_BATCH);
   // Here we just want a final frame render, no progressive (MTOA-909)
   AiRenderSetHintBool(renderSession, AtString("progressive"), false);

   MString filename;

   if (m_displayProgress && m_optionsTranslator)
   {
      MStringArray imageFilenames = m_optionsTranslator->GetActiveImageFilenames();
      if (imageFilenames.length() > 0)
         filename = imageFilenames[0];
   }
   if (m_displayProgress)
      MRenderUtil::sendRenderProgressInfo(filename, -1111); // magic number for start

   float lastProgress = -1;
   AiRenderBegin(renderSession, AI_RENDER_MODE_CAMERA);
   while (true)
   {
      AtRenderStatus status = AiRenderGetStatus(renderSession);
      if (status == AI_RENDER_STATUS_FINISHED)
      {
         AiRenderEnd(renderSession);
         return true;
      }
      if (status == AI_RENDER_STATUS_FAILED)
      {
         AiRenderEnd(renderSession);
         return false;
      }
#ifdef WIN32
      Sleep(0);
#else
      sleep(0);
#endif
      if (m_displayProgress)
      {
         float progress = 0;
         static AtString total_progress_str("total_progress");
         AiRenderGetHintFlt(renderSession, total_progress_str, progress);
         if (progress != lastProgress)
         {
            lastProgress = progress;
            MRenderUtil::sendRenderProgressInfo(filename, (int)progress);
         }
      }
   }
   if (m_displayProgress)
      MRenderUtil::sendRenderProgressInfo(filename, 100); // magic number for end

}

void CArnoldRenderSession::Clear()
{
   if (m_renderThread)
   {    
      if (m_renderSession)
         AiRenderInterrupt(m_renderSession, AI_BLOCKING);
      AiThreadWait(m_renderThread);
      AiThreadClose(m_renderThread);
      m_renderThread = nullptr; 
   }
   m_driverNode = nullptr;
   CArnoldSession::Clear();

}

bool CArnoldRenderSession::Render()
{
   AtRenderSession *renderSession = GetRenderSession();
   AiRenderInterrupt(renderSession, AI_BLOCKING);
   MComputation comp;
   comp.beginComputation();
   m_sessionOptions.SetupLog(renderSession);

   AiRenderSetHintStr(renderSession, AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_INTERACTIVE);
   // Here we just want a final frame render, no progressive (MTOA-909)
   AiRenderSetHintBool(renderSession, AtString("progressive"), false);
   
   AiRenderBegin(renderSession);
   float lastProgress = -1.f;

   MString filename;

   if (m_displayProgress && m_optionsTranslator)
   {
      MStringArray imageFilenames = m_optionsTranslator->GetActiveImageFilenames();
      if (imageFilenames.length() > 0)
         filename = imageFilenames[0];
   }
   if (m_displayProgress)
      MRenderUtil::sendRenderProgressInfo(filename, -1111); // magic number for start

   bool success = true;
   while(true)
   {
      if (comp.isInterruptRequested())
      {
         AiRenderInterrupt(renderSession, AI_BLOCKING);
         MGlobal::displayError("[mtoa] Render aborted");
         if (m_displayProgress)
            MRenderUtil::sendRenderProgressInfo(filename, -111); // magic number for abort/kill

         break;
      }
      AtRenderStatus status = AiRenderGetStatus(renderSession);
      if (status == AI_RENDER_STATUS_FINISHED)
         break;
      if (status == AI_RENDER_STATUS_FAILED)
      {
         success = false;
         break;
      }

#ifdef WIN32
      Sleep(0);
#else
      sleep(0);
#endif
      if (m_displayProgress)
      {
         float progress = 0;
         static AtString total_progress_str("total_progress");
         AiRenderGetHintFlt(renderSession, total_progress_str, progress);
         if (progress != lastProgress)
         {
            lastProgress = progress;
            MRenderUtil::sendRenderProgressInfo(filename, (int)progress);
         }
      }
      std::lock_guard<AtMutex> guard(m_renderLock);
      if (m_renderCallback != 0)
         m_renderCallback(m_driverNode);
   }
   comp.endComputation();
   if (m_renderCallback != 0)
      m_renderCallback(m_driverNode);

   AiRenderEnd(renderSession);
   if (m_displayProgress)
      MRenderUtil::sendRenderProgressInfo(filename, 100); // magic number for end
 
   return success;
}

void CArnoldRenderSession::SetRenderCallback(RenderCallbackType callback, AtNode *driverNode)
{   
   std::lock_guard<AtMutex> guard(m_renderLock);
   m_renderCallback = callback;
   m_driverNode = driverNode;
}
CArnoldRenderSession::RenderCallbackType CArnoldRenderSession::GetRenderCallback()
{
   RenderCallbackType res = nullptr;
   {
      std::lock_guard<AtMutex> guard(m_renderLock);
      res = m_renderCallback;
   }
   return res;

}
MStringArray CArnoldRenderSession::GetActiveImageFilenames() const
{
   return (m_optionsTranslator) ? m_optionsTranslator->GetActiveImageFilenames() : MStringArray();
}

static void IPRIdleCallback(void *clientData)
{
   CArnoldRenderSession *session = (CArnoldRenderSession *)clientData;
   session->UpdateIPRCallback();
}

void CArnoldRenderSession::UpdateIPRCallback()
{
   // Desactivate the callback (it's supposed to fire only once)
   if (m_IPRIdleCallbackId != 0)
   {
      MMessage::removeCallback(m_IPRIdleCallbackId);
      m_IPRIdleCallbackId = 0;
   }

   // Check that an update is really needed.
   if (m_requestUpdate)
   {
      InterruptRender(); // blocking

      MFnDependencyNode fnArnoldRenderOptions(m_sessionOptions.GetArnoldRenderOptions());
      bool forceUpdate = fnArnoldRenderOptions.findPlug("force_scene_update_before_IPR_refresh", true).asBool();
            
      if(forceUpdate)
      {
         UpdateFullScene();
      }
      else
      {
         Update();
      }
      
      IPR();
   }
}

void CArnoldRenderSession::RequestUpdate()
{
   CArnoldSession::RequestUpdate();
   MStatus status;
   MCallbackId id;

   // Add the IPR update callback, this is called in Maya's idle time
   if ( m_IPRIdleCallbackId == 0)// && !s_renderSession->IsIPRPaused() )
   {
      id = MEventMessage::addEventCallback("idle", IPRIdleCallback, this, &status);
      if (status == MS::kSuccess) m_IPRIdleCallbackId = id;
   }


}

/*
AtRenderStatus ArnoldIPRCallback(void *private_data, AtRenderUpdateType update_type, const AtRenderUpdateInfo *update_info)
{
   AtRenderStatus status = AI_RENDER_STATUS_RENDERING;
   if (update_type == AI_RENDER_UPDATE_FINISHED)
      status = AI_RENDER_STATUS_FINISHED;
   else if (update_type == AI_RENDER_UPDATE_INTERRUPT)
      status = AI_RENDER_STATUS_PAUSED;
   else if (update_type == AI_RENDER_UPDATE_ERROR)
      status = AI_RENDER_STATUS_FAILED;

   CArnoldRenderSession *session = (CArnoldRenderSession *)private_data;
   if (session)
      session->InvokeRenderCallback();

   return status;
}*/
/*
static unsigned int DoIPRThreadLoop(void *data)
{
   CArnoldRenderSession *session = (CArnoldRenderSession*)data;
   return session->IPRThreadLoop();

}

unsigned int CArnoldRenderSession::IPRThreadLoop()
{
   while(true)
   {
      if (m_renderSession == nullptr)
         break;
      AtRenderStatus status = AiRenderGetStatus(m_renderSession);
      if (status == AI_RENDER_STATUS_FINISHED || status == AI_RENDER_STATUS_FAILED)
         break;
#ifdef WIN32
      Sleep(0);
#else
      sleep(0);
#endif
      std::lock_guard<AtMutex> guard(m_renderLock);
      if (m_renderCallback != 0)
         m_renderCallback(m_driverNode);

   }
   return 0;
}
*/
void CArnoldRenderSession::IPR()
{
   if (m_pausedIPR)
      return;

   if (m_renderSession)
   {
      AiRenderInterrupt(m_renderSession, AI_BLOCKING);
   }

   if (m_renderThread)
   {    
      AiThreadWait(m_renderThread);
      AiThreadClose(m_renderThread);
      m_renderThread = nullptr; 
   }
   AtRenderSession *renderSession = GetRenderSession();

   m_sessionOptions.SetupLog(renderSession);

   if (AiRenderGetStatus(renderSession) == AI_RENDER_STATUS_NOT_STARTED)
   {
      AiRenderSetHintBool(renderSession, AtString("progressive"), m_sessionOptions.IsProgressive());
      int minAA = AiMin(1, m_sessionOptions.progressiveInitialLevel());
      AiRenderSetHintInt(renderSession, AtString("progressive_min_AA_samples"), minAA);
      AiRenderSetHintStr(renderSession, AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_INTERACTIVE);
    
      AiRenderBegin(renderSession);//, AI_RENDER_MODE_CAMERA, ArnoldIPRCallback, (void*)this);
      // start a thread that listens to 
   } else
   {
      AiRenderRestart(renderSession);
   }

   AddIdleRenderViewCallback("");
 // FIXME missing IPRStepStarted, etc...
}
void CArnoldRenderSession::PauseIPR()
{
   InterruptRender();
   m_pausedIPR = true;

}

static void DoInteractiveRenderCallback(float elapsedTime, float lastTime, void *data)
{
   CArnoldRenderSession *session = (CArnoldRenderSession *)data;
   if (session)
      session->InteractiveRenderCallback(elapsedTime, lastTime);

}

/// This static function runs periodically on the main thread and checks for render interrupts (pressing the Esc key) and
///  process the method provided to CRenderSession::SetCallback() in the driver.
void CArnoldRenderSession::InteractiveRenderCallback(float elapsedTime, float lastTime)
{
   
   const bool rendering = (m_renderSession) ? AiRenderGetStatus(m_renderSession) == AI_RENDER_STATUS_RENDERING : false;
   if (rendering)
   {
      if (m_comp)
      {
         if (m_comp->isInterruptRequested())
            AiRenderInterrupt(m_renderSession, AI_BLOCKING);
      }
      else
      {
         m_comp = new MComputation();
         m_comp->beginComputation();
         m_comp->endComputation();
      }
   }
   
   std::lock_guard<AtMutex> guard(m_renderLock);
   if (m_renderCallback)
   {
      if (m_comp == 0)
      {
         m_comp = new MComputation();
         m_comp->beginComputation();
         m_comp->endComputation();
      }
      m_renderCallback(m_driverNode);
   }
   else if (!rendering && m_comp != 0)
   {
      m_comp->endComputation();
      delete m_comp;
      m_comp = 0;
   }
}

/// Replace the idle event installed by AddIdleRenderViewCallback with one that
/// actually does the interrupt checking.
void CArnoldRenderSession::DoAddIdleRenderViewCallback(void* data)
{
   CArnoldRenderSession *session = (CArnoldRenderSession*)data;
   MMessage::removeCallback(session->m_idle_cb);
   session->m_idle_cb = 0;
   MStatus status;

   session->m_idle_cb = MTimerMessage::addTimerCallback(0.01f,
                                               DoInteractiveRenderCallback,
                                               data,
                                               &status);
}

// there is a very strange bug with Maya where MComputation will lock up the GUI if
// it is created somewhere below a MEL script that returns a value.  We sidestep this problem
// by using this idle event callback to setup the MComputation. The callback runs only once.
void CArnoldRenderSession::AddIdleRenderViewCallback(const MString& postRenderMel)
{
   MStatus status;
   m_postRenderMel = postRenderMel;
   if(m_idle_cb == 0)
   {
      m_idle_cb = MEventMessage::addEventCallback("idle",
                                                  CArnoldRenderSession::DoAddIdleRenderViewCallback,
                                                  this,
                                                  &status);
   }
}


void CArnoldRenderSession::UnPauseIPR()
{
   m_pausedIPR = false;
   RequestUpdate();
}
void CArnoldRenderSession::UpdateFullScene()
{
      // We save and restore the res instead of using the translated one because
      // the translated value is from the render globals. We may have been
      // passed in a different value to start with.
   MDagPath camera = m_sessionOptions.GetCamera();
 /*     int width = -1;
      int height = -1;
      sessionOptions.GetResolution(width, height);
      
      // Same deal for the camera.
   
      MString panel = sessionOptions.GetRenderViewPanelName();

      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] IPR refresh using last rendered camera '"+camera.partialPathName()+"' in panel '"+panel+"'");
*/    
   MSelectionList selected;
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   Clear();
   // End and restart a new render session re-using saved resolution and camera
   SetExportCamera(camera, false);
   if (!renderGlobals.renderAll)
   {
      MGlobal::getActiveSelectionList(selected);
      Export(&selected);
   }
   else
   {
      Export();
   }
/*
   // Set resolution and camera as passed in.
   SetResolution(width, height);
   renderSession->SetCamera(camera);
   renderSession->SetRenderViewPanelName(panel);
*/
   // Start off the render.
}