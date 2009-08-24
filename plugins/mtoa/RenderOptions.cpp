
#include "RenderOptions.h"
#include "maya_scene/MayaScene.h"

#include <ai_constants.h>
#include <ai_msg.h>
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
,  m_output_gamma(1.0f)
,  m_scene(NULL)
,  m_outputAssMask(AI_NODE_ALL)
,  m_log_filename("")
,  m_log_max_warnings(100)
,  m_log_console_verbosity(5)
,  m_log_file_verbosity(5)
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
            m_pixelAspectRatio = 1.0f / (((float)m_height / m_width) * fnRes.findPlug("deviceAspectRatio").asFloat());
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

      m_threads = fnArnoldRenderOptions.findPlug("threads_autodetect").asBool() ? 0 : fnArnoldRenderOptions.findPlug("threads").asInt();
      m_bucket_scanning = fnArnoldRenderOptions.findPlug("bucket_scanning").asInt();
      m_bucket_size = fnArnoldRenderOptions.findPlug("bucket_size").asInt();
      m_abort_on_error = fnArnoldRenderOptions.findPlug("abort_on_error").asBool();

      m_AA_samples = fnArnoldRenderOptions.findPlug("AA_samples").asInt();
      m_GI_hemi_samples = fnArnoldRenderOptions.findPlug("GI_hemi_samples").asInt();
      m_GI_specular_samples = fnArnoldRenderOptions.findPlug("GI_specular_samples").asInt();
      m_AA_sample_clamp = fnArnoldRenderOptions.findPlug("use_sample_clamp").asBool() ? fnArnoldRenderOptions.findPlug("AA_sample_clamp").asFloat() : (float) AI_INFINITE;


      m_output_gamma = fnArnoldRenderOptions.findPlug("driver_gamma").asFloat();
      m_TM_lgamma    = fnArnoldRenderOptions.findPlug("TM_lgamma").asFloat();
      m_TM_sgamma    = fnArnoldRenderOptions.findPlug("TM_sgamma").asFloat();
      m_TM_tgamma    = fnArnoldRenderOptions.findPlug("TM_tgamma").asFloat();

      m_clearBeforeRender = fnArnoldRenderOptions.findPlug("clear_before_render").asBool();

      m_GI_diffuse_depth    = fnArnoldRenderOptions.findPlug("GI_diffuse_depth").asInt();
      m_GI_glossy_depth     = fnArnoldRenderOptions.findPlug("GI_glossy_depth").asInt();
      m_GI_reflection_depth = fnArnoldRenderOptions.findPlug("GI_reflection_depth").asInt();
      m_GI_refraction_depth = fnArnoldRenderOptions.findPlug("GI_refraction_depth").asInt();
      m_GI_total_depth      = fnArnoldRenderOptions.findPlug("GI_total_depth").asInt();

      m_max_subdivisions = fnArnoldRenderOptions.findPlug("max_subdivisions").asInt();

      m_outputAssFile       = fnArnoldRenderOptions.findPlug("output_ass_filename").asString();
      m_outputAssCompressed = fnArnoldRenderOptions.findPlug("output_ass_compressed").asBool();
      m_outputAssMask       = fnArnoldRenderOptions.findPlug("output_ass_mask").asInt();

      m_log_filename          = fnArnoldRenderOptions.findPlug("log_filename").asString();
      m_log_max_warnings      = fnArnoldRenderOptions.findPlug("log_max_warnings").asInt();
      m_log_console_verbosity = fnArnoldRenderOptions.findPlug("log_console_verbosity").asInt();
      m_log_file_verbosity    = fnArnoldRenderOptions.findPlug("log_file_verbosity").asInt();

      m_background = fnArnoldRenderOptions.findPlug("background").asInt();
      m_atmosphere = fnArnoldRenderOptions.findPlug("atmosphere").asInt();
   }
}

void CRenderOptions::SetupRender() const
{
   SetupLog();
   SetupImageOptions();

   AiNodeSetInt(AiUniverseGetOptions(), "threads", m_threads);
   AiNodeSetInt(AiUniverseGetOptions(), "bucket_scanning", m_bucket_scanning);
   AiNodeSetInt(AiUniverseGetOptions(), "bucket_size", m_bucket_size);
   AiNodeSetBool(AiUniverseGetOptions(), "abort_on_error", m_abort_on_error);

   AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", m_AA_samples);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_hemi_samples", m_GI_hemi_samples);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_specular_samples", m_GI_specular_samples);
   AiNodeSetFlt(AiUniverseGetOptions(), "AA_sample_clamp", m_AA_sample_clamp);

   AiNodeSetFlt(AiUniverseGetOptions(), "TM_lgamma", m_TM_lgamma);
   AiNodeSetFlt(AiUniverseGetOptions(), "TM_sgamma", m_TM_sgamma);
   AiNodeSetFlt(AiUniverseGetOptions(), "TM_tgamma", m_TM_tgamma);

   AiNodeSetInt(AiUniverseGetOptions(), "GI_diffuse_depth", m_GI_diffuse_depth);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_glossy_depth", m_GI_glossy_depth);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_reflection_depth", m_GI_reflection_depth);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_refraction_depth", m_GI_refraction_depth);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_total_depth", m_GI_total_depth);

   AiNodeSetInt(AiUniverseGetOptions(), "max_subdivisions", m_max_subdivisions);

   // If the scene has not been set, avoid crashing by using a NULL pointer
   if (!m_scene)
      return;

   MSelectionList list;
   MObject        node;

   // BACKGROUND SHADER
   //
   list.clear();

   switch (m_background)
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
   list.clear();

   switch (m_atmosphere)
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

void CRenderOptions::SetupLog() const
{
   if (m_log_filename != "")
      AiMsgSetLogFileName(m_log_filename.asChar());

   AiMsgSetMaxWarnings(m_log_max_warnings);
   AiMsgSetConsoleFlags(GetFlagsFromVerbosityLevel(m_log_console_verbosity));
   AiMsgSetLogFileFlags(GetFlagsFromVerbosityLevel(m_log_file_verbosity));
}

void CRenderOptions::SetupImageOptions() const
{
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

AtInt CRenderOptions::GetFlagsFromVerbosityLevel(AtUInt level) const
{
   AtInt flags = 0;

   switch(level)
   {
   case 6:  flags = AI_LOG_ALL; break;
   case 5:  flags = AI_LOG_ALL & ~AI_LOG_DEBUG; break;
   case 4:  flags |= AI_LOG_PLUGINS;
   case 3:  flags |= AI_LOG_STATS;
   case 2:  flags |= AI_LOG_PROGRESS;
   case 1:  flags |= AI_LOG_INFO | AI_LOG_WARNINGS | AI_LOG_ERRORS | AI_LOG_TIMESTAMP | AI_LOG_BACKTRACE | AI_LOG_MEMORY; break;
   case 0:  flags = 0; break;
   }

   return flags;
}
