#include "platform/Platform.h"
#include "ArnoldSession.h"
#include "attributes/Components.h"
#include "extension/ExtensionsManager.h"
#include "scene/MayaScene.h"
#include "translators/options/OptionsTranslator.h"

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

#include <assert.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// When we're sure these utilities stay, we can expose them
// as static method on CArnoldSession or a separate helper class


namespace // <anonymous>
{
   // TODO: use the renderLayer specified in the CMayaScene instead
   bool IsInRenderLayer(MDagPath dagPath)
   {
      MObject renderLayerObj = MFnRenderLayer::currentLayer();

      MFnRenderLayer curLayer(renderLayerObj);

      return curLayer.inCurrentRenderLayer(dagPath);
   }

   bool IsTemplated(MFnDagNode & node)
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

   bool IsVisible(MFnDagNode &node)
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
         MFnDagNode node;
         node.setObject(dagPath.node());
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
         MFnDagNode node;
         node.setObject(dagPath.node());
         if (IsTemplated(node))
            return true;
         stat = dagPath.pop();
      }
      return false;
   }
}


// Public Methods

// Export a single dag path (a dag node or an instance of a dag node)
// Considered to be already filtered and checked
AtNode* CArnoldSession::ExportDagPath(MDagPath &dagPath, MStatus* stat)
{
   MStatus status = MStatus::kSuccess;
   AtNode* arnoldNode = NULL;

   MString name = dagPath.partialPathName();
   MString type = MFnDagNode(dagPath).typeName();

   AiMsgTab(1);
   CDagTranslator* translator = CExtensionsManager::GetTranslator(dagPath);

   if (translator == NULL)
   {
      if (stat != NULL) *stat = MStatus::kNotImplemented;
      AiMsgDebug("[mtoa.session]     %-30s | Ignoring DAG node of type %s", name.asChar(), type.asChar());
      AiMsgTab(-1);
      return NULL;
   }
   else if (!translator->IsMayaTypeDag())
   {
      if (stat != NULL) *stat = MStatus::kInvalidParameter;
      AiMsgDebug("[mtoa] translator for %s of type %s is not a DAG translator", name.asChar(), type.asChar());
      AiMsgTab(-1);
      return NULL;
   }

   AiMsgDebug("[mtoa.session]     %-30s | Exporting DAG node of type %s", name.asChar(), type.asChar());

   CNodeAttrHandle handle(dagPath);
   if (!translator->DisableCaching())
   {
      // Check if node has already been processed
      ObjectToTranslatorMap::iterator it = m_processedTranslators.find(handle);
      if (it != m_processedTranslators.end())
      {
         delete translator;
         status = MStatus::kSuccess;
         arnoldNode = it->second->GetArnoldRootNode();
         translator = (CDagTranslator*)it->second;
      }
   }
   if (arnoldNode == NULL)
   {
      status = MStatus::kSuccess;
      translator->Init(this, dagPath);
      m_processedTranslators.insert(ObjectToTranslatorPair(handle, translator));
      arnoldNode = translator->DoExport(0);
   }

   if (NULL != stat) *stat = status;
   AiMsgTab(-1);
   return arnoldNode;
}

// Export a plug (dependency node output attribute)
//
AtNode* CArnoldSession::ExportNode(const MPlug& shaderOutputPlug, AOVSet* aovs, MStatus *stat)
{
   MObject mayaNode = shaderOutputPlug.node();
   MStatus status = MStatus::kSuccess;
   AtNode* arnoldNode = NULL;

   MDagPath dagPath;
   if (MDagPath::getAPathTo(mayaNode, dagPath) == MS::kSuccess)
   {
      MStatus status = MStatus::kSuccess;
      arnoldNode = ExportDagPath(dagPath, &status);
      // kInvalidParameter is returned when a non-DAG translator is used on a DAG node, but we can still export that here
      if (status != MStatus::kInvalidParameter)
      {
         if (stat != NULL) *stat = status;
         return arnoldNode;
      }
   }

   AiMsgTab(1);
   CNodeTranslator* translator = CExtensionsManager::GetTranslator(mayaNode);

   if (translator == NULL)
   {
      status = MStatus::kNotImplemented;
      MFnDependencyNode fnNode(mayaNode);
      AiMsgDebug("[mtoa.session]     %30s: Maya node type not supported: %s", fnNode.name().asChar(),
                 fnNode.typeName().asChar());
      AiMsgTab(-1);
      return NULL;
   }
   MPlug resultPlug;

   // returns the primary attribute (i.e. if the shaderOutputPlug is outColorR, resultPlug is outColor)

   ResolveFloatComponent(shaderOutputPlug, resultPlug);

   MPlug resolvedPlug;
   // resolving the plug gives translators a chance to replace ".message" with ".outColor",
   // for example, or to reject it outright.
   // once the attribute is properly resolved it can be used as a key in our multimap cache
   if (translator->ResolveOutputPlug(resultPlug, resolvedPlug))
   {
      resultPlug = resolvedPlug;
   }
   else
   {
      delete translator;
      translator = NULL;
      status = MStatus::kNotImplemented;
      MFnDependencyNode fnNode(mayaNode);
      AiMsgDebug("[mtoa] [maya %s] Invalid output attribute: \"%s\"", fnNode.name().asChar(),
                 resultPlug.partialName(false, false, false, false, false, true).asChar());
      AiMsgTab(-1);
      return NULL;
   }

   CNodeAttrHandle handle(resultPlug);
   if (!translator->DisableCaching())
   {
      // Check if node has already been processed
      ObjectToTranslatorMap::iterator it = m_processedTranslators.find(handle);
      if (it != m_processedTranslators.end())
      {
         delete translator;
         status = MStatus::kSuccess;
         arnoldNode = it->second->GetArnoldRootNode();
         translator = it->second;
      }
   }
   if (arnoldNode == NULL)
   {
      status = MStatus::kSuccess;
      translator->Init(this, mayaNode, resultPlug.partialName(false, false, false, false, false, true));
      m_processedTranslators.insert(ObjectToTranslatorPair(handle, translator));
      arnoldNode = translator->DoExport(0);
   }
   if (arnoldNode != NULL)
   {
      if (aovs != NULL)
         translator->GetAOVs(aovs);
   }
   if (NULL != stat) *stat = status;
   AiMsgTab(-1);
   return arnoldNode;
}

// FIXME: it's a very bad idea to export without registering the translator, leads to multiple exports in IPR
AtNode* CArnoldSession::ExportWithTranslator(MObject mayaNode, const MString &mayaNodeClass, const MString &translatorName)
{
   assert(AiUniverseIsActive());

   AtNode* shader = NULL;

   AiMsgTab(1);
   CNodeTranslator* translator = CExtensionsManager::GetTranslator(mayaNodeClass, translatorName);
   if (translator != NULL)
   {
      shader = translator->Init(this, mayaNode);
      AiMsgDebug("[mtoa.session]     %-30s | Exporting type %s using translator: \"%s\"",
            MFnDependencyNode(mayaNode).name().asChar(),
            mayaNodeClass.asChar(),
            translatorName.asChar());
      translator->DoExport(0);
      CNodeAttrHandle handle(mayaNode, translatorName);
      m_processedTranslators.insert(ObjectToTranslatorPair(handle, translator));
   }
   else
   {
      AiMsgDebug("[mtoa.session]     %-30s | Skipping export of type %s: translator type not supported: \"%s\"",
            MFnDependencyNode(mayaNode).name().asChar(),
            mayaNodeClass.asChar(),
            translatorName.asChar());
   }
   AiMsgTab(-1);
   return shader;
}

AtNode* CArnoldSession::ExportDriver(MObject mayaNode, const MString &translatorName)
{
   return ExportWithTranslator(mayaNode, "<driver>", translatorName);
}

AtNode* CArnoldSession::ExportFilter(MObject mayaNode, const MString &translatorName)
{
   return ExportWithTranslator(mayaNode, "<filter>", translatorName);
}

bool CArnoldSession::GetActiveTranslators(const MObject& object, std::vector<CNodeTranslator* >& result)
{
   CNodeAttrHandle handle(object);
   ObjectToTranslatorMap::iterator it = m_processedTranslators.find(handle);
   if (it == m_processedTranslators.end())
      return false;
   for (; it != m_processedTranslators.end(); ++it)
   {
      result.push_back(static_cast< CNodeTranslator* >(it->second));
   }
   return true;
}

CNodeTranslator * CArnoldSession::GetActiveTranslator(const MPlug& plug)
{
   CNodeAttrHandle handle(plug);
   ObjectToTranslatorMap::iterator it = m_processedTranslators.find(handle);
   if (it != m_processedTranslators.end())
   {
      return static_cast< CNodeTranslator* >(it->second);
   }
   return NULL;
}

bool CArnoldSession::IsRenderablePath(MDagPath dagPath)
{

   MStatus stat = MStatus::kSuccess;
   while (stat == MStatus::kSuccess)
   {
      MFnDagNode node;
      node.setObject(dagPath.node());
      if (!IsVisible(node) || IsTemplated(node))
         return false;
      stat = dagPath.pop();
   }
   return true;
}

// Private Methods

/*
/// For each active AOV add a CAOV class to m_aovs
void CArnoldSession::ProcessAOVs()
{
   MFnDependencyNode fnOptions = MFnDependencyNode(GetArnoldRenderOptions());
   AOVMode aovMode = AOVMode(fnOptions.findPlug("aovMode").asInt());
   if (aovMode == AOV_MODE_ENABLED ||
         (IsBatch() && aovMode == AOV_MODE_BATCH_ONLY))
   {
      MPlugArray conns;
      MPlug pAOVs = fnOptions.findPlug("aovs");
      for (unsigned int i = 0; i < pAOVs.evaluateNumElements(); ++i)
      {
         if (pAOVs[i].connectedTo(conns, true, false))
         {
            CAOV aov;
            MObject oAOV = conns[0].node();
            if (aov.FromMaya(oAOV))
               if (aov.IsEnabled())
                  m_aovs.insert(aov);
            else
               MGlobal::displayWarning("[mtoa] Could not setup AOV attribute " + MFnDependencyNode(oAOV).name());
         }
      }
   }
   else
      AiMsgDebug("[mtoa] [aovs] disabled");
}
*/

MStatus CArnoldSession::Begin(const CSessionOptions &options)
{
   MStatus status = MStatus::kSuccess;

   m_sessionOptions = options;

   status = UpdateLightLinks();
   status = UpdateMotionFrames();

   m_is_active = true;

   //ProcessAOVs();
   return status;
}

MStatus CArnoldSession::End()
{
   MStatus status = MStatus::kSuccess;

   if (GetSessionMode() == MTOA_SESSION_IPR)
   {
      ClearUpdateCallbacks();
   }
   else if (GetSessionMode() == MTOA_SESSION_ASS && MGlobal::mayaState() == MGlobal::kInteractive && IsMotionBlurEnabled())
   {
      // reset to export frame
      MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));
   }

   // Delete stored translators
   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
   {
      //AiMsgDebug("[mtoa] Deleting translator for %s", MFnDependencyNode(it->first.object()).name().asChar());
      delete it->second;
   }
   // Any translators are in the processed translators map, so already deleted
   m_processedTranslators.clear();
   m_translatorsToUpdate.clear();
   m_optionsTranslator = NULL;
   m_masterInstances.clear();
   // Clear motion frames storage
   m_motion_frames.clear();

   m_is_active = false;

   return status;
}

MStatus CArnoldSession::UpdateLightLinks()
{
   MStatus status = MStatus::kSuccess;
   if (m_sessionOptions.GetLightLinkMode() == MTOA_LIGHTLINK_MAYA
         || m_sessionOptions.GetShadowLinkMode() == MTOA_SHADOWLINK_MAYA)
   {
      // Default values except last. We set componentSupport = false
      status = m_lightLinks.parseLinks(MObject::kNullObj, false, NULL, false, false);
   }

   return status;
}

MStatus CArnoldSession::UpdateMotionFrames()
{
   MStatus status = MStatus::kSuccess;

   double exportFrame         = m_sessionOptions.m_frame;
   if (m_sessionOptions.m_motion.enable_mask)
   {
      double byFrame             = m_sessionOptions.m_motion.by_frame;
      double shutterOffset       = m_sessionOptions.m_motion.shutter_offset;
      unsigned int motionSteps   = m_sessionOptions.m_motion.steps;
      double stepSize            = byFrame / double(motionSteps - 1);
      if (m_motion_frames.size() != motionSteps) m_motion_frames.clear();
      if (m_motion_frames.capacity() != motionSteps) m_motion_frames.reserve(motionSteps);

      double frame = exportFrame - byFrame * 0.5 + shutterOffset;
      for (unsigned int step=0; (step < motionSteps); ++step)
      {
         m_motion_frames.push_back(frame + (double)step * stepSize);
      }
   }
   else
   {
      m_motion_frames.clear();
      m_motion_frames.push_back(exportFrame);
   }

   return status;
}

/// Export the Arnold Render Options node
AtNode* CArnoldSession::ExportOptions()
{
   MObject options = m_sessionOptions.GetArnoldRenderOptions();
   if (options.isNull())
   {
      AiMsgWarning("[mtoa] Failed to find Arnold options node");
      return NULL;
   }
   MFnDependencyNode fnNode(options);
   AiMsgDebug("[mtoa] Exporting Arnold options '%s'", fnNode.name().asChar());
   MPlug optPlug = fnNode.findPlug("message");
   AtNode* result = ExportNode(optPlug);
   // Store the options translator for later use
   m_optionsTranslator = (COptionsTranslator*)GetActiveTranslator(optPlug);
   return result;
}

/// Primary entry point for exporting a Maya scene to Arnold
MStatus CArnoldSession::Export(MSelectionList* selected)
{
   MStatus status;

   if (!AiUniverseIsActive())
   {
      AiMsgError("[mtoa] Need an active Arnold universe to export to.");
      return MStatus::kFailure;
   }

   // It wouldn't be efficient to test the whole scene against selection state
   // so selected gets a special treatment
   bool exportSelected = (NULL != selected) ? true : false;
   if (exportSelected)
   {
      AiMsgDebug("[mtoa] Exporting selection (%i objects)", selected->length());
   }
   else
   {
      AiMsgDebug("[mtoa] Exporting scene");
   }

   // Set up export options
   ArnoldSessionMode exportMode = m_sessionOptions.m_mode;

   // Are we motion blurred (any type)?
   const bool mb = IsMotionBlurEnabled();

   // In case of motion blur we need to position ourselves to first step
   // TODO : what if specific frame was requested and it's != GetExportFrame()
   if (mb)
   {
      // first step is the real export
      // FIXME: use middle step will give better average results for values
      // that can't be motion blurred in Arnold
      AiMsgDebug("[mtoa.session]     Exporting step 0 at frame %f", m_motion_frames[0]);
   }
   else
   {
      AiMsgDebug("[mtoa.session]     Exporting frame %f", m_motion_frames[0]);
   }
   // When there is no motion blur we push the export frame in m_motion_frames[0]
   // so m_motion_frames[0] == m_frame
   MGlobal::viewFrame(MTime(m_motion_frames[0], MTime::uiUnit()));

   // Export the Arnold Render Options node
   ExportOptions();

   // First "real" export
   if (exportMode == MTOA_SESSION_RENDER || exportMode == MTOA_SESSION_BATCH || exportMode == MTOA_SESSION_IPR)
   {
      // Either for a specific camera or export all cameras
      // Note : in "render selected" mode Maya exports all lights and cameras
      if (m_sessionOptions.m_camera.isValid())
      {
         m_sessionOptions.m_camera.extendToShape();
         ExportDagPath(m_sessionOptions.m_camera);
      }
      else
      {
         status = ExportCameras();
      }
      // Then we filter them out to avoid double exporting cameras
      // m_sessionOptions.m_filter.excluded.insert(MFn::kCamera);
      if (exportSelected)
      {
         // And for render selected we need the lights too
         status = ExportLights();
         // m_sessionOptions.m_filter.excluded.insert(MFn::kLight);
         status = ExportSelection(*selected);
      }
      else
      {
         status = ExportScene();
      }
   }
   else if (exportMode == MTOA_SESSION_ASS)
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
         if (m_sessionOptions.m_camera.isValid())
         {
            m_sessionOptions.m_camera.extendToShape();
            ExportDagPath(m_sessionOptions.m_camera);
         }
         else
         {
            status = ExportCameras();
         }
         // Then we filter them out to avoid double exporting cameras
         // m_sessionOptions.m_filter.excluded.insert(MFn::kCamera);
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
         AiMsgDebug("[mtoa.session]     Exporting step %d at frame %f", step, m_motion_frames[step]);
         // then, loop through the already processed dag translators and export for current step
         // NOTE: these exports are subject to the normal pre-processed checks which prevent redundant exports.
         // Since all nodes *should* be exported at this point, the following calls to DoExport do not
         // traverse the DG even if the translators call ExportNode or ExportDag. This makes it safe
         // to re-export all objects from a flattened list

         // finally, loop through the already processed translators and export for current step
         ObjectToTranslatorMap::iterator it;
         for (it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
         {
            it->second->DoExport(step);
         }
      }
      // Note: only reset frame during interactive renders, otherwise that's an extra unnecessary scene eval
      // when exporting a sequence.  Other modes are reset to the export frame in CArnoldSessions::End().
      if (GetSessionMode() == MTOA_SESSION_RENDER || GetSessionMode() == MTOA_SESSION_IPR)
      {
         MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));
      }
      m_isExportingMotion = false;
   }

   // add callbacks after all is done
   if (GetSessionMode() == MTOA_SESSION_IPR)
   {
      // TODO: unify storage for translators, this is cumbersome
      // For Node translators
      ObjectToTranslatorMap::iterator it;
      for (it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
      {
         it->second->AddUpdateCallbacks();
      }
   }

   return status;
}

// Export the cameras of the maya scene
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CArnoldSession::ExportCameras()
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
         if (m_sessionOptions.m_filter.notinlayer == true && !IsInRenderLayer(path))
            continue;
         if (m_sessionOptions.m_filter.templated == true && IsTemplatedPath(path))
            continue;
         if (m_sessionOptions.m_filter.hidden == true && !IsVisiblePath(path))
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
MStatus CArnoldSession::ExportLights()
{
   MStatus status = MStatus::kSuccess;
   MDagPath path;
   MItDag   dagIterLights(MItDag::kDepthFirst, MFn::kLight);

   // First we export all cameras
   // We do not reset the iterator to avoid getting kWorld
   unsigned int mask = GetExportFilter();
   for (; (!dagIterLights.isDone()); dagIterLights.next())
   {
      if (dagIterLights.getPath(path))
      {
         // Only check for cameras being visible, not templated and in render layer
         // FIXME: does a light need to be in layer to render actually in Maya?
         MFnDagNode node(path.node());
         MString name = node.name();
         if ((mask & MTOA_FILTER_LAYER) && !IsInRenderLayer(path))
            continue;
         if ((mask & MTOA_FILTER_TEMPLATED) && IsTemplatedPath(path))
            continue;
         if ((mask & MTOA_FILTER_HIDDEN) && !IsVisiblePath(path))
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
MStatus CArnoldSession::ExportScene()
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
         ExportDagPath(path, &status);
      }
      else
      {
         AiMsgError("[mtoa] Could not get path for Maya DAG iterator.");
         status = MStatus::kInvalidParameter;
      }
   }
   
   return status;
}

// Get the selection from maya and export it with the IterSelection methode
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CArnoldSession::ExportSelection(MSelectionList& selected)
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
MStatus CArnoldSession::IterSelection(MSelectionList& selected)
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
            for (unsigned int child = 0; (child < path.childCount()); child++)
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

DagFiltered CArnoldSession::FilteredStatus(MDagPath path)
{
   // Tests that cause the whole branch to be pruned
   unsigned int mask = GetExportFilter();
   if ((mask & MTOA_FILTER_TEMPLATED) && IsTemplatedPath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;
   if ((mask & MTOA_FILTER_HIDDEN) && !IsVisiblePath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;
   // Tests that cause the node to be ignored
   if ((mask & MTOA_FILTER_LAYER) && !IsInRenderLayer(path))
      return MTOA_EXPORT_REJECTED_NODE;

   // Then test against all types passed in the MFN::Types array
   MObject obj = path.node();
   MFnDagNode node(obj);
   MString name = node.name();
   MFnTypeSet::const_iterator sit(m_sessionOptions.m_filter.excluded.begin()), send(m_sessionOptions.m_filter.excluded.end());
   for(; sit!=send;++sit)
      if (obj.hasFn(*sit))
         return MTOA_EXPORT_REJECTED_NODE;

   return MTOA_EXPORT_ACCEPTED;
}

// updates
void CArnoldSession::QueueForUpdate(CNodeTranslator * translator)
{
   if (translator != NULL) m_translatorsToUpdate.push_back(translator);
}

void CArnoldSession::RequestUpdate()
{
   m_requestUpdate = true;
   CMayaScene::UpdateIPR();
}

void CArnoldSession::DoUpdate()
{
   assert(AiUniverseIsActive());
   // Are we motion blurred?
   bool mb = IsMotionBlurEnabled();
   if (mb)
   {
      // don't step through frames if our translators don't need motion blur
      mb = false;
      for (std::vector<CNodeTranslator*>::iterator iter = m_translatorsToUpdate.begin();
         iter != m_translatorsToUpdate.end(); ++iter)
      {
         if ((*iter)->RequiresMotionData())
         {
            mb = true;
            break;
         }
      }
   }

   if (!mb)
   {
      for (std::vector<CNodeTranslator*>::iterator iter = m_translatorsToUpdate.begin();
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
         AiMsgDebug("[mtoa.session]     Updating step %d at frame %f", step, m_motion_frames[step]);
         MGlobal::viewFrame(MTime(m_motion_frames[step], MTime::uiUnit()));
         for (std::vector<CNodeTranslator*>::iterator iter = m_translatorsToUpdate.begin();
             iter != m_translatorsToUpdate.end(); ++iter)
         {
            CNodeTranslator* translator = (*iter);
            if (translator != NULL) translator->DoUpdate(step);
         }
      }
      MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));

      m_isExportingMotion = false;
   }

   // add callbacks after all is done
   if (GetSessionMode() == MTOA_SESSION_IPR)
   {
      // re-add IPR callbacks to all updated translators after ALL updates are done
      for(std::vector<CNodeTranslator*>::iterator iter = m_translatorsToUpdate.begin();
         iter != m_translatorsToUpdate.end(); ++iter)
      {
         CNodeTranslator* translator = (*iter);
         if (translator != NULL) translator->AddUpdateCallbacks();
      }
   }

   // Clear the list and the request update flag.
   m_translatorsToUpdate.clear();
   m_requestUpdate = false;
}

void CArnoldSession::ClearUpdateCallbacks()
{
   // Clear the list of translators to update.
   m_translatorsToUpdate.clear();

   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
   {
      if (it->second != NULL) it->second->RemoveUpdateCallbacks();
   }
}

/// Set the camera to export.

/// If called prior to export, only the specified camera will be exported. If not set, all cameras
/// will be exported, but some translators may not be able to fully export without an export camera specified.
/// To address this potential issue, this method should be called after a multi-cam export, as it will cause all
/// translators for which CNodeTranslator::DependsOnExportCamera() returns true to be updated.
///
void CArnoldSession::SetExportCamera(MDagPath camera)
{
   AiMsgDebug("[mtoa.session] Setting export camera to \"%s\"", camera.partialPathName().asChar());
   m_sessionOptions.SetExportCamera(camera);

   // queue up translators for update
   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
   {
      if (it->second->DependsOnExportCamera())
         QueueForUpdate(it->second);
   }
   DoUpdate();
}

bool CArnoldSession::IsActiveAOV(CAOV &aov) const
{
   if (m_optionsTranslator != NULL)
      return m_optionsTranslator->IsActiveAOV(aov);
   return false;
}

AOVSet CArnoldSession::GetActiveAOVs() const
{
   if (m_optionsTranslator != NULL)
      return m_optionsTranslator->GetActiveAOVs();
   AOVSet empty;
   return empty;
}

// Returns the instance number of the master instance (it's not always 0!)
// Returns -1 if no master instance has been encountered yet
int CArnoldSession::GetMasterInstanceNumber(MObject node)
{
   MObjectHandle handle = MObjectHandle(node);
   // if handle is not in the map, a new entry will be made with a default value
   MDagPath dagPath = m_masterInstances[handle];
   if (dagPath.isValid())
   {
      return dagPath.instanceNumber();
   }
   return -1;
}

