/*
 * Windows window and event handling
 */

#include <time.h>
#include <string.h>
#include <sys/timeb.h>
#include <deque>

#ifdef _WIN64
// Assume Windows XP as the build target
#define _WIN32_WINNT    0x501
#define _WIN32_WINDOWS  0x501

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <intrin.h>
#endif

#include "manipulators.h"
#include "utility.h"
#include <emmintrin.h>
#include "renderview.h"


void CRenderViewManipulator::MouseMove(int x, int y)
{

}

void CRenderViewManipulator::MouseRelease(int x, int y)
{
   
}

//////////////////////////////////////////////
// CROP REGION

CRenderViewCropRegion::CRenderViewCropRegion(CRenderView &rv, int x, int y) : CRenderViewManipulator(rv, x, y)
{

}
CRenderViewCropRegion::~CRenderViewCropRegion()
{

}

void CRenderViewCropRegion::MouseMove(int x, int y)
{
   if (x == m_startX || y == m_startY) return;

   // drag region from m_pickPoint and current Point

   int bufferStart[2];
   int bufferEnd[2];
   CRenderGLWidget *glWidget = m_renderView.GetGlWidget();
   glWidget->Project(m_startX, m_startY, bufferStart[0], bufferStart[1], true);
   glWidget->Project(x, y, bufferEnd[0], bufferEnd[1], true);
   bufferStart[0] = CLAMP(bufferStart[0], 0, m_renderView.Width() - 1);
   bufferStart[1] = CLAMP(bufferStart[1], 0, m_renderView.Height() - 1);

   bufferEnd[0] = CLAMP(bufferEnd[0], 0, m_renderView.Width() - 1);
   bufferEnd[1] = CLAMP(bufferEnd[1], 0, m_renderView.Height() - 1);

   if (bufferEnd[0] < bufferStart[0]) std::swap(bufferStart[0], bufferEnd[0]);
   if (bufferEnd[1] < bufferStart[1]) std::swap(bufferStart[1], bufferEnd[1]);


   glWidget->SetRegionCrop(bufferStart[0], bufferStart[1], bufferEnd[0], bufferEnd[1]);

   glWidget->ReloadBuffer(m_renderView.m_colorMode);
   // refresh the Region
}

void CRenderViewCropRegion::MouseRelease(int x, int y)
{
   int regionEnd[2];
   regionEnd[0] = x;
   regionEnd[1] = y;

   CRenderGLWidget *glWidget = m_renderView.GetGlWidget();

   if (ABS(regionEnd[0] - m_startX) < 3 || ABS(regionEnd[1] - m_startY) < 3)
   {
      // Region is too small -> let's remove the region crop
      m_startX = m_startY = -1;
      glWidget->ClearRegionCrop();
      m_renderView.InterruptRender();

      AtNode *options = AiUniverseGetOptions();
      AiNodeSetInt(options, "region_min_x", -1);
      AiNodeSetInt(options, "region_min_y", -1);
      AiNodeSetInt(options, "region_max_x", -1);
      AiNodeSetInt(options, "region_max_y", -1);

      m_renderView.GetInterface().NodeParamChanged(options, "region_min_x");
      m_renderView.RestartRender();
      return;
   }

   // draw region from m_pickPoint and current Point
   // set region in Arnold

   int bufferStart[2];
   int bufferEnd[2];
   glWidget->Project(m_startX, m_startY, bufferStart[0], bufferStart[1], true);
   glWidget->Project(regionEnd[0], regionEnd[1], bufferEnd[0], bufferEnd[1], true);

   bufferStart[0] = CLAMP(bufferStart[0], 0, m_renderView.Width() - 1);
   bufferStart[1] = CLAMP(bufferStart[1], 0, m_renderView.Height() - 1);

   bufferEnd[0] = CLAMP(bufferEnd[0], 0, m_renderView.Width() - 1);
   bufferEnd[1] = CLAMP(bufferEnd[1], 0, m_renderView.Height() - 1);

   if (bufferEnd[0] < bufferStart[0]) std::swap(bufferStart[0], bufferEnd[0]);
   if (bufferEnd[1] < bufferStart[1]) std::swap(bufferStart[1], bufferEnd[1]);

   glWidget->SetRegionCrop(bufferStart[0], bufferStart[1], bufferEnd[0], bufferEnd[1]);

   
   m_renderView.InterruptRender();
   
   AtNode *options = AiUniverseGetOptions();
   AiNodeSetInt(options, "region_min_x", bufferStart[0]);
   AiNodeSetInt(options, "region_min_y", bufferStart[1]);
   AiNodeSetInt(options, "region_max_x", bufferEnd[0]);
   AiNodeSetInt(options, "region_max_y", bufferEnd[1]);

   m_renderView.GetInterface().NodeParamChanged(options, "region_min_x");
   m_renderView.RestartRender();
}

///////////////////////////////////////////////
// 2D MANIPULATORS
CRenderView2DManipulator::CRenderView2DManipulator(CRenderView &rv, int x, int y) : CRenderViewManipulator(rv, x, y)
{
   m_renderView.GetGlWidget()->GetPan(m_previousPan[0], m_previousPan[1]);
   m_previousZoom = m_renderView.GetGlWidget()->GetZoomFactor();
}
CRenderView2DManipulator::~CRenderView2DManipulator()
{
}

//////////////////////////////////////////////
// 2D PAN
CRenderView2DPan::CRenderView2DPan(CRenderView &rv, int x, int y) : CRenderView2DManipulator(rv, x, y)
{   
}
CRenderView2DPan::~CRenderView2DPan()
{
}

void CRenderView2DPan::MouseMove(int x, int y)
{
   m_renderView.GetGlWidget()->SetPan(x - m_startX + m_previousPan[0], y - m_startY + m_previousPan[1]);
   m_renderView.Draw();
}

void CRenderView2DPan::MouseRelease(int x, int y)
{
}


//////////////////////////////////////////////
// 2D ZOOM
CRenderView2DZoom::CRenderView2DZoom(CRenderView &rv, int x, int y) : CRenderView2DManipulator(rv, x, y)
{
}
CRenderView2DZoom::~CRenderView2DZoom()
{
}

void CRenderView2DZoom::MouseMove(int x, int y)
{
   float zoomFactor = powf(2.f, ((float)(x - m_startX) / (float)m_renderView.Width()));
   zoomFactor *= m_previousZoom;
   m_renderView.GetGlWidget()->SetZoomFactor(zoomFactor);

   m_renderView.Draw();
}

void CRenderView2DZoom::MouseRelease(int x, int y)
{
}


void CRenderView2DZoom::Wheel(CRenderView &renderView, float delta)
{
   CRenderGLWidget *glWidget = renderView.GetGlWidget();

   float previousZoom = glWidget->GetZoomFactor();
   float zoomFactor = powf(2.f, delta / 240.0);

//   int pivot[2];
//   glWidget->Project(int(renderView.getMainWindow()->width() * 0.5), int (renderView.getMainWindow()->height() * 0.5), pivot[0], pivot[1], true);

   
   zoomFactor *= previousZoom;
   glWidget->SetZoomFactor(zoomFactor);

/*
   AtPoint2 regionCenter;
   // this was the previous image center
   regionCenter.x = pivot[0] - renderView.width()*0.5;
   regionCenter.y = pivot[1] - (renderView.height())*0.5;

   // I want my image center to be the same as before
   glWidget->SetPan(int(-regionCenter.x * zoomFactor),int(-regionCenter.y*zoomFactor));
   */
   renderView.Draw();

}

/////////////////////////////////////////////////////////////////////////////
// 3D MANIPULATOR
// 3D PAN
CRenderView3DManipulator::CRenderView3DManipulator(CRenderView &rv, int x, int y) : CRenderViewManipulator(rv, x, y)
{
   m_interface = &rv.GetInterface();

}
CRenderView3DManipulator::~CRenderView3DManipulator()
{
}

void CRenderView3DManipulator::MouseMove(int x, int y)
{

}

void CRenderView3DManipulator::MouseRelease(int x, int y)
{
}


//////////////////////////////////////////////
// 3D PAN
CRenderView3DPan::CRenderView3DPan(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y),
                                                                     m_panManipulator(NULL)
{   
   if (m_interface == NULL) return;
   m_panManipulator = m_interface->GetPanManipulator();
}
CRenderView3DPan::~CRenderView3DPan()
{
   delete m_panManipulator;
}

void CRenderView3DPan::MouseMove(int x, int y)
{
   if (m_panManipulator == NULL) return;

   // get the start and current mouse coordinates in image space
   int imageStart[2];
   int imagePoint[2];
   CRenderGLWidget *glWidget = m_renderView.GetGlWidget();
   glWidget->Project(m_startX, m_startY, imageStart[0], imageStart[1], true);
   glWidget->Project(x, y, imagePoint[0], imagePoint[1], true);

   int delta[2];
   delta[0] = (imagePoint[0] - imageStart[0]);
   delta[1] = (imagePoint[1] - imageStart[1]);

   m_panManipulator->MouseDelta(delta[0], delta[1]);
}


//////////////////////////////////////////////
// 3D ZOOM
CRenderView3DZoom::CRenderView3DZoom(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y),
                                                                  m_zoomManipulator(NULL)
{
   if (m_interface == NULL) return;
   m_zoomManipulator = m_interface->GetZoomManipulator();
}

CRenderView3DZoom::~CRenderView3DZoom()
{
   delete m_zoomManipulator;
}

void CRenderView3DZoom::MouseMove(int x, int y)
{
   if (m_zoomManipulator == NULL) return;
   int deltaX = x - m_startX;
   int deltaY = y - m_startY;

   m_zoomManipulator->MouseDelta(deltaX, deltaY);

}

void CRenderView3DZoom::Wheel(CRenderView &renderView, float delta)
{
   CRenderViewZoomManipulator *manipulator = renderView.GetInterface().GetZoomManipulator();
   manipulator->WheelDelta(delta);

   delete manipulator;
}

void CRenderView3DZoom::FrameSelection(CRenderView &renderView)
{
   CRenderViewZoomManipulator *manipulator = renderView.GetInterface().GetZoomManipulator();
   manipulator->FrameSelection();

   delete manipulator;
/*

   */
}

/////////////////////////////////////////////////////////
// 3D ROTATE
CRenderView3DRotate::CRenderView3DRotate(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y)
{
   if (m_interface == NULL) return;
   m_rotateManipulator = m_interface->GetRotateManipulator();
   
}
CRenderView3DRotate::~CRenderView3DRotate()
{
   delete m_rotateManipulator;

}

void CRenderView3DRotate::MouseMove(int x, int y)
{
   if (m_rotateManipulator == NULL) return;
   int delta[2];
   delta[0] = x - m_startX;
   delta[1] = y - m_startY;

   m_rotateManipulator->MouseDelta(delta[0], delta[1]);

   // reset the manipulator start
   m_startX = x;
   m_startY = y;

}
