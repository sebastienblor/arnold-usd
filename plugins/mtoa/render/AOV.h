#ifndef AOV_H
#define AOV_H

#include "platform/Platform.h"

#include <ai.h>

#include <maya/MString.h>
#include <maya/MPlug.h>
#include <maya/MFnDependencyNode.h>

#include <string>
#include <map>
#include <vector>

class CMayaScene;

struct CAOVData
{
   MString attribute;
   MString name;
   AtInt type;
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
   bool FromMaya(MObject &o);

   void SetImageFilename(const MString &filename)
   {
      m_filename = filename;
   }

   void SetupOutput(AtArray *outputs, int i, CMayaScene *scene, AtNode *defaultDriver, AtNode *defaultFilter) const;

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
   MObject   m_object;
   MFnDependencyNode m_fnNode;
};


#endif // AOV_H
