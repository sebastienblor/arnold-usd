#ifndef EXPORTOPTIONS_H
#define EXPORTOPTIONS_H

#include <set>

// The different ExportMode were not really mutually exclusive
// (you can have IPR render on selected only)
// So redone as ExportMode and ExportFilter
enum ExportMode
{
   MTOA_EXPORT_UNDEFINED,
   MTOA_EXPORT_ALL,
   MTOA_EXPORT_IPR,
   MTOA_EXPORT_SWATCH,
   MTOA_EXPORT_FILE
};

typedef std::set<MFn::Type> ExcludeSet;
// Any custom filter we might want on exports
// true means filtered OUT, ie NOT exported
struct CExportFilter
{
   bool unselected;
   bool templated;
   bool hidden;
   bool notinlayer;
   ExcludeSet excluded;

   CExportFilter() :  unselected(false),
                      templated(true),
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
   AtUInt enable_mask;
   AtFloat shutter_size;
   AtFloat shutter_offset;
   AtUInt shutter_type;
   AtUInt steps;
   AtFloat by_frame;

   CExportMotion() :  enable_mask(MTOA_MBLUR_DISABLE),
                     shutter_size(0.0f),
                     shutter_offset(0.0f),
                     shutter_type(0),
                     steps(1),
                     by_frame(0.0f) {}
};

struct CExportOptions
{
   friend class CMayaScene;

   CExportOptions() : m_frame(0.0f),
                      m_mode(MTOA_EXPORT_UNDEFINED),
                      m_filter(CExportFilter()),
                      m_motion(CExportMotion()) {}

   inline ExportMode GetExportMode() const {return m_mode;}
   inline void SetExportMode(ExportMode mode) { m_mode = mode; }
   inline CExportFilter GetExportFilter()const { return m_filter; }
   inline void SetExportFilter(CExportFilter& filter) { m_filter = filter; }

   inline bool IsMotionBlurEnabled(int type = MTOA_MBLUR_ALL) const { return m_motion.enable_mask && type; }
   inline AtUInt GetNumMotionSteps() const { return m_motion.steps; }
   inline AtFloat GetShutterSize() const { return m_motion.shutter_size; }
   inline AtUInt GetShutterType() const { return m_motion.shutter_type; }
   inline AtFloat GetCurrentFrame() const { return m_frame; }
   
private:

   AtFloat         m_frame;
   ExportMode      m_mode;
   CExportFilter   m_filter;
   CExportMotion   m_motion;

};

#endif // EXPORTOPTIONS_H
