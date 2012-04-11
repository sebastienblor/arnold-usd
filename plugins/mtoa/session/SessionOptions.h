#ifndef SESSIONOPTIONS_H
#define SESSIONOPTIONS_H

#include <set>

#include "maya/MObject.h"
#include "maya/MDagPath.h"
#include "maya/MAnimControl.h"

// Export
enum ArnoldSessionMode
{
   MTOA_SESSION_UNDEFINED,
   MTOA_SESSION_RENDER,
   MTOA_SESSION_BATCH,
   MTOA_SESSION_IPR,
   MTOA_SESSION_SWATCH,
   MTOA_SESSION_ASS
};

enum ArnoldLightLinkMode
{
   MTOA_LIGHTLINK_NONE,
   MTOA_LIGHTLINK_MAYA
};

enum ArnoldShadowLinkMode
{
   MTOA_SHADOWLINK_NONE,
   MTOA_SHADOWLINK_LIGHT,
   MTOA_SHADOWLINK_MAYA,
};

// Filters
#define MTOA_FILTER_DISABLE   0x0000
#define MTOA_FILTER_HIDDEN    0x0001
#define MTOA_FILTER_TEMPLATED 0x0002
#define MTOA_FILTER_LAYER     0x0004
#define MTOA_FILTER_ALL       0xFFFF

typedef std::set<MFn::Type> MFnTypeSet;

struct CMayaExportFilter
{
   unsigned int state_mask;
   MFnTypeSet excluded;

   CMayaExportFilter() :  state_mask(MTOA_FILTER_ALL) {}
};

#define MTOA_MBLUR_DISABLE 0x0000
#define MTOA_MBLUR_LIGHT   0x0001
#define MTOA_MBLUR_CAMERA  0x0002
#define MTOA_MBLUR_OBJECT  0x0004
#define MTOA_MBLUR_DEFORM  0x0008
#define MTOA_MBLUR_SHADER  0x0010
#define MTOA_MBLUR_ALL     0xFFFF

struct CMotionBlurOptions
{
   unsigned int   enable_mask;
   float          shutter_size;
   float          shutter_offset;
   unsigned int   shutter_type;
   unsigned int   steps;
   double         by_frame;

   CMotionBlurOptions() :  enable_mask(MTOA_MBLUR_DISABLE),
                           shutter_size(0.0f),
                           shutter_offset(0.0f),
                           shutter_type(0),
                           steps(1),
                           by_frame(0.0) {}
};

/// Structure to hold options relative to a CArnoldSession
struct CSessionOptions
{
   friend class CArnoldSession;
   friend class CMayaScene;

private:

   CSessionOptions() :  m_mode(MTOA_SESSION_UNDEFINED),
                        m_lightlink(MTOA_LIGHTLINK_NONE),
                        m_shadowlink(MTOA_SHADOWLINK_NONE),
                        m_frame(0.0f),
                        m_options(MObject()),
                        m_camera(MDagPath()),
                        m_filter(CMayaExportFilter()),
                        m_motion(CMotionBlurOptions())
   {
      m_frame = MAnimControl::currentTime().as(MTime::uiUnit());
   }

   inline const ArnoldSessionMode& GetSessionMode() const {return m_mode;}
   inline void SetSessionMode(ArnoldSessionMode mode) { m_mode = mode; }

   inline const ArnoldLightLinkMode& GetLightLinkMode() const {return m_lightlink;}
   inline void SetLightLinkMode(ArnoldLightLinkMode mode) { m_lightlink = mode; }

   inline const ArnoldShadowLinkMode& GetShadowLinkMode() const {return m_shadowlink;}
   inline void SetShadowLinkMode(ArnoldShadowLinkMode mode) { m_shadowlink = mode; }

   inline const MDagPath& GetExportCamera() const { return m_camera; }
   inline void SetExportCamera(MDagPath camera) { camera.extendToShape();m_camera = camera; }

   inline const CMayaExportFilter& GetExportFilter() const { return m_filter; }
   inline unsigned int GetExportFilterMask() const { return m_filter.state_mask; }
   inline void SetExportFilterMask(unsigned int mask) { m_filter.state_mask = mask; }

   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ALL) const { return m_motion.enable_mask & type; }
   inline unsigned int GetNumMotionSteps() const { return m_motion.steps; }
   inline float GetShutterSize() const { return m_motion.shutter_size; }
   inline unsigned int GetShutterType() const { return m_motion.shutter_type; }
   inline double GetExportFrame() const { return m_frame; }
   inline double GetMotionByFrame() const { return m_motion.by_frame; }

   inline const MObject& GetArnoldRenderOptions() const { return m_options; }
   inline void SetArnoldRenderOptions(const MObject& options) { m_options = options; }

   inline void SetExportFrame(double frame) { m_frame = frame; }

   MStatus GetFromMaya();

private:

   ArnoldSessionMode    m_mode;
   ArnoldLightLinkMode  m_lightlink;
   ArnoldShadowLinkMode m_shadowlink;
   double               m_frame;

   MObject              m_options;
   MDagPath             m_camera;

   CMayaExportFilter    m_filter;
   CMotionBlurOptions   m_motion;

};

#endif // SESSIONOPTIONS_H
