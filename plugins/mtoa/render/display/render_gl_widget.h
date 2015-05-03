/*
 * OpenGL display shader and buffer
 */

#pragma once

#include <ai.h>

#include <string>

#include "display_gl.h"
#include "renderview.h"


// QGlWidget  displaying the render buffer

class CRenderGLWidget : public QGLWidget {
  
public:
   CRenderGLWidget(QWidget *parent, CRenderView &rv, int width, int height);
   virtual ~CRenderGLWidget();
   
   void initializeGL();
   void resizeGL(int width, int height);
   void paintGL();
   void closeEvent(QCloseEvent *event);

   void displayBuffer(int w, int h, const AtBBox2 *update_region,
             AtKickColorMode color_mode, bool back_buffer);

   AtRGBA8 *buffer() { return m_front_buffer; }
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
