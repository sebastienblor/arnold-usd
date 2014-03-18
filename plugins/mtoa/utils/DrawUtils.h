#pragma once

#ifdef ENABLE_VP2
#include <GL/glew.h>
#else
#if defined(_DARWIN)
   #include <OpenGL/gl.h>
#else 
   #include <GL/gl.h>
#endif
#endif

#include <vector>

class CLinePrimitiveData{
public:
   std::vector<float> vertices;
   std::vector<unsigned int> indices;

   void draw();
};

// use for static initialization
// These two classes could use fewer indices if we used
// glPrimitiveRestartIndex, but that's only available
// since OpenGL 3.1, and it doesn't worth the code duplication
class CDiskPrimitive : public CLinePrimitiveData{
public:
   CDiskPrimitive(GLsizei resolution = 20);
};

class CCylinderPrimitive : public CLinePrimitiveData{
public:
   CCylinderPrimitive(float radius = 1.0f, float height = 1.0f, GLsizei resolution = 20);
};

class CSpherePrimitive : public CLinePrimitiveData{
public:
   CSpherePrimitive(float radius = 1.0f, GLsizei resolution = 16);
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
         GLuint m_VBO;
         GLuint m_IBO;
      };
      GLuint m_GLBuffers[2];
   };
   GLuint m_VAO;
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

bool checkShaderError(GLuint shader);
bool checkProgramError(GLuint shader);

#endif
