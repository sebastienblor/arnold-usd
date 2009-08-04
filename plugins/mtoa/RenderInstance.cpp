
#include "RenderInstance.h"
#include "OutputDriver.h"

#include <ai_msg.h>
#include <ai_render.h>
#include <ai_threads.h>
#include <ai_universe.h>

#include <cstdio>

extern AtNodeMethods* mtoa_driver_mtd;

static CRenderInstance* s_renderInstance = NULL;

// This is the code for the render thread. This thread is used only to run the AiRender() process outside of the main thread.
static unsigned int RenderThread(AtVoid* data)
{
   AiRender( AI_RENDER_MODE_CAMERA );

   return 0;
}

// Cheap singleton
CRenderInstance* CRenderInstance::GetInstance()
{
   if (!s_renderInstance)
      s_renderInstance = new CRenderInstance();

   return s_renderInstance;
}

void CRenderInstance::Init()
{
   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] ERROR: AiBegin() called twice without AiEnd().");
      return;
   }

   AiBegin();

   // TODO: For now, we will use stdout (in Maya, it will go to the output window)
   AiSetLogOptions(NULL, AI_LOG_ALL, 1000, 4);

   AiNodeInstall(AI_NODE_DRIVER, AI_TYPE_NONE, "renderview_display",  NULL, (AtNodeMethods*) mtoa_driver_mtd, AI_VERSION);

   AtNode* filter = AiNode("box_filter");
   m_driver = AiNode("renderview_display");

   AiNodeSetStr(m_driver, "name", "renderview_display");

   AtChar   str[1024];
   AtArray* outputs;

   sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(m_driver));
   outputs = AiArray(1, 1, AI_TYPE_STRING, str);
   AiNodeSetArray(AiUniverseGetOptions(), "outputs", outputs);
}

void CRenderInstance::End()
{
   AiEnd();
}

void CRenderInstance::SetGamma(float gamma)
{
   if (m_driver != NULL)
      AiNodeSetFlt(m_driver, "gamma", gamma);
}

void CRenderInstance::DoRender()
{
   InitializeDisplayUpdateQueue();

   AtVoid* handler = AiThreadCreate(RenderThread, NULL, AI_PRIORITY_LOW);

   // Process messages sent by the render thread, and exit when rendering is finished or aborted
   ProcessDisplayUpdateQueue();

   // Wait for the render thread to release everything and close it
   AiThreadWait(handler);
   AiThreadClose(handler);
}
