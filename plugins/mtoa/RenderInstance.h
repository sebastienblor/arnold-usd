#ifndef RENDER_INSTANCE_H
#define RENDER_INSTANCE_H

#include <ai_nodes.h>
#include <ai_universe.h>

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

   void SetGamma(float gamma);

   void DoRender();

private:

   CRenderInstance()
      :  m_driver(NULL)
   {
   }

private:

   AtNode* m_driver;

}; // class CRenderInstance

#endif // RENDER_INSTANCE_H
