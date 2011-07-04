
#include "ExportSession.h"
#include "extension/ExtensionsManager.h"

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



CExportSession::~CExportSession()
{
   if (GetExportMode() == MTOA_EXPORT_IPR)
   {
      ClearUpdateCallbacks();
   }

   if (m_fnCommonRenderOptions != NULL) delete m_fnCommonRenderOptions;
   if (m_fnArnoldRenderOptions != NULL) delete m_fnArnoldRenderOptions;

   // Delete translators
   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
   {
      AiMsgDebug("[mtoa] Deleting translator for %s", MFnDependencyNode(it->first.object()).name().asChar());
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
         MFnDagNode fnDag(dagIt->first.object());
         AiMsgDebug("[mtoa] Deleting translator for %s [%d]", fnDag.fullPathName().asChar(), instIt->first);
         delete instIt->second;
      }
   }
   m_processedDagTranslators.clear();
}

/// Primary entry point for exporting a Maya scene to Arnold
MStatus CExportSession::ExportToArnold()
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
   ExportNode(m_fnArnoldRenderOptions->object());

   // Are we motion blurred (any type)?
   const bool mb = IsMotionBlurEnabled();

   // In case of motion blur we need to position ourselves to first step
   // TODO : what if specific frame was requested and it's != GetExportFrame()
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
         MSelectionList selected;
         MGlobal::getActiveSelectionList(selected);
         status = ExportSelection(selected);
      }
      else
      {
         status = ExportScene();
         // Add callbacks if we're in IPR mode.
         // if (s_NewNodeCallbackId == 0x0)
         // {
         //    s_NewNodeCallbackId = MDGMessage::addNodeAddedCallback(CMayaScene::IPRNewNodeCallback);
         // }
      }
   }
   else if (exportMode == MTOA_EXPORT_FILE)
   {
      if (filterSelected)
      {
         // If we export selected to a file, not as a full render,
         // we just export as it is
         MSelectionList selected;
         MGlobal::getActiveSelectionList(selected);
         status = ExportSelection(selected);
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
      m_isExportingMotion = true;
      // loop through motion steps
      for (unsigned int step = 1; step < GetNumMotionSteps(); ++step)
      {
         MGlobal::viewFrame(MTime(m_motion_frames[step], MTime::uiUnit()));
         AiMsgDebug("[mtoa] Exporting step %d at frame %f", step, m_motion_frames[step]);
         // then, loop through the already processed dag translators and export for current step
         // NOTE: these exports are subject to the normal pre-processed checks which prevent redundant exports.
         // Since all nodes *should* be exported at this point, the following calls to DoExport do not
         // traverse the DG even if the translators call ExportNode or ExportDag. This makes it safe
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
      MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));
      m_isExportingMotion = false;
   }

   return status;
}

// TODO : allow this to take an argument passed custom ArnoldRenderOptions (and renderGlobals or?)
void CExportSession::PrepareExport()
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

void CExportSession::GetMotionBlurData()
{
   if (m_fnArnoldRenderOptions->findPlug("mb_en").asBool())
   {
      m_exportOptions.m_motion.enable_mask   = m_fnArnoldRenderOptions->findPlug("mb_len").asBool() * MTOA_MBLUR_LIGHT
                                             + m_fnArnoldRenderOptions->findPlug("mb_cen").asBool() * MTOA_MBLUR_CAMERA
                                             + m_fnArnoldRenderOptions->findPlug("mb_oen").asBool() * MTOA_MBLUR_OBJECT
                                             + m_fnArnoldRenderOptions->findPlug("mb_den").asBool() * MTOA_MBLUR_DEFORM
                                             + m_fnArnoldRenderOptions->findPlug("mb_sen").asBool() * MTOA_MBLUR_SHADER;
   }
   else
   {
      m_exportOptions.m_motion.enable_mask     = MTOA_MBLUR_DISABLE;
   }
   if (m_exportOptions.m_motion.enable_mask)
   {
      m_exportOptions.m_motion.shutter_size    = m_fnArnoldRenderOptions->findPlug("shutter_size").asFloat();
      m_exportOptions.m_motion.shutter_offset  = m_fnArnoldRenderOptions->findPlug("shutter_offset").asFloat();
      m_exportOptions.m_motion.shutter_type    = m_fnArnoldRenderOptions->findPlug("shutter_type").asInt();
      m_exportOptions.m_motion.by_frame        = m_fnArnoldRenderOptions->findPlug("motion_frames").asFloat();
      m_exportOptions.m_motion.steps           = m_fnArnoldRenderOptions->findPlug("motion_steps").asInt();

      m_motion_frames.clear();
      m_motion_frames.reserve(m_exportOptions.m_motion.steps);
      for (AtUInt J=0; (J < m_exportOptions.m_motion.steps); ++J)
      {
         float frame = GetExportFrame() -
                       m_exportOptions.m_motion.by_frame * 0.5f +
                       m_exportOptions.m_motion.shutter_offset +
                       m_exportOptions.m_motion.by_frame / (m_exportOptions.m_motion.steps - 1) * J;

         m_motion_frames.push_back(frame);
      }
   }
   else
   {
      m_exportOptions.m_motion.by_frame        = 0;
      m_exportOptions.m_motion.steps           = 1;

      m_motion_frames.clear();
      m_motion_frames.push_back(GetExportFrame());
   }
}

// Export the cameras of the maya scene
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CExportSession::ExportCameras()
{
   MStatus status = MStatus::kSuccess;
   MDagPath path;
   MItDag   dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);

   // First we export all cameras
   // We do not reset the iterator to avoid getting kWorld
   for (; (!dagIterCameras.isDone()); dagIterCameras.next())
   {
      if (dagIterCameras.getPath(path))
      {
         // Only check for cameras being visible, not templated and in render layer
         // FIXME: does a camera need to be visible to render actually in Maya?
         /*
         MFnDagNode node(path.node());
         MString name = node.name();
         if (m_exportOptions.m_filter.notinlayer == true && !IsInRenderLayer(path))
            continue;
         if (m_exportOptions.m_filter.templated == true && IsTemplatedPath(path))
            continue;
         if (m_exportOptions.m_filter.hidden == true && !IsVisiblePath(path))
            continue;
         */
         if (ExportDagPath(path) == NULL)
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
MStatus CExportSession::ExportLights()
{
   MStatus status = MStatus::kSuccess;
   MDagPath path;
   MItDag   dagIterLights(MItDag::kDepthFirst, MFn::kLight);

   // First we export all cameras
   // We do not reset the iterator to avoid getting kWorld
   for (; (!dagIterLights.isDone()); dagIterLights.next())
   {
      if (dagIterLights.getPath(path))
      {
         // Only check for cameras being visible, not templated and in render layer
         // FIXME: does a light need to be in layer to render actually in Maya?
         MFnDagNode node(path.node());
         MString name = node.name();
         if (m_exportOptions.m_filter.notinlayer == true && !IsInRenderLayer(path))
            continue;
         if (m_exportOptions.m_filter.templated == true && IsTemplatedPath(path))
            continue;
         if (m_exportOptions.m_filter.hidden == true && !IsVisiblePath(path))
            continue;
         if (ExportDagPath(path) == NULL)
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
MStatus CExportSession::ExportScene()
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
         filtered = FilteredStatus(m_exportOptions.m_filter, path);
         if (filtered != MTOA_EXPORT_ACCEPTED)
         {
            // Ignore node for MTOA_EXPORT_REJECTED_NODE or whole branch
            // for MTOA_EXPORT_REJECTED_BRANCH
            if (filtered == MTOA_EXPORT_REJECTED_BRANCH)
               dagIterator.prune();
            continue;
         }
         if (ExportDagPath(path) == NULL)
            status = MStatus::kFailure;
      }
      else
      {
         AiMsgError("[mtoa] Could not get path for Maya DAG iterator.");
         status = MS::kFailure;
      }
   }
   
   return status;
}

// Get the selection from maya and export it with the IterSelection methode
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CExportSession::ExportSelection(MSelectionList& selected)
{
   MStatus status = MStatus::kSuccess;

   // Get a expanded, flattened, filtered list of every dag
   // paths we need to export
   status = IterSelection(selected);
   MItSelectionList it(selected, MFn::kInvalid, &status);
   MDagPath path;
   for (it.reset(); !it.isDone(); it.next())
   {
      if (it.getDagPath(path) == MStatus::kSuccess)
      {
         if (ExportDagPath(path) == NULL)
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
MStatus CExportSession::IterSelection(MSelectionList& selected)
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
         if (FilteredStatus(m_exportOptions.m_filter, path) == MTOA_EXPORT_ACCEPTED)
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
AtNode* CExportSession::ExportDagPath(MDagPath &dagPath)
{
   MObjectHandle handle = MObjectHandle(dagPath.node());
   int instanceNum = dagPath.instanceNumber();
   MString name = dagPath.partialPathName();
   MString type = MFnDagNode(dagPath).typeName();
   AiMsgDebug("[mtoa] Exporting dag node %s of type %s", name.asChar(), type.asChar());
   // early out for nodes that have already been processed
   ObjectToDagTranslatorMap::iterator it = m_processedDagTranslators.find(handle);
   if (it != m_processedDagTranslators.end() && it->second.count(instanceNum))
      return it->second[instanceNum]->GetArnoldRootNode();
   // else get a new translator for that node
   CDagTranslator* translator = CExtensionsManager::GetTranslator(dagPath);
   if (translator != NULL && translator->IsDag())
   {
      if (translator->IsRenderable())
      {
         AtNode* result = translator->Init(this, dagPath);
         translator->DoExport(0);
         // save it for later
         m_processedDagTranslators[handle][instanceNum] = translator;
         return result;
      }
   }
   else
   {
      AiMsgDebug("[mtoa] Dag node %s of type %s ignored", name.asChar(), type.asChar());
   }
   return NULL;
}

// Export a plug (dependency node output attribute)
//
AtNode* CExportSession::ExportNode(MPlug& shaderOutputPlug)
{
   return ExportNode(shaderOutputPlug.node(), shaderOutputPlug.partialName(false, false, false, false, false, true));
}

AtNode* CExportSession::ExportNode(MObject mayaNode, const MString &attrName)
{
   MDagPath dagPath;
   if (MDagPath::getAPathTo(mayaNode, dagPath) == MS::kSuccess)
      return ExportDagPath(dagPath);

   // First check if this node has already been processed
   MObjectHandle handle = MObjectHandle(mayaNode);
   // early out for depend nodes that have already been processed
   ObjectToTranslatorMap::iterator it = m_processedTranslators.find(handle);
   if (it != m_processedTranslators.end() && it->second->m_outputAttr == attrName)
      return it->second->GetArnoldRootNode();

   AtNode* arnoldNode = NULL;
   // else get a new translator for that node
   CNodeTranslator* translator = CExtensionsManager::GetTranslator(mayaNode);
   if (translator != NULL)
   {
      arnoldNode = translator->Init(this, mayaNode, attrName);
      m_processedTranslators[handle] = translator;
      translator->DoExport(0);
   }
   else
   {
      AiMsgDebug("[mtoa] Maya node type not supported: %s", MFnDependencyNode(mayaNode).typeName().asChar());
   }

   return arnoldNode;
}


CNodeTranslator * CExportSession::GetActiveTranslator(const MObject node)
{
   MObjectHandle node_handle(node);

   ObjectToTranslatorMap::iterator translatorIt = m_processedTranslators.find(node_handle);
   if (translatorIt != m_processedTranslators.end())
   {
      return static_cast< CNodeTranslator* >(translatorIt->second);
   }

   ObjectToDagTranslatorMap::iterator dagIt = m_processedDagTranslators.find(node_handle);
   if (dagIt != m_processedDagTranslators.end())
   {
      // TODO: Figure out some magic to get the correct instance.
      const int instanceNum = 0;
      return static_cast< CNodeTranslator* >(dagIt->second[instanceNum]);
   }

   return NULL;
}


// updates

void CExportSession::DoUpdate()
{
   // Are we motion blurred?
   const bool mb = IsMotionBlurEnabled();
   if (!mb)
   {
      for(std::vector<CNodeTranslator*>::iterator iter = m_translatorsToUpdate.begin();
         iter != m_translatorsToUpdate.end(); ++iter)
      {
         CNodeTranslator* translator = (*iter);
         if (translator != NULL) translator->DoUpdate(0);
      }
   }
   else
   {
      m_isExportingMotion = true;
      // Scene is motion blured, get the data for the steps.
      for (unsigned int step = 0; (step < GetNumMotionSteps()); ++step)
      {
         MGlobal::viewFrame(MTime(m_motion_frames[step], MTime::uiUnit()));
         for(std::vector<CNodeTranslator*>::iterator iter = m_translatorsToUpdate.begin();
            iter != m_translatorsToUpdate.end(); ++iter)
         {
            CNodeTranslator* translator = (*iter);
            if (translator != NULL)translator->DoUpdate(step);
         }
      }
      MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));
      m_isExportingMotion = false;
   }

   // Clear the list.
   m_translatorsToUpdate.clear();
   m_requestUpdate = false;
}

void CExportSession::ClearUpdateCallbacks()
{
   // Clear the list of translators to update.
   m_translatorsToUpdate.clear();

   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
   {
      if (it->second != NULL) it->second->RemoveUpdateCallbacks();
   }

   ObjectToDagTranslatorMap::iterator dagIt;
   for(dagIt = m_processedDagTranslators.begin(); dagIt != m_processedDagTranslators.end(); ++dagIt)
   {
      std::map<int, CNodeTranslator*>::iterator instIt;
      for(instIt = dagIt->second.begin(); instIt != dagIt->second.end(); ++instIt)
      {
         instIt->second->RemoveUpdateCallbacks();
      }
   }
   
}
