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
,  m_display_gamma(1.0f)
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

// sets the m_imageFilename member
void CRenderOptions::UpdateImageFilename() 
{

   MString nameCamera;
   MString cameraFolderName;
   MObject renderLayer = MFnRenderLayer::currentLayer();
   double fileFrameNumber = MAnimControl::currentTime().value();

   // file name
   MFileObject fileObj;
   fileObj.setRawFullName(MFileIO::currentFile());
   MString sceneFileName = fileObj.resolvedName();
   sceneFileName = sceneFileName.substringW(0, sceneFileName.rindexW('.')-1);
   // get camera transform node for folder name
   MFnDagNode camDag(GetCamera());
   MFnDagNode camDagParent(camDag.parent(0));
   nameCamera = camDagParent.name();


   // Notes on MCommonRenderSettingsData::getImageName:
   //   - sceneFileName is only used if defaultRenderGlobals.imageFilePrefix is not set
   //   - a "<RenderPass>/" token is added before the file name if any pass nodes are
   //     connected to a render layer AND <RenderPass> does not appear in defaultRenderGlobals.imageFilePrefix
   // because getImageName ignores the sceneFileName arg when defaultRenderGlobals.imageFilePrefix is non-empty,
   // we can only achieve the proper addition of the <RenderPass> token by creating a dummy render pass node.
   // TODO: write a complete replacement for MCommonRenderSettingsData::getImageName
   MCommonRenderSettingsData::MpathType pathType;
   if (BatchMode())
   {
      // TODO: check that this is appropriate behavior (seems questionable)
      if (MultiCameraRender())
         nameCamera = sceneFileName + "/" + nameCamera;

      pathType = m_defaultRenderGlobalsData.kFullPathImage;
      m_imageFilename = m_defaultRenderGlobalsData.getImageName(pathType, fileFrameNumber,
                                                                sceneFileName, nameCamera,
                                                                m_imageFileExtension, renderLayer, 1);
      MFnDagNode camDag(GetCamera());
      MFnDagNode camDagParent(camDag.parent(0));
   }
   else
   {
      pathType = m_defaultRenderGlobalsData.kFullPathTmp;
      m_imageFilename = m_defaultRenderGlobalsData.getImageName(pathType, fileFrameNumber,
                                                                sceneFileName, nameCamera,
                                                                m_imageFileExtension, renderLayer, 1);
   }

   if (m_defaultRenderGlobalsData.name == "")
   {
      // setup the default RenderPass token
      sceneFileName = "<RenderPass>/" + sceneFileName;
      // FIXME: hard-wiring convention here:
      // need a a complete replacement for MCommonRenderSettingsData::getImageName to avoid this
      // (see mtoa.utils.expandFileTokens for the beginning of one)
      sceneFileName += ".<RenderPass>";
   }

   for (AtUInt i=0; i<NumAOVs(); ++i)
   {
      MString tokens = MString("RenderPass=") + m_aovs[i].GetName();
      m_aovs[i].SetImageFilename(m_defaultRenderGlobalsData.getImageName(pathType, fileFrameNumber,
                                                                         sceneFileName, nameCamera,
                                                                         m_imageFileExtension, renderLayer,
                                                                         tokens, 1));

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

      m_display_gamma  = fnArnoldRenderOptions.findPlug("display_gamma").asFloat();
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
         MFnDependencyNode fnAOVNode(oAOV);

         if (BatchMode() || !fnAOVNode.findPlug("aov_batch_mode_only").asBool())
         {
            MPlug pAOVs = fnAOVNode.findPlug("aovs");
            for (unsigned int i=0; i<pAOVs.numElements(); ++i)
            {
               CAOV aov;
               MPlug ipAOVs = pAOVs[i];
               if (aov.FromMaya(ipAOVs))
                  AddAOV(aov);
               else
                  MGlobal::displayWarning("[mtoa] Could not setup AOV attribute " + ipAOVs.partialName(true, false, false, false, false, true));
            }
         }
      }
   }
   else
   {
      AiMsgError("[mtoa] Could not find defaultArnoldRenderOptions");
   }
}

AtNode * CRenderOptions::CreateFileOutput()
{
   // Don't install the file driver when in IPR mode.
   if (m_scene->GetExportMode() == MTOA_EXPORT_IPR) return NULL;

   MObject node;
   GetOptionsNode(node);
   MFnDependencyNode fnArnoldRenderOptions(node);

   // set the output driver
   MString driverType = fnArnoldRenderOptions.findPlug("imageFormat").asString();
   AtNode* driver = m_scene->ExportDriver(node, driverType);
   if (driver != NULL)
   {
      AiNodeSetStr(driver, "filename", GetImageFilename().asChar());
  }
   else
      AiMsgError("[mtoa] image driver is NULL");

   return driver;
}

AtNode * CRenderOptions::CreateOutputFilter()
{
   MObject node;
   GetOptionsNode(node);
   MFnDependencyNode fnArnoldRenderOptions(node);

   // set the output driver
   MString filterType = fnArnoldRenderOptions.findPlug("filterType").asString();
   AiMsgInfo("exporting filter \"%s\"", filterType.asChar());
   AtNode* filter = m_scene->ExportFilter(node, filterType);
   if (filter == NULL)
      AiMsgError("[mtoa] filter is NULL");

   return filter;
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
