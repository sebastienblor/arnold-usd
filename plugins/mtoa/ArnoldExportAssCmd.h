#ifndef ARNOLD_EXPORT_ASS_CMD_H
#define ARNOLD_EXPORT_ASS_CMD_H

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

#include <ai_bbox.h>

class CArnoldExportAssCmd
   :   public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldExportAssCmd();
   }

   static MSyntax newSyntax();
   static MString GetCameraName();
   MStatus WriteAsstoc(const MString& filename, const AtBBox& bBox);
   MStatus doIt(const MArgList& argList);

}; // class CArnoldExportAssCmd

#endif // ARNOLD_EXPORT_ASS_CMD_H
