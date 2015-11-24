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

   struct BackgroundData
   {
      BackgroundData() : bgBuffer(NULL), bgWidth(0), bgHeight(0), textureId(0) {
         scale.x = scale.y = 1.f;
         offset.x = offset.y = 0.f;
      }
      ~BackgroundData() {
         if (bgBuffer) AiFree(bgBuffer);
         if (textureId) glDeleteTextures(1, &textureId);
      }


      AtRGBA8 *bgBuffer;
      unsigned int bgWidth;
      unsigned int bgHeight;
      GLuint textureId;
      AtPoint2 scale;
      AtPoint2 offset;

   };

   CRenderGLWidget(QWidget *parent, CRenderView &rv, int width, int height);
   virtual ~CRenderGLWidget();
   

   void initializeGL();
   void resizeGL(int width, int height);
   void paintGL();
   void closeEvent(QCloseEvent *event);

   AtRGBA8 *getBuffer() { return m_front_buffer; }
   
   int width() { return m_width; }
   int height() { return m_height; }

   void copyToBackBuffer();
   void reloadBuffer(AtRvColorMode color_mode);

   void setRegionCrop(int start_x, int start_y, int end_x, int end_y);
   void clearRegionCrop();

   void setPan(int x, int y)
   {
      m_pan[0] = x; 
      m_pan[1] = y; 
   }
   void getPan(int &x, int &y) const
   {
      x = m_pan[0];
      y = m_pan[1];
   }

   void setZoomFactor(float z) {m_zoomFactor = z;}
   float getZoomFactor() const {return m_zoomFactor;}

   const AtBBox2 *getRegion() const {return (m_regionCrop) ? &m_region : NULL;}

   void project(int windowX, int windowY, int& bufferX, int &bufferY, bool clamp = false) const;
   
   void debugGPUState() {m_printGPUState = true;}
   void setBackgroundImage(const std::string &filename);
   void setBackgroundChecker();
   void setBackgroundColor (const AtRGBA &c) {m_bg_color = c;}
   const AtRGBA &getBackgroundColor() const {return m_bg_color;}

   BackgroundData *getBackgroundData() {return m_bg_data;}

private:
   void setupTexture();
   void displayBuffer(int w, int h, const AtBBox2 *update_region,
             AtRvColorMode color_mode, bool back_buffer);

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

   bool m_regionCrop;
   AtBBox2 m_region;

   int m_pan[2];
   float m_zoomFactor;
  
   BackgroundData *m_bg_data;
   AtRGBA m_bg_color;

   bool m_printGPUState;
  
};

