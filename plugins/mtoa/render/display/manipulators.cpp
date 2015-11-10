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
#include "scene/MayaScene.h"
#include "session/ArnoldSession.h"

#include <maya/MEulerRotation.h>
#include <maya/MFnTransform.h>

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

   glWidget->reloadBuffer(m_renderView.m_color_mode);
   // refresh the Region
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
   glWidget->setPan(int(-regionCenter.x * zoomFactor),int(-regionCenter.y*zoomFactor));
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

   camList.getDagPath(0, m_cameraPath);

   if (!m_cameraPath.isValid()) return;

   MObject camNode = m_cameraPath.node();

   m_camera.setObject(m_cameraPath);      

   m_original_matrix = m_cameraPath.inclusiveMatrix();
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

   m_dist_factor = 1.f;
   AtNode *cam = AiUniverseGetCamera ();
   if (cam == NULL) return; // can this happen ?....
   
   if (strcmp (AiNodeEntryGetName(AiNodeGetNodeEntry(cam)), "persp_camera") != 0) return;

   MPoint original_position = m_camera.eyePoint(MSpace::kWorld);
   MPoint center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   float center_dist = center.distanceTo(original_position);

   m_dist_factor = center_dist * tanf(AiNodeGetFlt(cam, "fov") * AI_DTOR);

}
CRenderView3DPan::~CRenderView3DPan()
{
}

void CRenderView3DPan::mouseMove(int x, int y)
{

   // get the start and current mouse coordinates in image space
   int imageStart[2];
   int imagePoint[2];
   CRenderGLWidget *glWidget = m_renderView.getGlWidget();
   glWidget->project(m_start_x, m_start_y, imageStart[0], imageStart[1], true);
   glWidget->project(x, y, imagePoint[0], imagePoint[1], true);

   // get the delta factor relative to the width 
   float delta[2];
   delta[0] = (-m_dist_factor * (imagePoint[0] - imageStart[0]) / (m_renderView.width()));
   delta[1] = (m_dist_factor * (imagePoint[1] - imageStart[1]) / (m_renderView.width()));

   MPoint new_position = m_original_position + m_right_direction * delta[0] + m_up_direction * delta[1];
   m_camera.set(new_position, m_view_direction, m_up_direction, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
}


//////////////////////////////////////////////
// 3D ZOOM
CRenderView3DZoom::CRenderView3DZoom(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y)
{
   m_view_direction = m_camera.viewDirection(MSpace::kWorld);
   m_up_direction = m_camera.upDirection(MSpace::kWorld);
   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   m_dist = m_center.distanceTo(m_original_position);
}

CRenderView3DZoom::~CRenderView3DZoom()
{

}

void CRenderView3DZoom::mouseMove(int x, int y)
{
   int deltaX = x - m_start_x;

   float delta = (float)deltaX / (float)m_renderView.width();
   if (delta > 0.9f)
   {
      float diff = delta - 0.9f;
      delta = 0.9f;
      while (diff > 0.f)
      {
         delta += MIN(diff, 1.f) * (1.f - delta) * 0.5f;
         diff -= 1.f;
      }
   }
   delta *= m_dist;   

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

   MStatus status;
   MDagPath camTransform = m_cameraPath;
   camTransform.pop();
   MFnTransform transformPath(camTransform, &status);
   if (status != MS::kSuccess) 
      return;
   

   MVector right_direction = m_camera.rightDirection(MSpace::kWorld);
   MVector fElevationAxis = -right_direction;
   fElevationAxis.normalize();

   MPoint previousRp = transformPath.rotatePivot(MSpace::kWorld);
   MVector previousRt = transformPath.rotatePivotTranslation (MSpace::kWorld);

   MPoint fPivotPoint = m_center;
   transformPath.setRotatePivot(fPivotPoint, MSpace::kWorld, true);
   MMatrix matrix = camTransform.inclusiveMatrix();
   
   MEulerRotation rot;

   transformPath.getRotation(rot);
   
   rot.incrementalRotateBy (fElevationAxis, .01f * (y - m_start_y));
   
   rot.incrementalRotateBy (MGlobal::upAxis(), - .01f * (x - m_start_x));

   // start_x being "previous_x" here
   m_start_x = x;
   m_start_y = y;

   transformPath.setRotation(rot);

   transformPath.setRotatePivot (previousRp, MSpace::kWorld, true);
   MVector nextRt = transformPath.rotatePivotTranslation (MSpace::kWorld);

   transformPath.translateBy(nextRt - previousRt, MSpace::kWorld);
   transformPath.setRotatePivotTranslation(previousRt, MSpace::kWorld);
   m_camera.setCenterOfInterestPoint(m_center, MSpace::kWorld);
  
}
