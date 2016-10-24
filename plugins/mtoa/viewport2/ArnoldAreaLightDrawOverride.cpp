
#include "ArnoldAreaLightDrawOverride.h"

#include <iostream>
#include <vector>

#include "ViewportUtils.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/M3dView.h>

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

GLuint CArnoldAreaLightDrawOverride::s_vertexShader = 0;
GLuint CArnoldAreaLightDrawOverride::s_fragmentShader = 0;
GLuint CArnoldAreaLightDrawOverride::s_program = 0;

GLint CArnoldAreaLightDrawOverride::s_modelLoc = 0;
GLint CArnoldAreaLightDrawOverride::s_viewProjLoc = 0;
GLint CArnoldAreaLightDrawOverride::s_shadeColorLoc = 0;

#ifdef _WIN32
CDXConstantBuffer* CArnoldAreaLightDrawOverride::s_pDXConstantBuffer = 0;
DXShader* CArnoldAreaLightDrawOverride::s_pDXShader = 0;
#endif
#endif

bool CArnoldAreaLightDrawOverride::s_isValid = false;
bool CArnoldAreaLightDrawOverride::s_isInitialized = false;

#if MAYA_API_VERSION >= 201700
// TODO check about delete after use, and
// how to reuse buffers, rather than always
// recreating them, this might won't cause
// much performance problems, but cleaner,
// the better
struct CArnoldAreaLightUserData : public MUserData{
   MString m_primitiveType;
   double m_scale[3];
   MPointArray p_positions;
   MUintArray p_indices;
   float m_wireframeColor[4];

   CArnoldAreaLightUserData() : MUserData(false) 
   {
      m_scale[0] = m_scale[1] = 1.0;
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

      MString primitiveType = "quad";
      double scale2[3] = { 1.0, 1.0, 1.0 };

      MFnDependencyNode depNode(objPath.node());		
      MStatus status;
      MPlug plug = depNode.findPlug("aiTranslator", &status);
      MTransformationMatrix modelMatrix(objPath.inclusiveMatrix());
      if (status && !plug.isNull())
      {
         primitiveType = plug.asString();
         if (primitiveType.length() == 0)
            primitiveType = "quad";
      }
      bool primitiveTypeChanged = (m_primitiveType != primitiveType);

      // Change in primitive type
      if (primitiveType == "disk")
      {
         double scale[3];
         modelMatrix.getScale(scale, MSpace::kWorld);
         if (scale[0] != scale[1]) // non uniform scaling across x and y
         {
            if (scale[0] != 0.0)
               scale2[0] /= scale[0];
            if (scale[1] != 0)
               scale2[1] /= scale[1];
            const double avs = (scale[0] + scale[1]) * 0.5;
            scale2[0] *= avs;
            scale2[1] *= avs;
         }

         if (primitiveTypeChanged || 
            scale2[0] != m_scale[0] ||
            scale2[1] != m_scale[1] ||
            scale2[2] != m_scale[2])
         {
            CGDiskLightPrimitive::generateData(p_positions, p_indices, scale2);

            m_scale[0] = scale2[0];
            m_scale[1] = scale2[1];
            m_scale[2] = scale2[2];
         }
         m_primitiveType = "disk";
      }
      else if (primitiveType == "cylinder")
      {
         double scale[3];
         modelMatrix.getScale(scale, MSpace::kWorld);
         if (scale[0] != scale[2]) // non uniform scaling across x and y
         {
            if (scale[0] != 0.0)
               scale2[0] /= scale[0];
            if (scale[2] != 0)
               scale2[2] /= scale[2];
            const double avs = (scale[0] + scale[2]) * 0.5;
            scale2[0] *= avs;
            scale2[2] *= avs;
         }

         if (primitiveTypeChanged ||
            scale2[0] != m_scale[0] ||
            scale2[1] != m_scale[1] ||
            scale2[2] != m_scale[2])
         {
            CGCylinderPrimitive::generateData(p_positions, p_indices, scale2);

            m_scale[0] = scale2[0];
            m_scale[1] = scale2[1];
            m_scale[2] = scale2[2];
         }
         m_primitiveType = "cylinder";			
      }
      else
      {
         if (primitiveTypeChanged)
         {
            CGQuadLightPrimitive::generateData(p_positions, p_indices, scale2);
            m_primitiveType = "quad";			
         }
      }
   }

   ~CArnoldAreaLightUserData()
   {
   }
};

#else
// TODO check about delete after use, and
// how to reuse buffers, rather than always
// recreating them, this might won't cause
// much performance problems, but cleaner,
// the better
struct CArnoldAreaLightUserData : public MUserData{
   static CGPUPrimitive* s_primitives[3];
   CGPUPrimitive* p_primitive;
   MMatrix m_modelMatrix;
   float m_color[4];
   float m_wireframeColor[4];

   CArnoldAreaLightUserData() : MUserData(false) { }

   void update(const MDagPath& objPath)
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

      m_modelMatrix = modelMatrix.asMatrix();
   }

   ~CArnoldAreaLightUserData()
   {
   }
};

CGPUPrimitive* CArnoldAreaLightUserData::s_primitives[3] = {0, 0, 0};
#endif

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
#if MAYA_API_VERSION >= 201700
   initializeUserData();
#else
   initializeGPUResources();
#endif

   if (s_isValid)
   {
      CArnoldAreaLightUserData* data = reinterpret_cast<CArnoldAreaLightUserData*>(oldData);
      if (!data)
         data = new CArnoldAreaLightUserData();
      data->update(objPath);
      return data;
   }
   else return 0;
}

MHWRender::DrawAPI CArnoldAreaLightDrawOverride::supportedDrawAPIs() const
{
   return (MHWRender::kAllDevices);
}

#if MAYA_API_VERSION >= 201700
// Generate user data for each primitive type
void CArnoldAreaLightDrawOverride::initializeUserData()
{
   if (s_isInitialized == false)
   {
      s_isInitialized = true;
      s_isValid = true;
   }
}
#else
void CArnoldAreaLightDrawOverride::clearGPUResources()
{
   if (s_isInitialized && InitializeGLEW())
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

void CArnoldAreaLightDrawOverride::initializeGPUResources()
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

         CArnoldAreaLightUserData::s_primitives[0] = CGQuadLightPrimitive::generate(new CGLPrimitive());
         CArnoldAreaLightUserData::s_primitives[1] = CGDiskLightPrimitive::generate(new CGLPrimitive());
         CArnoldAreaLightUserData::s_primitives[2] = CGCylinderPrimitive::generate(new CGLPrimitive());

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

         CArnoldAreaLightUserData::s_primitives[0] = CGQuadLightPrimitive::generate(new CDXPrimitive(device));
         CArnoldAreaLightUserData::s_primitives[1] = CGDiskLightPrimitive::generate(new CDXPrimitive(device));
         CArnoldAreaLightUserData::s_primitives[2] = CGCylinderPrimitive::generate(new CDXPrimitive(device));
         if (!reinterpret_cast<CDXPrimitive*>(CArnoldAreaLightUserData::s_primitives[0])->createInputLayout(s_pDXShader->getVertexShaderBlob()))
            return;
         if (!reinterpret_cast<CDXPrimitive*>(CArnoldAreaLightUserData::s_primitives[1])->createInputLayout(s_pDXShader->getVertexShaderBlob()))
            return;
         if (!reinterpret_cast<CDXPrimitive*>(CArnoldAreaLightUserData::s_primitives[2])->createInputLayout(s_pDXShader->getVertexShaderBlob()))
            return;

         s_isValid = true;
#endif
      }        

      s_isValid = true;
   }
}
#endif

void CArnoldAreaLightDrawOverride::draw(
   const MHWRender::MDrawContext& context,
   const MUserData* data)
{
#if MAYA_API_VERSION < 201700
   if (s_isValid == false)
      return;
   if ((M3dView::active3dView().objectDisplay() & M3dView::kDisplayLights) == 0)
      return;
   const CArnoldAreaLightUserData* userData = reinterpret_cast<const CArnoldAreaLightUserData*>(data);

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
      userData->p_primitive->draw();    
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
      userData->p_primitive->draw(dxContext);
#endif
   }    
#endif
}

#if MAYA_API_VERSION >= 201700
void CArnoldAreaLightDrawOverride::addUIDrawables(const MDagPath& objPath,
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
   const CArnoldAreaLightUserData* userData = reinterpret_cast<const CArnoldAreaLightUserData*>(data);
   if (userData == 0)
      return;

   drawManager.beginDrawable();
   drawManager.setColor( userData->m_wireframeColor );
   drawManager.mesh(MHWRender::MUIDrawManager::kLines, 
      userData->p_positions, NULL, NULL, &userData->p_indices );
   drawManager.endDrawable();
}
#endif
