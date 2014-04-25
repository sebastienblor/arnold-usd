#pragma once

#ifdef ENABLE_VP2
#include <GL/glew.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#include <d3dcompiler.h>
#ifndef D3DCOMPILE_ENABLE_STRICTNESS
    #define D3DCOMPILE_ENABLE_STRICTNESS D3D10_SHADER_ENABLE_STRICTNESS
    #define D3DCOMPILE_DEBUG D3D10_SHADER_DEBUG
#endif
#endif
#endif

#include <vector>

class CLinePrimitiveData{
protected:
   CLinePrimitiveData() {}
public:
   virtual ~CLinePrimitiveData() {}
   std::vector<float> vertices;
   std::vector<unsigned int> indices;

   virtual void draw();
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

#ifdef ENABLE_VP2

// TODO unify these two classes later, that can deal both
// with vbo and the old pipeline and static / dynamic initialization
// IBO has to contain both wireframe and triangle indices
class CGLPrimitive {
protected:
   union{
      struct{
         unsigned int m_VBO;
         unsigned int m_IBO;
      };
      unsigned int m_GLBuffers[2];
   };
   unsigned int m_VAO;
   unsigned int m_numLineIndices;
   CGLPrimitive();
   void setPrimitiveData(const float* vertices, unsigned int numVertices, const unsigned int* indices, unsigned int numIndices);
public:
   virtual ~CGLPrimitive();
   virtual void draw() const;
};

class CGLQuadLightPrimitive : public CGLPrimitive{
public:
   CGLQuadLightPrimitive();
};

class CGLDiskLightPrimitive : public CGLPrimitive{
public:
   CGLDiskLightPrimitive();
};

class CGLCylinderPrimitive : public CGLPrimitive{
public:
   CGLCylinderPrimitive();
};

class CGLPhotometricLightPrimitive : public CGLPrimitive{
public:
   CGLPhotometricLightPrimitive();
};

bool checkShaderError(unsigned int shader);
bool checkProgramError(unsigned int shader);

#endif
