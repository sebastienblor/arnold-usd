#include "ArnoldPhotometricLightDrawOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>
#include <maya/M3dView.h>
#include <maya/MUIDrawManager.h>

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

#ifdef _WIN32
#pragma pack(1)
    struct SConstantBuffer{
        float wvp[4][4];
        float color[4];
    };
#pragma pack()
#endif
}

GLuint CArnoldPhotometricLightDrawOverride::s_vertexShader = 0;
GLuint CArnoldPhotometricLightDrawOverride::s_fragmentShader = 0;
GLuint CArnoldPhotometricLightDrawOverride::s_program = 0;

GLint CArnoldPhotometricLightDrawOverride::s_modelViewProjLoc = 0;
GLint CArnoldPhotometricLightDrawOverride::s_shadeColorLoc = 0;

CGPUPrimitive* CArnoldPhotometricLightDrawOverride::sp_primitive = 0;

bool CArnoldPhotometricLightDrawOverride::s_isValid = false;
bool CArnoldPhotometricLightDrawOverride::s_isInitialized = false;

#ifdef _WIN32
CDXConstantBuffer* CArnoldPhotometricLightDrawOverride::s_pDXConstantBuffer = 0;
DXShader* CArnoldPhotometricLightDrawOverride::s_pDXShader = 0;
#endif

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

    SArnoldPhotometricLightUserData() : MUserData(false) { }
    
    void update(const MDagPath& objPath)
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
    {
        SArnoldPhotometricLightUserData* data = reinterpret_cast<SArnoldPhotometricLightUserData*>(oldData);
        if (!data)
            data = new SArnoldPhotometricLightUserData();
        data->update(objPath);
        return data;
    }
    else return 0;
}

MHWRender::DrawAPI CArnoldPhotometricLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kAllDevices);
}

void CArnoldPhotometricLightDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{    
    if (!s_isValid)
        return;
    if ((M3dView::active3dView().objectDisplay() & M3dView::kDisplayLights) == 0)
        return;
    const SArnoldPhotometricLightUserData* userData = reinterpret_cast<const SArnoldPhotometricLightUserData*>(data);

    if (userData == 0)
        return;

    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();

    if (theRenderer->drawAPIIsOpenGL())
    {
        glUseProgram(s_program);

        float mat[4][4]; // load everything in one go, using one continous glUniformfv call
        context.getMatrix(MHWRender::MDrawContext::kWorldViewProjMtx).get(mat);
        glUniformMatrix4fv(s_modelViewProjLoc, 1, GL_FALSE, &mat[0][0]);
        glUniform4f(s_shadeColorLoc, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
                userData->m_wireframeColor[2], userData->m_wireframeColor[3]);
        
        sp_primitive->draw();

        glUseProgram(0);
    }
    else
    {
#ifdef _WIN32
        ID3D11Device* device = reinterpret_cast<ID3D11Device*>(theRenderer->GPUDeviceHandle());
        if (!device)
            return;
        ID3D11DeviceContext* dxContext = 0;
        device->GetImmediateContext(&dxContext);
        if (!dxContext)
            return;

        s_pDXShader->setShader(dxContext);

        // filling up constant buffer
        SConstantBuffer buffer;
        context.getMatrix(MHWRender::MDrawContext::kWorldViewProjMtx).transpose().get(buffer.wvp);
        buffer.color[0] = userData->m_wireframeColor[0];
        buffer.color[1] = userData->m_wireframeColor[1];
        buffer.color[2] = userData->m_wireframeColor[2];
        buffer.color[3] = userData->m_wireframeColor[3];

        s_pDXConstantBuffer->update(dxContext, &buffer);
        s_pDXConstantBuffer->set(dxContext);
        sp_primitive->draw(dxContext);
#endif
    }
}

void CArnoldPhotometricLightDrawOverride::initializeGPUResources()
{
    if ((s_isInitialized == false))
    {
        

        MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
        s_isInitialized = true;
        s_isValid = false;

        if (theRenderer->drawAPIIsOpenGL() && InitializeGLEW())
        {
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

            sp_primitive = CGPhotometricLightPrimitive::generate(new CGLPrimitive());

            s_modelViewProjLoc = glGetUniformLocation(s_program, "modelViewProj");
            s_shadeColorLoc = glGetUniformLocation(s_program, "shadeColor");
        }
        else
        {
#ifdef _WIN32
            ID3D11Device* device = reinterpret_cast<ID3D11Device*>(theRenderer->GPUDeviceHandle());
            if (!device)
                return;
            ID3D11DeviceContext* dxContext = 0;
            device->GetImmediateContext(&dxContext);
            if (!dxContext)
                return;

            s_pDXConstantBuffer = new CDXConstantBuffer(device, sizeof(SConstantBuffer));
            if (!s_pDXConstantBuffer->isValid())
                return;

            s_pDXShader = new DXShader(device, "photometricLight");
            if (!s_pDXShader->isValid())
                return;

            sp_primitive = CGPhotometricLightPrimitive::generate(new CDXPrimitive(device));

            if (!reinterpret_cast<CDXPrimitive*>(sp_primitive)->createInputLayout(s_pDXShader->getVertexShaderBlob()))
                return;
#endif
        }
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

#ifdef _WIN32
        if (s_pDXConstantBuffer)
        {
            delete s_pDXConstantBuffer;
            s_pDXConstantBuffer = 0;
        }
        if (s_pDXShader)
        {
            delete s_pDXShader;
            s_pDXShader = 0;
        }
#endif
    }
}

void CArnoldPhotometricLightDrawOverride::addUIDrawables(const MDagPath& objPath,
												MHWRender::MUIDrawManager& drawManager,
												const MHWRender::MFrameContext& frameContext,
												const MUserData* data)
{
}
