#ifndef RENDER_SESSION_H
#define RENDER_SESSION_H

#include "render/RenderOptions.h"
#include "maya_scene/MayaScene.h"

#include <ai_nodes.h>
#include <ai_universe.h>


class CMayaScene;

class CRenderSession
{

public:

   static CRenderSession* GetInstance();

   void Init(ExportMode exportMode=MTOA_EXPORT_ALL);
   void End();

   void Reset();

   void SetBatch(bool batch);
   void SetWidth(int width);
   void SetHeight(int height);
   void SetCamera(MString cameraNode);
   void SetMultiCameraRender(bool multi);

   const CRenderOptions* RenderOptions() const
   {
      return &m_renderOptions;
   }

   bool IsActive() const
   {
      return AiUniverseIsActive();
   }

   void DoRender();
   void DoBatchRender();
   void DoExport(MString customFileName = "");

private:

   CRenderSession()
   {
   }

   void SetupRenderOutput();

private:

   CRenderOptions m_renderOptions;
   CMayaScene*    m_scene;

}; // class CRenderSession

#endif // RENDER_SESSION_H
