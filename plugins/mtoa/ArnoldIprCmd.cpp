
#include "ArnoldIprCmd.h"
#include "OutputDriver.h"

#include <ai_api.h>
#include <ai_constants.h>
#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_threads.h>
#include <ai_universe.h>
#include <ai_version.h>

#include <maya/M3dView.h>
#include <maya/MArgDatabase.h>
#include <maya/MComputation.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MRenderView.h>
#include <maya/MSelectionList.h>

extern AtNodeMethods* mtoa_driver_mtd;

// This is the code for the render thread. This thread is used only to run the AiRender() process outside of the main thread.
static unsigned int RenderThread(AtVoid* data)
{
   AiRender( AI_RENDER_MODE_CAMERA );

   return 0;
}

MSyntax CArnoldIprCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("m", "mode", MSyntax::MArgType::kString);

   return syntax;
}

CArnoldIprCmd::CArnoldIprCmd()
:  m_minx(0), m_miny(0), m_maxx(0), m_maxy(0)
,  m_width(0), m_height(0)
,  m_pixelAspectRatio(1.0f)
,  m_useRenderRegion(false)
,  m_clearBeforeRender(false)
,  m_gamma(1.0f)
{
}

MStatus CArnoldIprCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList);

   // "-mode" flag is not set, so we simply return a bool with the rendering status
   if (!args.isFlagSet("mode"))
   {
      setResult(AiUniverseIsActive());

      return status;
   }

   // Get argument to "-mode" flag
   MString mode = args.flagArgumentString("mode", 0);

   printf("MODE = %s\n", mode);

   return status;
   if (mode == "start")
   {
      if (!AiUniverseIsActive())
      {
         if (!MRenderView::doesRenderEditorExist())
            return MStatus::kFailure;

         AiBegin();

         // TODO: For now, we will use stdout (in Maya, it will go to the output window)
         AiSetLogOptions(NULL, AI_LOG_ALL, 1000, 4);

         ProcessCommonRenderOptions();
         ProcessArnoldRenderOptions();

         status = m_scene.ExportToArnold();

         MDagPath cameraPath;

         M3dView::active3dView().getCamera(cameraPath);
         MRenderView::setCurrentCamera(cameraPath);

         InitOutputDriver();

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
         if (AiRendering())
            AiRenderInterrupt();
      }
   }
   else if (mode == "stop")
   {
   }
   else if (mode == "render")
   {
   }

   MComputation comp;

   comp.beginComputation();

   status = m_useRenderRegion ? MRenderView::startRegionRender(m_width, m_height, m_minx, m_maxx, m_miny, m_maxy, !m_clearBeforeRender, true)
                              : MRenderView::startRender(m_width, m_height, !m_clearBeforeRender, true);

   if ( status == MS::kSuccess)
   {
      Render();

      MRenderView::endRender();
   }

   comp.endComputation();

   AiEnd();

   return status;
}  // doIt()

void CArnoldIprCmd::InitOutputDriver()
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

void CArnoldIprCmd::Render()
{
   InitializeDisplayUpdateQueue();

   AtVoid* handler = AiThreadCreate(RenderThread, NULL, AI_PRIORITY_LOW);

   // Process messages sent by the render thread, and exit when rendering is finished or aborted
   ProcessDisplayUpdateQueue();

   // Wait for the render thread to release everything and close it
   AiThreadWait(handler);
   AiThreadClose(handler);
}  // Render()

void CArnoldIprCmd::ProcessCommonRenderOptions()
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

void CArnoldIprCmd::ProcessArnoldRenderOptions()
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

      int background = fnArnoldRenderOptions.findPlug("background").asInt();

      list.clear();

      switch (background)
      {
      case 0:
         break;

      case 1:  // Image
         list.add("defaultBackgroundImageShader");
         if (list.length() > 0)
         {
            list.getDependNode(0, node);
            AiNodeSetPtr(AiUniverseGetOptions(), "background", m_scene.ExportShader(node));
         }
         break;

      case 2:  // Sky
         list.add("defaultSkyShader");
         if (list.length() > 0)
         {
            list.getDependNode(0, node);
            AiNodeSetPtr(AiUniverseGetOptions(), "background", m_scene.ExportShader(node));
         }
         break;

      case 3:  // Sky_HDRI
         list.add("defaultSky_HDRIShader");
         if (list.length() > 0)
         {
            list.getDependNode(0, node);
            AiNodeSetPtr(AiUniverseGetOptions(), "background", m_scene.ExportShader(node));
         }
         break;
      }
   }
}
