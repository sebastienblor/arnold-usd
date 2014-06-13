#include "utils/DrawUtils.h"

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MObject.h>

class CArnoldStandInDrawOverride : public MHWRender::MPxDrawOverride{
public:
    static MHWRender::MPxDrawOverride* creator(const MObject& obj);

    ~CArnoldStandInDrawOverride();

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
    static void clearGPUResources();
private:
    CArnoldStandInDrawOverride(const MObject& obj);

    static void initializeGPUResources();

    static GLuint s_vertexShader;
    static GLuint s_fragmentShader;
    static GLuint s_program;

    static GLint s_modelViewProjLoc;
    static GLint s_scaleLoc;
    static GLint s_offsetLoc;
    static GLint s_shadeColorLoc;

    static GLuint s_VBO;
    static GLuint s_IBO;
    static GLuint s_VAO;

    static bool s_isValid;
    static bool s_isInitialized;
};
