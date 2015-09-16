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

///////////////////////////////////////////////
// 2D MANIPULATORS
CRenderView2DManipulator::CRenderView2DManipulator(CRenderView &rv, int x, int y) : CRenderViewManipulator(rv, x, y)
{
   m_renderView.getGlWidget()->getPan(m_previousPan[0], m_previousPan[1]);
   m_previousZoom = m_renderView.getGlWidget()->getZoomFactor();
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
CRenderView2DZoom::CRenderView2DZoom(CRenderView &rv, int x, int y) : CRenderView2DManipulator(rv, x, y)
{
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

/////////////////////////////////////////////////////////////////////////////
// 3D MANIPULATOR
// 3D PAN
CRenderView3DManipulator::CRenderView3DManipulator(CRenderView &rv, int x, int y) : CRenderViewManipulator(rv, x, y)
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;

   
   MSelectionList camList;
   camList.add(MString(AiNodeGetStr(arnold_camera, "name")));

   MDagPath camDag;
   camList.getDagPath(0, camDag);

   if (!camDag.isValid()) return;
   MObject camNode = camDag.node();

    m_camera.setObject(camDag);
      

   m_original_matrix = camDag.inclusiveMatrix();
   MStatus status;
   m_original_position = m_camera.eyePoint(MSpace::kWorld, &status);
   
}
CRenderView3DManipulator::~CRenderView3DManipulator()
{
}

void CRenderView3DManipulator::mouseMove(int x, int y)
{

}

void CRenderView3DManipulator::mouseRelease(int x, int y)
{
}


//////////////////////////////////////////////
// 3D PAN
CRenderView3DPan::CRenderView3DPan(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y)
{
   m_up_direction = m_camera.upDirection(MSpace::kWorld);
   m_right_direction = m_camera.rightDirection(MSpace::kWorld);
   m_view_direction = m_camera.viewDirection(MSpace::kWorld);

}
CRenderView3DPan::~CRenderView3DPan()
{
}

void CRenderView3DPan::mouseMove(int x, int y)
{
   int deltaX = x - m_start_x;
   int deltaY = y - m_start_y;

   // arbitrary multiplier 
   deltaX *= -0.1f;
   deltaY *= 0.1f;

   MPoint new_position = m_original_position + m_right_direction * deltaX + m_up_direction * deltaY;
   m_camera.set(new_position, m_view_direction, m_up_direction, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
 
}


//////////////////////////////////////////////
// 3D ZOOM
CRenderView3DZoom::CRenderView3DZoom(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y)
{
   m_view_direction = m_camera.viewDirection(MSpace::kWorld);
   m_up_direction = m_camera.upDirection(MSpace::kWorld);
   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
}
CRenderView3DZoom::~CRenderView3DZoom()
{

}

void CRenderView3DZoom::mouseMove(int x, int y)
{
   int deltaX = x - m_start_x;
   int deltaY = y - m_start_y;

   int delta = deltaX * 0.2f;

   MPoint new_position = m_original_position + m_view_direction * delta;
   m_camera.set(new_position, m_view_direction, m_up_direction, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
   m_camera.setCenterOfInterestPoint(m_center, MSpace::kWorld);
}


void CRenderView3DZoom::wheel(CRenderView &renderView, float delta)
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;

   
   MSelectionList camList;
   camList.add(MString(AiNodeGetStr(arnold_camera, "name")));

   MDagPath camDag;
   camList.getDagPath(0, camDag);

   if (!camDag.isValid()) return;
   MObject camNode = camDag.node();
   MFnCamera camera;
   camera.setObject(camDag);
      
   MVector view_direction = camera.viewDirection(MSpace::kWorld);
   MPoint original_position = camera.eyePoint(MSpace::kWorld);

   delta /= 120.f;
   MPoint center = camera.centerOfInterestPoint(MSpace::kWorld);
   float center_dist = center.distanceTo(original_position);
   delta *= center_dist / 10.f;
   MPoint new_position = original_position;
   MVector zoom = view_direction;
   zoom *= delta;
   new_position +=  zoom;
   camera.set(new_position, view_direction, camera.upDirection(MSpace::kWorld), camera.horizontalFieldOfView(), camera.aspectRatio());
   camera.setCenterOfInterestPoint(center, MSpace::kWorld);
}

/////////////////////////////////////////////////////////
// 3D ROTATE
CRenderView3DRotate::CRenderView3DRotate(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y)
{
   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   m_center_dist = m_center.distanceTo(m_original_position);
   m_up_direction = m_camera.upDirection(MSpace::kWorld);
}
CRenderView3DRotate::~CRenderView3DRotate()
{

}

void CRenderView3DRotate::mouseMove(int x, int y)
{
   int deltaX = x - m_start_x;
   int deltaY = y - m_start_y;

   // arbitrary multiplier 
   deltaX *= -0.1f;
   deltaY *= 0.1f;

   MPoint new_position = m_original_position + m_camera.rightDirection(MSpace::kWorld) * deltaX + m_up_direction * deltaY;
   MVector dir = new_position - m_center;
   dir.normalize();
   new_position = m_center;
   new_position += m_center_dist * dir;

   m_camera.set(new_position, -dir, m_up_direction, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
   m_camera.setCenterOfInterestPoint(m_center, MSpace::kWorld);
}


/*

CRenderView3DRotate::CRenderView3DRotate(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y)
{
   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   m_center_dist = m_center.distanceTo(m_original_position);
   m_up_direction = m_camera.upDirection(MSpace::kWorld);
   m_view_direction = m_original_position - m_center;

   // startO = m_view_direction
   // r = m_center_dist



      if (!m_view_direction.z)
      {
         m_orig_lon = m_original_position.x > 0.0 ? AI_PI/2.0 : AI_PI * 3.0 / 2.0;
      }
      else
      {
         m_orig_lon = atan(m_view_direction.x / -m_view_direction.z);
         if (m_view_direction.z > 0.0) m_orig_lon += AI_PI;
         if (m_view_direction.z < 0.0 && m_view_direction.x < 0.0) m_orig_lon += 2*AI_PI;
      }
      m_orig_lat = acos(m_view_direction.y / m_center_dist);

}
CRenderView3DRotate::~CRenderView3DRotate()
{

}

void CRenderView3DRotate::mouseMove(int x, int y)
{
   int deltaX = x - m_start_x;
   int deltaY = y - m_start_y;

   // arbitrary multiplier 
   deltaX *= -0.01f;
   deltaY *= 0.01f;


   float newLon = m_orig_lon - deltaX;
   float newLat = m_orig_lat - deltaY;
            
   MVector newO;
   newO.x = m_center_dist * sin(newLat) * sin(newLon);
   newO.y = m_center_dist * cos(newLat);
   newO.z = -(m_center_dist * sin(newLat) * cos(newLon));

   MVector newPos = m_original_position + newO - m_view_direction;
   MVector dir = newPos - m_center;
   dir.normalize();
   newPos = m_center;
   newPos += dir * m_center_dist;



   MPoint new_position = m_original_position + m_camera.rightDirection(MSpace::kWorld) * deltaX + m_up_direction * deltaY;
   dir = new_position - m_center;
   dir.normalize();
   new_position = m_center;
   new_position += m_center_dist * dir;
   
   m_camera.set(newPos, -dir, m_up_direction, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());

}

*/
