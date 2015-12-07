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
      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      renderSession->SetRegion(0, 0 , m_renderView.Width(), m_renderView.Height());
      AtNode *options = AiUniverseGetOptions();
      AiNodeSetInt(options, "region_min_x", -1);
      AiNodeSetInt(options, "region_min_y", -1);
      AiNodeSetInt(options, "region_max_x", -1);
      AiNodeSetInt(options, "region_max_y", -1);

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
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   renderSession->SetRegion(bufferStart[0], bufferStart[1], bufferEnd[0], bufferEnd[1]);
   AtNode *options = AiUniverseGetOptions();
   AiNodeSetInt(options, "region_min_x", bufferStart[0]);
   AiNodeSetInt(options, "region_min_y", bufferStart[1]);
   AiNodeSetInt(options, "region_max_x", bufferEnd[0]);
   AiNodeSetInt(options, "region_max_y", bufferEnd[1]);
   
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
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;
   
   MSelectionList camList;
   camList.add(MString(AiNodeGetStr(arnold_camera, "name")));

   camList.getDagPath(0, m_cameraPath);

   if (!m_cameraPath.isValid()) return;

   MObject camNode = m_cameraPath.node();

   m_camera.setObject(m_cameraPath);      

   m_originalMatrix = m_cameraPath.inclusiveMatrix();
   MStatus status;
   m_originalPosition = m_camera.eyePoint(MSpace::kWorld, &status);
   
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
CRenderView3DPan::CRenderView3DPan(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y)
{
   m_upDirection = m_camera.upDirection(MSpace::kWorld);
   m_rightDirection = m_camera.rightDirection(MSpace::kWorld);
   m_viewDirection = m_camera.viewDirection(MSpace::kWorld);

   m_distFactor = 1.f;
   AtNode *cam = AiUniverseGetCamera ();
   if (cam == NULL) return; // can this happen ?....
   
   if (strcmp (AiNodeEntryGetName(AiNodeGetNodeEntry(cam)), "persp_camera") != 0) return;

   MPoint originalPosition = m_camera.eyePoint(MSpace::kWorld);
   MPoint center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   float center_dist = center.distanceTo(originalPosition);

   m_distFactor = center_dist * tanf(AiNodeGetFlt(cam, "fov") * AI_DTOR);

}
CRenderView3DPan::~CRenderView3DPan()
{
}

void CRenderView3DPan::MouseMove(int x, int y)
{

   // get the start and current mouse coordinates in image space
   int imageStart[2];
   int imagePoint[2];
   CRenderGLWidget *glWidget = m_renderView.GetGlWidget();
   glWidget->Project(m_startX, m_startY, imageStart[0], imageStart[1], true);
   glWidget->Project(x, y, imagePoint[0], imagePoint[1], true);

   // get the delta factor relative to the width 
   float delta[2];
   delta[0] = (-m_distFactor * (imagePoint[0] - imageStart[0]) / (m_renderView.Width()));
   delta[1] = (m_distFactor * (imagePoint[1] - imageStart[1]) / (m_renderView.Width()));

   MPoint newPosition = m_originalPosition + m_rightDirection * delta[0] + m_upDirection * delta[1];
   m_camera.set(newPosition, m_viewDirection, m_upDirection, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
}


//////////////////////////////////////////////
// 3D ZOOM
CRenderView3DZoom::CRenderView3DZoom(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y)
{
   m_viewDirection = m_camera.viewDirection(MSpace::kWorld);
   m_upDirection = m_camera.upDirection(MSpace::kWorld);
   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   m_dist = m_center.distanceTo(m_originalPosition);
}

CRenderView3DZoom::~CRenderView3DZoom()
{

}

void CRenderView3DZoom::MouseMove(int x, int y)
{
   int deltaX = x - m_startX;

   float delta = (float)deltaX / (float)m_renderView.Width();
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

   MPoint newPosition = m_originalPosition + m_viewDirection * delta;
   m_camera.set(newPosition, m_viewDirection, m_upDirection, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
   m_camera.setCenterOfInterestPoint(m_center, MSpace::kWorld);
}

void CRenderView3DZoom::Wheel(CRenderView &renderView, float delta)
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
   MPoint originalPosition = camera.eyePoint(MSpace::kWorld);

   delta /= 120.f;
   MPoint center = camera.centerOfInterestPoint(MSpace::kWorld);
   float center_dist = center.distanceTo(originalPosition);
   delta *= center_dist / 10.f;
   MPoint newPosition = originalPosition;
   MVector zoom = view_direction;
   zoom *= delta;
   newPosition +=  zoom;
   camera.set(newPosition, view_direction, camera.upDirection(MSpace::kWorld), camera.horizontalFieldOfView(), camera.aspectRatio());
   camera.setCenterOfInterestPoint(center, MSpace::kWorld);
}

/////////////////////////////////////////////////////////
// 3D ROTATE
CRenderView3DRotate::CRenderView3DRotate(CRenderView &rv, int x, int y) : CRenderView3DManipulator(rv, x, y)
{
   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   m_centerDist = m_center.distanceTo(m_originalPosition);
   m_upDirection = m_camera.upDirection(MSpace::kWorld);
   
}
CRenderView3DRotate::~CRenderView3DRotate()
{

}

void CRenderView3DRotate::MouseMove(int x, int y)
{

   MStatus status;
   MDagPath camTransform = m_cameraPath;
   camTransform.pop();
   MFnTransform transformPath(camTransform, &status);
   if (status != MS::kSuccess) 
      return;
   

   MVector rightDirection = m_camera.rightDirection(MSpace::kWorld);
   MVector fElevationAxis = -rightDirection;
   fElevationAxis.normalize();

   MPoint previousRp = transformPath.rotatePivot(MSpace::kWorld);
   MVector previousRt = transformPath.rotatePivotTranslation (MSpace::kWorld);

   MPoint fPivotPoint = m_center;
   transformPath.setRotatePivot(fPivotPoint, MSpace::kWorld, true);
   MMatrix matrix = camTransform.inclusiveMatrix();
   
   MEulerRotation rot;

   transformPath.getRotation(rot);
   
   rot.incrementalRotateBy (fElevationAxis, .01f * (y - m_startY));
   
   rot.incrementalRotateBy (MGlobal::upAxis(), - .01f * (x - m_startX));

   // start_x being "previous_x" here
   m_startX = x;
   m_startY = y;

   transformPath.setRotation(rot);

   transformPath.setRotatePivot (previousRp, MSpace::kWorld, true);
   MVector nextRt = transformPath.rotatePivotTranslation (MSpace::kWorld);

   transformPath.translateBy(nextRt - previousRt, MSpace::kWorld);
   transformPath.setRotatePivotTranslation(previousRt, MSpace::kWorld);
   m_camera.setCenterOfInterestPoint(m_center, MSpace::kWorld);
  
}
