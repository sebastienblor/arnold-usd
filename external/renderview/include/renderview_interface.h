
#pragma once


#include <ai.h>

#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>


#ifdef _WIN32
#define AI_RV_DLLEXPORT __declspec(dllexport)
#endif
#ifdef _LINUX
#define AI_RV_DLLEXPORT __attribute__ ((visibility("default")))
#endif
#ifdef _DARWIN
#define AI_RV_DLLEXPORT __attribute__ ((visibility("default")))
#endif


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
class AI_RV_DLLEXPORT CRenderViewInterface
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
   void HostSelectionChanged(const AtNode **selection, unsigned int size);

   void InterruptRender();
   void ObjectNameChanged(const char *oldName, const char *newName);

   // This function should be invoked by the host
   // whenever the frame number is changed
   void SetFrame(float frame);

   // This function should be invoked by the host to
   // provide a default directory for images
   void SetDefaultImageDirectory(const char *dir);

   // This function should be invoked by the host to
   // provide a temp directory for snapshots
   void SetTempDirectory(const char *dir);

   void SetLogging (int consoleFlags, int fileFlags);

/**  
 *    Functions that may be invoked by the RenderView depending 
 *    user actions. They need to be overridden to define what
 *    this is supposed to produce in the Host
 **/

   // The RenderView code will invoke this function when it wants 
   // the Host to update the Arnold nodes
   // the optional vectors may be filled with the list of modified nodes
   virtual void UpdateSceneChanges() = 0;


   // this function returns the amount of Arnold selected nodes in the Host side
   virtual unsigned int GetSelectionCount() = 0;
   // this function returns the list of Arnold selected nodes in the Host side
   virtual void GetSelection(AtNode **selection) = 0;

   // This function is invoked by the RenderView to set the current selection
   // in the Host side
   virtual void SetSelection(const AtNode **selectedNodes, unsigned int selectionSize, bool append = false) = 0;

   // This function is called by the RenderView when it needs to receive the 
   // Selection Changed events. Today, this happens when "Isolate Selected"
   // mode is used
   virtual void ReceiveSelectionChanges(bool receive) = 0;

   // This Function is called when the Renderview window is closed
   virtual void RenderViewClosed() = 0;

   // This function is invoked by the RenderView when it changes a parameter 
   // in the scene, so that we can advert the host
   virtual void NodeParamChanged(AtNode *node, const char *paramName) = 0;

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

class AI_RV_DLLEXPORT CRenderViewPanManipulator
{
public:
   CRenderViewPanManipulator() {}
   virtual ~CRenderViewPanManipulator() {}

   virtual void MouseDelta(int deltaX, int deltaY) = 0;
   
};

class AI_RV_DLLEXPORT CRenderViewZoomManipulator
{
public:
   CRenderViewZoomManipulator() {}
   virtual ~CRenderViewZoomManipulator() {}

   virtual void MouseDelta(int deltaX, int deltaY) = 0;
   virtual void WheelDelta(float delta) = 0;
   virtual void FrameSelection() = 0;

};

class AI_RV_DLLEXPORT CRenderViewRotateManipulator
{
public:
   CRenderViewRotateManipulator() {}
   virtual ~CRenderViewRotateManipulator() {}

   virtual void MouseDelta(int deltaX, int deltaY) = 0;
};



