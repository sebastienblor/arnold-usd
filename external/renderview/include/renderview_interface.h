
#pragma once


#include <ai.h>

#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>


class CRenderViewMainWindow;
class CRenderViewPanManipulator;
class CRenderViewZoomManipulator;
class CRenderViewRotateManipulator;
class QWidget;
class QMainWindow;

/**
 *   CRenderViewInterface plays the role of an Interface between the "Host" (Plugin / Application)
 *   and the RenderView itself. Several functions are required, that can be invoked in both directions
 *   (from Host -> RenderView, or from RenderView -> Host)
 *   Some of them need to be overridden by the Host, who will derive from this class
 **/
class CRenderViewInterface
{
public:

   CRenderViewInterface() : m_mainWindow(NULL) {}
   virtual ~CRenderViewInterface() {DestroyRenderView();}

   void OpenRenderView(int width, int height, QWidget *parent = 0);
   void CloseRenderView();

   void DestroyRenderView();


/**
 *   Functions to be invoked by the Host
 *   interrogating the RenderView
 **/

   // return the renderView Qt Window
   QMainWindow *GetRenderView() {return (QMainWindow *)m_mainWindow;}

   // Render the scene.
   // This function assumes that the Arnold scene already exists
   void Render();

   // The plugin adverts the RenderView that something has changed
   // The RenderView will decide whether to re-render or not
   // and call us back in UpdateSceneChanges
   void SceneChanged();

   // If ReceiveSelectionChanges is set to true, then the Host
   // should call this function whenever its current selection
   // is changed
   void HostSelectionChanged(const std::vector<AtNode *> &selection);

   void InterruptRender();
   void ObjectNameChanged(const std::string &oldName, const std::string &newName);

   // This function should be invoked by the host
   // whenever the frame number is changed
   void SetFrame(float frame);

/**  
 *    Functions that may be invoked by the RenderView depending 
 *    user actions. They need to be overridden to define what
 *    this is supposed to produce in the Host
 **/

   // The RenderView code will invoke this function when it wants 
   // the Host to update the Arnold nodes
   // the optional vectors may be filled with the list of modified nodes
   virtual void UpdateSceneChanges(const std::vector<AtNode*> *modifiedNodes = NULL, 
   const std::vector<AtNode *> *addedNodes = NULL,
   const std::vector<AtNode *> *deletedNodes = NULL) = 0;


   // this function returns the list of Arnold selected nodes in the Host side
   virtual void GetSelection(std::vector<AtNode *> &selectedNodes) = 0;

   // This function is invoked by the RenderView to set the current selection
   // in the Host side
   virtual void SetSelection(const std::vector<AtNode *> &selectedNode, bool append = false) = 0;

   // This function is called by the RenderView when it needs to receive the 
   // Selection Changed events. Today, this happens when "Isolate Selected"
   // mode is used
   virtual void ReceiveSelectionChanges(bool receive) = 0;

   // This Function is called when the Renderview window is closed
   virtual void RenderViewClosed() = 0;

   // This function is invoked by the RenderView when it changes a parameter 
   // in the scene, so that we can advert the host
   virtual void NodeParamChanged(AtNode *node, const std::string &paramName) = 0;

// In the Future these Manipulator classes should be removed and handled
// internally by the RenderView code. As of now, MtoA's manipulators
// still rely on some Maya functions so we need to extract it

   virtual CRenderViewPanManipulator *GetPanManipulator() = 0;
   virtual CRenderViewZoomManipulator *GetZoomManipulator() = 0;
   virtual CRenderViewRotateManipulator *GetRotateManipulator() = 0;


private:

   CRenderViewMainWindow *m_mainWindow;

};

// In the Future these Manipulator classes should be removed and handled
// internally by the RenderView code. As of now, MtoA's manipulators
// still rely on some Maya functions so we need to extract it

class CRenderViewPanManipulator
{
public:
   CRenderViewPanManipulator() {}
   virtual ~CRenderViewPanManipulator() {}

   virtual void MouseDelta(int deltaX, int deltaY) = 0;
   
};

class CRenderViewZoomManipulator
{
public:
   CRenderViewZoomManipulator() {}
   virtual ~CRenderViewZoomManipulator() {}

   virtual void MouseDelta(int deltaX, int deltaY) = 0;
   virtual void WheelDelta(float delta) = 0;
   virtual void FrameSelection() = 0;

};

class CRenderViewRotateManipulator
{
public:
   CRenderViewRotateManipulator() {}
   virtual ~CRenderViewRotateManipulator() {}

   virtual void MouseDelta(int deltaX, int deltaY) = 0;
};



