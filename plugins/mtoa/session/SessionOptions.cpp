#include "platform/Platform.h"
#include "utils/MayaUtils.h"
#include "utils/MtoaLog.h"
#include "SessionOptions.h"
#include "ArnoldSession.h"

#include <ai_msg.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MDistance.h>
#include <maya/MPlugArray.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MGlobal.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {
   enum ArnoldRenderUnit {
      RU_USE_MAYA_UNIT = 0,
      RU_USE_CUSTOM_SCALING,
      RU_INCH,
      RU_FEET,
      RU_YARD,
      RU_MILE,
      RU_MILLIMETER,
      RU_CENTIMETER,
      RU_KILOMETER,
      RU_METER
   };
}

static std::unordered_set<std::string> s_loadedPlugins;

void ExpandEnvVariables(MString& str)
{
   std::string str2 = str.asChar();
   size_t token;
   while ((token = str2.find("]")) != std::string::npos)
      str2.replace(token, token + 1, "}");
   while ((token = str2.find("[")) != std::string::npos)
      str2.replace(token, token + 1, "${");
   str = str2.c_str();
   str = str.expandEnvironmentVariablesAndTilde();
}
CSessionOptions::CSessionOptions(): m_options(MObject()),
                                    m_camera(MDagPath()),
                                    m_textureSearchPaths(),
                                    m_exportPrefix(""),
                                    m_motion(CMotionBlurOptions()),
                                    m_exportSelection(false),
                                    m_origin(0.0, 0.0, 0.0),
                                    m_frame(0.0),
                                    m_scaleFactor(1.0),
                                    m_lightlink(MTOA_LIGHTLINK_NONE),
                                    m_shadowlink(MTOA_SHADOWLINK_NONE),                        
                                    m_progressiveRendering(false),
                                    m_absoluteTexturePaths(true),
                                    m_absoluteProceduralPaths(true),
                                    m_exportFullPath(true),
                                    m_exportAllShadingGroups(false),
                                    m_exportSlashSeparator(true),
                                    m_exportNamespace(MTOA_EXPORT_NAMESPACE_ON),
                                    m_exportDagTransformNames(false),
                                    m_startFrame(1.f),
                                    m_endFrame(1.f),
                                    m_byFrameStep(1.f),
                                    m_AA_sample_clamp(0.f),
                                    m_AA_sample_clamp_AOVs(0.f),
                                    m_minx(0),
                                    m_miny(0),
                                    m_maxx(0),
                                    m_maxy(0),
                                    m_width(-1),
                                    m_height(-1),
                                    m_overrideResolution(false),
                                    m_extensionPadding(0),
                                    m_log_max_warnings(5),
                                    m_log_verbosity(0),
                                    m_AA_samples(1),
                                    m_GI_diffuse_samples(1),
                                    m_GI_specular_samples(1),
                                    m_outputAssMask(0),
                                    m_progressive_initial_level(0),
                                    m_threads(0),
                                    m_stats_mode(0),
                                    m_useRenderRegion(false),
                                    m_clearBeforeRender(false),
                                    m_forceSceneUpdateBeforeIPRRefresh(false),
                                    m_forceTextureCacheFlushAfterRender(false),
                                    m_useBinaryEncoding(false),
                                    m_log_to_file(false),
                                    m_log_to_console(false),
                                    m_expandProcedurals(false),
                                    m_force_translate_shading_engines(false),
                                    m_lock_sampling_noise(false),
                                    m_useExistingTx(false),
                                    m_autotx(false),
                                    m_outputAssBoundingBox(true),
                                    m_isAnimated(false),
                                    m_multiCameraRender(false),
                                    m_mtoa_translation_info(false),
                                    m_exportShadingEngine(false),
                                    m_stats_enable(false),
                                    m_profile_enable(false),
                                    m_supportStereoCameras(false),
                                    m_supportGpu(true),
                                    m_exportFileDrivers(true),
                                    m_exportCameraOverscan(false),
                                    m_exportResolutionOverscan(true)


{
   m_frame = MAnimControl::currentTime().as(MTime::uiUnit());
   // Initialize the options to the default one in the scene
   m_options = CArnoldSession::GetDefaultArnoldRenderOptions();
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


void CSessionOptions::Update()
{
   MStatus status;

   if (m_options == MObject::kNullObj)
   {
      m_options = CArnoldSession::GetDefaultArnoldRenderOptions();
   }
   if (m_options == MObject::kNullObj)
   {
      AiMsgError("[mtoa] No known Arnold render options");
      status = MStatus::kFailure;
   }
   MFnDependencyNode fnArnoldRenderOptions(m_options);

   // Light linking options
   MPlug plug;
   plug = fnArnoldRenderOptions.findPlug("llnk", true);
   if (!plug.isNull())
      m_lightlink = ArnoldLightLinkMode(plug.asInt());

   plug = fnArnoldRenderOptions.findPlug("slnk", true);
   if (!plug.isNull())
      m_shadowlink = ArnoldShadowLinkMode(plug.asInt());

   // Motion blur options
   // disable motion for material viewer and swatch rendering
   // FIXME : override this in material view & swatch sessions
   if (/*m_mode != MTOA_SESSION_MATERIALVIEW && m_mode != MTOA_SESSION_SWATCH && */fnArnoldRenderOptions.findPlug("mb_en", true).asBool())
   {
      m_motion.enable_mask   = (fnArnoldRenderOptions.findPlug("mb_en", true).asBool() * MTOA_MBLUR_LIGHT)
                             | (fnArnoldRenderOptions.findPlug("mb_cen", true).asBool() * MTOA_MBLUR_CAMERA)
                             | (fnArnoldRenderOptions.findPlug("mb_en", true).asBool() * MTOA_MBLUR_OBJECT)
                             | (fnArnoldRenderOptions.findPlug("mb_den", true).asBool() * MTOA_MBLUR_DEFORM)
                             | (fnArnoldRenderOptions.findPlug("mb_sen", true).asBool() * MTOA_MBLUR_SHADER);
   }
   else
      m_motion.enable_mask     = MTOA_MBLUR_DISABLE;

   if (m_motion.enable_mask)
   {
      m_motion.steps           = fnArnoldRenderOptions.findPlug("motion_steps", true).asInt();
      m_motion.range_type      = fnArnoldRenderOptions.findPlug("range_type", true).asInt();
      m_motion.motion_frames   = fnArnoldRenderOptions.findPlug("motion_frames", true).asFloat();
      m_motion.motion_start    = fnArnoldRenderOptions.findPlug("motion_start", true).asFloat();
      m_motion.motion_end      = fnArnoldRenderOptions.findPlug("motion_end", true).asFloat();
   }
   else
   {
      m_motion.steps           = 1;
      m_motion.range_type      = MTOA_MBLUR_TYPE_START;
      m_motion.motion_frames   = 0;
   }
   m_frame = MAnimControl::currentTime().as(MTime::uiUnit());
   UpdateMotionFrames();

   plug = fnArnoldRenderOptions.findPlug("absolute_texture_paths", true);
   if (!plug.isNull())
      m_absoluteTexturePaths = plug.asBool();
   else
      m_absoluteTexturePaths = true;

   plug = fnArnoldRenderOptions.findPlug("exportAllShadingGroups", true);
   if (!plug.isNull())
      m_exportAllShadingGroups = plug.asBool();
   else
      m_exportAllShadingGroups = false;

   plug = fnArnoldRenderOptions.findPlug("exportFullPaths", true);
   if (!plug.isNull())
      m_exportFullPath = plug.asBool();

   plug = fnArnoldRenderOptions.findPlug("exportSeparator", true);
   if (!plug.isNull())
      m_exportSlashSeparator = (plug.asInt() == MTOA_EXPORT_SEPARATOR_SLASHES);

   plug = fnArnoldRenderOptions.findPlug("exportNamespace", true);
   if (!plug.isNull())
      m_exportNamespace = plug.asInt();

   plug = fnArnoldRenderOptions.findPlug("exportDagName", true);
   if (!plug.isNull())
      m_exportDagTransformNames = (plug.asInt() == MTOA_EXPORT_DAG_TRANSFORM);

   plug = fnArnoldRenderOptions.findPlug("texture_searchpath", true);
   if (!plug.isNull())
   {
      plug.asString().split(PATHSEP, m_textureSearchPaths);
      MStringArray arr = getSourceImagesPath();
      for (unsigned int i = 0; i < arr.length(); ++i)
         m_textureSearchPaths.append(arr[i]);
      for (unsigned int i = 0; i < m_textureSearchPaths.length(); ++i)
      {
         ExpandEnvVariables(m_textureSearchPaths[i]);
         ReplaceSlashes(m_textureSearchPaths[i], true);            
      }
   }
   else
      m_textureSearchPaths.clear();

   plug = fnArnoldRenderOptions.findPlug("absolute_procedural_paths", true);
   if (!plug.isNull())
      m_absoluteProceduralPaths = plug.asBool();
   else
      m_absoluteProceduralPaths = true;

   plug = fnArnoldRenderOptions.findPlug("procedural_searchpath",true);
   if (!plug.isNull())
   {         
      plug.asString().split(PATHSEP, m_proceduralSearchPaths);
      m_proceduralSearchPaths.append(getProjectFolderPath());
      for (unsigned int i = 0; i < m_proceduralSearchPaths.length(); ++i)
      {
         ExpandEnvVariables(m_proceduralSearchPaths[i]);
         ReplaceSlashes(m_proceduralSearchPaths[i], true);            
      }
   }
   else
      m_proceduralSearchPaths.clear();

   const short renderUnit = fnArnoldRenderOptions.findPlug("renderUnit", true).asShort();

   MDistance dist(1.0, MDistance::uiUnit());

   switch (renderUnit)
   {
      case RU_USE_MAYA_UNIT:
         m_scaleFactor = 1.0;
         break;
      case RU_USE_CUSTOM_SCALING:
         m_scaleFactor = fnArnoldRenderOptions.findPlug("sceneScale", true).asDouble();
         break;
      case RU_INCH:
         m_scaleFactor = dist.asInches();
         break;
      case RU_FEET:
         m_scaleFactor = dist.asFeet();
         break;
      case RU_YARD:
         m_scaleFactor = dist.asYards();
         break;
      case RU_MILE:
         m_scaleFactor = dist.asMiles();
         break;
      case RU_MILLIMETER:
         m_scaleFactor = dist.asMillimeters();
         break;
      case RU_CENTIMETER:
         m_scaleFactor = dist.asCentimeters();
         break;
      case RU_KILOMETER:
         m_scaleFactor = dist.asKilometers();
         break;
      case RU_METER:
         m_scaleFactor = dist.asMeters();
         break;
      default:
         m_scaleFactor = 1.0;
   }


   AtVector s(static_cast<float>(m_scaleFactor), static_cast<float>(m_scaleFactor), static_cast<float>(m_scaleFactor));
   m_scaleFactorAtMatrix = AiM4Scaling(s);

   double sc[3] = {m_scaleFactor, m_scaleFactor, m_scaleFactor};
   m_scaleFactorMMatrix.setToIdentity();
   MTransformationMatrix trmat(m_scaleFactorMMatrix);
   trmat.setScale(sc, MSpace::kWorld);
   m_scaleFactorMMatrix = trmat.asMatrix();

   m_origin = MVector(0.0, 0.0, 0.0);

   if (fnArnoldRenderOptions.findPlug("offsetOrigin", true).asBool())
   {
      MPlugArray conns;
      if (fnArnoldRenderOptions.findPlug("origin", true).connectedTo(conns, true, false, &status))
      {
         if (status && (conns.length() > 0))
         {
            MDagPath dgPath;
            if (MDagPath::getAPathTo(conns[0].node(), dgPath))
            {
               MTransformationMatrix tMat = dgPath.inclusiveMatrix();
               m_origin = tMat.getTranslation(MSpace::kWorld);
            }
         }
      }
   }  
   status = MStatus::kSuccess;  

   // from old renderOptions
   m_progressiveRendering     = fnArnoldRenderOptions.findPlug("progressive_rendering", true).asBool();
   m_progressive_initial_level = fnArnoldRenderOptions.findPlug("progressive_initial_level", true).asInt();
   m_clearBeforeRender = fnArnoldRenderOptions.findPlug("clear_before_render", true).asBool();
   m_useBinaryEncoding = fnArnoldRenderOptions.findPlug("binaryAss", true).asBool();
   m_forceSceneUpdateBeforeIPRRefresh = fnArnoldRenderOptions.findPlug("force_scene_update_before_IPR_refresh", true).asBool();
   m_forceTextureCacheFlushAfterRender = fnArnoldRenderOptions.findPlug("force_texture_cache_flush_after_render", true).asBool();
   m_mtoa_translation_info = fnArnoldRenderOptions.findPlug("mtoa_translation_info", true).asBool();
   
   m_autotx = fnArnoldRenderOptions.findPlug("autotx", true).asBool();

   // if auto-tx is ON, we force use TX to be ON
   m_useExistingTx = (m_autotx) ? true : fnArnoldRenderOptions.findPlug("use_existing_tiled_textures", true).asBool();

   m_outputAssFile       = fnArnoldRenderOptions.findPlug("output_ass_filename", true).asString();
   m_outputAssMask       = fnArnoldRenderOptions.findPlug("output_ass_mask", true).asInt();
   m_exportShadingEngine = fnArnoldRenderOptions.findPlug("exportShadingEngine", true).asBool();

   m_force_translate_shading_engines = fnArnoldRenderOptions.findPlug("forceTranslateShadingEngines", true).asBool();
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
   if (m_plugin_searchpath.length() > 0)
   {
      std::string searchPath(m_plugin_searchpath.asChar());
      if (s_loadedPlugins.find(searchPath) == s_loadedPlugins.end())
      {
         //This plugin wasn't loaded yet, let's ask Arnold to load it
         AiLoadPlugins(searchPath.c_str());
         // now let's store this path so that we don't need to load it again
         s_loadedPlugins.insert(searchPath);
      }
   }
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

void CSessionOptions::UpdateMotionFrames()
{

   double exportFrame         = m_frame;
   if (m_motion.enable_mask)
   {
      unsigned int range_type    = m_motion.range_type;
      unsigned int motionSteps   = m_motion.steps;
      m_motion_frames.clear();
      m_motion_frames.reserve(motionSteps);
      
      double motionFrames;
      double stepSize;
      double startFrame;
      double endFrame;
      
      switch(range_type)
      {
         case MTOA_MBLUR_TYPE_START:
            motionFrames = m_motion.motion_frames;
            stepSize = motionFrames / double(motionSteps - 1);
            startFrame = exportFrame;
            endFrame = exportFrame + motionFrames;
            for (unsigned int step=0; (step < motionSteps - 1); ++step)
            {
               m_motion_frames.push_back(startFrame + (double)step * stepSize);
            }
            m_motion_frames.push_back(endFrame);
            break;
         case MTOA_MBLUR_TYPE_CENTER:
            motionFrames = m_motion.motion_frames;
            stepSize = motionFrames / double(motionSteps - 1);
            startFrame = exportFrame - motionFrames * 0.5;
            endFrame = exportFrame + motionFrames * 0.5;
            for (unsigned int step=0; (step < motionSteps - 1); ++step)
            {
               if((motionSteps%2 == 1) && (step == ((motionSteps-1)/2)) )
                  m_motion_frames.push_back(exportFrame);
               else
                  m_motion_frames.push_back(startFrame + (double)step * stepSize);
            }
            m_motion_frames.push_back(endFrame);
            break;
         case MTOA_MBLUR_TYPE_END:
            motionFrames = m_motion.motion_frames;
            stepSize = motionFrames / double(motionSteps - 1);
            startFrame = exportFrame - motionFrames;
            endFrame = exportFrame;
            m_motion_frames.push_back(startFrame);
            for (unsigned int step=1; (step < motionSteps - 1); ++step)
            {
               m_motion_frames.push_back(endFrame - (double)(motionSteps - 1 - step) * stepSize);
            }
            m_motion_frames.push_back(endFrame);
            break;
         case MTOA_MBLUR_TYPE_CUSTOM:
            startFrame = exportFrame + m_motion.motion_start;
            endFrame = exportFrame + m_motion.motion_end;
            motionFrames = endFrame - startFrame;
            stepSize = motionFrames / double(motionSteps - 1);
            for (unsigned int step=0; (step < motionSteps - 1); ++step)
            {
               m_motion_frames.push_back(startFrame + (double)step * stepSize);
            }
            m_motion_frames.push_back(endFrame);
            break;
      }
   }
   else
   {
      m_motion_frames.clear();
      m_motion_frames.push_back(exportFrame);
   }
}


MMatrix& CSessionOptions::ScaleMatrix(MMatrix& matrix) const
{
   matrix *= m_scaleFactorMMatrix;
   return matrix;
}

AtMatrix& CSessionOptions::ScaleMatrix(AtMatrix& matrix) const
{
   matrix = AiM4Mult(m_scaleFactorAtMatrix, matrix);
   return matrix;
}

float& CSessionOptions::ScaleDistance(float& distance) const
{
   double s = static_cast<double>(distance);
   s *= m_scaleFactor;
   distance = static_cast<float>(s);
   return distance;
}

double& CSessionOptions::ScaleDistance(double& distance) const
{
   distance *= m_scaleFactor;
   return distance;
}


float& CSessionOptions::ScaleArea(float& area) const
{
   double s = static_cast<double>(area);
   s *= m_scaleFactor;
   s *= m_scaleFactor;
   area = static_cast<float>(s);
   return area;
}

float& CSessionOptions::ScaleLightExposure(float& exposure) const
{
   double e = static_cast<double>(exposure);
   e += log(m_scaleFactor * m_scaleFactor) / log(2.0);
   exposure = static_cast<float>(e);
   return exposure;
}

static inline void OptionsAddNamingOptions(MString &name, const CSessionOptions &options)
{
   // What to do with the namespaces ? either keep them (ON), or strip them (OFF),
   // or keep them only once at the root of the hierarchy (ROOT) as if it was a parent
   unsigned int exportNamespace = options.GetExportNamespace();
   if (exportNamespace != MTOA_EXPORT_NAMESPACE_ON)
   {
      MString origStr;
      while(true)
      {
         // store the name at the beginning of each iteration, in order to ensure
         // it does change and we don't enter an infinite loop
         origStr = name; 
         
         int namespaceEnd = name.indexW(':');
         if (namespaceEnd < 0)
            break;

         const char *data = name.asChar();
         int namespaceBegin = namespaceEnd-1;
         for (; namespaceBegin >= 0; namespaceBegin--)
         {
            // Found the beginning of the namespace
            if (data[namespaceBegin] == '|')
               break;
         }
         MString namespaceStr = (namespaceBegin < 0) ? 
               MString("|") + name.substringW(0, namespaceEnd) :
               name.substringW(namespaceBegin, namespaceEnd);
         
         if (exportNamespace == MTOA_EXPORT_NAMESPACE_OFF)
         {
            if (namespaceBegin < 0)
               name = name.substringW(namespaceEnd + 1, name.length() -1);
         
         } else if (exportNamespace ==  MTOA_EXPORT_NAMESPACE_ROOT)
         {
            MString tmpStr = name.substringW(0, namespaceEnd - 1) + MString("|");
            name = tmpStr + name.substringW(namespaceEnd + 1, name.length() -1); // replace the ':' by a '|'
         }
         name.substitute(namespaceStr, MString("|")); 

         if (name == origStr)
         {
            // Mayday, we have a problem, the name hasn't changed at all and we're going 
            // to enter an infinite loop. Let's get out of here before it explodes.
            // store the name at the beginning of each iteration
            AiMsgError("[mtoa] Namespace couldn't be stripped from name %s", name.asChar());
            break;
         }
      }
   }

   // replace all pipes by slashes if the separator is set to "/"
   if (options.GetExportSeparator() == MTOA_EXPORT_SEPARATOR_SLASHES)
      name.substitute(MString("|"), MString ("/"));
   
   const MString &prefix = options.GetExportPrefix();
   if (prefix.length() > 0) {
      name = prefix + name;

      // Ensure we don't have double-slashes, that cause problems in some formats
      if (options.GetExportSeparator() == MTOA_EXPORT_SEPARATOR_SLASHES)
         name.substitute(MString("//"), MString ("/"));
   }
}


MString CSessionOptions::GetArnoldNaming(const MDagPath &dagPath) const
{
   MDagPath dag(dagPath);

   // Check if we want to export the parent transform name
   if (GetExportDagTransformNames())
      dag.pop();


   // Use either the "short" name or the "full path" name depending on the option
   MString name = GetExportFullPath() ? 
      dag.fullPathName() : dag.partialPathName();

   OptionsAddNamingOptions(name, *this);
   return name;
}

MString CSessionOptions::GetArnoldNaming(const MObject &object) const
{
   MString name = MFnDependencyNode(object).name();
   OptionsAddNamingOptions(name, *this);
   return name;
}

void CSessionOptions::SetupLog(AtUniverse *universe) const
{
   if ((m_log_filename != "") && (m_log_to_file))
   {
      // this replaces the MAYA_PROJECT_PATH with the actual project path
      // if there are no such environment variables are declared
      MString logPath = ExpandMtoaLogPath(m_log_filename);
      AiMsgSetLogFileName(logPath.expandEnvironmentVariablesAndTilde().asChar());
      AiMsgSetLogFileFlags(universe, m_log_verbosity);
      AiMsgResetCallback();
   } else if (m_log_to_console && !IsBatch())
   {
      // no "Log to File" enabled, and we're not doing batch rendering.
      // Therefore we can rely on MtoA's callback
      // that invokes Maya log functions.
      // The reason why we don't always enable it is that it should also 
      // handle the "log to file" and we prefer letting arnold do it.

      AiMsgResetCallback();
      AiMsgSetCallback(MtoaLogCallback);
   } else
      AiMsgResetCallback();
   
   AiMsgSetMaxWarnings(m_log_max_warnings);
   if (m_log_to_console)
      AiMsgSetConsoleFlags(universe, m_log_verbosity | AI_LOG_COLOR);   
   
   
}

int CSessionOptions::GetLogConsoleVerbosity() const
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
int CSessionOptions::GetLogFileVerbosity() const
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
