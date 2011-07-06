#ifndef AOV_H
#define AOV_H

#include "platform/Platform.h"

#include <ai.h>

#include <maya/MString.h>
#include <maya/MPlug.h>

#include <string>
#include <map>
#include <vector>

struct CAOVData
{
   MString attribute;
   MString name;
   AtInt type;
};

/// child indices of compound plug ArnoldAOV.aovs
enum AOVComponents
{
   AOV_ENABLED,
   AOV_NAME,
   AOV_TYPE,
   AOV_PREFIX
};

enum AOVMode
{
   AOV_MODE_DISABLED,
   AOV_MODE_ENABLED,
   AOV_MODE_BATCH_ONLY
};

/// Represents a single AOV
class DLLEXPORT CAOV
{
public:
   CAOV();
   CAOV(const CAOV &rhs);
   ~CAOV();

   CAOV& operator=(const CAOV &rhs);

   inline const MString& GetName() const
   {
      return m_name;
   }

   inline const MString& GetPrefix() const
   {
      return m_prefix;
   }

   bool IsEnabled()
   {
      return m_enabled;
   }
   bool FromMaya(MPlug &p);

   void SetImageFilename(const MString &filename)
   {
      m_filename = filename;
   }

   void SetupOutput(AtArray *outputs, int i, AtNode *defaultDriver, AtNode *defaultFilter) const;

protected:

   void ReturnType(MString &mtype) const;
   void NormalizePath(MString &path) const;
   void Strip(MString &str) const;

protected:

   MString   m_name;
   // enumerator that corresponds to arnold's data type enum
   AtInt     m_type;
   bool      m_enabled;
   MString   m_prefix;
   MString   m_filename;
};


#endif // AOV_H
