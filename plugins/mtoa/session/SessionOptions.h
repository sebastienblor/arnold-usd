#pragma once

#include "common/UnorderedContainer.h"
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MAnimControl.h>
#include <maya/MStringArray.h>
#include <maya/MString.h>
#include <maya/MVector.h>
#include <maya/MSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MFnDagNode.h>
#include <maya/MCommonRenderSettingsData.h>

#include <string>
#include <vector>
#include <algorithm>

// FIXME Arnold5  to be removed and replaced in the whole code once the Arnold-5.0 branch is merged
typedef uint8_t AtByte;
typedef unsigned AtUInt32;
typedef size_t AtUInt64;

/// Determines the Light-linking rules for this export
enum ArnoldLightLinkMode
{
   MTOA_LIGHTLINK_NONE,  ///< Ignore Light-linking
   MTOA_LIGHTLINK_MAYA ///<  Follow Maya's Light linking
};

///< Determines the Shadow Linking rules for this export
enum ArnoldShadowLinkMode
{
   MTOA_SHADOWLINK_NONE, ///< Ignore Shadow linking
   MTOA_SHADOWLINK_LIGHT, ///< Follow same rules as light-linking
   MTOA_SHADOWLINK_MAYA, ///< Follow Maya's shadow linking
};

enum ArnoldLogVerbosity
{
   MTOA_LOG_ERRORS,
   MTOA_LOG_WARNINGS,
   MTOA_LOG_INFO,
   MTOA_LOG_DEBUG
};

typedef unordered_set<MFn::Type, hash<int> > MFnTypeSet;

#define MTOA_MBLUR_DISABLE 0x0000
#define MTOA_MBLUR_LIGHT   0x0001
#define MTOA_MBLUR_CAMERA  0x0002
#define MTOA_MBLUR_OBJECT  0x0004
#define MTOA_MBLUR_DEFORM  0x0008
#define MTOA_MBLUR_SHADER  0x0010
#define MTOA_MBLUR_ANY     0xFFFF

#define MTOA_MBLUR_TYPE_START    0x0000
#define MTOA_MBLUR_TYPE_CENTER   0x0001
#define MTOA_MBLUR_TYPE_END      0x0002
#define MTOA_MBLUR_TYPE_CUSTOM   0x0003

#define MTOA_EXPORT_NAMESPACE_OFF       0x0000
#define MTOA_EXPORT_NAMESPACE_ON        0x0001
#define MTOA_EXPORT_NAMESPACE_ROOT      0x0002

#define MTOA_EXPORT_SEPARATOR_PIPES     0x0000
#define MTOA_EXPORT_SEPARATOR_SLASHES   0x0001

#define MTOA_EXPORT_DAG_SHAPE           0x0000
#define MTOA_EXPORT_DAG_TRANSFORM       0x0001


struct CMotionBlurOptions
{
   unsigned int   enable_mask;
   unsigned int   steps;
   unsigned int   range_type;
   double         motion_frames;
   double         motion_start;
   double         motion_end;

   CMotionBlurOptions() :  enable_mask(MTOA_MBLUR_DISABLE),
                           steps(1),
                           range_type(MTOA_MBLUR_TYPE_CENTER),
                           motion_frames(0.0),
                           motion_start(-0.25),
                           motion_end(0.25) {}
};

struct DLLEXPORT CSessionOptions
{
   friend class CMayaScene;
   friend class CArnoldSession;

   // Update the session options based on the current maya settings
   void Update();

   // --- Getters


   inline const ArnoldLightLinkMode& GetLightLinkMode() const {return m_lightlink;}
   inline const ArnoldShadowLinkMode& GetShadowLinkMode() const {return m_shadowlink;}
   inline const MDagPath& GetExportCamera() const { return m_camera; }
   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ANY) const { return (m_motion.enable_mask & type) != 0; }
   inline void DisableMotionBlur() {m_motion.enable_mask = MTOA_MBLUR_DISABLE;}
   inline unsigned int GetRangeType() const {return m_motion.range_type;}
   inline unsigned int GetNumMotionSteps() const { return m_motion.steps; }
   inline double GetExportFrame() const { return m_frame; }
   inline double GetMotionByFrame() const { return m_motion.motion_frames; }
   inline const std::vector<double> &GetMotionFrames() const { return m_motion_frames; }
   inline bool IsProgressive() const { return m_progressiveRendering; }
   inline double GetScaleFactor() const { return m_scaleFactor; }
   inline MVector GetOrigin() const { return m_origin; }
   const MStringArray &GetTextureSearchPaths() const {return m_textureSearchPaths;}
   const MStringArray &GetProceduralSearchPaths() const {return m_proceduralSearchPaths;}
   inline bool GetExportFullPath() const {return m_exportFullPath;}
   inline const MString &GetExportPrefix() const {return m_exportPrefix;}
   inline bool GetExportAllShadingGroups() const {return m_exportAllShadingGroups;}
   inline bool GetExportDagTransformNames() const {return m_exportDagTransformNames;}

   // if I don't inline this here, some contrib libs fail at linking
   inline void GetMotionRange(double &motion_start, double &motion_end) const {
      switch (m_motion.range_type)
      {
         case MTOA_MBLUR_TYPE_START:
            motion_start = 0.;
            motion_end = m_motion.motion_frames;
            break;
         case MTOA_MBLUR_TYPE_CENTER:
            motion_start = -0.5 * m_motion.motion_frames;
            motion_end = 0.5 * m_motion.motion_frames;
            break;
         case MTOA_MBLUR_TYPE_END:
            motion_start = -m_motion.motion_frames;
            motion_end = 0.;
            break;
         default:
         case MTOA_MBLUR_TYPE_CUSTOM:
            motion_start = m_motion.motion_start;
            motion_end = m_motion.motion_end;
            break;
      }
   }

   inline const MObject& GetArnoldRenderOptions() const { return m_options; }
   void FormatTexturePath(MString& texturePath) const
   {
      ReplaceSlashes(texturePath);
      if (!m_absoluteTexturePaths)
      {
         // if the filename starts with one of the texture search paths, remove the path from the filename
         for (unsigned int i = 0; i < m_textureSearchPaths.length(); ++i)
         {
            const MString& currentSearchPath = m_textureSearchPaths[i];
            if (texturePath.indexW(currentSearchPath) == 0)
            {
               texturePath = texturePath.substringW(currentSearchPath.length(), texturePath.length());
               break;
            }
         }
      }
      // convert to UTF-8
      texturePath = texturePath.asUTF8();
   }

   void FormatProceduralPath(MString& proceduralPath) const
   {
      ReplaceSlashes(proceduralPath);
      if (!m_absoluteProceduralPaths)
      {
         for (unsigned int i = 0; i < m_proceduralSearchPaths.length(); ++i)
         {
            const MString& currentSearchPath = m_proceduralSearchPaths[i];
            if (proceduralPath.indexW(currentSearchPath) == 0)
            {
               proceduralPath = proceduralPath.substringW(currentSearchPath.length(), proceduralPath.length());
               break;
            }
         }
      }
      // convert to UTF-8
      proceduralPath = proceduralPath.asUTF8();
   }
   
   unsigned int GetExportSeparator() const {return (m_exportSlashSeparator) ? MTOA_EXPORT_SEPARATOR_SLASHES : MTOA_EXPORT_SEPARATOR_PIPES;}
   void SetExportSlashSeparator(bool slashSeparator) {m_exportSlashSeparator = slashSeparator;}
   unsigned int GetExportNamespace() const {return (unsigned int) m_exportNamespace;}

   void SetupLog(AtRenderSession *renderSession) const;

   MMatrix& ScaleMatrix(MMatrix& matrix) const;
   AtMatrix& ScaleMatrix(AtMatrix& matrix) const;
   float& ScaleDistance(float& distance) const;
   double& ScaleDistance(double& distance) const;
   float& ScaleArea(float& area) const;
   float& ScaleLightExposure(float& exposure) const;

   bool clearBeforeRender() const
   {
      return m_clearBeforeRender;
   }

   bool useBinaryEncoding() const
   {
      return m_useBinaryEncoding;
   }
   
   bool sceneUpdateBeforeIPRRender() const
   {
      return m_forceSceneUpdateBeforeIPRRefresh;
   }
   
   bool GetUseExistingTx() const 
   { 
     return m_useExistingTx;
   }
   void SetUseExistingTx(bool b)
   {
      m_useExistingTx = b;
   }


   bool GetAutoTx() const
   {
     return m_autotx;
   }
   void SetAutoTx(bool b)
   {
      m_autotx = b;
   }

   MString outputAssFile() const
   {
      return m_outputAssFile;
   }

   unsigned int outputAssMask() const
   {
      return m_outputAssMask;
   }
   bool outputAssBoundingBox() const
   {
      return m_outputAssBoundingBox;
   }
   
   bool expandProcedurals() const
   {
      return m_expandProcedurals;
   }

   void SetOutputAssMask(unsigned int mask)
   {
      m_outputAssMask = mask;
   }

   void SetExpandProcedurals(bool expand_procedurals)
   {
      m_expandProcedurals = expand_procedurals;
   }

   void SetUseBinaryEncoding(bool ube)
   {
      m_useBinaryEncoding = ube;
   }

   void SetCamera(MDagPath& camera)
   {
      m_camera = camera;
   }

   void ClearCamera()
   {
      m_camera = MDagPath();
   }

   void SetRenderViewPanelName(const MString &panel)
   {
      m_panel = panel;
   }
   
   void SetRegion(const unsigned int left, const unsigned int right, const unsigned int bottom, const unsigned int top)
   {
      m_useRenderRegion = true;

      m_minx = left;
      m_miny = bottom;
      m_maxx = right;
      m_maxy = top;
   }
   void SetResolution(int width, int height)
   {
      if (width <= 0 || height <= 0)
      {
         m_overrideResolution = false;
         m_width = m_height = -1;
         return;
      }
      m_width = width;
      m_height = height;
      m_overrideResolution = true;
   }
   bool GetResolution(int &width, int &height) const
   {
      if (!m_overrideResolution)
         return false;
      width = m_width;
      height = m_height;
      return true;
   }
   bool GetRegion(int &minx, int &miny, int &maxx, int &maxy)
   {
      if (!m_useRenderRegion)
         return false;
      minx = m_minx;
      miny = m_miny;
      maxx = m_maxx;
      maxy = m_maxy;
      return true;
   }
   bool UseRenderRegion() const
   {
      return m_useRenderRegion;
   }
   void ClearRegion()
   {
      m_useRenderRegion = false;
   }

   MDagPath GetCamera() const
   {
      return m_camera;
   }

   MString GetCameraName() const
   {
      return MFnDagNode(m_camera).name();
   }

   MString GetRenderViewPanelName() const
   {
      return m_panel;
   }

   bool MultiCameraRender() const
   {
      return m_multiCameraRender;
   }

   int progressiveInitialLevel() const
   {
      return m_progressive_initial_level;
   }

   bool forceTranslateShadingEngines() const
   {
      return m_force_translate_shading_engines;
   }

   void SetForceTranslateShadingEngines(const bool force_translate_shading_engines)
   {
      m_force_translate_shading_engines = force_translate_shading_engines;
   }

   bool mtoaTranslationInfo() const
   {
      return m_mtoa_translation_info;
   }

   /// Get the Arnold name corresponding to a Maya object. For Dag nodes, prefer the function in CDagTranslator with a MDagPath argument
   MString GetArnoldNaming(const MObject &object) const;
   /// Returns the arnold name for a corresponding maya dag path
   MString GetArnoldNaming(const MDagPath &dagPath) const;

   inline void SetLightLinkMode(ArnoldLightLinkMode mode) { m_lightlink = mode; }
   inline void SetShadowLinkMode(ArnoldShadowLinkMode mode) { m_shadowlink = mode; }
   inline void SetArnoldRenderOptions(const MObject& options) { 
      m_options = options; 
      if (!m_options.isNull())
         Update();
   }

   inline void SetExportFrame(double frame) {
      if (frame != m_frame)
      {
         m_frame = frame;  
         UpdateMotionFrames();       
      }      
   }
   inline void SetProgressive(const bool is_progressive) { m_progressiveRendering = is_progressive; }

   inline void SetExportFullPath(bool b) {m_exportFullPath = b;}
   inline void SetExportPrefix(const MString &prefix) {m_exportPrefix = prefix;}
   inline void SetExportAllShadingGroups(bool b) {m_exportAllShadingGroups = b;}

   int GetLogConsoleVerbosity() const;
   int GetLogFileVerbosity() const;
   bool GetExportShadingEngine() const {return m_exportShadingEngine;}

   void SetSupportStereoCameras(bool b) {m_supportStereoCameras = b;}
   bool GetSupportStereoCameras() const {return m_supportStereoCameras;}

   void SetSupportGpu(bool b) {m_supportGpu = b;}
   bool GetSupportGpu() const {return m_supportGpu;}

   bool GetExportFileDrivers() const {return m_exportFileDrivers;}
   void SetExportFileDrivers(bool b) {m_exportFileDrivers = b;}

   bool GetExportCameraOverscan() const {return m_exportCameraOverscan;}
   void SetExportCameraOverscan(bool b) {m_exportCameraOverscan = b;}

private:

   CSessionOptions();
   // Setters : private as this is not meant to be modified
  
   void UpdateMotionFrames();

   static void ReplaceSlashes(MString& str, bool isDir = false)
   {
   #ifdef _WIN32
      MStringArray tmp;
      str.split('\\', tmp);
      if (tmp.length() > 1)
      {
         str = tmp[0];
         for (unsigned int i = 1; i < tmp.length(); ++i)
         {
            str += "/";
            str += tmp[i];
         }
      }
   #endif
      if (isDir && (str.length() > 1) && (str.substring(str.length() - 1, str.length() - 1) != "/"))
         str += "/";
   }

   
private:

   MObject              m_options;
   MDagPath             m_camera;
   MStringArray         m_textureSearchPaths;
   MStringArray         m_proceduralSearchPaths;
   MString              m_exportPrefix;

   CMotionBlurOptions   m_motion;
   std::vector<double>  m_motion_frames;

   bool                 m_exportSelection;
   MSelectionList       m_selection;
   
   MVector              m_origin;

   double               m_frame;
   double               m_scaleFactor;
   MMatrix              m_scaleFactorMMatrix;
   AtMatrix             m_scaleFactorAtMatrix;
   
   ArnoldLightLinkMode  m_lightlink;
   ArnoldShadowLinkMode m_shadowlink;

   bool                 m_progressiveRendering;
   bool                 m_absoluteTexturePaths;
   bool                 m_absoluteProceduralPaths;
   bool                 m_exportFullPath;
   bool                 m_exportAllShadingGroups;
   bool                 m_exportSlashSeparator;
   unsigned char        m_exportNamespace;
   bool                 m_exportDagTransformNames;
   
   // from old RenderOptions
   MString m_plugins_path;
   MString m_outputAssFile;
   MString m_log_filename;
   MString m_renderDriver;
   MString m_imageFileExtension;
   MString m_imageFilename;
   MString m_panel;
   MString m_plugin_searchpath;
   MString m_stats_file;
   MString m_profile_file;

   float m_startFrame;
   float m_endFrame;
   float m_byFrameStep;
   float m_AA_sample_clamp;
   float m_AA_sample_clamp_AOVs;

   AtUInt32 m_minx, m_miny, m_maxx, m_maxy;
   AtUInt32 m_width, m_height;
   bool m_overrideResolution;
   AtUInt32 m_extensionPadding;
   
   unsigned int m_log_max_warnings;
   unsigned int m_log_verbosity;
   unsigned int m_AA_samples;
   unsigned int m_GI_diffuse_samples;
   unsigned int m_GI_specular_samples;
   unsigned int m_outputAssMask;
   unsigned int m_progressive_initial_level;
   unsigned int m_threads;
   unsigned int m_stats_mode;

   bool m_useRenderRegion;
   bool m_clearBeforeRender; 
   bool m_forceSceneUpdateBeforeIPRRefresh;
   bool m_forceTextureCacheFlushAfterRender;
   bool m_useBinaryEncoding;
   bool m_log_to_file;
   bool m_log_to_console;
   bool m_expandProcedurals;
   bool m_force_translate_shading_engines;
   bool m_lock_sampling_noise;
   bool m_useExistingTx;
   bool m_autotx;
   bool m_outputAssBoundingBox;
   bool m_isAnimated;
   bool m_multiCameraRender;  
   bool m_mtoa_translation_info;
   bool m_exportShadingEngine;
   bool m_stats_enable;
   bool m_profile_enable;
   bool m_supportStereoCameras;
   bool m_supportGpu;
   bool m_exportFileDrivers;
   bool m_exportCameraOverscan;
};
