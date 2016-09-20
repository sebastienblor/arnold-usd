
#include "ArnoldMeshLightDrawOverride.h"
#include "nodes/light/ArnoldMeshLightNode.h"

#include <iostream>
#include <vector>

#include "ViewportUtils.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/M3dView.h>
#include <maya/MFnMesh.h>

#if MAYA_API_VERSION >= 201700
#include <maya/MUIDrawManager.h>
#include <maya/MPointArray.h>
#include <maya/MUintArray.h>
#else
namespace {
   const char* shaderUniforms = "#version 120\n"
      "uniform mat4 model;\n"
      "uniform mat4 viewProj;\n"
      "uniform vec4 shadeColor;\n";

   const char* vertexShader = 
      "void main()\n"
      "{\n"
      "gl_Position = viewProj * (model * gl_Vertex);\n"
      "}\n";

   const char* fragmentShader =
      "void main() { gl_FragColor = shadeColor;}\n";

#ifdef _WIN32
#pragma pack(1)
   struct SConstantBuffer{
      float w[4][4];
      float vp[4][4];
      float color[4];
   };
#pragma pack()
#endif
}

GLuint CArnoldMeshLightDrawOverride::s_vertexShader = 0;
GLuint CArnoldMeshLightDrawOverride::s_fragmentShader = 0;
GLuint CArnoldMeshLightDrawOverride::s_program = 0;

GLint CArnoldMeshLightDrawOverride::s_modelLoc = 0;
GLint CArnoldMeshLightDrawOverride::s_viewProjLoc = 0;
GLint CArnoldMeshLightDrawOverride::s_shadeColorLoc = 0;

#ifdef _WIN32
CDXConstantBuffer* CArnoldMeshLightDrawOverride::s_pDXConstantBuffer = 0;
DXShader* CArnoldMeshLightDrawOverride::s_pDXShader = 0;
#endif
#endif

bool CArnoldMeshLightDrawOverride::s_isValid = false;
bool CArnoldMeshLightDrawOverride::s_isInitialized = false;

#if MAYA_API_VERSION >= 201700
// TODO check about delete after use, and
// how to reuse buffers, rather than always
// recreating them, this might won't cause
// much performance problems, but cleaner,
// the better
struct CArnoldMeshLightUserData : public MUserData
{
   MPointArray m_positions;
   MUintArray m_indices;
   float m_wireframeColor[4];

   CArnoldMeshLightUserData() : MUserData(false) 
   {
   }

   void update(const MDagPath& objPath)
   {
      // Update the wireframe color
      if (MHWRender::kDormant == MHWRender::MGeometryUtilities::displayStatus(objPath))
      {
         m_wireframeColor[0] = 0.75f;
         m_wireframeColor[1] = 0.f;
         m_wireframeColor[2] = 0.f;
         m_wireframeColor[3] = 0.2f;
      }
      else
      {
         MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
         m_wireframeColor[0] = color.r;
         m_wireframeColor[1] = color.g;
         m_wireframeColor[2] = color.b;
         m_wireframeColor[3] = color.a;
      }

      MFnDependencyNode fnNode(objPath.node());
      MPxNode* userNode = fnNode.userNode();
      CArnoldMeshLightNode* meshLightNode = static_cast<CArnoldMeshLightNode*>(userNode);

      if (meshLightNode->m_vp2GeometryUpdate)
      {
         meshLightNode->m_vp2GeometryUpdate = false;

         MObject meshObj = meshLightNode->GetMeshObject();
         if (meshObj != MObject::kNullObj)
         {
            MFnMesh mesh(meshObj);

            mesh.getPoints(m_positions);

            const int numEdges = mesh.numEdges();
            m_indices.setLength(numEdges * 2);
            int2 edgeVertexIndices;
            for (int i=0, j=0; i<numEdges; ++i)
            {
               mesh.getEdgeVertices(i, edgeVertexIndices);
               m_indices[j++] = edgeVertexIndices[0];
               m_indices[j++] = edgeVertexIndices[1];
            }
         }
         else
         {
            m_positions.setLength(0);
            m_indices.setLength(0);
         }
      }
   }

   ~CArnoldMeshLightUserData()
   {
   }
};

#else

// TODO check about delete after use, and
// how to reuse buffers, rather than always
// recreating them, this might won't cause
// much performance problems, but cleaner,
// the better
struct CArnoldMeshLightUserData : public MUserData
{
   CGPUPrimitive* m_primitive;
   std::vector<float> m_positions;
   std::vector<unsigned int> m_indices;
   MMatrix m_modelMatrix;
   float m_wireframeColor[4];

   CArnoldMeshLightUserData(CGPUPrimitive* primitive)
      : MUserData(false)
      , m_primitive(primitive)
   {
   }

   void update(const MDagPath& objPath)
   {
      // Update the wireframe color
      if (MHWRender::kDormant == MHWRender::MGeometryUtilities::displayStatus(objPath))
      {
         m_wireframeColor[0] = 0.75f;
         m_wireframeColor[1] = 0.f;
         m_wireframeColor[2] = 0.f;
         m_wireframeColor[3] = 0.2f;
      }
      else
      {
         MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
         m_wireframeColor[0] = color.r;
         m_wireframeColor[1] = color.g;
         m_wireframeColor[2] = color.b;
         m_wireframeColor[3] = color.a;
      }

      MFnDependencyNode fnNode(objPath.node());
      MPxNode* userNode = fnNode.userNode();
      CArnoldMeshLightNode* meshLightNode = static_cast<CArnoldMeshLightNode*>(userNode);

      if (meshLightNode->m_vp2GeometryUpdate)
      {
         meshLightNode->m_vp2GeometryUpdate = false;

         MObject meshObj = meshLightNode->GetMeshObject();
         if (meshObj != MObject::kNullObj)
         {
            MStatus status;
            MFnMesh mesh(meshObj);

            const int numVertices = mesh.numVertices();
            m_positions.resize(numVertices * 3);
            float* positionData = &m_positions[0];
            memcpy(positionData, mesh.getRawPoints(&status), numVertices * 3 * sizeof(float));

            const int numEdges = mesh.numEdges();
            m_indices.resize(numEdges * 2);
            int2 edgeVertexIndices;
            for (int i=0, j=0; i<numEdges; ++i)
            {
               mesh.getEdgeVertices(i, edgeVertexIndices);
               m_indices[j++] = edgeVertexIndices[0];
               m_indices[j++] = edgeVertexIndices[1];
            }

            m_primitive->setPrimitiveData(positionData, numVertices*3, &m_indices[0], numEdges*2);
         }
         else
         {
            m_primitive->setPrimitiveData(NULL, 0, NULL, 0);
         }
      }

      MTransformationMatrix modelMatrix(objPath.inclusiveMatrix());
      m_modelMatrix = modelMatrix.asMatrix();
   }

   ~CArnoldMeshLightUserData()
   {
      delete m_primitive;
   }
};

#endif

MHWRender::MPxDrawOverride* CArnoldMeshLightDrawOverride::creator(const MObject& obj)
{
   return new CArnoldMeshLightDrawOverride(obj);
}

CArnoldMeshLightDrawOverride::CArnoldMeshLightDrawOverride(const MObject& obj) :
   MHWRender::MPxDrawOverride(obj, draw)
{
}

CArnoldMeshLightDrawOverride::~CArnoldMeshLightDrawOverride()
{
}

bool CArnoldMeshLightDrawOverride::isBounded(
   const MDagPath& objPath,
   const MDagPath& cameraPath) const
{
   return true;
}

MBoundingBox CArnoldMeshLightDrawOverride::boundingBox(
   const MDagPath& objPath,
   const MDagPath& cameraPath) const
{
   MFnDagNode fnNode(objPath);
   return fnNode.boundingBox();
}

bool CArnoldMeshLightDrawOverride::disableInternalBoundingBoxDraw() const
{
   return false;
}

MUserData* CArnoldMeshLightDrawOverride::prepareForDraw(
   const MDagPath& objPath,
   const MDagPath& cameraPath,
   const MHWRender::MFrameContext& frameContext,
   MUserData* oldData)
{
#if MAYA_API_VERSION >= 201700
   initializeUserData();
   if (s_isValid)
   {
      CArnoldMeshLightUserData* data = reinterpret_cast<CArnoldMeshLightUserData*>(oldData);
      if (!data) {
         data = new CArnoldMeshLightUserData();
      }
      data->update(objPath);
      return data;
   }
   return 0;
#else
   initializeGPUResources();
   if (s_isValid)
   {
      CArnoldMeshLightUserData* data = reinterpret_cast<CArnoldMeshLightUserData*>(oldData);
      if (!data)
      {
         CGPUPrimitive* primitive = NULL;
         MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
         if (theRenderer->drawAPIIsOpenGL())
         {
            primitive = new CGLPrimitive();
         }
         else
         {
#ifdef _WIN32
            ID3D11Device* device = reinterpret_cast<ID3D11Device*>(theRenderer->GPUDeviceHandle());
            CDXPrimitive* dxprim = new CDXPrimitive(device);
            dxprim->createInputLayout(s_pDXShader->getVertexShaderBlob());
            primitive = dxprim;
#endif
         }
         data = new CArnoldMeshLightUserData(primitive);
      }
      data->update(objPath);
      return data;
   }
   return 0;
#endif
}

MHWRender::DrawAPI CArnoldMeshLightDrawOverride::supportedDrawAPIs() const
{
   return (MHWRender::kAllDevices);
}

#if MAYA_API_VERSION >= 201700
// Generate user data for each primitive type
void CArnoldMeshLightDrawOverride::initializeUserData()
{
   if ((s_isInitialized == false))
   {
      s_isInitialized = true;
      s_isValid = true;
   }
}
#else
void CArnoldMeshLightDrawOverride::clearGPUResources()
{
   if (s_isInitialized && InitializeGLEW())
   {
      glDeleteShader(s_vertexShader);
      glDeleteShader(s_fragmentShader);
      glDeleteProgram(s_program);
      s_isInitialized = false;
      s_isValid = false;

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

void CArnoldMeshLightDrawOverride::initializeGPUResources()
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

         s_modelLoc = glGetUniformLocation(s_program, "model");
         s_viewProjLoc = glGetUniformLocation(s_program, "viewProj");
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

         s_pDXShader = new DXShader(device, "areaLight");
         if (!s_pDXShader->isValid())
            return;
#endif
      }

      s_isValid = true;
   }
}
#endif

void CArnoldMeshLightDrawOverride::draw(
   const MHWRender::MDrawContext& context,
   const MUserData* data)
{
#if MAYA_API_VERSION < 201700
   if (s_isValid == false)
      return;
   if ((M3dView::active3dView().objectDisplay() & M3dView::kDisplayLights) == 0)
      return;
   const CArnoldMeshLightUserData* userData = reinterpret_cast<const CArnoldMeshLightUserData*>(data);

   if (userData == 0)
      return;

   MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();

   if (theRenderer->drawAPIIsOpenGL())
   {
      float mat[4][4];

      glUseProgram(s_program);
      userData->m_modelMatrix.get(mat);
      glUniformMatrix4fv(s_modelLoc, 1, GL_FALSE, &mat[0][0]);
      context.getMatrix(MHWRender::MDrawContext::kViewProjMtx).get(mat);
      glUniformMatrix4fv(s_viewProjLoc, 1, GL_FALSE, &mat[0][0]);
      glUniform4f(s_shadeColorLoc, userData->m_wireframeColor[0], userData->m_wireframeColor[1],
         userData->m_wireframeColor[2], userData->m_wireframeColor[3]);    
      userData->m_primitive->draw();
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
      context.getMatrix(MHWRender::MDrawContext::kViewProjMtx).transpose().get(buffer.vp);
      userData->m_modelMatrix.transpose().get(buffer.w);
      buffer.color[0] = userData->m_wireframeColor[0];
      buffer.color[1] = userData->m_wireframeColor[1];
      buffer.color[2] = userData->m_wireframeColor[2];
      buffer.color[3] = userData->m_wireframeColor[3];

      s_pDXConstantBuffer->update(dxContext, &buffer);
      s_pDXConstantBuffer->set(dxContext);
      userData->m_primitive->draw(dxContext);
#endif
   }    
#endif
}

#if MAYA_API_VERSION >= 201700
void CArnoldMeshLightDrawOverride::addUIDrawables(const MDagPath& objPath,
                                                  MHWRender::MUIDrawManager& drawManager,
                                                  const MHWRender::MFrameContext& frameContext,
                                                  const MUserData* data)
{
   if (s_isValid == false)
      return;

   // Note that we don't need to add in a light type filter
   // since the classification string "drawdb/geometry/light" usage
   // will automatically do this for us.
   //
   const CArnoldMeshLightUserData* userData = reinterpret_cast<const CArnoldMeshLightUserData*>(data);
   if (userData == 0)
      return;

   drawManager.beginDrawable();
   drawManager.setColor( userData->m_wireframeColor );
   drawManager.mesh(MHWRender::MUIDrawManager::kLines, 
      userData->m_positions, NULL, NULL, &userData->m_indices );
   drawManager.endDrawable();
}
#endif
