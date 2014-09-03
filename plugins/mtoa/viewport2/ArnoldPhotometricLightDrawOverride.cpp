#include "ArnoldPhotometricLightDrawOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>
#include <maya/M3dView.h>

#include <iostream>

#include "ViewportUtils.h"

#include <ai.h>

namespace{
    const char* shaderUniforms = "#version 120\n"
"uniform mat4 modelViewProj;\n"
"uniform vec4 shadeColor;\n";

    const char* vertexShader = 
"void main()\n"
"{\n"
"gl_Position = modelViewProj * gl_Vertex;\n"
"}\n";

    const char* fragmentShader =
"void main() { gl_FragColor = shadeColor;}\n";    
}

GLuint CArnoldPhotometricLightDrawOverride::s_vertexShader = 0;
GLuint CArnoldPhotometricLightDrawOverride::s_fragmentShader = 0;
GLuint CArnoldPhotometricLightDrawOverride::s_program = 0;

GLint CArnoldPhotometricLightDrawOverride::s_modelViewProjLoc = 0;
GLint CArnoldPhotometricLightDrawOverride::s_shadeColorLoc = 0;

CGLPrimitive* CArnoldPhotometricLightDrawOverride::sp_primitive = 0;

bool CArnoldPhotometricLightDrawOverride::s_isValid = false;
bool CArnoldPhotometricLightDrawOverride::s_isInitialized = false;


MHWRender::MPxDrawOverride* CArnoldPhotometricLightDrawOverride::creator(const MObject& obj)
{
    return new CArnoldPhotometricLightDrawOverride(obj);
}

CArnoldPhotometricLightDrawOverride::CArnoldPhotometricLightDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw)
{

}

CArnoldPhotometricLightDrawOverride::~CArnoldPhotometricLightDrawOverride()
{

}

bool CArnoldPhotometricLightDrawOverride::isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    return false;
}

MBoundingBox CArnoldPhotometricLightDrawOverride::boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    MFnDependencyNode depNode(objPath.node());

    
    return bbox;
}

bool CArnoldPhotometricLightDrawOverride::disableInternalBoundingBoxDraw() const
{
    return true;
}

struct SArnoldPhotometricLightUserData : public MUserData{
    float m_wireframeColor[4];
    
    SArnoldPhotometricLightUserData(const MDagPath& objPath) : MUserData(true)
    {
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        m_wireframeColor[0] = color.r;
        m_wireframeColor[1] = color.g;
        m_wireframeColor[2] = color.b;
        m_wireframeColor[3] = color.a;
    }

    ~SArnoldPhotometricLightUserData()
    {
    }
};


MUserData* CArnoldPhotometricLightDrawOverride::prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData)
{
    initializeGPUResources();
    if (s_isValid)
        return new SArnoldPhotometricLightUserData(objPath);
    else return 0;
}

MHWRender::DrawAPI CArnoldPhotometricLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL); // | MHWRender::kDirectX11); TODO support dx11 later
}

void CArnoldPhotometricLightDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{    
    if (!s_isValid)
        return;
    if ((M3dView::active3dView().objectDisplay() & M3dView::kDisplayLights) == 0)
        return;
    const SArnoldPhotometricLightUserData* userData = reinterpret_cast<const SArnoldPhotometricLightUserData*>(data);

    glUseProgram(s_program);

    float mat[4][4]; // load everything in one go, using one continous glUniformfv call
    context.getMatrix(MHWRender::MDrawContext::kWorldViewProjMtx).get(mat);
    glUniformMatrix4fv(s_modelViewProjLoc, 1, GL_FALSE, &mat[0][0]);
    glUniform4f(s_shadeColorLoc, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
            userData->m_wireframeColor[2], userData->m_wireframeColor[3]);
    
    sp_primitive->draw();

    glUseProgram(0);
}

void CArnoldPhotometricLightDrawOverride::initializeGPUResources()
{
    if ((s_isInitialized == false) && InitializeGLEW())
    {
        s_isInitialized = true;
        s_isValid = false;

        if (!GLEW_VERSION_2_1)
            return;

        // program for wireframe display

        s_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* stringPointers[2] = {shaderUniforms, vertexShader};
        glShaderSource(s_vertexShader, 2, stringPointers, 0);
        glCompileShader(s_vertexShader);

        if (checkShaderError(s_vertexShader))
            return;

        s_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        stringPointers[1] = fragmentShader;
        glShaderSource(s_fragmentShader, 2, stringPointers, 0);
        glCompileShader(s_fragmentShader);

        if (checkShaderError(s_fragmentShader))
            return;

        s_program = glCreateProgram();
        glAttachShader(s_program, s_vertexShader);
        glAttachShader(s_program, s_fragmentShader);
        glLinkProgram(s_program);

        if (checkProgramError(s_program))
            return;       

        sp_primitive = new CGLPhotometricLightPrimitive();

        s_modelViewProjLoc = glGetUniformLocation(s_program, "modelViewProj");
        s_shadeColorLoc = glGetUniformLocation(s_program, "shadeColor");

        s_isValid = true;
    }
}

void CArnoldPhotometricLightDrawOverride::clearGPUResources()
{    
    if (s_isInitialized && InitializeGLEW())
    {
        glDeleteShader(s_vertexShader);
        glDeleteShader(s_fragmentShader);
        glDeleteProgram(s_program);
        if (s_isValid)
            delete sp_primitive;
        s_isValid = false;
        s_isInitialized = false;
    }
}
