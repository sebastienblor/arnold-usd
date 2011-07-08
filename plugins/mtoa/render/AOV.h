#ifndef AOV_H
#define AOV_H

#include "platform/Platform.h"

#include <ai.h>

#include <maya/MString.h>
#include <maya/MPlug.h>

class DLLEXPORT CAOV
{
public:
//Deprecated ?
//   enum Type
//   {
//      AOV_BOOL = 0,
//      AOV_INT,
//      AOV_FLT,
//      AOV_RGB,
//      AOV_RGBA,
//      AOV_VEC,
//      AOV_PNT,
//      AOV_PNT2,
//      AOV_PTR,
//      AOV_UNKNOWN
//   };

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

   bool FromMaya(MPlug &p);

   void UpdateImageFilename(const MString &cameraName, const MString &defaultExt, bool multiCam, bool batchMode);

   void SetupOutput(AtArray *outputs, int i, AtNode *defaultDriver, AtNode *defaultFilter) const;

protected:

   void ReturnType(MString &mtype) const;
   void NormalizePath(MString &path) const;
   void Strip(MString &str) const;

protected:

   MString   m_name;
   MString   m_type;
   MString   m_prefix;
   MString   m_filename;
};


#endif // AOV_H
