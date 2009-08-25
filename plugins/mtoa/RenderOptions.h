#ifndef RENDER_OPTIONS_H
#define RENDER_OPTIONS_H

#include <ai_types.h>

#include <maya/MString.h>

class CMayaScene;

class CRenderOptions
{
public:

   CRenderOptions();

   // GETTING OPTIONS FROM MAYA

   void GetRenderOptions(CMayaScene* scene);

   // MODIFYING OPTIONS

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

   float pixelAspectRatio() const
   {
      return m_pixelAspectRatio;
   }

   float outputGamma() const
   {
      return m_output_gamma;
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

   // SETTING OPTIONS IN ARNOLD

   void SetupLog() const;
   void SetupRender() const;
   
private:

   void ProcessCommonRenderOptions();
   void ProcessArnoldRenderOptions();
   
   void SetupImageOptions() const;

   AtInt GetFlagsFromVerbosityLevel(AtUInt level) const;

private:

   AtUInt32 m_minx, m_miny, m_maxx, m_maxy;
   AtUInt32 m_width, m_height;
   float    m_pixelAspectRatio;
   bool     m_useRenderRegion;
   bool     m_clearBeforeRender; 
   
   AtUInt   m_threads;
   AtUInt   m_bucket_scanning;
   AtUInt   m_bucket_size;
   bool     m_abort_on_error;
   MString  m_plugins_path;

   AtUInt    m_AA_samples;
   AtUInt    m_GI_hemi_samples;
   AtUInt    m_GI_specular_samples;
   float     m_AA_sample_clamp;

   float    m_output_gamma;
   float    m_TM_lgamma;
   float    m_TM_sgamma;
   float    m_TM_tgamma;

   AtUInt   m_GI_diffuse_depth;
   AtUInt   m_GI_glossy_depth;
   AtUInt   m_GI_reflection_depth;
   AtUInt   m_GI_refraction_depth;
   AtUInt   m_GI_total_depth;

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
