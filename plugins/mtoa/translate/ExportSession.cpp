
#include "ExportSession.h"
#include "extension/ExtensionsManager.h"
#include "scene/MayaScene.h"

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

// When we're sure these utilities stay, we can expose them
// as static method on CExportSession or a separate helper class


namespace // <anonymous>
{
   // TODO: use the renderLayer specified in the CMayaScene instead
   bool IsInRenderLayer(MDagPath dagPath)
   {
      MObject renderLayerObj = MFnRenderLayer::currentLayer();

      MFnRenderLayer curLayer(renderLayerObj);

      return curLayer.inCurrentRenderLayer(dagPath);
   }

   bool IsTemplated(MFnDagNode node)
   {
      MStatus status;

      if (node.isIntermediateObject())
        return false;

      MPlug templatePlug = node.findPlug("template", &status);
      MPlug overDispPlug = node.findPlug("overrideDisplayType", &status);

      if (status == MStatus::kFailure)
         return false;

      if (templatePlug.asBool())
        return true;
      else
         if (overDispPlug.asInt()==1)
            return true;
         else
            return false;
   }

   bool IsVisible(MFnDagNode node)
   {
      MStatus status;

      if (node.isIntermediateObject())
         return false;

      MPlug visPlug = node.findPlug("visibility", &status);
      MPlug overVisPlug = node.findPlug("overrideVisibility", &status);

      if (status == MStatus::kFailure)
         return false;

      if (visPlug.asBool() && overVisPlug.asBool())
         return true;
      else
         return false;
   }

   bool IsVisiblePath(MDagPath dagPath)
   {

      MStatus stat = MStatus::kSuccess;
      while (stat == MStatus::kSuccess)
      {
         MFnDagNode node(dagPath.node());
         if (!IsVisible(node))
            return false;
         stat = dagPath.pop();
      }
      return true;
   }

   bool IsTemplatedPath(MDagPath dagPath)
   {

      MStatus stat = MStatus::kSuccess;
      while (stat == MStatus::kSuccess)
      {
         MFnDagNode node(dagPath.node());
         if (IsTemplated(node))
            return true;
         stat = dagPath.pop();
      }
      return false;
   }

} // namespace

// Public Methods

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

bool CExportSession::IsRenderablePath(MDagPath dagPath)
{

   MStatus stat = MStatus::kSuccess;
   while (stat == MStatus::kSuccess)
   {
      MFnDagNode node(dagPath.node());
      if (!IsVisible(node) || IsTemplated(node))
         return false;
      stat = dagPath.pop();
   }
   return true;
}

// Private Methods

MStatus CExportSession::Begin(CExportOptions* options)
{
   MStatus status = MStatus::kSuccess;

   m_exportOptions = *options;
   UpdateMotionBlurData();

   return status;
}

MStatus CExportSession::End()
{
   MStatus status = MStatus::kSuccess;

   if (GetExportMode() == MTOA_EXPORT_IPR)
   {
      ClearUpdateCallbacks();
   }

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

   // Clear motion frames storage
   m_motion_frames.clear();

   return status;
}

MStatus CExportSession::UpdateMotionBlurData()
{
   MStatus status = MStatus::kSuccess;

   m_exportOptions.UpdateMotionBlurData();
   status = UpdateMotionFrames();

   return status;
}

MStatus CExportSession::UpdateMotionFrames()
{
   MStatus status = MStatus::kSuccess;

   double exportFrame         = m_exportOptions.m_frame;
   if (m_exportOptions.m_motion.enable_mask)
   {
      double byFrame             = m_exportOptions.m_motion.by_frame;
      double shutterOffset       = m_exportOptions.m_motion.shutter_offset;
      unsigned int motionSteps   = m_exportOptions.m_motion.steps;
      double stepSize            = byFrame / double(motionSteps - 1);
      if (m_motion_frames.size() != motionSteps) m_motion_frames.clear();
      if (m_motion_frames.capacity() != motionSteps) m_motion_frames.reserve(motionSteps);

      double frame = exportFrame - byFrame * 0.5 + shutterOffset;
      for (unsigned int step=0; (step < motionSteps); ++step)
      {
         frame += double(step) * stepSize;
         m_motion_frames.push_back(frame);
      }
   }
   else
   {
      m_motion_frames.clear();
      m_motion_frames.push_back(exportFrame);
   }

   return status;
}


/// Primary entry point for exporting a Maya scene to Arnold
MStatus CExportSession::Export(MSelectionList* selected)
{
   MStatus status;

   // It wouldn't be efficient to test the whole scene against selection state
   // so selected gets a special treatment
   bool exportSelected = (NULL != selected) ? true : false;

   // Set up export options
   ExportMode exportMode = m_exportOptions.m_mode;
   // Export the Arnold Render Options node
   ExportNode(m_exportOptions.m_options);
   // Are we motion blurred (any type)?
   const bool mb = IsMotionBlurEnabled();

   // In case of motion blur we need to position ourselves to first step
   // TODO : what if specific frame was requested and it's != GetExportFrame()
   if (mb)
   {
      // first step is the real export
      // FIXME: use middle step will give better average results for values
      // that can't be motion blurred in Arnold
      AiMsgDebug("[mtoa] Exporting step 0 at frame %f", m_motion_frames[0]);
   }
   else
   {
      AiMsgDebug("[mtoa] Exporting frame %f", m_motion_frames[0]);
   }
   // When there is no motion blur we push the export frame in m_motion_frames[0]
   // so m_motion_frames[0] == m_frame
   MGlobal::viewFrame(MTime(m_motion_frames[0], MTime::uiUnit()));

   // First "real" export
   if (exportMode == MTOA_EXPORT_RENDER || exportMode == MTOA_EXPORT_IPR)
   {
      // Either for a specific camera or export all cameras
      // Note : in "render selected" mode Maya exports all lights and cameras
      if (m_exportOptions.m_camera.isValid())
      {
         m_exportOptions.m_camera.extendToShape();
         ExportDagPath(m_exportOptions.m_camera);
      }
      else
      {
         status = ExportCameras();
      }
      // Then we filter them out to avoid double exporting cameras
      // m_exportOptions.m_filter.excluded.insert(MFn::kCamera);
      if (exportSelected)
      {
         // And for render selected we need the lights too
         status = ExportLights();
         // m_exportOptions.m_filter.excluded.insert(MFn::kLight);
         status = ExportSelection(*selected);
      }
      else
      {
         status = ExportScene();
      }
   }
   else if (exportMode == MTOA_EXPORT_FILE)
   {
      if (exportSelected)
      {
         // If we export selected to a file, not as a full render,
         // we don't need to export all lights / cameras
         status = ExportSelection(*selected);
      }
      else
      {
         // Else if it's a full / renderable scene
         status = ExportCameras();
         // Then we filter them out to avoid double exporting cameras
         // m_exportOptions.m_filter.excluded.insert(MFn::kCamera);
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
      // Note: do not reset frame or that's an extra unnecessary scene eval when exporting a sequence
      // caller (CMayaScene usually) should take care of resetting in single frame export mode
      // MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));
      m_isExportingMotion = false;
   }

   return status;
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

// Export the full maya scene
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
         filtered = FilteredStatus(path);
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
         if (FilteredStatus(path) == MTOA_EXPORT_ACCEPTED)
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

DagFiltered CExportSession::FilteredStatus(MDagPath path)
{
   // Tests that cause the whole branch to be pruned
   if (m_exportOptions.m_filter.templated == true && IsTemplatedPath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;
   if (m_exportOptions.m_filter.hidden == true && !IsVisiblePath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;

   // Tests that cause the node to be ignored
   if (m_exportOptions.m_filter.notinlayer == true && !IsInRenderLayer(path))
      return MTOA_EXPORT_REJECTED_NODE;

   // Then test against all types passed in the MFN::Types array
   MObject obj = path.node();
   MFnDagNode node(obj);
   MString name = node.name();
   ExcludeSet::const_iterator sit(m_exportOptions.m_filter.excluded.begin()), send(m_exportOptions.m_filter.excluded.end());
   for(; sit!=send;++sit)
      if (obj.hasFn(*sit))
         return MTOA_EXPORT_REJECTED_NODE;

   return MTOA_EXPORT_ACCEPTED;
}

// updates

void CExportSession::QueueForUpdate(CNodeTranslator * translator)
{
   if (translator != NULL) m_translatorsToUpdate.push_back(translator);
}

void CExportSession::RequestUpdate()
{
   m_requestUpdate = true;
   CMayaScene::UpdateIPR();
}

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
