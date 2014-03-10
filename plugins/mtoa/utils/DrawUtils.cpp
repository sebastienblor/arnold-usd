#include <ai.h>

#include "DrawUtils.h"

void CLinePrimitiveData::draw() // TODO : use VBOs, but use glew to determine if vbos are working (virtual machines)
{
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, vertices.data());
   glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
   glDisableClientState(GL_VERTEX_ARRAY);
}

CDiskPrimitive::CDiskPrimitive(GLsizei resolution)
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

CCylinderPrimitive::CCylinderPrimitive(float radius, float height, GLsizei resolution)
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

CSpherePrimitive::CSpherePrimitive(float radius, GLsizei resolution)
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

CBoxPrimitive::CBoxPrimitive(float size)
{
   GLsizei id = 0;
   vertices.resize(8 * 3);   
   vertices[id++] = -size; vertices[id++] = -size; vertices[id++] = -size;
   vertices[id++] = -size; vertices[id++] = -size; vertices[id++] = size;
   vertices[id++] = size; vertices[id++] = -size; vertices[id++] = size;
   vertices[id++] = size; vertices[id++] = -size; vertices[id++] = -size;

   vertices[id++] = -size; vertices[id++] = size; vertices[id++] = -size;
   vertices[id++] = -size; vertices[id++] = size; vertices[id++] = size;
   vertices[id++] = size; vertices[id++] = size; vertices[id++] = size;
   vertices[id++] = size; vertices[id++] = size; vertices[id++] = -size;

   id = 0;
   indices.resize(12 * 2);   
   indices[id++] = 0; indices[id++] = 1;
   indices[id++] = 1; indices[id++] = 2;
   indices[id++] = 2; indices[id++] = 3;
   indices[id++] = 3; indices[id++] = 0;

   indices[id++] = 4; indices[id++] = 5;
   indices[id++] = 5; indices[id++] = 6;
   indices[id++] = 6; indices[id++] = 7;
   indices[id++] = 7; indices[id++] = 4;

   indices[id++] = 0; indices[id++] = 4;
   indices[id++] = 1; indices[id++] = 5;
   indices[id++] = 2; indices[id++] = 6;
   indices[id++] = 3; indices[id++] = 7;
}

#ifdef ENABLE_VP2

CGLPrimitive::CGLPrimitive() : m_VBO(0), m_IBO(0), m_VAO(0),
   m_numLineIndices(0)
{

}

CGLPrimitive::~CGLPrimitive()
{
   glDeleteBuffers(2, m_GLBuffers);
   glDeleteVertexArrays(1, &m_VAO);
}

void CGLPrimitive::setPrimitiveData(const float* vertices, unsigned int numVertices, const unsigned int* indices, unsigned int numIndices)
{
   glGenBuffers(2, m_GLBuffers);

   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glGenVertexArrays(1, &m_VAO);
   glBindVertexArray(m_VAO);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
   glBindVertexArray(0);
   m_numLineIndices = numIndices;
}

void CGLPrimitive::draw() const
{
   glBindVertexArray(m_VAO);

   glDrawElements(GL_LINES, m_numLineIndices, GL_UNSIGNED_INT, 0);

   glBindVertexArray(0);
}

CGLQuadLightPrimitive::CGLQuadLightPrimitive()
{
   const float vertices [] = {
      -1.0f, -1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      -1.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, -1.0f
   };

   const unsigned int indices[] = {
      0, 1,
      1, 2,
      2, 3,
      3, 0,
      0, 2,
      3, 1,
      4, 5
   };

   setPrimitiveData(vertices, 6 * 3, indices, 7 * 2);
}

#endif
