#pragma once

#include <maya/MCommonRenderSettingsData.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <platform/Platform.h>

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

MStringArray getSourceImagesPath();
