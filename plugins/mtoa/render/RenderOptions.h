#ifndef RENDEROPTIONS_H
#define RENDEROPTIONS_H

#include "platform/Platform.h"
#include "AOV.h"

#include <ai_types.h>

#include <maya/MString.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>

#include <vector>
#include <set>
#include <map>
#include <string>

class CMayaScene;

enum RenderType
{
   MTOA_RENDER_INTERACTIVE,
   MTOA_RENDER_EXPORTASS,
   MTOA_RENDER_EXPORTASS_AND_KICK
};

class DLLEXPORT CRenderOptions
{
   friend class CRenderSession;

public:

   CRenderOptions();

   AtUInt32 minX() const
   {
      return m_minx;
   }

   AtUInt32 minY() const
   {
      return m_miny;
   }

   AtUInt32 maxX() const
   {
      return m_maxx;
   }

   AtUInt32 maxY() const
   {
      return m_maxy;
   }

   AtUInt32 width() const
   {
      return m_width;
   }

   AtUInt32 height() const
   {
      return m_height;
   }

   AtFloat startFrame() const
   {
      return m_startFrame;
   }

   AtFloat endFrame() const
   {
      return m_endFrame;
   }

   AtFloat byFrameStep() const
   {
      return m_byFrameStep;
   }

   float pixelAspectRatio() const
   {
      return m_pixelAspectRatio;
   }

   float outputGamma() const
   {
      return m_display_gamma;
   }

   MString filterType() const
   {
      return m_filter_type;
   }

   bool useRenderRegion() const
   {
      return m_useRenderRegion;
   }

   bool clearBeforeRender() const
   {
      return m_clearBeforeRender;
   }

   MString outputAssFile() const
   {
      return m_outputAssFile;
   }

   bool outputAssCompressed() const
   {
      return m_outputAssCompressed;
   }

   AtUInt outputAssMask() const
   {
      return m_outputAssMask;
   }

   MString pluginsPath() const
   {
      return m_plugins_path;
   }

   MString GetImageFilename() const
   {
      return m_imageFilename;
   }

   void SetCamera(MDagPath& camera)
   {
      m_camera = camera;
   }

   void SetMultiCameraRender(bool multicam)
   {
      m_multiCameraRender = multicam;
   }

   void SetBatch(bool batch)
   {
      m_batchMode = batch;
   }

   void SetWidth(AtUInt width)
   {
      m_width = width;

      UpdateImageOptions();
   }

   void SetHeight(AtUInt height)
   {
      m_height = height;

      UpdateImageOptions();
   }

   void SetRegion(const AtUInt left, const AtUInt right, const AtUInt bottom, const AtUInt top)
   {
      m_useRenderRegion = true;

      m_minx = left;
      m_miny = bottom;
      m_maxx = right;
      m_maxy = top;

      UpdateImageOptions();
   }

   MDagPath GetCamera() const
   {
      return m_camera;
   }

   MString GetCameraName() const
   {
      return MFnDagNode(m_camera).name();
   }

   bool MultiCameraRender() const
   {
      return m_multiCameraRender;
   }

   bool BatchMode() const
   {
      return m_batchMode;
   }

   bool isAnimated() const
   {
      return m_isAnimated;
   }

   bool isProgressive() const
   {
      return m_progressive_rendering;
   }

   void SetProgressive(const bool is_progressive)
   {
      m_progressive_rendering = is_progressive;
   }

   AtUInt NumAASamples() const
   {
      return m_AA_samples;
   }

   AtUInt NumAOVs() const
   {
      return static_cast<AtUInt>(m_aovs.size());
   }

   void AddAOV(const CAOV &aov)
   {
      m_aovs.insert(aov);
   }

   bool IsActiveAOV(CAOV &aov) const
   {
      if (m_aovs.count(aov))
         return true;
      else
         return false;
   }

   AOVSet GetAOVs() const
   {
      return m_aovs;
   }

   void ClearAOVs()
   {
      m_aovs.clear();
   }

   void GetFromMaya();

   void SetupLog() const;
   
   MString VerifyFileName(MString fileName, bool compressed);
   MString GetFileExtension(const MString& imageRenderFormat) const;
   void UpdateImageFilename();
   void UpdateImageOptions();

private:

   void ProcessCommonRenderOptions();
   void ProcessArnoldRenderOptions();
   
   void SetupImageOptions() const;
   void SetupImageFilter() const;


   MStatus GetOptionsNode(MObject& optionsNode) const;

private:

   AtUInt32 m_minx, m_miny, m_maxx, m_maxy;
   AtUInt32 m_width, m_height;
   float    m_pixelAspectRatio;
   bool     m_useRenderRegion;
   bool     m_clearBeforeRender; 

   AtFloat  m_startFrame;
   AtFloat  m_endFrame;
   AtFloat  m_byFrameStep;
   bool     m_batchMode;
   bool     m_multiCameraRender;
   MDagPath m_camera;
   MString  m_renderDriver;
   MString  m_imageFileExtension;
   MString  m_imageFilename;
   AtUInt32 m_extensionPadding;

   MString  m_arnoldRenderImageFormat;
   bool     m_isAnimated;

   MCommonRenderSettingsData m_defaultRenderGlobalsData;

   bool     m_progressive_rendering;
   AtUInt   m_threads;
   MString  m_plugins_path;

   AtUInt   m_AA_samples;
   AtUInt   m_GI_diffuse_samples;
   AtUInt   m_GI_glossy_samples;
   AtUInt   m_GI_sss_hemi_samples;
   float    m_AA_sample_clamp;
   bool     m_lock_sampling_noise;

   MString  m_filter_type;

   float    m_display_gamma;
   float    m_light_gamma;
   float    m_shader_gamma;
   float    m_texture_gamma;

   MString  m_outputAssFile;
   bool     m_outputAssCompressed;

   bool     m_outputAssBoundingBox;
   AtUInt   m_outputAssMask;

   MString  m_log_filename;
   AtUInt   m_log_max_warnings;
   AtUInt   m_log_console_verbosity;
   AtUInt   m_log_file_verbosity;

   AOVSet   m_aovs;

};

#endif // RENDEROPTIONS_H
