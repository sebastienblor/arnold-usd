#include "utils/DrawUtils.h"

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MObject.h>

class CArnoldSkyDomeLightDrawOverride : public MHWRender::MPxDrawOverride{
public:
    static MHWRender::MPxDrawOverride* creator(const MObject& obj);

    ~CArnoldSkyDomeLightDrawOverride();

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
    CArnoldSkyDomeLightDrawOverride(const MObject& obj);

    static void initializeGPUResources();

    static GLuint s_vertexShaderWireframe;
    static GLuint s_fragmentShaderWireframe;
    static GLuint s_programWireframe;

    static GLuint s_vertexShaderTextured;
    static GLuint s_fragmentShaderTextured;
    static GLuint s_programTextured;

    static GLuint s_VBO;
    static GLuint s_IBOWireframe;
    static GLuint s_IBOTextured;

    static GLuint s_VAOWireframe;
    static GLuint s_VAOTexturedBall;
    static GLuint s_VAOTexturedAngular;
    static GLuint s_VAOTexturedLatLong;
    static GLuint s_VAOTexturedCubic;

    static GLuint s_numWireframeIndices;
    static GLuint s_numTexturedIndices;

    static bool s_isValid;
    static bool s_isInitialized;
};
