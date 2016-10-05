#include "platform/Platform.h"
#include "ArnoldSession.h"
#include "attributes/Components.h"
#include "extension/ExtensionsManager.h"
#include "scene/MayaScene.h"
#include "translators/options/OptionsTranslator.h"
#include "translators/camera/ImagePlaneTranslator.h"
#include "translators/shader/ShaderTranslators.h"
#include "translators/shader/ShadingEngineTranslator.h"
#include "nodes/ShaderUtils.h"
#include "translators/DagTranslator.h"
#include "translators/NodeTranslatorImpl.h"
#include "utils/MakeTx.h"

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
#include <maya/MNodeMessage.h>
#include <maya/MProgressWindow.h>

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
CDagTranslator* CArnoldSession::ExportDagPath(const MDagPath &dagPath, bool initOnly, MStatus* stat)
{
   //m_motionStep = 0;
   MStatus status = MStatus::kSuccess;
   AtNode* arnoldNode = NULL;

   MString name = CDagTranslator::GetArnoldNaming(dagPath);
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
   else if (!translator->m_impl->IsMayaTypeDag())
   {
      if (stat != NULL) *stat = MStatus::kInvalidParameter;
      AiMsgDebug("[mtoa] translator for %s of type %s is not a DAG translator", name.asChar(), type.asChar());
      AiMsgTab(-1);
      return NULL;
   }

   AiMsgDebug("[mtoa.session]     %-30s | Exporting DAG node of type %s", name.asChar(), type.asChar());

   CNodeAttrHandle handle(dagPath);
   // Check if node has already been processed
   // FIXME: since it's a multimap there can be more than one translator associated ?
   // ObjectToTranslatorMap::iterator it, itlo, itup;
   // itlo = m_processedTranslators.lower_bound(handle);
   // itup = m_processedTranslators.upper_bound(handle);
   ObjectToTranslatorMap::iterator it = m_processedTranslators.find(handle);
   if (it != m_processedTranslators.end())
   {
      AiMsgDebug("[mtoa.session]     %-30s | Reusing previous export of DAG node of type %s", name.asChar(), type.asChar());

      delete translator;
      status = MStatus::kSuccess;
      arnoldNode = it->second->GetArnoldNode();
      translator = (CDagTranslator*)it->second;
   }

   if (arnoldNode == NULL)
   {
      if (initOnly)
         AiMsgDebug("[mtoa.session]     %-30s | Initializing DAG node of type %s", name.asChar(), type.asChar());
      status = MStatus::kSuccess;
      translator->m_impl->Init(this, dagPath);
      if (it != m_processedTranslators.end())
      {
         it->second = translator;
      }
      else
      {
         m_processedTranslators.insert(ObjectToTranslatorPair(handle, translator));
         // This node handle might have already been added to the list of objects to update
         // but since no translator was found in m_processedTranslators, it might have been discarded
         // if we don't QueueForUpdate now, addUpdateCallbacks could not be called and we'd loose all callbacks
         // for this shader
         if (IsInteractiveRender()) QueueForUpdate(translator);
      }
      if (!initOnly)
         arnoldNode = translator->m_impl->DoExport();
   }

   if (NULL != stat) *stat = status;
   AiMsgTab(-1);
   return translator;
}

// Export a plug (dependency node output attribute)
//
CNodeTranslator* CArnoldSession::ExportNode(const MPlug& shaderOutputPlug, AtNodeSet* nodes, AOVSet* aovs,
                                   bool initOnly, int instanceNumber, MStatus *stat)
{
   //instanceNumber is currently used only for bump. We provide a specific instance number

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
   if (translator->m_impl->ResolveOutputPlug(resultPlug, resolvedPlug))
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
      handle.set(resultPlug, instanceNumber);
   else
      handle.set(mayaNode, "", instanceNumber);
   ObjectToTranslatorMap::iterator it = m_processedTranslators.end();
   
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
      arnoldNode = it->second->GetArnoldNode();
      translator = it->second;
   }
   
   if (arnoldNode == NULL)
   {
      if (initOnly)
         AiMsgDebug("[mtoa.session]     %-30s | Initializing node of type %s", name.asChar(), type.asChar());
      status = MStatus::kSuccess;
      translator->m_impl->SetShadersList(nodes);
      translator->m_impl->Init(this, mayaNode, resultPlug.partialName(false, false, false, false, false, true));
      if (it != m_processedTranslators.end())
      {
         it->second = translator;
      }
      else
      {
         m_processedTranslators.insert(ObjectToTranslatorPair(handle, translator));
         
         // This node handle might have already been added to the list of objects to update
         // but since no translator was found in m_processedTranslators, it might have been discarded
         // if we don't QueueForUpdate now, addUpdateCallbacks could not be called and we'd loose all callbacks
         // for this shader
         if (IsInteractiveRender()) QueueForUpdate(translator);
      }
      if (!initOnly)
         arnoldNode = translator->m_impl->DoExport();
   }
   if (arnoldNode != NULL)
   {
      if (nodes != NULL)
      {
         nodes->insert(translator->m_impl->m_atNode);

         if (translator->m_impl->m_additionalAtNodes)
         {
            AtMap<std::string, AtNode*>::iterator nodeIt;
            for (nodeIt = translator->m_impl->m_additionalAtNodes->begin(); nodeIt != translator->m_impl->m_additionalAtNodes->end(); ++nodeIt)
            {
               nodes->insert(nodeIt->second);
            }         
         }
      }
      if (aovs != NULL)
      {
         // only ShadingEngine doesn't TrackAOVs as it's the root of the shading tree
         translator->m_impl->TrackAOVs(aovs);
      }
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

   m_scaleFactor = options.GetScaleFactor();
   AtVector s = {static_cast<float>(m_scaleFactor), static_cast<float>(m_scaleFactor), static_cast<float>(m_scaleFactor)};
   AiM4Scaling(m_scaleFactorAtMatrix, &s);

   double sc[3] = {m_scaleFactor, m_scaleFactor, m_scaleFactor};
   m_scaleFactorMMatrix.setToIdentity();
   MTransformationMatrix trmat(m_scaleFactorMMatrix);
   trmat.setScale(sc, MSpace::kWorld);
   m_scaleFactorMMatrix = trmat.asMatrix();

   m_origin = options.GetOrigin();

   //ProcessAOVs();
   return status;
}

MStatus CArnoldSession::End()
{
   MStatus status = MStatus::kSuccess;

   m_requestUpdate = false;
   if (IsInteractiveRender())
   {
      ClearUpdateCallbacks();
   }
   else if (GetSessionMode() == MTOA_SESSION_ASS && MGlobal::mayaState() == MGlobal::kInteractive && IsMotionBlurEnabled())
   {
      // reset to export frame
      MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));
   }

   // Delete stored translators
   ObjectToTranslatorMap::iterator it = m_processedTranslators.begin();
   ObjectToTranslatorMap::iterator itEnd = m_processedTranslators.end();
   for ( ; it != itEnd; ++it)
   {
      delete it->second;
   }

   for(unsigned int i = 0; i < m_hiddenObjectsCallbacks.size(); ++i)
   {
      MNodeMessage::removeCallback(m_hiddenObjectsCallbacks[i].second);
   }
   m_hiddenObjectsCallbacks.clear();

   m_processedTranslators.clear();
   m_objectsToUpdate.clear();
   m_optionsTranslator = NULL;
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
         m_arnoldLightLinks.ParseLights();
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
      unsigned int range_type    = m_sessionOptions.m_motion.range_type;
      unsigned int motionSteps   = m_sessionOptions.m_motion.steps;
      m_motion_frames.clear();
      m_motion_frames.reserve(motionSteps);
      
      double motionFrames;
      double stepSize;
      double startFrame;
      double endFrame;
      
      switch(range_type)
      {
         case MTOA_MBLUR_TYPE_START:
            motionFrames = m_sessionOptions.m_motion.motion_frames;
            stepSize = motionFrames / double(motionSteps - 1);
            startFrame = exportFrame;
            endFrame = exportFrame + motionFrames;
            for (unsigned int step=0; (step < motionSteps - 1); ++step)
            {
               m_motion_frames.push_back(startFrame + (double)step * stepSize);
            }
            m_motion_frames.push_back(endFrame);
            break;
         case MTOA_MBLUR_TYPE_CENTER:
            motionFrames = m_sessionOptions.m_motion.motion_frames;
            stepSize = motionFrames / double(motionSteps - 1);
            startFrame = exportFrame - motionFrames * 0.5;
            endFrame = exportFrame + motionFrames * 0.5;
            for (unsigned int step=0; (step < motionSteps - 1); ++step)
            {
               if((motionSteps%2 == 1) && (step == ((motionSteps-1)/2)) )
                  m_motion_frames.push_back(exportFrame);
               else
                  m_motion_frames.push_back(startFrame + (double)step * stepSize);
            }
            m_motion_frames.push_back(endFrame);
            break;
         case MTOA_MBLUR_TYPE_END:
            motionFrames = m_sessionOptions.m_motion.motion_frames;
            stepSize = motionFrames / double(motionSteps - 1);
            startFrame = exportFrame - motionFrames;
            endFrame = exportFrame;
            m_motion_frames.push_back(startFrame);
            for (unsigned int step=1; (step < motionSteps - 1); ++step)
            {
               m_motion_frames.push_back(endFrame - (double)(motionSteps - 1 - step) * stepSize);
            }
            m_motion_frames.push_back(endFrame);
            break;
         case MTOA_MBLUR_TYPE_CUSTOM:
            startFrame = exportFrame + m_sessionOptions.m_motion.motion_start;
            endFrame = exportFrame + m_sessionOptions.m_motion.motion_end;
            motionFrames = endFrame - startFrame;
            stepSize = motionFrames / double(motionSteps - 1);
            for (unsigned int step=0; (step < motionSteps - 1); ++step)
            {
               m_motion_frames.push_back(startFrame + (double)step * stepSize);
            }
            m_motion_frames.push_back(endFrame);
            break;
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

   return m_optionsTranslator->GetArnoldNode();
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
      status = FlattenSelection(selected, false); // false = don't skip root nodes (ticket #1061)
      unsigned int fns = selected->length();
      AiMsgDebug("[mtoa] Exporting selection (%i:%i)", ns, fns);
   }
   else
   {
      AiMsgDebug("[mtoa] Exporting scene");
   }

   // Set up export options
   ArnoldSessionMode exportMode = m_sessionOptions.m_mode;
   m_motionStep = 0;

   // Are we motion blurred (any type)?
   const bool mb = IsMotionBlurEnabled();

   AiMsgDebug("[mtoa.session]     Initializing at frame %f", GetExportFrame());

   ExportOptions();  // inside loop so that we're on the proper frame

   // First "real" export
   MGlobal::viewFrame(m_sessionOptions.m_frame);
   if (exportMode == MTOA_SESSION_RENDER || exportMode == MTOA_SESSION_BATCH || 
      exportMode == MTOA_SESSION_IPR || exportMode == MTOA_SESSION_RENDERVIEW || exportMode == MTOA_SESSION_SEQUENCE)
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
      {
         // it used to be done at if(step ==1)
         // but we could possibly be changing the frame temporarily
         // without setting exportingMode=true
         // so it's better to do it here. This way we can block the NodeDirty signals as desired
         m_isExportingMotion = true; 
         MGlobal::viewFrame(MTime(m_motion_frames[step], MTime::uiUnit()));
      }
      AiMsgDebug("[mtoa.session]     Exporting step %d of %d at frame %f", step+1, numSteps, m_motion_frames[step]);
      
      // then, loop through the already processed dag translators and export for current step
      // NOTE: these exports are subject to the normal pre-processed checks which prevent redundant exports.
      // Since all nodes *should* be exported at this point, the following calls to DoExport do not
      // traverse the DG even if the translators call ExportNode or ExportDag. This makes it safe
      // to re-export all objects from a flattened list

      // The list of processedTranslators can grow while we call doExport a few lines below.
      // So we can't call doExport while iterating over them.
      // Thus we first store the list of translators to process.
      std::vector<CNodeTranslator*> translatorsToExport;
      translatorsToExport.reserve(m_processedTranslators.size());
      ObjectToTranslatorMap::iterator it = m_processedTranslators.begin();
      ObjectToTranslatorMap::iterator itEnd = m_processedTranslators.end();
      for (; it != itEnd; ++it)
      {
         if (it->second) translatorsToExport.push_back(it->second);
      }
      
      // for safety we're not doing the loop on m_motionSteps directly in case it is modified somewhere else
      m_motionStep = step; 

      // finally, loop through the already processed translators and export for current step
      for (size_t i=0; i < translatorsToExport.size(); ++i)
      {         
         translatorsToExport[i]->m_impl->DoExport();
      }
   }
   m_motionStep = 0;
   if (mb)
   {
      // Note: only reset frame during interactive renders, otherwise that's an extra unnecessary scene eval
      // when exporting a sequence.  Other modes are reset to the export frame in CArnoldSession::End().
      if (GetSessionMode() == MTOA_SESSION_RENDER || GetSessionMode() == MTOA_SESSION_IPR || GetSessionMode() == MTOA_SESSION_RENDERVIEW)
      {
         MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));
      }
   }

   m_isExportingMotion = false;

   // add callbacks after all is done
   if (IsInteractiveRender())
   {
      ObjectToTranslatorMap::iterator it = m_processedTranslators.begin();
      ObjectToTranslatorMap::iterator itEnd = m_processedTranslators.end();
      for ( ; it != itEnd; ++it)
      {
         CNodeTranslator *nodeTr = it->second;
         if (nodeTr == NULL) continue;
         nodeTr->AddUpdateCallbacks();
         nodeTr->m_impl->m_updateMode = CNodeTranslator::AI_UPDATE_ONLY;
         nodeTr->m_impl->m_holdUpdates = false; // allow to trigger new updates
         nodeTr->m_impl->m_isExported = true; // next export will be an update
         // for motion blur, check which nodes are static and which aren't (#2316)
         if (mb && numSteps > 1)
         {
            nodeTr->m_impl->m_animArrays = nodeTr->m_impl->HasAnimatedArrays();
         } else nodeTr->m_impl->m_animArrays = false;
      }
      m_objectsToUpdate.clear(); // I finished exporting, I don't have any other object to Update now
   }

   // it would seem correct to only call ExportTxFiles if m_updateTx = true
   // but it's not a good moment to take that risk...
   ExportTxFiles();

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

      MFnDagNode cameraNode;
      MPlug renderable;
      // First we export all cameras
      // We do not reset the iterator to avoid getting kWorld
      for (; (!dagIterCameras.isDone()); dagIterCameras.next())
      {
         if (dagIterCameras.getPath(path))
         {
            
            MStatus stat;
            cameraNode.setObject(path);
            renderable = cameraNode.findPlug("renderable", false, &stat);
            if (stat == MS::kSuccess && renderable.asBool())
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

// This callback is invoked when one of the skipped (hidden) objects is modified 
// during an IPR session. We have to check if it became visible in order to export it
void CArnoldSession::HiddenNodeCallback(MObject& node, MPlug& plug, void* clientData)
{
   // just check if this node is visible now 
   MFnDagNode dagNode(node);
   MDagPath path;
   if (dagNode.getPath(path) != MS::kSuccess) return;

   if(!path.isValid()) return;

   CArnoldSession *session = (CArnoldSession*)clientData;
   DagFiltered filtered = session->FilteredStatus(path);
   if (filtered != MTOA_EXPORT_ACCEPTED) return; // this object is still hidden

   session->SetDagVisible(path);
}

void CArnoldSession::SetDagVisible(MDagPath &path)
{
   MObject object = path.node();
   // we need to clear the existing hiddenObjectCallback
   // starting with a simple linear search
   for(size_t i = 0; i < m_hiddenObjectsCallbacks.size(); ++i)
   {
      if (m_hiddenObjectsCallbacks[i].first.object() == object)
      {
         // found the object in our hidden list         
         const MStatus status = MNodeMessage::removeCallback(m_hiddenObjectsCallbacks[i].second);
         if (status == MS::kSuccess) 
         {
            m_hiddenObjectsCallbacks.erase(m_hiddenObjectsCallbacks.begin() + i);
            break;
         }
      }
   }

   MItDag   dagIterator(MItDag::kDepthFirst, MFn::kInvalid);
   MStatus status;
   bool pruneDag = false;
   MDagPath parentPruneDag;

   for (dagIterator.reset(path); (!dagIterator.isDone()); dagIterator.next())
   {
      if (dagIterator.getPath(path))
      {
         if (path.apiType() == MFn::kWorld)
            continue;

         if (pruneDag)
         {
            MDagPath tmpPath(path);
            tmpPath.pop();
            if (tmpPath == parentPruneDag)
            {
               dagIterator.prune();
               continue;
            }
            pruneDag = false;
         }

         MObject obj = path.node();
         MFnDagNode node(obj);
         MString name = node.name();
         DagFiltered filtered = FilteredStatus(path);
         if (filtered != MTOA_EXPORT_ACCEPTED)
         {
            if (IsInteractiveRender())
            {
               HiddenObjectCallbackPair hiddenObj;
               hiddenObj.first = CNodeAttrHandle(obj, "");
               hiddenObj.second = MNodeMessage::addNodeDirtyCallback(obj,
                                        HiddenNodeCallback,
                                        this,
                                        &status);
               m_hiddenObjectsCallbacks.push_back(hiddenObj);
            }
            // Ignore node for MTOA_EXPORT_REJECTED_NODE or whole branch
            // for MTOA_EXPORT_REJECTED_BRANCH
            if (filtered == MTOA_EXPORT_REJECTED_BRANCH)
               dagIterator.prune();
            continue;
         }
         MStatus stat;
         CDagTranslator *tr = ExportDagPath(path, true, &stat);
         QueueForUpdate(path);
         if (stat != MStatus::kSuccess)
            status = MStatus::kFailure;

         if (tr != NULL && !tr->ExportDagChildren())
         {
            pruneDag = true;
            parentPruneDag = path;
            parentPruneDag.pop();
            dagIterator.prune();
         }
      }
   }

   RequestUpdate();
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

      bool pruneDag = false;
      MDagPath parentPruneDag;

      for (dagIterator.reset(); (!dagIterator.isDone()); dagIterator.next())
      {
         if (dagIterator.getPath(path))
         {
            if (path.apiType() == MFn::kWorld)
               continue;

            if (pruneDag)
            {
               MDagPath tmpPath(path);
               tmpPath.pop();
               if (tmpPath == parentPruneDag)
               {
                  dagIterator.prune();
                  continue;
               }
               pruneDag = false;
            }
            MObject obj = path.node();
            MFnDagNode node(obj);
            MString name = node.name();
            filtered = FilteredStatus(path);
            if (filtered != MTOA_EXPORT_ACCEPTED)
            {
               if (IsInteractiveRender())
               {
                  HiddenObjectCallbackPair hiddenObj;
                  hiddenObj.first = CNodeAttrHandle(obj, "");
                  hiddenObj.second = MNodeMessage::addNodeDirtyCallback(obj,
                                           HiddenNodeCallback,
                                           this,
                                           &status);
                  m_hiddenObjectsCallbacks.push_back(hiddenObj);
               }
               // Ignore node for MTOA_EXPORT_REJECTED_NODE or whole branch
               // for MTOA_EXPORT_REJECTED_BRANCH
               if (filtered == MTOA_EXPORT_REJECTED_BRANCH)
                  dagIterator.prune();
               continue;
            }
            MStatus stat;
            CDagTranslator *tr = ExportDagPath(path, true, &stat);
            if (stat != MStatus::kSuccess)
               status = MStatus::kFailure;

            if (tr != NULL && !tr->ExportDagChildren())
            {
               pruneDag = true;
               parentPruneDag = path;
               parentPruneDag.pop();
               dagIterator.prune();
            }
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
MStatus CArnoldSession::FlattenSelection(MSelectionList* selected, bool skipRoot)
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
            // add this path, unless skipRoot is true
            if (!skipRoot) selected->add(path, MObject::kNullObj, true);

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
               if (MStatus::kSuccess != FlattenSelection(&children, true)) // true = skipRoot
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
            if (MStatus::kSuccess != FlattenSelection(&children, true)) // true = skipRoot
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
   if (m_isExportingMotion && IsInteractiveRender()) return;
   m_objectsToUpdate.push_back(ObjectToTranslatorPair(handle, (CNodeTranslator*)NULL));
}

void CArnoldSession::EraseActiveTranslator(const CNodeAttrHandle &handle)
{
   m_processedTranslators.erase(handle);
}

void CArnoldSession::QueueForUpdate(CNodeTranslator * translator)
{
   // don't add translator twice to the list, it could crash if its updateMode
   // is "delete". Other solution would be to use a set, but the extra-cost is not
   // necessary since we already have this flag
   if (translator == NULL || translator->m_impl->m_holdUpdates) return; 

   // During IPR with motion blur, we change the current frame to export the motion 
   // and this might propagate nodeDirty signals that end up here. 
   // We don't want to consider those
   if (m_isExportingMotion && IsInteractiveRender()) return;

   // Set this translator as being in the update list, to avoid useless future signals
   translator->m_impl->m_holdUpdates = true;   

   // add this translator to the list of objects to be updated in next DoUpdate()
   m_objectsToUpdate.push_back(ObjectToTranslatorPair(translator->m_impl->m_handle, translator));
}

void CArnoldSession::RequestUpdate()
{
   //if (!forceUpdate && !m_continuousUpdates) return;

   m_requestUpdate = true;
   CMayaScene::UpdateIPR();
}

// During nodes export in DoUpdate(), some new translators can be created by the export of other ones 
//( e.g. connections in the shading tree). So once update is finished, we might get a new list of
// objects to update. In that case we'll invoke DoUpdate() recursively, but we don't want it to end up in an infinite loop
// so we set an arbitrary maximum amount of updates
static int s_recursiveUpdates = 0;
static const int s_maxRecursiveUpdates = 5;


void CArnoldSession::DoUpdate()
{
   s_recursiveUpdates++;

   MStatus status;
   assert(AiUniverseIsActive());

   if (m_updateMotionData)
   {  
      std::vector<bool> prevRequiresMotion;
      prevRequiresMotion.reserve(m_processedTranslators.size());

      // stores requiresMotionData from all translators
      ObjectToTranslatorMap::iterator it = m_processedTranslators.begin();
      ObjectToTranslatorMap::iterator itEnd = m_processedTranslators.end();
      for ( ; it != itEnd; ++it)
         prevRequiresMotion.push_back(it->second->RequiresMotionData());

      // this will update the motion blur settings in SessionOptions
      // which is necessary for RequiresMotionData
      m_sessionOptions.GetFromMaya();

      // check again all translators
      int trIdx = 0;
      for (it = m_processedTranslators.begin() ; it != itEnd; ++it, ++trIdx)
      {
         if (prevRequiresMotion[trIdx])
         {
            // This translator used to be motion blurred
            if (!it->second->RequiresMotionData())
            {
               // now it's not motion blurred anymore.
               // Just need to re-export it
               it->second->SetUpdateMode(CNodeTranslator::AI_UPDATE_ONLY);
               it->second->RequestUpdate();
            } else if (it->second->m_impl->m_animArrays)
            {
               // Now it is sill motion blurred, and its arrays were animated
               // need to recreated the node since the motion steps might have changed
               it->second->SetUpdateMode(CNodeTranslator::AI_RECREATE_NODE);
               it->second->RequestUpdate();               
            } // otherwise this node was exported with motion, but its arrays were found to be static, so we don't update it

         } else
         {
            // this node didn't require motion blur before.
            if (it->second->RequiresMotionData())
            {
               // it now requires motion blur ! need to re-generate the node 
               it->second->SetUpdateMode(CNodeTranslator::AI_RECREATE_NODE);
               it->second->RequestUpdate();                 
            } // otherwise, this node shouldn't be updated at all 
         }
      }
      UpdateMotionFrames();
      m_updateMotionData = false;
   }

   if (m_updateOptions)
   {
      CRenderSession *renderSession = CMayaScene::GetRenderSession();
      renderSession->UpdateRenderOptions();
      m_updateOptions = false;
   }


   double frame = MAnimControl::currentTime().as(MTime::uiUnit());
   bool frameChanged = (frame != GetExportFrame());

   // only change the frame for interactive renders
   // It appears that Export() doesn't restore the current frame
   // in maya otherwise ( to avoid useless maya evaluations )
   if (frameChanged && IsInteractiveRender()) SetExportFrame(frame);

   // hack to support deleting procedurals
   // we need to force arnold to re-generate 
   // eventual connections from one procedural to another
   if (!m_proceduralsToUpdate.empty())
      UpdateProceduralReferences();

   std::vector< CNodeTranslator * > translatorsToUpdate;
   std::vector<ObjectToTranslatorPair>::iterator itObj;
   std::vector<CNodeAttrHandle> newToUpdate;
   
   bool dagFound   = false;
   bool newDag = false;
   bool exportMotion = false;
   bool motionBlur = IsMotionBlurEnabled();
   bool mbRequiresFrameChange = false;

   m_motionStep = 0;

   // In theory, no objectsToUpdate are supposed to be 
   // added to this list during the loop. But to make 
   // sure this won't be done by any of the functions 
   // we'll be invoking here it's safer to loop 
   // with the vector's index instead of relying on iterators...
   for (size_t i = 0; i < m_objectsToUpdate.size(); ++i)
   {
      CNodeAttrHandle handle(m_objectsToUpdate[i].first);           // TODO : test isValid and isAlive ?
      CNodeTranslator * translator = m_objectsToUpdate[i].second;

      // Check if this translator needs to be re-created
      if (translator != NULL && translator->m_impl->m_updateMode == CNodeTranslator::AI_RECREATE_TRANSLATOR)
      {
         // delete the current translator, just like AI_DELETE_NODE does
         translator->Delete();
         m_processedTranslators.erase(handle); 
         
         // we're now deleting this transator, this was never done...make sure it doesn't introduce issues
         delete translator;
         // callbacks are now removed in the destructor
         //translator->m_impl->RemoveUpdateCallbacks();

         // setting translator to NULL will consider that this is a new node,
         // re-create the translator and export it appropriately
         translator = NULL; 
         m_objectsToUpdate[i].second = NULL; // safety
      }

      if (translator != NULL)
      {
         // Translator already exists
         // check its update mode
         if(translator->m_impl->m_updateMode == CNodeTranslator::AI_RECREATE_NODE)
         {
            // to be updated properly, the Arnold node must 
            // be deleted and re-exported            
            translator->Delete();
            translator->m_impl->DoCreateArnoldNodes();
            
            // no longer re-exporting here. This will be done in the translatorsToUpdateList
            // since DoUpdate will call DoExport (isExported=false)
            //translator->m_impl->DoExport();
            translatorsToUpdate.push_back(translator);

            if (motionBlur && translator->RequiresMotionData())
            {
               // this node needs to be export with motion
               exportMotion = true;
               mbRequiresFrameChange = true;
            }

         } else if(translator->m_impl->m_updateMode == CNodeTranslator::AI_DELETE_NODE)
         {
            translator->Delete();
            // the translator has already been removed from our list
            //m_processedTranslators.erase(handle);

            // we're now deleting this transator, this was never done...make sure it doesn't introduce issues
            delete translator;
            // removing callbacks now handled in the destructor
            //translator->m_impl->RemoveUpdateCallbacks();
         }  
         else
         {  
            // AI_UPDATE_ONLY => simple update
            if (motionBlur && (!(exportMotion && mbRequiresFrameChange)) && translator->RequiresMotionData())
            {
               // Find out if we need to call ExportMotion for each motion step
               // or if a single Export is enough. 
               exportMotion = true;

               // If the arnold node doesn't have any animated array then there's no need 
               // to change the view frame in maya during the ExportMotion calls.
               // However if the frame has just been changed, then the arrays might have become 
               // animated now.
               if (frameChanged || translator->m_impl->m_animArrays) 
                  mbRequiresFrameChange = true;
            }
            if (translator->m_impl->IsMayaTypeDag()) dagFound = true;
            translatorsToUpdate.push_back(translator);
         }
      } else
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
            else // If new node is a dependency node, we will register its
                 //  update callbacks later if it correctly exported.
            {
               newToUpdate.push_back(handle);
            }
            // Dependency nodes are not exported by themselves, their export
            // will be requested if they're connected to an exported node
         }
         // Add the newly recovered or created translators to the list
         for (unsigned int i=0; i < translators.size(); ++i)
         {
            if (motionBlur && (!(exportMotion && mbRequiresFrameChange)) && translators[i]->RequiresMotionData())
            {
               // Find out if we need to call ExportMotion for each motion step
               // or if a single Export is enough. 
               exportMotion = true;

               // If the arnold node doesn't have any animated array then there's no need 
               // to change the view frame in maya during the ExportMotion calls.
               // However if the frame has just been changed, then the arrays might have become 
               // animated now.
               if (frameChanged || translators[i]->m_impl->m_animArrays) 
                  mbRequiresFrameChange = true;
            }

            if (translators[i]->m_impl->IsMayaTypeDag()) dagFound = true;

            // we no longer need to call DoExport here as DoUpdate will call it (isExported=false)
            //translators[i]->m_impl->DoExport();
            translatorsToUpdate.push_back(translators[i]);
         }
      }
   }

   // store the amount of updated objects as this list can increase during the actual updates
   // (e.g. when a new node is added to a shading tree)
   size_t updatedObjects = m_objectsToUpdate.size();
   
   // FIXME: n
         
   if (newDag || IsLightLinksDirty())
   {
      UpdateLightLinks();
   }

   // Need something finer to determine if the changes have an influence
   if (dagFound)
   {
      AiUniverseCacheFlush(AI_CACHE_HAIR_DIFFUSE);
   }
   // Now do an update for all the translators in our list
   if (!exportMotion)
   {
      for (std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
         iter != translatorsToUpdate.end(); ++iter)
      {
         CNodeTranslator* translator = (*iter);
         if (translator != NULL) translator->m_impl->DoUpdate();
      }
   }
   else
   {
      m_isExportingMotion = true;
      // Scene is motion blured, get the data for the steps.
      unsigned int numSteps = GetNumMotionSteps();
      for (unsigned int step = 0; step < numSteps; ++step)
      {
         AiMsgDebug("[mtoa.session]     Updating step %d at frame %f", step, m_motion_frames[step]);

         if (mbRequiresFrameChange) 
            MGlobal::viewFrame(MTime(m_motion_frames[step], MTime::uiUnit()));

         m_motionStep = step;
         for (std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
             iter != translatorsToUpdate.end(); ++iter)
         {
            CNodeTranslator* translator = (*iter);
            if (translator != NULL) translator->m_impl->DoUpdate();
            
            if (numSteps > 1 && step == numSteps - 1)
            {
               // last motion blur step, check once again if this translators has animated arrays
               translator->m_impl->m_animArrays = translator->m_impl->HasAnimatedArrays();
            }
         }
      }
      m_motionStep = 0;
      if (mbRequiresFrameChange)
      {
         MGlobal::viewFrame(MTime(GetExportFrame(), MTime::uiUnit()));
      }

      m_isExportingMotion = false;
   }
   

   if (m_updateTx) 
   {
      m_updateTx = false;
      ExportTxFiles();
   }

   // Refresh translator callbacks after all is done
   if (IsInteractiveRender())
   {
      for (std::vector<CNodeAttrHandle>::iterator iter = newToUpdate.begin();
         iter != newToUpdate.end(); ++iter)
      {
         CNodeAttrHandle handle = (*iter);
         ObjectToTranslatorMap::iterator it = m_processedTranslators.end();
         it = m_processedTranslators.find(handle);
         if(it != m_processedTranslators.end())
         {
            translatorsToUpdate.push_back(it->second);
         }
      }

      // re-add IPR callbacks to all updated translators after ALL updates are done
      for(std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
         iter != translatorsToUpdate.end(); ++iter)
      {
         CNodeTranslator* translator = (*iter);
         if (translator != NULL)
         {
            // For RenderView, we don't clear the update callbacks
            // we just add them if they're missing
            if (translator->m_impl->m_mayaCallbackIDs.length() == 0)
            {
               translator->AddUpdateCallbacks();
            } 
            translator->m_impl->m_holdUpdates = false; // I'm allowed to receive updates once again
            translator->m_impl->m_isExported = true;
            // restore the update mode to "update Only"
            translator->m_impl->m_updateMode = CNodeTranslator::AI_UPDATE_ONLY;
         }
      }
   }

   // Clear the list and the request update flag.
   translatorsToUpdate.clear();

   if (m_objectsToUpdate.size() > updatedObjects)
   {
      // some nodes have been added to the update list.
      // let's keep them in this list so that next update invokes them
      m_objectsToUpdate.erase(m_objectsToUpdate.begin(), m_objectsToUpdate.begin() + updatedObjects);

      // all the remaining objects to update must have holdUpdates On
      for (size_t i = 0; i < m_objectsToUpdate.size(); ++i)
      {
         CNodeTranslator *createdTranslator = m_objectsToUpdate[i].second;
         if (createdTranslator)
         {
            createdTranslator->m_impl->m_holdUpdates = true;
            // this new translator might not have its callbacks yet
            if (createdTranslator->m_impl->m_mayaCallbackIDs.length() == 0)
            {
               createdTranslator->AddUpdateCallbacks();
            }
         }
      }

      // During nodes export in DoUpdate(), some new translators can be created by the export of other ones 
      //( e.g. connections in the shading tree). So once update is finished, we might get a new list of
      // objects to update. In that case we'll invoke DoUpdate() recursively, but we don't want it to end up in an infinite loop
      // so we set an arbitrary maximum amount of updates
      if (s_recursiveUpdates < s_maxRecursiveUpdates)
         DoUpdate();
      else
      {
         AiMsgError("[mtoa.ipr] Recursive updates during IPR");
      }
   } else 
   {
      m_objectsToUpdate.clear();
      m_requestUpdate = false;
   } 
   s_recursiveUpdates = 0;    
}

void CArnoldSession::ClearUpdateCallbacks()
{
   // Clear the list of translators to update.
   m_objectsToUpdate.clear();

   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
   {	   
      if (it->second != NULL) it->second->m_impl->RemoveUpdateCallbacks();
   }
}

/// Set the camera to export.

/// If called prior to export, only the specified camera will be exported. If not set, all cameras
/// will be exported, but some translators may not be able to fully export without an export camera specified.
/// To address this potential issue, this method should be called after a multi-cam export, as it will cause
/// the options translator to be updated
///
void CArnoldSession::SetExportCamera(MDagPath camera)
{
   AiMsgDebug("[mtoa.session] Setting export camera to \"%s\"", camera.partialPathName().asChar());
   m_sessionOptions.SetExportCamera(camera);

   if (m_optionsTranslator == NULL) return;
   // just queue the options translator now 
   // instead of relying on the DependsOnExportCamera.
   // In the future we should have a generic way to make translators dependent from others,
   // so that whatever change in one translator propagates an update on the others
   QueueForUpdate(m_optionsTranslator);
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

void CArnoldSession::FormatTexturePath(MString& texturePath)
{
    m_sessionOptions.FormatTexturePath(texturePath);
}

void CArnoldSession::FormatProceduralPath(MString& proceduralPath)
{
    m_sessionOptions.FormatProceduralPath(proceduralPath);
}

MMatrix& CArnoldSession::ScaleMatrix(MMatrix& matrix) const
{
   matrix *= m_scaleFactorMMatrix;
   return matrix;
}

AtMatrix& CArnoldSession::ScaleMatrix(AtMatrix& matrix) const
{
   AiM4Mult(matrix, m_scaleFactorAtMatrix, matrix);
   return matrix;
}

float& CArnoldSession::ScaleDistance(float& distance) const
{
   double s = static_cast<double>(distance);
   s *= m_scaleFactor;
   distance = static_cast<float>(s);
   return distance;
}

double& CArnoldSession::ScaleDistance(double& distance) const
{
   distance *= m_scaleFactor;
   return distance;
}


float& CArnoldSession::ScaleArea(float& area) const
{
   double s = static_cast<double>(area);
   s *= m_scaleFactor;
   s *= m_scaleFactor;
   area = static_cast<float>(s);
   return area;
}

float& CArnoldSession::ScaleLightExposure(float& exposure) const
{
   double e = static_cast<double>(exposure);
   e += log(m_scaleFactor * m_scaleFactor) / log(2.0);
   exposure = static_cast<float>(e);
   return exposure;
}

MVector CArnoldSession::GetOrigin() const
{
   return m_origin;
}

MString CArnoldSession::GetMayaObjectName(const AtNode *node) const
{   
   // first check if an object exists with the same name ?
   const char *arnoldName = AiNodeGetName(node);

   MSelectionList camList;
   camList.add(MString(arnoldName));
   MObject mayaObject;
   if (camList.getDependNode(0, mayaObject) == MS::kSuccess && !mayaObject.isNull())
   {
      // There is an object with the same name in Maya.
      // We're assuming it's this one....
      return MString(arnoldName);
   }


   // There is no object with this name in the scene.
   // Let's search it amongst the list of processed translators
   ObjectToTranslatorMap::const_iterator it = m_processedTranslators.begin();
   ObjectToTranslatorMap::const_iterator itEnd = m_processedTranslators.end();
   for ( ; it != itEnd; ++it)
   {
      CNodeTranslator *translator = it->second;
      if (translator == NULL) continue;

      // check if this translator corresponds to this AtNode
      // FIXME : should we check for all of the possible AtNodes corresponding to this translator ?
      if (translator->GetArnoldNode() == node)
      {
         // We found our translator
         return translator->GetMayaNodeName().asChar();
      }
   }

   return "";
}
const char *CArnoldSession::GetArnoldObjectName(const MString &mayaName) const
{
   AtNode* node = AiNodeLookUpByName(mayaName.asChar());

   if (node == NULL)
   {
      // There is no object with this name in the scene.
      // Let's search it amongst the list of processed translators

      ObjectToTranslatorMap::const_iterator it = m_processedTranslators.begin();
      ObjectToTranslatorMap::const_iterator itEnd = m_processedTranslators.end();
      for ( ; it != itEnd; ++it)
      {
         CNodeTranslator *translator = it->second;
         if (translator == NULL) continue;

         // check if this translator corresponds to this AtNode
         // FIXME : should we check for all of the possible AtNodes corresponding to this translator ?
         if (translator->GetMayaNodeName() == mayaName)
         {
            // We found our translator
            node = translator->GetArnoldNode();
         }
      }
   }

   if (node) return AiNodeGetName(node);   

   return "";
}


bool CArnoldSession::IsVisible(MFnDagNode &node) const
{
   MStatus status;

   if (node.isIntermediateObject())
      return false;

   // The material view objects in Maya has always visibility disabled
   // to not show up by default in the scenes. So we need to override
   // that here and always return true for objects in material view session
   if (GetSessionMode() ==  MTOA_SESSION_MATERIALVIEW)
      return true;

   MPlug visPlug = node.findPlug("visibility", &status);
   // Check standard visibility
   if (status == MStatus::kFailure || !visPlug.asBool())
      return false;


   // FIXME do we really want lodVisibility to affect the visibility in arnold ?
   // Maya viewport seems indeed to take it into account. Note that this is used
   // only by Mesh lights
   MPlug lodVisPlug = node.findPlug("lodVisibility", &status);
   if (status == MStatus::kFailure || !lodVisPlug.asBool())
      return false;

   // Check override visibility
   MPlug overPlug = node.findPlug("overrideEnabled", &status);
   if (status == MStatus::kSuccess && overPlug.asBool())
   {
      MPlug overVisPlug = node.findPlug("overrideVisibility", &status);
      if (status == MStatus::kFailure || !overVisPlug.asBool())
         return false;
   }
   return true;
}

bool CArnoldSession::IsVisiblePath(MDagPath dagPath) const
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

const MStringArray &CArnoldSession::GetTextureSearchPaths() const
{
   return m_sessionOptions.GetTextureSearchPaths();   
}
const MStringArray &CArnoldSession::GetProceduralSearchPaths() const
{
   return m_sessionOptions.GetProceduralSearchPaths();
}


void CArnoldSession::ExportTxFiles()
{

   // Do not call makeTx if we're doing swatch rendering or material view
   int sessionMode = GetSessionMode();
   if (sessionMode == MTOA_SESSION_MATERIALVIEW || sessionMode == MTOA_SESSION_SWATCH ||
      sessionMode == MTOA_SESSION_UNDEFINED) return;
   
   CRenderOptions *renderOptions = CMayaScene::GetRenderSession()->RenderOptions();
   renderOptions->GetFromMaya(); 

   bool autoTx = renderOptions->autoTx();
   bool useTx = renderOptions->useExistingTiledTextures();

   if (useTx == false && autoTx == false) return;

   const MStringArray &searchPaths = GetTextureSearchPaths();

   bool progressBar = autoTx && (MGlobal::mayaState() == MGlobal::kInteractive);

   std::vector<CNodeTranslator *> textureNodes;
   textureNodes.reserve(100); // completely empirical value, to avoid first allocations


   ObjectToTranslatorMap::iterator it = m_processedTranslators.begin();
   ObjectToTranslatorMap::iterator itEnd = m_processedTranslators.end();
   for ( ; it != itEnd; ++it)
   {
      CNodeTranslator *translator = it->second;
      if (translator == NULL) continue;

      AtNode *node = translator->GetArnoldNode();
      if (node == NULL) continue;

      if (AiNodeIs(node, "MayaFile") || AiNodeIs(node, "image") || AiNodeIs(node, "MayaImagePlane")) textureNodes.push_back(translator);
      
   }

   bool progressStarted = false;
   AtMap<std::string, std::string> textureColorSpaces;
   for (size_t i = 0; i < textureNodes.size(); ++i)
   {
      CNodeTranslator *translator = textureNodes[i];
      if (translator == NULL) continue;

      AtNode *node = translator->GetArnoldNode();
      if (node == NULL) continue;
      
      MString filename = AiNodeGetStr(node, "filename");
      std::string filenameStr = filename.asChar();

      const char *autoTxParam = AiNodeIs(node, "image") ? "autoTx" : "aiAutoTx";
      bool fileAutoTx = autoTx && translator->FindMayaPlug(autoTxParam).asBool();
      MString searchPath = "";
      bool invalidProgressWin = false;
      if (fileAutoTx)
      {
         MString colorSpace = translator->FindMayaPlug("colorSpace").asString();
         std::string colorSpaceStr = colorSpace.asChar();

         AtMap<std::string, std::string>::iterator it = textureColorSpaces.find(filenameStr);
         if (it == textureColorSpaces.end())
         {
            textureColorSpaces[filenameStr] = colorSpaceStr;
         } else
         {
            // already dealt with this filename, skip the auto-tx
            if (colorSpaceStr != it->second)
            {
               AiMsgDebug("[mtoa.autotx]  %s is referenced multiple times with different color spaces", filename.asChar());
            }
            goto USE_TX;
         }

         if (progressBar)
         {
            if (!progressStarted)
            {
               MProgressWindow::reserve();
               MProgressWindow::setProgressRange(0, 100);
               MProgressWindow::setTitle("Converting Images to TX");
               MProgressWindow::setInterruptable(true);

               // if the progress bar was already cancelled before it started
               // (it seems that it happens sometimes...), the we simply
               // don't test for cancel anymore
               if (MProgressWindow::isCancelled()) invalidProgressWin = true;
            }
            if ((!invalidProgressWin) && MProgressWindow::isCancelled()) 
            {
               // FIXME show a confirm dialog to mention color management will be wrong
               //MString cmd;
               //cmd.format("import maya.cmds as cmds; cmds.confirmDialog(title='Warning', message='Color Management will be invalid if TX files aren't generated', button='Ok')");
               //MGlobal::executePythonCommandStringResult(cmd);

               // if progress was cancelled we consider that auto-Tx is OFF
               // but we still need to handle "use Tx"
               MProgressWindow::endProgress();
               fileAutoTx = false;

               goto USE_TX;
            }

            // FIXME use basename instead
            MString progressStatus = filename;
            int basenameIndex = progressStatus.rindexW('/');
            if (basenameIndex > 0)
            {
               progressStatus = progressStatus.substring(basenameIndex + 1, progressStatus.numChars() - 1);
            }
            progressStatus += " (";
            progressStatus += (unsigned int)(i + 1);
            progressStatus += "/";
            progressStatus += (unsigned int)textureNodes.size();
            progressStatus += ")";

            while (progressStatus.length() < 50)
            {
               progressStatus += "    ";
            }

            MProgressWindow::setProgressStatus(progressStatus);

            if (!progressStarted)
            {
               MProgressWindow::startProgress();
               // strange, but I need to change the value once so that it is displayed
               MProgressWindow::setProgress(1);
               MProgressWindow::setProgress(0);
            }
            else
            {
               MProgressWindow::setProgress(i * 100 / textureNodes.size());
            }
            progressStarted = true;
         }


         // convert TX
         int createdFiles = 0;
         int skippedFiles = 0;
         int errorFiles = 0;

         makeTx(filename, colorSpace, &createdFiles, &skippedFiles, &errorFiles);
         
         if (createdFiles + skippedFiles + errorFiles == 0)
         {               
            // no file has been found
            // let's try with the search paths
            for (unsigned int t = 0; t < searchPaths.length(); ++t)
            {
               searchPath = searchPaths[t];
               MString searchFilename = searchPath + filename;
               makeTx(searchFilename, colorSpace, &createdFiles, &skippedFiles, &errorFiles);

               if (createdFiles + skippedFiles + errorFiles > 0) break; // textures have been found with this search path. Let's stop looking for them
            }
         }
      }
USE_TX:
      if (useTx)
      {

         MString txFilename(filename.substring(0, filename.rindexW(".")) + MString("tx"));

         MString searchFilename = searchPath + txFilename;

         MStringArray expandedFilenames = expandFilename(searchFilename);

         if(expandedFilenames.length() == 0 && !autoTx)
         {
            // No file was found for this filename
            // and mipmap hasn't been generated above (auto-tx = false)
            // we should check in the search paths
         
            for (unsigned int i = 0; i < searchPaths.length(); ++i)
            {
               searchFilename = searchPaths[i] + txFilename;
               expandedFilenames = expandFilename(searchFilename);
               
               // we found the texture, stop searching
               if (expandedFilenames.length() > 0) break;
            }
         }
         // if expandedFilenames.length >= 1 then we're OK ?
         if (expandedFilenames.length() > 0)
         {
            filename = txFilename;
            FormatTexturePath(filename);
            AiNodeSetStr(node, "filename", filename.asChar()); 
            
         
         }
      }      
   }
   if (progressBar && progressStarted) MProgressWindow::endProgress();
   
}

void CArnoldSession::RequestUpdateMotion()
{
   m_updateMotionData = true;
   RequestUpdate();
}
void CArnoldSession::RequestUpdateOptions()
{
   m_updateOptions = true;
   RequestUpdate();
}

void CArnoldSession::RecursiveUpdateDagChildren(MDagPath &parent)
{
   MDagPath path = parent;
   // check if there is a translator for this dag path
   // If yes, Remove its update callbacks, and request an update on it
   CNodeAttrHandle handle(path);
   std::vector< CNodeTranslator * > translators;
   if (GetActiveTranslators(handle, translators))
   {
      for (size_t i = 0; i < translators.size(); ++i)
      {
         translators[i]->m_impl->RemoveUpdateCallbacks();
         translators[i]->RequestUpdate();
      }
   }
 
   // Recursively dive into the dag children
   for (unsigned int i = 0; i < path.childCount(); i++)
   {
      MObject ChildObject = path.child(i);
      path.push(ChildObject);
      RecursiveUpdateDagChildren(path);
      path.pop(1);
   }

}


//--------------------------
// This is an unpleasant piece of code that is meant to allow for 
// procedurals updates during IPR. It's there because arnold itself
// can't support connections updates. If someday this is solved in the core,
// we'll happily get rid of this code
// I tried to centralize this as much as possible, so that it doesn't spread out in mtoa.
// If you remove this code, don't forget to remove the member CNodeTranslatorImpl::m_isProcedural

static inline AtNode *GetRootParentNode(AtNode *node)
{
   AtNode *currentNode = NULL;
   AtNode *parentNode = AiNodeGetParent(node);
   while(parentNode)
   {
      currentNode = AiNodeGetParent(parentNode);
      if (currentNode == NULL) return parentNode;

      parentNode = currentNode;
   }
   return NULL;
}

struct SessionProceduralData
{
   SessionProceduralData(CNodeTranslator *tr) : translator(tr), state(PROC_STATE_UNKNOWN){}

   CNodeTranslator *translator;

   enum TranslatorReferenceState
   {
      PROC_STATE_UNKNOWN = 0,
      PROC_STATE_ISOLATED = 1,
      PROC_STATE_EXTERNAL_REFS
   };
   TranslatorReferenceState state;
};

   
static std::map<AtNode*, SessionProceduralData*> s_registeredProcedurals;

// some procedurals are being deleted. 
// Make sure there are no connections from one procedural to another.
// Since Arnold doesn't support deleting nodes properly, we must also re-generate all the 
// procedurals connected to one of these nodes about to be deleted
void CArnoldSession::UpdateProceduralReferences()
{
   if (m_proceduralsToUpdate.empty()) return;
   // ok folks, some procedurals are being re-generated here.
   // This means we have to deal with possible translators dangling references
     
   // Part 1 : Check all nodes in the arnold scene (doh)
   // and see if they have connections on nodes belonging to another procedural
   AtSet<SessionProceduralData *> registeredProceduralData;
   AtSet<CNodeTranslator *> outsideConnectionsList;
   std::vector<AtNode*> nodeConnections;

   // optimization, to avoid calling the hash map too often
   AtNode *lastParent = NULL;
   SessionProceduralData *lastData = NULL;
   AtNode *lastTargetParent = NULL;
   SessionProceduralData *lastTargetData = NULL;

   AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(AI_NODE_ALL);         
   while (!AiNodeIteratorFinished(nodeIter))
   {
      AtNode* node = AiNodeIteratorGetNext(nodeIter);

      // get the root parent node
      AtNode *parentNode = GetRootParentNode(node);

      // we don't need to consider nodes that don't belong to a procedural
      // because in practice (to my knowledge) there is no way to connect
      // a "parentless" node in maya to a node living on a procedural
      if (parentNode == NULL) continue;
      
      // parentNode is the proceduralNode that should have a corresponding translator in the scene
      SessionProceduralData *procData = (parentNode == lastParent) ? lastData : s_registeredProcedurals[parentNode]; 

      // just to optimize things and avoid calling the map too frequently
      lastParent = parentNode;
      lastData = procData;
      if(procData == NULL || procData->translator == NULL) continue;

      CNodeTranslator *rootTranslator = procData->translator;

      // FIXME should we test m_holdUpdates ?
      if (rootTranslator->m_impl->m_updateMode >= CNodeTranslator::AI_RECREATE_NODE) continue;

      // this translator has already been treated previously. The references are correctly filled
      // so he don't need to check the connections
      if (procData->state != SessionProceduralData::PROC_STATE_UNKNOWN) continue;

      registeredProceduralData.insert(procData);
      // ok, heaviest part now....
      // loop over all attributes and see if there is a connection to the outside world (another translator)
      AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(node));
      AtNode *target = NULL;
      while (!AiParamIteratorFinished(nodeParam))
      {
         const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
         const char* paramName = AiParamGetName(paramEntry);
         std::string paramStr = paramName;
         
         nodeConnections.clear();
         int paramType = AiParamGetType(paramEntry);
         
         if(paramType == AI_TYPE_NODE)
         {
            // it seems that AiNodeGetLink doesn't work for node references
            // so we need a special case here
            target = (AtNode*)AiNodeGetPtr(node, paramName);
            if (target)
               nodeConnections.push_back(target);
         } else if (AiNodeIsLinked(node, paramName))
         {
            if(AiParamGetType(paramEntry) == AI_TYPE_ARRAY)
            {
               AtArray *arr = AiNodeGetArray(node, paramName);
               if (arr == NULL) continue; // shouldn't happen since this is linked
               for (int a = 0; a < (int)arr->nelements; ++a)
               {
                  target = AiNodeGetLink(node, paramName, &a);
                  if (target) 
                     nodeConnections.push_back(target);
               }
            } else
            {
               target = AiNodeGetLink(node, paramName);
               if (target)
                  nodeConnections.push_back(target);
            }
         } else if (paramType == AI_TYPE_ARRAY)
         {
            AtArray *arr = AiNodeGetArray(node, paramName);
            if (arr && arr->type == AI_TYPE_NODE)
            {
               for (int a = 0; a < (int)arr->nelements; ++a)
               {
                  target = (AtNode*)AiArrayGetPtr(arr, a);
                  if (target) 
                     nodeConnections.push_back(target);
               }
            }
         }

         // no link
         if (nodeConnections.empty()) 
            continue;

         for (size_t t = 0; t < nodeConnections.size(); ++t)
         {
            target = nodeConnections[t];
            if (target == NULL) 
               continue;

            AtNode *targetParent = GetRootParentNode(target);

            // self-contained connections
            if (targetParent == NULL || targetParent == parentNode) 
               continue;

            // ok, so at this point, I have unfortunately a connection to another procedural, damn....
            SessionProceduralData *targetData = (lastTargetParent == targetParent) ? lastTargetData : s_registeredProcedurals[targetParent]; 
            
            // just to optimize things and avoid calling the map too frequently
            lastTargetParent = targetParent;
            lastTargetData = targetData;
            if (targetData == NULL || targetData->translator == NULL) continue;
            CNodeTranslator *targetTranslator = targetData->translator;

            outsideConnectionsList.insert(rootTranslator);
            rootTranslator->m_impl->AddReference(targetTranslator);
         }
      }
      AiParamIteratorDestroy(nodeParam);
   }
   AiNodeIteratorDestroy(nodeIter);


   // Part 2 Set the procedural flags on the translators so that we don't have to do this mess at every IPR update
   AtSet<SessionProceduralData *>::iterator it = registeredProceduralData.begin();
   AtSet<SessionProceduralData *>::iterator itEnd = registeredProceduralData.end();

   for ( ; it != itEnd; ++it)
   {
      SessionProceduralData *procData = *it;
      
      procData->state = (outsideConnectionsList.find(procData->translator) == outsideConnectionsList.end()) ? 
         SessionProceduralData::PROC_STATE_ISOLATED : SessionProceduralData::PROC_STATE_EXTERNAL_REFS;

   }

   // Part 3: now that all references are connected, 
   // RE-set the update mode so that all the propagations happens correctly
   AtSet<CNodeTranslator*>::iterator iter = m_proceduralsToUpdate.begin();
   AtSet<CNodeTranslator*>::iterator iterEnd = m_proceduralsToUpdate.end();
   for ( ; iter != iterEnd; ++iter)
   {      
      // we temporarily reset the update mode to update_only
      // so that SetUpdateMode does its job
      CNodeTranslator::UpdateMode updateMode = (*iter)->m_impl->m_updateMode;
      (*iter)->m_impl->m_updateMode = CNodeTranslator::AI_UPDATE_ONLY; 
      (*iter)->SetUpdateMode(updateMode);
   }

   m_proceduralsToUpdate.clear();
}

// a procedural node is being created
void CArnoldSession::RegisterProcedural(AtNode *node, CNodeTranslator *translator)
{
   s_registeredProcedurals[node] = new SessionProceduralData(translator);
}
// a procedural node is being deleted
void CArnoldSession::UnRegisterProcedural(AtNode *node)
{
   std::map<AtNode*, SessionProceduralData*>::iterator iter = s_registeredProcedurals.find(node);
   if (iter == s_registeredProcedurals.end()) return;

   delete iter->second; // delete the SessionProceduralData
   s_registeredProcedurals.erase(iter);
}
// a procedural is going to be deleted / regenerated
void CArnoldSession::QueueProceduralUpdate(CNodeTranslator *translator)
{
   m_proceduralsToUpdate.insert(translator);
}
