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
  m_shader_searchpath(""),
  m_pixelAspectRatio(1.0f),
  m_minx(0), m_miny(0), m_maxx(0), m_maxy(0),
  m_width(0), m_height(0),
  m_log_max_warnings(100),
  m_log_verbosity(DEFAULT_LOG_FLAGS),
  m_outputAssMask(AI_NODE_ALL),
  m_useRenderRegion(false),
  m_clearBeforeRender(false),
  m_useBinaryEncoding(true),
  m_log_to_file(false),
  m_log_to_console(false),
  m_expandProcedurals(false),
  m_force_translate_shading_engines(false),
  m_use_existing_tiled_textures(true),
  m_autotx(true),
  m_multiCameraRender(false)
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

      m_useRenderRegion = fnRenderGlobals.findPlug("useRenderRegion").asBool();

      MRenderUtil::getCommonRenderSettings(m_defaultRenderGlobalsData);

      // unsigned int   imageFormat
      // MString     customImageFormat
      // MString     customExt;
      // m_imageFormat = m_defaultRenderGlobalsData.customImageFormat();

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
      m_progressive_rendering     = fnArnoldRenderOptions.findPlug("progressive_rendering").asBool();
      m_progressive_initial_level = fnArnoldRenderOptions.findPlug("progressive_initial_level").asInt();
      m_clearBeforeRender = fnArnoldRenderOptions.findPlug("clear_before_render").asBool();
      m_useBinaryEncoding = fnArnoldRenderOptions.findPlug("binaryAss").asBool();
      m_forceSceneUpdateBeforeIPRRefresh = fnArnoldRenderOptions.findPlug("force_scene_update_before_IPR_refresh").asBool();
      m_forceTextureCacheFlushAfterRender = fnArnoldRenderOptions.findPlug("force_texture_cache_flush_after_render").asBool();
      
      m_autotx = fnArnoldRenderOptions.findPlug("autotx").asBool();

      // if auto-tx is ON, we force use TX to be ON
      m_use_existing_tiled_textures = (m_autotx) ? true : fnArnoldRenderOptions.findPlug("use_existing_tiled_textures").asBool();

      m_outputAssFile       = fnArnoldRenderOptions.findPlug("output_ass_filename").asString();
      m_outputAssMask       = fnArnoldRenderOptions.findPlug("output_ass_mask").asInt();


      m_log_to_file           = fnArnoldRenderOptions.findPlug("log_to_file").asBool();
      m_log_to_console        = fnArnoldRenderOptions.findPlug("log_to_console").asBool();
      m_log_filename          = fnArnoldRenderOptions.findPlug("log_filename").asString();
      m_log_max_warnings      = fnArnoldRenderOptions.findPlug("log_max_warnings").asInt();
      m_log_verbosity = GetFlagsFromVerbosityLevel(fnArnoldRenderOptions.findPlug("log_verbosity").asInt());

      m_shader_searchpath = fnArnoldRenderOptions.findPlug("shader_searchpath").asString();

      status = MStatus::kSuccess;
   }
   else
   {
      AiMsgError("[mtoa] No known Arnold render options");
      status = MStatus::kFailure;
   }

   return status;
}


void CRenderOptions::SetupLog() const
{
   if ((m_log_filename != "") && (m_log_to_file))
   {
      // this replaces the MAYA_PROJECT_PATH with the actual project path
      // if there are no such environment variables are declared
      MString logPath = m_log_filename;
      if (m_log_filename.substringW(0, 14) == MString("$MTOA_LOG_PATH/"))
      {
         if (getenv("MTOA_LOG_PATH") == 0)
         {
            MString result;
            MGlobal::executeCommand("workspace -q -directory", result);
            logPath = result + m_log_filename.substringW(15, m_log_filename.length());
         }
      }
      int extPos = logPath.rindexW('.');
      int frame;
      MGlobal::executeCommand("currentTime -q", frame);
      if (extPos > 0) // The file name has extension
         logPath = logPath.substringW(0, extPos) + frame + logPath.substringW(extPos, logPath.length());
      else
      {
         unsigned int slashPos = logPath.rindexW('/');
         if (slashPos+1 < logPath.length()) // File name without extension
            logPath = logPath +"."+ frame + ".log";
         else // No file name
            logPath = logPath + frame + ".log";
      }
      AiMsgSetLogFileName(logPath.expandEnvironmentVariablesAndTilde().asChar());
      AiMsgSetLogFileFlags(m_log_verbosity);
      AiMsgResetCallback();
   } else if (m_log_to_console)
   {
      // no "Log to File" enabled. Therefore we can rely on MtoA's callback
      // that invokes Maya log functions.
      // The reason why we don't always enable it is that it should also 
      // handle the "log to file" and we prefer letting arnold do it.
      AiMsgSetCallback(MtoaLogCallback);
   } else
      AiMsgResetCallback();
   
   AiMsgSetMaxWarnings(m_log_max_warnings);
   if (m_log_to_console)
      AiMsgSetConsoleFlags(m_log_verbosity | AI_LOG_COLOR);   
   
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
   AiNodeSetFlt(options, "aspect_ratio", pixelAspectRatio());
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

