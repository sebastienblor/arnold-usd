#ifndef RENDER_SESSION_H
#define RENDER_SESSION_H

#include <ai_nodes.h>
#include <ai_universe.h>

class CRenderOptions;

class CRenderSession
{

public:

   static CRenderSession* GetInstance();

   void Init();
   void End();

   bool IsActive() const
   {
      return AiUniverseIsActive();
   }

   void SetRenderOptions(CRenderOptions* renderOptions)
   {
      m_renderOptions = renderOptions;
   }

   void DoRender();

private:

   CRenderSession()
   {
   }

private:

   CRenderOptions* m_renderOptions;

}; // class CRenderSession

#endif // RENDER_SESSION_H
