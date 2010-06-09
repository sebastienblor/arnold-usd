
#include "RenderOptions.h"
#include "maya_scene/MayaScene.h"

#include <ai_constants.h>
#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_universe.h>

#include <maya/MFnDependencyNode.h>
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

#include <direct.h>

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
,  m_multiCameraRender(false)
{}

void CRenderOptions::GetFromMaya(CMayaScene* scene)
{
   m_scene = scene;

   ProcessCommonRenderOptions();
   ProcessArnoldRenderOptions();
}

void CRenderOptions::UpdateImageFilename() 
{

   MString sceneFileName, nameCamera;
   MString cameraFolderName;
   MObject renderLayer = MFnRenderLayer::defaultRenderLayer();  	
   double  fileFrameNumber;

   // get the frame number
   MTime cT = MAnimControl::currentTime();
   fileFrameNumber = double(cT.value());
   MGlobal::executeCommand("basename( (`file -q -sceneName -shortName`),(\".\" + (fileExtension((`file -q -sceneName -shortName`)))))", sceneFileName);


   if (BatchMode())
   {

      // get camera transform node for folder name
      MSelectionList list;
      MObject        node,parentNode;
      list.add(GetCameraName());
      list.getDependNode(0, node);
      MFnDagNode camDag(node);
      parentNode = camDag.parent(0);
      MFnDagNode camDagParent(parentNode);
      nameCamera = camDagParent.name();

      if (MultiCameraRender())
      {
         nameCamera = sceneFileName + "/" + nameCamera;
      }
      else
      {
         sceneFileName = sceneFileName + "/" + sceneFileName;
      }
      m_imageFilename = m_defaultRenderGlobalsData.getImageName(m_defaultRenderGlobalsData.kFullPathImage, fileFrameNumber, sceneFileName, nameCamera, m_imageFileExtension, renderLayer, 1);
   }
   else
   {
      m_imageFilename = m_defaultRenderGlobalsData.getImageName(m_defaultRenderGlobalsData.kFullPathTmp, fileFrameNumber, sceneFileName, nameCamera, m_imageFileExtension, renderLayer, 1);
   }

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
      m_extensionPadding = fnRenderGlobals.findPlug("extensionPadding").asInt();

      fnRenderGlobals.findPlug("startFrame").getValue(time);
      m_startFrame = time.as(MTime::uiUnit());
      fnRenderGlobals.findPlug("endFrame").getValue(time);
      m_endFrame = time.as(MTime::uiUnit());
      m_byFrameStep = fnRenderGlobals.findPlug("byFrameStep").asFloat();

      MRenderUtil::getCommonRenderSettings(m_defaultRenderGlobalsData);
      m_isAnimated = m_defaultRenderGlobalsData.isAnimated();

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

      MFnEnumAttribute arnold_render_format(fnArnoldRenderOptions.findPlug("arnoldRenderImageFormat").attribute());
      m_arnoldRenderImageFormat         = arnold_render_format.fieldName(fnArnoldRenderOptions.findPlug("arnoldRenderImageFormat").asShort());
      m_arnoldRenderImageCompression    = fnArnoldRenderOptions.findPlug("compression").asInt();
      m_arnoldRenderImageHalfPrecision  = fnArnoldRenderOptions.findPlug("half_precision").asBool();
      m_arnoldRenderImageOutputPadded   = fnArnoldRenderOptions.findPlug("output_padded").asBool();
      m_arnoldRenderImageGamma          = fnArnoldRenderOptions.findPlug("gamma").asFloat();
      m_arnoldRenderImageQuality        = fnArnoldRenderOptions.findPlug("quality").asInt();
      m_arnoldRenderImageOutputFormat   = fnArnoldRenderOptions.findPlug("format").asInt();
      m_arnoldRenderImageTiled          = fnArnoldRenderOptions.findPlug("tiled").asBool();
      m_arnoldRenderImageUnpremultAlpha = fnArnoldRenderOptions.findPlug("unpremult_alpha").asBool();

      m_progressive_rendering           = fnArnoldRenderOptions.findPlug("progressive_rendering").asBool();

      m_threads         = fnArnoldRenderOptions.findPlug("threads_autodetect").asBool() ? 0 : fnArnoldRenderOptions.findPlug("threads").asInt();
      m_bucket_scanning = fnArnoldRenderOptions.findPlug("bucket_scanning").asInt();
      m_bucket_size     = fnArnoldRenderOptions.findPlug("bucket_size").asInt();
      m_abort_on_error  = fnArnoldRenderOptions.findPlug("abort_on_error").asBool();
      m_plugins_path    = fnArnoldRenderOptions.findPlug("plugins_path").asString();

      m_AA_samples          = fnArnoldRenderOptions.findPlug("AA_samples").asInt();
      m_GI_diffuse_samples  = fnArnoldRenderOptions.findPlug("GI_diffuse_samples").asInt();
      m_GI_glossy_samples   = fnArnoldRenderOptions.findPlug("GI_glossy_samples").asInt();
      m_GI_sss_hemi_samples = fnArnoldRenderOptions.findPlug("GI_sss_hemi_samples").asInt();
      m_AA_sample_clamp     = fnArnoldRenderOptions.findPlug("use_sample_clamp").asBool() ? fnArnoldRenderOptions.findPlug("AA_sample_clamp").asFloat() : (float) AI_INFINITE;

      MFnEnumAttribute enum_filter_type(fnArnoldRenderOptions.findPlug("filter_type").attribute());
      m_filter_type  = enum_filter_type.fieldName(fnArnoldRenderOptions.findPlug("filter_type").asShort());
      m_filter_width = fnArnoldRenderOptions.findPlug("filter_width").asFloat();

      MFnEnumAttribute enum_filter_domain(fnArnoldRenderOptions.findPlug("filter_domain").attribute());
      m_filter_domain      = enum_filter_domain.fieldName(fnArnoldRenderOptions.findPlug("filter_domain").asShort());
      m_filter_scalar_mode = fnArnoldRenderOptions.findPlug("filter_scalar_mode").asBool();
      m_filter_maximum     = fnArnoldRenderOptions.findPlug("filter_maximum").asFloat();
      m_filter_minimum     = fnArnoldRenderOptions.findPlug("filter_minimum").asFloat();

      m_output_gamma  = fnArnoldRenderOptions.findPlug("driver_gamma").asFloat();
      m_light_gamma   = fnArnoldRenderOptions.findPlug("light_gamma").asFloat();
      m_shader_gamma  = fnArnoldRenderOptions.findPlug("shader_gamma").asFloat();
      m_texture_gamma = fnArnoldRenderOptions.findPlug("texture_gamma").asFloat();

      m_clearBeforeRender = fnArnoldRenderOptions.findPlug("clear_before_render").asBool();

      m_GI_diffuse_depth    = fnArnoldRenderOptions.findPlug("GI_diffuse_depth").asInt();
      m_GI_glossy_depth     = fnArnoldRenderOptions.findPlug("GI_glossy_depth").asInt();
      m_GI_reflection_depth = fnArnoldRenderOptions.findPlug("GI_reflection_depth").asInt();
      m_GI_refraction_depth = fnArnoldRenderOptions.findPlug("GI_refraction_depth").asInt();
      m_GI_total_depth      = fnArnoldRenderOptions.findPlug("GI_total_depth").asInt();

      m_auto_transparency_depth = fnArnoldRenderOptions.findPlug("auto_transparency_depth").asInt();
      m_auto_transparency_threshold = fnArnoldRenderOptions.findPlug("auto_transparency_threshold").asFloat();

      m_sss_lazy_evaluation = fnArnoldRenderOptions.findPlug("sss_lazy_evaluation").asBool();
      m_sss_subpixel_cache  = fnArnoldRenderOptions.findPlug("sss_subpixel_cache").asBool();
      m_show_samples        = fnArnoldRenderOptions.findPlug("show_samples").asInt();

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

   SetupImageOutputs();
}

void CRenderOptions::SetupRenderOptions() const
{
   SetupImageOptions();

   AiNodeSetInt(AiUniverseGetOptions(), "threads", m_threads);
   AiNodeSetInt(AiUniverseGetOptions(), "bucket_scanning", m_bucket_scanning);
   AiNodeSetInt(AiUniverseGetOptions(), "bucket_size", m_bucket_size);
   AiNodeSetBool(AiUniverseGetOptions(), "abort_on_error", m_abort_on_error);

   AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", m_AA_samples);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_diffuse_samples", m_GI_diffuse_samples);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_glossy_samples", m_GI_glossy_samples);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_sss_hemi_samples", m_GI_sss_hemi_samples);
   AiNodeSetFlt(AiUniverseGetOptions(), "AA_sample_clamp", m_AA_sample_clamp);

   AiNodeSetFlt(AiUniverseGetOptions(), "light_gamma", m_light_gamma);
   AiNodeSetFlt(AiUniverseGetOptions(), "shader_gamma", m_shader_gamma);
   AiNodeSetFlt(AiUniverseGetOptions(), "texture_gamma", m_texture_gamma);

   AiNodeSetInt(AiUniverseGetOptions(), "GI_diffuse_depth", m_GI_diffuse_depth);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_glossy_depth", m_GI_glossy_depth);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_reflection_depth", m_GI_reflection_depth);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_refraction_depth", m_GI_refraction_depth);
   AiNodeSetInt(AiUniverseGetOptions(), "GI_total_depth", m_GI_total_depth);

   AiNodeSetInt(AiUniverseGetOptions(), "auto_transparency_depth", m_auto_transparency_depth);
   AiNodeSetFlt(AiUniverseGetOptions(), "auto_transparency_threshold", m_auto_transparency_threshold);

   AiNodeSetBool(AiUniverseGetOptions(), "sss_lazy_evaluation", m_sss_lazy_evaluation);
   AiNodeSetBool(AiUniverseGetOptions(), "sss_subpixel_cache", m_sss_subpixel_cache);
   AiNodeSetInt(AiUniverseGetOptions(), "show_samples", m_show_samples);

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
      AiMsgSetLogFileName(m_log_filename.expandEnvironmentVariablesAndTilde().asChar());

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

MString CRenderOptions::VerifyFileName(MString fileName, bool compressed)
{
   unsigned int len = fileName.length();

   if (!compressed)
   {
      if ((len < 4) || (fileName.substring(len - 4, len - 1).toLowerCase() != ".ass"))
         fileName += ".ass";
   }
   else
   {
      if ((len < 7) || (fileName.substring(len - 7, len - 1).toLowerCase() != ".ass.gz"))
      {
         if ((len < 4) || (fileName.substring(len - 4, len - 1).toLowerCase() == ".ass"))
            fileName += ".gz";
         else if ((len < 3) || (fileName.substring(len - 3, len - 1).toLowerCase() == ".gz"))
            fileName = fileName.substring(0, len - 4) + ".ass.gz";
         else
            fileName += ".ass.gz";
      }
   }

   return fileName;
}

void CRenderOptions::SetupImageOutputs()
{
   MString imageRenderFormat = arnoldRenderImageFormat();

   if (imageRenderFormat == "OpenEXR")
   {
      m_renderDriver       = "driver_exr";
      m_imageFileExtension = "exr";
   }
   if (imageRenderFormat == "Tiff")
   {
      m_renderDriver       = "driver_tiff";
      m_imageFileExtension = "tif";
   }
   if (imageRenderFormat == "Jpg")
   {
      m_renderDriver       = "driver_jpeg";
      m_imageFileExtension = "jpg";
   }
   if (imageRenderFormat == "Png")
   {
      m_renderDriver       = "driver_png";
      m_imageFileExtension = "png";
   }
}

