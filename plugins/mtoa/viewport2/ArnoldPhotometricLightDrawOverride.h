#pragma once

#include "utils/DrawUtils.h"

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MObject.h>

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
    static void clearGPUResources();
private:
    CArnoldPhotometricLightDrawOverride(const MObject& obj);

    static void initializeGPUResources();

#ifdef _WIN32
    static CDXConstantBuffer* s_pDXConstantBuffer;
    static DXShader* s_pDXShader;
#endif

    static GLuint s_vertexShader;
    static GLuint s_fragmentShader;
    static GLuint s_program;

    static GLint s_modelViewProjLoc;
    static GLint s_shadeColorLoc;

    static CGPUPrimitive* sp_primitive;

    static bool s_isValid;
    static bool s_isInitialized;
};
