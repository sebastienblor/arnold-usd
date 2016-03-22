
#pragma once
#include <ai.h>

#ifndef MTOA_DISABLE_RV
#include "renderview_interface.h"
#else

// Arnold RenderView is disabled :
// here is an empty RenderView Interface class
// so that all these functions exist and we don't have 
// fill the rest of MtoA code with #ifdef

class CRenderViewInterface
{
public:
   CRenderViewInterface() {}
   virtual ~CRenderViewInterface() {}

   void CloseRenderView(){}
   void Render(){}
   void SceneChanged(){}
   void HostSelectionChanged(const AtNode **selection, unsigned int size){}
   void InterruptRender(){}
   void ObjectNameChanged(const char *oldName, const char *newName){}
   void SetFrame(float frame){}
   void SetOption(const char *option, const char *value){}

};

class CRenderViewPanManipulator;
class CRenderViewRotateManipulator;
class CRenderViewZoomManipulator;

#endif


#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <maya/MEventMessage.h>
#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>
#include <maya/MMatrix.h>

class CRenderViewMtoA : public CRenderViewInterface
{
public:

	CRenderViewMtoA();
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
   static void RenderLayerChangedCallback(void *);
   static void SceneSaveCallback(void *);
   static void SceneOpenCallback(void *);
   static void ColorMgtChangedCallback(void *);
   static void ColorMgtCallback(MObject& node, MPlug& plug, void* clientData);
   static void ResolutionCallback(MObject& node, MPlug& plug, void* clientData);
   static void ResolutionChangedCallback(void *);

   void OpenMtoARenderView(int width, int height);
private:
   void UpdateColorManagement();


   MCallbackId m_rvSelectionCb;
   MCallbackId m_rvSceneSaveCb;
   MCallbackId m_rvSceneOpenCb;
   MCallbackId m_rvLayerManagerChangeCb;
   MCallbackId m_rvLayerChangeCb;
   MCallbackId m_rvColorMgtCb;
   MCallbackId m_rvResCb;
   MCallbackId m_rvIdleCb;

   bool m_convertOptionsParam;

};

#ifndef MTOA_DISABLE_RV

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

#endif
