#ifndef ARNOLD_EXPORT_ASS_CMD_H
#define ARNOLD_EXPORT_ASS_CMD_H

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MDagPath.h>


class CArnoldExportAssCmd
   :   public MPxCommand
{

public:

   static void* creator()
   {
      return new CArnoldExportAssCmd();
   }

   static MSyntax newSyntax();
   static MDagPath GetCamera();
   void SetBatch(bool batch) {m_batch = batch;}
   bool IsBatch() const {return m_batch;}
   MStatus doIt(const MArgList& argList);

private:
   bool m_batch;
}; // class CArnoldExportAssCmd

#endif // ARNOLD_EXPORT_ASS_CMD_H
