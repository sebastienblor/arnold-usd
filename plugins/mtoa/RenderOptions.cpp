
#include "RenderOptions.h"
#include "maya_scene/MayaScene.h"

#include <ai_constants.h>
#include <ai_nodes.h>
#include <ai_universe.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MSelectionList.h>

CRenderOptions::CRenderOptions()
:  m_minx(0), m_miny(0), m_maxx(0), m_maxy(0)
,  m_width(0), m_height(0)
,  m_pixelAspectRatio(1.0f)
,  m_useRenderRegion(false)
,  m_clearBeforeRender(false)
,  m_gamma(1.0f)
,  m_scene(NULL)
{
}

void CRenderOptions::GetRenderOptions(CMayaScene* scene)
{
   m_scene = scene;

   ProcessCommonRenderOptions();
   ProcessArnoldRenderOptions();
}

void CRenderOptions::ProcessCommonRenderOptions()
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

void CRenderOptions::ProcessArnoldRenderOptions()
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

      AiNodeSetInt(AiUniverseGetOptions(), "max_subdivisions", fnArnoldRenderOptions.findPlug("max_subdivisions").asInt());

      m_outputAssFile       = fnArnoldRenderOptions.findPlug("output_ass_filename").asString();
      m_outputAssCompressed = fnArnoldRenderOptions.findPlug("output_ass_compressed").asBool();

      // If the scene has not been set, avoid crashing by using a NULL pointer
      if (!m_scene)
         return;

      // BACKGROUND SHADER
      //
      int background = fnArnoldRenderOptions.findPlug("background").asInt();

      list.clear();

      switch (background)
      {
      case 0:
         break;

      case 1:  // Sky
         list.add("defaultSkyShader");
         if (list.length() > 0)
         {
            list.getDependNode(0, node);
            AiNodeSetPtr(AiUniverseGetOptions(), "background", m_scene->ExportShader(node));
         }
         break;
      }

      // ATMOSPHERE SHADER
      //
      int atmosphere = fnArnoldRenderOptions.findPlug("atmosphere").asInt();

      list.clear();

      switch (atmosphere)
      {
      case 0:
         break;

      case 1:  // Fog
         list.add("defaultFogShader");
         if (list.length() > 0)
         {
            list.getDependNode(0, node);
            AiNodeSetPtr(AiUniverseGetOptions(), "atmosphere", m_scene->ExportShader(node));
         }
         break;

      case 2:  // Volume Scattering
         list.add("defaultVolumeScatteringShader");
         if (list.length() > 0)
         {
            list.getDependNode(0, node);
            AiNodeSetPtr(AiUniverseGetOptions(), "atmosphere", m_scene->ExportShader(node));
         }
         break;
      }
   }
}
