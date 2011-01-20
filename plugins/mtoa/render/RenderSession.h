#ifndef RENDER_SESSION_H
#define RENDER_SESSION_H

#include "render/RenderOptions.h"
#include "maya_scene/MayaScene.h"

#include <maya/MGlobal.h>

#include <ai_nodes.h>
#include <ai_universe.h>

#include <maya/MMessage.h> // for MCallbackId
#include <maya/MComputation.h>

class MImage;
class CMayaScene;

class CRenderSession
{

public:

   static CRenderSession* GetInstance();
   CMayaScene* GetMayaScene();

   // Get the render view ready.
   // Optionally installs a callback for IPR.
   MStatus PrepareRenderView(bool addIdleRenderViewUpdate=false);

   void Init();
   void LoadPlugins();

   // Translate the Maya scene to Arnold.
   void Translate(ExportMode exportMode=MTOA_EXPORT_ALL);

   // Render Methods.
   void DoInteractiveRender();
   void DoBatchRender();
   void DoExport(MString customFileName);

   // Stop a render, leaving the data in Arnold.
   void InterruptRender();

   // Finish/abort a render. This will shutdown Arnold.
   void Finish();

   // IPR Methods, most have obvious tasks.
   void DoIPRRender();
   void PauseIPR();
   void FinishedIPRTuning();
   void UnPauseIPR();
   AtUInt64 GetUsedMemory();

   // Swatch Rendering methods
   void DoSwatchRender(const AtInt resolution);
   bool GetSwatchImage(MImage & image);

   AtInt LoadAss(const MString filename);

   void SetBatch(bool batch);
   void SetResolution(const int width, const int height);
   void SetCamera(MString cameraNode);
   void SetMultiCameraRender(bool multi);
   void SetProgressive( bool is_progressive );
   void SetRegion(const AtUInt left,const AtUInt right,
                  const AtUInt bottom, const AtUInt top);

   const CRenderOptions* RenderOptions() const
   {
      return &m_renderOptions;
   }

   bool IsActive() const
   {
      return m_is_active;
   }

   void ExecuteScript(const MString &str, bool echo=false)
   {
      if (str.length() > 0)
      {
         MGlobal::executeCommand(str, echo);
      }
   }


private:

   CRenderSession()
   : m_scene(0x0)
   , m_paused_ipr(false)
   , m_is_active(false)
   , m_idle_cb(0)
   , m_render_thread(0x0)
   {
   }   

   static unsigned int RenderThread(AtVoid* data);
   
   // The idle callback is used to update the
   // render view when rendering IPR.
   void AddIdleRenderViewCallback();
   static void updateRenderViewCallback(void *);
   void ClearIdleRenderViewCallback();

   // These functions setup the file output
   // and Render View output driver.
   void SetupRenderOutput();
   AtNode * CreateFileOutput();
   AtNode * CreateRenderViewOutput();
   AtNode * CreateOutputFilter();

private:

   CRenderOptions m_renderOptions;
   CMayaScene*    m_scene;
   bool           m_paused_ipr;
   bool           m_is_active;

   // This is a special callback installed to update the render
   // view while Arnold is rendering in IPR.
   MCallbackId m_idle_cb;

   // This is the render thread that Arnolds AiRender() is
   // called from.
   AtVoid* m_render_thread;


}; // class CRenderSession

#endif // RENDER_SESSION_H
