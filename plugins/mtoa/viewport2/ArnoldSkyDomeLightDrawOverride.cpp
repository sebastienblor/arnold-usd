#include "ArnoldSkyDomeLightDrawOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>

#include <iostream>

#include <ai.h>

namespace{
    const char* shaderUniforms = "#version 430\n"
"layout (location = 0) uniform mat4 model;\n"
"layout (location = 4) uniform mat4 viewProj;\n"
"layout (location = 8) uniform float scale;\n"
"layout (location = 9) uniform vec4 shadeColor;\n";

    const char* vertexShaderWireframe = 
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = viewProj * (model * vec4(scale * position, 1.0f));\n"
"}\n";

    const char* vertexShaderTextured = 
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 texcoord;\n"
"out vec2 tx;\n"
"void main()\n"
"{\n"
"gl_Position = viewProj * (scale * vec4(position, 1.0f));\n"
"tx = texcoord;\n"
"}\n";    

    const char* fragmentShaderWireframe =
"out vec4 frag_color;\n"
"void main() { frag_color = shadeColor;}\n";

    const char* fragmentShaderTextured =
"in vec2 tx;\n"
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

GLuint CArnoldSkyDomeLightDrawOverride::s_VBO = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_IBOWireframe = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_IBOTextured = 0;

GLuint CArnoldSkyDomeLightDrawOverride::s_VAOWireframe = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_VAOTexturedBall = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_VAOTexturedAngular = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_VAOTexturedLatLong = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_VAOTexturedCubic = 0;

GLuint CArnoldSkyDomeLightDrawOverride::s_numWireframeIndices = 0;
GLuint CArnoldSkyDomeLightDrawOverride::s_numTexturedIndices = 0;

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
    return true;
}

MBoundingBox CArnoldSkyDomeLightDrawOverride::boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    MFnDependencyNode depNode(objPath.node());

    MStatus status;
    double radius = 1.0f;
    MPlug plug = depNode.findPlug("skyRadius", &status);
    if (status && !plug.isNull())
        radius = (double)plug.asFloat();
    bbox.expand(MPoint(radius, radius, radius));
    bbox.expand(MPoint(-radius, -radius, -radius));
    // we cannot compensate for translation
    // because this function is not called for every
    // translation
    return bbox;
}

bool CArnoldSkyDomeLightDrawOverride::disableInternalBoundingBoxDraw() const
{
    return true;
}

struct SArnoldSkyDomeLightUserData : public MUserData{
    float m_modelMatrix[4][4]; // cut out translation, that doesn't matter
    float m_wireframeColor[4];
    float m_radius;
    int m_format;    
    // 0 - MirroredBall
    // 1 - Angular
    // 2 - LatLong
    // 3 - Cubic
    SArnoldSkyDomeLightUserData(const MDagPath& objPath) : MUserData(true)
    {
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        m_wireframeColor[0] = color.r;
        m_wireframeColor[1] = color.g;
        m_wireframeColor[2] = color.b;
        m_wireframeColor[3] = color.a;

        MFnDependencyNode depNode(objPath.node());

        MStatus status;
        MPlug plug = depNode.findPlug("skyRadius", &status);
        if (status && !plug.isNull())
            m_radius = plug.asFloat();

        plug = depNode.findPlug("format", &status);
        if (status && !plug.isNull())
            m_format = plug.asInt();

        // zeroing the translation component out
        objPath.inclusiveMatrix().get(m_modelMatrix);
        m_modelMatrix[3][0] = 0.0f;
        m_modelMatrix[3][1] = 0.0f;
        m_modelMatrix[3][2] = 0.0f;
    }

    ~SArnoldSkyDomeLightUserData()
    {

    }
};


MUserData* CArnoldSkyDomeLightDrawOverride::prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData)
{
    initializeGPUResources();
    return new SArnoldSkyDomeLightUserData(objPath);
}

MHWRender::DrawAPI CArnoldSkyDomeLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL); // | MHWRender::kDirectX11); TODO support dx11 later
}

void CArnoldSkyDomeLightDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{
    if (!s_isValid)
        return;
    const SArnoldSkyDomeLightUserData* userData = reinterpret_cast<const SArnoldSkyDomeLightUserData*>(data);

    float mat[4][4];
    context.getMatrix(MHWRender::MDrawContext::kViewProjMtx).get(mat);

    if (context.getDisplayStyle() & MHWRender::MDrawContext::kGouraudShaded)
    {

    }
    else
    {
        glUseProgram(s_programWireframe);

        glUniformMatrix4fv(0, 1, GL_FALSE, &userData->m_modelMatrix[0][0]);    
        glUniformMatrix4fv(4, 1, GL_FALSE, &mat[0][0]);
        glUniform1f(8, userData->m_radius);
        glUniform4f(9, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
            userData->m_wireframeColor[2], userData->m_wireframeColor[3]);

        glBindVertexArray(s_VAOWireframe);
        glDrawElements(GL_LINES, s_numWireframeIndices, GL_UNSIGNED_INT, 0);
    }
    
    glBindVertexArray(0);
    glUseProgram(0);
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

        const int resolution = 20; // 20 by 20 sphere should be enough
        // creating Vertex, Index buffers and Vertex Arrays
        const int stride = 3 + 4 * 2; // first 3 for positions, the next 4 for the different UV coordinates

        // first vertex for the bottom pole, second for the top, and the rest
        // for the rings along the sphere from the bottom
        // to the top

        float vertices[stride * (2 + resolution * resolution)];
        int id = 0;
        vertices[id++] = 0.0f;
        vertices[id++] = -1.0f;
        vertices[id++] = 0.0f;
        AtVector dir = {0.0f, -1.0f, 0.0f}; // if I convert vertices
        // into AtVector*, it will generate a gcc warning
        // dereferencing type-punned pointer will break strict-aliasing rules
        AiMappingMirroredBall(&dir, vertices + id, vertices + id + 1);
        id += 2;
        AiMappingAngularMap(&dir, vertices + id, vertices + id + 1);
        id += 2;
        AiMappingLatLong(&dir, vertices + id, vertices + id + 1);
        id += 2;
        AiMappingCubicMap(&dir, vertices + id, vertices + id + 1);
        id += 2;
        dir.y = 1.0f;
        vertices[id++] = 0.0f;
        vertices[id++] = 1.0f;
        vertices[id++] = 0.0f;
        AiMappingMirroredBall(&dir, vertices + id, vertices + id + 1);
        id += 2;
        AiMappingAngularMap(&dir, vertices + id, vertices + id + 1);
        id += 2;
        AiMappingLatLong(&dir, vertices + id, vertices + id + 1);
        id += 2;
        AiMappingCubicMap(&dir, vertices + id, vertices + id + 1);
        id += 2;
        for (int yy = 0; yy < resolution; ++yy)
        {
            const float dy = AI_PI * float(yy) / float(resolution) - AI_PIOVER2;
            const float y = sinf(dy);
            const float pr = cosf(dy);
            for (int xx = 0; xx < resolution; ++xx)
            {
                const float dx = AI_PITIMES2 * float(xx) / float(resolution);
                dir.x = vertices[id++] = cosf(dx) * pr;
                dir.y = vertices[id++] = y;
                dir.z = vertices[id++] = sinf(dx) * pr;
                AiMappingMirroredBall(&dir, vertices + id, vertices + id + 1);
                id += 2;
                AiMappingAngularMap(&dir, vertices + id, vertices + id + 1);
                id += 2;
                AiMappingLatLong(&dir, vertices + id, vertices + id + 1);
                id += 2;
                AiMappingCubicMap(&dir, vertices + id, vertices + id + 1);
                id += 2;
            }
        }

        glGenBuffers(1, &s_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
        glBufferData(GL_ARRAY_BUFFER, stride * (2 + resolution * resolution) * sizeof(float),
            vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        const unsigned int numWireframeIndices = resolution * resolution * 2 + // for horizontal lines
                                (resolution + 1) * resolution * 2;
        s_numWireframeIndices = numWireframeIndices; // array init requires a constant expression
        unsigned int indicesWireframe[numWireframeIndices]; // for vertical lines
        // fill horizontal lines
        id = 0;
        for (int yy = 0; yy < resolution; ++yy)
        {
            const int wy = 2 + yy * resolution;
            for (int xx = 0; xx < resolution; ++xx)
            {
                indicesWireframe[id++] = wy + xx;
                indicesWireframe[id++] = wy + (xx + 1) % resolution;
            }
        }

        // fill vertical lines
        for (int xx = 0; xx < resolution; ++xx)
        {
            const int xx2 = 2 + xx;
            indicesWireframe[id++] = 0;         
            indicesWireframe[id++] = xx2;
            for (int yy = 0; yy < (resolution - 1); ++yy)
            {
                indicesWireframe[id++] = xx2 + yy * resolution;
                indicesWireframe[id++] = xx2 + (yy + 1) * resolution;
            }
            indicesWireframe[id++] = xx2 + (resolution - 1) * resolution;
            indicesWireframe[id++] = 1;
        }

        glGenBuffers(1, &s_IBOWireframe);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IBOWireframe);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_numWireframeIndices * sizeof(unsigned int),
                        indicesWireframe, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &s_VAOWireframe);
        glBindVertexArray(s_VAOWireframe);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IBOWireframe);
        glBindVertexArray(0);
    }
}
