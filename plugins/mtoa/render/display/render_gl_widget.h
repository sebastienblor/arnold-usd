/*
 * OpenGL display shader and buffer
 */

#pragma once
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QtOpenGL>

#include <ai.h>

#include <string>

#include "display_gl.h"

class CRenderView;

// QGlWidget  displaying the render buffer
enum AtRvColorMode
{
   COLOR_MODE_RGBA = 0,
   COLOR_MODE_R,
   COLOR_MODE_G,
   COLOR_MODE_B,
   COLOR_MODE_A,
};

struct AtRGBA8
{
   AtUInt8 r, g, b, a;
};

class CRenderGLWidget : public QGLWidget {
  
public:
   CRenderGLWidget(QWidget *parent, CRenderView &rv, int width, int height);
   virtual ~CRenderGLWidget();
   
   void initializeGL();
   void resizeGL(int width, int height);
   void paintGL();
   void closeEvent(QCloseEvent *event);

   void displayBuffer(int w, int h, const AtBBox2 *update_region,
             AtRvColorMode color_mode, bool back_buffer);

   AtRGBA8 *getBuffer() { return m_front_buffer; }
   
   int width() { return m_width; }
   int height() { return m_height; }

   void copyToBackBuffer();

private:
   void setupTexture();

   CRenderView &m_renderview;

   // buffer
   AtRGBA8 *m_front_buffer;
   AtRGBA8 *m_back_buffer;
   
   // OpenGL textured handle
   GLuint m_texture;
   // buffer and texture dimensions
   int m_width, m_height;
   // texture coordinates for drawing
   float m_tx, m_ty;
  
};
