
#include "MayaScene.h"
#include "NodeTranslator.h"
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

std::map<int, CreatorFunction>  CMayaScene::s_dagTranslators;
std::map<int, CreatorFunction>  CMayaScene::s_dependTranslators;
std::vector< CNodeTranslator * > CMayaScene::s_translatorsToIPRUpdate;
MCallbackId CMayaScene::s_IPRIdleCallbackId = 0;
MCallbackId CMayaScene::s_NewNodeCallbackId = 0;

CMayaScene::~CMayaScene()
{
   if ( GetExportMode() == MTOA_EXPORT_IPR )
   {
      ClearIPRCallbacks();
   }

   if ( m_fnCommonRenderOptions != 0x0 ) delete m_fnCommonRenderOptions;
   if ( m_fnArnoldRenderOptions != 0x0 ) delete m_fnArnoldRenderOptions;

   // Delete translators
   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); it++)
   {
      delete it->second;
   }

   m_processedTranslators.clear();
}

MStatus CMayaScene::ExportToArnold(ExportMode exportMode)
{
   // This export mode is used here, but also in the NodeTranslators.
   // For example, if it's IPR mode, they install callbacks to trigger
   // a refresh of the data sent to Arnold.
   m_exportMode = exportMode;
   
   MStatus status;

   PrepareExport();
   
   // Are we motion blurred?
   const bool mb = m_motionBlurData.enabled &&
                   ( m_fnArnoldRenderOptions->findPlug("mb_camera_enable").asBool()    ||
                     m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool()   ||
                     m_fnArnoldRenderOptions->findPlug("mb_lights_enable").asBool()    );

   if (exportMode == MTOA_EXPORT_ALL)
   {
      if (!mb)
      {
         status = ExportScene(0);
      }
      else
      {
         // Scene is motion blured, get the data for the steps.
         for (int J = 0; (J < m_motionBlurData.motion_steps); ++J)
         {
            MGlobal::viewFrame(MTime(m_motionBlurData.frames[J], MTime::uiUnit()));
            status = ExportScene(J);
         }
         MGlobal::viewFrame(MTime(GetCurrentFrame(), MTime::uiUnit()));
      }
   }
   else if ( exportMode == MTOA_EXPORT_IPR )
   {
      if (!mb)
      {
         status = ExportForIPR(0);
      }
      else
      {
         // Scene is motion blured, get the data for the steps.
         for (int J = 0; (J < m_motionBlurData.motion_steps); ++J)
         {
            MGlobal::viewFrame(MTime(m_motionBlurData.frames[J], MTime::uiUnit()));
            status = ExportForIPR(J);
         }
         MGlobal::viewFrame(MTime(GetCurrentFrame(), MTime::uiUnit()));
      }
   }
   else if ( exportMode == MTOA_EXPORT_SELECTED )
   {
      status = ExportSelected();
   }
   else
   {
      AiMsgDebug( "[mtoa] unsupported export mode: %d", exportMode );
   }

   return status;
}

// Loop and export the selection, and all its hirarchy down stream
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::IterSelection(MSelectionList selected)
{
   MStatus status;
   MItSelectionList it(selected, MFn::kInvalid, &status);

   // loop users selection
   for (it.reset(); !it.isDone(); it.next())
   {
      MDagPath path;
      it.getDagPath(path);

      // iterate Hierarchy
      if (IsVisible(path.node()) || !IsTemplated(path.node()))
      {
         for (AtUInt child = 0; (child < path.childCount()); child++)
         {
            MObject ChildObject = path.child(child);
            path.push(ChildObject);

            selected.clear();
            selected.add(path.fullPathName());

            MFnDagNode node(path.node());
            if (!node.isIntermediateObject())
            {
               ExportDagPath(path, 0);
            }
            path.pop(1);
            IterSelection(selected);
         }
      }
   }
   return status;
}

// Get the selection from maya and export it with the IterSelection methode
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::ExportSelected()
{
   MStatus status;

   MSelectionList selected;
   MGlobal::getActiveSelectionList(selected);

   IterSelection(selected);

   selected.clear();

   return status;
}

bool CMayaScene::ExportDagPath(MDagPath &dagPath, AtUInt step)
{
   MFnDagNode node(dagPath.node());
   if (step == 0)
   {
      std::map<int, CreatorFunction>::iterator translatorIt;
      translatorIt = s_dagTranslators.find(node.typeId().id());
      if (translatorIt != s_dagTranslators.end())
      {
         CDagTranslator* translator;
         translator = (CDagTranslator*)translatorIt->second();
         translator->Init(dagPath, this);
         translator->DoExport(step);
         m_processedTranslators[MObjectHandle(dagPath.node())] = translator;
         return true;
      }
   }
   else
   {
      CDagTranslator* translator = (CDagTranslator*)m_processedTranslators[MObjectHandle(dagPath.node())];
      if (translator != NULL)
         translator->DoExport(step);
   }
   return false;
}

// Export the maya scene
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::ExportScene(AtUInt step)
{
   MStatus  status;
   MDagPath dagPath;
   MItDag   dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);

   AiMsgDebug("[mtoa] exporting step %d at frame %f", step, static_cast<float>(MAnimControl::currentTime().as(MTime::uiUnit())));

   // First we export all cameras
   // We do not reset the iterator to avoid getting kWorld
   for (; (!dagIterCameras.isDone()); dagIterCameras.next())
   {
      if (!dagIterCameras.getPath(dagPath))
      {
         AiMsgError("[mtoa] ERROR: Could not get path for DAG iterator.");
         return status;
      }
      ExportDagPath(dagPath, step);
   }

   // And now we export the rest of the DAG
   MItDag   dagIterator(MItDag::kDepthFirst, MFn::kInvalid);
   for (dagIterator.reset(); (!dagIterator.isDone()); dagIterator.next())
   {
      if (!dagIterator.getPath(dagPath))
      {
         AiMsgError("[mtoa] ERROR: Could not get path for DAG iterator.");
         return status;
      }

      if (dagPath.apiType() == MFn::kWorld || dagPath.node().hasFn(MFn::kCamera))
         continue;

      MFnDagNode node(dagPath.node());

      if (!IsVisible(node) || IsTemplated(node))
      {
         dagIterator.prune();
         continue;
      }

      ExportDagPath(dagPath, step);
   }
   return MS::kSuccess;
}

MStatus CMayaScene::ExportForIPR(AtUInt step )
{
   if ( s_NewNodeCallbackId == 0x0 )
   {
      s_NewNodeCallbackId = MDGMessage::addNodeAddedCallback( CMayaScene::IPRNewNodeCallback );
   }

   return ExportScene( step );
}


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

   m_currentFrame = static_cast<float>(MAnimControl::currentTime().as(MTime::uiUnit()));

   GetMotionBlurData();
}

void CMayaScene::GetMotionBlurData()
{
   m_motionBlurData.enabled        = m_fnArnoldRenderOptions->findPlug("motion_blur_enable").asBool();
   m_motionBlurData.shutter_size   = m_fnArnoldRenderOptions->findPlug("shutter_size").asFloat();
   m_motionBlurData.shutter_offset = m_fnArnoldRenderOptions->findPlug("shutter_offset").asFloat();
   m_motionBlurData.shutter_type   = m_fnArnoldRenderOptions->findPlug("shutter_type").asInt();
   m_motionBlurData.motion_steps   = m_fnArnoldRenderOptions->findPlug("motion_steps").asInt();
   m_motionBlurData.motion_frames  = m_fnArnoldRenderOptions->findPlug("motion_frames").asFloat();

   if (m_motionBlurData.enabled)
   {
      for (int J = 0; (J < m_motionBlurData.motion_steps); ++J)
      {
         float frame = GetCurrentFrame() -
                       m_motionBlurData.motion_frames * 0.5f +
                       m_motionBlurData.shutter_offset +
                       m_motionBlurData.motion_frames / (m_motionBlurData.motion_steps - 1) * J;

         m_motionBlurData.frames.push_back(frame);
      }
   }
}

void CMayaScene::RegisterDagTranslator(int typeId, CreatorFunction creator)
{
   s_dagTranslators[typeId] = creator;
}

void CMayaScene::RegisterTranslator(int typeId, CreatorFunction creator)
{
   s_dependTranslators[typeId] = creator;
}

CNodeTranslator * CMayaScene::GetActiveTranslator( const MObject node )
{
   MObjectHandle node_handle( node );

   ObjectToTranslatorMap::iterator translatorIt = m_processedTranslators.find( node_handle );
   if ( translatorIt != m_processedTranslators.end() )
   {
      return static_cast< CNodeTranslator* >( translatorIt->second );
   }

   return 0x0;
}

void CMayaScene::ClearIPRCallbacks()
{
   // Clear the list of stuff to update.
   s_translatorsToIPRUpdate.clear();

   if ( s_IPRIdleCallbackId != 0 )
   {
      MMessage::removeCallback( s_IPRIdleCallbackId );
      s_IPRIdleCallbackId = 0;
   }

   if ( s_NewNodeCallbackId != 0 )
   {
      MMessage::removeCallback( s_NewNodeCallbackId );
      s_NewNodeCallbackId = 0;
   }


   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); it++)
   {
      if ( it->second != 0x0 ) it->second->RemoveCallbacks();
   }
}

void CMayaScene::IPRNewNodeCallback(MObject & node, void *)
{
   // If this is a node we've exported before (e.g. user deletes then undos)
   // we can shortcut and just call the update for it's already existing translator.
   CRenderSession* renderSession = CRenderSession::GetInstance();
   // Interupt rendering
   renderSession->InterruptRender();
   CNodeTranslator * translator = renderSession->GetMayaScene()->GetActiveTranslator(node);
   if ( translator != 0x0 )
   {
      renderSession->GetMayaScene()->UpdateIPR( translator );
      return;
   }

   // Then export this node as it's completely new to us.
   MFnDagNode dag_node(node);
   MDagPath path;
   const MStatus status = dag_node.getPath(path);
   if (status == MS::kSuccess)
   {
      AiMsgDebug( "[mtoa] Exporting new node: %s", path.partialPathName().asChar() );
      renderSession->GetMayaScene()->ExportDagPath(path, 0);
      renderSession->GetMayaScene()->UpdateIPR();
   }
}

void CMayaScene::IPRIdleCallback(void *)
{
   if ( s_IPRIdleCallbackId != 0 )
   {
      MMessage::removeCallback( s_IPRIdleCallbackId );
      s_IPRIdleCallbackId = 0;
   }

   CRenderSession* renderSession = CRenderSession::GetInstance();
   renderSession->InterruptRender();
   CMayaScene* scene = renderSession->GetMayaScene();
   // Are we motion blurred?
   const bool mb = scene->m_motionBlurData.enabled &&
                   ( scene->m_fnArnoldRenderOptions->findPlug("mb_camera_enable").asBool()    ||
                     scene->m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool()   ||
                     scene->m_fnArnoldRenderOptions->findPlug("mb_lights_enable").asBool()    );

   if (!mb)
   {
      for( std::vector<CNodeTranslator*>::iterator iter=s_translatorsToIPRUpdate.begin();
         iter != s_translatorsToIPRUpdate.end(); ++iter)
      {
         CNodeTranslator* translator = (*iter);
         if ( translator != 0x0 ) translator->DoUpdate(0);
      }
   }
   else
   {
      // Scene is motion blured, get the data for the steps.
      for (int J = 0; (J < scene->m_motionBlurData.motion_steps); ++J)
      {
         MGlobal::viewFrame(MTime(scene->m_motionBlurData.frames[J], MTime::uiUnit()));
         for( std::vector<CNodeTranslator*>::iterator iter=s_translatorsToIPRUpdate.begin();
            iter != s_translatorsToIPRUpdate.end(); ++iter)
         {
            CNodeTranslator* translator = (*iter);
            if ( translator != 0x0 )translator->DoUpdate(J);
         }
      }
      MGlobal::viewFrame(MTime(scene->GetCurrentFrame(), MTime::uiUnit()));
   }

   // Clear the list.
   s_translatorsToIPRUpdate.clear();

   renderSession->DoIPRRender();
}

void CMayaScene::UpdateIPR( CNodeTranslator * translator )
{
   if ( translator != 0x0 )
   {
      s_translatorsToIPRUpdate.push_back( translator );
   }

   // Add the IPR update callback, this is called in Maya's
   // idle time (Arnold may not be idle, that's okay).
   if ( s_IPRIdleCallbackId == 0 )
   {
      MStatus status;
      MCallbackId id = MEventMessage::addEventCallback( "idle", IPRIdleCallback, NULL, &status );
      if ( status == MS::kSuccess ) s_IPRIdleCallbackId = id;
   }
}

