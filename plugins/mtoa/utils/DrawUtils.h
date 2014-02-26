#pragma once

#include <maya/M3dView.h>

#if defined(_DARWIN)
   #include <OpenGL/gl.h>
#else 
   #include <GL/gl.h>
#endif   

#include <vector>

class CLinePrimitiveData{
public:
   std::vector<float> vertices;
   std::vector<unsigned int> indices;
   GLenum elementType;

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
