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

CRenderOptions::CRenderOptions()
:  m_minx(0), m_miny(0), m_maxx(0), m_maxy(0)
,  m_width(0), m_height(0)
,  m_pixelAspectRatio(1.0f)
,  m_useRenderRegion(false)
,  m_clearBeforeRender(false)
,  m_multiCameraRender(false)
,  m_use_existing_tiled_textures(true)
,  m_outputAssMask(AI_NODE_ALL)
,  m_log_filename("")
,  m_log_max_warnings(100)
,  m_log_console_verbosity(5)
,  m_log_file_verbosity(5)
{}

void CRenderOptions::GetFromMaya()
{
   ProcessCommonRenderOptions();
   ProcessArnoldRenderOptions();
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

void CRenderOptions::ProcessCommonRenderOptions()
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

   }
}

void CRenderOptions::ProcessArnoldRenderOptions()
{
   MObject node = GetArnoldRenderOptions();
   if (node != MObject::kNullObj)
   {
      MPlugArray conns;
      MFnDependencyNode fnArnoldRenderOptions(node);

      m_progressive_rendering    = fnArnoldRenderOptions.findPlug("progressive_rendering").asBool();
      m_sss_sample_factor        = fnArnoldRenderOptions.findPlug("sss_sample_factor").asInt();
      m_clearBeforeRender = fnArnoldRenderOptions.findPlug("clear_before_render").asBool();
      
      m_use_existing_tiled_textures = fnArnoldRenderOptions.findPlug("use_existing_tiled_textures").asBool();

      m_outputAssFile       = fnArnoldRenderOptions.findPlug("output_ass_filename").asString();
      m_outputAssMask       = fnArnoldRenderOptions.findPlug("output_ass_mask").asInt();

      m_log_filename          = fnArnoldRenderOptions.findPlug("log_filename").asString();
      m_log_max_warnings      = fnArnoldRenderOptions.findPlug("log_max_warnings").asInt();
      m_log_console_verbosity = fnArnoldRenderOptions.findPlug("log_console_verbosity").asInt();
      m_log_file_verbosity    = fnArnoldRenderOptions.findPlug("log_file_verbosity").asInt();
   }
   else
   {
      AiMsgError("[mtoa] Could not find defaultArnoldRenderOptions");
   }
}


void CRenderOptions::SetupLog() const
{
   if (m_log_filename != "")
      AiMsgSetLogFileName(m_log_filename.expandEnvironmentVariablesAndTilde().asChar());

   AiMsgSetMaxWarnings(m_log_max_warnings);
   AiMsgSetConsoleFlags(GetFlagsFromVerbosityLevel(m_log_console_verbosity) | AI_LOG_COLOR);
   AiMsgSetLogFileFlags(GetFlagsFromVerbosityLevel(m_log_file_verbosity));

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

