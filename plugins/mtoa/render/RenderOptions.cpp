#include "utils/MtoaLog.h"
#include "RenderOptions.h"
#include "scene/MayaScene.h"
#include "translators/options/OptionsTranslator.h"

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
,  m_output_gamma(1.0f)
,  m_outputAssMask(AI_NODE_ALL)
,  m_log_filename("")
,  m_log_max_warnings(100)
,  m_log_console_verbosity(5)
,  m_log_file_verbosity(5)
,  m_scene(NULL)
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
   MObject renderLayer = MFnRenderLayer::currentLayer();
   double  fileFrameNumber;

   // get the frame number
   MTime cT = MAnimControl::currentTime();
   fileFrameNumber = double(cT.value());
   MGlobal::executeCommand("basename((`file -q -sceneName -shortName`),(\".\" + (fileExtension((`file -q -sceneName -shortName`)))))", sceneFileName);


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
      m_imageFilename = m_defaultRenderGlobalsData.getImageName(m_defaultRenderGlobalsData.kFullPathImage, fileFrameNumber, sceneFileName, nameCamera, m_imageFileExtension, renderLayer, 1);
   }
   else
   {
      m_imageFilename = m_defaultRenderGlobalsData.getImageName(m_defaultRenderGlobalsData.kFullPathTmp, fileFrameNumber, sceneFileName, nameCamera, m_imageFileExtension, renderLayer, 1);
   }

   for (AtUInt i=0; i<NumAOVs(); ++i)
   {
      m_aovs[i].UpdateImageFilename(GetCameraName(), m_imageFileExtension, MultiCameraRender(), BatchMode());
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

      MRenderUtil::getCommonRenderSettings(m_defaultRenderGlobalsData);
      m_isAnimated = m_defaultRenderGlobalsData.isAnimated();
      m_extensionPadding = m_defaultRenderGlobalsData.framePadding;
      m_startFrame = static_cast<float>(m_defaultRenderGlobalsData.frameStart.as(MTime::uiUnit()));
      m_endFrame = static_cast<float>(m_defaultRenderGlobalsData.frameEnd.as(MTime::uiUnit()));
      m_byFrameStep = m_defaultRenderGlobalsData.frameBy;

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
   MObject        node;
   if (GetOptionsNode(node) == MS::kSuccess)
   {
      MPlugArray conns;
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

      m_progressive_rendering    = fnArnoldRenderOptions.findPlug("progressive_rendering").asBool();
      m_threads                  = fnArnoldRenderOptions.findPlug("threads_autodetect").asBool() ? 0 : fnArnoldRenderOptions.findPlug("threads").asInt();
      m_plugins_path             = fnArnoldRenderOptions.findPlug("plugins_path").asString();

      m_AA_samples               = fnArnoldRenderOptions.findPlug("AA_samples").asInt();
      m_GI_diffuse_samples       = fnArnoldRenderOptions.findPlug("GI_diffuse_samples").asInt();
      m_GI_glossy_samples        = fnArnoldRenderOptions.findPlug("GI_glossy_samples").asInt();
      m_GI_sss_hemi_samples      = fnArnoldRenderOptions.findPlug("GI_sss_hemi_samples").asInt();
      m_AA_sample_clamp          = fnArnoldRenderOptions.findPlug("use_sample_clamp").asBool() ? fnArnoldRenderOptions.findPlug("AA_sample_clamp").asFloat() : (float) AI_INFINITE;

      m_lock_sampling_noise      = fnArnoldRenderOptions.findPlug("lock_sampling_noise").asBool();

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

      m_outputAssFile       = fnArnoldRenderOptions.findPlug("output_ass_filename").asString();
      m_outputAssCompressed = fnArnoldRenderOptions.findPlug("output_ass_compressed").asBool();
      m_outputAssMask       = fnArnoldRenderOptions.findPlug("output_ass_mask").asInt();

      m_log_filename          = fnArnoldRenderOptions.findPlug("log_filename").asString();
      m_log_max_warnings      = fnArnoldRenderOptions.findPlug("log_max_warnings").asInt();
      m_log_console_verbosity = fnArnoldRenderOptions.findPlug("log_console_verbosity").asInt();
      m_log_file_verbosity    = fnArnoldRenderOptions.findPlug("log_file_verbosity").asInt();

      // AOVs
      ClearAOVs();
      MPlug pAOV = fnArnoldRenderOptions.findPlug("aovs");
      pAOV.connectedTo(conns, true, false);
      if (conns.length() == 1)
      {
         MObject oAOV = conns[0].node();
         MFnDependencyNode nAOV(oAOV);

         MPlug pBMO = nAOV.findPlug("aov_batch_mode_only");
         if (BatchMode() || (!pBMO.isNull() && !pBMO.asBool()))
         {
            MPlug pAOVs = nAOV.findPlug("aovs");
            for (unsigned int i=0; i<pAOVs.numElements(); ++i)
            {
               CAOV aov;
               MPlug ipAOVs = pAOVs[i];
               if (aov.FromMaya(ipAOVs))
               {
                  AddAOV(aov);
               }
            }
         }
      }
   }
   else
   {
      AiMsgError("[mtoa] Could not find defaultArnoldRenderOptions");
   }
   SetupImageOutputs();
}

void CRenderOptions::SetupLog() const
{
   if (m_log_filename != "")
      AiMsgSetLogFileName(m_log_filename.expandEnvironmentVariablesAndTilde().asChar());

   AiMsgSetMaxWarnings(m_log_max_warnings);
   AiMsgSetConsoleFlags(GetFlagsFromVerbosityLevel(m_log_console_verbosity) | AI_LOG_COLOR);
   AiMsgSetLogFileFlags(GetFlagsFromVerbosityLevel(m_log_file_verbosity));

   // Callback for script editor echo has to be disabled, because not way to know
   // the log filename and write to it from callback
   // AiMsgSetCallback(MtoaLogCallback);
   AiMsgResetCallback();
}

void CRenderOptions::SetupImageOptions() const
{
   MObject        node;
   if (GetOptionsNode(node) == MS::kSuccess)
   {
      COptionsTranslator* translator = (COptionsTranslator*)m_scene->GetActiveTranslator(node);
      translator->SetupImageOptions(AiUniverseGetOptions());
   }
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
MStatus CRenderOptions::GetOptionsNode(MObject& optionsNode) const
{
   MSelectionList list;
   list.add("defaultArnoldRenderOptions");

   if (list.length() > 0)
   {
      list.getDependNode(0, optionsNode);
      return MS::kSuccess;
   }
   return MS::kFailure;
}
