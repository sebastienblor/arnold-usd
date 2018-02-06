#include "MayaScene.h"
#include "extension/ExtensionsManager.h"
#include "utils/MtoaLog.h"
#include "utils/BuildID.h"
#include "nodes/ShaderUtils.h"
#include "render/MaterialView.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/M3dView.h>
#include <maya/MAnimControl.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnSet.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnTransform.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnInstancer.h>
#include <maya/MItInstancer.h>
#include <maya/MPlugArray.h>
#include <maya/MMessage.h>
#include <maya/MEventMessage.h>
#include <maya/MDGMessage.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFileObject.h>

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MCallbackId CMayaScene::s_IPRIdleCallbackId = 0;
MCallbackId CMayaScene::s_NewNodeCallbackId = 0;
MCallbackId CMayaScene::s_QuitApplicationCallbackId = 0;
MCallbackId CMayaScene::s_FileOpenCallbackId = 0;
MCallbackId CMayaScene::s_AddParentCallbackId = 0;
MCallbackId CMayaScene::s_RemoveParentCallbackId = 0;


CRenderSession* CMayaScene::s_renderSession = NULL;
CArnoldSession* CMayaScene::s_arnoldSession = NULL;
AtCritSec CMayaScene::s_lock = NULL;
bool CMayaScene::s_active = false;

// Cheap singleton
CRenderSession* CMayaScene::GetRenderSession()
{
   return s_renderSession;
}

CArnoldSession* CMayaScene::GetArnoldSession()
{
   return s_arnoldSession;
}

bool CMayaScene::IsActive(ArnoldSessionMode mode)
{
   if (mode != MTOA_SESSION_ANY && mode != GetSessionMode())
      return false;
   AiCritSecEnter(&s_lock);
   bool active = (s_arnoldSession != NULL && s_arnoldSession->IsActive())
         || (s_renderSession != NULL && s_renderSession->IsActive());
   AiCritSecLeave(&s_lock);
   return active;
}

ArnoldSessionMode CMayaScene::GetSessionMode()
{
   if (s_arnoldSession != NULL)
      return s_arnoldSession->GetSessionMode();
   else
      return MTOA_SESSION_UNDEFINED;
}

bool CMayaScene::IsExportingMotion()
{
   if (s_arnoldSession != NULL)
      return s_arnoldSession->IsExportingMotion();
   else
      return false;
}

MStatus CMayaScene::Begin(ArnoldSessionMode mode)
{
   AiCritSecEnter(&s_lock);
   // prevent this from running twice
   if (s_active)
   {
      AiCritSecLeave(&s_lock);
      return MStatus::kSuccess;
   }
   s_active = true;
   AiCritSecLeave(&s_lock);

   MStatus status = MStatus::kSuccess;

   MSelectionList optionsList;
   optionsList.add("defaultArnoldRenderOptions");
   if (optionsList.length() == 0)
   {
      // arnold options weren't initialized yet
      MGlobal::executePythonCommand("import mtoa.core as core; core.createOptions()");
   }


   // Suspend material view during all render sessions, except for 
   // sequence rendering that handles this seperatelly, and swatches
   // which has lower priority and will be aborted by material viewer,
   // 
   if (mode != MTOA_SESSION_MATERIALVIEW && mode != MTOA_SESSION_SEQUENCE && mode != MTOA_SESSION_SWATCH)
   {
      CMaterialView::Suspend();
   }

   // FIXME: raise an error if Begin is called on active session
   // (forcing a CMayaScene::End() to be called before a CMayaScene::Begin() ?
   if (s_renderSession == NULL)
      s_renderSession = new CRenderSession();
   if (s_arnoldSession == NULL)
      s_arnoldSession = new CArnoldSession();

   MString mayaVersion = MGlobal::mayaVersion();     
   MString appString = MString("MtoA ") + MTOA_VERSION + " " + BUILD_ID + " Maya " + mayaVersion;
   AiSetAppString(appString.asChar());

   MSelectionList    list;
   MObject           defaultRenderGlobalsNode;
   MObject           ArnoldRenderOptionsNode;
   MFnDependencyNode fnCommonRenderOptions;
   MFnDependencyNode fnArnoldRenderOptions;
   
   list.add("defaultRenderGlobals");
   if (list.length() > 0)
   {
      list.getDependNode(0, defaultRenderGlobalsNode);
      fnCommonRenderOptions.setObject(defaultRenderGlobalsNode);
   }

   list.clear();
   // FIXME : allow to pass a specific options node
   list.add("defaultArnoldRenderOptions");
   if (list.length() > 0)
   {
      list.getDependNode(0, ArnoldRenderOptionsNode);
      fnArnoldRenderOptions.setObject(ArnoldRenderOptionsNode);
   }
   else
   {
      AiMsgError("[mtoa] could not find defaultArnoldRenderOptions");
   }

   // FIXME: why do we have a double storage for the render options node?
   CSessionOptions sessionOptions;
   sessionOptions.SetArnoldRenderOptions(ArnoldRenderOptionsNode);
   sessionOptions.SetSessionMode(mode);
   sessionOptions.SetExportFrame(MAnimControl::currentTime().as(MTime::uiUnit()));

   CRenderOptions renderOptions;
   // Don't use the scene render options (aiOptions) for swatches
   if (mode != MTOA_SESSION_SWATCH)
   {
      sessionOptions.GetFromMaya();
      renderOptions.SetArnoldRenderOptions(ArnoldRenderOptionsNode);
      renderOptions.GetFromMaya();
   }
   // Setup log
   renderOptions.SetupLog();

   if (mode == MTOA_SESSION_SWATCH)
   {
      // FIXME: default or use swatch defaults
      // renderOptions.SetBatch(false);
      renderOptions.SetProgressive(false);
      sessionOptions.SetProgressive(false);
   }
   else if (mode == MTOA_SESSION_ASS)
   {
      // renderOptions.SetBatch(true);
   }
   else if (mode == MTOA_SESSION_IPR ||mode == MTOA_SESSION_RENDERVIEW)
   {
      // renderOptions.SetBatch(false);
      status = SetupIPRCallbacks();
	  s_renderSession->CloseRenderViewWithSession(true);
   }
   else if (mode == MTOA_SESSION_RENDER || mode == MTOA_SESSION_SEQUENCE)
   {
      // renderOptions.SetBatch(false);
      renderOptions.SetProgressive(false);
      sessionOptions.SetProgressive(false);
   }
   else if (mode == MTOA_SESSION_BATCH)
   {
      // renderOptions.SetBatch(true);
      renderOptions.SetProgressive(false);
      sessionOptions.SetProgressive(false);
   }

   // Init both render and export sessions
   status = s_renderSession->Begin(renderOptions);
   status = s_arnoldSession->Begin(sessionOptions);

   MStatus cbStatus;
   
   MCallbackId id = MEventMessage::addEventCallback("quitApplication", QuitApplicationCallback, NULL, &cbStatus);
   if (cbStatus == MS::kSuccess) s_QuitApplicationCallbackId = id;
   
   id = MEventMessage::addEventCallback("PreFileNewOrOpened", FileOpenCallback, NULL, &cbStatus);
   if (cbStatus == MS::kSuccess) s_FileOpenCallbackId = id;

   return status;
}

MStatus CMayaScene::End()
{
   MStatus status = MStatus::kSuccess;

   AiCritSecEnter(&s_lock);
   // prevent this from running twice
   if (!s_active)
   {
      AiCritSecLeave(&s_lock);
      return MStatus::kSuccess;
   }
   s_active = false;
   AiCritSecLeave(&s_lock);

   // Terminate material view session if active
   if (GetSessionMode() == MTOA_SESSION_MATERIALVIEW)
   {
      CMaterialView::Abort();
   }

   ClearIPRCallbacks();
   if (s_renderSession != NULL)
   {
      // status = s_renderSession->End(); // Unnecessary it's in the destructor for CRenderSession already
      delete s_renderSession;
      s_renderSession = NULL;
   }
   if (s_arnoldSession != NULL)
   {
      // status = s_arnoldSession->End(); // Unnecessary it's in the destructor for CArnoldSession already
      delete s_arnoldSession;
      s_arnoldSession = NULL;
   }

   if (s_QuitApplicationCallbackId)
   {
      MMessage::removeCallback(s_QuitApplicationCallbackId);
      s_QuitApplicationCallbackId = 0;
   }

   if (s_FileOpenCallbackId)
   {
      MMessage::removeCallback(s_FileOpenCallbackId);
      s_FileOpenCallbackId = 0;
   }

   // Resume material view if previously suspended
   // Ignore for sequence rendering which handles this seperatelly, since 
   // the whole sequence of frames might not be completed yet and we don't 
   // want to resume in the middle of the sequence
   if (GetSessionMode() != MTOA_SESSION_SEQUENCE)
   {
      CMaterialView::Resume();
   }

   return status;
}

MStatus CMayaScene::Restart()
{
   CRenderOptions options = s_renderSession->m_renderOptions;
   CSessionOptions optionss = s_arnoldSession->m_sessionOptions;

   s_renderSession->End();
   s_arnoldSession->End();

   optionss.SetExportFrame(MAnimControl::currentTime().as(MTime::uiUnit()));

   s_renderSession->Begin(options);
   s_arnoldSession->Begin(optionss);

   s_arnoldSession->Export();

   s_renderSession->m_renderOptions.UpdateImageDimensions();

      // Execute post export callback
   MFnDependencyNode fnArnoldRenderOptions(GetSceneArnoldRenderOptionsNode());
   MString postTranslationCallbackScript = fnArnoldRenderOptions.findPlug("post_translation").asString();
   ExecuteScript(postTranslationCallbackScript);

   return MStatus::kSuccess;
}

bool CMayaScene::IsArnoldLight(const MObject & object)
{
   MFnDependencyNode depFn(object);
   std::string classification(MFnDependencyNode::classification(depFn.typeName()).asChar());
   if (classification.find(CLASSIFY_ARNOLD_LIGHT.asChar()) != std::string::npos)
      return true;
   else
      return false;
}


MObject CMayaScene::GetSceneArnoldRenderOptionsNode()
{
   // Get the current Arnold Render Options
   MSelectionList    list;
   MObject           ArnoldRenderOptionsNode;
   
   list.add("defaultArnoldRenderOptions");
   if (list.length() > 0)
   {
      list.getDependNode(0, ArnoldRenderOptionsNode);
   }
   
   return ArnoldRenderOptionsNode;
}

MStatus CMayaScene::Export(MSelectionList* selected)
{
   MStatus status;
   if (NULL != s_arnoldSession)
   {
      status = s_arnoldSession->Export(selected);
      // FIXME: provide access to resolution settings in arnoldSession, and export them
      // in OptionsTranslator
      s_renderSession->m_renderOptions.UpdateImageDimensions();

      // Execute post export callback
      // FIXME: do we also want to do it in Update() ? 
      MFnDependencyNode fnArnoldRenderOptions(GetSceneArnoldRenderOptionsNode());
      MString postTranslationCallbackScript = fnArnoldRenderOptions.findPlug("post_translation").asString();
      ExecuteScript(postTranslationCallbackScript);
   }
   else
   {
      status = MStatus::kFailure;
   }

   return status;
}

MStatus CMayaScene::Render()
{
   MStatus status;

   if (NULL != s_arnoldSession && NULL!= s_renderSession)
   {
      // Save current frame
      // double currentFrame = MAnimControl::currentTime().as(MTime::uiUnit());

      bool isIpr = (s_arnoldSession->GetSessionMode() == MTOA_SESSION_IPR) ? true : false;
      // if (isIpr) status = SetupIPRCallbacks();

      // FIXME: a generic renderSession->Render() method that chooses render from the ArnoldSessionMode ?
      if (isIpr)
      {
         s_renderSession->DoIPRRender();
      }
      else
      {
         s_renderSession->DoInteractiveRender();
      }

      // if (isIpr) ClearIPRCallbacks();
   }
   else
   {
      status = MStatus::kFailure;
   }

   return status;
}

MStatus CMayaScene::ExportAndRenderFrame( ArnoldSessionMode mode,
                                          MSelectionList* selected)
{

   if (MStatus::kSuccess != Begin(mode)) return MStatus::kFailure;
   if (MStatus::kSuccess != Export(selected)) return MStatus::kFailure;
   if (MStatus::kSuccess != Render()) return MStatus::kFailure;
   if (MStatus::kSuccess != End()) return MStatus::kFailure;

   return MStatus::kSuccess;
}

// TODO : implement that
MStatus CMayaScene::ExportAndRenderSequence( ArnoldSessionMode mode,
                                             MSelectionList* selected)
{

   if (MStatus::kSuccess != Begin(mode)) return MStatus::kFailure;
   if (MStatus::kSuccess != Export(selected)) return MStatus::kFailure;
   if (MStatus::kSuccess != Render()) return MStatus::kFailure;
   if (MStatus::kSuccess != End()) return MStatus::kFailure;

   return MStatus::kSuccess;
}

MStatus CMayaScene::ExecuteScript(const MString &str, bool echo, bool idle)
{
   MStatus status = MStatus::kSuccess;

   if (str.length() > 0)
   {
      if (idle)
         status = MGlobal::executeCommandOnIdle(str, echo);
      else
         status = MGlobal::executeCommand(str, echo);
   }

   return status;
}

MStatus CMayaScene::UpdateIPR()
{
   if (s_arnoldSession->GetSessionMode() == MTOA_SESSION_RENDERVIEW)
   {
      s_renderSession->UpdateRenderView();
      return MS::kSuccess;
   }
   MStatus status;
   MCallbackId id;

   // Add the IPR update callback, this is called in Maya's idle time
   if ( s_IPRIdleCallbackId == 0 && !s_renderSession->m_paused_ipr )
   {
      id = MEventMessage::addEventCallback("idle", IPRIdleCallback, NULL, &status);
      if (status == MS::kSuccess) s_IPRIdleCallbackId = id;
   }

   return status;
}

void CMayaScene::Init() {AiCritSecInit((void**)&s_lock);}

void CMayaScene::DeInit()  {
   AiCritSecClose((void**)&s_lock);
   CRenderSession::DeleteRenderView();
}

// Private Methods

MStatus CMayaScene::SetupIPRCallbacks()
{
   MStatus status;
   MCallbackId id;
   // Add the node added callback
   if (s_NewNodeCallbackId == 0)
   {
      // We used to call the callback for every "dependNode", but now we're just doing it
      // for dag nodes. This is introduced for #2540 when hypershade is opened, but more generically
      // it shouldn't be necessary to restart when every node is created. Depend nodes should
      // only appear once they're connected to other exported nodes
      id = MDGMessage::addNodeAddedCallback(IPRNewNodeCallback, "dagNode", NULL, &status);
      if (status == MS::kSuccess)
      {
         s_NewNodeCallbackId = id;
      }
      else
      {
         AiMsgError("[mtoa] Unable to install IPR node added callback");
      }
   }

   if (s_AddParentCallbackId == 0)
   {
      id = MDagMessage::addParentAddedCallback(IPRParentingChangedCallback, NULL, &status);
      if (status == MS::kSuccess)
      {
         s_AddParentCallbackId = id;
      }
      else
      {
         AiMsgError("[mtoa] Unable to install IPR parent added callback");
      }
   }
   if (s_RemoveParentCallbackId == 0)
   {
      id = MDagMessage::addParentRemovedCallback(IPRParentingChangedCallback, NULL, &status);
      if (status == MS::kSuccess)
      {
         s_RemoveParentCallbackId = id;
      }
      else
      {
         AiMsgError("[mtoa] Unable to install IPR parent removed callback");
      }
   }
   
   // TODO : might add a forceUpdateCallback to re-export when frame changes
   // static MCallbackId 	addForceUpdateCallback (MMessage::MTimeFunction func, void *clientData=NULL, MStatus *ReturnStatus=NULL)
 	// This method registers a callback that is called after the time changes and after all nodes have been evaluated in the dependency graph. 

   return status;
}

void CMayaScene::ClearIPRCallbacks()
{
   // Clear the global IPR callbacks
   if (s_IPRIdleCallbackId != 0)
   {
      MMessage::removeCallback(s_IPRIdleCallbackId);
      s_IPRIdleCallbackId = 0;
   }

   if (s_NewNodeCallbackId != 0)
   {
      MMessage::removeCallback(s_NewNodeCallbackId);
      s_NewNodeCallbackId = 0;
   }
   if (s_AddParentCallbackId != 0)
   {
      MMessage::removeCallback(s_AddParentCallbackId);
      s_AddParentCallbackId = 0;
   }
   if (s_RemoveParentCallbackId != 0)
   {
      MMessage::removeCallback(s_RemoveParentCallbackId);
      s_RemoveParentCallbackId = 0;
   }
   // Clear the callbacks on the translators of the current export session
   if (NULL != s_arnoldSession)
   {
      s_arnoldSession->ClearUpdateCallbacks();
   }
}

// Actuall callback functions

// We used to call the callback for every "dependNode", but now we're just doing it
// for dag nodes. This is introduced for #2540 when hypershade is opened, but more generically
// it shouldn't be necessary to restart when every node is created. Depend nodes should
// only appear once they're connected to other exported nodes
void CMayaScene::IPRNewNodeCallback(MObject & node, void *)
{

   MFnDependencyNode depNodeFn(node);
   MString type = depNodeFn.typeName();

   
   MString name = depNodeFn.name();
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.ipr] IPRNewNodeCallback on "+ MString(name.asChar())+" ("+MString(type.asChar())+")");
      
   CArnoldSession* arnoldSession = GetArnoldSession();

   MFnDagNode dagNodeFn(node);
   MDagPath path;
   const MStatus status = dagNodeFn.getPath(path);
   if (status == MS::kSuccess)
   {
      arnoldSession->QueueForUpdate(path);
   }
   else
   {
      arnoldSession->QueueForUpdate(node);
   }

   // FIXME : instead of testing specific types, we could 
   // simply get a translator for this type (as ArnoldSession::ExportDagPath does).
   // if no translator is provided then we skip it
   if(type == "transform" ||type == "locator") return; // no need to do anything with a simple transform node

   // new cameras shouldn't restart IPR
   if (node.hasFn(MFn::kCamera)) 
   {
      // add this camera to the renderview list
      s_renderSession->SetRenderViewOption(MString("Add Camera"), CDagTranslator::GetArnoldNaming(path));
      return;
   }

   arnoldSession->RequestUpdate();
}

void CMayaScene::IPRParentingChangedCallback(MDagPath &child, MDagPath &parent, void *clientData)
{
   GetArnoldSession()->RecursiveUpdateDagChildren(child); 
}
void CMayaScene::FileOpenCallback(void *)
{
   // something we might want to do when a new file is opened

   // for now we only call End() for the RenderView or MaterialView
   // as IPR already handles it by calling IPR "stop"
   if (GetSessionMode() == MTOA_SESSION_RENDERVIEW || GetSessionMode() == MTOA_SESSION_MATERIALVIEW)
   {      
      End();
   }
   // We want to clear the caches, since the scene is changing (#3277)
   AiUniverseCacheFlush(AI_CACHE_ALL);
}


void CMayaScene::QuitApplicationCallback(void *)
{
   // something we might want to do when closing maya
   End();
}

void CMayaScene::IPRIdleCallback(void *)
{
   // Desactivate the callback (it's supposed to fire only once)
   if (s_IPRIdleCallbackId != 0)
   {
      MMessage::removeCallback(s_IPRIdleCallbackId);
      s_IPRIdleCallbackId = 0;
   }

   // Check that an update is really needed.
   if (s_arnoldSession->NeedsUpdate())
   {
      s_renderSession->InterruptRender();

      MFnDependencyNode fnArnoldRenderOptions(GetSceneArnoldRenderOptionsNode());
      bool forceUpdate = fnArnoldRenderOptions.findPlug("force_scene_update_before_IPR_refresh").asBool();
            
      if(forceUpdate)
      {
         CMayaScene::Restart();
      }
      else
      {
         s_arnoldSession->DoUpdate();
      }
      
      s_renderSession->DoIPRRender();
   }
}



void CMayaScene::UpdateSceneChanges()
{
   s_arnoldSession->DoUpdate();
}


