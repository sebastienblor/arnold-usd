#ifndef RENDER_OPTIONS_H
#define RENDER_OPTIONS_H

#include <ai_types.h>

#include <maya/MString.h>
#include <maya/MCommonRenderSettingsData.h> 

class CMayaScene;

class CRenderOptions
{
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
      return m_output_gamma;
   }

   MString filterType() const
   {
      return m_filter_type;
   }

   float filterWidth() const
   {
      return m_filter_width;
   }

   MString filterDomain() const
   {
      return m_filter_domain;
   }

   bool filterScalarMode() const
   {
      return m_filter_scalar_mode;
   }

   float filterMaximum() const
   {
      return m_filter_maximum;
   }

   float filterMinimum() const
   {
      return m_filter_minimum;
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

   void SetCameraName(MString name)
   {
      m_cameraName = name;
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

      SetupImageOptions();
   }

   void SetHeight(AtUInt height)
   {
      m_height = height;

      SetupImageOptions();
   }

   MString GetCameraName() const
   {
      return m_cameraName;
   }

   bool MultiCameraRender() const
   {
      return m_multiCameraRender;
   }

   bool BatchMode() const
   {
      return m_batchMode;
   }

   MString RenderDriver() const
   {
      return m_renderDriver;
   }

   MString arnoldRenderImageFormat() const
   {
      return m_arnoldRenderImageFormat;
   }

   AtUInt32 arnoldRenderImageCompression() const
   {
      return m_arnoldRenderImageCompression;
   }

   bool arnoldRenderImageHalfPrecision() const
   {
      return m_arnoldRenderImageHalfPrecision;
   }

   bool arnoldRenderImageOutputPadded() const
   {
      return m_arnoldRenderImageOutputPadded;
   }

   float arnoldRenderImageGamma() const
   {
      return m_arnoldRenderImageGamma;
   }

   AtUInt32 arnoldRenderImageQuality() const
   {
      return m_arnoldRenderImageQuality;
   }

   AtUInt32 arnoldRenderImageOutputFormat() const
   {
      return m_arnoldRenderImageOutputFormat;
   }

   bool arnoldRenderImageTiled() const
   {
      return m_arnoldRenderImageTiled;
   }

   bool arnoldRenderImageUnpremultAlpha() const
   {
      return m_arnoldRenderImageUnpremultAlpha;
   }

   bool isAnimated() const
   {
      return m_isAnimated;
   }

   void GetFromMaya(CMayaScene* scene);

   void SetupLog() const;
   void SetupRenderOptions() const;
   
   MString VerifyFileName(MString fileName, bool compressed);
   void    UpdateImageFilename();

private:

   void ProcessCommonRenderOptions();
   void ProcessArnoldRenderOptions();
   
   void SetupImageOptions() const;
   void SetupImageFilter() const;
   void SetupImageOutputs();

   AtInt GetFlagsFromVerbosityLevel(AtUInt level) const;

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
   MString  m_cameraName;
   MString  m_renderDriver;
   MString  m_imageFileExtension;
   MString  m_imageFilename;
   AtUInt32 m_extensionPadding;

   MString   m_arnoldRenderImageFormat;
   AtUInt32  m_arnoldRenderImageCompression;
   bool      m_arnoldRenderImageHalfPrecision;
   bool      m_arnoldRenderImageOutputPadded;
   float     m_arnoldRenderImageGamma;
   AtUInt32  m_arnoldRenderImageQuality;
   AtUInt32  m_arnoldRenderImageOutputFormat;
   bool      m_arnoldRenderImageTiled;
   bool      m_arnoldRenderImageUnpremultAlpha;
   bool      m_isAnimated;

   MCommonRenderSettingsData m_defaultRenderGlobalsData;
   
   AtUInt   m_threads;
   AtUInt   m_bucket_scanning;
   AtUInt   m_bucket_size;
   bool     m_abort_on_error;
   MString  m_plugins_path;

   AtUInt   m_AA_samples;
   AtUInt   m_GI_diffuse_samples;
   AtUInt   m_GI_glossy_samples;
   AtUInt   m_GI_sss_hemi_samples;
   float    m_AA_sample_clamp;

   MString  m_filter_type;
   float    m_filter_width;
   MString  m_filter_domain;
   bool     m_filter_scalar_mode;
   float    m_filter_maximum;
   float    m_filter_minimum;

   float    m_output_gamma;
   float    m_light_gamma;
   float    m_shader_gamma;
   float    m_texture_gamma;

   AtUInt   m_GI_diffuse_depth;
   AtUInt   m_GI_glossy_depth;
   AtUInt   m_GI_reflection_depth;
   AtUInt   m_GI_refraction_depth;
   AtUInt   m_GI_total_depth;

   bool     m_sss_lazy_evaluation;
   bool     m_sss_subpixel_cache;
   AtUInt   m_show_samples;

   AtUInt   m_max_subdivisions;

   AtUInt   m_background;
   AtUInt   m_atmosphere;

   MString m_outputAssFile;
   bool    m_outputAssCompressed;
   AtUInt  m_outputAssMask;

   MString m_log_filename;
   AtUInt  m_log_max_warnings;
   AtUInt  m_log_console_verbosity;
   AtUInt  m_log_file_verbosity;

   CMayaScene* m_scene;
};

#endif // RENDER_OPTIONS_H
