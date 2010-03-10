
#include "RenderInstance.h"
#include "RenderOptions.h"
#include "OutputDriver.h"

#include <ai_msg.h>
#include <ai_plugins.h>
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

   m_renderOptions->SetupLog();

   MString resolvedPathList = m_renderOptions->pluginsPath().expandEnvironmentVariablesAndTilde();

   AiLoadPlugins(resolvedPathList.asChar());

   m_renderOptions->SetupRender();

   AiNodeInstall(AI_NODE_DRIVER, AI_TYPE_NONE, "renderview_display",  NULL, (AtNodeMethods*) mtoa_driver_mtd, AI_VERSION);

   AtNode* filter = AiNode(m_renderOptions->filterType().asChar());
   AtNode* driver = AiNode("renderview_display");

   AiNodeSetStr(driver, "name", "renderview_display");
   AiNodeSetFlt(driver, "gamma", m_renderOptions->outputGamma());

   if ( AiNodeEntryLookUpParameter(filter->base_node,"width") )
   {
      AiNodeSetFlt(filter, "width", m_renderOptions->filterWidth());
   }
   if ( AiNodeEntryLookUpParameter(filter->base_node,"domain") )
   {
      AiNodeSetStr(filter, "domain", m_renderOptions->filterDomain().asChar());
   }
   if ( AiNodeEntryLookUpParameter(filter->base_node,"scalar_mode") )
   {
      AiNodeSetBool(filter, "scalar_mode", m_renderOptions->filterScalarMode());
   }
   if ( AiNodeEntryLookUpParameter(filter->base_node,"maximum") )
   {
      AiNodeSetFlt(filter, "maximum", m_renderOptions->filterMaximum());
   }
   if ( AiNodeEntryLookUpParameter(filter->base_node,",minimum") )
   {
      AiNodeSetFlt(filter, "minimum", m_renderOptions->filterMinimum());
   }

   AtChar   str[1024];
   AtArray* outputs;

   sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(driver));
   outputs = AiArray(1, 1, AI_TYPE_STRING, str);
   AiNodeSetArray(AiUniverseGetOptions(), "outputs", outputs);
}

void CRenderInstance::End()
{
   AiEnd();
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
