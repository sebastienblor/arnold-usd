#ifndef ARNOLD_EXPORT_ASS_CMD_H
#define ARNOLD_EXPORT_ASS_CMD_H

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>

class CArnoldExportAssCmd
   :   public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldExportAssCmd();
   }

   static MSyntax newSyntax();
   static int GetRenderCameras(MDagPathArray &cameras);
   MStatus doIt(const MArgList& argList);

}; // class CArnoldExportAssCmd

#endif // ARNOLD_EXPORT_ASS_CMD_H
