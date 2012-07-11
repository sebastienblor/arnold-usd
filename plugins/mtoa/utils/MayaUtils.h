#ifndef MAYA_UTILS_H
#define MAYA_UTILS_H

#include <maya/MCommonRenderSettingsData.h>
#include <maya/MString.h>

MString getFileName(MCommonRenderSettingsData::MpathType pathType,
                     double frameNumber=0,
                     const MString& sceneFileName="",
                     const MString& cameraName="",
                     const MString& fileFormat="",
                     const MObject& layer=MObject::kNullObj,
                     const MString& customTokenString="",
                     bool createDirectory=true,
                     const MString& fileRuleType="",
                     const MString& path="",
                     const bool* isSequence=NULL,
                     const bool* stricAOVs=NULL);

#endif
