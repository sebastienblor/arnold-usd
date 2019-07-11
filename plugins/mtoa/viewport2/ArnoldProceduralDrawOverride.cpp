#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "ArnoldProceduralDrawOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>

#include <iostream>

#include <ai.h>

#include "utils/MayaUtils.h"
#include "ViewportUtils.h"

namespace {
    const char* shaderUniforms = "#version 120\n"
"uniform mat4 modelViewProj;\n"
"uniform vec4 scale;"
"uniform vec4 offset;"
"uniform vec4 shadeColor;\n";

    const char* vertexShader =
"void main()\n"
"{\n"
"gl_Position = modelViewProj * vec4(gl_Vertex.xyz * scale.xyz + offset.xyz, 1.0f);\n"
"}\n";

    const char* fragmentShader =
"void main() { gl_FragColor = shadeColor;}\n";

#ifdef _WIN32
#pragma pack(1)
    struct SConstantBuffer{
        float wvp[4][4];
        float scale[4];
        float offset[4];
        float color[4];
    };
#pragma pack()
#endif
}

#ifdef _WIN32
CDXConstantBuffer* CArnoldProceduralDrawOverride::s_pDXConstantBuffer = 0;
DXShader* CArnoldProceduralDrawOverride::s_pDXShader = 0;
#endif

GLuint CArnoldProceduralDrawOverride::s_vertexShader = 0;
GLuint CArnoldProceduralDrawOverride::s_fragmentShader = 0;
GLuint CArnoldProceduralDrawOverride::s_program = 0;

GLint CArnoldProceduralDrawOverride::s_modelViewProjLoc = 0;
GLint CArnoldProceduralDrawOverride::s_scaleLoc = 0;
GLint CArnoldProceduralDrawOverride::s_offsetLoc = 0;
GLint CArnoldProceduralDrawOverride::s_shadeColorLoc = 0;

CGPUPrimitive* CArnoldProceduralDrawOverride::s_pPrimitive = 0;

bool CArnoldProceduralDrawOverride::s_isValid = false;
bool CArnoldProceduralDrawOverride::s_isInitialized = false;


MHWRender::MPxDrawOverride* CArnoldProceduralDrawOverride::creator(const MObject& obj)
{
    return new CArnoldProceduralDrawOverride(obj);
}

CArnoldProceduralDrawOverride::CArnoldProceduralDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw)
{

}

CArnoldProceduralDrawOverride::~CArnoldProceduralDrawOverride()
{

}

bool CArnoldProceduralDrawOverride::isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    return true;
}

MBoundingBox CArnoldProceduralDrawOverride::boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    MFnDependencyNode depNode(objPath.node());

    MPlug minPlug = depNode.findPlug("minBoundingBox", true);
    MPlug maxPlug = depNode.findPlug("maxBoundingBox", true);

    if (minPlug.isNull() || maxPlug.isNull())
    {
        bbox.expand(MPoint(1.f, 1.f, 1.f));
        bbox.expand(MPoint(-1.f, -1.f, -1.f));
    } else
    {
        bbox.expand(MPoint(minPlug.child(0).asFloat(), minPlug.child(1).asFloat(), minPlug.child(2).asFloat()));
        bbox.expand(MPoint(maxPlug.child(0).asFloat(), maxPlug.child(1).asFloat(), maxPlug.child(2).asFloat()));
    }   
    return bbox;
}

bool CArnoldProceduralDrawOverride::disableInternalBoundingBoxDraw() const
{
    return true;
}

struct SArnoldProceduralUserData : public MUserData{
    float m_wireframeColor[4];
    float m_scale[4];
    float m_offset[4];
    
    SArnoldProceduralUserData() : MUserData(false) {}

    void Update(const MDagPath& objPath)
    {
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        m_wireframeColor[0] = color.r;
        m_wireframeColor[1] = color.g;
        m_wireframeColor[2] = color.b;
        m_wireframeColor[3] = color.a;

        MFnDependencyNode depNode(objPath.node());
        MPlug minPlug = depNode.findPlug("minBoundingBox", true);
        AtVector mn (-1.f, -1.f, -1.f);
        if (!minPlug.isNull())
        {
            mn.x = minPlug.child(0).asFloat();
            mn.y = minPlug.child(1).asFloat();
            mn.z = minPlug.child(2).asFloat();
        }

        AtVector mx (1.f, 1.f, 1.f);
        MPlug maxPlug = depNode.findPlug("maxBoundingBox", true);
        if (!maxPlug.isNull())
        {
            mx.x = maxPlug.child(0).asFloat();
            mx.y = maxPlug.child(1).asFloat();
            mx.z = maxPlug.child(2).asFloat();
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

    ~SArnoldProceduralUserData()
    {
    }
};


MUserData* CArnoldProceduralDrawOverride::prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData)
{
    initializeGPUResources();
    if (s_isValid)
    {
        SArnoldProceduralUserData* data = reinterpret_cast<SArnoldProceduralUserData*>(oldData);
        if (!data)
            data = new SArnoldProceduralUserData();
        data->Update(objPath);
        return data;
    }
    else return 0;
}

MHWRender::DrawAPI CArnoldProceduralDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kAllDevices);
}

void CArnoldProceduralDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{    
    if (!s_isValid)
        return;
    const SArnoldProceduralUserData* userData = reinterpret_cast<const SArnoldProceduralUserData*>(data);
    if (userData == 0)
        return;

    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();

    if (theRenderer->drawAPIIsOpenGL())
    {
        glUseProgram(s_program);

        float mat[4][4]; // load everything in one go, using one continous glUniformfv call
        context.getMatrix(MHWRender::MDrawContext::kWorldViewProjMtx).get(mat);
        glUniformMatrix4fv(s_modelViewProjLoc, 1, GL_FALSE, &mat[0][0]);
        glUniform4f(s_scaleLoc, userData->m_scale[0], userData->m_scale[1], userData->m_scale[2], userData->m_scale[3]);
        glUniform4f(s_offsetLoc, userData->m_offset[0], userData->m_offset[1], userData->m_offset[2], userData->m_offset[3]);
        glUniform4f(s_shadeColorLoc, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
                userData->m_wireframeColor[2], userData->m_wireframeColor[3]);

        s_pPrimitive->draw();
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

        // setting up shader
        s_pDXShader->setShader(dxContext);

        // filling up constant buffer
        SConstantBuffer buffer;
        context.getMatrix(MHWRender::MDrawContext::kWorldViewProjMtx).transpose().get(buffer.wvp);
        buffer.scale[0] = userData->m_scale[0];
        buffer.scale[1] = userData->m_scale[1];
        buffer.scale[2] = userData->m_scale[2];
        buffer.scale[3] = userData->m_scale[3];

        buffer.offset[0] = userData->m_offset[0];
        buffer.offset[1] = userData->m_offset[1];
        buffer.offset[2] = userData->m_offset[2];
        buffer.offset[3] = userData->m_offset[3];
        
        buffer.color[0] = userData->m_wireframeColor[0];
        buffer.color[1] = userData->m_wireframeColor[1];
        buffer.color[2] = userData->m_wireframeColor[2];
        buffer.color[3] = userData->m_wireframeColor[3];

        s_pDXConstantBuffer->update(dxContext, &buffer);
        s_pDXConstantBuffer->set(dxContext);
        s_pPrimitive->draw(dxContext);
#endif
    }
}

void CArnoldProceduralDrawOverride::initializeGPUResources()
{    
    if (s_isInitialized == false)
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

            s_isValid = true;

            s_pPrimitive = CGBoxPrimitive::generate(new CGLPrimitive());

            s_modelViewProjLoc = glGetUniformLocation(s_program, "modelViewProj");
            s_scaleLoc = glGetUniformLocation(s_program, "scale");
            s_offsetLoc = glGetUniformLocation(s_program, "offset");
            s_shadeColorLoc = glGetUniformLocation(s_program, "shadeColor");
        }
        else
        {
#ifdef _WIN32
            ID3D11Device* device = reinterpret_cast<ID3D11Device*>(theRenderer->GPUDeviceHandle());
            if (!device)
                return;
            ID3D11DeviceContext* context = 0;
            device->GetImmediateContext(&context);
            if (!context)
                return;

            s_pDXConstantBuffer = new CDXConstantBuffer(device, sizeof(SConstantBuffer));
            if (!s_pDXConstantBuffer->isValid())
                return;
            s_pDXShader = new DXShader(device, "standInBBox");
            if (!s_pDXShader->isValid())
                return;
            s_pPrimitive = CGBoxPrimitive::generate(new CDXPrimitive(device));
            if (!reinterpret_cast<CDXPrimitive*>(s_pPrimitive)->createInputLayout(s_pDXShader->getVertexShaderBlob()))
                return;

            s_isValid = true;
#endif
        }
    }
}

void CArnoldProceduralDrawOverride::clearGPUResources()
{
    if (s_isInitialized && InitializeGLEW())
    {
        if (s_pPrimitive) delete s_pPrimitive;
        glDeleteShader(s_vertexShader);
        glDeleteShader(s_fragmentShader);
        glDeleteProgram(s_program);
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
