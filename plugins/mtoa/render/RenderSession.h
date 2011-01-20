#ifndef RENDER_SESSION_H
#define RENDER_SESSION_H

#include "render/RenderOptions.h"
#include "maya_scene/MayaScene.h"

#include <maya/MGlobal.h>

#include <ai_nodes.h>
#include <ai_universe.h>

#include <maya/MMessage.h> // for MCallbackId

class CMayaScene;

class CRenderSession
{

public:

   static CRenderSession* GetInstance();
   CMayaScene* GetMayaScene();

   void Init(ExportMode exportMode=MTOA_EXPORT_ALL, bool preMel=false, bool preLayerMel=false, bool preFrameMel=false);
   void End(bool postMel=false, bool postLayerMel=false, bool postFrameMel=false);
   void Interrupt();

   void Reset(bool postMel=false, bool postLayerMel=false, bool postFrameMel=false, bool preMel=false, bool preLayerMel=false, bool preFrameMel=false);

   void SetBatch(bool batch);
   void SetWidth(int width);
   void SetHeight(int height);
   void SetCamera(MString cameraNode);
   void SetMultiCameraRender(bool multi);
   void SetRegion( const AtUInt left, const AtUInt right, const AtUInt bottom, const AtUInt top );
   void SetProgressive( bool is_progressive );

   const CRenderOptions* RenderOptions() const
   {
      return &m_renderOptions;
   }

   bool IsActive() const
   {
      return (AiUniverseIsActive() == TRUE);
   }

   void ExecuteScript(const MString &str, bool echo=false)
   {
      if (str.length() > 0)
      {
         MGlobal::executeCommand(str, echo);
      }
   }

   void DoRender();
   void DoBatchRender();
   void DoExport(MString customFileName = "", ExportMode exportMode=MTOA_EXPORT_ALL);

   // IPR Methods.
   MStatus PrepareIPR();
   MStatus PrepareRenderView();
   void DoIPRRender();
   void StopIPR();
   void PauseIPR();
   void FinishedIPRTuning();
   void UnPauseIPR();
   AtUInt64 GetUsedMemory();
   // The idle callback is used to update the
   // render view when rendering IPR.
   void AddIdleRenderViewCallback();
   void ClearIdleRenderViewCallback();


private:

   CRenderSession()
   :m_scene(0x0), m_paused_ipr( false ), m_idle_cb(0), m_render_thread(0x0)
   {
   }

   static void updateRenderViewCallback(void *);

   void SetupRenderOutput();
   AtNode * CreateFileOutput();
   AtNode * CreateRenderViewOutput();
   AtNode * CreateOutputFilter();

private:

   CRenderOptions m_renderOptions;
   CMayaScene*    m_scene;
   bool           m_paused_ipr;

   // This is a special callback installed to update the render
   // view while Arnold is rendering.
   MCallbackId m_idle_cb;

   // This is the render thread that Arnolds AiRender() is
   // called from.
   AtVoid* m_render_thread;


}; // class CRenderSession

#endif // RENDER_SESSION_H
