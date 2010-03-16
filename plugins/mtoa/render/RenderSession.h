#ifndef RENDER_SESSION_H
#define RENDER_SESSION_H

#include "render/RenderOptions.h"

#include <ai_nodes.h>
#include <ai_universe.h>

class CMayaScene;

class CRenderSession
{

public:

   static CRenderSession* GetInstance();

   void Init();
   void End();

   void Reset();

   void SetWidth(int width);
   void SetHeight(int height);
   void SetCamera(MString cameraNode);

   const CRenderOptions* RenderOptions() const
   {
      return &m_renderOptions;
   }

   bool IsActive() const
   {
      return AiUniverseIsActive();
   }

   void DoRender();
   void DoExport();

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
