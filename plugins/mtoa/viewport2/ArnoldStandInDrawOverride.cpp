#include "ArnoldStandInDrawOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>

#include <iostream>

#include <ai.h>

namespace{
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
        AtMatrix wvp;
        float scale[4];
        float offset[4];
        float color[4];
    };
#pragma pack()
#endif
}

#ifdef _WIN32
ID3D11Buffer* CArnoldStandInDrawOverride::s_pDXVertexBuffer = 0;
ID3D11Buffer* CArnoldStandInDrawOverride::s_pDXIndexBuffer = 0;
ID3D11VertexShader* CArnoldStandInDrawOverride::s_pDXVertexShader = 0;
ID3D11PixelShader* CArnoldStandInDrawOverride::s_pDXPixelShader = 0;
ID3D11InputLayout* CArnoldStandInDrawOverride::s_pDXVertexLayout = 0;
ID3D11Buffer* CArnoldStandInDrawOverride::s_pDXConstantBuffer = 0;
#endif

GLuint CArnoldStandInDrawOverride::s_vertexShader = 0;
GLuint CArnoldStandInDrawOverride::s_fragmentShader = 0;
GLuint CArnoldStandInDrawOverride::s_program = 0;

GLint CArnoldStandInDrawOverride::s_modelViewProjLoc = 0;
GLint CArnoldStandInDrawOverride::s_scaleLoc = 0;
GLint CArnoldStandInDrawOverride::s_offsetLoc = 0;
GLint CArnoldStandInDrawOverride::s_shadeColorLoc = 0;

GLuint CArnoldStandInDrawOverride::s_VBO = 0;
GLuint CArnoldStandInDrawOverride::s_IBO = 0;
GLuint CArnoldStandInDrawOverride::s_VAO = 0;

bool CArnoldStandInDrawOverride::s_isValid = false;
bool CArnoldStandInDrawOverride::s_isInitialized = false;


MHWRender::MPxDrawOverride* CArnoldStandInDrawOverride::creator(const MObject& obj)
{
    return new CArnoldStandInDrawOverride(obj);
}

CArnoldStandInDrawOverride::CArnoldStandInDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw)
{

}

CArnoldStandInDrawOverride::~CArnoldStandInDrawOverride()
{

}

bool CArnoldStandInDrawOverride::isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    return true;
}

MBoundingBox CArnoldStandInDrawOverride::boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    MFnDependencyNode depNode(objPath.node());

    bbox.expand(MPoint(
        depNode.findPlug("MinBoundingBox0").asFloat(),
        depNode.findPlug("MinBoundingBox1").asFloat(),
        depNode.findPlug("MinBoundingBox2").asFloat(), 1.0));

    bbox.expand(MPoint(
        depNode.findPlug("MaxBoundingBox0").asFloat(),
        depNode.findPlug("MaxBoundingBox1").asFloat(),
        depNode.findPlug("MaxBoundingBox2").asFloat(), 1.0));
    
    return bbox;
}

bool CArnoldStandInDrawOverride::disableInternalBoundingBoxDraw() const
{
    return true;
}

struct SArnoldStandInUserData : public MUserData{
    float m_wireframeColor[4];
    float m_scale[4];
    float m_offset[4];
    
    SArnoldStandInUserData(const MDagPath& objPath) : MUserData(true)
    {
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        m_wireframeColor[0] = color.r;
        m_wireframeColor[1] = color.g;
        m_wireframeColor[2] = color.b;
        m_wireframeColor[3] = color.a;

        MFnDependencyNode depNode(objPath.node());
        AtVector mn = {
            depNode.findPlug("MinBoundingBox0").asFloat(),
            depNode.findPlug("MinBoundingBox1").asFloat(),
            depNode.findPlug("MinBoundingBox2").asFloat()
        };

        AtVector mx = {
            depNode.findPlug("MaxBoundingBox0").asFloat(),
            depNode.findPlug("MaxBoundingBox1").asFloat(),
            depNode.findPlug("MaxBoundingBox2").asFloat()
        };

        m_scale[0] = mx.x - mn.x;
        m_scale[1] = mx.y - mn.y;
        m_scale[2] = mx.z - mn.z;
        m_scale[3] = 1.0f;

        m_offset[0] = mn.x;
        m_offset[1] = mn.y;
        m_offset[2] = mn.z;
        m_offset[3] = 0.0f;
    }

    ~SArnoldStandInUserData()
    {
    }
};


MUserData* CArnoldStandInDrawOverride::prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData)
{
    initializeGPUResources();
    if (s_isValid)
        return new SArnoldStandInUserData(objPath);
    else return 0;
}

MHWRender::DrawAPI CArnoldStandInDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL | MHWRender::kDirectX11);
}

void CArnoldStandInDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{    
    if (!s_isValid)
        return;
    const SArnoldStandInUserData* userData = reinterpret_cast<const SArnoldStandInUserData*>(data);

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
        glBindVertexArray(s_VAO);

        glDrawElements(GL_LINES, 3 * 4 * 2, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);
    }
}

void CArnoldStandInDrawOverride::initializeGPUResources()
{    
    if (s_isInitialized == false)
    {
        MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
        s_isInitialized = true;
        s_isValid = false;

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

        unsigned int indices[3 * 4 * 2] = {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7
        };

        if (theRenderer->drawAPIIsOpenGL())
        {
            if (!GLEW_VERSION_2_1)
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

            glGenBuffers(1, &s_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
            glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

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

            HRESULT hr;
            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));
            D3D11_SUBRESOURCE_DATA initData;
            ZeroMemory(&initData, sizeof(initData));

            bd.Usage = D3D11_USAGE_IMMUTABLE;
            bd.ByteWidth = 8 * 3 * sizeof(float);
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.CPUAccessFlags = 0;
            initData.pSysMem = vertices;
            hr = device->CreateBuffer(&bd, &initData, &s_pDXVertexBuffer);
            if (FAILED(hr)) return;

            bd.ByteWidth = 4 * 3 * 2 * sizeof(unsigned int);
            bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
            initData.pSysMem = indices;
            hr = device->CreateBuffer(&bd, &initData, &s_pDXIndexBuffer);
            if (FAILED(hr)) return;

            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(SConstantBuffer);
            bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            hr = device->CreateBuffer(&bd, 0, &s_pDXConstantBuffer);
            if (FAILED(hr)) return;

            DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
            ID3DBlob* vertexShaderBlob = 0;
            ID3DBlob* pixelShaderBlob = 0;
            ID3DBlob* errorBlob = 0;
            MString effectLocation = MString(getenv("MTOA_PATH")) + MString("\\vp2\\standInBBox.hlsl");

            hr = D3DCompileFromFile(
                (LPCWSTR)effectLocation.asChar(),
                0,
                0,
                "mainVS",
                "vs_5_0",
                0,
                0,
                &vertexShaderBlob,
                &errorBlob);
            if (FAILED(hr))
            {
                if (errorBlob) errorBlob->Release();
                return;  
            }
            if (errorBlob) errorBlob->Release();
            hr = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), 0, &s_pDXVertexShader);
            if (FAILED(hr))
            {
                vertexShaderBlob->Release();
                return;
            }

            hr = D3DCompileFromFile(
                (LPCWSTR)effectLocation.asChar(),
                0,
                0,
                "mainPS",
                "ps_5_0",
                0,
                0,
                &pixelShaderBlob,
                &errorBlob);
            if (FAILED(hr))
            {
                if (errorBlob) errorBlob->Release();
                return;  
            } 

            hr = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), 0, &s_pDXPixelShader);
            if (FAILED(hr))
            {
                pixelShaderBlob->Release();
                if (vertexShaderBlob) vertexShaderBlob->Release();
                if (errorBlob) errorBlob->Release();
                return;
            }

            std::cerr << "Hooray\n";
#endif
        }
    }
}

void CArnoldStandInDrawOverride::clearGPUResources()
{
    if (s_isInitialized)
    {
        glDeleteBuffers(1, &s_VBO);
        glDeleteBuffers(1, &s_IBO);
        glDeleteVertexArrays(1, &s_VAO);
        glDeleteShader(s_vertexShader);
        glDeleteShader(s_fragmentShader);
        glDeleteProgram(s_program);
        s_isValid = false;
        s_isInitialized = false;

#ifdef _WIN32
        if (s_pDXVertexBuffer)
        {
            s_pDXVertexBuffer->Release();
            s_pDXVertexBuffer = 0;
        }
        if (s_pDXIndexBuffer)
        {
            s_pDXIndexBuffer->Release();
            s_pDXIndexBuffer = 0;
        }
        if (s_pDXVertexShader)
        {
            s_pDXVertexShader->Release();
            s_pDXVertexShader = 0;
        }
        if (s_pDXPixelShader)
        {
            s_pDXPixelShader->Release();
            s_pDXPixelShader = 0;
        }
        if (s_pDXVertexLayout)
        {
            s_pDXVertexLayout->Release();
            s_pDXVertexLayout = 0;
        }
        if (s_pDXConstantBuffer)
        {
            s_pDXConstantBuffer->Release();
            s_pDXConstantBuffer = 0;
        }
#endif
    }
}
