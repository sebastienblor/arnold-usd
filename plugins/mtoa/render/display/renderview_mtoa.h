
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

/**
  * Preparing MtoA's interface code with the RenderView
  * Once the RenderView is extracted from MtoA, renderview_mtoa.cpp and renderview_mtoa.h
  * will be the only files left in MtoA repository
 **/


class CRenderViewMtoA : public CRenderViewInterface
{
public:

	CRenderViewMtoA() : CRenderViewInterface() {}
	virtual ~CRenderViewMtoA() {}

   virtual void UpdateSceneChanges(const std::vector<AtNode*> *modifiedNodes = NULL, 
   const std::vector<AtNode *> *addedNodes = NULL,
   const std::vector<AtNode *> *deletedNodes = NULL);


   virtual void GetSelection(std::vector<AtNode *> &selectedNodes);
   virtual void SetSelection(const std::vector<AtNode *> &selectedNode, bool append = false);
   virtual void ReceiveSelectionChanges(bool receive);
   virtual void NodeParamChanged(AtNode *node, const std::string &paramName);
   virtual void RenderViewClosed();

// In the Future these Manipulator classes should be removed and handled
// internally by the RenderView code. As of now, MtoA's manipulators
// still rely on some Maya functions so we need to extract it

   virtual CRenderViewPanManipulator *GetPanManipulator();
   virtual CRenderViewZoomManipulator *GetZoomManipulator();
   virtual CRenderViewRotateManipulator *GetRotateManipulator();
   
   static void SelectionChangedCallback(void *);
};

// In the Future these Manipulator classes should be removed and handled
// internally by the RenderView code. As of now, MtoA's manipulators
// still rely on some Maya functions so we need to extract it

// To be implemented
class CRenderViewMtoAPan : public CRenderViewPanManipulator
{
public:
   CRenderViewMtoAPan() : CRenderViewPanManipulator() {}
   virtual ~CRenderViewMtoAPan() {}

   virtual void MouseDelta(int deltaX, int deltaY) {};
};

class CRenderViewMtoAZoom : public CRenderViewZoomManipulator
{
public:

   CRenderViewMtoAZoom() : CRenderViewZoomManipulator() {}
   virtual ~CRenderViewMtoAZoom() {}

   virtual void MouseDelta(int deltaX, int deltaY) {}
   virtual void WheelDelta(int delta) {}
};

class CRenderViewMtoARotate : public CRenderViewRotateManipulator
{
public:
   CRenderViewMtoARotate() : CRenderViewRotateManipulator() {}
   virtual ~CRenderViewMtoARotate() {}

   virtual void MouseDelta(int deltaX, int deltaY) {};
};



