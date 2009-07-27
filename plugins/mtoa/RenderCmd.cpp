
#include "RenderCmd.h"
#include "OutputDriver.h"
#include "maya_scene/MayaScene.h"

#include <ai_api.h>
#include <ai_constants.h>
#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_threads.h>
#include <ai_universe.h>
#include <ai_version.h>

#include <maya/M3dView.h>
#include <maya/MComputation.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MRenderView.h>
#include <maya/MSelectionList.h>

extern AtNodeMethods* mtoa_driver_mtd;

// This is the code for the render thread. This thread is used only to run the AiRender() process outside of the main thread.
unsigned int RenderThread(AtVoid* data)
{
   AiRender( AI_RENDER_MODE_CAMERA );

   return 0;
}

CRenderCmd::CRenderCmd()
:  m_minx(0), m_miny(0), m_maxx(0), m_maxy(0)
,  m_width(0), m_height(0)
,  m_pixelAspectRatio(1.0f)
,  m_useRenderRegion(false)
,  m_clearBeforeRender(false)
,  m_gamma(1.0f)
{
}

MStatus CRenderCmd::doIt(const MArgList& argList)
{
   MStatus status;

   AiBegin();

   // TODO: For now, we will use stdout (in Maya, it will go to the output window)
   AiSetLogOptions(NULL, AI_LOG_ALL, 1000, 4);

   ProcessCommonRenderOptions();
   ProcessArnoldRenderOptions();

   CMayaScene scene;

   status = scene.ExportToArnold();

   if (MRenderView::doesRenderEditorExist())
   {
      MDagPath cameraPath;

      M3dView::active3dView().getCamera(cameraPath);

      MRenderView::setCurrentCamera(cameraPath);
   }

   MComputation comp;

   comp.beginComputation();

   if (MRenderView::doesRenderEditorExist())
   {
      InitOutputDriver();

      // Exports .ass file for debugging purposes.
      //AiMsgDebug( "Exporting Maya scene for debug.\n" );
      //AiASSWrite( "c:/Maya2Arnold.ass", AI_NODE_ALL, false );

      if (m_useRenderRegion)
         status = m_useRenderRegion ? MRenderView::startRegionRender(m_width, m_height, m_minx, m_maxx, m_miny, m_maxy, !m_clearBeforeRender, true)
                                    : MRenderView::startRender(m_width, m_height, !m_clearBeforeRender, true);

      if ( status == MS::kSuccess)
      {
         Render();

         MRenderView::endRender();
      }
   }
   else
   {
      Render();
   }

   comp.endComputation();

   AiEnd();

   return status;
}  // doIt()

void CRenderCmd::InitOutputDriver()
{
   AiNodeInstall(AI_NODE_DRIVER, AI_TYPE_NONE, "renderview_display",  NULL, (AtNodeMethods*) mtoa_driver_mtd, AI_VERSION);

   AtNode* filter = AiNode("box_filter");
   AtNode* driver = AiNode("renderview_display");

   AiNodeSetStr(driver, "name", "renderview_display");
   AiNodeSetFlt(driver, "gamma", m_gamma);

   AtChar   str[1024];
   AtArray* outputs;

   sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(driver));
   outputs = AiArray(1, 1, AI_TYPE_STRING, str);
   AiNodeSetArray(AiUniverseGetOptions(), "outputs", outputs);

}  // InitOutputDriver()

void CRenderCmd::Render()
{
   InitializeDisplayUpdateQueue();

   AtVoid* handler = AiThreadCreate(RenderThread, NULL, AI_PRIORITY_LOW);

   // Process messages sent by the render thread, and exit when rendering is finished or aborted
   ProcessDisplayUpdateQueue();

   // Wait for the render thread to release everything and close it
   AiThreadWait(handler);
   AiThreadClose(handler);
}  // Render()

void CRenderCmd::ProcessCommonRenderOptions()
{
   MStatus        status;
   MSelectionList list;
   MObject        node;

   list.add("defaultRenderGlobals");

   if (list.length() > 0)
   {
      list.getDependNode(0, node);

      MFnDependencyNode fnRenderGlobals(node);

      m_useRenderRegion = fnRenderGlobals.findPlug("useRenderRegion").asBool();

      if (m_useRenderRegion)
      {
         m_minx = fnRenderGlobals.findPlug("left").asInt();
         m_miny = fnRenderGlobals.findPlug("bot").asInt();
         m_maxx = fnRenderGlobals.findPlug("rght").asInt();
         m_maxy = fnRenderGlobals.findPlug("top").asInt();
      }

      MPlugArray connectedPlugs;
      MPlug      resPlug = fnRenderGlobals.findPlug("resolution");

      resPlug.connectedTo(connectedPlugs,
         true,  // asDestination
         false, // asSource
         &status);

      // Must be length 1 or we would have fan-in
      if (status && (connectedPlugs.length() == 1))
      {
         MObject resNode = connectedPlugs[0].node(&status);

         if (status)
         {
            MFnDependencyNode fnRes(resNode);

            m_width  = fnRes.findPlug("width").asShort();
            m_height = fnRes.findPlug("height").asShort();
            m_pixelAspectRatio = ((float)m_height / m_width) * fnRes.findPlug("deviceAspectRatio").asFloat();
   
            if (m_useRenderRegion)
            {
               AiNodeSetInt(AiUniverseGetOptions(), "region_min_x", m_minx);
               AiNodeSetInt(AiUniverseGetOptions(), "region_min_y", m_height - m_maxy - 1);
               AiNodeSetInt(AiUniverseGetOptions(), "region_max_x", m_maxx);
               AiNodeSetInt(AiUniverseGetOptions(), "region_max_y", m_height - m_miny - 1);
            }

            AiNodeSetInt(AiUniverseGetOptions(), "xres", m_width);
            AiNodeSetInt(AiUniverseGetOptions(), "yres", m_height);
            AiNodeSetFlt(AiUniverseGetOptions(), "aspect_ratio", m_pixelAspectRatio);
         }
      }
   }
}

void CRenderCmd::ProcessArnoldRenderOptions()
{
   MSelectionList list;
   MObject        node;

   list.add("defaultArnoldRenderOptions");

   if (list.length() > 0)
   {
      list.getDependNode(0, node);

      MFnDependencyNode fnArnoldRenderOptions(node);

      AiNodeSetInt(AiUniverseGetOptions(), "threads", fnArnoldRenderOptions.findPlug("threads_autodetect").asBool() ? 0 : fnArnoldRenderOptions.findPlug("threads").asInt());
      AiNodeSetInt(AiUniverseGetOptions(), "bucket_scanning", fnArnoldRenderOptions.findPlug("bucket_scanning").asInt());
      AiNodeSetInt(AiUniverseGetOptions(), "bucket_size", fnArnoldRenderOptions.findPlug("bucket_size").asInt());
      AiNodeSetBool(AiUniverseGetOptions(), "abort_on_error", fnArnoldRenderOptions.findPlug("abort_on_error").asBool());

      AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", fnArnoldRenderOptions.findPlug("AA_samples").asInt());
      AiNodeSetInt(AiUniverseGetOptions(), "GI_hemi_samples", fnArnoldRenderOptions.findPlug("GI_hemi_samples").asInt());
      AiNodeSetInt(AiUniverseGetOptions(), "GI_specular_samples", fnArnoldRenderOptions.findPlug("GI_specular_samples").asInt());
      AiNodeSetFlt(AiUniverseGetOptions(), "AA_sample_clamp", fnArnoldRenderOptions.findPlug("use_sample_clamp").asBool() ? fnArnoldRenderOptions.findPlug("AA_sample_clamp").asFloat() : (float) AI_INFINITE);

      m_gamma = fnArnoldRenderOptions.findPlug("driver_gamma").asFloat();

      AiNodeSetFlt(AiUniverseGetOptions(), "TM_lgamma", fnArnoldRenderOptions.findPlug("TM_lgamma").asFloat());
      AiNodeSetFlt(AiUniverseGetOptions(), "TM_sgamma", fnArnoldRenderOptions.findPlug("TM_sgamma").asFloat());
      AiNodeSetFlt(AiUniverseGetOptions(), "TM_tgamma", fnArnoldRenderOptions.findPlug("TM_tgamma").asFloat());

      m_clearBeforeRender = fnArnoldRenderOptions.findPlug("clear_before_render").asBool();

      AiNodeSetInt(AiUniverseGetOptions(), "GI_diffuse_depth", fnArnoldRenderOptions.findPlug("GI_diffuse_depth").asInt());
      AiNodeSetInt(AiUniverseGetOptions(), "GI_glossy_depth", fnArnoldRenderOptions.findPlug("GI_glossy_depth").asInt());
      AiNodeSetInt(AiUniverseGetOptions(), "GI_reflection_depth", fnArnoldRenderOptions.findPlug("GI_reflection_depth").asInt());
      AiNodeSetInt(AiUniverseGetOptions(), "GI_refraction_depth", fnArnoldRenderOptions.findPlug("GI_refraction_depth").asInt());
      AiNodeSetInt(AiUniverseGetOptions(), "GI_total_depth", fnArnoldRenderOptions.findPlug("GI_total_depth").asInt());
   }
}
