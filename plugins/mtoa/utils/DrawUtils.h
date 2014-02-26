#include <vector>


#if defined(_DARWIN)
   #include <OpenGL/gl.h>
#else 
   #include <GL/gl.h>
#endif   

class CLinePrimitiveData{
public:
   std::vector<float> vertices;
   std::vector<unsigned int> indices;
   GLenum elementType;

   void draw() // TODO : use VBOs, but use glew to determine if vbos are working (virtual machines)
   {
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, vertices.data());
      glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, indices.data());
      glDisableClientState(GL_VERTEX_ARRAY);
   }
};

// use for static initialization
// These two classes could use fewer indices if we used
// glPrimitiveRestartIndex, but that's only available
// since OpenGL 3.1, and it doesn't worth the code duplication
class CDiskPrimitive : public CLinePrimitiveData{
public:
   CDiskPrimitive(GLsizei resolution = 20)
   {
      elementType = GL_LINE_STRIP;
      vertices.resize((resolution + 1) * 3);
      vertices[0] = 0.0f;
      vertices[1] = 0.0f;
      vertices[2] = 0.0f;
      for (GLsizei i = 0; i < resolution; ++i)
      {
         const float d = AI_PITIMES2 * (float(i) / float(resolution));
         float* v = &vertices[(i + 1) * 3];
         v[0] = cosf(d);
         v[1] = sinf(d);
         v[2] = 0.0f;
      }
      indices.resize(resolution * 4);
      const GLsizei res2 = resolution * 2;
      for (GLsizei i = 0; i < resolution; ++i)
      {
         const GLsizei i2 = i * 2;
         indices[i2] = 0;
         indices[i2 + 1] = i + 1;
         indices[i2 + res2] = i + 1;
         indices[i2 + res2 + 1] = (i + 1) % resolution + 1;
      }
   }
};

class CCylinderPrimitive : public CLinePrimitiveData{
public:
   CCylinderPrimitive(float radius = 1.0f, float height = 1.0f, GLsizei resolution = 20)
   {
      vertices.resize(resolution * 6);
      const GLsizei indexDiff = resolution * 3;
      for (GLsizei i = 0; i < resolution; ++i)
      {
         const float d = AI_PITIMES2 * (float(i) / float(resolution));
         float* v = &vertices[i * 3];
         const float x = cosf(d) * radius;
         const float z = sinf(d) * radius;
         v[0] = x;
         v[1] = height;
         v[2] = z;
         v = &vertices[i * 3 + indexDiff];
         v[0] = x;
         v[1] = -height;
         v[2] = z;
      }
      indices.resize(resolution * 6);
      const GLsizei res2 = resolution * 2;
      for (GLsizei i = 0; i < resolution; ++i)
      {
         const GLsizei i2 = i * 2;
         const GLsizei i1 = (i + 1) % resolution;
         indices[i2] = i;
         indices[i2 + 1] = i1;
         indices[i2 + res2] = i + resolution;
         indices[i2 + res2 + 1] = i1 + resolution;
         const GLsizei i2o = i2 + resolution * 4;
         indices[i2o] = i;
         indices[i2o + 1] = i + resolution;
      }
   }
};

class CSpherePrimitive : public CLinePrimitiveData{
public:
   CSpherePrimitive(float radius = 1.0f, GLsizei resolution = 16)
   {
      // 0 is the bottom point
      // 1 is the top point
      vertices.resize((resolution * resolution + 2) * 6);
      vertices[0] = 0.0f;
      vertices[1] = -radius;
      vertices[2] = 0.0f;
      vertices[3] = 0.0f;
      vertices[4] = radius;
      vertices[5] = 0.0f;
      GLsizei vid = 5;
      for (GLsizei yy = 0; yy < resolution; ++yy)
      {
         const float dy = AI_PI * float(yy) / float(resolution) - AI_PIOVER2;
         const float y = sinf(dy) * radius;
         const float pr = cosf(dy) * radius;
         for (GLsizei xx = 0; xx < resolution; ++xx)
         {
            const float dx = AI_PITIMES2 * float(xx) / float(resolution);
            vertices[++vid] = cosf(dx) * pr;
            vertices[++vid] = y;
            vertices[++vid] = sinf(dx) * pr;
         }
      }
      indices.resize(resolution * resolution * 2 + // for horizontal lines
         (resolution + 1) * resolution * 2); // for vertical lines
      // fill horizontal lines
      GLsizei id = 0;
      for (GLsizei yy = 0; yy < resolution; ++yy)
      {
         const GLsizei wy = 2 + yy * resolution;
         for (GLsizei xx = 0; xx < resolution; ++xx)
         {
            indices[id++] = wy + xx;
            indices[id++] = wy + (xx + 1) % resolution;
         }
      }

      for (GLsizei xx = 0; xx < resolution; ++xx)
      {
         const GLsizei xx2 = 2 + xx;
         indices[id++] = 0;         
         indices[id++] = xx2;
         for (GLsizei yy = 0; yy < (resolution - 1); ++yy)
         {
            indices[id++] = xx2 + yy * resolution;
            indices[id++] = xx2 + (yy + 1) * resolution;
         }
         indices[id++] = xx2 + (resolution - 1) * resolution;
         indices[id++] = 1;
      }
   }
};
