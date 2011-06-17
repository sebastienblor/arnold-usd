#ifndef ARNOLD_EXPORT_ASS_CMD_H
#define ARNOLD_EXPORT_ASS_CMD_H

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MCommonRenderSettingsData.h>

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
   void SetBatch(bool batch) {m_batch = batch;}
   bool IsBatch() const {return m_batch;}
   MString GetAssName(const MString& customName,
                      const MCommonRenderSettingsData& renderGlobals,
                      double frameNumber,
                      const MString &sceneName,
                      const MString &cameraName,
                      const MString &fileFormat,
                      const MObject layer,
                      const bool createDirectory=true,
                      const bool isSequence=false,
                      const bool subFrames=false,
                      MStatus *ReturnStatus=NULL) const;
   MStatus WriteAsstoc(const MString& filename, const AtBBox& bBox);
   MStatus doIt(const MArgList& argList);

private:
   bool m_batch;
}; // class CArnoldExportAssCmd

#endif // ARNOLD_EXPORT_ASS_CMD_H
