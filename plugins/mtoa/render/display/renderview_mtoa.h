
#pragma once

#include "renderview_interface.h"

#include <ai.h>

#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>


#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>
#include <maya/MMatrix.h>
/**
  * Preparing MtoA's interface code with the RenderView
  * Once the RenderView is extracted from MtoA, renderview_mtoa.cpp and renderview_mtoa.h
  * will be the only files left in MtoA repository
 **/


class CRenderViewMtoA : public CRenderViewInterface
{
public:

	CRenderViewMtoA() : CRenderViewInterface() {}
	virtual ~CRenderViewMtoA();

   virtual void UpdateSceneChanges();

   virtual unsigned int GetSelectionCount();
   virtual void GetSelection(AtNode **selectedNodes);
   virtual void SetSelection(const AtNode **selectedNodes, unsigned int selectionCount, bool append = false);
   virtual void ReceiveSelectionChanges(bool receive);
   virtual void NodeParamChanged(AtNode *node, const char *paramName);
   virtual void RenderViewClosed();

// In the Future these Manipulator classes should be removed and handled
// internally by the RenderView code. As of now, MtoA's manipulators
// still rely on some Maya functions so we need to extract it

   virtual CRenderViewPanManipulator *GetPanManipulator();
   virtual CRenderViewZoomManipulator *GetZoomManipulator();
   virtual CRenderViewRotateManipulator *GetRotateManipulator();
   
   static void SelectionChangedCallback(void *);
   static void SceneSaveCallback(void *);
   static void SceneOpenCallback(void *);

   void OpenMtoARenderView(int width, int height);
};

// In the Future these Manipulator classes should be removed and handled
// internally by the RenderView code. As of now, MtoA's manipulators
// still rely on some Maya functions so we need to extract it

// To be implemented
class CRenderViewMtoAPan : public CRenderViewPanManipulator
{
public:
   CRenderViewMtoAPan();
   virtual ~CRenderViewMtoAPan() {}

   virtual void MouseDelta(int deltaX, int deltaY);

private:

   MDagPath m_cameraPath;
   MFnCamera m_camera;
   
   MMatrix m_originalMatrix;
   MPoint m_originalPosition;

   MVector  m_upDirection;
   MVector  m_rightDirection;
   MVector  m_viewDirection;
   float    m_distFactor;
   int m_width;
   
};

class CRenderViewMtoAZoom : public CRenderViewZoomManipulator
{
public:

   CRenderViewMtoAZoom();
   virtual ~CRenderViewMtoAZoom() {}

   virtual void MouseDelta(int deltaX, int deltaY);
   virtual void WheelDelta(float delta);
   virtual void FrameSelection();

private:
   
   MDagPath m_cameraPath;
   MFnCamera m_camera;
   
   MMatrix m_originalMatrix;
   MPoint m_originalPosition;

   MVector  m_viewDirection;
   MVector  m_upDirection;
   MPoint   m_center;
   float    m_dist;
   int m_width;
};

class CRenderViewMtoARotate : public CRenderViewRotateManipulator
{
public:
   CRenderViewMtoARotate();
   virtual ~CRenderViewMtoARotate() {}

   virtual void MouseDelta(int deltaX, int deltaY);

private:
   
   MDagPath m_cameraPath;
   MFnCamera m_camera;
   
   MMatrix m_originalMatrix;
   MPoint m_originalPosition;

   MPoint m_center;
   MVector m_upDirection;
   float m_centerDist;
   float m_origLon;
   float m_origLat;
   MVector m_viewDirection;
   MVector m_origRotation;
   int m_width;
   

};



