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
      MFnDependencyNode fnArnoldRenderOptions(node);

      m_progressive_rendering     = fnArnoldRenderOptions.findPlug("progressive_rendering").asBool();
      m_progressive_initial_level = fnArnoldRenderOptions.findPlug("progressive_initial_level").asInt();
      m_clearBeforeRender = fnArnoldRenderOptions.findPlug("clear_before_render").asBool();
      m_useBinaryEncoding = fnArnoldRenderOptions.findPlug("binaryAss").asBool();
      m_forceSceneUpdateBeforeIPRRefresh = fnArnoldRenderOptions.findPlug("force_scene_update_before_IPR_refresh").asBool();
      m_forceTextureCacheFlushAfterRender = fnArnoldRenderOptions.findPlug("force_texture_cache_flush_after_render").asBool();
      
      m_use_existing_tiled_textures = fnArnoldRenderOptions.findPlug("use_existing_tiled_textures").asBool();

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
   }
   
   AiMsgSetMaxWarnings(m_log_max_warnings);
   if (m_log_to_console)
      AiMsgSetConsoleFlags(m_log_verbosity | AI_LOG_COLOR);   

   // Not working correctly until we can add to callback rather than replace it,
   // or have access to original callback code
   // Callback for script editor echo has to be disabled, because not way to know
   // the log filename and write to it from callback
   // AiMsgSetCallback(MtoaLogCallback);
   // AiMsgResetCallback();
}

void CRenderOptions::SetCamera(MDagPath& camera)
{
   m_camera = camera;
}

void ParseOverscanSettings(const MString& s, float& overscan, bool& isPercent)
{
   std::cerr << "Parsing string : " << s.asChar() << std::endl;
   MString ms = s;
   ms.toLowerCase();
   if (ms.rindex('%') == (ms.length() - 1))
   {
      isPercent = true;
      ms = ms.substring(0, ms.length() - 1);
   }
   else if (ms.rindexW(MString("px")) == (ms.length() - 2))
   {
      isPercent = false;
      ms = ms.substring(0, ms.length() - 2);
   }
   else
      isPercent = false;

   if (ms.isInt())
      overscan = (float)ms.asInt();
   else if (ms.isFloat())
      overscan = ms.asFloat();
   else if (ms.isDouble())
      overscan = (float)ms.asDouble();
   else
      overscan = 0.0f;

   if (overscan < AI_EPSILON)
      overscan = 0.0f;
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
   else
   {
      MObject node = GetArnoldRenderOptions();
      if (node != MObject::kNullObj)
      {
         MFnDependencyNode fnArnoldRenderOptions(node);
         
         float overscanL = 0.0f;
         float overscanR = 0.0f;
         float overscanT = 0.0f;
         float overscanB = 0.0f;
         bool overscanLP = false;
         bool overscanRP = false;
         bool overscanTP = false;
         bool overscanBP = false;

         MString overscanString = fnArnoldRenderOptions.findPlug("outputOverscan").asString();
         std::cerr << overscanString.asChar() << std::endl;
         MStringArray split;
         overscanString.split(' ', split);
         const unsigned int splitLength = split.length();
         std::cerr << "Split length : " << splitLength << std::endl;
         if (splitLength == 1)
         {
            ParseOverscanSettings(split[0], overscanL, overscanLP);
            overscanR = overscanL;
            overscanT = overscanL;
            overscanB = overscanL;
            overscanRP = overscanLP;
            overscanTP = overscanLP;
            overscanBP = overscanLP;
         }
         else if (splitLength == 2)
         {
            ParseOverscanSettings(split[0], overscanT, overscanTP);
            overscanB = overscanT;
            overscanBP = overscanTP;
            ParseOverscanSettings(split[1], overscanL, overscanLP);
            overscanR = overscanL;
            overscanRP = overscanLP;
         }
         else if (splitLength == 3)
         {
            ParseOverscanSettings(split[0], overscanT, overscanTP);
            ParseOverscanSettings(split[1], overscanL, overscanLP);
            overscanR = overscanL;
            overscanRP = overscanLP;
            ParseOverscanSettings(split[2], overscanB, overscanBP);
         }
         else if (splitLength == 4)
         {
            ParseOverscanSettings(split[0], overscanT, overscanTP);
            ParseOverscanSettings(split[1], overscanR, overscanRP);
            ParseOverscanSettings(split[2], overscanB, overscanBP);
            ParseOverscanSettings(split[3], overscanL, overscanLP);
         }

         AiNodeSetInt(options, "region_min_x", overscanLP ? (int)ceilf(-(float)width() * overscanL) : -(int)overscanL);
         AiNodeSetInt(options, "region_max_x", overscanRP ? width() + (int)ceilf((float)width() * overscanR) : width() + (int)overscanR);
         AiNodeSetInt(options, "region_min_y", overscanTP ? (int)ceilf(-(float)height() * overscanT) : -(int)overscanT);
         AiNodeSetInt(options, "region_max_y", overscanBP ? height() + (int)ceilf((float)height() * overscanB) : height() + (int)overscanB);

         std::cerr << AiNodeGetInt(options, "region_min_x") << " - " << AiNodeGetInt(options, "region_max_x") << " - " << AiNodeGetInt(options, "region_min_y") << " - " << AiNodeGetInt(options, "region_max_y") << std::endl;
      }      
   }   

   AiNodeSetInt(options, "xres", width());
   AiNodeSetInt(options, "yres", height());
   AiNodeSetFlt(options, "aspect_ratio", pixelAspectRatio());
}

