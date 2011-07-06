#ifndef EXPORTOPTIONS_H
#define EXPORTOPTIONS_H

#include <set>

#include "maya/MObject.h"
#include "maya/MDagPath.h"
#include "maya/MAnimControl.h"

// Export
enum ExportMode
{
   MTOA_EXPORT_UNDEFINED,
   MTOA_EXPORT_RENDER,
   MTOA_EXPORT_IPR,
   MTOA_EXPORT_SWATCH,
   MTOA_EXPORT_FILE
};

typedef std::set<MFn::Type> ExcludeSet;
// Any custom filter we might want on exports
// true means filtered OUT, ie NOT exported
struct CExportFilter
{
   bool templated;
   bool hidden;
   bool notinlayer;
   ExcludeSet excluded;

   CExportFilter() :  templated(true),
                      hidden(true),
                      notinlayer(true) {}
};

#define MTOA_MBLUR_DISABLE 0x0000
#define MTOA_MBLUR_LIGHT   0x0001
#define MTOA_MBLUR_CAMERA  0x0002
#define MTOA_MBLUR_OBJECT  0x0004
#define MTOA_MBLUR_DEFORM  0x0008
#define MTOA_MBLUR_SHADER  0x0010
#define MTOA_MBLUR_ALL     0xFFFF

struct CExportMotion
{
   unsigned int   enable_mask;
   float          shutter_size;
   float          shutter_offset;
   unsigned int   shutter_type;
   unsigned int   steps;
   double         by_frame;

   CExportMotion() : enable_mask(MTOA_MBLUR_DISABLE),
                     shutter_size(0.0f),
                     shutter_offset(0.0f),
                     shutter_type(0),
                     steps(1),
                     by_frame(0.0) {}
};

struct CExportOptions
{
   friend class CExportSession;

   CExportOptions() : m_mode(MTOA_EXPORT_UNDEFINED),
                      m_frame(0.0f),
                      m_options(MObject()),
                      m_camera(MDagPath()),
                      m_filter(CExportFilter()),
                      m_motion(CExportMotion())
   {
      m_frame = MAnimControl::currentTime().as(MTime::uiUnit());
   }

   inline const MDagPath& GetExportCamera() const { return m_camera; }
   inline void SetExportCamera(MDagPath camera) { camera.extendToShape();m_camera = camera; }
   inline const ExportMode& GetExportMode() const {return m_mode;}
   inline void SetExportMode(ExportMode mode) { m_mode = mode; }
   inline const CExportFilter& GetExportFilter() const { return m_filter; }
   inline CExportFilter* ExportFilter() { return &m_filter; }
   inline void SetExportFilter(CExportFilter& filter) { m_filter = filter; }
   inline const MObject& GetArnoldRenderOptions() const { return m_options; }
   inline void SetArnoldRenderOptions(const MObject& options) { m_options = options; }

   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ALL) const { return m_motion.enable_mask && type; }
   inline unsigned int GetNumMotionSteps() const { return m_motion.steps; }
   inline float GetShutterSize() const { return m_motion.shutter_size; }
   inline unsigned int GetShutterType() const { return m_motion.shutter_type; }
   inline double GetExportFrame() const { return m_frame; }
   inline void SetExportFrame(double frame) { m_frame = frame; }
   
private:

   void UpdateMotionBlurData();

private:

   ExportMode      m_mode;
   double          m_frame;

   MObject         m_options;
   MDagPath        m_camera;

   CExportFilter   m_filter;
   CExportMotion   m_motion;

};

#endif // EXPORTOPTIONS_H
