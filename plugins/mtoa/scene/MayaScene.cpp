
#include "MayaScene.h"
#include "extension/ExtensionsManager.h"
#include "utils/MtoaLog.h"

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

std::vector< CNodeTranslator * > CMayaScene::s_translatorsToIPRUpdate;
MCallbackId CMayaScene::s_IPRIdleCallbackId = 0;
MCallbackId CMayaScene::s_NewNodeCallbackId = 0;
CRenderSession* CMayaScene::s_renderSession = NULL;
CArnoldSession* CMayaScene::s_exportSession = NULL;

// Cheap singleton
CRenderSession* CMayaScene::GetRenderSession()
{
   if (!s_renderSession)
      s_renderSession = new CRenderSession();

   return s_renderSession;
}

CArnoldSession* CMayaScene::GetExportSession()
{
   if (!s_exportSession)
      s_exportSession = new CArnoldSession();

   return s_exportSession;
}

MStatus CMayaScene::Begin(ArnoldSessionMode mode)
{
   MStatus status = MStatus::kSuccess;

   CRenderSession* renderSession = GetRenderSession();
   CArnoldSession* arnoldSession = GetExportSession();

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

   CSessionOptions sessionOptions;
   sessionOptions.SetSessionMode(mode);
   sessionOptions.SetExportFrame(MAnimControl::currentTime().as(MTime::uiUnit()));
   sessionOptions.SetArnoldRenderOptions(ArnoldRenderOptionsNode);

   CRenderOptions renderOptions;
   // FIXME : allow to pass a specific maya ArnoldRenderOptions node
   renderOptions.GetFromMaya();
   if (mode == MTOA_SESSION_SWATCH)
   {
      // FIXME: default or use swatch defaults
      // renderOptions = CRenderOptions();
      renderOptions.SetBatch(false);
      renderOptions.SetProgressive(false);
      //FIXME: fill renderOptions instead
      MtoaSetupSwatchLogging();
   }
   else if (mode == MTOA_SESSION_ASS)
   {
      renderOptions.SetBatch(true);
      renderOptions.SetupLog();
   }
   else if (mode == MTOA_SESSION_IPR)
   {
      renderOptions.SetBatch(false);
      renderOptions.SetProgressive(true);
      renderOptions.SetupLog();
      status = SetupIPRCallbacks();
   }
   else
   {
      renderOptions.SetupLog();
   }

   // Init both render and export sessions
   status = renderSession->Begin(&renderOptions);
   status = arnoldSession->Begin(&sessionOptions);

   // renderSession->RenderOptions()->SetupLog();


   return status;
}

MStatus CMayaScene::End()
{
   MStatus status = MStatus::kSuccess;

   if (NULL != s_renderSession)
   {
      status = s_renderSession->End();
   }
   if (NULL != s_exportSession)
   {
      if (s_exportSession->GetSessionMode() == MTOA_SESSION_IPR)
      {
         ClearIPRCallbacks();
      }
      status = s_exportSession->End();
   }

   return status;
}

MStatus CMayaScene::Export(MSelectionList* selected)
{
   MStatus status;

   if (NULL != s_exportSession)
   {
      status = s_exportSession->Export(selected);
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

   if (NULL != s_exportSession && NULL!= s_renderSession)
   {
      // Save current frame
      // double currentFrame = MAnimControl::currentTime().as(MTime::uiUnit());

      bool isIpr = (s_exportSession->GetSessionMode() == MTOA_SESSION_IPR) ? true : false;
      // if (isIpr) status = SetupIPRCallbacks();

      // FIXME: a generic renderSessio->Render() method that chooses render from the ArnoldSessionMode ?
      if (isIpr)
      {
         s_renderSession->DoIPRRender();
      }
      else
      {
         s_renderSession->DoInteractiveRender();
      }

      // if (isIpr) ClearIPRCallbacks();

      // Restorecurrent frame
      // MGlobal::viewFrame(MTime(currentFrame, MTime::uiUnit()));
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

MStatus CMayaScene::ExecuteScript(const MString &str, bool echo)
{
   MStatus status = MStatus::kSuccess;

   if (str.length() > 0)
   {
      status = MGlobal::executeCommand(str, echo);
   }

   return status;
}

MStatus CMayaScene::UpdateIPR()
{
   MStatus status;
   MCallbackId id;

   // Add the IPR update callback, this is called in Maya's idle time
   if ( s_IPRIdleCallbackId == 0 )
   {
      id = MEventMessage::addEventCallback("idle", IPRIdleCallback, NULL, &status);
      if (status == MS::kSuccess) s_IPRIdleCallbackId = id;
   }

   return status;
}

// Private Methods

MStatus CMayaScene::SetupIPRCallbacks()
{
   MStatus status;
   MCallbackId id;
   // Add the node added callback
   if (s_NewNodeCallbackId == 0)
   {
      id = MDGMessage::addNodeAddedCallback(IPRNewNodeCallback, "dependNode", NULL, &status);
      if (status == MS::kSuccess) s_NewNodeCallbackId = id;
   }

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

   // Clear the callbacks on the translators of the current export session
   if (NULL != s_exportSession)
   {
      s_exportSession->ClearUpdateCallbacks();
   }
}

// Actuall callback functions

void CMayaScene::IPRNewNodeCallback(MObject & node, void *)
{
   // If this is a node we've exported before (e.g. user deletes then undos)
   // we can shortcut and just call the update for it's already existing translator.
   // Interupt rendering
   CRenderSession* renderSession = GetRenderSession();
   CArnoldSession* arnoldSession = GetExportSession();
   renderSession->InterruptRender();
   CNodeTranslator * translator = arnoldSession->GetActiveTranslator(node);
   if (translator != NULL)
   {
      arnoldSession->QueueForUpdate(translator);
   }
   else
   {
      // Else export this node as it's completely new to us.
      MFnDagNode dag_node(node);
      MDagPath path;
      const MStatus status = dag_node.getPath(path);
      if (status == MS::kSuccess)
      {
         AiMsgDebug("[mtoa] Exporting new node: %s", path.partialPathName().asChar());
         arnoldSession->ExportDagPath(path);
         // arnoldSession->QueueForUpdate(); // add it?
      }
   }
   UpdateIPR();
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
   if (s_exportSession->NeedsUpdate() && !s_renderSession->m_paused_ipr)
   {
      s_renderSession->InterruptRender();
      s_exportSession->SetExportFrame(MAnimControl::currentTime().as(MTime::uiUnit()));
      s_exportSession->DoUpdate();
      s_renderSession->DoIPRRender();
   }
}






