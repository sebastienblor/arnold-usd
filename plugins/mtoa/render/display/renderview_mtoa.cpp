
#include "renderview_mtoa.h"


#ifdef MTOA_DISABLE_RV

// define the functions in case we disabled the RenderView
// so that we don't have to uncomment all the places where CRenderViewMtoA is used

CRenderViewMtoA::CRenderViewMtoA() {}
CRenderViewMtoA::~CRenderViewMtoA() {}

void CRenderViewMtoA::UpdateSceneChanges(){}

unsigned int CRenderViewMtoA::GetSelectionCount() {return 0;}
void CRenderViewMtoA::GetSelection(AtNode **selectedNodes) {}
void CRenderViewMtoA::SetSelection(const AtNode **selectedNodes, unsigned int selectionCount, bool append){}
void CRenderViewMtoA::ReceiveSelectionChanges(bool receive){}
void CRenderViewMtoA::NodeParamChanged(AtNode *node, const char *paramName) {}
void CRenderViewMtoA::RenderViewClosed(bool close_ui) {}
void CRenderViewMtoA::RenderChanged(){}

CRenderViewPanManipulator *CRenderViewMtoA::GetPanManipulator() {return NULL;}
CRenderViewZoomManipulator *CRenderViewMtoA::GetZoomManipulator() {return NULL;}
CRenderViewRotateManipulator *CRenderViewMtoA::GetRotateManipulator() {return NULL;}
   
void CRenderViewMtoA::SelectionChangedCallback(void *) {}
void CRenderViewMtoA::RenderLayerChangedCallback(void *) {}
void CRenderViewMtoA::SceneSaveCallback(void *) {}
void CRenderViewMtoA::SceneOpenCallback(void *) {}
void CRenderViewMtoA::ColorMgtChangedCallback(void *) {}
void CRenderViewMtoA::ColorMgtCallback(MObject& node, MPlug& plug, void* clientData) {}
void CRenderViewMtoA::ResolutionCallback(MObject& node, MPlug& plug, void* clientData) {}
void CRenderViewMtoA::ResolutionChangedCallback(void *) {}
void CRenderViewMtoA::OpenMtoARenderView(int width, int height) {}
void CRenderViewMtoA::UpdateColorManagement(){}
MStatus CRenderViewMtoA::RenderSequence(float first, float last, float step) {return MStatus::kSuccess;}

void CRenderViewMtoA::PreProgressiveStep() {}
void CRenderViewMtoA::PostProgressiveStep() {}
void CRenderViewMtoA::ProgressiveRenderStarted() {}
void CRenderViewMtoA::ProgressiveRenderFinished() {}

void CRenderViewMtoA::Resize(int w, int h){}

#else

#include "QtWidgets/qmainwindow.h"
static QWidget *s_arvWorkspaceControl = NULL;
static QWidget *s_optWorkspaceControl = NULL;

// Arnold RenderView is defined
#include "scene/MayaScene.h"
#include "translators/DagTranslator.h"
//#include <maya/MQtUtil.h>
#include <maya/MBoundingBox.h>
#include <maya/MFloatMatrix.h>
#include <maya/MGlobal.h>

#include <maya/MBoundingBox.h>
#include <maya/MFloatMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnTransform.h>
#include <maya/MRenderUtil.h>
#include <maya/MQtUtil.h>
#include <maya/M3dView.h>
#include <maya/MDagPathArray.h>
#include <maya/MNodeMessage.h>
#include <maya/MProgressWindow.h>
#include <maya/MSceneMessage.h>
#include <maya/MTimerMessage.h>
#include <maya/MPlugArray.h>
#include <maya/MQtUtil.h>


#ifdef _DARWIN
static Qt::WindowFlags RvQtFlags = Qt::Tool;
#else
static Qt::WindowFlags RvQtFlags = Qt::Window|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint;
#endif

struct CARVSequenceData
{
   float first;
   float last;
   float current;
   float step;
   bool renderStarted;
   std::string sceneUpdatesValue;
   std::string saveImagesValue;
};
static CARVSequenceData *s_sequenceData = NULL;

static bool s_creatingARV = false;
static MString s_renderLayer = "";
static std::string s_lastCameraName = "";

#if MAYA_API_VERSION >= 20190000

class CRenderViewMtoA::CustomCallback : public MColorPickerCallback
{
public:
   CustomCallback(QWidget* key, CRenderViewMtoA* renderView)
      : MColorPickerCallback(key)
      , m_renderView(renderView)
   {
   }

   MColor getColor(QWidget* pickedWidget, const QPoint& pt, bool viewTransform) const
   {
      if(pickedWidget && getKey()==m_renderView->GetRenderView())
      {
         const QPoint localPt = getKey()->mapFromGlobal(pt);
         const AtRGBA color 
            = m_renderView->GetWidgetColorAtPosition(localPt.x(), localPt.y(), viewTransform, pickedWidget);
         return MColor(color.r, color.g, color.b, color.a);
      }
      return MColor(0., 0., 0., 1.);
   }

private:
   CRenderViewMtoA* m_renderView;
};

#endif


CRenderViewMtoA::CRenderViewMtoA() : CRenderViewInterface(),
   m_rvSelectionCb(0),
   m_rvSceneSaveCb(0),
   m_rvSceneOpenCb(0),
   m_rvLayerManagerChangeCb(0),
   m_rvLayerChangeCb(0),
   m_rvColorMgtCb(0),
   m_rvResCb(0),
   m_rvIdleCb(0),
   m_colorMgtRefreshCb(0),
   m_convertOptionsParam(true),
   m_hasPreProgressiveStep(false),
   m_hasPostProgressiveStep(false),
   m_hasProgressiveRenderStarted(false),
   m_hasProgressiveRenderFinished(false),
   m_viewportRendering(false)
{   
#if MAYA_API_VERSION >= 20190000
   m_colorPickingCallback = 0x0;
#endif
}
CRenderViewMtoA::~CRenderViewMtoA()
{
#if MAYA_API_VERSION >= 20190000
   delete m_colorPickingCallback;
   MColorPickerUtilities::unregisterFromColorPicking(GetRenderView());
#endif

   if (m_rvSceneSaveCb)
   {
      MMessage::removeCallback(m_rvSceneSaveCb);
      m_rvSceneSaveCb = 0;
   }
   if (m_rvSceneOpenCb)
   {
      MMessage::removeCallback(m_rvSceneOpenCb);
      m_rvSceneOpenCb = 0;
   }
   if (m_rvSelectionCb)
   {
      MMessage::removeCallback(m_rvSelectionCb);
      m_rvSelectionCb = 0;
   }
   if (m_rvLayerManagerChangeCb)
   {
      MMessage::removeCallback(m_rvLayerManagerChangeCb);
      m_rvLayerManagerChangeCb = 0;
   }
   if (m_rvLayerChangeCb)
   {
      MMessage::removeCallback(m_rvLayerChangeCb);
      m_rvLayerChangeCb = 0;
   }
   if (m_rvColorMgtCb)
   {
      MMessage::removeCallback(m_rvColorMgtCb);
      m_rvColorMgtCb = 0;
   }
   if (m_rvResCb)
   {
      MMessage::removeCallback(m_rvResCb);
      m_rvResCb = 0;
   }
   if (m_rvIdleCb)
   {
      MMessage::removeCallback(m_rvIdleCb);
      m_rvIdleCb = 0;
   }
   if (m_colorMgtRefreshCb)
   {
      MMessage::removeCallback(m_colorMgtRefreshCb);
      m_colorMgtRefreshCb = 0;
   }
}

// Return all renderable cameras
static int GetRenderCamerasList(MDagPathArray &cameras)
{

   M3dView view;
   MDagPath activeCameraPath;
   MStatus viewStatus;
   view = M3dView::active3dView(&viewStatus);
   if (viewStatus == MS::kSuccess && view.getCamera(activeCameraPath) == MS::kSuccess)
   {
      cameras.append(activeCameraPath);
      return 1;
   }

   MItDag dagIter(MItDag::kDepthFirst, MFn::kCamera);
   MDagPath cameraPath;
   // MFnCamera cameraNode;
   MFnDagNode cameraNode;
   MPlug renderable;
   MStatus stat;
   while (!dagIter.isDone())
   {
      dagIter.getPath(cameraPath);
      cameraNode.setObject(cameraPath);
      renderable = cameraNode.findPlug("renderable", false, &stat);
      if (stat && renderable.asBool())
      {
         cameras.append(cameraPath);
      }
      dagIter.next();
   }
   int size = cameras.length();
   if (size > 1)
      MGlobal::displayWarning("More than one renderable camera. (use the -cam/-camera option to override)");
   else if (!size)
      MGlobal::displayWarning("Did not find a renderable camera. (use the -cam/-camera option to specify one)");
   return size;
}

#define ARV_DOCKED 1

#ifndef ARV_DOCKED
   static int s_arvWidth = -1;
   static int s_arvHeight = -1;
#endif

static bool HasArvOptionsAttr()
{
   int exists = 0;
   MGlobal::executeCommand("attributeExists \"ARV_options\" \"defaultArnoldRenderOptions\" ", exists);

   return (exists != 0);
}
static void InitArvOptionsAttr()
{
   if (!HasArvOptionsAttr())
   {
      MGlobal::executeCommand("addAttr -ln \"ARV_options\"  -dt \"string\"  defaultArnoldRenderOptions");
   } 
}

void CRenderViewMtoA::OpenMtoARenderView(int width, int height)
{
   // need to add this margin for the status bar + toolbar height
   height += 70;

   bool arvOptionsExisted = HasArvOptionsAttr();
   InitArvOptionsAttr();   
   
#ifdef ARV_DOCKED

   // Docking in maya workspaces only supported from maya 2017.
   // For older versions, we tried using QDockWindows (see branch FB-2470)
   // but the docking was way too sensitive, and not very usable in practice

   s_creatingARV = true;
   MString workspaceCmd = "workspaceControl ";

   bool firstCreation = true;
   if (s_arvWorkspaceControl)
   {
      workspaceCmd += " -edit -visible true ";
      firstCreation = false;
   } else
   {
      int arv_exists = 0;
      MString testExisting = "workspaceControl -exists \"ArnoldRenderView\"";
      MGlobal::executeCommand(testExisting, arv_exists);
      if (arv_exists)
      {
         workspaceCmd += " -edit -visible true ";
      }
      workspaceCmd += " -li 1"; // load immediately
      workspaceCmd += " -iw "; // initial width
      workspaceCmd += width;
      workspaceCmd += " -ih "; // initiall height
      workspaceCmd += height;

      workspaceCmd += " -requiredPlugin \"mtoa\"";


      // command called when closed. It's not ARV itself that is closed now, but the workspace !
      // Now we need to rely on the visibilityChanbe callback so we no longer need the close callback
      // workspaceCmd += " -cc \"arnoldRenderView -mode close\" ";
      workspaceCmd += " -l \"Arnold RenderView\" "; // label
   }
   workspaceCmd += " \"ArnoldRenderView\""; // name of the workspace, to get it back later

   double scaleFactor = 1.0;
   scaleFactor = MQtUtil::dpiScale(100.0f)/100.0f;

   OpenRenderView(width, height,scaleFactor, nullptr, false); // this creates ARV or restarts the render

   QMainWindow *arv = GetRenderView();  
   arv->setWindowFlags(Qt::Widget);

   
   MGlobal::executeCommand(workspaceCmd); // create the workspace, or get it back
   
   if (firstCreation)
   {
      // returns a pointer to th workspace called above, 
      // but only for the creation ! if I call it with "-edit visible true" it can return 0
      s_arvWorkspaceControl = MQtUtil::getCurrentParent(); 
      MQtUtil::addWidgetToMayaLayout(arv, s_arvWorkspaceControl);  // attaches ARV to the workspace
      arv->show();
      s_arvWorkspaceControl->show();

   }
   // now set the uiScript, so that Maya can create ARV in the middle of the workspaces
   MString uiScriptCommand("workspaceControl -e -uiScript \"arnoldRenderView -mode open\"  -visibleChangeCommand \"arnoldRenderView -mode visChanged_cb\" \"ArnoldRenderView\"");
   MGlobal::executeCommand(uiScriptCommand);

//   MString visChangeCommand("workspaceControl -e -vcc \"arnoldRenderView -mode workspaceChange\" \"ArnoldRenderView\"");
//   MGlobal::executeCommand(visChangeCommand);


   s_creatingARV = false;

   
#else
   if (s_arvWidth > 0)
   {
      // restoring previous width/height
      width = s_arvWidth;
      height = s_arvHeight;

      // reset the static values (we don't want to set it again)
      s_arvWidth = -1;
      s_arvHeight = -1;
   }
   OpenRenderView(width, height,scaleFactor, MQtUtil::mainWindow(),scaleFactor); // this creates ARV or restarts the render
#endif

   MStatus status;   
   if (m_rvSceneSaveCb == 0)
   {
      m_rvSceneSaveCb = MSceneMessage::addCallback(MSceneMessage::kBeforeSave, CRenderViewMtoA::SceneSaveCallback, (void*)this, &status);
   }
   if (m_rvSceneOpenCb == 0)
   {
      m_rvSceneOpenCb = MSceneMessage::addCallback(MSceneMessage::kAfterOpen, CRenderViewMtoA::SceneOpenCallback, (void*)this, &status);
   }
   if (m_rvLayerManagerChangeCb == 0)
   {
      m_rvLayerManagerChangeCb = MEventMessage::addEventCallback("renderLayerManagerChange",
                                      CRenderViewMtoA::RenderLayerChangedCallback,
                                      (void*)this);
   }
   if (m_rvLayerChangeCb == 0)
   {
      m_rvLayerChangeCb =  MEventMessage::addEventCallback("renderLayerChange",
                                      CRenderViewMtoA::RenderLayerChangedCallback,
                                      (void*)this);
   }

   if(m_colorMgtRefreshCb == 0)
   {
      m_colorMgtRefreshCb = MEventMessage::addEventCallback( MString( "colorMgtRefreshed" ), CRenderViewMtoA::ColorMgtRefreshed, (void*)this);
   }

   MSelectionList activeList;
   activeList.add(MString(":defaultColorMgtGlobals"));
   
   // get the maya node contraining the color management options         
   if(activeList.length() > 0)
   {
      MObject colorMgtObject;
      activeList.getDependNode(0,colorMgtObject);

      if (m_rvColorMgtCb == 0)
      {
         m_rvColorMgtCb = MNodeMessage::addNodeDirtyCallback(colorMgtObject,
                                              ColorMgtCallback,
                                              this,
                                              &status);
      }

      if (m_convertOptionsParam) UpdateColorManagement();
   }
   // Moving the ARV_options load *after* calling UpdateColorManagement because of #2719
   if (m_convertOptionsParam)
   {
      if (arvOptionsExisted)
      {
         // assign the ARV_options parameter as it is the first time since I opened this scene
         MString optParam;
         if (HasArvOptionsAttr())
         {
            MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.ARV_options\"", optParam);
            SetFromSerialized(optParam.asChar());
         }
      }

      bool varExists = false;
      MString optionVarValue = MGlobal::optionVarStringValue("arv_user_options", &varExists);
      if (varExists)
         SetFromSerialized(optionVarValue.asChar());
      
   
      SetOption("Real Size", "1");
   }

   m_convertOptionsParam = false;

   MSelectionList resList;
   resList.add(MString(":defaultResolution"));
   
   // get the maya node contraining the color management options         
   if(resList.length() > 0)
   {
      MObject resObject;
      resList.getDependNode(0,resObject);

      if (m_rvResCb == 0)
      {
         m_rvResCb = MNodeMessage::addNodeDirtyCallback(resObject,
                                              ResolutionCallback,
                                              this,
                                              &status);
      }

   }

   // Set image Dir
   MString workspace;
   status = MGlobal::executeCommand(MString("workspace -q -rd;"), workspace);
   if (status == MS::kSuccess)
   {
      workspace += "/images";
      SetDefaultImageDirectory(workspace.asChar());
   }
   MString tmpDir;
   status = MGlobal::executeCommand(MString("getenv \"TMPDIR\";"), tmpDir);
   if (status == MS::kSuccess)
   {
      SetTempDirectory(tmpDir.asChar());
   }

   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
   {
      CRenderOptions *renderOptions = renderSession->RenderOptions();
      int consoleLogging = renderOptions->GetLogConsoleVerbosity();
      int fileLogging = renderOptions->GetLogFileVerbosity();
      SetLogging(consoleLogging, fileLogging);
   }
   UpdateRenderCallbacks();

   MGlobal::executePythonCommand("import mtoa.utils;mtoa.utils.getActiveRenderLayerName()", s_renderLayer);
   if (s_renderLayer.length() == 0)
      s_renderLayer = "masterLayer";


#if MAYA_API_VERSION >= 20190000
   if(!m_colorPickingCallback)
   {
      m_colorPickingCallback = new CustomCallback(GetRenderView(), this);
      MColorPickerUtilities::doRegisterToColorPicking(GetRenderView(), m_colorPickingCallback);
   }
#endif
}

void CRenderViewMtoA::OpenMtoAViewportRendererOptions()
{ 
#ifdef ARV_DOCKED

   // Docking in maya workspaces only supported from maya 2017.
   // For older versions, we tried using QDockWindows (see branch FB-2470)
   // but the docking was way too sensitive, and not very usable in practice

   //s_creatingARV = true;
   MString workspaceCmd = "workspaceControl ";

   bool firstCreation = true;
   if (s_optWorkspaceControl)
   {
      workspaceCmd += " -edit -visible true ";
      firstCreation = false;
   }
   else
   {
      workspaceCmd += " -li 1"; // load immediately
      workspaceCmd += " -iw 250 -ih 50"; // initial width
      

      workspaceCmd += " -requiredPlugin \"mtoa\"";

      // command called when closed. It's not ARV itself that is closed now, but the workspace !
      // Now we need to rely on the visibilityChange callback
      workspaceCmd += " -l \"Arnold ViewportRenderer Options\" "; // label
   }
   workspaceCmd += " \"ArnoldViewportRendererOptions\""; // name of the workspace, to get it back later
   
   double scaleFactor = 1.0;
   scaleFactor = MQtUtil::dpiScale(100.0f)/100.0f;

   std::string menusFilter = "Crop Region;AOVs;Update Full Scene;Abort Render;Log;Save UI Threads;Debug Shading;Isolate Selection;Lock Selection;Test Resolution";
   menusFilter += ";Save Final Images;Save Multi-Layer EXR;Run IPR";
   CRenderViewInterface::OpenOptionsWindow(250, 50,scaleFactor, menusFilter.c_str(), nullptr, false);
   QMainWindow *optWin = GetOptionsWindow();
   optWin->setWindowFlags(Qt::Widget);
   
   MGlobal::executeCommand(workspaceCmd); // create the workspace, or get it back

   if (firstCreation)
   {
      // returns a pointer to th workspace called above, 
      // but only for the creation ! if I call it with "-edit visible true" it can return 0
      s_optWorkspaceControl = MQtUtil::getCurrentParent();
      MQtUtil::addWidgetToMayaLayout(optWin, s_optWorkspaceControl);  // attaches ARV to the workspace
      optWin->show();
      s_optWorkspaceControl->show();
   }
   // now set the uiScript, so that Maya can create ARV in the middle of the workspaces
   MString uiScriptCommand("workspaceControl -e -uiScript \"arnoldViewOverrideOptionBox\" -visibleChangeCommand \"arnoldViewOverrideOptionBox -mode visChanged_cb\" \"ArnoldViewportRendererOptions\"");
   MGlobal::executeCommand(uiScriptCommand);

    //s_creatingARV = false;
#else
   CRenderViewInterface::OpenOptionsWindow(200, 50,scaleFactor, NULL, MQtUtil::mainWindow(), false);
#endif

   // Callbacks for scene open/save, as well as render layers changes
   MStatus status;   
   if (m_rvSceneSaveCb == 0)
   {
      m_rvSceneSaveCb = MSceneMessage::addCallback(MSceneMessage::kBeforeSave, CRenderViewMtoA::SceneSaveCallback, (void*)this, &status);
   }
   if (m_rvSceneOpenCb == 0)
   {
      m_rvSceneOpenCb = MSceneMessage::addCallback(MSceneMessage::kAfterOpen, CRenderViewMtoA::SceneOpenCallback, (void*)this, &status);
   }
   if (m_rvLayerManagerChangeCb == 0)
   {
      m_rvLayerManagerChangeCb = MEventMessage::addEventCallback("renderLayerManagerChange",
                                      CRenderViewMtoA::RenderLayerChangedCallback,
                                      (void*)this);
   }
   if (m_rvLayerChangeCb == 0)
   {
      m_rvLayerChangeCb =  MEventMessage::addEventCallback("renderLayerChange",
                                      CRenderViewMtoA::RenderLayerChangedCallback,
                                      (void*)this);
   }

   UpdateRenderCallbacks();
}

void CRenderViewMtoA::RenderChanged()
{
   CRenderViewInterface::RenderChanged();
    
   if (m_viewportRendering)
      MGlobal::executeCommandOnIdle("refresh -f;");
}

/**
  * Preparing MtoA's interface code with the RenderView
  * Once the RenderView is extracted from MtoA, renderview_mtoa.cpp and renderview_mtoa.h
  * will be the only files left in MtoA repository
 **/

void CRenderViewMtoA::UpdateSceneChanges()
{
   if (!AiUniverseIsActive())
      UpdateFullScene();
   else
   {
      CMayaScene::UpdateSceneChanges();
      SetFrame((float)CMayaScene::GetArnoldSession()->GetExportFrame());
   }
}

/** When this funtion is invoked, it means that the whole scene needs to 
 * be re-exported from scratch.
 **/
void CRenderViewMtoA::UpdateFullScene()
{
   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   // Universe isn't active, oh my....
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   // the renderSession will be NULL if ARV was opened without rendering
   if (renderSession)
   {   
      renderSession->SetRendering(false);
      CMayaScene::End();
      CMayaScene::ExecuteScript(renderGlobals.postMel);
      CMayaScene::ExecuteScript(renderGlobals.postRenderMel);

   }

   // Make sure the caches are flushed (#3369)
   AiUniverseCacheFlush(AI_CACHE_ALL);

   // Re-export everything !
   MDagPathArray cameras;
   GetRenderCamerasList(cameras);
   CMayaScene::ExecuteScript(renderGlobals.preMel);
   CMayaScene::ExecuteScript(renderGlobals.preRenderMel);

   CMayaScene::Begin(MTOA_SESSION_RENDERVIEW);

   if (!renderGlobals.renderAll)
   {
      MSelectionList selected;
      MGlobal::getActiveSelectionList(selected);
      CMayaScene::Export(&selected);
   }
   else
   {
      CMayaScene::Export();
   }

   if (cameras.length() > 0)
   {
      MDagPath renderCamera = cameras[0];

      // try to restore the last camera we had in ARV (#3372)
      // FIXME replace this by a function GetOptionValue("Camera") that would return the 
      // menu value
      if (!s_lastCameraName.empty())
      {
        // Search for the MDagPath for this camera   
         MItDag itDag(MItDag::kDepthFirst, MFn::kCamera);
         itDag.reset();

         while (!itDag.isDone())
         {
            MDagPath camPath;
            itDag.getPath(camPath);
            std::string camName = CDagTranslator::GetArnoldNaming(camPath).asChar();
            if (camName == s_lastCameraName)
            {
               camPath.extendToShape();
               renderCamera = camPath;
               break;
            }      
            itDag.next();
         }
      }
      if (renderCamera.isValid())
      {
         CMayaScene::GetArnoldSession()->ExportDagPath(renderCamera, true);
      }
      // SetExportCamera mus be called AFTER CMayaScene::Export
      CMayaScene::GetArnoldSession()->SetExportCamera(renderCamera, false);

      // Set resolution and camera as passed in.
      CMayaScene::GetRenderSession()->SetResolution(-1, -1);
      CMayaScene::GetRenderSession()->SetCamera(renderCamera);
   }

   UpdateRenderCallbacks();

   // GetRenderSession() might have changed since we exported the scene
   renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
      renderSession->SetRendering(true); // this allows MtoA to know that a render process is going on
}
void CRenderViewMtoA::SetCameraName(const MString &name)
{
   s_lastCameraName=name.asChar();
}
void CRenderViewMtoA::UpdateRenderCallbacks()
{
   MStatus status;
   MFnDependencyNode optionsNode(CMayaScene::GetSceneArnoldRenderOptionsNode(), &status);
   if (status)
   {
      m_progressiveRenderStarted = optionsNode.findPlug("IPRRefinementStarted", true).asString();
      m_preProgressiveStep = optionsNode.findPlug("IPRStepStarted", true).asString();
      m_postProgressiveStep = optionsNode.findPlug("IPRStepFinished", true).asString();
      m_progressiveRenderFinished = optionsNode.findPlug("IPRRefinementFinished", true).asString();
   }
   else
   {
      m_progressiveRenderStarted = "";
      m_preProgressiveStep = "";
      m_postProgressiveStep = "";
      m_progressiveRenderFinished = "";
   }
   m_hasPreProgressiveStep = (m_preProgressiveStep != "");
   m_hasPostProgressiveStep = (m_postProgressiveStep != "");
   m_hasProgressiveRenderStarted = (m_progressiveRenderStarted != "");
   m_hasProgressiveRenderFinished = (m_progressiveRenderFinished != "");
}

static void GetSelectionVector(std::vector<AtNode *> &selectedNodes)
{
   MSelectionList activeList;
   MGlobal::getActiveSelectionList(activeList);
   if(activeList.isEmpty()) return;

   MDagPath dagPath;
   MObject objNode;
   activeList.getDependNode(0, objNode);

   MString name;

   if (activeList.getDagPath(0, dagPath) == MS::kSuccess)
   {
      dagPath.extendToShape();
      name = CDagTranslator::GetArnoldNaming(dagPath);
   } else
   {
      if (objNode.hasFn(MFn::kDisplacementShader))
      {
         MFnDependencyNode depNode(objNode);
         MPlug dispPlug = depNode.findPlug("displacement", true);
         if (!dispPlug.isNull())
         {
            MPlugArray conn;
            dispPlug.connectedTo(conn, true, false);
            if (conn.length() > 0)
               objNode = conn[0].node();
               
         }
       
      }
      name = MFnDependencyNode(objNode).name();
   }

   AtNode *selected = AiNodeLookUpByName(name.asChar());
   if (selected) selectedNodes.push_back(selected);
   
}
unsigned int CRenderViewMtoA::GetSelectionCount()
{
   if (AiUniverseIsActive())
   {
      std::vector<AtNode *>selection;
      GetSelectionVector(selection);
      return (unsigned int)selection.size();
   }
   
   return 0;
}
void CRenderViewMtoA::GetSelection(AtNode **selection)
{
   std::vector<AtNode *> selectionVec;
   GetSelectionVector(selectionVec);
   if (selectionVec.empty()) return;

   memcpy(selection, &selectionVec[0], selectionVec.size() * sizeof(AtNode*));
}

void CRenderViewMtoA::SceneSaveCallback(void *data)
{
   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;


   // Get Scene-dependent ARV data
   const char *sceneSerialized = renderViewMtoA->Serialize(false, true); // Scene settings
   if (sceneSerialized)
   {
      InitArvOptionsAttr();
      MString command = "setAttr -type \"string\" \"defaultArnoldRenderOptions.ARV_options\" \"";
      command += sceneSerialized;
      command +="\"";
      MGlobal::executeCommand(command);
   }

   const char *userSerialized = renderViewMtoA->Serialize(true, false); // user settings
   if (userSerialized)
   {
      MString arvOptionName("arv_user_options");
      MGlobal::setOptionVarValue(arvOptionName, MString(userSerialized));
   }
}
void CRenderViewMtoA::ColorMgtRefreshed(void *data)
{
   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;
   renderViewMtoA->SetOption("Color Management.Refresh", "1");
   MGlobal::displayWarning("[mtoa] OCIO Context might have changed for input textures. If so, you may have to re-generate the textures using 'arnoldUpdateTx -f'");
}
void CRenderViewMtoA::SceneOpenCallback(void *data)
{
   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;
   renderViewMtoA->m_convertOptionsParam = true;

   QMainWindow *arv = renderViewMtoA->GetRenderView();  
   if (arv == NULL)
      return;

   if (arv->isVisible())
   {
      CMayaScene::GetRenderSession()->FillRenderViewCameras();

      // assign the ARV_options parameter as it is the first time since I opened this scene
      int exists = 0;
      MGlobal::executeCommand("objExists defaultArnoldRenderOptions", exists);

      if (exists != 0)
      {
         MString optParam;
         if (HasArvOptionsAttr())
         {
            MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.ARV_options\"", optParam);
            renderViewMtoA->SetFromSerialized(optParam.asChar());
         }
      }

      // ARV is already visible -> set the options right away
      renderViewMtoA->UpdateColorManagement();

      renderViewMtoA->m_convertOptionsParam = false;      
   }

   // otherwise, next time I open the RenderView, convert the ARV_options param
}

void CRenderViewMtoA::RenderLayerChangedCallback(void *data)
{
   if (!AiUniverseIsActive()) return;

   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;

   MString layerName;
   MGlobal::executePythonCommand("mtoa.utils.getActiveRenderLayerName()", layerName);

   // we haven't changed the visible render layer
   if (layerName.length() == 0 || layerName == s_renderLayer) 
      return;

   s_renderLayer = layerName;
   renderViewMtoA->SetOption("Full IPR Update", "1");


}
// For "Isolate Selected" debug shading mode,
// we need to receive the events that current
// Selection has changed
void CRenderViewMtoA::SelectionChangedCallback(void *data)
{
   if (!AiUniverseIsActive()) return;

   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;
   MSelectionList activeList;
   MGlobal::getActiveSelectionList(activeList);
   if( activeList.isEmpty()) return;

   //CArnoldSession *session = CMayaScene::GetArnoldSession();
   //session->FlattenSelection(&activeList, false);
   
   MObject objNode;
   std::vector<AtNode *> selection;
   unsigned int count = activeList.length();

   for (unsigned int i = 0; i < count; ++i)
   {
      activeList.getDependNode(i, objNode);
      MDagPath dagSel;
      if (objNode.hasFn(MFn::kTransform))
      {
         // from Transform to Shape
         activeList.getDagPath(i, dagSel);
         objNode = dagSel.child(0);
         dagSel.push(objNode);
      }
      if (objNode.hasFn(MFn::kDisplacementShader))
      {
         MFnDependencyNode depNode(objNode);
         MPlug dispPlug = depNode.findPlug("displacement", true);
         if (!dispPlug.isNull())
         {
            MPlugArray conn;
            dispPlug.connectedTo(conn, true, false);
            if (conn.length() > 0)
               objNode = conn[0].node();
         }
      }

      MFnDependencyNode nodeFn( objNode );

      AtNode *selected_shader = AiNodeLookUpByName (nodeFn.name().asChar());
      if (selected_shader == NULL && dagSel.isValid())
      {
         MString selName = CDagTranslator::GetArnoldNaming(dagSel);
         selected_shader = AiNodeLookUpByName(selName.asChar());
      }

      if(selected_shader) selection.push_back(selected_shader);
   }

   
   renderViewMtoA->HostSelectionChanged((selection.empty()) ? NULL : (const AtNode **)&selection[0], selection.size());

}


void CRenderViewMtoA::SetSelection(const AtNode **selectedNodes, unsigned int selectionCount, bool append)
{  
   MStringArray selectedObjects;
   CArnoldSession *session = CMayaScene::GetArnoldSession();
   unordered_map<std::string, std::vector<std::string> > proceduralSelectedItems;

   for (unsigned int i = 0; i < selectionCount; ++i)
   {
      const AtNode *pickedObject = selectedNodes[i];
      if (pickedObject == NULL)
         continue;

      AtString selName = AtString(AiNodeGetName(pickedObject));

      MString selectedPath = MString(selName.c_str());

      // get the root parent node
      AtNode *parentNode = AiNodeGetParent(pickedObject);
      while(parentNode)
      {
         pickedObject = parentNode;
         parentNode = AiNodeGetParent(pickedObject);
         if (parentNode)
         {
            AtString childSelName = AtString(AiNodeGetName(pickedObject));
            selectedPath = MString(childSelName.c_str()) + MString("/") + selectedPath;
         }
         else
            break;      
      }
      MString objName = session->GetMayaObjectName(pickedObject);
      std::string objNameStr(objName.asChar());
      // selectedObjects contain the list of root parent nodes (eg the root standin that exists in the maya scene)
      selectedObjects.append(objName);

      // selectedFullName contain the list of paths (procedural child names) that we'll use to update the UI list
      if(AiNodeEntryGetDerivedType(AiNodeGetNodeEntry(pickedObject)) == AI_NODE_SHAPE_PROCEDURAL) 
         proceduralSelectedItems[objNameStr].push_back(selectedPath.asChar());

   }
   
   if (selectedObjects.length() == 0 && !append)
   {
      MGlobal::clearSelectionList();
      return;
   }

   for (unsigned int i = 0; i < selectedObjects.length(); ++i)
      MGlobal::selectByName(selectedObjects[i], (append || i > 0) ? MGlobal::kAddToList : MGlobal::kReplaceList);

   unordered_map<std::string, std::vector<std::string> >::iterator it = proceduralSelectedItems.begin();
   
   for (; it != proceduralSelectedItems.end(); ++it)
   {
      // First check if the procedural has an attribute called "selectedItems"
      int exists = 0;
      MString cmd = "attributeExists \"selectedItems\" \"";
      cmd += MString(it->first.c_str());
      cmd += MString("\"");
      MGlobal::executeCommand(cmd, exists);
      if (exists == 0)
         continue;

      std::vector<std::string> &selectedItems = it->second;
      cmd = MString("setAttr -type \"string\" ");
      cmd += MString(it->first.c_str());
      cmd += ".selectedItems \"";

      for (size_t i = 0; i < selectedItems.size(); ++i)
      {
         if (i > 0)
            cmd += MString(",");
         cmd += MString(selectedItems[i].c_str());
      }
      cmd += MString("\"");
      MGlobal::executeCommand(cmd);
   }
}

void CRenderViewMtoA::NodeParamChanged(AtNode *node, const char *paramNameChar)
{
   if (node != AiUniverseGetOptions()) return;
   std::string paramName = paramNameChar;

/*
   We used to advert the RenderSession that region had changed, but in fact this just causes more confusion
   since the renderSession will do other changes to "region_min*" "region_max*"
   So from now on, we're no longer telling the Render Session what is happening with the crop region
   and it's much simpler...

   if (paramName == "region_min_x" || paramName == "region_min_y" || paramName == "region_max_x" || paramName == "region_max_y")
   {
      CRenderSession* renderSession = CMayaScene::GetRenderSession();

      int minx = AiNodeGetInt(node, "region_min_x");
      int miny = AiNodeGetInt(node, "region_min_y");
      int maxx = AiNodeGetInt(node, "region_max_x");
      int maxy = AiNodeGetInt(node, "region_max_y");


      if (minx < 0 || miny < 0)
      {
         int xres = AiNodeGetInt(node, "xres");
         int yres = AiNodeGetInt(node, "yres");

         renderSession->SetRegion(0, xres, 0, yres);
      } else
      {
         renderSession->SetRegion(minx, maxx , miny, maxy);
      }
      return;
   }

   */

   if (paramName == "camera")
   {

      AtNode *cam = (AtNode*)AiNodeGetPtr(node, "camera");
      if (cam == NULL) return;

      std::string cameraName = AiNodeGetName(cam);
      // store the camera name, we might need it for "Update Full Scene" (#3372)
      s_lastCameraName = cameraName;

      // Search for the MDagPath for this camera   
      MItDag itDag(MItDag::kDepthFirst, MFn::kCamera);
      itDag.reset();

      while (!itDag.isDone())
      {
         MDagPath camPath;
         itDag.getPath(camPath);
         std::string camName = CDagTranslator::GetArnoldNaming(camPath).asChar();
         if (camName == cameraName)
         {
            // why do we need to have this information in 2 several places ??
            CMayaScene::GetRenderSession()->SetCamera(camPath);
            CMayaScene::GetArnoldSession()->SetExportCamera(camPath, false);  // false means we don't want MtoA to trigger a new render, this is already being done by ARV
            CMayaScene::GetArnoldSession()->ExportImagePlane();
            break;
         }      
         itDag.next();
      }
   }
}

void CRenderViewMtoA::ReceiveSelectionChanges(bool receive)
{
   if ((!receive) && m_rvSelectionCb)
   {
      MMessage::removeCallback(m_rvSelectionCb);
      m_rvSelectionCb = 0;
      return;
   }

   if (receive && (m_rvSelectionCb == 0))
   {

      m_rvSelectionCb = MEventMessage::addEventCallback("SelectionChanged",
                                      CRenderViewMtoA::SelectionChangedCallback,
                                      (void*)this);

   } 

}

void CRenderViewMtoA::RenderViewClosed(bool close_ui)
{

#ifdef ARV_DOCKED
   if (close_ui)
   {
      if (!s_arvWorkspaceControl)
         return;
      
      // ARV is docked into a workspace, we must close it too (based on its unique name in maya)
      if (s_arvWorkspaceControl)
         MGlobal::executeCommand("workspaceControl -edit -cl \"ArnoldRenderView\"");
   } 
   
#else

   // closing ARV, we want to get the previous width / height
   // since there are no workspaces here
   QMainWindow *arv = GetRenderView();  
   if (arv)
   {
      s_arvWidth = arv->width();
      s_arvHeight = arv->height();
   }

#endif

   ReceiveSelectionChanges(false);

   if (close_ui)
   {
      // Saving user prefs when the render view is closed
      const char *userSerialized = Serialize(true, false); // user settings
      if (userSerialized)
      {
         MString arvOptionName("arv_user_options");
         MGlobal::setOptionVarValue(arvOptionName, MString(userSerialized));
      }

      if (s_sequenceData != NULL)
      {
         SetOption("Scene Updates", s_sequenceData->sceneUpdatesValue.c_str());
         SetOption("Save Final Images", s_sequenceData->saveImagesValue.c_str());
         if (m_rvIdleCb)
         {
            MMessage::removeCallback(m_rvIdleCb);
            m_rvIdleCb = 0;
         }
         delete s_sequenceData;
         s_sequenceData = NULL;
      }
   }

   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
   {   
      renderSession->SetRendering(false);
      CMayaScene::End();

      MCommonRenderSettingsData renderGlobals;
      MRenderUtil::getCommonRenderSettings(renderGlobals);

      CMayaScene::ExecuteScript(renderGlobals.postRenderMel);
      CMayaScene::ExecuteScript(renderGlobals.postMel);
   }
   MMessage::removeCallback(m_rvSceneSaveCb);
   m_rvSceneSaveCb = 0;

   MMessage::removeCallback(m_rvLayerManagerChangeCb);
   m_rvLayerManagerChangeCb = 0;

   MMessage::removeCallback(m_rvLayerChangeCb);
   m_rvLayerChangeCb = 0;
   
   MProgressWindow::endProgress();
}

void CRenderViewMtoA::RenderOptionsClosed()
{

#ifdef ARV_DOCKED
   if (s_optWorkspaceControl)
   {
      MGlobal::executeCommand("workspaceControl -edit -cl \"ArnoldViewportRendererOptions\"");      
   }
#endif
}

CRenderViewPanManipulator *CRenderViewMtoA::GetPanManipulator()
{
   return new CRenderViewMtoAPan();
}

CRenderViewZoomManipulator *CRenderViewMtoA::GetZoomManipulator()
{
   return new CRenderViewMtoAZoom();
}

CRenderViewRotateManipulator *CRenderViewMtoA::GetRotateManipulator()
{
   return new CRenderViewMtoARotate();
}
   
CRenderViewMtoAPan::CRenderViewMtoAPan() : CRenderViewPanManipulator()
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;
   
   MSelectionList camList;
   AtString camName = (AiNodeLookUpUserParameter(arnold_camera, "dcc_name")) ? 
      AiNodeGetStr(arnold_camera, "dcc_name") : AtString(AiNodeGetName(arnold_camera));

   camList.add(MString(camName.c_str()));

   camList.getDagPath(0, m_cameraPath);

   if (!m_cameraPath.isValid()) return;

   MObject camNode = m_cameraPath.node();

   m_camera.setObject(m_cameraPath);      

   m_originalMatrix = m_cameraPath.inclusiveMatrix();
   MStatus status;
   m_originalPosition = m_camera.eyePoint(MSpace::kWorld, &status);

   m_upDirection = m_camera.upDirection(MSpace::kWorld);
   m_rightDirection = m_camera.rightDirection(MSpace::kWorld);
   m_viewDirection = m_camera.viewDirection(MSpace::kWorld);

   m_distFactor = 1.f;
   
   if (strcmp (AiNodeEntryGetName(AiNodeGetNodeEntry(arnold_camera)), "persp_camera") != 0) return;

   MPoint originalPosition = m_camera.eyePoint(MSpace::kWorld);
   MPoint center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   float center_dist = (float)center.distanceTo(originalPosition);

   m_distFactor = center_dist * tanf(AiNodeGetFlt(arnold_camera, "fov") * AI_DTOR);

   m_width = AiNodeGetInt(AiUniverseGetOptions(), "xres");
}

void CRenderViewMtoAPan::MouseDelta(int deltaX, int deltaY)
{
      // get the delta factor relative to the width 
   float delta[2];
   delta[0] = (-m_distFactor * ((deltaX)/((float)m_width)));
   delta[1] = (m_distFactor * ((deltaY)/((float)m_width)));

   MPoint newPosition = m_originalPosition + m_rightDirection * delta[0] + m_upDirection * delta[1];
   m_camera.set(newPosition, m_viewDirection, m_upDirection, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
}

CRenderViewMtoAZoom::CRenderViewMtoAZoom() : CRenderViewZoomManipulator()
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;
   
   MSelectionList camList;
   AtString camName = (AiNodeLookUpUserParameter(arnold_camera, "dcc_name")) ? 
      AiNodeGetStr(arnold_camera, "dcc_name") : AtString(AiNodeGetName(arnold_camera));

   camList.add(MString(camName.c_str()));

   camList.getDagPath(0, m_cameraPath);

   if (!m_cameraPath.isValid()) return;

   MObject camNode = m_cameraPath.node();

   m_camera.setObject(m_cameraPath);      

   m_originalMatrix = m_cameraPath.inclusiveMatrix();
   MStatus status;
   m_originalPosition = m_camera.eyePoint(MSpace::kWorld, &status);

   m_viewDirection = m_camera.viewDirection(MSpace::kWorld);
   m_upDirection = m_camera.upDirection(MSpace::kWorld);
   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   m_dist = (float)m_center.distanceTo(m_originalPosition);

   m_width = AiNodeGetInt(AiUniverseGetOptions(), "xres");
}

void CRenderViewMtoAZoom::MouseDelta(int deltaX, int deltaY)
{
   float delta = (float)deltaX / (float)m_width;

   if (delta > 0.9f)
   {
      float diff = delta - 0.9f;
      delta = 0.9f;
      while (diff > 0.f)
      {
         delta += AiMin(diff, 1.f) * (1.f - delta) * 0.5f;
         diff -= 1.f;
      }
   }
   delta *= m_dist;   

   MPoint newPosition = m_originalPosition + m_viewDirection * delta;
   m_camera.set(newPosition, m_viewDirection, m_upDirection, m_camera.horizontalFieldOfView(), m_camera.aspectRatio());
   m_camera.setCenterOfInterestPoint(m_center, MSpace::kWorld);
}

void CRenderViewMtoAZoom::WheelDelta(float delta)
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;
   
   MSelectionList camList;
   AtString camName = (AiNodeLookUpUserParameter(arnold_camera, "dcc_name")) ? 
      AiNodeGetStr(arnold_camera, "dcc_name") : AtString(AiNodeGetName(arnold_camera));

   camList.add(MString(camName.c_str()));

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
   float center_dist = (float)center.distanceTo(originalPosition);
   delta *= center_dist / 10.f;
   MPoint newPosition = originalPosition;
   MVector zoom = view_direction;
   zoom *= delta;
   newPosition +=  zoom;
   camera.set(newPosition, view_direction, camera.upDirection(MSpace::kWorld), camera.horizontalFieldOfView(), camera.aspectRatio());
   camera.setCenterOfInterestPoint(center, MSpace::kWorld);
}

void CRenderViewMtoAZoom::FrameSelection()
{
   // FIXME to be extracted       
      // Frame the selected geometries bounding box
   
   MSelectionList selected; 
   MBoundingBox globalBox;

   MGlobal::getActiveSelectionList(selected);
   for (unsigned int i = 0; i < selected.length(); ++i)
   {
      MDagPath dagPath;
      if (selected.getDagPath(i, dagPath) != MS::kSuccess) continue;
      MStatus status;
      MFnDagNode dagShape(dagPath, &status);
      if (status != MS::kSuccess) continue;

      MBoundingBox boundingBox = dagShape.boundingBox(&status);
      if (status != MS::kSuccess) continue;

      MMatrix mtx = dagPath.inclusiveMatrix();

      boundingBox.transformUsing(mtx);
      globalBox.expand(boundingBox);
   }
   AtNode *arnoldCamera = AiUniverseGetCamera();
   if (arnoldCamera == NULL) return;
   
   MSelectionList camList;
   AtString camName = (AiNodeLookUpUserParameter(arnoldCamera, "dcc_name")) ? 
      AiNodeGetStr(arnoldCamera, "dcc_name") : AtString(AiNodeGetName(arnoldCamera));

   camList.add(MString(camName.c_str()));

   MDagPath camDag;
   camList.getDagPath(0, camDag);

   if (!camDag.isValid()) return;
   MObject camNode = camDag.node();
   MFnCamera camera;
   camera.setObject(camDag);
   MMatrix camToWorld = camDag.inclusiveMatrix();

   // don't want to change the viewDirection & upDirection
   MVector viewDirection = camera.viewDirection(MSpace::kWorld);
   MVector upDirection = camera.upDirection(MSpace::kWorld);
   MPoint eyePoint = camera.eyePoint(MSpace::kWorld);
   MPoint centerInterest = camera.centerOfInterestPoint(MSpace::kWorld);

   MPoint center = globalBox.center();
   MFloatMatrix projectionMatrix = camera.projectionMatrix();

   MPoint newPos = eyePoint + center - centerInterest;
   float centerDist = float(center.distanceTo(newPos));

   camToWorld[3][0] = newPos.x;
   camToWorld[3][1] = newPos.y;
   camToWorld[3][2] = newPos.z;

   MMatrix worldToCam = camToWorld.inverse();

   globalBox.transformUsing(worldToCam);

   MPoint minBox = globalBox.min(); // in camera space
   MPoint maxBox = globalBox.max(); // in camera space

   MMatrix proj;
   for (int i =0; i < 4; ++i)
   {
      for (int j = 0; j < 4; ++j)
      {
         proj[i][j] = projectionMatrix[i][j];
      }
   }
   minBox = minBox * proj;
   maxBox = maxBox * proj;
   minBox.x /= minBox.z;
   minBox.y /= minBox.z;
   maxBox.x /= maxBox.z;
   maxBox.y /= maxBox.z;
   
   float maxScreen = float(AiMax(AiMax(std::abs(minBox.x), std::abs(maxBox.x)), AiMax(std::abs(minBox.y), std::abs(maxBox.y))));
   // if maxScreen == 1 -> don't zoom
   // > 1 need to zoom out
   // < 1 need to zoom in

   newPos = center;
   newPos -=  viewDirection * centerDist * maxScreen;
   camera.set(newPos, viewDirection, upDirection, camera.horizontalFieldOfView(), camera.aspectRatio());
   camera.setCenterOfInterestPoint(center, MSpace::kWorld);


}

CRenderViewMtoARotate::CRenderViewMtoARotate() : CRenderViewRotateManipulator()
{
   AtNode *arnold_camera = AiUniverseGetCamera();
   if (arnold_camera == NULL) return;

   AtString camName = (AiNodeLookUpUserParameter(arnold_camera, "dcc_name")) ? 
      AiNodeGetStr(arnold_camera, "dcc_name") : AtString(AiNodeGetName(arnold_camera));

   MSelectionList camList;
   camList.add(MString(camName.c_str()));

   camList.getDagPath(0, m_cameraPath);

   if (!m_cameraPath.isValid()) return;

   MObject camNode = m_cameraPath.node();

   m_camera.setObject(m_cameraPath);      

   m_originalMatrix = m_cameraPath.inclusiveMatrix();
   MStatus status;
   m_originalPosition = m_camera.eyePoint(MSpace::kWorld, &status);

   m_center = m_camera.centerOfInterestPoint(MSpace::kWorld);
   m_centerDist = float(m_center.distanceTo(m_originalPosition));
   m_upDirection = m_camera.upDirection(MSpace::kWorld);
   

}


void CRenderViewMtoARotate::MouseDelta(int deltaX, int deltaY)
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
   
   rot.incrementalRotateBy (fElevationAxis, .01f * (deltaY));
   
   rot.incrementalRotateBy (MGlobal::upAxis(), - .01f * (deltaX));

   
   transformPath.setRotation(rot);

   transformPath.setRotatePivot (previousRp, MSpace::kWorld, true);
   MVector nextRt = transformPath.rotatePivotTranslation (MSpace::kWorld);

   transformPath.translateBy(nextRt - previousRt, MSpace::kWorld);
   transformPath.setRotatePivotTranslation(previousRt, MSpace::kWorld);
   m_camera.setCenterOfInterestPoint(m_center, MSpace::kWorld);
}


void CRenderViewMtoA::UpdateColorManagement()
{
   // Maya Color Management (aka SynColor) offers a command to retrieve 
   // its complete status; the command is colorManagementPrefs.
   // At the same time it also offers
   // capabilities to listen on any Color Management events using the
   // already existing MEventMessage (or scriptJob for mel code), 
   // the tags are prefixed with 'ColorMgt'.
   // By default the Maya Color Mgt is on; however, it could be disabled
   // at any time.

   int cmEnabled = 0;
   MGlobal::executeCommand("colorManagementPrefs -q -cmEnabled", cmEnabled);

   int cmOcioEnabled = 0;
   MGlobal::executeCommand("colorManagementPrefs -q -cmConfigFileEnabled", cmOcioEnabled);

   MString ocioFilepath;
   MGlobal::executeCommand("colorManagementPrefs -q -configFilePath", ocioFilepath);

   MString renderingSpace;
   MGlobal::executeCommand("colorManagementPrefs -q -renderingSpaceName", renderingSpace);

   MString viewTransform;
   MGlobal::executeCommand("colorManagementPrefs -q -viewTransformName", viewTransform);

   MStringArray viewTransforms;
   MGlobal::executeCommand("colorManagementPrefs -q -viewTransformNames", viewTransforms);
   MString userPrefsDir;
   MGlobal::executeCommand("internalVar -userPrefDir", userPrefsDir);
   userPrefsDir += "/synColorConfig.xml";

   std::string allViewTransforms;
   for(unsigned idx=0; idx<viewTransforms.length(); ++idx)
   {
      allViewTransforms += viewTransforms[idx].asChar();
      allViewTransforms += ";";
   }

   // Set the Color Management configuration before doing anything.
   SetOption("Color Management.Config Files",  userPrefsDir.asChar()); 

   // The order of initialization is important to avoid useless changes.
   SetOption("Color Management.Enabled",        "false");
   SetOption("Color Management.OCIO File",   ocioFilepath.asChar()); 
   SetOption("Color Management.OCIO",    cmOcioEnabled==1 ? "true" : "false"); 
   SetOption("Color Management.Rendering Space", renderingSpace.asChar()); 
   if (cmEnabled == 1)
   {
      SetOption("Color Management.View Transforms", allViewTransforms.c_str()); 
      SetOption("Color Management.View Transform",  viewTransform.asChar()); 
   } else
   {
      SetOption("Color Management.View Transforms", ""); 
      SetOption("Color Management.View Transform",  ""); 
   }
   SetOption("Color Management.Gamma",          "1"); 
   SetOption("Color Management.Exposure",       "0");
   SetOption("Color Management.Enabled",        cmEnabled==1 ? "true" : "false");
  
}

void CRenderViewMtoA::ColorMgtChangedCallback(void *data)
{
   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;
   
   if(renderViewMtoA->m_rvIdleCb)
   {
      MMessage::removeCallback(renderViewMtoA->m_rvIdleCb);
      renderViewMtoA->m_rvIdleCb = 0;   
   }
   renderViewMtoA->UpdateColorManagement();
}

void CRenderViewMtoA::ColorMgtCallback(MObject& node, MPlug& plug, void* clientData)
{
   CRenderViewMtoA *rvMtoA = (CRenderViewMtoA *)clientData;
   MStatus status;
   if(rvMtoA->m_rvIdleCb == 0)
   {

      rvMtoA->m_rvIdleCb = MEventMessage::addEventCallback("idle",
                                                  CRenderViewMtoA::ColorMgtChangedCallback,
                                                  clientData,
                                                  &status);
   }
}
void CRenderViewMtoA::ResolutionChangedCallback(void *data)
{
   if (data == NULL) return;
   CRenderViewMtoA *renderViewMtoA = (CRenderViewMtoA *)data;
   
   if(renderViewMtoA->m_rvIdleCb)
   {
      MMessage::removeCallback(renderViewMtoA->m_rvIdleCb);
      renderViewMtoA->m_rvIdleCb = 0;   
   }

   MSelectionList resList;
   resList.add(MString(":defaultResolution"));
   
   if(resList.length() == 0) return;

   MObject resObject;
   resList.getDependNode(0,resObject);
   MFnDependencyNode depNode(resObject);

   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   CRenderOptions *renderOptions = (renderSession) ? renderSession->RenderOptions() : NULL;
   
   if (renderOptions == NULL) return;

   MStatus status;
   int width = 1;
   int height = 1;
   float pixelAspectRatio = 1.f;
   bool updateRender = false;
   
   MPlug plug = depNode.findPlug("width", true, &status);
   if (status == MS::kSuccess)
   {
      width = plug.asInt();
      if (width != (int)renderOptions->width()) updateRender = true;
   }
   plug = depNode.findPlug("height", true, &status);
   if (status == MS::kSuccess)
   {
      height = plug.asInt();
      if (height != (int)renderOptions->height()) updateRender = true;
   }
   plug = depNode.findPlug("deviceAspectRatio", true, &status);
   if (status == MS::kSuccess)
   {
      pixelAspectRatio = 1.0f / (((float)height / width) * plug.asFloat());
      if (std::abs(pixelAspectRatio - renderOptions->pixelAspectRatio()) > AI_EPSILON)
      {
         updateRender = true;
      }
   }

   if(updateRender)
   {
      /* FIXME we could probably do a simple re-render
      renderSession->InterruptRender(true);
      renderSession->SetResolution(width, height);
      renderViewMtoA->SetOption("Refresh Render", "1");
      */
      renderViewMtoA->SetOption("Full IPR Update", "1");

      // want to resize the window
      if (width  > 1 && height > 1 && pixelAspectRatio > 0.f)
         renderViewMtoA->Resize(width * pixelAspectRatio, height);
   }


}
void CRenderViewMtoA::ResolutionCallback(MObject& node, MPlug& plug, void* clientData)
{
   CRenderViewMtoA *rvMtoA = (CRenderViewMtoA *)clientData;
   MStatus status;

   if(rvMtoA->m_rvIdleCb == 0)
   {

      rvMtoA->m_rvIdleCb = MEventMessage::addEventCallback("idle",
                                                  CRenderViewMtoA::ResolutionChangedCallback,
                                                  clientData,
                                                  &status);
   }

}

void CRenderViewMtoA::SequenceRenderCallback(float elapsedTime, float lastTime, void *data)
{
   if (s_sequenceData == NULL){return;}

   CRenderViewMtoA *rvMtoA = (CRenderViewMtoA *)data;

   if (MProgressWindow::isCancelled())
   {
      CMayaScene::GetRenderSession()->InterruptRender(true);
      MProgressWindow::endProgress();
      rvMtoA->SetOption("Scene Updates", s_sequenceData->sceneUpdatesValue.c_str());
      rvMtoA->SetOption("Save Final Images", s_sequenceData->saveImagesValue.c_str());
      if (rvMtoA->m_rvIdleCb)
      {
         MMessage::removeCallback(rvMtoA->m_rvIdleCb);
         rvMtoA->m_rvIdleCb = 0;
      }
      return;
   }

   if(!s_sequenceData->renderStarted)
   {
      if (AiRendering()) {s_sequenceData->renderStarted = true; }
      
   } else
   {
      if(!AiRendering())
      {
         // this frame has finished !
         s_sequenceData->current += s_sequenceData->step;
         if (s_sequenceData->current > s_sequenceData->last)
         {
            MProgressWindow::endProgress();
            rvMtoA->SetOption("Scene Updates", s_sequenceData->sceneUpdatesValue.c_str());
            rvMtoA->SetOption("Save Final Images", s_sequenceData->saveImagesValue.c_str());
            if (rvMtoA->m_rvIdleCb)
            {
               MMessage::removeCallback(rvMtoA->m_rvIdleCb);
               rvMtoA->m_rvIdleCb = 0;
            }
            return;
         }
         s_sequenceData->renderStarted = false;
         MProgressWindow::setProgress(s_sequenceData->current);

         MString progressStr = MString("Rendering Frame ") + MProgressWindow::progress();
         MGlobal::viewFrame(s_sequenceData->current);
         MProgressWindow::setProgressStatus(progressStr);
         MGlobal::displayInfo(progressStr);
         rvMtoA->SetOption("Update Full Scene", "1");

      } else
      {
         // still computing
         // nothing to do ?
      }
   }

}


MStatus CRenderViewMtoA::RenderSequence(float first, float last, float step)
{
   if (m_rvIdleCb)
   {
      MMessage::removeCallback(m_rvIdleCb);
      m_rvIdleCb = 0;
   } 
   // make sure no render is going on
   CMayaScene::GetRenderSession()->InterruptRender(true);

   if (s_sequenceData) 
   {
      delete s_sequenceData;
      s_sequenceData = NULL;
   }


   /*
   FIXME : we'd need to find the original value of scene updates / save final images
   but they're not in "serialize" yet

   std::string serialized = Serialize();

   size_t npos = serialized.find("Scene Updates");
   if (npos != std::string::npos)
   {
   }
   npos = serialized.find("Save Final Images");
   if (npos != std::string::npos)
   {
   }
   */

   SetOption("Scene Updates", "0");
   SetOption("Save Final Images", "1");
   
   s_sequenceData = new CARVSequenceData;
   s_sequenceData->first = first;
   s_sequenceData->current = first;
   s_sequenceData->last = last;
   s_sequenceData->step = step;
   s_sequenceData->renderStarted = false;
   s_sequenceData->sceneUpdatesValue = "1";
   s_sequenceData->saveImagesValue = "0";

   if (!MProgressWindow::reserve())
   {
      MGlobal::displayError("Progress window already in use.");
      return MS::kFailure;
   }

   MProgressWindow::setProgressRange(first, last);
   MProgressWindow::setTitle(MString("Sequence Rendering"));
   MProgressWindow::setInterruptable(true);
   MProgressWindow::setProgress(first);

   MString progressWindowState = MString("Sequence Rendering:") +
          MString("\nFrames ") + MProgressWindow::progressMin() +
          MString(" to ") + MProgressWindow::progressMax() + 
          MString(" (step ") + step + MString(")");

   MGlobal::displayInfo(progressWindowState);
   MProgressWindow::startProgress();

   MGlobal::viewFrame(first);
   MString progressStr = MString("Rendering Frame ") + MProgressWindow::progress();
   MProgressWindow::setProgressStatus(progressStr);
   MGlobal::displayInfo(progressStr);

   SetOption("Update Full Scene", "1");
   
   // connect to Idle
   MStatus status;

   m_rvIdleCb = MTimerMessage::addTimerCallback(0.1f,
                                                  CRenderViewMtoA::SequenceRenderCallback,
                                                  this,
                                                  &status);

   return status;
}

void CRenderViewMtoA::PreProgressiveStep()
{
   if (!m_hasPreProgressiveStep) return;
   CMayaScene::ExecuteScript(m_preProgressiveStep, false, true);

}
void CRenderViewMtoA::PostProgressiveStep()
{
   if (!m_hasPostProgressiveStep) return;
   CMayaScene::ExecuteScript(m_postProgressiveStep, false, true);

}
void CRenderViewMtoA::ProgressiveRenderStarted()
{
   if (!m_hasProgressiveRenderStarted) return;
   CMayaScene::ExecuteScript(m_progressiveRenderStarted, false, true);
}

void CRenderViewMtoA::ProgressiveRenderFinished()
{
   if (!m_hasProgressiveRenderFinished) return;
   CMayaScene::ExecuteScript(m_progressiveRenderFinished, false, true);
}

void CRenderViewMtoA::Resize(int width, int height, bool drawableArea)
{
#ifdef ARV_DOCKED
   if (s_arvWorkspaceControl == NULL)
      return;
#endif
   
   CRenderViewInterface::Resize(width, height);

   if(MGlobal::apiVersion() < 201760) // this option was only implemented in Maya 2017 Update 4
      return;

   if (s_creatingARV)
      return;

#ifdef ARV_DOCKED

   int isFloating = 0;
   MGlobal::executeCommand("workspaceControl -q -fl \"ArnoldRenderView\"", isFloating);

   // only resize the workspace if the window is floating
   if (isFloating)
   {
      MString workspaceCmd = "workspaceControl -edit";
      workspaceCmd += " -rsw ";
      workspaceCmd += width;
      workspaceCmd += " -rsh ";
      workspaceCmd += height;
      workspaceCmd += " \"ArnoldRenderView\"";
      MGlobal::executeCommand(workspaceCmd);
   }


#endif
}

#endif
