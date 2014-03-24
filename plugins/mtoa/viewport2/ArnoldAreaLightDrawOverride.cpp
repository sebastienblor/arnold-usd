#include "ArnoldAreaLightDrawOverride.h"

#include <iostream>
#include <vector>

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>

const char* shaderUniforms = "#version 150\n"
"uniform mat4 model;\n"
"uniform mat4 viewProj;\n"
"uniform vec4 shadeColor;\n";

const char* vertexShader = 
"in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = viewProj * (model * vec4(position, 1.0f));\n"
"}\n";

const char* fragmentShader =
"out vec4 frag_color;\n"
"void main() { frag_color = shadeColor;}\n";

GLuint CArnoldAreaLightDrawOverride::s_vertexShader = 0;
GLuint CArnoldAreaLightDrawOverride::s_fragmentShader = 0;
GLuint CArnoldAreaLightDrawOverride::s_program = 0;

GLint CArnoldAreaLightDrawOverride::s_modelLoc = 0;
GLint CArnoldAreaLightDrawOverride::s_viewProjLoc = 0;
GLint CArnoldAreaLightDrawOverride::s_shadeColorLoc = 0;

bool CArnoldAreaLightDrawOverride::s_isValid = false;
bool CArnoldAreaLightDrawOverride::s_isInitialized = false;

// TODO check about delete after use, and
// how to reuse buffers, rather than always
// recreating them, this might won't cause
// much performance problems, but cleaner,
// the better
struct CArnoldAreaLightUserData : public MUserData{
    static CGLPrimitive* s_primitives[3];
    CGLPrimitive* p_primitive;
    float m_modelMatrix[4][4];
    float m_color[4];
    float m_wireframeColor[4];
    CArnoldAreaLightUserData(const MDagPath& objPath) : MUserData(true)
    {
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        m_wireframeColor[0] = color.r;
        m_wireframeColor[1] = color.g;
        m_wireframeColor[2] = color.b;
        m_wireframeColor[3] = color.a;

        MFnDependencyNode depNode(objPath.node());

        MStatus status;
        MPlug plug = depNode.findPlug("aiTranslator", &status);
        MTransformationMatrix modelMatrix(objPath.inclusiveMatrix());
        if (status && !plug.isNull())
        {
            if (plug.asString() == "disk")
            {
                p_primitive = s_primitives[1];
                double scale[3];
                modelMatrix.getScale(scale, MSpace::kWorld);
                if (scale[0] != scale[1]) // non uniform scaling across x and y
                {
                    double scale2[3] = {1.0, 1.0, 1.0};
                    if (scale[0] != 0.0)
                        scale2[0] /= scale[0];
                    if (scale[1] != 0)
                        scale2[1] /= scale[1];
                    const double avs = (scale[0] + scale[1]) * 0.5;
                    scale2[0] *= avs;
                    scale2[1] *= avs;
                    modelMatrix.addScale(scale2, MSpace::kWorld);
                }
            }
            else if (plug.asString() == "cylinder")
            {
                p_primitive = s_primitives[2];
                double scale[3];
                modelMatrix.getScale(scale, MSpace::kWorld);
                if (scale[0] != scale[2]) // non uniform scaling across x and y
                {
                    double scale2[3] = {1.0, 1.0, 1.0};
                    if (scale[0] != 0.0)
                        scale2[0] /= scale[0];
                    if (scale[2] != 0)
                        scale2[2] /= scale[2];
                    const double avs = (scale[0] + scale[2]) * 0.5;
                    scale2[0] *= avs;
                    scale2[2] *= avs;
                    modelMatrix.addScale(scale2, MSpace::kWorld);
                }
            }
            else
                p_primitive = s_primitives[0];
        }
        else p_primitive = s_primitives[0];
        
        modelMatrix.asMatrix().get(m_modelMatrix);
    }

    ~CArnoldAreaLightUserData()
    {
    }
};

CGLPrimitive* CArnoldAreaLightUserData::s_primitives[3] = {0, 0, 0};

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
    initializeGPUResources();
    if (s_isValid)
        return new CArnoldAreaLightUserData(objPath);
    else return 0;
}

MHWRender::DrawAPI CArnoldAreaLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL); // | MHWRender::kDirectX11); TODO support dx11 later
}

void CArnoldAreaLightDrawOverride::clearGPUResources()
{
    glDeleteShader(s_vertexShader);
    glDeleteShader(s_fragmentShader);
    glDeleteProgram(s_program);
    if (s_isValid)
    {
        delete CArnoldAreaLightUserData::s_primitives[0];
        delete CArnoldAreaLightUserData::s_primitives[1];
        delete CArnoldAreaLightUserData::s_primitives[2];
    }
    s_isInitialized = false;
    s_isValid = false;
}

void CArnoldAreaLightDrawOverride::initializeGPUResources()
{
    if (s_isInitialized == false)
    {
        s_isInitialized = true;
        s_isValid = false;

        if (!GLEW_VERSION_4_3)
            return; // right now, only opengl 4.3, we can lower this later       

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

        CArnoldAreaLightUserData::s_primitives[0] = new CGLQuadLightPrimitive();
        CArnoldAreaLightUserData::s_primitives[1] = new CGLDiskLightPrimitive();
        CArnoldAreaLightUserData::s_primitives[2] = new CGLCylinderPrimitive();

        s_modelLoc = glGetUniformLocation(s_program, "model");
        s_viewProjLoc = glGetUniformLocation(s_program, "viewProj");
        s_shadeColorLoc = glGetUniformLocation(s_program, "shadeColor");

        s_isValid = true;
    }
}

void CArnoldAreaLightDrawOverride::draw(
                                    const MHWRender::MDrawContext& context,
                                    const MUserData* data)
{
    if (s_isValid == false)
        return;
    const CArnoldAreaLightUserData* userData = reinterpret_cast<const CArnoldAreaLightUserData*>(data);        
    glUseProgram(s_program);

    glUniformMatrix4fv(s_modelLoc, 1, GL_FALSE, &userData->m_modelMatrix[0][0]);
    float mat[4][4];
    context.getMatrix(MHWRender::MDrawContext::kViewProjMtx).get(mat);
    glUniformMatrix4fv(s_viewProjLoc, 1, GL_FALSE, &mat[0][0]);
    glUniform4f(s_shadeColorLoc, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
        userData->m_wireframeColor[2], userData->m_wireframeColor[3]);
    
    userData->p_primitive->draw();
    
    glUseProgram(0);
}
