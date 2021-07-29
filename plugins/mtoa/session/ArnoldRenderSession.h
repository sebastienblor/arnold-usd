#pragma once

#include "ArnoldSession.h"
#include "../render/display/renderview_mtoa.h"

class DLLEXPORT CArnoldRenderSession : public CArnoldSession
{

public:

   CArnoldRenderSession();
   virtual ~CArnoldRenderSession();

   
   bool BatchRender();
   bool Render();      

   void Clear() override;

   typedef void (*RenderCallbackType) (AtNode *);   
   void SetRenderCallback(RenderCallbackType callback, AtNode *driverNode);
   RenderCallbackType GetRenderCallback();
   void InvokeRenderCallback()
   {
      if (m_renderCallback)
         m_renderCallback(m_driverNode);
   }
   void DisplayProgress(bool b) {m_displayProgress = b;}

   MStringArray GetActiveImageFilenames() const;
   MString GetLastRenderViewCaptionCommand() const
   {
      return m_renderViewCaptionCmd;
   }

   void SetRenderViewCaptionCmd(const MString &cmd)
   {
      m_renderViewCaptionCmd = cmd;
   }
   void IPR();
   void PauseIPR();
   void UnPauseIPR();
   void RequestUpdate() override;   
   void UpdateIPRCallback();
   void UpdateFullScene();
   //unsigned int IPRThreadLoop();
   void InteractiveRenderCallback(float elapsedTime, float lastTime);
   static void DoAddIdleRenderViewCallback(void* data);
   void AddIdleRenderViewCallback(const MString& postRenderMel);
   void ClearIdleRenderViewCallback();
protected:
   

   RenderCallbackType m_renderCallback;
   AtNode *m_driverNode;
   AtMutex m_renderLock;
   bool m_displayProgress;
   MString m_renderViewCaptionCmd;
   bool m_pausedIPR;
   MCallbackId m_IPRIdleCallbackId;
   void *m_renderThread;
   MComputation *m_comp;
   MCallbackId m_idle_cb;
   MString m_postRenderMel;

}; 
