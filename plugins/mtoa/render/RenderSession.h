#ifndef RENDERSESSION_H
#define RENDERSESSION_H

#include "render/RenderOptions.h"

#include <maya/MGlobal.h>

#include <ai_nodes.h>
#include <ai_universe.h>
#include <ai_bbox.h>

#include <maya/MMessage.h> // for MCallbackId
#include <maya/MComputation.h>

class MImage;

/** CRenderSession handles the management of Arnold and rendering.
 * 
 * This is an important class. It handles rendering as well
 * loading plugins to Arnold, exporting ASS files and swatch
 * rendering.
 */

class DLLEXPORT CRenderSession
{
   friend class CMayaScene;

public:

   /// Initialize the Arnold universe, it will be ready for translation and render
   MStatus Begin(CRenderOptions* options);
   /// Terminate a render. This will shutdown the Arnold universe.
   MStatus End();

   /// Get the render view ready.
   /// \param addIdleRenderViewUpdate Optionally install a callback for IPR.
   MStatus PrepareRenderView(bool addIdleRenderViewUpdate=false);

   // Render Methods.
   /// Render into the Render View, not IPR.
   void DoInteractiveRender();
   /// Render in the background of Maya.
   int DoBatchRender();
   /// Get a valid ass name
   MString GetAssName(const MString& customName,
                      const MCommonRenderSettingsData& renderGlobals,
                      double frameNumber,
                      const MString &sceneName,
                      const MString &cameraName,
                      const MString &fileFormat,
                      const MObject layer,
                      const bool createDirectory=true,
                      const bool isSequence=false,
                      const bool subFrames=false,
                      const bool isBatch=false,
                      MStatus *ReturnStatus=NULL) const;
   /// Export and ass file.
   /// \param customFileName file to export too.
   void DoAssWrite(MString customFileName, const bool compressed=false);
   /// Get the translated scene bounding box.
   AtBBox GetBoundingBox();
   MStatus WriteAsstoc(const MString& filename, const AtBBox& bBox);

   /// Stop a render, leaving Arnold univierse active.
   void InterruptRender();

   /// Start and IPR render.
   void DoIPRRender();
   /// Pause IPR, callbacks will still fire and Arnold will get the changes.
   void PauseIPR();
   /// Start off rendering again.
   void UnPauseIPR();
   /// When the progressive renders are all done this is called to update
   /// the render view with a render time and memory usage data.
   void FinishedIPRTuning();
   /// Get memory usage from Arnold.
   /// \return memory used in MB.
   AtUInt64 GetUsedMemory();

   // Swatch Rendering methods
   /// Start a swatch render.
   /// \param resolution the resolution of the swatch, it must be square.
   void DoSwatchRender(const int resolution);
   /// Return the rendered swatch.
   /// \param image storage to place the rendered image into.
   /// \return returns false if there was no complete image.
   bool GetSwatchImage(MImage & image);

   /// Set the ass output mask
   inline void SetOutputAssMask(unsigned int mask) { m_renderOptions.SetOutputAssMask(mask); }

   /// Set the resolution of the render.
   /// \param width width in pixels.
   /// \param height height in pixels.
   void SetResolution(const int width, const int height);
   /// Get the camera used to render
   MDagPath GetCamera() const;
   /// Set the camera to use for render.
   void SetCamera(MDagPath cameraNode);
   void SetProgressive(bool is_progressive);
   void SetRegion(const unsigned int left,const unsigned int right,
                  const unsigned int bottom, const unsigned int top);

   /// Return a pointer to the render options.
   /// \see CRenderOptions
   inline CRenderOptions* RenderOptions() { return &m_renderOptions; }

   /// Returns the state of rendering.
   /// This is different from if Arnold is active. After tuning in IPR
   /// this will return true, because the RenderSession is busy. Even
   /// though Arnold is not currently rendering.
   /// \return if we're active.
   bool IsActive() const
   {
      return m_is_active;
   }


private:

   CRenderSession()
      : m_paused_ipr(false)
      , m_is_active(false)
      , m_idle_cb(0)
      , m_timer_cb(0)
      , m_render_thread(NULL)
   {
   }

   ~CRenderSession() { End(); };

   /// This is the static method which is the thread that calls AiRender().
   static unsigned int RenderThread(void* data);
   
   /// The idle callback is used to update the
   /// render view when rendering IPR.
   void AddIdleRenderViewCallback();
   void ClearIdleRenderViewCallback();
   /// Tells the render view to update the image, it's called during idle time.
   static void RefreshRenderView(float, float, void *);
   /// Passes the pending tiles from the output driver to the render view, called by a
   /// timer so we don't overload the render view (it's very slow).
   static void TransferTilesToRenderView(void*);

private:

   CRenderOptions m_renderOptions;
   bool           m_paused_ipr;  ///< True when IPR is paused.
   bool           m_is_active;   ///< True when after a Init() and before a Finish().

   /// This is a special callback installed to update the render view while Arnold is rendering in IPR.
   /// \see AddIdleRenderViewCallback
   /// \see ClearIdleRenderViewCallback
   MCallbackId    m_idle_cb;
   MCallbackId    m_timer_cb;

   /// This is a pointer to the thread which is running RenderThread.
   void*        m_render_thread;


}; // class CRenderSession

#endif // RENDERSESSION_H
