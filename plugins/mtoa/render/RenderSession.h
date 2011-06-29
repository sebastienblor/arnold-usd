#ifndef RENDERSESSION_H
#define RENDERSESSION_H

#include "render/RenderOptions.h"
#include "scene/MayaScene.h"

#include <maya/MGlobal.h>

#include <ai_nodes.h>
#include <ai_universe.h>
#include <ai_bbox.h>

#include <maya/MMessage.h> // for MCallbackId
#include <maya/MComputation.h>

class MImage;
class CMayaScene;

/** CRenderSession handles the management of Arnold and rendering.
 * 
 * This is an important class. It handles rendering as well
 * loading plugins to Arnold, exporting ASS files and swatch
 * rendering.
 */

class DLLEXPORT CRenderSession
{

public:

   /// Return the instance of the render session.
   static CRenderSession* GetInstance();
   /// Return the Maya Scene class, this is not a Maya API.
   /// \see CMayaScene
   CMayaScene* GetMayaScene();

   /// Get the render view ready.
   /// \param addIdleRenderViewUpdate Optionally install a callback for IPR.
   MStatus PrepareRenderView(bool addIdleRenderViewUpdate=false);

   /// Initialize translation of Maya scene to Arnold
   void Init();
   void Init(CExportOptions& options);

   /// Load the external shaders/procedrals into Arnold.
   void LoadPlugins();

   /// Translate the Maya scene to Arnold.
   void Translate();
   void Translate(CExportOptions& options);

   /// Get the translated scene bounding box.
   AtBBox GetBoundingBox();
   MStatus WriteAsstoc(const MString& filename, const AtBBox& bBox);

   // Render Methods.
   /// Render into the Render View, not IPR.
   void DoInteractiveRender();
   /// Render in the background of Maya.
   void DoBatchRender();
   /// Export and ass file.
   /// \param customFileName file to export too.
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
   void DoExport(MString customFileName);

   /// Stop a render, leaving Arnold univierse active.
   void InterruptRender();

   /// Finish/abort a render. This will shutdown the Arnold universe.
   void Finish();

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
   void DoSwatchRender(const AtInt resolution);
   /// Return the rendered swatch.
   /// \param image storage to place the rendered image into.
   /// \return returns false if there was no complete image.
   bool GetSwatchImage(MImage & image);

   /// Load an Ass file into the Universe.
   /// \param filename file to load.
   /// \return returns 0 on success.
   AtInt LoadAss(const MString filename);

   /// Set whether we're in batch mode.
   void SetBatch(bool batch);
   /*
   /// Set the options to use when exporting/translating the CMayaScene
   void SetSceneExportOptions(const ExportOptions);
   */
   /// Set the resolution of the render.
   /// \param width width in pixels.
   /// \param height height in pixels.
   void SetResolution(const int width, const int height);
   /// Set the the camera to render.
   void SetCamera(MString cameraNode);
   void SetMultiCameraRender(bool multi);
   void SetProgressive(bool is_progressive);
   void SetRegion(const AtUInt left,const AtUInt right,
                  const AtUInt bottom, const AtUInt top);

   /// Return the render options.
   /// \see CRenderOptions
   const CRenderOptions* RenderOptions() const
   {
      return &m_renderOptions;
   }

   /// Returns the state of rendering.
   /// This is different from if Arnold is active. After tuning in IPR
   /// this will return true, because the RenderSession is busy. Even
   /// though Arnold is not currently rendering.
   /// \return if we're active.
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
   : m_scene(NULL)
   , m_paused_ipr(false)
   , m_is_active(false)
   , m_idle_cb(0)
   , m_render_thread(NULL)
   {
   }   

   /// This is the static method which is the thread that calls AiRender().
   static unsigned int RenderThread(AtVoid* data);
   
   /// The idle callback is used to update the
   /// render view when rendering IPR.
   void AddIdleRenderViewCallback();
   static void updateRenderViewCallback(void *);
   void ClearIdleRenderViewCallback();

   // These functions setup the file output
   // and Render View output driver.
   void SetupRenderOutput();
   AtNode * CreateFileOutput();
   AtNode * CreateRenderViewOutput();
   AtNode * CreateOutputFilter();
   AtNode * CreateAovOutputFilter();

private:

   CRenderOptions m_renderOptions;
   CMayaScene*    m_scene;
   bool           m_paused_ipr;  ///< True when IPR is paused.
   bool           m_is_active;   ///< True when after a Init() and before a Finish().

   /// This is a special callback installed to update the render view while Arnold is rendering in IPR.
   /// \see AddIdleRenderViewCallback
   /// \see updateRenderViewCallback
   /// \see ClearIdleRenderViewCallback
   MCallbackId m_idle_cb;

   /// This is a pointer to the thread which is running RenderThread.
   AtVoid* m_render_thread;


}; // class CRenderSession

#endif // RENDERSESSION_H
