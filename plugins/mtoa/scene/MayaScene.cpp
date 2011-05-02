
#include "MayaScene.h"
#include "TranslatorRegistry.h"
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

std::vector< CNodeTranslator * > CMayaScene::s_translatorsToIPRUpdate;
MCallbackId CMayaScene::s_IPRIdleCallbackId = 0;
MCallbackId CMayaScene::s_NewNodeCallbackId = 0;

CMayaScene::~CMayaScene()
{
   if (GetExportMode() == MTOA_EXPORT_IPR)
   {
      ClearIPRCallbacks();
   }

   if (m_fnCommonRenderOptions != NULL) delete m_fnCommonRenderOptions;
   if (m_fnArnoldRenderOptions != NULL) delete m_fnArnoldRenderOptions;

   // Delete translators
   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
   {
      delete it->second;
   }
   m_processedTranslators.clear();

   // Delete Dag Translators
   ObjectToDagTranslatorMap::iterator dagIt;
   for(dagIt = m_processedDagTranslators.begin(); dagIt != m_processedDagTranslators.end(); ++dagIt)
   {
      std::map<int, CNodeTranslator*>::iterator instIt;
      for(instIt = dagIt->second.begin(); instIt != dagIt->second.end(); ++instIt)
      {
         delete instIt->second;
      }
   }
   m_processedDagTranslators.clear();
}

MStatus CMayaScene::ExportToArnold()
{
   MStatus status;

   // This export mode is used here, but also in the NodeTranslators.
   // For example, if it's IPR mode, they install callbacks to trigger
   // a refresh of the data sent to Arnold.
   // ExportMode exportMode   = GetExportMode();
   // ExportFilter exportFilter = GetExportFilter();
   // It wouldn't be efficient to test the whole scene against selection state
   // so selected gets a special treatment
   ExportMode exportMode = m_exportOptions.mode;
   bool filterSelected = m_exportOptions.filter.unselected;
   
   PrepareExport();

   // Export Options - this needs to occur after PrepareExport is called
   ExportShader(m_fnArnoldRenderOptions->object());

   // Are we motion blurred?
   const bool mb = m_motionBlurData.enabled &&
                   (m_fnArnoldRenderOptions->findPlug("mb_camera_enable").asBool()    ||
                     m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool()   ||
                     m_fnArnoldRenderOptions->findPlug("mb_lights_enable").asBool());

   // In case of motion blur we need to position ourselves to first step
   // TODO : what if specific frame was requested and it's != GetCurrentFrame()
   if (mb)
   {
      // first step is the real export
      AiMsgDebug("[mtoa] Exporting step 0 at frame %f", m_motionBlurData.frames[0]);
      MGlobal::viewFrame(MTime(m_motionBlurData.frames[0], MTime::uiUnit()));
   }
   // First "real" export
   if (exportMode == MTOA_EXPORT_ALL || exportMode == MTOA_EXPORT_IPR)
   {
      // Cameras are always exported currently
      status = ExportCameras();
      // Then we filter them out to avoid double exporting them
      m_exportOptions.filter.excluded.insert(MFn::kCamera);
      if (filterSelected)
      {
         // And for render selected we need the lights too
         status = ExportLights();
         m_exportOptions.filter.excluded.insert(MFn::kLight);
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
         m_exportOptions.filter.excluded.insert(MFn::kCamera);
         status = ExportScene();
      }
   }
   else
   {
      AiMsgDebug("[mtoa] Unsupported export mode: %d", exportMode);
      return MStatus::kFailure;
   }

   // Then in case of motion blur do the other steps
   if (mb)
   {
      // loop through motion steps
      for (AtUInt step = 1; step < m_motionBlurData.motion_steps; ++step)
      {
         MGlobal::viewFrame(MTime(m_motionBlurData.frames[step], MTime::uiUnit()));
         AiMsgDebug("[mtoa] Exporting step %d at frame %f", step, m_motionBlurData.frames[step]);
         // then, loop through the already processed dag translators and export for current step
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
      }
      MGlobal::viewFrame(MTime(GetCurrentFrame(), MTime::uiUnit()));
   }

   return status;
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

   m_currentFrame = static_cast<float>(MAnimControl::currentTime().as(MTime::uiUnit()));


   GetMotionBlurData();
}

void CMayaScene::GetMotionBlurData()
{
   m_motionBlurData.enabled        = m_fnArnoldRenderOptions->findPlug("motion_blur_enable").asBool();
   m_motionBlurData.shutter_size   = m_fnArnoldRenderOptions->findPlug("shutter_size").asFloat();
   m_motionBlurData.shutter_offset = m_fnArnoldRenderOptions->findPlug("shutter_offset").asFloat();
   m_motionBlurData.shutter_type   = m_fnArnoldRenderOptions->findPlug("shutter_type").asInt();
   m_motionBlurData.motion_frames  = m_fnArnoldRenderOptions->findPlug("motion_frames").asFloat();

   if (m_motionBlurData.enabled)
   {
      m_motionBlurData.motion_steps   = m_fnArnoldRenderOptions->findPlug("motion_steps").asInt();
      for (AtUInt J = 0; (J < m_motionBlurData.motion_steps); ++J)
      {
         float frame = GetCurrentFrame() -
                       m_motionBlurData.motion_frames * 0.5f +
                       m_motionBlurData.shutter_offset +
                       m_motionBlurData.motion_frames / (m_motionBlurData.motion_steps - 1) * J;

         m_motionBlurData.frames.push_back(frame);
      }
   }
   else
      m_motionBlurData.motion_steps   = 1;
}

// Export the cameras of the maya scene
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::ExportCameras()
{
   MStatus status = MStatus::kSuccess;
   MDagPath path;
   MItDag   dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);

   // First we export all cameras
   // We do not reset the iterator to avoid getting kWorld
   ExportFilter filter = GetExportFilter();
   for (; (!dagIterCameras.isDone()); dagIterCameras.next())
   {
      if (dagIterCameras.getPath(path))
      {
         // Only check for cameras being visible, not templated and in render layer
         // FIXME: does a camera need to be visible to render actually in Maya?
         /*
         MFnDagNode node(path.node());
         MString name = node.name();
         if (filter.notinlayer == true && !IsInRenderLayer(path))
            continue;
         if (filter.templated == true && IsTemplatedPath(path))
            continue;
         if (filter.hidden == true && !IsVisiblePath(path))
            continue;
         */
         if (MStatus::kSuccess != ExportDagPath(path))
            status = MStatus::kFailure;
      }
      else
      {
         AiMsgError("[mtoa] Could not get path for Maya cameras DAG iterator.");
         status = MS::kFailure;
      }
   }

   return status;
}

// Export the lights of the maya scene
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::ExportLights()
{
   MStatus status = MStatus::kSuccess;
   MDagPath path;
   MItDag   dagIterLights(MItDag::kDepthFirst, MFn::kLight);

   // First we export all cameras
   // We do not reset the iterator to avoid getting kWorld
   ExportFilter filter = GetExportFilter();
   for (; (!dagIterLights.isDone()); dagIterLights.next())
   {
      if (dagIterLights.getPath(path))
      {
         // Only check for cameras being visible, not templated and in render layer
         // FIXME: does a light need to be in layer to render actually in Maya?
         MFnDagNode node(path.node());
         MString name = node.name();
         if (filter.notinlayer == true && !IsInRenderLayer(path))
            continue;
         if (filter.templated == true && IsTemplatedPath(path))
            continue;
         if (filter.hidden == true && !IsVisiblePath(path))
            continue;
         if (MStatus::kSuccess != ExportDagPath(path))
            status = MStatus::kFailure;
      }
      else
      {
         AiMsgError("[mtoa] Could not get path for Maya lights DAG iterator.");
         status = MS::kFailure;
      }
   }

   return status;
}

// Export the maya scene
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::ExportScene()
{
   MStatus status = MStatus::kSuccess;
   MDagPath path;

   DagFiltered filtered;
   MItDag   dagIterator(MItDag::kDepthFirst, MFn::kInvalid);
   for (dagIterator.reset(); (!dagIterator.isDone()); dagIterator.next())
   {
      if (dagIterator.getPath(path))
      {
         if (path.apiType() == MFn::kWorld)
            continue;
         MObject obj = path.node();
         MFnDagNode node(obj);
         MString name = node.name();
         filtered = FilteredStatus(GetExportFilter(), path);
         if (filtered != MTOA_EXPORT_ACCEPTED)
         {
            // Ignore node for MTOA_EXPORT_REJECTED_NODE or whole branch
            // for MTOA_EXPORT_REJECTED_BRANCH
            if (filtered == MTOA_EXPORT_REJECTED_BRANCH)
               dagIterator.prune();
            continue;
         }
         if (MStatus::kSuccess != ExportDagPath(path))
            status = MStatus::kFailure;
      }
      else
      {
         AiMsgError("[mtoa] Could not get path for Maya DAG iterator.");
         status = MS::kFailure;
      }
   }
   
   // Add callbacks if we're in IPR mode.
   if (GetExportMode() == MTOA_EXPORT_IPR && s_NewNodeCallbackId == 0x0)
   {
      s_NewNodeCallbackId = MDGMessage::addNodeAddedCallback(CMayaScene::IPRNewNodeCallback);
   }
   
   return status;
}

// Get the selection from maya and export it with the IterSelection methode
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::ExportSelected()
{
   MStatus status = MStatus::kSuccess;

   MSelectionList selected;
   MGlobal::getActiveSelectionList(selected);

   // Get a expanded, flattened, filtered list of every dag
   // paths we need to export
   status = IterSelection(selected);
   MItSelectionList it(selected, MFn::kInvalid, &status);
   MDagPath path;
   for (it.reset(); !it.isDone(); it.next())
   {
      if (it.getDagPath(path) == MStatus::kSuccess)
      {
         if (MStatus::kSuccess != ExportDagPath(path))
            status = MStatus::kFailure;
      }
      else
      {
         status = MStatus::kFailure;
      }
   }
   selected.clear();

   return status;
}

// Loop and export the selection, and all its hirarchy down stream
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::IterSelection(MSelectionList& selected)
{
   MStatus status;

   MObject node;
   MDagPath path;
   MFnDagNode dgNode;
   MFnSet set;
   MSelectionList children;
   // loop users selection
   MItSelectionList it(selected, MFn::kInvalid, &status);
   selected.clear();
   for (it.reset(); !it.isDone(); it.next())
   {
      if (it.getDagPath(path) == MStatus::kSuccess)
      {
         // FIXME: if we selected a shape, and it's an instance,
         // should we export all its dag paths?
         if (FilteredStatus(GetExportFilter(), path) == MTOA_EXPORT_ACCEPTED)
         {
            for (AtUInt child = 0; (child < path.childCount()); child++)
            {
               MObject ChildObject = path.child(child);
               path.push(ChildObject);
               children.clear();
               children.add(path.fullPathName());
               dgNode.setObject(path.node());
               if (!dgNode.isIntermediateObject())
                  selected.add (path, MObject::kNullObj, true);
               path.pop(1);
               if (MStatus::kSuccess != IterSelection(children))
                  status = MStatus::kFailure;
               selected.merge(children);
            }
         }
      }
      else if (MStatus::kSuccess == it.getDependNode(node))
      {
         // Got a dependency (not dag) node
         if (node.hasFn(MFn::kSet))
         {
            // if it's a set we actually iterate on its content
            set.setObject(node);
            children.clear();
            // get set members, we don't set flatten to true in case we'd want a
            // test on each set recursively
            set.getMembers(children, false);
            if (MStatus::kSuccess != IterSelection(children))
               status = MStatus::kFailure;
            selected.merge(children);
         }
         else
         {
            // TODO: if it's a node we don't support / export should we set status to failure
            // or just raise a warning?
         }
      }
      else
      {
         status = MStatus::kFailure;
      }
   }

   return status;
}

// Export a single dag path (a dag node or an instance of a dag node)
// Considered to be already filtered and checked
MStatus CMayaScene::ExportDagPath(MDagPath &dagPath)
{
   MFnDagNode node(dagPath.node());
   MObjectHandle handle = MObjectHandle(dagPath.node());
   int instanceNum = dagPath.instanceNumber();
   // early out for nodes that have already been processed
   if (m_processedDagTranslators[handle].count(instanceNum))
      return MStatus::kSuccess;
   CDagTranslator* translator = CTranslatorRegistry::GetDagTranslator(node.typeId().id());
   if (translator != NULL)
   {
      translator->Init(dagPath, this);
      translator->DoExport(0);
      // save it for later
      m_processedDagTranslators[handle][instanceNum] = translator;
      return MStatus::kSuccess;
   }
   return MStatus::kFailure;
}

// Export a shader (dependency node)
//
// TODO: export motion blur for shaders
AtNode* CMayaScene::ExportShader(MPlug& shaderOutputPlug)
{
   return ExportShader(shaderOutputPlug.node(), shaderOutputPlug.partialName(false, false, false, false, false, true));
}

AtNode* CMayaScene::ExportShader(MObject mayaShader, const MString &attrName)
{
   // First check if this shader has already been processed
   for (std::vector<CShaderData>::const_iterator it = m_processedShaders.begin(); (it != m_processedShaders.end()); ++it)
   {
      if (it->mayaShader == mayaShader && it->attrName == attrName)
      {
         return it->arnoldShader;
      }
   }

   AtNode* shader = NULL;

   MFnDependencyNode node(mayaShader);

   CNodeTranslator* translator = CTranslatorRegistry::GetDependTranslator(node.typeId().id());
   if (translator != NULL)
   {
      if (mayaShader.hasFn(MFn::kDagNode))
      {
         CDagTranslator* dagTranslator = (CDagTranslator*)translator;
         MDagPath dagPath;
         MDagPath::getAPathTo(mayaShader, dagPath);
         dagTranslator->Init(dagPath, this, attrName);
         // FIXME: currently shaders are only exported for step = 0
         shader = dagTranslator->DoExport(0);
         m_processedTranslators[MObjectHandle(mayaShader)] = dagTranslator;
      }
      else
      {
         translator->Init(mayaShader, this, attrName);
         // FIXME: currently shaders are only exported for step = 0
         shader = translator->DoExport(0);
         m_processedTranslators[MObjectHandle(mayaShader)] = translator;
      }
   }
   else
      AiMsgWarning("[mtoa] Shader type not supported: %s", node.typeName().asChar());

   if (shader)
   {
      CShaderData   data;
      data.mayaShader   = mayaShader;
      data.arnoldShader = shader;
      data.attrName     = attrName;
      m_processedShaders.push_back(data);
   }
   return shader;
}

CNodeTranslator * CMayaScene::GetActiveTranslator(const MObject node)
{
   MObjectHandle node_handle(node);

   ObjectToTranslatorMap::iterator translatorIt = m_processedTranslators.find(node_handle);
   if (translatorIt != m_processedTranslators.end())
   {
      return static_cast< CNodeTranslator* >(translatorIt->second);
   }

   return NULL;
}

void CMayaScene::ClearIPRCallbacks()
{
   // Clear the list of stuff to update.
   s_translatorsToIPRUpdate.clear();

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


   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
   {
      if (it->second != NULL) it->second->RemoveIPRCallbacks();
   }

   ObjectToDagTranslatorMap::iterator dagIt;
   for(dagIt = m_processedDagTranslators.begin(); dagIt != m_processedDagTranslators.end(); ++dagIt)
   {
      std::map<int, CNodeTranslator*>::iterator instIt;
      for(instIt = dagIt->second.begin(); instIt != dagIt->second.end(); ++instIt)
      {
         instIt->second->RemoveIPRCallbacks();
      }
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
   if (translator != NULL)
   {
      renderSession->GetMayaScene()->UpdateIPR(translator);
      return;
   }

   // Then export this node as it's completely new to us.
   MFnDagNode dag_node(node);
   MDagPath path;
   const MStatus status = dag_node.getPath(path);
   if (status == MS::kSuccess)
   {
      AiMsgDebug("[mtoa] Exporting new node: %s", path.partialPathName().asChar());
      renderSession->GetMayaScene()->ExportDagPath(path);
      renderSession->GetMayaScene()->UpdateIPR();
   }
}

void CMayaScene::IPRIdleCallback(void *)
{
   if (s_IPRIdleCallbackId != 0)
   {
      MMessage::removeCallback(s_IPRIdleCallbackId);
      s_IPRIdleCallbackId = 0;
   }

   CRenderSession* renderSession = CRenderSession::GetInstance();
   renderSession->InterruptRender();
   CMayaScene* scene = renderSession->GetMayaScene();
   // Are we motion blurred?
   const bool mb = scene->m_motionBlurData.enabled &&
                   (scene->m_fnArnoldRenderOptions->findPlug("mb_camera_enable").asBool()    ||
                     scene->m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool()   ||
                     scene->m_fnArnoldRenderOptions->findPlug("mb_lights_enable").asBool());

   if (!mb)
   {
      for(std::vector<CNodeTranslator*>::iterator iter=s_translatorsToIPRUpdate.begin();
         iter != s_translatorsToIPRUpdate.end(); ++iter)
      {
         CNodeTranslator* translator = (*iter);
         if (translator != NULL) translator->DoUpdate(0);
      }
   }
   else
   {
      // Scene is motion blured, get the data for the steps.
      for (AtUInt J = 0; (J < scene->m_motionBlurData.motion_steps); ++J)
      {
         MGlobal::viewFrame(MTime(scene->m_motionBlurData.frames[J], MTime::uiUnit()));
         for(std::vector<CNodeTranslator*>::iterator iter=s_translatorsToIPRUpdate.begin();
            iter != s_translatorsToIPRUpdate.end(); ++iter)
         {
            CNodeTranslator* translator = (*iter);
            if (translator != NULL)translator->DoUpdate(J);
         }
      }
      MGlobal::viewFrame(MTime(scene->GetCurrentFrame(), MTime::uiUnit()));
   }

   // Clear the list.
   s_translatorsToIPRUpdate.clear();

   renderSession->DoIPRRender();
}

void CMayaScene::UpdateIPR(CNodeTranslator * translator)
{
   if (translator != NULL)
   {
      s_translatorsToIPRUpdate.push_back(translator);
   }

   // Add the IPR update callback, this is called in Maya's
   // idle time (Arnold may not be idle, that's okay).
   if (s_IPRIdleCallbackId == 0)
   {
      MStatus status;
      MCallbackId id = MEventMessage::addEventCallback("idle", IPRIdleCallback, NULL, &status);
      if (status == MS::kSuccess) s_IPRIdleCallbackId = id;
   }
}

