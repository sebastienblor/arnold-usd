/*
 * Windows window and event handling
 */

#include <time.h>
#include <string.h>
#include <sys/timeb.h>
#include <deque>

// Assume Windows XP as the build target
#define _WIN32_WINNT    0x501
#define _WIN32_WINDOWS  0x501

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include "manipulators.h"



#include "utility.h"
#include <emmintrin.h>
#include <intrin.h>

#include "renderview.h"

#include "scene/MayaScene.h"
#include "session/ArnoldSession.h"


#pragma warning (disable : 4244)



void CRenderViewManipulator::mouseMove(int x, int y)
{

}

void CRenderViewManipulator::mouseRelease(int x, int y)
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

void CRenderViewCropRegion::mouseMove(int x, int y)
{
   if (x == m_start_x || y == m_start_y) return;

   // drag region from m_pickPoint and current Point

   int bufferStart[2];
   int bufferEnd[2];
   CRenderGLWidget *glWidget = m_renderView.getGlWidget();
   glWidget->project(m_start_x, m_start_y, bufferStart[0], bufferStart[1], true);
   glWidget->project(x, y, bufferEnd[0], bufferEnd[1], true);
   glWidget->setRegionCrop(bufferStart[0], bufferStart[1], bufferEnd[0], bufferEnd[1]);
}

void CRenderViewCropRegion::mouseRelease(int x, int y)
{
   int regionEnd[2];
   regionEnd[0] = x;
   regionEnd[1] = y;

   CRenderGLWidget *glWidget = m_renderView.getGlWidget();

   if (ABS(regionEnd[0] - m_start_x) < 3 || ABS(regionEnd[1] - m_start_y) < 3)
   {
      // Region is too small -> let's remove the region crop
      m_start_x = m_start_y = -1;
      glWidget->clearRegionCrop();
      m_renderView.interruptRender();
      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      renderSession->SetRegion(0, 0 , m_renderView.width(), m_renderView.height());
      AtNode *options = AiUniverseGetOptions();
      AiNodeSetInt(options, "region_min_x", -1);
      AiNodeSetInt(options, "region_min_y", -1);
      AiNodeSetInt(options, "region_max_x", -1);
      AiNodeSetInt(options, "region_max_y", -1);

      m_renderView.restartRender();
      return;
   }

   // draw region from m_pickPoint and current Point
   // set region in Arnold

   int bufferStart[2];
   int bufferEnd[2];
   glWidget->project(m_start_x, m_start_y, bufferStart[0], bufferStart[1], true);
   glWidget->project(regionEnd[0], regionEnd[1], bufferEnd[0], bufferEnd[1], true);
   glWidget->setRegionCrop(bufferStart[0], bufferStart[1], bufferEnd[0], bufferEnd[1]);

   
   m_renderView.interruptRender();
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   renderSession->SetRegion(bufferStart[0], bufferStart[1], bufferEnd[0], bufferEnd[1]);
   AtNode *options = AiUniverseGetOptions();
   AiNodeSetInt(options, "region_min_x", bufferStart[0]);
   AiNodeSetInt(options, "region_min_y", bufferStart[1]);
   AiNodeSetInt(options, "region_max_x", bufferEnd[0]);
   AiNodeSetInt(options, "region_max_y", bufferEnd[1]);
   
   m_renderView.restartRender();
}

//////////////////////////////////////////////
// 2D PAN
CRenderView2DPan::CRenderView2DPan(CRenderView &rv, int x, int y) : CRenderViewManipulator(rv, x, y)
{
   m_renderView.getGlWidget()->getPan(m_previousPan[0], m_previousPan[1]);
   m_previousZoom = m_renderView.getGlWidget()->getZoomFactor();
}
CRenderView2DPan::~CRenderView2DPan()
{

}

void CRenderView2DPan::mouseMove(int x, int y)
{
   m_renderView.getGlWidget()->setPan(x - m_start_x + m_previousPan[0], y - m_start_y + m_previousPan[1]);
   m_renderView.draw();
}

void CRenderView2DPan::mouseRelease(int x, int y)
{
}


//////////////////////////////////////////////
// 2D ZOOM
CRenderView2DZoom::CRenderView2DZoom(CRenderView &rv, int x, int y) : CRenderViewManipulator(rv, x, y)
{
   m_renderView.getGlWidget()->getPan(m_previousPan[0], m_previousPan[1]);
   m_previousZoom = m_renderView.getGlWidget()->getZoomFactor();
}
CRenderView2DZoom::~CRenderView2DZoom()
{

}

void CRenderView2DZoom::mouseMove(int x, int y)
{
   float zoomFactor = powf(2.f, ((float)(x - m_start_x) / (float)m_renderView.width()));
   zoomFactor *= m_previousZoom;
   m_renderView.getGlWidget()->setZoomFactor(zoomFactor);

   m_renderView.draw();
}

void CRenderView2DZoom::mouseRelease(int x, int y)
{
}

void CRenderView2DZoom::wheel(CRenderView &renderView, float delta)
{
   CRenderGLWidget *glWidget = renderView.getGlWidget();

   float previousZoom = glWidget->getZoomFactor();
   float zoomFactor = powf(2.f, delta / 240.0);

   int pivot[2];
   glWidget->project(int(renderView.getMainWindow()->width() * 0.5), int (renderView.getMainWindow()->height() * 0.5), pivot[0], pivot[1], true);

   AtPoint2 regionCenter;
   // this was the previous image center
   regionCenter.x = pivot[0] - renderView.width()*0.5;
   regionCenter.y = pivot[1] - renderView.height()*0.5;

   zoomFactor *= previousZoom;
   glWidget->setZoomFactor(zoomFactor);

   // I want my image center to be the same as before
   glWidget->setPan(-regionCenter.x * zoomFactor,-regionCenter.y*zoomFactor);
   renderView.draw();

}
