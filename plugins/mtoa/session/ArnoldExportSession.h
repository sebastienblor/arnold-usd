#pragma once

#include "ArnoldSession.h"

class DLLEXPORT CArnoldExportSession : public CArnoldSession
{

public:
   CArnoldExportSession() : CArnoldSession() {}
   virtual ~CArnoldExportSession() {}
   
   void Export(MSelectionList* selected = NULL) override;
   /// Get a valid ass name
   MString GetExportFilename(const MString& customName,
                            const MCommonRenderSettingsData& renderGlobals,
                            double frameNumber,
                            const MString &sceneName,
                            const MString &cameraName,
                            const MString &fileFormat,
                            const MObject layer,
                            const bool createDirectory=true,
                            const bool isSequence=false,
                            const bool subFrames=false,
                            const bool isBatch=false,
                            MStatus *ReturnStatus=NULL) const;
   /// Export and ass file.
   /// \param customFileName file to export too.
   void WriteScene(MString customFileName, const bool compressed=false, const bool writeBox=false, bool append = false);
   
	bool IsFileExport() const override {return true;}

private:
   AtBBox GetBoundingBox();

}; 
