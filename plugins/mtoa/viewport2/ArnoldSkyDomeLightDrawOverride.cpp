#include "ArnoldSkydomeLightDrawOverride.h"

#include <iostream>

namespace{
    const char* shaderUniforms = "#version 430\n"
"layout (location = 0) uniform mat4 viewProj;\n"
"layout (location = 5) uniform float scale;\n"
"layout (location = 6) uniform vec4 shadeColor;\n";

    const char* vertexShaderTextured = 
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 texcoord;\n"
"out vec2 txc;"
"void main()\n"
"{\n"
"gl_Position = viewProj * (scale * vec4(position, 1.0f));\n"
"txc = texcoord;"
"}\n";

    const char* vertexShaderWireframe = 
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = viewProj * (scale * vec4(position, 1.0f));\n"
"}\n";

    const char* fragmentShaderTextured =
"int vec2 txc;\n"
"out vec4 frag_color;\n"
"void main() { frag_color = shadeColor;}\n";

    const char* fragmentShaderWireframe =
"out vec4 frag_color;\n"
"void main() { frag_color = shadeColor;}\n";
}

GLuint CArnoldSkyDomeLightDrawOverride::s_vertexShaderWireframe = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_fragmentShaderWireframe = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_programWireframe = 0;

GLuint CArnoldSkyDomeLightDrawOverride::s_vertexShaderTextured = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_fragmentShaderTextured = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_programTextured = 0;

bool CArnoldSkyDomeLightDrawOverride::s_isValid = false;
bool CArnoldSkyDomeLightDrawOverride::s_isInitialized = false;

MHWRender::MPxDrawOverride* CArnoldSkyDomeLightDrawOverride::creator(const MObject& obj)
{
    return new CArnoldSkyDomeLightDrawOverride(obj);
}

CArnoldSkyDomeLightDrawOverride::CArnoldSkyDomeLightDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw)
{

}

CArnoldSkyDomeLightDrawOverride::~CArnoldSkyDomeLightDrawOverride()
{

}

bool CArnoldSkyDomeLightDrawOverride::isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    return false;
}

MBoundingBox CArnoldSkyDomeLightDrawOverride::boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    return bbox;
}

bool CArnoldSkyDomeLightDrawOverride::disableInternalBoundingBoxDraw() const
{
    return true;
}

MUserData* CArnoldSkyDomeLightDrawOverride::prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData)
{    
    return 0;
}

MHWRender::DrawAPI CArnoldSkyDomeLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL); // | MHWRender::kDirectX11); TODO support dx11 later
}

void CArnoldSkyDomeLightDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{

}

void CArnoldSkyDomeLightDrawOverride::initializeGPUResources()
{
    if (s_isInitialized == false)
    {
        s_isInitialized = true;
        s_isValid = false;

        if (!GLEW_VERSION_4_3)
            return; // right now, only opengl 4.3, we can lower this later

        // program for wireframe display

        s_vertexShaderWireframe = glCreateShader(GL_VERTEX_SHADER);
        const char* stringPointers[2] = {shaderUniforms, vertexShaderWireframe};
        glShaderSource(s_vertexShaderWireframe, 2, stringPointers, 0);
        glCompileShader(s_vertexShaderWireframe);

        if (checkShaderError(s_vertexShaderWireframe))
            return;

        s_fragmentShaderWireframe = glCreateShader(GL_FRAGMENT_SHADER);
        stringPointers[1] = fragmentShaderWireframe;
        glShaderSource(s_fragmentShaderWireframe, 2, stringPointers, 0);
        glCompileShader(s_fragmentShaderWireframe);

        if (checkShaderError(s_fragmentShaderWireframe))
            return;

        s_programWireframe = glCreateProgram();
        glAttachShader(s_programWireframe, s_vertexShaderWireframe);
        glAttachShader(s_programWireframe, s_fragmentShaderWireframe);
        glLinkProgram(s_programWireframe);

        if (checkProgramError(s_programWireframe))
            return;

        // program for textured display

        s_vertexShaderTextured = glCreateShader(GL_VERTEX_SHADER);
        stringPointers[1] = vertexShaderTextured;
        glShaderSource(s_vertexShaderTextured, 2, stringPointers, 0);
        glCompileShader(s_vertexShaderTextured);

        if (checkShaderError(s_vertexShaderTextured))
            return;

        s_fragmentShaderTextured = glCreateShader(GL_FRAGMENT_SHADER);
        stringPointers[1] = fragmentShaderTextured;
        glShaderSource(s_fragmentShaderTextured, 2, stringPointers, 0);
        glCompileShader(s_fragmentShaderTextured);

        if (checkShaderError(s_fragmentShaderTextured))
            return;

        s_programTextured = glCreateProgram();
        glAttachShader(s_programTextured, s_vertexShaderTextured);
        glAttachShader(s_programTextured, s_fragmentShaderTextured);
        glLinkProgram(s_programTextured);

        if (checkProgramError(s_programTextured))
            return;

        s_isValid = true;
    }
}
