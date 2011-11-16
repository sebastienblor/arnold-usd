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

   float pixelAspectRatio() const
   {
      return m_pixelAspectRatio;
   }

   bool useRenderRegion() const
   {
      return m_useRenderRegion;
   }

   bool clearBeforeRender() const
   {
      return m_clearBeforeRender;
   }
   
   bool useExistingTiledTextures() const 
   { 
     return m_use_existing_tiled_textures; 
   }

   MString outputAssFile() const
   {
      return m_outputAssFile;
   }

   AtUInt outputAssMask() const
   {
      return m_outputAssMask;
   }

   void SetCamera(MDagPath& camera);

   void SetWidth(AtUInt width)
   {
      m_width = width;

      UpdateImageDimensions();
   }

   void SetHeight(AtUInt height)
   {
      m_height = height;

      UpdateImageDimensions();
   }

   void SetRegion(const AtUInt left, const AtUInt right, const AtUInt bottom, const AtUInt top)
   {
      m_useRenderRegion = true;

      m_minx = left;
      m_miny = bottom;
      m_maxx = right;
      m_maxy = top;

      UpdateImageDimensions();
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

   bool isProgressive() const
   {
      return m_progressive_rendering;
   }

   int progressiveInitialLevel() const
   {
      return m_progressive_initial_level;
   }

   void SetProgressive(const bool is_progressive)
   {
      m_progressive_rendering = is_progressive;
   }

   MStatus GetFromMaya();

   void SetupLog() const;
   
   MString GetFileExtension(const MString& imageRenderFormat) const;
   void UpdateImageFilename();
   void UpdateImageOptions();
   void UpdateImageDimensions();
   inline const MObject& GetArnoldRenderOptions() const { return m_options; }
   inline void SetArnoldRenderOptions(const MObject& options) { m_options = options; }

private:

   MStatus ProcessCommonRenderOptions();
   MStatus ProcessArnoldRenderOptions();

   void SetupImageOptions() const;
   void SetupImageFilter() const;

private:

   AtUInt32 m_minx, m_miny, m_maxx, m_maxy;
   AtUInt32 m_width, m_height;
   float    m_pixelAspectRatio;
   bool     m_useRenderRegion;
   bool     m_clearBeforeRender; 

   AtFloat  m_startFrame;
   AtFloat  m_endFrame;
   AtFloat  m_byFrameStep;
   bool     m_multiCameraRender;

   MObject  m_options;
   MDagPath m_camera;

   MString  m_renderDriver;
   MString  m_imageFileExtension;
   MString  m_imageFilename;
   AtUInt32 m_extensionPadding;
   MString  m_arnoldRenderImageFormat;
   bool     m_isAnimated;

   MCommonRenderSettingsData m_defaultRenderGlobalsData;

   bool     m_progressive_rendering;
   AtUInt   m_progressive_initial_level;
   AtUInt   m_threads;
   MString  m_plugins_path;

   AtUInt   m_AA_samples;
   AtUInt   m_GI_diffuse_samples;
   AtUInt   m_GI_glossy_samples;
   AtUInt   m_sss_sample_factor;
   float    m_AA_sample_clamp;
   bool     m_lock_sampling_noise;

   bool m_use_existing_tiled_textures;
   
   MString  m_outputAssFile;

   bool     m_outputAssBoundingBox;
   AtUInt   m_outputAssMask;

   MString  m_log_filename;
   AtUInt   m_log_max_warnings;
   AtUInt   m_log_console_verbosity;
   AtUInt   m_log_file_verbosity;
};

#endif // RENDEROPTIONS_H
