#pragma once

#include "platform/Platform.h"

#include <maya/MCommonRenderSettingsData.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <platform/Platform.h>

DLLEXPORT MString getFileName(
                            MCommonRenderSettingsData::MpathType pathType,
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
                            const bool* stricAOVs=NULL,
                            const MString& eye=""
                            );

DLLEXPORT MStringArray getSourceImagesPath();

DLLEXPORT MString getProjectFolderPath();

DLLEXPORT MString replaceInString(const MString& source, const MString& origString, const MString& toReplace);

DLLEXPORT MString resolveFilePathForSequences(const MString& sourceFileName, const int frameNumber);

DLLEXPORT bool IsFloatAttrDefault(MPlug plug, float value);
DLLEXPORT bool IsBoolAttrDefault(MPlug plug, bool value);
DLLEXPORT bool IsVec2AttrDefault(MPlug plug, float valueX, float valueY);
DLLEXPORT bool IsRGBAttrDefault(MPlug plug, float valueR, float valueG, float valueB);
