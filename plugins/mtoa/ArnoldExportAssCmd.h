#ifndef ARNOLD_EXPORT_ASS_CMD_H
#define ARNOLD_EXPORT_ASS_CMD_H

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

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
   MStatus doIt(const MArgList& argList);

}; // class CArnoldExportAssCmd

#endif // ARNOLD_EXPORT_ASS_CMD_H
