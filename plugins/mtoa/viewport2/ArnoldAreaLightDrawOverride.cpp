#include "ArnoldAreaLightDrawOverride.h"

#include <iostream>
#include <vector>

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>

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

GLuint CArnoldAreaLightDrawOverride::s_vertexShader = 0;
GLuint CArnoldAreaLightDrawOverride::s_fragmentShader = 0;
GLuint CArnoldAreaLightDrawOverride::s_program = 0;

bool CArnoldAreaLightDrawOverride::s_isValid = false;
bool CArnoldAreaLightDrawOverride::s_isInitialized = false;

// TODO check about delete after use, and
// how to reuse buffers, rather than always
// recreating them, this might won't cause
// much performance problems, but cleaner,
// the better
struct CArnoldAreaLightUserData : public MUserData{
    CGLPrimitive* primitive;
    float m_color[4];
    float m_wireframeColor[4];
    CArnoldAreaLightUserData(const MDagPath& objPath) : MUserData(true)
    { // first draw the quad light
        
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        m_wireframeColor[0] = color.r;
        m_wireframeColor[1] = color.g;
        m_wireframeColor[2] = color.b;
        m_wireframeColor[3] = color.a;

        MFnDependencyNode depNode(objPath.node());

        MStatus status;
        MPlug plug = depNode.findPlug("aiTranslator", &status);
        if (status && !plug.isNull())
        {
            if (plug.asString() == "disk")
                primitive = new CGLDiskLightPrimitive();
            else if (plug.asString() == "cylinder")
                primitive = new CGLCylinderPrimitive();
            else
                primitive = new CGLQuadLightPrimitive();
        }
        else primitive = new CGLQuadLightPrimitive();
    }

    ~CArnoldAreaLightUserData()
    {
        delete primitive;
    }
};

MHWRender::MPxDrawOverride* CArnoldAreaLightDrawOverride::creator(const MObject& obj)
{
    return new CArnoldAreaLightDrawOverride(obj);
}

CArnoldAreaLightDrawOverride::CArnoldAreaLightDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw)
{

}

CArnoldAreaLightDrawOverride::~CArnoldAreaLightDrawOverride()
{

}

bool CArnoldAreaLightDrawOverride::isBounded(
                                        const MDagPath& objPath,
                                        const MDagPath& cameraPath) const
{
    return true;
}

MBoundingBox CArnoldAreaLightDrawOverride::boundingBox(
                                                const MDagPath& objPath,
                                                const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    bbox.expand(MPoint(-1.0, -1.0, 0.0));
    bbox.expand(MPoint(1.0, 1.0, -1.0));
    return bbox;
}

bool CArnoldAreaLightDrawOverride::disableInternalBoundingBoxDraw() const
{
    return false;
}

MUserData* CArnoldAreaLightDrawOverride::prepareForDraw(
                                                    const MDagPath& objPath,
                                                    const MDagPath& cameraPath,
                                                    const MHWRender::MFrameContext& frameContext,
                                                    MUserData* oldData)
{
    return new CArnoldAreaLightUserData(objPath);
}

MHWRender::DrawAPI CArnoldAreaLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL); // | MHWRender::kDirectX11); TODO support dx11 later
}

bool checkShaderError(GLuint shader)
{
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());

        std::cerr << "[MtoA] Error compiling vertex shader : " << errorLog.data() << std::endl;
        return true;
    }
    return false;
}

bool checkProgramError(GLuint program)
{
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());

        std::cerr << "[MtoA] Error linking shader program : " << errorLog.data() << std::endl;
        return true;
    }
    return false;
}

void CArnoldAreaLightDrawOverride::initializeGPUResources()
{
    if (s_isInitialized == false)
    {
        s_isInitialized = true;
        s_isValid = false;
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
    }
}

void CArnoldAreaLightDrawOverride::draw(
                                    const MHWRender::MDrawContext& context,
                                    const MUserData* data)
{
    const CArnoldAreaLightUserData* userData = reinterpret_cast<const CArnoldAreaLightUserData*>(data);
    initializeGPUResources();
    if (s_isValid == false)
        return;
    glUseProgram(s_program);
    float mat[4][4];
    context.getMatrix(MHWRender::MDrawContext::kWorldViewProjMtx).get(mat);
    glUniformMatrix4fv(0, 1, GL_FALSE, &mat[0][0]);
    glUniform4f(4, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
        userData->m_wireframeColor[2], userData->m_wireframeColor[3]);
    
    userData->primitive->draw();
    
    glUseProgram(0);
}
