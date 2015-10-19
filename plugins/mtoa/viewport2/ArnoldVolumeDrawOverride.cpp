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
CDXConstantBuffer* CArnoldVolumeDrawOverride::s_pDXConstantBuffer = 0;
DXShader* CArnoldVolumeDrawOverride::s_pDXShader = 0;
#endif

GLuint CArnoldVolumeDrawOverride::s_vertexShader = 0;
GLuint CArnoldVolumeDrawOverride::s_fragmentShader = 0;
GLuint CArnoldVolumeDrawOverride::s_program = 0;

GLint CArnoldVolumeDrawOverride::s_modelViewProjLoc = 0;
GLint CArnoldVolumeDrawOverride::s_scaleLoc = 0;
GLint CArnoldVolumeDrawOverride::s_offsetLoc = 0;
GLint CArnoldVolumeDrawOverride::s_shadeColorLoc = 0;

CGPUPrimitive* CArnoldVolumeDrawOverride::s_pPrimitive = 0;

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

   return MBoundingBox(minCoords, maxCoords);
}

bool CArnoldVolumeDrawOverride::disableInternalBoundingBoxDraw() const
{
    return true;
}

struct SArnoldVolumeUserData : public MUserData{
    float m_wireframeColor[4];
    float m_scale[4];
    float m_offset[4];

    SArnoldVolumeUserData() : MUserData(false) {}
    
    void update(const MDagPath& objPath)
    {
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        m_wireframeColor[0] = color.r;
        m_wireframeColor[1] = color.g;
        m_wireframeColor[2] = color.b;
        m_wireframeColor[3] = color.a;

        AtVector mn = {
            -1.0f, -1.0f, -1.0f
        };

        AtVector mx = {
            1.0f, 1.0f, 1.0f
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
    {
        SArnoldVolumeUserData* data = reinterpret_cast<SArnoldVolumeUserData*>(oldData);
        if (!data)
            data = new SArnoldVolumeUserData();
        data->update(objPath);
        return data;
    }
    else return 0;
}

MHWRender::DrawAPI CArnoldVolumeDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kAllDevices);
}

void CArnoldVolumeDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{    
    if (!s_isValid)
        return;
    const SArnoldVolumeUserData* userData = reinterpret_cast<const SArnoldVolumeUserData*>(data);

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

void CArnoldVolumeDrawOverride::initializeGPUResources()
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

void CArnoldVolumeDrawOverride::clearGPUResources()
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
