
#include "MayaScene.h"
#include "extension/ExtensionsManager.h"
#include "render/RenderSession.h"

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
CExportSession* CMayaScene::s_exportSession = NULL;
bool CMayaScene::s_isExportingMotion = false;


// Cheap singleton
CRenderSession* CMayaScene::GetRenderSession()
{
   if (!s_renderSession)
      s_renderSession = new CRenderSession();

   return s_renderSession;
}

CExportSession* CMayaScene::GetExportSession()
{
   if (!s_exportSession)
      s_exportSession = new CExportSession();

   return s_exportSession;
}

bool IsExportingMotion()
{
   CExportSession* exportSession = GetExportSession();
   if (NULL != exportSession)
   {
      return exportSession->IsExportingMotion();
   }
   else
   {
      return false;
   }
}

/// Primary entry point for exporting a Maya scene to Arnold
MStatus CMayaScene::ExportToArnold()
{
   MStatus status;

   // This export mode is used here, but also in the NodeTranslators.
   // For example, if it's IPR mode, they install callbacks to trigger
   // a refresh of the data sent to Arnold.
   // ExportMode exportMode   = GetExportMode();
   // CExportFilter* exportFilter = GetExportFilter();
   // It wouldn't be efficient to test the whole scene against selection state
   // so selected gets a special treatment
   ExportMode exportMode = m_exportOptions.m_mode;
   bool filterSelected = m_exportOptions.m_filter.unselected;
   
   PrepareExport();

   // Export Options - this needs to occur after PrepareExport is called
   AtNode* options = ExportNode(m_fnArnoldRenderOptions->object());
   SetupImageOptions(AtNode* options)

   // Are we motion blurred (any type)?
   const bool mb = IsMotionBlurEnabled();

   // In case of motion blur we need to position ourselves to first step
   // TODO : what if specific frame was requested and it's != GetCurrentFrame()
   if (mb)
   {
      // first step is the real export
      AiMsgDebug("[mtoa] Exporting step 0 at frame %f", m_motion_frames[0]);
      MGlobal::viewFrame(MTime(m_motion_frames[0], MTime::uiUnit()));
   }
   // First "real" export
   if (exportMode == MTOA_EXPORT_ALL || exportMode == MTOA_EXPORT_IPR)
   {
      if (m_exportOptions.m_camera.isValid())
      {
         m_exportOptions.m_camera.extendToShape();
         ExportDagPath(m_exportOptions.m_camera);
      }
      else
      {
         status = ExportCameras();
      }
      m_exportOptions.m_filter.excluded.insert(MFn::kCamera);
      if (filterSelected)
      {
         // And for render selected we need the lights too
         status = ExportLights();
         m_exportOptions.m_filter.excluded.insert(MFn::kLight);
         status = ExportSelected();
      }
      else
      {
         status = ExportScene();
      }
   }
   else if (exportMode == MTOA_EXPORT_FILE)
   {
      if (filterSelected)
      {
         // If we export selected to a file, not as a full render,
         // we just export as it is
         status = ExportSelected();
      }
      else
      {
         // Else if it's a full / renderable scene
         status = ExportCameras();
         // Then we filter them out to avoid double exporting them
         m_exportOptions.m_filter.excluded.insert(MFn::kCamera);
         status = ExportScene();
      }
   }
   else
   {
      AiMsgError("[mtoa] Unsupported export mode: %d", exportMode);
      return MStatus::kFailure;
   }

   // Then in case of motion blur do the other steps
   if (mb)
   {
      s_isExportingMotion = true;
      // loop through motion steps
      for (unsigned int step = 1; step < GetNumMotionSteps(); ++step)
      {
         MGlobal::viewFrame(MTime(m_motion_frames[step], MTime::uiUnit()));
         AiMsgDebug("[mtoa] Exporting step %d at frame %f", step, m_motion_frames[step]);
         // then, loop through the already processed dag translators and export for current step
         // NOTE: these exports are subject to the normal pre-processed checks which prevent redundant exports.
         // Since all nodes *should* be exported at this point, the following calls to DoExport do not
         // traverse the DG even if the translators call ExportShader or ExportDagPath. This makes it safe
         // to re-export all objects from a flattened list
         ObjectToDagTranslatorMap::iterator dagIt;
         for(dagIt = m_processedDagTranslators.begin(); dagIt != m_processedDagTranslators.end(); ++dagIt)
         {
            // finally, loop through instances
            std::map<int, CNodeTranslator*>::iterator instIt;
            for(instIt = dagIt->second.begin(); instIt != dagIt->second.end(); ++instIt)
            {
               instIt->second->DoExport(step);
            }
         }
         // finally, loop through the already processed depend translators and export for current step
         ObjectToTranslatorMap::iterator dependIt;
         for(dependIt = m_processedTranslators.begin(); dependIt != m_processedTranslators.end(); ++dependIt)
         {
            dependIt->second->DoExport(step);
         }
      }
      MGlobal::viewFrame(MTime(GetCurrentFrame(), MTime::uiUnit()));
      s_isExportingMotion = false;
   }

   return status;
}

void CMayaScene::SetupImageOptions(AtNode* options)
{
   const CRenderOptions* renderOptions = GetRenderSession()->RenderOptions();
   if (renderOptions->useRenderRegion())
   {
      AiNodeSetInt(options, "region_min_x", renderOptions->minX());
      AiNodeSetInt(options, "region_min_y", renderOptions->height() - renderOptions->maxY() - 1);
      AiNodeSetInt(options, "region_max_x", renderOptions->maxX());
      AiNodeSetInt(options, "region_max_y", renderOptions->height() - renderOptions->minY() - 1);
   }

   AiNodeSetInt(options, "xres", renderOptions->width());
   AiNodeSetInt(options, "yres", renderOptions->height());
   AiNodeSetFlt(options, "aspect_ratio", renderOptions->pixelAspectRatio());
}

// TODO : allow this to take an argument passed custom ArnoldRenderOptions (and renderGlobals or?)
void CMayaScene::PrepareExport()
{
   MSelectionList list;
   MObject        node;

   list.add("defaultRenderGlobals");
   if (list.length() > 0)
   {
      list.getDependNode(0, node);
      m_fnCommonRenderOptions = new MFnDependencyNode(node);
   }

   list.clear();

   list.add("defaultArnoldRenderOptions");
   if (list.length() > 0)
   {
      list.getDependNode(0, node);
      m_fnArnoldRenderOptions = new MFnDependencyNode(node);
   }

   m_exportOptions.m_frame = static_cast<float>(MAnimControl::currentTime().as(MTime::uiUnit()));

   GetMotionBlurData();
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
   s_exportSession->ClearUpdateCallbacks();
   
}

void CMayaScene::IPRNewNodeCallback(MObject & node, void *)
{
   // If this is a node we've exported before (e.g. user deletes then undos)
   // we can shortcut and just call the update for it's already existing translator.
   // Interupt rendering
   CRenderSession* renderSession = GetRenderSession();
   CExportSession* exportSession = GetExportSession();
   renderSession->InterruptRender();
   CNodeTranslator * translator = exportSession->GetActiveTranslator(node);
   if (translator != NULL)
   {
      exportSession->QueueForUpdate(translator);
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
         exportSession->Export(path);
         // exportSession->QueueForUpdate(); // add it?
      }
   }
   UpdateIPR();
}

void CMayaScene::UpdateIPR()
{
   // Add the IPR update callback, this is called in Maya's idle time (Arnold may not be idle, that's okay).
   if ( s_IPRIdleCallbackId == 0 && !IsExportingMotion() )
   {
      MStatus status;
      MCallbackId id = MEventMessage::addEventCallback("idle", IPRIdleCallback, NULL, &status);
      if (status == MS::kSuccess) s_IPRIdleCallbackId = id;
   }
}

void CMayaScene::IPRIdleCallback(void *)
{
   if (s_IPRIdleCallbackId != 0)
   {
      MMessage::removeCallback(s_IPRIdleCallbackId);
      s_IPRIdleCallbackId = 0;
   }

   CRenderSession* renderSession = GetRenderSession();
   CExportSession* exportSession = GetExportSession();

   renderSession->InterruptRender();
   exportSession->DoUpdate();
   renderSession->DoIPRRender();
}






