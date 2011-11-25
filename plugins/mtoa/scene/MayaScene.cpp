
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
CArnoldSession* CMayaScene::s_arnoldSession = NULL;

// Cheap singleton
CRenderSession* CMayaScene::GetRenderSession()
{
   if (!s_renderSession)
      s_renderSession = new CRenderSession();

   return s_renderSession;
}

CArnoldSession* CMayaScene::GetArnoldSession()
{
   if (!s_arnoldSession)
      s_arnoldSession = new CArnoldSession();

   return s_arnoldSession;
}

MStatus CMayaScene::Begin(ArnoldSessionMode mode)
{
   MStatus status = MStatus::kSuccess;

   CRenderSession* renderSession = GetRenderSession();
   CArnoldSession* arnoldSession = GetArnoldSession();

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
   renderOptions.SetArnoldRenderOptions(ArnoldRenderOptionsNode);

   if (mode == MTOA_SESSION_SWATCH)
   {
      // FIXME: default or use swatch defaults
      //renderOptions.SetBatch(false);
      renderOptions.SetProgressive(false);
      //FIXME: renderOptions.SetupLog() should do this for given mode
      MtoaSetupSwatchLogging();
   }
   else if (mode == MTOA_SESSION_ASS)
   {
      sessionOptions.GetFromMaya();
      renderOptions.GetFromMaya();
      //renderOptions.SetBatch(true);
      renderOptions.SetupLog();
   }
   else if (mode == MTOA_SESSION_IPR)
   {
      sessionOptions.GetFromMaya();
      renderOptions.GetFromMaya();
      //renderOptions.SetBatch(false);
      renderOptions.SetupLog();
      status = SetupIPRCallbacks();
   }
   else if (mode == MTOA_SESSION_RENDER)
   {
      sessionOptions.GetFromMaya();
      renderOptions.GetFromMaya();
      //renderOptions.SetBatch(false);
      renderOptions.SetProgressive(false);
      renderOptions.SetupLog();
   }
   else if (mode == MTOA_SESSION_BATCH)
   {
      sessionOptions.GetFromMaya();
      renderOptions.GetFromMaya();
      //renderOptions.SetBatch(true);
      renderOptions.SetupLog();
   }

   // Init both render and export sessions
   status = renderSession->Begin(&renderOptions);
   status = arnoldSession->Begin(&sessionOptions);


   return status;
}

MStatus CMayaScene::End()
{
   MStatus status = MStatus::kSuccess;

   ClearIPRCallbacks();
   if (NULL != s_renderSession) status = s_renderSession->End();
   if (NULL != s_arnoldSession) status = s_arnoldSession->End();

   return status;
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
   if ( s_IPRIdleCallbackId == 0 && !s_renderSession->m_paused_ipr )
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
   if (NULL != s_arnoldSession)
   {
      s_arnoldSession->ClearUpdateCallbacks();
   }
}

// Actuall callback functions

void CMayaScene::IPRNewNodeCallback(MObject & node, void *)
{
   // If this is a node we've exported before (e.g. user deletes then undos)
   // we can shortcut and just call the update for it's already existing translator.
   // Interupt rendering
   CRenderSession* renderSession = GetRenderSession();
   CArnoldSession* arnoldSession = GetArnoldSession();
   renderSession->InterruptRender();
   std::vector<CNodeTranslator *> translators;
   if (arnoldSession->GetActiveTranslators(node, translators))
   {
      for (unsigned int i=0; i < translators.size(); ++i)
         arnoldSession->QueueForUpdate(translators[i]);
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
   if (s_arnoldSession->NeedsUpdate())
   {
      s_renderSession->InterruptRender();
      s_arnoldSession->SetExportFrame(MAnimControl::currentTime().as(MTime::uiUnit()));
      s_arnoldSession->DoUpdate();
      s_renderSession->DoIPRRender();
   }
}






