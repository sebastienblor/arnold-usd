#ifndef RENDER_INSTANCE_H
#define RENDER_INSTANCE_H

#include <ai_nodes.h>
#include <ai_universe.h>

class CRenderOptions;

class CRenderInstance
{

public:

   static CRenderInstance* GetInstance();

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

   CRenderInstance()
   {
   }

private:

   CRenderOptions* m_renderOptions;

}; // class CRenderInstance

#endif // RENDER_INSTANCE_H
