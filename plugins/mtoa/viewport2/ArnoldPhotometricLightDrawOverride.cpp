#include "ArnoldPhotometricLightDrawOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>

#include <iostream>

#include <ai.h>

namespace{
    const char* shaderUniforms = "#version 430\n"
"layout (location = 0) uniform mat4 modelViewProj;\n"
"layout (location = 4) uniform vec4 shadeColor;\n";

    const char* vertexShader = 
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = modelViewProj * vec4(position, 1.0f);\n"
"}\n";

    const char* fragmentShader =
"out vec4 frag_color;\n"
"void main() { frag_color = shadeColor;}\n";    
}

GLuint CArnoldPhotometricLightDrawOverride::s_vertexShader = 0;
GLuint CArnoldPhotometricLightDrawOverride::s_fragmentShader = 0;
GLuint CArnoldPhotometricLightDrawOverride::s_program = 0;

GLuint CArnoldPhotometricLightDrawOverride::s_VBO = 0;
GLuint CArnoldPhotometricLightDrawOverride::s_IBO = 0;
GLuint CArnoldPhotometricLightDrawOverride::s_VAO = 0;

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
    return new SArnoldPhotometricLightUserData(objPath);
}

MHWRender::DrawAPI CArnoldPhotometricLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL); // | MHWRender::kDirectX11); TODO support dx11 later
}

void CArnoldPhotometricLightDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{    
    if (!s_isValid)
        return;
    const SArnoldPhotometricLightUserData* userData = reinterpret_cast<const SArnoldPhotometricLightUserData*>(data);

    glUseProgram(s_program);

    float mat[4][4]; // load everything in one go, using one continous glUniformfv call
    context.getMatrix(MHWRender::MDrawContext::kWorldViewProjMtx).get(mat);
    glUniformMatrix4fv(0, 1, GL_FALSE, &mat[0][0]);
    glUniform4f(4, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
            userData->m_wireframeColor[2], userData->m_wireframeColor[3]);
    
    sp_primitive->draw();

    glUseProgram(0);
}

void CArnoldPhotometricLightDrawOverride::initializeGPUResources()
{
    if (s_isInitialized == false)
    {
        s_isInitialized = true;
        s_isValid = false;

        if (!GLEW_VERSION_4_3)
            return; // right now, only opengl 4.3, we can lower this later

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

        s_isValid = true;

        /*float vertices[8 * 3] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f
        };*/

        glGenBuffers(1, &s_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
        //glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        /*unsigned int indices[3 * 4 * 2] = {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7
        };*/

        glGenBuffers(1, &s_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &s_VAO);
        glBindVertexArray(s_VAO);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IBO);
        glBindVertexArray(0);

        sp_primitive = new CGLPhotometricLightPrimitive();

    }
}
