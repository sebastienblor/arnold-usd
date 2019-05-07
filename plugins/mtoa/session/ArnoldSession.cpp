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
#include <maya/MSceneMessage.h>

#include <assert.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

      MPlug templatePlug = node.findPlug("template", true, &status);
      MPlug overEnablePlug = node.findPlug("overrideEnabled", true, &status);
      MPlug overDispPlug = node.findPlug("overrideDisplayType", true, &status);

      if (status == MStatus::kFailure)
         return false;

      if (templatePlug.asBool())
        return true;
      else
         if (overEnablePlug.asBool() && overDispPlug.asInt()==1)
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
template <class T>
static void ChangeCurrentFrame(T time, int sessionMode)
{   
   if (sessionMode == MTOA_SESSION_RENDERVIEW && CRenderSession::IsViewportRendering())
      return;
   
   MGlobal::viewFrame(time); // time can be either MTime or double
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
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.session]     "+ name + " | Ignoring DAG node of type "+ type);

      AiMsgTab(-1);
      return NULL;
   }
   else if (!translator->m_impl->IsMayaTypeDag())
   {
      if (stat != NULL) *stat = MStatus::kInvalidParameter;
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] translator for "+ name+" of type "+type+" is not a DAG translator");

      AiMsgTab(-1);
      return NULL;
   }

   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.session]     "+name+" | Exporting DAG node of type "+ type);

   CNodeAttrHandle handle(dagPath);
   MString hashCode;
   handle.GetHashString(hashCode);
   std::string hashStr(hashCode.asChar());
   // Check if node has already been processed
   ObjectToTranslatorMap::iterator it = m_processedTranslators.find(hashStr);

   if (it != m_processedTranslators.end())
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.session]     "+ name+" | Reusing previous export of DAG node of type "+type);

      delete translator;
      status = MStatus::kSuccess;
      arnoldNode = it->second->GetArnoldNode();
      translator = (CDagTranslator*)it->second;
   }

   if (arnoldNode == NULL)
   {
      status = MStatus::kSuccess;
      translator->m_impl->Init(this, dagPath);
      if (it != m_processedTranslators.end())
      {
         it->second = translator;
      }
      else
      {
         m_processedTranslators[hashStr] = translator;
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
CNodeTranslator* CArnoldSession::ExportNode(const MPlug& shaderOutputPlug, 
                                   bool initOnly, int instanceNumber, MStatus *stat)
{
   //instanceNumber is currently used only for bump. We provide a specific instance number

   MObject mayaNode = shaderOutputPlug.node();
   MStatus status = MStatus::kSuccess;
   AtNode* arnoldNode = NULL;
   CNodeTranslator* translator = NULL;
   MDagPath dagPath;
   // FIXME: should get correct instance number from plug
   if (MFnDagNode(MFnDagNode(mayaNode).parent(0)).getPath(dagPath) == MS::kSuccess)
   {
      dagPath.push(mayaNode);
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
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.session]     "+name+": Maya node type not supported: "+type);

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
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] [maya "+name+"] Invalid output attribute: \""+ resultPlug.partialName(false, false, false, false, false, true) +"\"");
      AiMsgTab(-1);
      return NULL;
   }

   MString plugName = resultPlug.name();
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.session]     "+name+" | Exporting plug "+plugName+" for type "+type);

   CNodeAttrHandle handle;
   MString handleCode;
   if (translator->DependsOnOutputPlug())
   {
      handle.set(resultPlug, instanceNumber);
      handle.GetHashString(handleCode, true); // true because I need the plug name
   }
   else
   {
      handle.set(mayaNode, "", instanceNumber);
      handle.GetHashString(handleCode, false); // I don't need the plug name
   }
   std::string hashStr(handleCode.asChar());
   ObjectToTranslatorMap::iterator it = m_processedTranslators.find(hashStr);

   if (it != m_processedTranslators.end())
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.session]     "+name+" | Reusing previous export of node of type "+type);

      delete translator;
      status = MStatus::kSuccess;
      arnoldNode = it->second->GetArnoldNode();
      translator = it->second;
   }
   
   if (arnoldNode == NULL)
   {
      status = MStatus::kSuccess;
      translator->m_impl->Init(this, mayaNode, resultPlug.partialName(false, false, false, false, false, true), instanceNumber);
      if (it != m_processedTranslators.end())
      {
         it->second = translator;
      }
      else
      {
         m_processedTranslators[hashStr] = translator;
         
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

CNodeTranslator *CArnoldSession::GetActiveTranslator(const CNodeAttrHandle &handle)
{
   MString hashCode;
   handle.GetHashString(hashCode);
   std::string hashStr(hashCode.asChar());
   ObjectToTranslatorMap::iterator it = m_processedTranslators.find(hashStr);

   if (it != m_processedTranslators.end())
      return it->second;


   // FIXME, should we try now with the attribute name for multi-output translators ?
   /*
   handle.GetHashString(hashCode, true);
   hashStr= hashCode.asChar();
   ObjectToTranslatorMap::iterator it = m_processedTranslators.find(hashStr);

   if (it != m_processedTranslators.end())
      return it->second;   
   */

   return NULL;
}

// This function removes the translator from our list of 
// processed translators. Note that it doesn't actually erase it,
// this will be done later
void CArnoldSession::EraseActiveTranslator(CNodeTranslator *translator)
{
   if (translator == NULL) return;

   CNodeAttrHandle &handle = translator->m_impl->m_handle;
   MString hashCode;
   handle.GetHashString(hashCode, translator->DependsOnOutputPlug());

   std::string hashStr(hashCode.asChar());
   
   m_processedTranslators.erase(hashStr);
   // we're not deleting the translator yet
}



bool CArnoldSession::IsRenderablePath(MDagPath dagPath)
{
   MStatus stat = MStatus::kSuccess;

   // We were testing the render layer in some places but not here.
   // This made us ignore the render layers during IPR updates (see #3144 #3350)
   unsigned int mask = GetExportFilterMask();
   if ((mask & MTOA_FILTER_LAYER) && !IsInRenderLayer(dagPath))
      return false;
   
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

MStatus CArnoldSession::Begin(const CSessionOptions &options)
{
 
   MStatus status = MStatus::kSuccess;

   m_sessionOptions = options;

   status = UpdateMotionFrames();

   m_is_active = true;
   m_requestUpdate = false;

   m_scaleFactor = options.GetScaleFactor();
   AtVector s(static_cast<float>(m_scaleFactor), static_cast<float>(m_scaleFactor), static_cast<float>(m_scaleFactor));
   m_scaleFactorAtMatrix = AiM4Scaling(s);

   double sc[3] = {m_scaleFactor, m_scaleFactor, m_scaleFactor};
   m_scaleFactorMMatrix.setToIdentity();
   MTransformationMatrix trmat(m_scaleFactorMMatrix);
   trmat.setScale(sc, MSpace::kWorld);
   m_scaleFactorMMatrix = trmat.asMatrix();

   m_origin = options.GetOrigin();

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
      ChangeCurrentFrame(MTime(GetExportFrame(), MTime::uiUnit()), GetSessionMode());
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

      if (MS::kSuccess != status)
         AiMsgError("[mtoa] Failed to parse light linking information for %i lights", m_numLights);
      
   }
   else
   {
      AiMsgWarning("[mtoa] No light in scene");
   }

   FlagLightLinksDirty(false);
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.sesion]    Updating Light-linking information");

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
   if(MtoaTranslationInfo())
      MtoaDebugLog("[mtoa] Exporting Arnold options "+ fnNode.name());

   MPlug optPlug = fnNode.findPlug("message", true);
   m_optionsTranslator = (COptionsTranslator*)ExportNode(optPlug, false);

   ExportColorManager();

   return m_optionsTranslator->GetArnoldNode();
}

AtNode *CArnoldSession::ExportColorManager()
{
// Color Management is only supported for 2016 and higher   
#ifndef ENABLE_COLOR_MANAGEMENT
   return NULL;
#endif

   // get the maya node contraining the color management options         
   MSelectionList activeList;
   activeList.add(MString(":defaultColorMgtGlobals"));
   
   if(activeList.length() > 0)
   {
      MObject colorMgtObject;
      activeList.getDependNode(0,colorMgtObject);
      MFnDependencyNode fnSNode(colorMgtObject);
      MPlug mgtPlug = fnSNode.findPlug("message", true);
      CNodeTranslator* syncolorTr = ExportNode(mgtPlug, false);

      if(syncolorTr)
         return syncolorTr->GetArnoldNode();
   }
   return NULL;
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

      if (MtoaTranslationInfo())
      {
         MString log = "[mtoa] Exporting selection (";
         log += ns;
         log += ":";
         log += fns;
         log += ")";
         MtoaDebugLog(log);
      }
   }
   else
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] Exporting scene");
   }

   // Set up export options
   ArnoldSessionMode exportMode = m_sessionOptions.m_mode;
   m_motionStep = 0;


   if (exportMode == MTOA_SESSION_BATCH || exportMode == MTOA_SESSION_ASS)
   {
      MGlobal::executeCommand("prepareRender -setup");
      MGlobal::executeCommand("prepareRender -invokePreRender");
   }

   CRenderSession *renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
      renderSession->SetRenderViewStatusInfo(MString("Exporting Arnold Scene..."));

   // Are we motion blurred (any type)?
   const bool mb = IsMotionBlurEnabled();

   if (MtoaTranslationInfo())
   {
      MString log = "[mtoa.session]     Initializing at frame ";
      log += GetExportFrame();
      MtoaDebugLog(log);
   }

   ExportOptions();  // inside loop so that we're on the proper frame

   // First "real" export
   ChangeCurrentFrame(m_sessionOptions.m_frame, GetSessionMode());
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
      // For render selected we need all the lights (including unselected ones)
      status = ExportLights();
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

   // Eventually export all shading groups
   if (m_sessionOptions.GetExportAllShadingGroups())
   {
      MStringArray shadingGroups;
      if (exportSelected)
         MGlobal::executeCommand("ls -sl -typ shadingEngine", shadingGroups); // get selected shading groups and export them
      else
         MGlobal::executeCommand("ls -typ shadingEngine", shadingGroups); // get all shading groups in the scene and export them
      
      for (unsigned int shg = 0; shg < shadingGroups.length(); ++shg)
      {
         MSelectionList shgElem;
         shgElem.add(shadingGroups[shg]);
         MPlug shgPlug;
         shgElem.getPlug(0, shgPlug);

         ExportNode(shgPlug); // in case the shading group has custom AOVs (#3284)

         // Since we no longer export MayaShadingEngine, we might need to export the assigned shaders
         MFnDependencyNode shEngineNode(shgPlug.node());
         MPlugArray connections;
         MStringArray shaderAttrs;
         shaderAttrs.append("aiSurfaceShader");
         shaderAttrs.append("surfaceShader");
         shaderAttrs.append("aiVolumeShader");
         shaderAttrs.append("volumeShader");
         shaderAttrs.append("displacementShader");
         for (unsigned int a = 0; a < shaderAttrs.length(); a++)
         {
            MPlug shaderPlug = shEngineNode.findPlug(shaderAttrs[a], true);
            if (!shaderPlug.isNull())
            {
               shaderPlug.connectedTo(connections, true, false);
               if (connections.length() > 0)
               {
                  ExportNode(connections[0]);
                  if (shaderAttrs[a] == MString("aiSurfaceShader") ||
                     shaderAttrs[a] == MString("aiVolumeShader"))
                     a++; // if an "ai" connection was found, skip the maya native one
               }
            }
         }
      }
   }

   // The list of processedTranslators can grow while we call doExport a few lines below.
   // So we can't call doExport while iterating over them.
   // Thus we first store the list of translators to process.
   std::vector<CNodeTranslator*> translatorsToExport;
   translatorsToExport.reserve(m_processedTranslators.size());
   ObjectToTranslatorMap::iterator it = m_processedTranslators.begin();
   ObjectToTranslatorMap::iterator itEnd = m_processedTranslators.end();
   for (; it != itEnd; ++it)
      if (it->second) translatorsToExport.push_back(it->second);
   
   // First export the current frame 
   m_isExportingMotion = false;
   // set motion step
   m_motionStep = 0;
   int currentFrameIndex = -1;
   if (mb)
   {
      for (size_t i = 0; i < m_motion_frames.size(); ++i)
      {
         if (std::abs(m_motion_frames[i] - m_sessionOptions.m_frame) < 
            (m_motion_frames[1] - m_motion_frames[0]) /10.)
         {            
            currentFrameIndex = m_motionStep = i;
            break;
         }
      }
   }
   
   // Loop through the already processed translators and export for current frame
   std::vector<CNodeTranslator*>::iterator trIt = translatorsToExport.begin();
   std::vector<CNodeTranslator*>::iterator trItEnd = translatorsToExport.end();
   for ( ; trIt != trItEnd; ++trIt)
   {
      // actual export
      (*trIt)->m_impl->DoExport();
      
      if (!mb)
         (*trIt)->PostExport((*trIt)->m_impl->m_atNode); // post export if no motion blur
   }
   
   if (mb)
   {
      // re-generate the translators to export list as it may have increased during 
      // the first export
      translatorsToExport.clear();
      translatorsToExport.reserve(m_processedTranslators.size());
      it = m_processedTranslators.begin();
      itEnd = m_processedTranslators.end();
      for (; it != itEnd; ++it)
         if (it->second) translatorsToExport.push_back(it->second);

      // now loop through motion steps
      unsigned int numSteps = GetNumMotionSteps();
      m_isExportingMotion = true;
      
      if (renderSession)
         renderSession->SetRenderViewStatusInfo(MString("Exporting Motion Data..."));

      for (unsigned int step = 0; step < numSteps; ++step)
      {
         if (step == (unsigned int)currentFrameIndex)
            continue; // current frame, has already been exported above
         ChangeCurrentFrame(MTime(m_motion_frames[step], MTime::uiUnit()), GetSessionMode());

         if (MtoaTranslationInfo())
         {
            MString log = "[mtoa.session]     Exporting step ";
            log += step + 1;
            log += " of ";
            log += numSteps;
            log += " at frame ";
            log +=  m_motion_frames[step];
            MtoaDebugLog(log);
         }
         
         // then, loop through the already processed dag translators and export for current step
         // NOTE: these exports are subject to the normal pre-processed checks which prevent redundant exports.
         // Since all nodes *should* be exported at this point, the following calls to DoExport do not
         // traverse the DG even if the translators call ExportNode or ExportDag. This makes it safe
         // to re-export all objects from a flattened list
         
         // for safety we're not doing the loop on m_motionSteps directly in case it is modified somewhere else
         m_motionStep = step; 

         trItEnd = translatorsToExport.end();
         for (trIt = translatorsToExport.begin(); trIt != trItEnd; ++trIt)
            (*trIt)->m_impl->DoExport();
   
      }

      // invoke post export callback. 
      // FIXME should we set anim_arrays here too ? (as in DoUpdate)
      for (trIt = translatorsToExport.begin(); trIt != trItEnd; ++trIt)
         (*trIt)->PostExport((*trIt)->m_impl->m_atNode);


      // Note: only reset frame during interactive renders, otherwise that's an extra unnecessary scene eval
      // when exporting a sequence.  Other modes are reset to the export frame in CArnoldSession::End() 
      // (see tickets #418 and #444)
      // FIXME: however we have a ticket "2044 about frame not being correct during post-export scripts
      if (GetSessionMode() == MTOA_SESSION_RENDER || GetSessionMode() == MTOA_SESSION_IPR || GetSessionMode() == MTOA_SESSION_RENDERVIEW)
      {
         ChangeCurrentFrame(MTime(GetExportFrame(), MTime::uiUnit()), GetSessionMode());
      }
      m_isExportingMotion = false;
   }

   m_motionStep = 0;

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
         nodeTr->m_impl->m_inUpdateQueue = false; // allow to trigger new updates
         nodeTr->m_impl->m_isExported = true; // next export will be an update
         // for motion blur, check which nodes are static and which aren't (#2316)
         if (mb && GetNumMotionSteps() > 1)
         {
            nodeTr->m_impl->m_animArrays = nodeTr->m_impl->HasAnimatedArrays();
         } else nodeTr->m_impl->m_animArrays = false;
      }
      m_objectsToUpdate.clear(); // I finished exporting, I don't have any other object to Update now
   }

   // it would seem correct to only call ExportTxFiles if m_updateTx = true
   ExportTxFiles();

   if (exportMode == MTOA_SESSION_BATCH || exportMode == MTOA_SESSION_ASS)
   {
      MGlobal::executeCommand("prepareRender -invokePostRender");
      MGlobal::executeCommand("prepareRender -restore");    
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

      MFnDagNode cameraNode;
      MPlug renderablePlug;
      // First we export all cameras
      // We do not reset the iterator to avoid getting kWorld
      for (; (!dagIterCameras.isDone()); dagIterCameras.next())
      {
         if (dagIterCameras.getPath(path))
         {
            
            MStatus stat;
            cameraNode.setObject(path);
            // Note that some non-renderable cameras are still exported in 
            // ExportDag, if their filteredStatus is "accepted"
            renderablePlug = cameraNode.findPlug("renderable", false, &stat);
            bool isRenderable = (stat == MS::kSuccess) ? renderablePlug.asBool() : false;

            // Force the export of default persp camera for ARV (#3655)
            if (isRenderable == false && GetSessionMode() ==  MTOA_SESSION_RENDERVIEW && path.partialPathName() == MString("perspShape"))
               isRenderable = true;

            if (isRenderable)
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

            // FIXME both filters below happen to always be enabled 
            if (/*(mask & MTOA_FILTER_TEMPLATED) && */ IsTemplatedPath(path))
               continue;
            if (/*(mask & MTOA_FILTER_HIDDEN) &&*/ !IsVisiblePath(path))
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
            MPlug plug = depFn.findPlug("message", true);
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

// When a hidden node gets "unhidden", we need to go through an "idle" callback
// so that Maya has time to process its visibility status properly
static MCallbackId s_hiddeNodeCb = 0;
static MDagPathArray s_hiddenNodesArray;
void CArnoldSession::DoHiddenCallback(void* clientData)
{
   MMessage::removeCallback(s_hiddeNodeCb);
   s_hiddeNodeCb = 0;
   MStatus status;
   CArnoldSession *session = (CArnoldSession*)clientData;
   if (session)
   {
      for (unsigned int i = 0; i< s_hiddenNodesArray.length(); i++)
      {
         DagFiltered filtered = session->FilteredStatus(s_hiddenNodesArray[i]);
         if (filtered == MTOA_EXPORT_ACCEPTED)
            session->SetDagVisible(s_hiddenNodesArray[i]);
      }
   }
   s_hiddenNodesArray.clear();
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

   MString plugName = plug.partialName(false, false, false, false, false, true);

   //CArnoldSession *session = (CArnoldSession*)clientData;
   //DagFiltered filtered = session->FilteredStatus(path); // We shouldn't invoke this here otherwise we're messing with Maya's DAG 
   if (/*filtered != MTOA_EXPORT_ACCEPTED && */ plugName != "visibility")
    return; 

   // We need to go through an "idle" callback, otherwise Maya won't have time to process the 
   // visibility status of this node 
   if(s_hiddeNodeCb == 0)
   {
      s_hiddeNodeCb = MEventMessage::addEventCallback("idle",
                                                  CArnoldSession::DoHiddenCallback,
                                                  clientData
                                                  );
   } 
   s_hiddenNodesArray.append(path);
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

         // We shouldn't call this function during a render !
         // It's going to create new nodes (#3355)
         //CDagTranslator *tr = ExportDagPath(path, true, &stat);
         QueueForUpdate(path);
         if (stat != MStatus::kSuccess)
            status = MStatus::kFailure;

/*
         // Since we're not calling ExportDagPath anymore, we don't have any translator and we don't know
         if (tr != NULL && !tr->ExportDagChildren())
         {
            pruneDag = true;
            parentPruneDag = path;
            parentPruneDag.pop();
            dagIterator.prune();
         }*/
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
   // FIXME both filters below appear to always be enabled
   if (/*(mask & MTOA_FILTER_TEMPLATED) &&*/ IsTemplatedPath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;
   if (/*(mask & MTOA_FILTER_HIDDEN) &&*/ !IsVisiblePath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;
   // Tests that cause the node to be ignored
   if ((mask & MTOA_FILTER_LAYER) && !IsInRenderLayer(path))
      return MTOA_EXPORT_REJECTED_NODE;

   // Then test against all types passed in the MFN::Types array
   /* This is no longer used
   MObject obj = path.node();
   MFnDagNode node(obj);
   MString name = node.name();
   MFnTypeSet::const_iterator sit(filter->excluded.begin()), send(filter->excluded.end());
   for(; sit!=send;++sit)
      if (obj.hasFn(*sit))
         return MTOA_EXPORT_REJECTED_NODE;
   */
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


void CArnoldSession::QueueForUpdate(CNodeTranslator * translator)
{
   // don't add translator twice to the list, it could crash if its updateMode
   // is "delete". Other solution would be to use a set, but the extra-cost is not
   // necessary since we already have this flag
   if (translator == NULL || translator->m_impl->m_inUpdateQueue) return; 

   // During IPR with motion blur, we change the current frame to export the motion 
   // and this might propagate nodeDirty signals that end up here. 
   // We don't want to consider those
   if (m_isExportingMotion && IsInteractiveRender()) return;

   // Set this translator as being in the update list, to avoid useless future signals
   translator->m_impl->m_inUpdateQueue = true;   

   if (MtoaTranslationInfo())
   {
      MString log ="[mtoa.session]    Queuing "+ translator->GetMayaNodeName()+" for "; 
      switch(translator->m_impl->m_updateMode)
      {
         default:
         case CNodeTranslator::AI_UPDATE_ONLY:
            log += " Update";
            break;
         case CNodeTranslator::AI_RECREATE_NODE:
            log += " Re-generation";
            break;
         case CNodeTranslator::AI_RECREATE_TRANSLATOR:
            log += " Translator re-generation";
            break;
         case CNodeTranslator::AI_DELETE_NODE:
            log += "Deletion";
            break;
      }
      MtoaDebugLog(log);
   }
   // add this translator to the list of objects to be updated in next DoUpdate()
   m_objectsToUpdate.push_back(ObjectToTranslatorPair(translator->m_impl->m_handle, translator));
}

void CArnoldSession::RequestUpdate()
{
   m_requestUpdate = true;
   CMayaScene::UpdateIPR();
}

void CArnoldSession::DoUpdate()
{
   bool mtoa_translation_info = MtoaTranslationInfo();

   if (mtoa_translation_info)
      MtoaDebugLog("[mtoa.session]    Updating Arnold Scene....");

   CRenderSession *renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
      renderSession->SetRenderViewStatusInfo(MString("Updating Arnold Scene..."));

   MStatus status;
   assert(AiUniverseIsActive());

   std::vector< CNodeTranslator * > translatorsToUpdate;
   std::vector<ObjectToTranslatorPair>::iterator itObj;
   size_t objectsToUpdateCount;
   bool newDag, exportMotion, motionBlur, mbRequiresFrameChange;
   int currentFrameIndex;
   MString hashCode;
   std::string hashStr;

   bool interactiveRender = IsInteractiveRender();
   
   double frame = MAnimControl::currentTime().as(MTime::uiUnit());
   bool frameChanged = (frame != GetExportFrame());

   // only change the frame for interactive renders
   // It appears that Export() doesn't restore the current frame
   // in maya otherwise ( to avoid useless maya evaluations )
   if (frameChanged && interactiveRender) SetExportFrame(frame);

   size_t previousUpdatedTranslators = 0;

   int updateRecursions = 0;
   static const int maxUpdateRecursions = 5;

   if (m_updateMotionData)
   {  
      if (mtoa_translation_info)
         MtoaDebugLog("[mtoa.session]    Updating Motion Blur data");

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
      if (mtoa_translation_info)
         MtoaDebugLog("[mtoa.session]     Updating Scene Options");

      renderSession->UpdateRenderOptions();
      m_updateOptions = false;
   }

   exportMotion = false;
   motionBlur = IsMotionBlurEnabled();
   mbRequiresFrameChange = false;
   m_isExportingMotion = false;

UPDATE_BEGIN:

   newDag = false;
   
   m_motionStep = 0;
   currentFrameIndex = -1;
   if (motionBlur)
   {

      for (size_t i = 0; i < m_motion_frames.size(); ++i)
      {
         if (std::abs(m_motion_frames[i] - m_sessionOptions.m_frame) < 
            (m_motion_frames[1] - m_motion_frames[0]) /10.)
         {            
            currentFrameIndex = m_motionStep = i;
            break;
         }
      }
   }
      // store the amount of updated objects as this list can increase during the actual updates
   // (e.g. when a new node is added to a shading tree)
   objectsToUpdateCount = m_objectsToUpdate.size();

   // In theory, no objectsToUpdate are supposed to be 
   // added to this list during the loop. But to make 
   // sure this won't be done by any of the functions 
   // we'll be invoking here it's safer to loop 
   // with the vector's index instead of relying on iterators...
   for (size_t i = 0; i < objectsToUpdateCount; ++i)
   {
      CNodeAttrHandle handle(m_objectsToUpdate[i].first);           // TODO : test isValid and isAlive ?
      CNodeTranslator * translator = m_objectsToUpdate[i].second;

      // Check if this translator needs to be re-created
      if (translator != NULL && translator->m_impl->m_updateMode == CNodeTranslator::AI_RECREATE_TRANSLATOR)
      {
         handle.GetHashString(hashCode, translator->DependsOnOutputPlug());
         hashStr = hashCode.asChar();
         // delete the current translator, just like AI_DELETE_NODE does
         translator->Delete();
         
         m_processedTranslators.erase(hashStr); 

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
            if (mtoa_translation_info)
               MtoaDebugLog("[mtoa.ipr]   Deleting and recreating arnold node for "+ translator->GetMayaNodeName());

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
            if (mtoa_translation_info)
               MtoaDebugLog("[mtoa.ipr]   Deleting arnold node for "+ translator->GetMayaNodeName());
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
            if (mtoa_translation_info)
               MtoaDebugLog("[mtoa.ipr]   Updating arnold node for "+ translator->GetMayaNodeName());
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
            
            translatorsToUpdate.push_back(translator);
         }
      } else
      {
         // No translator was provided, it's either a new node creation or
         // the undo of a delete node
         MObject node = handle.object();
         MString name = MFnDependencyNode(node).name();
         
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
            CDagTranslator *dagTr = ExportDagPath(path, true, &status);
            if (MStatus::kSuccess == status)
            {
               name = path.partialPathName();
               if (mtoa_translation_info)
                  MtoaDebugLog("[mtoa] Exported new node: "+name);

               newDag = true;
               translatorsToUpdate.push_back(dagTr);
               if (motionBlur && (!(exportMotion && mbRequiresFrameChange)) && dagTr->RequiresMotionData())
               {
                  // Find out if we need to call ExportMotion for each motion step
                  // or if a single Export is enough. 
                  exportMotion = true;
                  mbRequiresFrameChange = true;
               }
            } else
            {
               // if we create a maya instance (duplicate special), then the shape will never emit a "new node created" signal
               // and will therefore never be updated (#2657). So we're checking this new node shape and see if it is an instance
               MDagPath shapePath = path;
               shapePath.extendToShape();

               if (shapePath.isInstanced())
               {
                  MDagPath srcParent;
                  MFnDagNode(MFnDagNode(shapePath.node()).parent(0)).getPath(srcParent);
                  srcParent.push(shapePath.node());

                  // If this isn't the master instance
                  if (!(srcParent == shapePath))
                  {
                     dagTr = ExportDagPath(shapePath);
                     if (dagTr)
                     {
                        name = shapePath.partialPathName();
                        if (mtoa_translation_info)
                           MtoaDebugLog("[mtoa] Exported new node: "+ name);

                        newDag = true;
                        
                        translatorsToUpdate.push_back(dagTr);
                        if (motionBlur && (!(exportMotion && mbRequiresFrameChange)) && dagTr->RequiresMotionData())
                        {
                           // Find out if we need to call ExportMotion for each motion step
                           // or if a single Export is enough. 
                           exportMotion = true;
                           mbRequiresFrameChange = true;
                        }
                     }
                  }
               }
            }
         }
         // Dependency nodes are not exported by themselves, their export
         // will be requested if they're connected to an exported node
      }
   }
   bool isLightLinksDirty = IsLightLinksDirty();
   if (newDag || isLightLinksDirty)
      UpdateLightLinks();
   
   // Now update all the translators in our list

   if (mtoa_translation_info)
   {
      MString log = "[mtoa.session]    Updating ";
      log += (int)translatorsToUpdate.size();
      log += " nodes at current frame";
      MtoaDebugLog(log);
   }
   //-------- Exporting all translators at Current Frame
   if (translatorsToUpdate.size() > previousUpdatedTranslators)
   {
      for (std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin() + previousUpdatedTranslators;
         iter != translatorsToUpdate.end(); ++iter)
      {
         CNodeTranslator* translator = (*iter);
         if (translator == NULL)
            continue;

         translator->m_impl->DoUpdate();
         if (!exportMotion)
            translator->PostExport(translator->m_impl->m_atNode);

      }
   }

   if (isLightLinksDirty)
   {
      //-------- now that everything was exported, since light links required an update
      // we need to make sure all shapes are correctly light-linked.
      // We could achieve this by re-exporting the shapes, but it's not actually necessary to do such a 
      // heavy process just for the light links

      ObjectToTranslatorMap::iterator it;
      for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
      {     
         if (it->second == NULL || !it->second->m_impl->IsMayaTypeDag())
            continue;
         CDagTranslator *tr = (CDagTranslator*)it->second;
         AtNode *node = tr->GetArnoldNode();
         if (node == NULL)
            continue;
         if (AiNodeEntryGetType(AiNodeGetNodeEntry(node)) != AI_NODE_SHAPE)
            continue;
         // this is a shape, exporting light linking for it
         m_arnoldLightLinks.ExportLightLinking(node, tr->GetMayaDagPath());
      }
   }

   // During nodes export in DoUpdate(), some new translators can be created by the export of other ones 
   //( e.g. connections in the shading tree). So once update is finished, we might get a new list of
   // objects to update. In that case we'll invoke DoUpdate() recursively, but we don't want it to end up in an infinite loop
   // so we set an arbitrary maximum amount of updates
   if (m_objectsToUpdate.size() > objectsToUpdateCount)
   {
      // some nodes have been added to the update list.
      // let's keep them in this list so that next update invokes them
      m_objectsToUpdate.erase(m_objectsToUpdate.begin(), m_objectsToUpdate.begin() + objectsToUpdateCount);
      previousUpdatedTranslators = translatorsToUpdate.size();
      if (++updateRecursions < maxUpdateRecursions)
         goto UPDATE_BEGIN;
   }
   m_objectsToUpdate.clear();

   //--------- Now handling motion blur export
   if (exportMotion)
   {      
      if (renderSession)
         renderSession->SetRenderViewStatusInfo(MString("Exporting Motion Data..."));

      // Scene is motion blured, get the data for the steps.
      unsigned int numSteps = GetNumMotionSteps();

      // raise this flag to say we're currently exporting the motion steps
      m_isExportingMotion = true;

      // loop over the motion steps
      for (unsigned int step = 0; step < numSteps; ++step)
      {
         if (step == (unsigned int)currentFrameIndex)
            continue; // we have already processed this step during the "current frame export"

         if (mtoa_translation_info)
         {
            MString log = "[mtoa.session]     Updating step ";
            log += step;
            log += " at frame ";
            log += m_motion_frames[step];
            log += " for ";
            log += (int)translatorsToUpdate.size();
            log += " nodes";
            MtoaDebugLog(log);
         }

         // if none of the objects arrays have proven to be animated, 
         // we don't need to change the current frame. However we still need to process
         // the export for every step. Otherwise some AtArray keys could be missing
         if (mbRequiresFrameChange)
            ChangeCurrentFrame(MTime(m_motion_frames[step], MTime::uiUnit()), GetSessionMode());

         // set the motion step as it will be used by translators to fill the arrays
         // at the right index
         m_motionStep = step;

         for (std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
             iter != translatorsToUpdate.end(); ++iter)
         {
            CNodeTranslator* translator = (*iter);
            if (translator == NULL)
               continue;

            translator->m_impl->DoUpdate();
         }
      }      

      // one last loop over the modified translators to call post-export callback. Also verify if their 
      // motion arrays are actually animated or not
      for (std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
             iter != translatorsToUpdate.end(); ++iter)
      {
         CNodeTranslator* translator = (*iter);
         if (translator == NULL)
            continue;

         // invoke post export callback
         translator->PostExport(translator->m_impl->m_atNode);

         // check if it has animated arrays
         translator->m_impl->m_animArrays = translator->m_impl->HasAnimatedArrays();
      }

      // we've done enough harm, let's restore the current frame...
      if (mbRequiresFrameChange)
         ChangeCurrentFrame(MTime(GetExportFrame(), MTime::uiUnit()), GetSessionMode());

      // we're done exporting motion now
      m_isExportingMotion = false;

   }
   m_motionStep = 0;   

   if (m_updateTx) 
   {
      m_updateTx = false;
      ExportTxFiles();
   }

   // Reset IPR status and eventuall add the IPR callbacks now that export is finished
   for(std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
      iter != translatorsToUpdate.end(); ++iter)
   {
      CNodeTranslator* translator = (*iter);
      if (translator == NULL)
         continue;

      CNodeTranslatorImpl *trImpl = translator->m_impl;
      // we no longer clear the update callbacks
      // we just add them if they're missing
      if (interactiveRender && (trImpl->m_mayaCallbackIDs.length() == 0))
         translator->AddUpdateCallbacks();
      
      trImpl->m_inUpdateQueue = false; // I'm allowed to receive updates once again
      trImpl->m_isExported = true;
      // restore the update mode to "update Only"
      trImpl->m_updateMode = CNodeTranslator::AI_UPDATE_ONLY;
   }
      
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
      if (it->second != NULL) it->second->m_impl->RemoveUpdateCallbacks();
   }
}

/// Set the camera to export.

/// If called prior to export, only the specified camera will be exported. If not set, all cameras
/// will be exported, but some translators may not be able to fully export without an export camera specified.
/// To address this potential issue, this method should be called after a multi-cam export, as it will cause
/// the options translator to be updated
///
void CArnoldSession::SetExportCamera(MDagPath camera, bool updateRender)
{
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.session] Setting export camera to \""+ camera.partialPathName() + "\"");

   // first we need to make sure this camera is properly exported
   if (camera.isValid())
   {
      camera.extendToShape();
      ExportDagPath(camera);
   }
   
   m_sessionOptions.SetExportCamera(camera);

   if (updateRender == false  && m_optionsTranslator == NULL) return;
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
MStringArray CArnoldSession::GetActiveImageFilenames() const
{
   if (m_optionsTranslator)
      return m_optionsTranslator->GetActiveImageFilenames();
   return MStringArray();
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
   matrix = AiM4Mult(m_scaleFactorAtMatrix, matrix);
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


bool CArnoldSession::IsVisible(MFnDagNode &node)
{
   MStatus status;

   if (node.isIntermediateObject())
      return false;

   // The material view objects in Maya has always visibility disabled
   // to not show up by default in the scenes. So we need to override
   // that here and always return true for objects in material view session
   CArnoldSession *session = CMayaScene::GetArnoldSession();
   if (session && session->GetSessionMode() ==  MTOA_SESSION_MATERIALVIEW)
      return true;

   MPlug visPlug = node.findPlug("visibility", true, &status);
   // Check standard visibility
   if (status == MStatus::kFailure || !visPlug.asBool())
      return false;

/*
   Reverting this lodVisibility code, as explained in #2679
   
   // FIXME do we really want lodVisibility to affect the visibility in arnold ?
   // Maya viewport seems indeed to take it into account. Note that this is used
   // only by Mesh lights
   MPlug lodVisPlug = node.findPlug("lodVisibility", &status);
   if (status == MStatus::kFailure || !lodVisPlug.asBool())
      return false;
*/
   // Check override visibility
   MPlug overPlug = node.findPlug("overrideEnabled", true, &status);
   if (status == MStatus::kSuccess && overPlug.asBool())
   {
      MPlug overVisPlug = node.findPlug("overrideVisibility", true, &status);
      if (status == MStatus::kFailure || !overVisPlug.asBool())
         return false;
   }
   return true;
}

bool CArnoldSession::IsVisiblePath(MDagPath dagPath)
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
   if (sessionMode == MTOA_SESSION_MATERIALVIEW ||
      sessionMode == MTOA_SESSION_UNDEFINED) return;

   bool autoTx = false;
   bool useTx = false;

   if (sessionMode == MTOA_SESSION_SWATCH )
   {
      // This function is only called by swatch if useTx is true, and we want all this to be fast
      // so it's useless to get the value once again
      useTx = true;
   } else
   {
      // FIXME really inconvenient
      // we must not do this from the global render options, or we might override some export 
      // command line settings
      CRenderOptions renderOptions; 
      renderOptions.SetArnoldRenderOptions(GetArnoldRenderOptions()); 
      renderOptions.GetFromMaya(); 
      
      autoTx = renderOptions.autoTx();
      useTx = renderOptions.useExistingTiledTextures();
   }

   if (useTx == false && autoTx == false) return;

   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.session]    Updating TX files");

   const MStringArray &searchPaths = GetTextureSearchPaths();

   bool progressBar = autoTx && (MGlobal::mayaState() == MGlobal::kInteractive);

   std::vector<CNodeTranslator *> textureNodes;
   textureNodes.reserve(100); // completely empirical value, to avoid first allocations


   ObjectToTranslatorMap::iterator it = m_processedTranslators.begin();
   ObjectToTranslatorMap::iterator itEnd = m_processedTranslators.end();

   static const AtString image_str("image");

   for ( ; it != itEnd; ++it)
   {
      CNodeTranslator *translator = it->second;
      if (translator == NULL) continue;

      AtNode *node = translator->GetArnoldNode();
      if (node == NULL) continue;

      if (AiNodeIs(node, image_str)) textureNodes.push_back(translator);
      
   }

   if (textureNodes.empty())
      return; // nothing to do regarding textures


   // First, let's get Color management configuration files

   // check if color management prefs is enabled
   // colorManagementPrefs(q=True, inputSpaceNames=True)

   int cmEnabled = 0;
   MGlobal::executeCommand("colorManagementPrefs -q -cmEnabled", cmEnabled);

   MString renderingSpace = "";
   //MString colorConfig = "";

   if(cmEnabled)
   {
      /*
      int configFileEnabled = 0;
      MGlobal::executeCommand("colorManagementPrefs -q -cmConfigFileEnabled", configFileEnabled);

      if (configFileEnabled)
         MGlobal::executeCommand("colorManagementPrefs -q -configFilePath", colorConfig);
      else
         MGlobal::executeCommand("internalVar -userPrefDir", colorConfig);         
      
      */
      MGlobal::executeCommand("colorManagementPrefs -q -renderingSpaceName", renderingSpace);      
   }

   unordered_map<std::string, std::string> textureColorSpaces;

   MStringArray expandedFilenames;
   std::string txArguments;
   std::vector<std::string> listTextures;
   std::vector<bool> listConvertTx;
   std::vector<std::string> listArguments;
   
   std::vector<AtNode *> listNodes;
   MStringArray  listFullPaths;

   MString txFilename;


   listNodes.reserve(textureNodes.size());
   listTextures.reserve(textureNodes.size());
   listArguments.reserve(textureNodes.size());

//============= Part 1 : get the list of textures that  need to be converted to TX
// or replaced by the .tx version


   for (size_t i = 0; i < textureNodes.size(); ++i)
   {
      CNodeTranslator *translator = textureNodes[i];
      if (translator == NULL) continue;

      AtNode *node = translator->GetArnoldNode();
      if (node == NULL) continue;
      
      MString filename = AiNodeGetStr(node, "filename").c_str();
      if (filename.length() == 0)
         continue;

      std::string filenameStr = filename.asChar();

      MPlug autoTxPlug = translator->FindMayaPlug("autoTx");
      if (autoTxPlug.isNull())
         autoTxPlug = translator->FindMayaPlug("aiAutoTx");

      bool fileAutoTx = autoTx && (!autoTxPlug.isNull()) && autoTxPlug.asBool();

      MString colorSpace = translator->FindMayaPlug("colorSpace").asString();
      std::string colorSpaceStr = colorSpace.asChar();

      unordered_map<std::string, std::string>::iterator it = textureColorSpaces.find(filenameStr);
      if (it == textureColorSpaces.end())
      {
         textureColorSpaces[filenameStr] = colorSpaceStr;
      } else
      {
         // already dealt with this filename, skip the auto-tx
         if (colorSpaceStr != it->second)
         {
            AiMsgWarning("[mtoa.autotx]  %s is referenced multiple times with different color spaces", filename.asChar());
         }

         fileAutoTx = false; // => we don't want that texture to be converted to TX   
      }

      // Xheck texture extension, if .tx => set fileAutoTx = false
      int filenameLength = filename.numChars();

      // empty filename, nothing to do
      if (filenameLength == 0)
         continue;

      if (filenameLength > 4 && filename.substring(filenameLength - 3, filenameLength - 1) == ".tx")
         fileAutoTx = false;
      

      // no auto-tx, no use tx, nothing to do here
      if ((!fileAutoTx) && (!useTx))
         continue;

      MString searchPath = "";
      MString searchFilename = filename;
      

      // First we need to get the path to the expanded filename(s)
      expandedFilenames = expandFilename(searchFilename);

      // Now expandedFilenames contains the list of files found on disk that could match
      // the eventual tokens

      // if the file wasn't found on disk, we should check in the search paths
      if (expandedFilenames.length() == 0)
      {
         for (unsigned int t = 0; t < searchPaths.length(); ++t)
         {
            searchPath = searchPaths[t];
            searchFilename = searchPath + filename;
            expandedFilenames = expandFilename(searchFilename);

            // found some files, no need to continue
            if (expandedFilenames.length() > 0)
               break;

         }
      }

      // append the AtNode as well as its resolved filename
      // so that use-tx can verify if the tx file does exist
      // (even though we're not converting it to TX now)
      listNodes.push_back(node);

      if (expandedFilenames.length() > 0)
         listFullPaths.append(searchFilename);
      else
         listFullPaths.append("");


      if (!fileAutoTx)
         continue;

      txArguments = "-v -u --unpremult --oiio";
      if (cmEnabled && colorSpace != renderingSpace && colorSpace.length() > 0)
      {
         //txArguments += " --colorengine syncolor --colorconfig ";
         //txArguments += colorConfig.asChar();
         txArguments += " --colorconvert \"";
         txArguments += colorSpace.asChar();
         txArguments += "\" \"";
         txArguments += renderingSpace.asChar();
         txArguments += "\"";
      }

      // now add the current expanded filenames to the total list of files to process.
      // We also store the list of files that require to be converted to TX.
      // Note that the full textures list is necessary for "use tx", since all textures filenames
      // must be replaced
      for (unsigned int t = 0; t < expandedFilenames.length(); ++t)
      {
         listTextures.push_back(expandedFilenames[t].asChar());
         txFilename = expandedFilenames[t].substring(0, expandedFilenames[t].rindexW(".")) + MString("tx");
         // need to invalidate the TX file from the cache otherwise the conversion to TX will faill on windows
         AiTextureInvalidate(AtString(txFilename.asChar()));

         listArguments.push_back(txArguments);
      }
   }

//================= Part 2 : run MakeTX on the necessary textures   

   MString arv_msg("Converting ");
   arv_msg += (int) listTextures.size();
   arv_msg += " textures to .TX....";
   CMayaScene::GetRenderSession()->SetRenderViewStatusInfo(arv_msg);

      

   // We now have the full list of textures, let's loop over them
   for (unsigned int i = 0; i < listTextures.size(); ++i)
   {      
      // now call AiMakeTx with the corresponding arguments (including color space)
      AiMakeTx(listTextures[i].c_str(), listArguments[i].c_str());
   }

   // we told arnold to run TX conversion for the previous files

   AtMakeTxStatus *status;
   const char** source_filenames;
   unsigned num_submitted_textures;
   
   bool progressStarted = false;
   bool invalidProgressWin = false;
   unsigned int num_jobs_left = listTextures.size();
   while ( num_jobs_left > 0)
   {
      num_jobs_left = AiMakeTxWaitJob(status, source_filenames, num_submitted_textures);

      if (num_jobs_left >= num_submitted_textures)
         continue; // can this even happen ?
      
      
      int index = num_submitted_textures - num_jobs_left - 1;
      
      if (status[index] == AiTxError)
         AiMsgError("[maketx] Couldn't convert the texture to TX %s", source_filenames[index]);
      else if (status[index] == AiTxUpdated)
      {
         if (MtoaTranslationInfo())
            MtoaDebugLog("[maketx] Successfully converted texture to TX " + MString(source_filenames[index]));


         if ((progressBar) && (!progressStarted))
         {
            // need to start progress bar
            MProgressWindow::reserve();
            MProgressWindow::setProgressRange(0, 100);
            MProgressWindow::setTitle("Converting Images to TX");
            MProgressWindow::setInterruptable(true);

            progressStarted = true;

            // if the progress bar was already cancelled before it started
            // (it seems that it happens sometimes...), the we simply
            // don't test for cancel anymore
            if (MProgressWindow::isCancelled()) invalidProgressWin = true;
            else
            {
               MProgressWindow::startProgress();
               // strange, but I need to change the value once so that it is displayed
               MProgressWindow::setProgress(1);
               MProgressWindow::setProgress(0);
            }
         }
      }
      if ((progressBar) && (!invalidProgressWin) && MProgressWindow::isCancelled()) 
      {
         // FIXME is there a way to interrupt the conversion ?

         // FIXME show a confirm dialog to mention color management will be wrong
         //MString cmd;
         //cmd.format("import maya.cmds as cmds; cmds.confirmDialog(title='Warning', message='Color Management will be invalid if TX files aren't generated', button='Ok')");
         //MGlobal::executePythonCommandStringResult(cmd);

         // if progress was cancelled we consider that auto-Tx is OFF
         // but we still need to handle "use Tx"
         MProgressWindow::endProgress();
         AiMakeTxAbort(status, source_filenames, num_submitted_textures); // This tells arnold to abort conversion
         break;
      }

      if (progressBar && progressStarted)
      {

         // shouldn't happen, until last texture
         if (index + 1 >= (int)listTextures.size() )
            continue;

         // FIXME use basename instead         
         MString progressStatus(listTextures[index + 1].c_str());
         int basenameIndex = progressStatus.rindexW('/');
         if (basenameIndex > 0)
         {
            progressStatus = progressStatus.substring(basenameIndex + 1, progressStatus.numChars() - 1);
         }
         progressStatus += " (";
         progressStatus += (unsigned int)(index + 1);
         progressStatus += "/";
         progressStatus += (unsigned int)textureNodes.size();
         progressStatus += ")";

         while (progressStatus.length() < 50)
         {
            progressStatus += "    ";
         }

         MProgressWindow::setProgressStatus(progressStatus);
         MProgressWindow::setProgress(index * 100 / textureNodes.size());
      }      
   }
   if (progressBar && progressStarted)
      MProgressWindow::endProgress();
    
   // invalidate these textures so that they're not in the cache anymore
   for (unsigned int i = 0; i < listTextures.size(); ++i)
   { 
      txFilename = MString(listTextures[i].c_str()); 
      txFilename = txFilename.substring(0, txFilename.rindexW(".")) + MString("tx");

      // need to invalidate the TX files now that conversion was done (otherwise arnold keeps a handle to the file)
      AiTextureInvalidate(AtString(txFilename.asChar()));     

   }
//============= Part 3 : Use existing TX. Loop over the list of nodes and eventually replace the extension
//  by .tx. 

   // FIXME now that we're out of Maya, should we multi-thread this ?
   if (useTx)
   {
      // loop over listNodes
      // get the previously used search path
      for (size_t i = 0; i < listNodes.size(); ++i)
      {
         AtNode *imgNode = listNodes[i];
         if (imgNode == NULL)
            continue;

         MString filename = MString(AiNodeGetStr(imgNode, "filename").c_str());

         MString txFilename = (listFullPaths[i].numChars() > 0) ? listFullPaths[i] : filename;
         txFilename = txFilename.substring(0, txFilename.rindexW(".")) + MString("tx");

         MStringArray expandedFilenames = expandFilename(txFilename);

         // if no TX file was found, check the search paths,
         // but only do this if the current search path (listFullPaths[i]) was empty,
         // which happens if the original texture wasn't found on disk
         if (expandedFilenames.length() == 0 && listFullPaths[i].numChars() == 0)
         {            
            for (unsigned int s = 0; s < searchPaths.length(); ++s)
            {
               MString searchFilename = searchPaths[s] + txFilename;
               expandedFilenames = expandFilename(searchFilename);
               
               // we found the texture, stop searching
               if (expandedFilenames.length() > 0) break;
            }
         }

         // TX files were found, we can replace the extension to TX
         if (expandedFilenames.length() > 0)
         {
            filename = txFilename;
            FormatTexturePath(filename);
            AiNodeSetStr(imgNode, "filename", filename.asChar()); 
            // since we replace the filename by TX we need to reset the color space
            AiNodeSetStr(imgNode, "color_space", AtString(""));
         }
      }
   }
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
   MString hashCode;
   handle.GetHashString(hashCode);
   std::string hashStr(hashCode.asChar());

   ObjectToTranslatorMap::iterator it = m_processedTranslators.find(hashStr);
   if (it != m_processedTranslators.end())
   {
      it->second->m_impl->RemoveUpdateCallbacks();
      it->second->RequestUpdate();
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

void CArnoldSession::ExportImagePlane()
{
   MDagPath camera = m_sessionOptions.GetExportCamera();

   MFnDependencyNode fnNode (camera.node());
   MPlug imagePlanePlug = fnNode.findPlug("imagePlane", true);

   AtNode *options = AiUniverseGetOptions();

   CNodeTranslator *imgTranslator = NULL;
   MStatus status;

   AiNodeSetPtr(options, "background", NULL);

   if (imagePlanePlug.numConnectedElements() == 0)
      return;

   for (unsigned int ips = 0; (ips < imagePlanePlug.numElements()); ips++)
   {
      MPlugArray connectedPlugs;
      MPlug imagePlaneNodePlug = imagePlanePlug.elementByPhysicalIndex(ips);
      imagePlaneNodePlug.connectedTo(connectedPlugs, true, false, &status);


      if (status && (connectedPlugs.length() > 0))
      {
         imgTranslator = ExportNode(connectedPlugs[0], true);
         CImagePlaneTranslator *imgPlaneTranslator =  dynamic_cast<CImagePlaneTranslator*>(imgTranslator);

         if (imgPlaneTranslator)
         {
            imgPlaneTranslator->SetCamera(fnNode.name());

            AtNode *imgPlaneShader = imgPlaneTranslator->m_impl->m_atRoot;
            
            if (imgPlaneShader)      
            {
               AiNodeSetPtr(options, "background", imgPlaneShader);
               AiNodeSetByte(options, "background_visibility", 1);
            }
         }
      }
   }
}


