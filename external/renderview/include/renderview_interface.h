
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

   CRenderViewInterface() {};
   virtual ~CRenderViewInterface() {DestroyRenderView();}

/**
 *   Functions to be invoked by the Host
 *   interrogating the RenderView
 **/

   // RenderView functions
   void OpenRenderView(int width, int height, float scaleFactor, QWidget *parent = NULL, bool showWin = true);
   void CloseRenderView();
   void DestroyRenderView();
   // return the renderView Qt Window
   QMainWindow *GetRenderView();

   // Options Window doesn't show the render itself, only the menus
   void OpenOptionsWindow(int width, int height, float scaleFactor, const char  *menusFilter = NULL, QWidget *parent = NULL, bool showWin = true);
   void CloseOptionsWindow();
   void DestroyOptionsWindow();

   // Stops the rendering and clears the Arnold scene, but leave window visible
   void DisableRendering();

   // return the Qt Options Window
   QMainWindow *GetOptionsWindow();


   // Render the scene.
   // This function assumes that the Arnold scene already exists
   void Render();

   // function needed to get the buffer region that needs to be updated.
   // Note that it's necessary to invoke this function when a paint is done, so that 
   // the renderer can reset its data and advert us again next time new buckets are computed
   bool HasRenderResults(AtBBox2 &region);

   // FIXME temp. function to be removed after we switch to new Render Control API
   void PostDisplay();
   
   // Get the buffer currently being displayed
   AtRGBA *GetDisplayedBuffer(unsigned int *w = NULL, unsigned int *h = NULL);

   // Return the renderer's buffer, eventually for a specific AOV
   AtRGBA *GetBuffer(int aovIndex = -1);

   // The plugin adverts the RenderView that something has changed
   // The RenderView will decide whether to re-render or not
   // and call us back in UpdateSceneChanges.
   // Return value tells us if render will actually update
   bool SceneChanged();

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

   // Set the console and file logging flags
   // We should be able to get rid of this function in next Arnold version
   void SetLogging (int consoleFlags, int fileFlags);

   // set an option in the render view
   void SetOption(const char *option, const char *value);

   // get the value of an option in the render view. The (const char *) result must immediately be
   // copied to another container. Its life scope will end after any subsequent call to this same function.
   const char *GetOption(const char *option);


   // Get a serialized definition of the RenderView options
   const char *Serialize(bool userSettings = true, bool sceneSettings = true);
   void SetFromSerialized(const char *);

   // Host tells the renderview what status to display, so that we can get finer information about what happens during export
   void SetStatusInfo(const char *); 

/**  
 *    Functions that may be invoked by the RenderView depending 
 *    user actions. They need to be overridden to define what
 *    this is supposed to produce in the Host
 **/

   // The RenderView code will invoke this function when it wants 
   // the Host to update the Arnold nodes
   // the optional vectors may be filled with the list of modified nodes
   virtual void UpdateSceneChanges() = 0;

   // The RenderView will invoke this function when it expects the whole 
   // Arnold scene to be re-exported from scratch. 
   virtual void UpdateFullScene() {UpdateSceneChanges();}

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
   virtual void RenderViewClosed(bool close_ui){};
   virtual void RenderOptionsClosed(){};

   // This function is invoked by the RenderView when it changes a parameter 
   // in the scene, so that we can advert the host
   virtual void NodeParamChanged(AtNode *node, const char *paramName) = 0;

   // This function is invoked whenever the render buffer is resized by other than 
   // manual resizing. User can invoke it to resize the viewer. One can also override this function
   // in case the host application needs to be adverted
   // == Note: virtual methods must be defined here to avoid linking errors on *nix.
   // == We just do an inline call to ResizeMainWindow() which is doing the real job
   virtual void Resize(int width, int height) {ResizeMainWindow(width, height);}


   // Renderer telling us that something has changed in the render results
   // If you override it, you have to invoke this parent class so that the window can be refreshed
   // == Note: virtual methods must be defined here to avoid linking errors on *nix.
   // == We just do an inline call to UpdateGlWidget() which is doing the real job
   virtual void RenderChanged() {UpdateGlWidget();}

// In the Future these Manipulator classes should be removed and handled
// internally by the RenderView code. As of now, MtoA's manipulators
// still rely on some Maya functions so we need to extract it

   virtual CRenderViewPanManipulator *GetPanManipulator() = 0;
   virtual CRenderViewZoomManipulator *GetZoomManipulator() = 0;
   virtual CRenderViewRotateManipulator *GetRotateManipulator() = 0;

   // Callbacks before / after each IPR step is computed, 
   // and also when the progressive render is finished
   // To be eventually overridden if desired   
   virtual void PreProgressiveStep() {}
   virtual void PostProgressiveStep() {}
   virtual void ProgressiveRenderStarted() {}
   virtual void ProgressiveRenderFinished() {}

   // Function used by Maya color picking.
   AtRGBA GetWidgetColorAtPosition(unsigned posX, unsigned posY, bool viewTransform, QWidget* pickedWidget = NULL);

private:
   
   // internal usage only, we need these functions to avoid linking issues on *nix
   void UpdateGlWidget();
   void ResizeMainWindow(int w, int h);
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



