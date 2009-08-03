#ifndef RENDER_INSTANCE_H
#define RENDER_INSTANCE_H

#include <ai_nodes.h>

class CRenderInstance
{

public:

   CRenderInstance()
      :  m_driver(NULL)
   {
   }

   void Init();
   void End();

   void SetGamma(float gamma);

   void DoRender();
   void WaitForRender();

private:

   AtNode* m_driver;

}; // class CRenderInstance

#endif // RENDER_INSTANCE_H
