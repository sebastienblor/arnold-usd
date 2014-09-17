#include <ai.h>

#include "DrawUtils.h"

#include <maya/MPxLocatorNode.h>

#if defined(_DARWIN)
   #include <OpenGL/gl.h>
#else 
   #include <GL/gl.h>
#endif   

void CLinePrimitiveData::draw() // TODO : use VBOs, but use glew to determine if vbos are working (virtual machines)
{
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
   glDrawElements(GL_LINES, (unsigned int)indices.size(), GL_UNSIGNED_INT, &indices[0]);
   glDisableClientState(GL_VERTEX_ARRAY);
}

CDiskPrimitive::CDiskPrimitive(unsigned int resolution)
{
   vertices.resize((resolution + 1) * 3);
   vertices[0] = 0.0f;
   vertices[1] = 0.0f;
   vertices[2] = 0.0f;
   for (unsigned int i = 0; i < resolution; ++i)
   {
      const float d = AI_PITIMES2 * (float(i) / float(resolution));
      float* v = &vertices[(i + 1) * 3];
      v[0] = cosf(d);
      v[1] = sinf(d);
      v[2] = 0.0f;
   }
   indices.resize(resolution * 4);
   const unsigned int res2 = resolution * 2;
   for (unsigned int i = 0; i < resolution; ++i)
   {
      const unsigned int i2 = i * 2;
      indices[i2] = 0;
      indices[i2 + 1] = i + 1;
      indices[i2 + res2] = i + 1;
      indices[i2 + res2 + 1] = (i + 1) % resolution + 1;
   }
}

CCylinderPrimitive::CCylinderPrimitive(float radius, float height, unsigned int resolution)
{
   vertices.resize(resolution * 6);
   const unsigned int indexDiff = resolution * 3;
   for (unsigned int i = 0; i < resolution; ++i)
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
   const unsigned int res2 = resolution * 2;
   for (unsigned int i = 0; i < resolution; ++i)
   {
      const unsigned int i2 = i * 2;
      const unsigned int i1 = (i + 1) % resolution;
      indices[i2] = i;
      indices[i2 + 1] = i1;
      indices[i2 + res2] = i + resolution;
      indices[i2 + res2 + 1] = i1 + resolution;
      const unsigned int i2o = i2 + resolution * 4;
      indices[i2o] = i;
      indices[i2o + 1] = i + resolution;
   }
}

CSpherePrimitive::CSpherePrimitive(float radius, unsigned int resolution)
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
   unsigned int vid = 5;
   for (unsigned int yy = 0; yy < resolution; ++yy)
   {
      const float dy = AI_PI * float(yy) / float(resolution) - AI_PIOVER2;
      const float y = sinf(dy) * radius;
      const float pr = cosf(dy) * radius;
      for (unsigned int xx = 0; xx < resolution; ++xx)
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
   unsigned int id = 0;
   for (unsigned int yy = 0; yy < resolution; ++yy)
   {
      const unsigned int wy = 2 + yy * resolution;
      for (unsigned int xx = 0; xx < resolution; ++xx)
      {
         indices[id++] = wy + xx;
         indices[id++] = wy + (xx + 1) % resolution;
      }
   }

   for (unsigned int xx = 0; xx < resolution; ++xx)
   {
      const unsigned int xx2 = 2 + xx;
      indices[id++] = 0;         
      indices[id++] = xx2;
      for (unsigned int yy = 0; yy < (resolution - 1); ++yy)
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
   unsigned int id = 0;
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

CPhotometricLightPrimitive::CPhotometricLightPrimitive()
{
   unsigned int id = 0;

   vertices.resize(3 * 3 * 360 + // vertices for the three circles
      3 * 3 * 8 + // vertices for the "spikes"
      3 * 4 + // for the direction
      3 * 4 // for the rotational direction
      );

   for (unsigned int i = 0; i < 360; ++i)
   {
      const float d = float(i) * 2.0f * AI_PI / 360.0f;
      const unsigned int i3 = i * 3;
      const unsigned int i31 = i3 + 1;
      const unsigned int i32 = i3 + 2;
      const float cd = cosf(d) * 0.5f;
      const float sd = sinf(d) * 0.5f;

      vertices[i3] = cd;
      vertices[i31] = sd;
      vertices[i32] = 0.0f;

      vertices[i3 + 360 * 3] = 0.0f;
      vertices[i31 + 360 * 3] = cd;
      vertices[i32 + 360 * 3] = sd;

      vertices[i3 + 360 * 3 * 2] = cd;
      vertices[i31 + 360 * 3 * 2] = 0.0f;
      vertices[i32 + 360 * 3 * 2] = sd;
   }

   id = 360 * 3 * 3;

   for (unsigned int i = 0; i < 8; ++i)
   {
      const float d = float(i * 45) * 2.0f * AI_PI / 360.0f;
      const unsigned int i3 = i * 3 + id;
      const unsigned int i31 = i3 + 1;
      const unsigned int i32 = i3 + 2;
      const float cd = cosf(d) * 0.7f;
      const float sd = sinf(d) * 0.7f;

      vertices[i3] = cd;
      vertices[i31] = sd;
      vertices[i32] = 0.0f;

      vertices[i3 + 8 * 3] = 0.0f;
      vertices[i31 + 8 * 3] = cd;
      vertices[i32 + 8 * 3] = sd;

      vertices[i3 + 8 * 3 * 2] = cd;
      vertices[i31 + 8 * 3 * 2] = 0.0f;
      vertices[i32 + 8 * 3 * 2] = sd;
   }

   id += 8 * 3 * 3;

   vertices[id++] = 0.0f; vertices[id++] = -0.5f; vertices[id++] = 0.0f;
   vertices[id++] = 0.0f; vertices[id++] = -2.0f; vertices[id++] = 0.0f;
   vertices[id++] = 0.3f; vertices[id++] = -1.5f; vertices[id++] = 0.0f;
   vertices[id++] = -0.3f; vertices[id++] = -1.5f; vertices[id++] = 0.0f;

   vertices[id++] = 0.5f; vertices[id++] = 0.0f; vertices[id++] = 0.0f;
   vertices[id++] = 1.0f; vertices[id++] = 0.0f; vertices[id++] = 0.0f;
   vertices[id++] = 1.0f; vertices[id++] = -0.3f; vertices[id++] = 0.0f;
   vertices[id++] = 1.0f; vertices[id++] = 0.3f; vertices[id++] = 0.0f;

   id = 0;
   indices.resize(360 * 2 * 3 + // for the 3 circles
      8 * 2 * 3 + // for the "spikes"
      3 * 2 + // for the direction
      2 * 2 // for the rotational direction
      );

   for (unsigned int i = 0; i < 360; ++i)
   {
      const unsigned int i360 = (i + 1) % 360;
      indices[id++] = i;
      indices[id++] = i360;
      indices[id++] = i + 360;
      indices[id++] = i360 + 360;
      indices[id++] = i + 360 * 2;
      indices[id++] = i360 + 360 * 2;      
   }

   for (unsigned int i = 0; i < 8; ++i)
   {
      unsigned int idb = 360 * 3;
      indices[id++] = i * 45;
      indices[id++] = idb + i;
      indices[id++] = i * 45 + 360;
      indices[id++] = idb + i + 8;
      indices[id++] = i * 45 + 360 * 2;
      indices[id++] = idb + i + 8 * 2;
   }

   unsigned int idb = 360 * 3 + 8 * 3;

   indices[id++] = idb; indices[id++] = idb + 1;
   indices[id++] = idb + 2; indices[id++] = idb + 1;
   indices[id++] = idb + 3; indices[id++] = idb + 1;

   indices[id++] = idb + 4; indices[id++] = idb + 5;
   indices[id++] = idb + 6; indices[id++] = idb + 7;
}

#ifdef ENABLE_VP2

#include <iostream>

CGLPrimitive::CGLPrimitive() : CGPUPrimitive(), m_VBO(0), m_IBO(0)
{
}

CGLPrimitive::~CGLPrimitive()
{
   glDeleteBuffers(2, m_GLBuffers);
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

   m_numLineIndices = numIndices;
}

void CGLPrimitive::draw() const
{
   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, (char*)0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

   glDrawElements(GL_LINES, m_numLineIndices, GL_UNSIGNED_INT, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glDisableClientState(GL_VERTEX_ARRAY);
}

CGPUPrimitive* CGQuadLightPrimitive::generate(CGPUPrimitive* prim)
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

   prim->setPrimitiveData(vertices, 6 * 3, indices, 7 * 2);
   return prim;
}

CGPUPrimitive* CGDiskLightPrimitive::generate(CGPUPrimitive* prim)
{
   float vertices[22 * 3];
   vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f;
   vertices[3] = 0.0f; vertices[4] = 0.0f; vertices[5] = -1.0f;

   for (unsigned int i = 0; i < 20; ++i)
   {
      const float d = AI_PITIMES2 * (float(i) / 20.0f);
      float* v = &vertices[(i + 2) * 3];
      v[0] = cosf(d);
      v[1] = sinf(d);
      v[2] = 0.0f;
   }

   unsigned int indices[20 * 4 + 2];
   unsigned int id = 0;
   indices[id++] = 0;
   indices[id++] = 1;
   for (unsigned int i = 0; i < 20; ++i)
   {
      indices[id++] = i + 2;
      indices[id++] = (i + 1) % 20 + 2;
      indices[id++] = 0;
      indices[id++] = i + 2;
   }

   prim->setPrimitiveData(vertices, 22 * 3, indices, 20 * 4 + 2);
   return prim;
}

CGPUPrimitive* CGCylinderPrimitive::generate(CGPUPrimitive* prim)
{
   float vertices[20 * 6];
   const unsigned int indexDiff = 20 * 3;
   for (unsigned int i = 0; i < 20; ++i)
   {
      const float d = AI_PITIMES2 * (float(i) / 20.0f);
      float* v = &vertices[i * 3];
      const float x = cosf(d);
      const float z = sinf(d);
      v[0] = x;
      v[1] = 1.0f;
      v[2] = z;
      v = &vertices[i * 3 + indexDiff];
      v[0] = x;
      v[1] = -1.0f;
      v[2] = z;
   }
   unsigned int indices[20 * 6];
   const unsigned int res2 = 20 * 2;
   for (unsigned int i = 0; i < 20; ++i)
   {
      const unsigned int i2 = i * 2;
      const unsigned int i1 = (i + 1) % 20;
      indices[i2] = i;
      indices[i2 + 1] = i1;
      indices[i2 + res2] = i + 20;
      indices[i2 + res2 + 1] = i1 + 20;
      const unsigned int i2o = i2 + 20 * 4;
      indices[i2o] = i;
      indices[i2o + 1] = i + 20;
   }

   prim->setPrimitiveData(vertices, 20 * 6, indices, 20 * 6);
   return prim;
}

CGPUPrimitive* CGPhotometricLightPrimitive::generate(CGPUPrimitive* prim)
{
   unsigned int id = 0;

   const unsigned int numVertices = 3 * 3 * 360 + // vertices for the three circles
      3 * 3 * 8 + // vertices for the "spikes"
      3 * 4 + // for the direction
      3 * 4; // for the rotational direction

   float vertices[numVertices];

   for (unsigned int i = 0; i < 360; ++i)
   {
      const float d = float(i) * 2.0f * AI_PI / 360.0f;
      const unsigned int i3 = i * 3;
      const unsigned int i31 = i3 + 1;
      const unsigned int i32 = i3 + 2;
      const float cd = cosf(d) * 0.5f;
      const float sd = sinf(d) * 0.5f;

      vertices[i3] = cd;
      vertices[i31] = sd;
      vertices[i32] = 0.0f;

      vertices[i3 + 360 * 3] = 0.0f;
      vertices[i31 + 360 * 3] = cd;
      vertices[i32 + 360 * 3] = sd;

      vertices[i3 + 360 * 3 * 2] = cd;
      vertices[i31 + 360 * 3 * 2] = 0.0f;
      vertices[i32 + 360 * 3 * 2] = sd;
   }

   id = 360 * 3 * 3;

   for (unsigned int i = 0; i < 8; ++i)
   {
      const float d = float(i * 45) * 2.0f * AI_PI / 360.0f;
      const unsigned int i3 = i * 3 + id;
      const unsigned int i31 = i3 + 1;
      const unsigned int i32 = i3 + 2;
      const float cd = cosf(d) * 0.7f;
      const float sd = sinf(d) * 0.7f;

      vertices[i3] = cd;
      vertices[i31] = sd;
      vertices[i32] = 0.0f;

      vertices[i3 + 8 * 3] = 0.0f;
      vertices[i31 + 8 * 3] = cd;
      vertices[i32 + 8 * 3] = sd;

      vertices[i3 + 8 * 3 * 2] = cd;
      vertices[i31 + 8 * 3 * 2] = 0.0f;
      vertices[i32 + 8 * 3 * 2] = sd;
   }

   id += 8 * 3 * 3;

   vertices[id++] = 0.0f; vertices[id++] = -0.5f; vertices[id++] = 0.0f;
   vertices[id++] = 0.0f; vertices[id++] = -2.0f; vertices[id++] = 0.0f;
   vertices[id++] = 0.3f; vertices[id++] = -1.5f; vertices[id++] = 0.0f;
   vertices[id++] = -0.3f; vertices[id++] = -1.5f; vertices[id++] = 0.0f;

   vertices[id++] = 0.5f; vertices[id++] = 0.0f; vertices[id++] = 0.0f;
   vertices[id++] = 1.0f; vertices[id++] = 0.0f; vertices[id++] = 0.0f;
   vertices[id++] = 1.0f; vertices[id++] = -0.3f; vertices[id++] = 0.0f;
   vertices[id++] = 1.0f; vertices[id++] = 0.3f; vertices[id++] = 0.0f;

   id = 0;
   const unsigned int numIndices = 360 * 2 * 3 + // for the 3 circles
      8 * 2 * 3 + // for the "spikes"
      3 * 2 + // for the direction
      2 * 2; // for the rotational direction
   unsigned int indices[numIndices];

   for (unsigned int i = 0; i < 360; ++i)
   {
      const unsigned int i360 = (i + 1) % 360;
      indices[id++] = i;
      indices[id++] = i360;
      indices[id++] = i + 360;
      indices[id++] = i360 + 360;
      indices[id++] = i + 360 * 2;
      indices[id++] = i360 + 360 * 2;      
   }

   for (unsigned int i = 0; i < 8; ++i)
   {
      unsigned int idb = 360 * 3;
      indices[id++] = i * 45;
      indices[id++] = idb + i;
      indices[id++] = i * 45 + 360;
      indices[id++] = idb + i + 8;
      indices[id++] = i * 45 + 360 * 2;
      indices[id++] = idb + i + 8 * 2;
   }

   unsigned int idb = 360 * 3 + 8 * 3;

   indices[id++] = idb; indices[id++] = idb + 1;
   indices[id++] = idb + 2; indices[id++] = idb + 1;
   indices[id++] = idb + 3; indices[id++] = idb + 1;

   indices[id++] = idb + 4; indices[id++] = idb + 5;
   indices[id++] = idb + 6; indices[id++] = idb + 7;

   prim->setPrimitiveData(vertices, numVertices, indices, numIndices);
   return prim;
}

CGPUPrimitive* CGBoxPrimitive::generate(CGPUPrimitive* prim)
{
   const float vertices [] = {
      0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 1.0f,
      0.0f, 1.0f, 1.0f
   };

   const unsigned int indices[] = {
      0, 1, 1, 2, 2, 3, 3, 0,
      4, 5, 5, 6, 6, 7, 7, 4,
      0, 4, 1, 5, 2, 6, 3, 7
   };

   prim->setPrimitiveData(vertices, 8 * 3, indices, 4 * 3 * 2);
   return prim;
}

bool checkShaderError(unsigned int shader)
{
   GLint success = 0;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
   if (success == GL_FALSE)
   {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<char> errorLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

      std::cerr << "[MtoA] Error compiling shader : " << &errorLog[0] << std::endl;
      return true;
   }
   return false;
}

bool checkProgramError(unsigned int program)
{
   GLint success = 0;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   if (success == GL_FALSE)
   {
      GLint maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<char> errorLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

      std::cerr << "[MtoA] Error linking shader program : " << &errorLog[0] << std::endl;
      return true;
   }
   return false;
}

#ifdef _WIN32

#include "MayaUtils.h"
#include <windows.h>

DXShader::DXShader(ID3D11Device* device, const MString& shaderName) :
   p_vertexShader(0), p_pixelShader(0), p_vertexShaderBlob(0), p_pixelShaderBlob(0), m_isValid(false)
{
   DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
   ID3DBlob* errorBlob = 0;

   MString effectLocation = replaceInString(MString(getenv("MTOA_PATH")), "\\", "/") + MString("/vp2/") + shaderName + MString(".hlsl");

   HRESULT hr;

#if _MSC_VER < 1700
   hr = D3DX11CompileFromFile(
      effectLocation.asChar(),
      0,
      0,
      "mainVS",
      "vs_5_0",
      shaderFlags,
      0,
      0,
      &p_vertexShaderBlob,
      &errorBlob,
      NULL);
#else
   hr = D3DCompileFromFile(
      effectLocation.asWChar(),
      0,
      0,
      "mainVS",
      "vs_5_0",
      shaderFlags,
      0,
      &p_vertexShaderBlob,
      &errorBlob);
#endif
   if (FAILED(hr))
   {                
      if (errorBlob) errorBlob->Release();
         return;  
   }

   if (errorBlob) errorBlob->Release();
   hr = device->CreateVertexShader(p_vertexShaderBlob->GetBufferPointer(), p_vertexShaderBlob->GetBufferSize(), 0, &p_vertexShader);
   if (FAILED(hr))
   {
      return;
   }
#if _MSC_VER < 1700
   hr = D3DX11CompileFromFile(
      effectLocation.asChar(),
      0,
      0,
      "mainPS",
      "ps_5_0",
      shaderFlags,
      0,
      0,
      &p_pixelShaderBlob,
      &errorBlob,
      NULL);
#else
   hr = D3DCompileFromFile(
      effectLocation.asWChar(),
      0,
      0,
      "mainPS",
      "ps_5_0",
      0,
      0,
      &p_pixelShaderBlob,
      &errorBlob);
#endif
   if (FAILED(hr))
   {
      if (errorBlob) errorBlob->Release();
         return;  
   } 

   hr = device->CreatePixelShader(p_pixelShaderBlob->GetBufferPointer(), p_pixelShaderBlob->GetBufferSize(), 0, &p_pixelShader);   
   if (FAILED(hr))
   {                
      return;
   }

   m_isValid = true;
}

DXShader::~DXShader()
{
   if (p_vertexShader)
      p_vertexShader->Release();
   if (p_pixelShader)
      p_pixelShader->Release();
   if (p_vertexShaderBlob)
      p_vertexShaderBlob->Release();
   if (p_pixelShaderBlob)
      p_pixelShaderBlob->Release();
}

ID3DBlob* DXShader::getVertexShaderBlob()
{
   return p_vertexShaderBlob;   
}

void DXShader::setShader(ID3D11DeviceContext* context)
{
   if (m_isValid)
   {
      context->VSSetShader(p_vertexShader, 0, 0);
      context->PSSetShader(p_pixelShader, 0, 0);
   }
}

bool DXShader::isValid() const
{
   return m_isValid;
}

#endif

#endif
