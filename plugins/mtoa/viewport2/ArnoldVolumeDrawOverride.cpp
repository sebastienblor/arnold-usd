#include "ArnoldVolumeDrawOverride.h"

#include "nodes/shape/ArnoldVolume.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>

#include "ViewportUtils.h"

#include <iostream>

#include <ai.h>

namespace{
    const char* shaderUniforms = "#version 150\n"
"uniform mat4 modelViewProj;\n"
"uniform vec4 scale;"
"uniform vec4 offset;"
"uniform vec4 shadeColor;\n";

    const char* vertexShader = 
"in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = modelViewProj * vec4(position * scale.xyz + offset.xyz, 1.0f);\n"
"}\n";

    const char* fragmentShader =
"out vec4 frag_color;\n"
"void main() { frag_color = shadeColor;}\n";    
}

GLuint CArnoldVolumeDrawOverride::s_vertexShader = 0;
GLuint CArnoldVolumeDrawOverride::s_fragmentShader = 0;
GLuint CArnoldVolumeDrawOverride::s_program = 0;

GLint CArnoldVolumeDrawOverride::s_modelViewProjLoc = 0;
GLint CArnoldVolumeDrawOverride::s_scaleLoc = 0;
GLint CArnoldVolumeDrawOverride::s_offsetLoc = 0;
GLint CArnoldVolumeDrawOverride::s_shadeColorLoc = 0;

GLuint CArnoldVolumeDrawOverride::s_VBO = 0;
GLuint CArnoldVolumeDrawOverride::s_IBO = 0;
GLuint CArnoldVolumeDrawOverride::s_VAO = 0;

bool CArnoldVolumeDrawOverride::s_isValid = false;
bool CArnoldVolumeDrawOverride::s_isInitialized = false;


MHWRender::MPxDrawOverride* CArnoldVolumeDrawOverride::creator(const MObject& obj)
{
    return new CArnoldVolumeDrawOverride(obj);
}

CArnoldVolumeDrawOverride::CArnoldVolumeDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw)
{

}

CArnoldVolumeDrawOverride::~CArnoldVolumeDrawOverride()
{

}

bool CArnoldVolumeDrawOverride::isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    return true;
}

MBoundingBox CArnoldVolumeDrawOverride::boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
   MPoint bbMin = MPoint(-1, -1, -1);
   MPoint bbMax = MPoint(1, 1, 1);

   float minCoords[4];
   float maxCoords[4];

   bbMin.get(minCoords);
   bbMax.get(maxCoords);

   return MBoundingBox (minCoords, maxCoords);
}

bool CArnoldVolumeDrawOverride::disableInternalBoundingBoxDraw() const
{
    return true;
}

struct SArnoldVolumeUserData : public MUserData{
    float m_wireframeColor[4];
    float m_scale[4];
    float m_offset[4];
    
    SArnoldVolumeUserData(const MDagPath& objPath) : MUserData(true)
    {
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        m_wireframeColor[0] = color.r;
        m_wireframeColor[1] = color.g;
        m_wireframeColor[2] = color.b;
        m_wireframeColor[3] = color.a;

        AtVector mn = {
            -1, -1, -1
        };

        AtVector mx = {
            1, 1, 1
        };
        
        MStatus status;
        MFnDependencyNode dnode(objPath.node(), &status);
        if(MStatus::kSuccess == status)
        {
            CArnoldVolumeShape * shapeNode = dynamic_cast<CArnoldVolumeShape*>(dnode.userNode());
            if( shapeNode != NULL)
            {
               MBoundingBox * bbox = shapeNode->geometry();
               mn.x = (float) bbox->min().x;
               mn.y = (float) bbox->min().y;
               mn.z = (float) bbox->min().z;
               
               mx.x = (float) bbox->max().x;
               mx.y = (float) bbox->max().y;
               mx.z = (float) bbox->max().z;
            }
        }

        m_scale[0] = mx.x - mn.x;
        m_scale[1] = mx.y - mn.y;
        m_scale[2] = mx.z - mn.z;
        m_scale[3] = 1.0f;

        m_offset[0] = mn.x;
        m_offset[1] = mn.y;
        m_offset[2] = mn.z;
        m_offset[3] = 0.0f;
    }

    ~SArnoldVolumeUserData()
    {
    }
};

MUserData* CArnoldVolumeDrawOverride::prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData)
{
    initializeGPUResources();
    if (s_isValid)
        return new SArnoldVolumeUserData(objPath);
    else return 0;
}

MHWRender::DrawAPI CArnoldVolumeDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL); // | MHWRender::kDirectX11); TODO support dx11 later
}

void CArnoldVolumeDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{    
    if (!s_isValid)
        return;
    const SArnoldVolumeUserData* userData = reinterpret_cast<const SArnoldVolumeUserData*>(data);

    glUseProgram(s_program);

    float mat[4][4]; // load everything in one go, using one continous glUniformfv call
    context.getMatrix(MHWRender::MDrawContext::kWorldViewProjMtx).get(mat);
    glUniformMatrix4fv(s_modelViewProjLoc, 1, GL_FALSE, &mat[0][0]);
    glUniform4f(s_scaleLoc, userData->m_scale[0], userData->m_scale[1], userData->m_scale[2], userData->m_scale[3]);
    glUniform4f(s_offsetLoc, userData->m_offset[0], userData->m_offset[1], userData->m_offset[2], userData->m_offset[3]);
    glUniform4f(s_shadeColorLoc, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
            userData->m_wireframeColor[2], userData->m_wireframeColor[3]);
    glBindVertexArray(s_VAO);

    glDrawElements(GL_LINES, 3 * 4 * 2, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void CArnoldVolumeDrawOverride::initializeGPUResources()
{
    if ((s_isInitialized == false) && InitializeGLEW())
    {
        s_isInitialized = true;
        s_isValid = false;

        if (!GLEW_VERSION_3_2)
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

        float vertices[8 * 3] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f
        };

        glGenBuffers(1, &s_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
        glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        unsigned int indices[3 * 4 * 2] = {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7
        };

        glGenBuffers(1, &s_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &s_VAO);
        glBindVertexArray(s_VAO);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IBO);
        glBindVertexArray(0);

        s_modelViewProjLoc = glGetUniformLocation(s_program, "modelViewProj");
        s_scaleLoc = glGetUniformLocation(s_program, "scale");
        s_offsetLoc = glGetUniformLocation(s_program, "offset");
        s_shadeColorLoc = glGetUniformLocation(s_program, "shadeColor");
    }
}

void CArnoldVolumeDrawOverride::clearGPUResources()
{
    if (s_isInitialized && InitializeGLEW())
    {
        glDeleteBuffers(1, &s_VBO);
        glDeleteBuffers(1, &s_IBO);
        glDeleteVertexArrays(1, &s_VAO);
        glDeleteShader(s_vertexShader);
        glDeleteShader(s_fragmentShader);
        glDeleteProgram(s_program);
        s_isValid = false;
        s_isInitialized = false;
    }
}
