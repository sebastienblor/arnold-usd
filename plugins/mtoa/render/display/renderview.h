/*
 * Common header file for macros, data types, prototypes and global vars
 * 
 * --------------------------------------------------------------------------
 * --------------------------------------------------------------------------
 */

#pragma once


#include <ai.h>

#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "shading_manager.h"

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <QtGui/qevent.h>
#include <QtGui/qmainwindow.h>

#include "render_gl_widget.h"
#include <QtCore/qbytearray.h>

#include <QtCore/qthread.h>
#include <iostream>
#include "luts.h"


struct AtDisplaySync;
class CRenderViewManipulator;
class CRenderGLWidget;
class QMenu;


class CRenderView;

class CRenderViewMainWindow : public QMainWindow
{

Q_OBJECT
 
public:
   CRenderViewMainWindow(QWidget *parent, CRenderView &rv) : QMainWindow(parent, Qt::Tool/* Qt::WindowStaysOnTopHint*/), 
      m_renderView(rv), 
      m_menuFile(NULL),
      m_menuView(NULL),
      m_menuRender(NULL),
      m_menuAovs(NULL),
      m_menuCamera(NULL),
      m_toolBar(NULL),
      m_aovsCombo(NULL),
      m_camerasCombo(NULL),
      m_ccWindow(NULL),
      m_manipulator(NULL) {}
    ~CRenderViewMainWindow();
 
   void InitMenus();
   void PopulateAOVsMenu();
   void PopulateCamerasMenu();
   void UpdateCamerasMenu();

   void EnableMenus(bool b)
   {
      if (b == m_activeMenus) return;

      m_renderAction->setEnabled(b);
      m_actionAutoRefresh->setEnabled(b);
      m_actionCropRegion->setEnabled(b);
      m_activeMenus = b;
   }
   
private:

   void UpdateStoredSlider();

   CRenderView &m_renderView;
   QMenu *m_menuFile;
   QMenu *m_menuView;
   QMenu *m_menuRender;
   QMenu *m_menuAovs;
   QMenu *m_menuCamera;

   QToolBar *m_toolBar;
   QComboBox *m_aovsCombo;
   QComboBox *m_camerasCombo;
   QToolButton *m_rgbaButton;

   QAction *m_actionShowRenderingTiles;
   QAction *m_actionAutoRefresh;
   QAction *m_actionProgressiveRefinement;
   QAction *m_actionEnableAovs;
   QAction *m_actionCropRegion;
   QAction *m_actionStatusBar;
   QAction *m_actionStatusInfo;
   QAction *m_storedSliderAction;
   QAction *m_deleteStoredAction;
   QAction *m_storeAction;
   QAction *m_renderAction;
   QAction *m_abortAction;
   QAction *m_3dManipulationAction;
   QAction *m_lutAction;
   QAction *m_debugShadingActionDisabled;
   QAction *m_channelRgbaAction;
   QAction *m_channelRedAction;
   QAction *m_channelGreenAction;
   QAction *m_channelBlueAction;
   QAction *m_channelAlphaAction;


   QActionGroup *m_channelActionGroup;
   QActionGroup *m_aovsActionGroup;
   QActionGroup *m_camerasActionGroup;
   QActionGroup *m_debugShadingActionGroup;   

   QSlider *m_storedSlider;
   CRenderViewCCWindow *m_ccWindow;

   bool m_leftButtonDown;
   int  m_pickPoint[2];

   CRenderViewManipulator *m_manipulator;
   bool  m_activeMenus;
   bool  m_3dManipulation;
   
protected:
   // virtual Qt methods redefined here
   void mouseMoveEvent ( QMouseEvent * event );
   void mousePressEvent ( QMouseEvent * event );
   void mouseReleaseEvent ( QMouseEvent * event );
   void resizeEvent(QResizeEvent *event);
   void moveEvent(QMoveEvent *event);
   void wheelEvent ( QWheelEvent * event );
   void closeEvent(QCloseEvent *event);
   

private:

private slots:
   void SaveImage();
   void AbortRender();
   void ShowRenderingTiles();
   void AutoRefresh();
   void UpdateRender();
   void StoreImage();
   void PreviousStoredImage();
   void NextStoredImage();
   void DeleteStoredImage();
   void ProgressiveRefinement();
   void ShowChannel();
   void EnableAOVs();
   void ShowAOV();
   void ShowBoxAOV();
   void SelectCamera();
   void SelectBoxCamera();
   void CropRegion();
   void FrameAll();
   void FrameRegion();
   void RealSize();
   void ColorCorrection();
   void EnableStatusBar();
   void DisplayPixelInfo();
   void StoredSliderMoved(int);
   void ToggleManipulationMode();
   void DebugShading();
   void RgbaClicked();
   void ShowCamerasMenu();

// If you add a slot to this class,
// don't forget to run
// moc  renderview.h -o renderview.moc
   
};
 

extern AtNodeMethods *kick_driver_mtd;

class CRenderView
{
public:

   // create and destroy window
   CRenderView(int w, int h);

   ~CRenderView();

   
   // request to close the window
   void Close();
   void Show();

   void InterruptRender();
   void FinishRender();

   // properties
   int Width() { return m_width; }
   int Height() { return m_height; }

   AtRGBA *GetBuffer() {return m_buffer;} 
   
   int GetWidth() const {return m_width;}
   int GetHeight() const{return m_height;}

   void Render();

   // utility functions, time is in microseconds
   static AtUInt64 Time();
   static void SyncPause();
   static void Sleep(AtUInt64 usecs);

   void Draw(AtBBox2 *region = 0);
   void Draw(int xmin, int ymin, int width, int height)
   {
      AtBBox2 box;
      box.minx = xmin;
      box.miny = ymin;
      box.maxx = xmin + width;
      box.maxy = ymin + height;
      Draw(&box);
   }

   CRenderGLWidget *GetGlWidget() {return m_gl;}

   void DisplaySyncCreate();
   AtDisplaySync *DisplaySync() {return displaySync;}

   // image parameters
   int m_xRes, m_yRes;        // window and image size
   int m_bucketSize;
   int m_minX, m_minY;        // region window offset
   int m_regX, m_regY;        // region window size

   // OS window handle
   AtCritSec m_windowCloseLock;
   AtCritSec m_pickLock;

   bool CanRestartRender() const;
   void RestartRender();
   void UpdateRender();
   void UpdateRenderOptions();
   void InitRender(int w, int h);

   void SaveImage(const std::string &filename);
   void StoreImage();
   void ShowPreviousStoredImage();
   void ShowNextStoredImage();
   void DeleteStoredImage();
   CRenderViewMainWindow *GetMainWindow() {return m_mainWindow;}

   
   // this method doesn't check for boundaries
   void SetPixelColor(int x, int y, const AtRGBA &rgba)
   {
      int pixelIndex = y * m_width + x;

      m_buffer[pixelIndex] = rgba;

      if (m_displayedImageIndex < 0 && m_displayedAovIndex < 0)
      {
         FillGLPixel(rgba, x, y, pixelIndex);
      }
   }

   // this method doesn't check for boundaries
   void SetAOVPixelColor(int aovIndex, int x, int y, const AtRGBA &rgba)
   {
      int pixelIndex = y * m_width + x;

      m_aovBuffers[aovIndex][pixelIndex] = rgba;

      if (m_displayedAovIndex == aovIndex && m_displayedImageIndex < 0)
      {
         FillGLPixel(rgba, x, y, pixelIndex);
      }
   }


   void SetPixelsBlock(int minx, int miny, int maxx, int maxy, const AtRGBA &rgba, int aovIndex = -1)
   {
      // Apply LUTs just once for the whole block of pixels
      AtRGBA outColor(rgba);
      AtRGBA *rgbaBuffer = (aovIndex < 0) ? m_buffer : m_aovBuffers[aovIndex];

      // is this buffer being currently displayed ?
      bool displayGL = (m_displayedImageIndex < 0 && m_displayedAovIndex == aovIndex);
      
      if (displayGL)
      {
         ApplyLUTs(outColor);

         // if picking in progress, compare the picked ID with the ID AOV
         if (m_pickedId)
         {
            AiCritSecEnter(&m_pickLock);
            //int pixelId = *((int*)(&m_aovBuffers.back()[x + y * m_width].r));
            int pixelId = reinterpret_type<float, int>(m_aovBuffers.back()[minx + miny * m_width].r);

            if ( m_pickedId && pixelId == *m_pickedId )
            {
               // mix the color with white (50%)
               outColor.rgb() *= 0.5f;
               outColor.rgb() += 0.5f;
            }
            AiCritSecLeave(&m_pickLock);
         }

         AtRGBA8 rgba8;

         // forcing dithering to FALSE during progressive AA steps
         bool dither = false; //m_colorCorrectSettings.dither;
         rgba8.r = AiQuantize8bit(minx, miny, 0, outColor.r, dither);
         rgba8.g = AiQuantize8bit(minx, miny, 1, outColor.g, dither);
         rgba8.b = AiQuantize8bit(minx, miny, 2, outColor.b, dither);
         rgba8.a = AiQuantize8bit(minx, miny, 2, outColor.a, dither);

         AtRGBA8 *glBuffer = m_gl->GetBuffer();      
         int pixelIndex;
         for (int y = miny; y < maxy; ++y)
         {
            pixelIndex = minx + (y * m_width);
            for (int x = minx; x < maxx; ++x, pixelIndex++)
            {
               // original color
               rgbaBuffer[pixelIndex] = rgba;

               // Color-Corrected RGBA8 color
               glBuffer[pixelIndex] = rgba8;            
            }
         }
      } else
      {
         // This buffer is not being displayed
         // so just fill RgbaBuffer with the original, unmodified color
         int pixelIndex;
         for (int y = miny; y < maxy; ++y)
         {
            pixelIndex = minx + (y * m_width);
            for (int x = minx; x < maxx; ++x, pixelIndex++)
            {
               // original color
               rgbaBuffer[pixelIndex] = rgba;
            }
         }
      }
   }

   void SetShowRenderingTiles(bool b) {m_showRenderingTiles = b;}
   bool GetShowRenderingTiles() const {return m_showRenderingTiles;}
   
   void RefreshGLBuffer();
   void CheckSceneUpdates();

   void SetStatus (const std::string &status)
   {
      m_statusLog = status;
      DirtyStatusBar();
   }
   void DirtyStatusBar()
   {
      m_statusChanged = true;  
   }

   void RefreshStatusBar(int *mousePosition = NULL);

   void RestoreContinuous() {if (m_continuousUpdates)m_restoreContinuous = true;}
   void PickShape(int x, int y);
   void ClearPicking();
   void SetDebugShading(RenderViewDebugShading d);
   void ObjectNameChanged(const std::string &newName, const std::string &oldName);

   static void SelectionChangedCallback(void *);
   
   AtRvColorMode m_colorMode;
protected:

friend class CRenderViewMainWindow;

   struct StoredSnapshot {
      StoredSnapshot() : buffer(NULL), width(0), height(0) {}
      ~StoredSnapshot() {/*if (buffer) AiFree(buffer);*/}
      AtRGBA* buffer;
      int width;
      int height;
      std::string status;
   };

   void Init();
   void ManageDebugShading();
   
   void FillGLPixel(const AtRGBA &color, int x, int y, int pixelIndex)
   {
      AtRGBA outColor(color);

      ApplyLUTs(outColor);
      
      // if picking in progress, compare the picked ID with the ID AOV
      if (m_pickedId)
      {
         AiCritSecEnter(&m_pickLock);
         //int pixelId = *((int*)(&m_aovBuffers.back()[x + y * m_width].r));
         int pixelId = reinterpret_type<float, int>(m_aovBuffers.back()[pixelIndex].r);

         if ( m_pickedId && pixelId == *m_pickedId )
         {
            // mix the color with white (50%)
            outColor.rgb() *= 0.5f;
            outColor.rgb() += 0.5f;
         }
         AiCritSecLeave(&m_pickLock);
      }
      // Now let'sfill the GLWidget's RGBA8 buffer
      AtRGBA8 &rgba8 = m_gl->GetBuffer()[pixelIndex];
      rgba8.r = AiQuantize8bit(x, y, 0, outColor.r, m_colorCorrectSettings.dither);
      rgba8.g = AiQuantize8bit(x, y, 1, outColor.g, m_colorCorrectSettings.dither);
      rgba8.b = AiQuantize8bit(x, y, 2, outColor.b, m_colorCorrectSettings.dither);
      rgba8.a = AiQuantize8bit(x, y, 2, outColor.a, m_colorCorrectSettings.dither);
   }

   void ApplyLUTs(AtRGBA &color)
   {
      if (m_displayID)
      {
         // basic pseudo-random color per ID.
         // I tried using AiVCellNoise3, but some geometries 
         // were displayed with the same color. 
         // For now let's use this dirty hash         
         unsigned int val;
         //val = *((unsigned int*)&rgba.r);
         val = reinterpret_type<float, unsigned int>(color.r);
         color.r = (float((val+943) % 1257))/1257.f;
         color.g = (float((val+189) % 438))/438.f;
         color.b = (float((val+789) % 939))/939.f;
         return;
      }
      AtRGB &rgb = color.rgb();
      
      // un-premult for the color correction
      static float oneMinusEpsilon = 1.f - AI_EPSILON;
      bool unpremult = (color.a < oneMinusEpsilon  && color.a > AI_EPSILON);
      if (unpremult)
      {
         float invAlpha = 1.f / color.a;
         rgb *= invAlpha;
      }

      if (m_colorCorrectSettings.space == RV_COLOR_SPACE_SRGB)
      {
         //  (x <= 0.04045) ? x * (1.0 / 12.92) : powf((x + 0.055) * (1.0 / (1 + 0.055)), 2.4);

         rgb.r = (rgb.r <= 0.0031308f) ? rgb.r * 12.92f : (1.055) * powf(rgb.r,1.f/2.4f) - 0.055f;
         rgb.g = (rgb.g <= 0.0031308f) ? rgb.g * 12.92f : (1.055) * powf(rgb.g,1.f/2.4f) - 0.055f;
         rgb.b = (rgb.b <= 0.0031308f) ? rgb.b * 12.92f : (1.055) * powf(rgb.b,1.f/2.4f) - 0.055f;

      } else if (m_colorCorrectSettings.space == RV_COLOR_SPACE_REC709)
      {
         rgb.r = (rgb.r >= 0.018f) ? (1.099 * (powf(rgb.r, 0.45f))) - 0.099 : (4.5f * rgb.r);
         rgb.g = (rgb.g >= 0.018f) ? (1.099 * (powf(rgb.g, 0.45f))) - 0.099 : (4.5f * rgb.g);
         rgb.b = (rgb.b >= 0.018f) ? (1.099 * (powf(rgb.b, 0.45f))) - 0.099 : (4.5f * rgb.b);

         /*
            Reverse : From Rec709 To Lin
            if value >= 0.081:
            return ((value + 0.099) / 1.099) ** (1/0.45)
            else:
            return value / 4.5
         */
      } else if (m_colorCorrectSettings.space == RV_COLOR_SPACE_LUT_FILE && m_colorCorrectSettings.lut3d != NULL)
      {
         m_colorCorrectSettings.lut3d->ApplyLUT(rgb);
      }
      if (m_colorCorrectSettings.exposure != 0.f)
      {
         rgb *= m_colorCorrectSettings.exposureFactor;
      }
      if (m_colorCorrectSettings.gamma != 1.0f)
      {

         AiColorClamp(rgb, rgb, 0, 1);
         AiColorGamma(&rgb, m_colorCorrectSettings.gamma);
      }
      if (unpremult)
      {
         rgb *= color.a;
      }
   }
   AtRGBA *GetDisplayedBuffer()
   {
      return (m_displayedImageIndex < 0) ? 
               ((m_displayedAovIndex < 0) ? m_buffer : m_aovBuffers[m_displayedAovIndex]) :
               m_storedSnapshots[m_displayedImageIndex].buffer;
   }
   const std::string &GetDisplayedStatus() const
   {
      return (m_displayedImageIndex < 0) ? m_statusLog : m_storedSnapshots[m_displayedImageIndex].status;
   }

   int m_width;
   int m_height;
   CRenderViewMainWindow *m_mainWindow;
   QWidget *m_centralWidget;
   CRenderGLWidget *m_gl;
   AtDisplaySync *displaySync;
   
   bool m_showRenderingTiles;
   bool m_progressiveRefinement;
   bool m_regionCrop; 

   void *m_renderThread;
   bool m_continuousUpdates;


   AtRGBA *m_buffer;
   std::vector<StoredSnapshot> m_storedSnapshots;
   std::vector<AtRGBA *> m_aovBuffers;
   std::vector<std::string> m_aovNames;

   int m_displayedImageIndex;
   int m_displayedAovIndex;
   CRenderViewCCSettings m_colorCorrectSettings;

   bool m_displayID;
   int *m_pickedId;
   std::string m_statusLog;
   bool m_statusChanged;
   bool m_restoreContinuous;
   bool m_statusBarEnabled;
   bool m_statusBarPixelInfo;
   CRvShadingManager  m_shadingManager;
   RenderViewDebugShading m_debugShading;   
};


// This structure comes from the Kick API
// it is not really adapted to Qt
// so several things here have to be changed / removed
struct AtDisplaySync
{
   // spin lock would be better here, but it's not in the API
   AtCritSec lock;
   AtCritSec eventLock;
   
   // last time a draw or interrupt was done
   AtUInt64 lastDrawTime;
   AtUInt64 lastInterruptTime;

   // region to be updated
   AtBBox2 frontUpdateRegion;

   // back buffer to avoid screen tearing
   AtBBox2 backUpdateRegion;
   bool backBufferReady;

   // do we need to draw or interrupt after a timeout?
   bool waitingDraw;
   bool waitingInterrupt;   
   // for detecting full frame render completion
   int bucketsLeft;
   
   int previousAASamples;
   bool interrupted;   
};


