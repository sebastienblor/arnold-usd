#include "ArnoldAreaLightDrawOverride.h"

#include <iostream>
#include <vector>

#include <maya/MHWGeometryUtilities.h>

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
    union{
        struct{
            GLuint m_VBO;
            GLuint m_IBO;
        };
        GLuint m_GLBuffers[2];
    };
    GLuint m_VAO;
    float m_color[4];
    float m_wireframeColor[4];
    CArnoldAreaLightUserData(const MDagPath& objPath) : MUserData(true), m_VBO(0), m_IBO(0), m_VAO(0)
    { // first draw the quad light
        glGenBuffers(2, m_GLBuffers);

        const float vertices [] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -1.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        const unsigned int indices[] = {
            0, 1,
            1, 2,
            2, 3,
            3, 0,
            0, 2,
            3, 1,
            4, 5,
            0, 1, 2,
            0, 2, 3
        };

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (7 * 2 + 6) * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        m_wireframeColor[0] = color.r;
        m_wireframeColor[1] = color.g;
        m_wireframeColor[2] = color.b;
        m_wireframeColor[3] = color.a;

        m_color[0] = 1.0f;
        m_color[1] = 1.0f;
        m_color[2] = 1.0f;
        m_color[3] = 1.0f;

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBindVertexArray(0);
    }

    ~CArnoldAreaLightUserData()
    {
        glDeleteBuffers(2, m_GLBuffers);
        glDeleteVertexArrays(1, &m_VAO);
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
    
    glBindVertexArray(userData->m_VAO);
    if (context.getDisplayStyle() & MHWRender::MDrawContext::kGouraudShaded)
    {
        glPushAttrib(GL_POLYGON_BIT);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        glUniform4f(4, userData->m_color[0], userData->m_color[1],
            userData->m_color[2], userData->m_color[3]);
        glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (unsigned int *)0 + 7 * 2);
        glPopAttrib();
    }
    else
    {
        glUniform4f(4, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
            userData->m_wireframeColor[2], userData->m_wireframeColor[3]);
        glDrawElements(GL_LINES, 7 * 2, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}
