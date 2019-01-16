#include "platform/Platform.h"
#include "utils/MtoaLog.h"
#include "RenderOptions.h"
#include "scene/MayaScene.h"
#include "translators/options/OptionsTranslator.h"
#include "extension/ExtensionsManager.h"

#include <ai_constants.h>
#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_universe.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MSelectionList.h>
#include <maya/MAnimControl.h>
#include <maya/MRenderView.h>
#include <maya/MGlobal.h>
#include <maya/MTime.h>
#include <maya/MRenderUtil.h> 
#include <maya/MFnRenderLayer.h>
#include <maya/MObject.h>
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>

#ifdef _WIN32
#include <direct.h>
#endif // _WIN32

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern AtNodeMethods* mtoa_driver_mtd;

CRenderOptions::CRenderOptions() 
: m_log_filename(""),
  m_plugin_searchpath(""),
  m_pixelAspectRatio(1.0f),
  m_minx(0), m_miny(0), m_maxx(0), m_maxy(0),
  m_width(0), m_height(0),
  m_log_max_warnings(100),
  m_log_verbosity(DEFAULT_LOG_FLAGS),
  m_outputAssMask(AI_NODE_ALL),
  m_stats_mode(AI_STATS_MODE_APPEND),
  m_useRenderRegion(false),
  m_clearBeforeRender(false),
  m_useBinaryEncoding(true),
  m_log_to_file(false),
  m_log_to_console(false),
  m_expandProcedurals(false),
  m_force_translate_shading_engines(false),
  m_use_existing_tiled_textures(true),
  m_autotx(true),
  m_multiCameraRender(false),
  m_mtoa_translation_info(false),
  m_exportShadingEngine(false),
  m_stats_enable(false),
  m_profile_enable(false)
{}

MStatus CRenderOptions::GetFromMaya()
{
   MStatus status;
   status = ProcessCommonRenderOptions();
   status = ProcessArnoldRenderOptions();
   return status;
}

// Unused by AOV branch but that will mess existing scripts
// TODO : share this with the UI proc (call a Python script)
// so that we know they're both in sync
MString CRenderOptions::GetFileExtension(const MString& imageRenderFormat) const
{
   MString imageFileExtension;

   if (imageRenderFormat == "exr")
   {
      imageFileExtension = "exr";
   }
   else if (imageRenderFormat == "tiff")
   {
      imageFileExtension = "tif";
   }
   else if (imageRenderFormat == "jpeg")
   {
      imageFileExtension = "jpg";
   }
   else if (imageRenderFormat == "png")
   {
      imageFileExtension = "png";
   }
   else
   {
      AiMsgError("[mtoa] Unknown image render format '%s'", imageRenderFormat.asChar());
      imageFileExtension = "";
   }

   return imageFileExtension;
}

MStatus CRenderOptions::ProcessCommonRenderOptions()
{
   MStatus        status;
   MSelectionList list;
   MObject        node;
   MTime          time;

   list.add("defaultRenderGlobals");

   if (list.length() > 0)
   {
      list.getDependNode(0, node);

      MFnDependencyNode fnRenderGlobals(node);

      m_useRenderRegion = fnRenderGlobals.findPlug("useRenderRegion", true).asBool();

      MRenderUtil::getCommonRenderSettings(m_defaultRenderGlobalsData);

      // unsigned int   imageFormat
      // MString     customImageFormat
      // MString     customExt;
      // m_imageFormat = m_defaultRenderGlobalsData.customImageFormat();

      if (m_useRenderRegion)
      {
         m_minx = fnRenderGlobals.findPlug("left", true).asInt();
         m_miny = fnRenderGlobals.findPlug("bot", true).asInt();
         m_maxx = fnRenderGlobals.findPlug("rght", true).asInt();
         m_maxy = fnRenderGlobals.findPlug("top", true).asInt();
      }

      MPlugArray connectedPlugs;
      MPlug      resPlug = fnRenderGlobals.findPlug("resolution", true);

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

            m_width  = fnRes.findPlug("width", true).asInt();
            m_height = fnRes.findPlug("height", true).asInt();
            m_pixelAspectRatio = 1.0f / (((float)m_height / m_width) * fnRes.findPlug("deviceAspectRatio", true).asFloat());
         }
      }

      status = MStatus::kSuccess;
   }
   else
   {
      AiMsgError("[mtoa] No known common render options");
      status = MStatus::kFailure;
   }

   return status;
}

MStatus CRenderOptions::ProcessArnoldRenderOptions()
{
   MStatus status;

   MObject node = GetArnoldRenderOptions();
   if (node != MObject::kNullObj)
   {
      MPlugArray conns;
      MFnDependencyNode fnArnoldRenderOptions(node, &status);

      if (status != MStatus::kSuccess)
      {
         AiMsgError("[mtoa] Invalid Arnold render options node");
         return status;
      }
      m_progressive_rendering     = fnArnoldRenderOptions.findPlug("progressive_rendering", true).asBool();
      m_progressive_initial_level = fnArnoldRenderOptions.findPlug("progressive_initial_level", true).asInt();
      m_clearBeforeRender = fnArnoldRenderOptions.findPlug("clear_before_render", true).asBool();
      m_useBinaryEncoding = fnArnoldRenderOptions.findPlug("binaryAss", true).asBool();
      m_forceSceneUpdateBeforeIPRRefresh = fnArnoldRenderOptions.findPlug("force_scene_update_before_IPR_refresh", true).asBool();
      m_forceTextureCacheFlushAfterRender = fnArnoldRenderOptions.findPlug("force_texture_cache_flush_after_render", true).asBool();
      m_mtoa_translation_info = fnArnoldRenderOptions.findPlug("mtoa_translation_info", true).asBool();
      
      m_autotx = fnArnoldRenderOptions.findPlug("autotx", true).asBool();

      // if auto-tx is ON, we force use TX to be ON
      m_use_existing_tiled_textures = (m_autotx) ? true : fnArnoldRenderOptions.findPlug("use_existing_tiled_textures", true).asBool();

      m_outputAssFile       = fnArnoldRenderOptions.findPlug("output_ass_filename", true).asString();
      m_outputAssMask       = fnArnoldRenderOptions.findPlug("output_ass_mask", true).asInt();
      m_exportShadingEngine = fnArnoldRenderOptions.findPlug("exportShadingEngine", true).asBool();

      m_log_to_file           = fnArnoldRenderOptions.findPlug("log_to_file", true).asBool();
      m_log_to_console        = fnArnoldRenderOptions.findPlug("log_to_console", true).asBool();
      m_log_filename          = fnArnoldRenderOptions.findPlug("log_filename", true).asString();
      m_log_max_warnings      = fnArnoldRenderOptions.findPlug("log_max_warnings", true).asInt();
      m_log_verbosity = GetFlagsFromVerbosityLevel(fnArnoldRenderOptions.findPlug("log_verbosity", true).asInt());

      m_stats_enable         =  fnArnoldRenderOptions.findPlug("stats_enable",true).asBool();
      m_stats_mode           = fnArnoldRenderOptions.findPlug("stats_mode", true).asInt();
      m_stats_file           = fnArnoldRenderOptions.findPlug("stats_file", true).asString();
      m_profile_enable         =  fnArnoldRenderOptions.findPlug("profile_enable", true).asBool();
      m_profile_file           = fnArnoldRenderOptions.findPlug("profile_file", true).asString();

      m_plugin_searchpath = fnArnoldRenderOptions.findPlug("plugin_searchpath", true).asString();

      status = MStatus::kSuccess;
   }
   else
   {
      AiMsgError("[mtoa] No known Arnold render options");
      status = MStatus::kFailure;
   }

   return status;
}

static MString ExpandMtoaLogPath(const MString &file)
{
   MString res = file;
   if (file.substringW(0, 14) == MString("$MTOA_LOG_PATH/"))
   {
      if (getenv("MTOA_LOG_PATH") == 0)
      {
         MString workspaceFolder;
         MGlobal::executeCommand("workspace -q -directory", workspaceFolder);
         res = workspaceFolder + file.substringW(15, file.length());
      }
   }
   int extPos = res.rindexW('.');
   int frame;
   MGlobal::executeCommand("currentTime -q", frame);
   if (extPos > 0) // The file name has extension
      res = res.substringW(0, extPos) + frame + res.substringW(extPos, res.length());
   else
   {
      unsigned int slashPos = res.rindexW('/');
      if (slashPos+1 < res.length()) // File name without extension
         res = res +"."+ frame + ".log";
      else // No file name
         res = res + frame + ".log";
   }
   return res;
}
void CRenderOptions::SetupLog() const
{
   if ((m_log_filename != "") && (m_log_to_file))
   {
      // this replaces the MAYA_PROJECT_PATH with the actual project path
      // if there are no such environment variables are declared
      MString logPath = ExpandMtoaLogPath(m_log_filename);
      AiMsgSetLogFileName(logPath.expandEnvironmentVariablesAndTilde().asChar());
      AiMsgSetLogFileFlags(m_log_verbosity);
      AiMsgResetCallback();
   } else if (m_log_to_console && MGlobal::mayaState() != MGlobal::kBatch)
   {
      // no "Log to File" enabled, and we're not doing batch rendering.
      // Therefore we can rely on MtoA's callback
      // that invokes Maya log functions.
      // The reason why we don't always enable it is that it should also 
      // handle the "log to file" and we prefer letting arnold do it.

      AiMsgSetCallback(MtoaLogCallback);
   } else
      AiMsgResetCallback();
   
   AiMsgSetMaxWarnings(m_log_max_warnings);
   if (m_log_to_console)
      AiMsgSetConsoleFlags(m_log_verbosity | AI_LOG_COLOR);   
   
   // Stats
   if (m_stats_enable)
   {
      MString statsPath = ExpandMtoaLogPath(m_stats_file);
      AiStatsSetFileName(statsPath.asChar());
      AiStatsSetMode((AtStatsMode)m_stats_mode);
   } else
      AiStatsSetFileName("");

   // Profile
   if (m_profile_enable)
   {
      MString profilePath = ExpandMtoaLogPath(m_profile_file);
      AiProfileSetFileName(profilePath.asChar());
   } else
      AiProfileSetFileName("");

}

void CRenderOptions::SetCamera(MDagPath& camera)
{
   m_camera = camera;
}

void CRenderOptions::UpdateImageDimensions()
{
   AtNode* options = AiUniverseGetOptions();
   if (useRenderRegion())
   {
      AiNodeSetInt(options, "region_min_x", minX());
      AiNodeSetInt(options, "region_min_y", height() - maxY() - 1);
      AiNodeSetInt(options, "region_max_x", maxX());
      AiNodeSetInt(options, "region_max_y", height() - minY() - 1);
   }

   AiNodeSetInt(options, "xres", width());
   AiNodeSetInt(options, "yres", height());

   float aspect_ratio = pixelAspectRatio();

   if (std::abs(aspect_ratio - 1.f) < 0.001)
      aspect_ratio = 1.f;
   else
      aspect_ratio = 1.f / AiMax(AI_EPSILON, aspect_ratio);

   AiNodeSetFlt(options, "pixel_aspect_ratio", aspect_ratio);

}

int CRenderOptions::GetLogConsoleVerbosity() const
{
   if (m_log_to_console)
   {
      return m_log_verbosity | AI_LOG_COLOR;
   } else { 
      // I'm missing some situations here, as the logging can be set from different place.
      // Ideally we'd like to have a function AiMsgGetConsoleFlags 
      return DEFAULT_LOG_FLAGS;
   }
}
int CRenderOptions::GetLogFileVerbosity() const
{
   if ((m_log_filename != "") && (m_log_to_file))
   {
      return m_log_verbosity;
   } else 
   {
      // I'm missing some situations here, as the logging can be set from different place.
      // Ideally we'd like to have a function AiMsgGetFileFlags 
      return DEFAULT_LOG_FLAGS;
   }

}

