#include "platform/Platform.h"
#include "ArnoldSession.h"
#include "attributes/Components.h"
#include "extension/ExtensionsManager.h"
#include "scene/MayaScene.h"
#include "translators/options/OptionsTranslator.h"
#include "nodes/ShaderUtils.h"

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
#include <maya/MItDependencyNodes.h>
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
CDagTranslator* CArnoldSession::ExportDagPath(MDagPath &dagPath, bool initOnly, MStatus* stat)
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

   ObjectToTranslatorMap::iterator it = m_processedTranslators.end();
   if (!translator->DisableCaching())
   {
      // Check if node has already been processed
      // FIXME: since it's a multimap there can be more than one translator associated ?
      // ObjectToTranslatorMap::iterator it, itlo, itup;
      // itlo = m_processedTranslators.lower_bound(handle);
      // itup = m_processedTranslators.upper_bound(handle);
      it = m_processedTranslators.find(handle);
      if (it != m_processedTranslators.end())
      {
         AiMsgDebug("[mtoa.session]     %-30s | Reusing previous export of DAG node of type %s", name.asChar(), type.asChar());

         delete translator;
         status = MStatus::kSuccess;
         arnoldNode = it->second->GetArnoldRootNode();
         translator = (CDagTranslator*)it->second;
      }
   }
   if (arnoldNode == NULL)
   {
      if (initOnly)
         AiMsgDebug("[mtoa.session]     %-30s | Initializing DAG node of type %s", name.asChar(), type.asChar());
      status = MStatus::kSuccess;
      translator->Init(this, dagPath);
      if (it != m_processedTranslators.end())
      {
         it->second = translator;
      }
      else
      {
         m_processedTranslators.insert(ObjectToTranslatorPair(handle, translator));
         m_processedTranslatorList.push_back(translator);
      }
      if (!initOnly)
         arnoldNode = translator->DoExport(0);
   }

   if (NULL != stat) *stat = status;
   AiMsgTab(-1);
   return translator;
}

// Export a plug (dependency node output attribute)
//
CNodeTranslator* CArnoldSession::ExportNode(const MPlug& shaderOutputPlug, AtNodeSet* nodes, AOVSet* aovs,
                                   bool initOnly, MStatus *stat)
{
   MObject mayaNode = shaderOutputPlug.node();
   MStatus status = MStatus::kSuccess;
   AtNode* arnoldNode = NULL;
   CNodeTranslator* translator = NULL;
   MDagPath dagPath;
   // FIXME: should get correct instance number from plug
   if (MDagPath::getAPathTo(mayaNode, dagPath) == MS::kSuccess)
   {
      MStatus status = MStatus::kSuccess;
      translator = (CNodeTranslator*)ExportDagPath(dagPath, initOnly, &status);
      // kInvalidParameter is returned when a non-DAG translator is used on a DAG node, but we can still export that here
      if (status != MStatus::kInvalidParameter)
      {
         if (stat != NULL) *stat = status;
         return translator;
      }
   }

   MFnDependencyNode fnNode(mayaNode);
   MString name = fnNode.name();
   MString type = fnNode.typeName();

   translator = CExtensionsManager::GetTranslator(mayaNode);
   AiMsgTab(1);

   if (translator == NULL)
   {
      status = MStatus::kNotImplemented;
      AiMsgDebug("[mtoa.session]     %30s: Maya node type not supported: %s", name.asChar(), type.asChar());
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
      AiMsgDebug("[mtoa] [maya %s] Invalid output attribute: \"%s\"", name.asChar(),
                 resultPlug.partialName(false, false, false, false, false, true).asChar());
      AiMsgTab(-1);
      return NULL;
   }

   MString plugName = resultPlug.name();
   AiMsgDebug("[mtoa.session]     %-30s | Exporting plug %s for type %s",
      name.asChar(), plugName.asChar(), type.asChar());
   CNodeAttrHandle handle;
   if (translator->DependsOnOutputPlug())
      handle.set(resultPlug);
   else
      handle.set(mayaNode);
   ObjectToTranslatorMap::iterator it = m_processedTranslators.end();
   if (!translator->DisableCaching())
   {
      // Check if node has already been processed
      // FIXME: since it's a multimap there can be more than one translator associated ?
      // ObjectToTranslatorMap::iterator it, itlo, itup;
      // itlo = m_processedTranslators.lower_bound(handle);
      // itup = m_processedTranslators.upper_bound(handle);
      it = m_processedTranslators.find(handle);
      if (it != m_processedTranslators.end())
      {
         AiMsgDebug("[mtoa.session]     %-30s | Reusing previous export of node of type %s", name.asChar(), type.asChar());

         delete translator;
         status = MStatus::kSuccess;
         arnoldNode = it->second->GetArnoldRootNode();
         translator = it->second;
      }
   }
   if (arnoldNode == NULL)
   {
      if (initOnly)
         AiMsgDebug("[mtoa.session]     %-30s | Initializing node of type %s", name.asChar(), type.asChar());
      status = MStatus::kSuccess;
      translator->TrackShaders(nodes);
      translator->Init(this, mayaNode, resultPlug.partialName(false, false, false, false, false, true));
      if (it != m_processedTranslators.end())
      {
         it->second = translator;
      }
      else
      {
         m_processedTranslators.insert(ObjectToTranslatorPair(handle, translator));
         m_processedTranslatorList.push_back(translator);
      }
      if (!initOnly)
         arnoldNode = translator->DoExport(0);
   }
   if (arnoldNode != NULL)
   {
      if (nodes != NULL)
      {
         std::map<std::string, AtNode*>::iterator nodeIt;
         for (nodeIt = translator->m_atNodes.begin(); nodeIt != translator->m_atNodes.end(); ++nodeIt)
         {
            nodes->insert(nodeIt->second);
         }
      }
      if (aovs != NULL)
         translator->TrackAOVs(aovs);
   }
   if (NULL != stat) *stat = status;
   AiMsgTab(-1);
   return translator;
}

unsigned int CArnoldSession::GetActiveTranslators(const CNodeAttrHandle &handle, std::vector<CNodeTranslator* >& result)
{
   result.clear();
   ObjectToTranslatorMap::iterator it, itlo, itup;
   itlo = m_processedTranslators.lower_bound(handle);
   itup = m_processedTranslators.upper_bound(handle);
   for (it = itlo; it != itup; it++)
   {
      result.push_back(static_cast< CNodeTranslator* >(it->second));
   }
   return result.size();
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

   status = UpdateMotionFrames();

   m_is_active = true;
   m_requestUpdate = false;

   //ProcessAOVs();
   return status;
}

MStatus CArnoldSession::End()
{
   MStatus status = MStatus::kSuccess;

   m_requestUpdate = false;
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
   for (unsigned int i=0; i < m_processedTranslatorList.size(); ++i)
   {
      //AiMsgDebug("[mtoa] Deleting translator for %s in %p", MFnDependencyNode(it->first.object()).name().asChar(), it->second);
      delete m_processedTranslatorList[i];
   }
   // Any translators are in the processed translators map, so already deleted
   m_processedTranslators.clear();
   m_objectsToUpdate.clear();
   m_optionsTranslator = NULL;
   m_processedTranslatorList.clear();
   m_masterInstances.clear();
   // Clear motion frames storage
   m_motion_frames.clear();

   m_is_active = false;

   return status;
}

MStatus CArnoldSession::UpdateLightLinks()
{
   // FIXME: we are not sure all these lights will actually have been exported to 
   // the Arnold universe
   // Possible solution, make sure ExportLights is done first, before dag objects
   // are exported and light linking queried, and count the number of lights
   // actually in the Arnold universe
   m_numLights = 0;
   MItDag dagIterLights(MItDag::kDepthFirst, MFn::kLight);
   for (; (!dagIterLights.isDone()); dagIterLights.next())
   {
      m_numLights += 1;
   }
   // For plugin lights, 
   MItDag dagIterPlugin(MItDag::kDepthFirst, MFn::kPluginLocatorNode);
   for (; (!dagIterPlugin.isDone()); dagIterPlugin.next())
   {
      if (CMayaScene::IsArnoldLight(dagIterPlugin.currentItem()))
      {
         m_numLights += 1;
      }
   }
   // TODO : turn off light linking option if we detect here that all lights
   // "illuminate by default" ?

   MStatus status = MStatus::kSuccess;   
   m_arnoldLightLinks.ClearLightLinks();

   if (m_numLights > 0)
   {      
      m_arnoldLightLinks.SetLinkingMode(m_sessionOptions.GetLightLinkMode(), 
           m_sessionOptions.GetShadowLinkMode());
      if (m_sessionOptions.GetLightLinkMode() == MTOA_LIGHTLINK_MAYA
            || m_sessionOptions.GetShadowLinkMode() == MTOA_SHADOWLINK_MAYA)
      {
         // Default values except last. We set componentSupport = false
         //status = m_lightLinks.parseLinks(MObject::kNullObj, false, NULL, false, false);
         m_arnoldLightLinks.ParseLightLinks();
      }

      if (MS::kSuccess == status)
      {
         AiMsgDebug("[mtoa] Parsed light linking information for %i lights", m_numLights);
      }
      else
      {
         AiMsgError("[mtoa] Failed to parse light linking information for %i lights", m_numLights);
      }
   }
   else
   {
      AiMsgWarning("[mtoa] No light in scene");
   }

   FlagLightLinksDirty(false);
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
   m_optionsTranslator = (COptionsTranslator*)ExportNode(optPlug, NULL, NULL, true);

   return m_optionsTranslator->GetArnoldRootNode();
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
      unsigned int ns = selected->length();
      status = FlattenSelection(selected);
      unsigned int fns = selected->length();
      AiMsgDebug("[mtoa] Exporting selection (%i:%i)", ns, fns);
   }
   else
   {
      AiMsgDebug("[mtoa] Exporting scene");
   }

   // Set up export options
   ArnoldSessionMode exportMode = m_sessionOptions.m_mode;

   // Are we motion blurred (any type)?
   const bool mb = IsMotionBlurEnabled();

   AiMsgDebug("[mtoa.session]     Initializing at frame %f", GetExportFrame());

   ExportOptions();  // inside loop so that we're on the proper frame

   // First "real" export
   MGlobal::viewFrame(m_sessionOptions.m_frame);
   if (exportMode == MTOA_SESSION_RENDER || exportMode == MTOA_SESSION_BATCH || exportMode == MTOA_SESSION_IPR)
   {
      // Either for a specific camera or export all cameras
      // Note : in "render selected" mode Maya exports all lights and cameras
      if (m_sessionOptions.m_camera.isValid())
      {
         m_sessionOptions.m_camera.extendToShape();
         ExportDagPath(m_sessionOptions.m_camera, true);
      }
      else
      {
         status = ExportCameras();
      }
      // Then we filter them out to avoid double exporting cameras
      // m_sessionOptions.m_filter.excluded.insert(MFn::kCamera);
      // For render selected we need all the lights (including unselected ones)
      status = ExportLights();
      // m_sessionOptions.m_filter.excluded.insert(MFn::kLight);
      status = ExportDag(selected);
   }
   else if (exportMode == MTOA_SESSION_ASS)
   {
      if (exportSelected)
      {
         // If we export selected to a file, not as a full render,
         // we don't need to export all lights / cameras, but
         // we export the selected ones first
         status = ExportCameras(selected);
         status = ExportLights(selected);
         status = ExportDag(selected);
      }
      else
      {
         // Else if it's a full / renderable scene
         if (m_sessionOptions.m_camera.isValid())
         {
            m_sessionOptions.m_camera.extendToShape();
            ExportDagPath(m_sessionOptions.m_camera, true);
         }
         else
         {
            status = ExportCameras();
         }
         // Then we filter them out to avoid double exporting cameras
         // m_sessionOptions.m_filter.excluded.insert(MFn::kCamera);
         // Update light linking info
         // FIXME: use a translator for light linker node(s)
         status = ExportLights();
         status = ExportDag();
      }
   }
   else
   {
      AiMsgError("[mtoa] Unsupported export mode: %d", exportMode);
      return MStatus::kFailure;
   }

   // loop through motion steps
   unsigned int numSteps = GetNumMotionSteps();
   for (unsigned int step = 0; step < numSteps; ++step)
   {
      if ((step != 0) || (m_motion_frames[step] != m_sessionOptions.m_frame))
         MGlobal::viewFrame(MTime(m_motion_frames[step], MTime::uiUnit()));
      AiMsgDebug("[mtoa.session]     Exporting step %d of %d at frame %f", step+1, numSteps, m_motion_frames[step]);
      if (step == 1)
         m_isExportingMotion = true;

      // then, loop through the already processed dag translators and export for current step
      // NOTE: these exports are subject to the normal pre-processed checks which prevent redundant exports.
      // Since all nodes *should* be exported at this point, the following calls to DoExport do not
      // traverse the DG even if the translators call ExportNode or ExportDag. This makes it safe
      // to re-export all objects from a flattened list

      // get the size first, because on step 0, m_processedTranslatorList will grow as we export
      unsigned int size = m_processedTranslatorList.size();
      // finally, loop through the already processed translators and export for current step
      for (unsigned int i=0; i < size; ++i)
      {
         m_processedTranslatorList[i]->DoExport(step);
      }
   }
   m_isExportingMotion = false;

   if (mb)
   {
      // Note: only reset frame during interactive renders, otherwise that's an extra unnecessary scene eval
      // when exporting a sequence.  Other modes are reset to the export frame in CArnoldSessions::End().
      if (GetSessionMode() == MTOA_SESSION_RENDER || GetSessionMode() == MTOA_SESSION_IPR)
      {
         MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));
      }
   }


   // add callbacks after all is done
   if (GetSessionMode() == MTOA_SESSION_IPR)
   {
      ObjectToTranslatorMap::iterator it;
      for (unsigned int i=0; i < m_processedTranslatorList.size(); ++i)
      {
         m_processedTranslatorList[i]->AddUpdateCallbacks();
      }
   }

   return status;
}

// Export the cameras of the maya scene
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CArnoldSession::ExportCameras(MSelectionList* selected)
{
   MStatus status = MStatus::kSuccess;

   // If we got a selection list iterate it and pick cameras
   if (NULL != selected)
   {
      MItSelectionList it(*selected, MFn::kInvalid, &status);
      MDagPath path;
      for (it.reset(); !it.isDone(); it.next())
      {
         // Silently skip non dag and non camera items in selection
         if (it.getDagPath(path) == MStatus::kSuccess)
         {
            if (path.node().hasFn(MFn::kCamera))
            {
               if (ExportDagPath(path) == NULL) status = MStatus::kFailure;
            }
         }
      }
   }
   else
   {
      // No selection we need all cameras in scene
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
            MStatus stat;
            ExportDagPath(path, true, &stat);
            if (stat != MStatus::kSuccess)
               status = MStatus::kFailure;
         }
         else
         {
            AiMsgError("[mtoa] Could not get path for Maya cameras DAG iterator.");
            status = MS::kFailure;
         }
      }
   }

   return status;
}

// Export the lights of the maya scene
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CArnoldSession::ExportLights(MSelectionList* selected)
{
   MStatus status = MStatus::kSuccess;

   // If we got a selection list iterate it and pick lights
   if (NULL != selected)
   {
      MItSelectionList it(*selected, MFn::kInvalid, &status);
      MDagPath path;
      MObject node;
      for (it.reset(); !it.isDone(); it.next())
      {
         // Silently skip non dag and non light items in selection
         if (it.getDagPath(path) == MStatus::kSuccess)
         {
            node = path.node();
            if (node.hasFn(MFn::kLight) || CMayaScene::IsArnoldLight(node))
            {
               if (ExportDagPath(path, true) == NULL) status = MStatus::kFailure;
            }
         }
      }
   }
   else
   {
      // No selection we need all lights in scene

      MDagPath path;
      MItDag   dagIterLights(MItDag::kDepthFirst, MFn::kLight, &status);

      // First we export all lights
      // We do not reset the iterator to avoid getting kWorld
      unsigned int mask = GetExportFilterMask();
      for (; (!dagIterLights.isDone()); dagIterLights.next())
      {
         if (dagIterLights.getPath(path))
         {
            // Only check for lights being visible, not templated and in render layer
            // FIXME: does a light need to be in layer to render actually in Maya?
            MFnDagNode node(path.node());
            MString name = node.name();
            if ((mask & MTOA_FILTER_LAYER) && !IsInRenderLayer(path))
               continue;
            if ((mask & MTOA_FILTER_TEMPLATED) && IsTemplatedPath(path))
               continue;
            if ((mask & MTOA_FILTER_HIDDEN) && !IsVisiblePath(path))
               continue;
            MStatus stat;
            ExportDagPath(path, true, &stat);
            if (stat != MStatus::kSuccess)
               status = MStatus::kFailure;
         }
         else
         {
            AiMsgError("[mtoa] Could not get path for Maya lights DAG iterator.");
            status = MS::kFailure;
         }
      }

      // Above will not catch plugin lights,
      MString           classification;
      MItDag            dagIterPlugin(MItDag::kDepthFirst, MFn::kPluginLocatorNode, &status);
      for (; (!dagIterPlugin.isDone()); dagIterPlugin.next())
      {
         if (CMayaScene::IsArnoldLight(dagIterPlugin.currentItem()))
         {
            if (dagIterPlugin.getPath(path))
            {
               // Only check for lights being visible, not templated and in render layer
               // FIXME: does a light need to be in layer to render actually in Maya?
               MFnDagNode node(path.node());
               MString name = node.name();
               if ((mask & MTOA_FILTER_LAYER) && !IsInRenderLayer(path))
                  continue;
               if ((mask & MTOA_FILTER_TEMPLATED) && IsTemplatedPath(path))
                  continue;
               if ((mask & MTOA_FILTER_HIDDEN) && !IsVisiblePath(path))
                  continue;
               if (ExportDagPath(path, true) == NULL)
                  status = MStatus::kFailure;
            }
            else
            {
               AiMsgError("[mtoa] Could not get path for Arnold plugin lights DAG iterator.");
               status = MS::kFailure;
            }
         }
      }

      // Now export light linker nodes
      MFnDependencyNode depFn;
      MItDependencyNodes depIterLinkers(MFn::kLightLink, &status);
      for (; (!depIterLinkers.isDone()); depIterLinkers.next())
      {
         MObject node = depIterLinkers.thisNode(&status);
         if (MStatus::kSuccess == status)
         {
            depFn.setObject(node);
            MPlug plug = depFn.findPlug("message");
            ExportNode(plug);
         }
         else
         {
            AiMsgError("[mtoa] Could not get node for Arnold plugin lights DAG iterator.");
         }
      }
   }

   // UpdateLightLinks refreshes global light linking info
   UpdateLightLinks();

   return status;
}

// Export the full maya scene or the passed selection
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CArnoldSession::ExportDag(MSelectionList* selected)
{
   MStatus status = MStatus::kSuccess;

   // If we got a selection list iterate it and export
   if (NULL != selected)
   {
      MItSelectionList it(*selected, MFn::kInvalid, &status);
      MDagPath path;
      MObject node;
      for (it.reset(); !it.isDone(); it.next())
      {
         if (it.getDagPath(path) == MStatus::kSuccess)
         {
            MStatus stat;
            ExportDagPath(path, true, &stat);
            if (stat != MStatus::kSuccess)
               status = MStatus::kFailure;
         }
         else
         {
            status = MStatus::kFailure;
         }
      }
   }
   else
   {
      // No selection export whole scene

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
            MStatus stat;
            ExportDagPath(path, true, &stat);
            if (stat != MStatus::kSuccess)
               status = MStatus::kFailure;
         }
         else
         {
            AiMsgError("[mtoa] Could not get path for Maya DAG iterator.");
            status = MStatus::kInvalidParameter;
         }
      }
   }
   
   return status;
}


// Filter and expand the selection, and all its hirarchy down stream.
// Also flattens sets in selection.
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CArnoldSession::FlattenSelection(MSelectionList* selected)
{
   MStatus status;

   MObject node;
   MDagPath path;
   MFnDagNode dgNode;
   MFnSet set;
   MSelectionList children;
   // loop users selection
   MItSelectionList it(*selected, MFn::kInvalid, &status);
   selected->clear();
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
                  selected->add (path, MObject::kNullObj, true);
               path.pop(1);
               if (MStatus::kSuccess != FlattenSelection(&children))
                  status = MStatus::kFailure;
               selected->merge(children);
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
            if (MStatus::kSuccess != FlattenSelection(&children))
               status = MStatus::kFailure;
            selected->merge(children);
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

/// Determine if the DAG node should be skipped.
/// The MDGContext defines the frame at which to test visibility
DagFiltered CArnoldSession::FilteredStatus(const MDagPath &path, const CMayaExportFilter *filter) const
{
   if (NULL == filter) filter = &GetExportFilter();
   // Tests that cause the whole branch to be pruned
   unsigned int mask = filter->state_mask;
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
   MFnTypeSet::const_iterator sit(filter->excluded.begin()), send(filter->excluded.end());
   for(; sit!=send;++sit)
      if (obj.hasFn(*sit))
         return MTOA_EXPORT_REJECTED_NODE;

   return MTOA_EXPORT_ACCEPTED;
}

void CArnoldSession::ExportLightLinking(AtNode* shape, const MDagPath& path)
{
   m_arnoldLightLinks.ExportLightLinking(shape, path);
}

// updates
void CArnoldSession::QueueForUpdate(const CNodeAttrHandle & handle)
{
   m_objectsToUpdate.push_back(ObjectToTranslatorPair(handle, (CNodeTranslator*)NULL));
}

void CArnoldSession::QueueForUpdate(CNodeTranslator * translator)
{
   m_objectsToUpdate.push_back(ObjectToTranslatorPair(translator->GetMayaHandle(), translator));
}

void CArnoldSession::RequestUpdate()
{
   m_requestUpdate = true;
   // Loads the IPRIdleCallback
   CMayaScene::UpdateIPR();
}

void CArnoldSession::DoUpdate()
{
   MStatus status;
   assert(AiUniverseIsActive());

   std::vector< CNodeTranslator * > translatorsToUpdate;
   std::vector<ObjectToTranslatorPair>::iterator itObj;
   bool aDag   = false;
   bool newDag = false;
   bool reqMob = false;
   bool moBlur = IsMotionBlurEnabled();
   for (itObj = m_objectsToUpdate.begin(); itObj != m_objectsToUpdate.end(); itObj++)
   {
      CNodeAttrHandle handle(itObj->first);           // TODO : test isValid and isAlive ?
      CNodeTranslator * translator = itObj->second;
      if (translator != NULL && translator->m_updateMode != AI_RECREATE_NODE)
      {
         // A translator was provided, just add it to the list
         if(translator->m_updateMode == AI_DELETE_NODE)
         {
            translator->RemoveUpdateCallbacks();
            translator->Delete();
            m_processedTranslators.erase(handle);
         }
         else
         {
            if (moBlur) reqMob = reqMob || translator->RequiresMotionData();
            if (translator->IsMayaTypeDag()) aDag = true;
            translatorsToUpdate.push_back(translator);
         }
      }
      else if(translator != NULL && translator->m_updateMode == AI_RECREATE_NODE)
      {
         translator->Delete();
         translator->m_atNodes.clear();
         translator->DoCreateArnoldNodes();

         translator->DoExport(0);
         translatorsToUpdate.push_back(translator);
      }
      else
      {
         // No translator was provided, it's either a new node creation or
         // the undo of a delete node
         MObject node = handle.object();
         MString name = MFnDependencyNode(node).name();
         std::vector< CNodeTranslator * > translators;
         if (GetActiveTranslators(handle, translators))
         {
            // Restored node, we have the translator(s) already
            AiMsgDebug("[mtoa] Updating restored node translators: %s", name.asChar());
         }
         else
         {
            // New node, dag node or dependency node?
            MFnDagNode dagNodeFn(node);
            MDagPath path;
            status = dagNodeFn.getPath(path);
            if (status == MS::kSuccess)
            {
               // This is a Dag node, is it instanced ?
               int instanceNum = handle.instanceNum();
               if (instanceNum >= 0)
               {
                  MDagPathArray allPaths;
                  dagNodeFn.getAllPaths(allPaths);
                  if (instanceNum < (int)allPaths.length())
                  {
                     path = allPaths[instanceNum];
                  }
               }
               // Just export it then
               ExportDagPath(path, true, &status);
               if (MStatus::kSuccess == status)
               {
                  name = path.partialPathName();
                  AiMsgDebug("[mtoa] Exported new node: %s", name.asChar());
                  newDag = true;
                  // Then queue newly created translators to the list
                  GetActiveTranslators(handle, translators);
               }
            }
            // Dependency nodes are not exported by themselves, their export
            // will be requested if they're connected to an exported node
         }
         // Add the newly recovered or created translators to the list
         for (unsigned int i=0; i < translators.size(); ++i)
         {
            if (moBlur) reqMob = reqMob || translators[i]->RequiresMotionData();
            if (translators[i]->IsMayaTypeDag()) aDag = true;
            translators[i]->DoExport(0);
            translatorsToUpdate.push_back(translators[i]);
         }
      }
   }
   // FIXME: n
   if (newDag || IsLightLinksDirty())
   {
      UpdateLightLinks();
   }

   // Need something finer to determine if the changes have an influence
   if (aDag)
   {
      AiUniverseCacheFlush(AI_CACHE_SSS & AI_CACHE_HAIR_DIFFUSE);
   }
   // Now do an update for all the translators in our list
   // TODO : we'll probably need to be able to passe precisely to each
   // translator what event or plug triggered the update request

   if (!reqMob)
   {
      for (std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
         iter != translatorsToUpdate.end(); ++iter)
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
         for (std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
             iter != translatorsToUpdate.end(); ++iter)
         {
            CNodeTranslator* translator = (*iter);
            if (translator != NULL) translator->DoUpdate(step);
         }
      }
      MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));

      m_isExportingMotion = false;
   }

   // Refresh translator callbacks after all is done
   if (GetSessionMode() == MTOA_SESSION_IPR)
   {
      // re-add IPR callbacks to all updated translators after ALL updates are done
      for(std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
         iter != translatorsToUpdate.end(); ++iter)
      {
         CNodeTranslator* translator = (*iter);
         if (translator != NULL)
         {
            translator->RemoveUpdateCallbacks();
            translator->AddUpdateCallbacks();
         }
      }
   }

   // Clear the list and the request update flag.
   translatorsToUpdate.clear();
   m_objectsToUpdate.clear();
   m_requestUpdate = false;
}

void CArnoldSession::ClearUpdateCallbacks()
{
   // Clear the list of translators to update.
   m_objectsToUpdate.clear();

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

