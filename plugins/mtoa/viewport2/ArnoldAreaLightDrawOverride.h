#pragma once

#include <GL/glew.h>

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>

class CArnoldAreaLightDrawOverride : public MHWRender::MPxDrawOverride{
public:
    static MHWRender::MPxDrawOverride* creator(const MObject& obj);

    ~CArnoldAreaLightDrawOverride();

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
    static void draw(const MHWRender::MDrawContext& context, const MUserData* data);    
private:
    CArnoldAreaLightDrawOverride(const MObject& obj);

    static void initializeGPUResources();

    static GLuint s_vertexShader;
    static GLuint s_fragmentShader;
    static GLuint s_program;

    static bool s_isValid;
    static bool s_isInitialized;
};
