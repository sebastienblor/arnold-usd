#pragma once

#include <GL/glew.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#if _MSC_VER < 1700
#include <d3dx11.h>
#endif
#include <d3dcompiler.h>
#ifndef D3DCOMPILE_ENABLE_STRICTNESS
    #define D3DCOMPILE_ENABLE_STRICTNESS D3D10_SHADER_ENABLE_STRICTNESS
    #define D3DCOMPILE_DEBUG D3D10_SHADER_DEBUG
#endif
#endif

#include <vector>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MUintArray.h>
#include <maya/MObject.h>


class CLinePrimitiveData{
protected:
   CLinePrimitiveData() {}
public:
   virtual ~CLinePrimitiveData() {}
   std::vector<float> vertices;
   std::vector<unsigned int> indices;

   virtual void draw();
   virtual void update(const MObject& obj) {};
};

// use for static initialization
// These two classes could use fewer indices if we used
// glPrimitiveRestartIndex, but that's only available
// since OpenGL 3.1, and it doesn't worth the code duplication
class CDiskPrimitive : public CLinePrimitiveData{
public:
   CDiskPrimitive(unsigned int resolution = 20);
};

class CCylinderPrimitive : public CLinePrimitiveData{
public:
   CCylinderPrimitive(float radius = 1.0f, float height = 1.0f, unsigned int resolution = 20);
};

class CSpherePrimitive : public CLinePrimitiveData{
public:
   CSpherePrimitive(float radius = 1.0f, unsigned int resolution = 16);
};

class CBoxPrimitive : public CLinePrimitiveData{
public:
   CBoxPrimitive(float size = 1.0f);
};

class CPhotometricLightPrimitive : public CLinePrimitiveData{
public:
   CPhotometricLightPrimitive();
};

class CMeshPrimitive : public CLinePrimitiveData{
public:
   CMeshPrimitive() {}
   virtual void update(const MObject& obj);
};


class CGPUPrimitive {
protected:
   unsigned int m_numLineIndices;
   CGPUPrimitive() : m_numLineIndices(0) { }
public:
   virtual ~CGPUPrimitive() {}
   virtual void draw(void* platform = 0) const = 0; // passing platform specific data for dx

   virtual void setPrimitiveData(const float* vertices, unsigned int numVertices, const unsigned int* indices, unsigned int numIndices) = 0;
};

// TODO unify these two classes later, that can deal both
// with vbo and the old pipeline and static / dynamic initialization
// IBO has to contain both wireframe and triangle indices
class CGLPrimitive : public CGPUPrimitive {
protected:
   union{
      struct{
         unsigned int m_VBO;
         unsigned int m_IBO;
      };
      unsigned int m_GLBuffers[2];
   };   
public:
   CGLPrimitive();

   virtual ~CGLPrimitive();
   virtual void draw(void* platform = 0) const;

   virtual void setPrimitiveData(const float* vertices, unsigned int numVertices, const unsigned int* indices, unsigned int numIndices);
};

class CGQuadLightPrimitive{
public:
   static void generateData(MPointArray &positions, MUintArray &indices, double scale[3]);
   static CGPUPrimitive* generate(CGPUPrimitive* prim);
};

class CGDiskLightPrimitive{
public:
   static void generateData(MPointArray &positions, MUintArray &indices, double scale[3]);
   static CGPUPrimitive* generate(CGPUPrimitive* prim);
};

class CGCylinderPrimitive{
public:
   static void generateData(MPointArray &positions, MUintArray &indices, double scale[3]);
   static void generateData(MFloatVectorArray &positions, MUintArray &indices, double scale[3]);
   static CGPUPrimitive* generate(CGPUPrimitive* prim);
};

class CGPhotometricLightPrimitive{
public:
   static void generateData(MPointArray &positions, MUintArray &indices);
   static CGPUPrimitive* generate(CGPUPrimitive* prim);
};

class CGBoxPrimitive{
public:
   static void generateData(MPointArray &positions, MUintArray &indices, double scale[3], double offset[3], bool atOrigin=true);
   static void generateData(MFloatVectorArray &positions, MUintArray &indices, double scale[3], double offset[3], bool atOrigin=true);
   static CGPUPrimitive* generate(CGPUPrimitive* prim);
};

class CGSpherePrimitive{
public:
   static void generateData(MFloatVectorArray &positions, MUintArray &indices, double radius, unsigned int resolution=16);
   static void generateData(MPointArray &positions, MUintArray &indices, double radius, unsigned int resolution=16);
};

class CGQuadPrimitive{
public:
   static void generateData(MFloatVectorArray &positions, MUintArray &indices, double scale[3]);
   static void generateData(MPointArray &positions, MUintArray &indices, double scale[3]);
};

bool checkShaderError(unsigned int shader);
bool checkProgramError(unsigned int shader);

#ifdef _WIN32
#include <maya/MString.h>

// simple class to handle most of our needs
class DXShader{
private:
   ID3D11VertexShader* p_vertexShader;
   ID3D11PixelShader* p_pixelShader;
   
   ID3DBlob* p_vertexShaderBlob;
   ID3DBlob* p_pixelShaderBlob;

   bool m_isValid;
public:
   DXShader(ID3D11Device* device, const MString& shaderName);
   ~DXShader();

   ID3DBlob* getVertexShaderBlob();
   void setShader(ID3D11DeviceContext* context);
   bool isValid() const;
};

class CDXPrimitive : public CGPUPrimitive {
protected:
   ID3D11Buffer* p_vertexBuffer;
   ID3D11Buffer* p_indexBuffer;
   ID3D11InputLayout* p_vertexLayout;
   ID3D11Device* p_device;
public:
   CDXPrimitive(ID3D11Device* device);

   virtual ~CDXPrimitive();
   virtual void draw(void* platform = 0) const;

   virtual void setPrimitiveData(const float* vertices, unsigned int numVertices, const unsigned int* indices, unsigned int numIndices);

   bool createInputLayout(ID3DBlob* vertexShaderBlob);
};

class CDXConstantBuffer {
private:
   ID3D11Buffer* p_buffer;
public:
   CDXConstantBuffer(ID3D11Device* device, size_t bufferSize);
   ~CDXConstantBuffer();

   void update(ID3D11DeviceContext* context, void* data);
   void set(ID3D11DeviceContext* context);
   
   bool isValid() const;
};

#endif
