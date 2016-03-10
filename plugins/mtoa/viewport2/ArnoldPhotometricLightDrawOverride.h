#pragma once

#include "utils/DrawUtils.h"

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MObject.h>
#include <maya/MPointArray.h>
#include <maya/MUintArray.h>

class CArnoldPhotometricLightDrawOverride : public MHWRender::MPxDrawOverride{
public:
    static MHWRender::MPxDrawOverride* creator(const MObject& obj);

    ~CArnoldPhotometricLightDrawOverride();

    virtual bool isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const;

    virtual MBoundingBox boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const;

    virtual bool disableInternalBoundingBoxDraw() const;

    virtual MUserData* prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData);

    virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual bool hasUIDrawables() const { return true; }
	virtual void addUIDrawables(
		const MDagPath& objPath,
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext,
		const MUserData* data);

    static void draw(const MHWRender::MDrawContext& context, const MUserData* data);
private:
	CArnoldPhotometricLightDrawOverride(const MObject& obj);

	static void initializeUserData();
    static MPointArray s_positions;
    static MUintArray s_indexing;

    static bool s_isValid;
    static bool s_isInitialized;
};
