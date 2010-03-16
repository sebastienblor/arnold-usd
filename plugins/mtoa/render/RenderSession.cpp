
#include "RenderSession.h"
#include "RenderOptions.h"
#include "OutputDriver.h"
#include "maya_scene/MayaScene.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_plugins.h>
#include <ai_render.h>
#include <ai_threads.h>
#include <ai_universe.h>

#include <cstdio>

extern AtNodeMethods* mtoa_driver_mtd;

static CRenderSession* s_renderSession = NULL;

// This is the code for the render thread. This thread is used only to run the AiRender() process outside of the main thread.
static unsigned int RenderThread(AtVoid* data)
{
   AiRender( AI_RENDER_MODE_CAMERA );

   return 0;
}

// Cheap singleton
CRenderSession* CRenderSession::GetInstance()
{
   if (!s_renderSession)
      s_renderSession = new CRenderSession();

   return s_renderSession;
}

void CRenderSession::Init()
{
   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] ERROR: There can only be one RenderSession active.");
      return;
   }

   m_scene = new CMayaScene;
   
   m_renderOptions.GetFromMaya(m_scene);

   AiBegin();

   m_renderOptions.SetupLog();

   MString resolvedPathList = m_renderOptions.pluginsPath().expandEnvironmentVariablesAndTilde();

   AiLoadPlugins(resolvedPathList.asChar());

   m_scene->ExportToArnold();

   SetupRenderOutput();
}

void CRenderSession::End()
{
   AiEnd();

   delete m_scene;
}

void CRenderSession::Reset()
{
   if (IsActive())
   {
      End();
   }

   Init();
}

void CRenderSession::SetWidth(int width)
{
   if (width != -1)
      m_renderOptions.SetWidth(width);
}

void CRenderSession::SetHeight(int height)
{
   if (height != -1)
      m_renderOptions.SetHeight(height);
}

void CRenderSession::SetCamera(MString cameraNode)
{
   if (cameraNode != "")
   {
      AtNode* camera = AiNodeLookUpByName(cameraNode.asChar());

      if (!camera)
      {
         cameraNode += "Shape";
         camera = AiNodeLookUpByName(cameraNode.asChar());
      }

      if (!camera)
      {
         AiMsgError("Cannot find camera node %s.", cameraNode.asChar());
         return;
      }

      AiNodeSetPtr(AiUniverseGetOptions(), "camera", camera);
   }
}

void CRenderSession::DoRender()
{
   m_renderOptions.SetupRenderOptions();

   InitializeDisplayUpdateQueue();

   AtVoid* handler = AiThreadCreate(RenderThread, NULL, AI_PRIORITY_LOW);

   // Process messages sent by the render thread, and exit when rendering is finished or aborted
   ProcessDisplayUpdateQueue();

   // Wait for the render thread to release everything and close it
   AiThreadWait(handler);
   AiThreadClose(handler);
}

void CRenderSession::DoExport()
{
   MString fileName = m_renderOptions.VerifyFileName(m_renderOptions.outputAssFile().expandEnvironmentVariablesAndTilde(), m_renderOptions.outputAssCompressed());

   if (fileName == "")
   {
      AiMsgError("[mtoa] File name must be set before exporting .ass file");
   }
   else
   {
      AiMsgInfo("[mtoa] Exporting Maya scene to file '%s'", fileName.asChar());

      m_renderOptions.SetupRenderOptions();

      AiASSWrite(fileName.asChar(), m_renderOptions.outputAssMask(), false);
   }
}

void CRenderSession::SetupRenderOutput()
{
   // DISPLAY DRIVER (for interactive rendering)
   //
   AiNodeInstall(AI_NODE_DRIVER, AI_TYPE_NONE, "renderview_display",  NULL, (AtNodeMethods*) mtoa_driver_mtd, AI_VERSION);

   AtNode* driver = AiNode("renderview_display");

   AiNodeSetStr(driver, "name", "renderview_display");
   AiNodeSetFlt(driver, "gamma", m_renderOptions.outputGamma());

   // OUTPUT FILTER (use for all image outputs)
   //
   AtNode* filter = AiNode(m_renderOptions.filterType().asChar());

   // Only set filter parameters if they exist within that specific node
   if (AiNodeEntryLookUpParameter(filter->base_node, "width"))
   {
      AiNodeSetFlt(filter, "width", m_renderOptions.filterWidth());
   }
   if (AiNodeEntryLookUpParameter(filter->base_node, "domain"))
   {
      AiNodeSetStr(filter, "domain", m_renderOptions.filterDomain().asChar());
   }
   if (AiNodeEntryLookUpParameter(filter->base_node, "scalar_mode"))
   {
      AiNodeSetBool(filter, "scalar_mode", m_renderOptions.filterScalarMode());
   }
   if (AiNodeEntryLookUpParameter(filter->base_node, "maximum"))
   {
      AiNodeSetFlt(filter, "maximum", m_renderOptions.filterMaximum());
   }
   if (AiNodeEntryLookUpParameter(filter->base_node, "minimum"))
   {
      AiNodeSetFlt(filter, "minimum", m_renderOptions.filterMinimum());
   }

   // OUTPUT STRINGS
   //
   AtChar   str[1024];
   AtArray* outputs;

   sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(driver));
   outputs = AiArray(1, 1, AI_TYPE_STRING, str);
   AiNodeSetArray(AiUniverseGetOptions(), "outputs", outputs);
}
