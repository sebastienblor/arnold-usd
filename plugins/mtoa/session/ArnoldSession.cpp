#include "ArnoldSession.h"
#include <ai_msg.h>
#include <ai_nodes.h>
#include <assert.h>
#include <stdio.h>
#include <maya/MGlobal.h>
#include <maya/MProgressWindow.h>
#include <maya/MPlugArray.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MFnSet.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MEventMessage.h>
#include <maya/MDagMessage.h>
#include <maya/M3dView.h>
#include "translators/NodeTranslatorImpl.h"
#include "extension/ExtensionsManager.h"
#include "utils/MakeTx.h"
#include "nodes/ShaderUtils.h"
#include "attributes/Components.h"


CArnoldSession::CArnoldSession(bool initOptions, bool defaultUniverse)
{
   MSelectionList list;

   /* This will be initialized when session options will call GetDefaultArnoldRenderOptions();
   MObject optNode = GetDefaultArnoldRenderOptions();
   if (optNode.isNull())
   {
      // defaultArnoldRenderOptions doesn't exist, we need to initialize it
      MGlobal::executePythonCommand("import mtoa.core;mtoa.core.createOptions()"); 
      
   }*/
   m_universe = (defaultUniverse) ? nullptr : AiUniverse();
   m_isExportingMotion = false;
   m_renderSession = nullptr;
   m_motionStep = 0;
   m_updateOptions = false;
   m_updateMotion = false;
   m_updateLightLinks = false;
   m_updateTx = false;
   m_checkVisibility = true;
   m_checkRenderLayer = true;
   m_batch = false;
   m_optionsTranslator = nullptr;

   m_requestUpdate = false;
   m_newNodeCallbackId = 0;
   m_addParentCallbackId = 0;
   m_removeParentCallbackId = 0;
   m_updateCallbacks = false;
   m_hiddenNodeCb = 0;
   m_lightLinks.SetOptions(&m_sessionOptions);
   m_lightLinks.SetUniverse(m_universe);
   if (initOptions)
      InitSessionOptions();
}

CArnoldSession::~CArnoldSession()
{
   ObjectToTranslatorMap::iterator it = m_translators.begin();
   ObjectToTranslatorMap::iterator itEnd = m_translators.end();
   for ( ; it != itEnd; ++it)
      delete it->second;
   
   if (m_renderSession)
   {
      AiRenderInterrupt(m_renderSession, AI_BLOCKING);
      AiRenderSessionDestroy(m_renderSession);
      m_renderSession = nullptr;
   }

   m_translators.clear();
   if (m_universe)
      AiUniverseDestroy(m_universe);
   
   ClearUpdateCallbacks();
   m_masterInstances.clear();
}

void CArnoldSession::InitSessionOptions()
{
   m_updateOptions = false;
   m_sessionOptions.Update();
}

void CArnoldSession::UpdateSessionOptions()
{
   InitSessionOptions();
}

template <class T>
static void ChangeCurrentFrame(T time, bool forceViewport = false)
{   
   // time can be either MTime or double

   // Ensure we don't do anything for AVP
   /*
   FIXME
   if (sessionMode == MTOA_SESSION_RENDERVIEW && CRenderSession::IsViewportRendering())
      return;
      */
   MTime currentTime = MAnimControl::currentTime();
   if (currentTime == time)
      return;

   // viewFrame will force the refresh of the viewport. This seems to be needed in batch render
   // to ensure that the DG data is properly set to the "current" frame. Otherwise, in some test scenes, 
   // we can get wrong data (see #4447). So we want to keep using viewFrame for the main current frame.
   // However, for other "motion" frames, it's better to use "setCurrentTime" which doesn't force a refresh 
   // of the viewport (until next maya "idle").
   if (forceViewport)
      MGlobal::viewFrame(time); 
   else
      MAnimControl::setCurrentTime(time); 
}


AtRenderSession *CArnoldSession::GetRenderSession()
{
   if (m_renderSession == nullptr)
      m_renderSession = AiRenderSession(m_universe, IsBatchSession() ? AI_SESSION_BATCH : AI_SESSION_INTERACTIVE);
   return m_renderSession;
}
MDagPathArray CArnoldSession::GetRenderCameras(bool activeView)
{
   MDagPathArray cameras;
   MDagPath activeCameraPath;
   // optionally include the active camera in the viewport
   if (activeView)
   {
      M3dView view;
 
      MStatus viewStatus;
      view = M3dView::active3dView(&viewStatus);
      if (viewStatus == MS::kSuccess && view.getCamera(activeCameraPath) == MS::kSuccess)
      {
         cameras.append(activeCameraPath);
      }
   }

   MItDag dagIter(MItDag::kDepthFirst, MFn::kCamera);
   MDagPath cameraPath;
   // MFnCamera cameraNode;
   MFnDagNode cameraNode;
   MPlug renderable;
   MStatus stat;
   while (!dagIter.isDone())
   {
      dagIter.getPath(cameraPath);
      dagIter.next();
      // don't need to include the active camera twice
      if (cameraPath == activeCameraPath)
         continue;

      cameraNode.setObject(cameraPath);
      renderable = cameraNode.findPlug("renderable", true, &stat);
      if (stat && renderable.asBool())
         cameras.append(cameraPath);
      
   }

   return cameras;
}


namespace // <anonymous>
{

   bool IsVisible(MFnDagNode &node)
   {
      MStatus status;

      if (node.isIntermediateObject())
         return false;


      MPlug visPlug = node.findPlug("visibility", true, &status);
      // Check standard visibility
      if (status == MStatus::kFailure || !visPlug.asBool())
         return false;

      // LOD visibility support had first been implemented for mesh lights, 
      // but it was then reverted in #2679 , because some users were relying
      // on this attribute to be ignored by Arnold. This was, back at the time, 
      // the only was to hide an object from the viewport and still render it.
      // There are now different ways to achieve this, so we're re-introducing the
      // support for lodVisibility in #4259
      
      MPlug lodVisPlug = node.findPlug("lodVisibility", true, &status);
      if (status == MStatus::kFailure || !lodVisPlug.asBool())
         return false;

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

   inline bool IsVisiblePath(MDagPath dagPath)
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

   // TODO: use the renderLayer specified in the CMayaScene instead
   inline bool IsInRenderLayer(MDagPath dagPath)
   {
      MObject renderLayerObj = MFnRenderLayer::currentLayer();

      MFnRenderLayer curLayer(renderLayerObj);

      return curLayer.inCurrentRenderLayer(dagPath);
   }

   inline bool IsTemplated(MFnDagNode & node)
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

   inline bool IsTemplatedPath(MDagPath dagPath)
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
   // Filter and expand the selection, and all its hirarchy down stream.
   // Also flattens sets in selection.
   //
   // @return              MS::kSuccess / MS::kFailure is returned in case of failure.
   //
   MStatus FlattenSelection(CArnoldSession *session, MSelectionList* selected, bool skipRoot = false)
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
            if (session->IsExportable(path) == CArnoldSession::MTOA_EXPORT_ACCEPTED)
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
                  if (MStatus::kSuccess != FlattenSelection(session, &children, true)) // true = skipRoot
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
               if (MStatus::kSuccess != FlattenSelection(session, &children, true)) // true = skipRoot
                  status = MStatus::kFailure;
               selected->merge(children);
            }
            else
            {
               // Keep all the DependencyNodes (shaders, etc...) in the selected list
               // because we want to ensure that they're exported #4148
               selected->add(node);
            }
         }
         else
         {
            status = MStatus::kFailure;
         }
      }

      return status;
   }
}

void CArnoldSession::Clear()
{
   // First ensure there's no render in progress for this universe
   if (m_renderSession)
   {
      AiRenderInterrupt(m_renderSession, AI_BLOCKING);
      AiRenderSessionDestroy(m_renderSession);
      m_renderSession = nullptr;
   }

   // Deleting the translators will delete the AtNodes
   ObjectToTranslatorMap::iterator it = m_translators.begin();
   ObjectToTranslatorMap::iterator itEnd = m_translators.end();
   for ( ; it != itEnd; ++it)
      delete it->second;
   
   m_translators.clear();
   // if we had an explicit universe, let's create a new one
   if (m_universe)
   {      
      AiUniverseDestroy(m_universe);
      m_universe = AiUniverse();
   }
   
   m_lightLinks.ClearLightLinks();
   m_lightLinks.SetUniverse(m_universe);
   m_updateLightLinks = true;
   // ClearUpdateCallbacks(); we shouldn't have to clear the update callbacks, since the session type is not meant to change
   m_masterInstances.clear();
   m_optionsTranslator = nullptr;
   m_updateTxFiles.clear();
 
}

void CArnoldSession::Export(MSelectionList* selected)
{
   MStatus status;
   
   // It wouldn't be efficient to test the whole scene against selection state
   // so selected gets a special treatment
   if (selected)
   {
      unsigned int ns = selected->length();
      status = FlattenSelection(this, selected, false); // false = don't skip root nodes (ticket #1061)
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
   //ArnoldSessionMode exportMode = m_sessionOptions.m_mode;
   m_motionStep = 0;

   SetStatus(MString("Exporting Arnold Scene..."));

   // If the options have changed, we need to update CSessionOptions
   if (m_updateOptions)
   {
      UpdateSessionOptions();
   }


   // Are we motion blurred (any type)?
   const bool mb = m_sessionOptions.IsMotionBlurEnabled();

   if (MtoaTranslationInfo())
   {
      MString log = "[mtoa.session]     Initializing at frame ";
      log += m_sessionOptions.GetExportFrame();
      MtoaDebugLog(log);
   }
   ExportOptions();  // inside loop so that we're on the proper frame

   // First "real" export
   ChangeCurrentFrame(m_sessionOptions.m_frame, true);

   if (IsFileExport() && selected)
   {
      // If we export selected to a file, not as a full render,
      // we don't need to export all lights / cameras, but
      // we export the selected ones first
      status = ExportCameras(selected);
      status = ExportLights(selected);
      status = ExportDag(selected);

      // Ensure that any node that is selected is properly exported.
      // Dag nodes were already treated above, so we only consider non-dag nodes below
      MItSelectionList it(*selected, MFn::kInvalid, &status);
      MDagPath selectedPath;
      MObject selectedNode;
      for (it.reset(); !it.isDone(); it.next())
      {
         if (it.getDagPath(selectedPath) == MStatus::kSuccess)
            continue; // dag node, no need to consider it
         
         if (it.getDependNode (selectedNode) != MStatus::kSuccess)
            continue;
         MPlug nodePlug = (!selectedNode.isNull()) ? MFnDependencyNode(selectedNode).findPlug("message", true) : MPlug();
         if (!nodePlug.isNull())
            ExportNode(nodePlug);            
      }
   } else
   {
      // Either for a specific camera or export all cameras
      // Note : in "render selected" mode Maya exports all lights and cameras
      /* FIXME !!! we used to do this, do we want to keep it ???
      if (m_sessionOptions.m_camera.isValid())
      {
         m_sessionOptions.m_camera.extendToShape();
         ExportDagPath(m_sessionOptions.m_camera, true);
      }
      else
      {
         status = ExportCameras();   
      }
      */


      status = ExportCameras();

      // For render selected we need all the lights (including unselected ones)
      status = ExportLights();
      status = ExportDag(selected);
   }

   // Eventually export all shading groups
   if (m_sessionOptions.GetExportAllShadingGroups())
   {
      MStringArray shadingGroups;
      if (selected)
         MGlobal::executeCommand("ls -sl -typ shadingEngine", shadingGroups); // get selected shading groups and export them
      else
         MGlobal::executeCommand("ls -typ shadingEngine", shadingGroups); // get all shading groups in the scene and export them
      

      unsigned int mask = m_sessionOptions.outputAssMask();
      bool exportShadingGroupData = (mask & AI_NODE_SHADER) && !(mask & AI_NODE_SHAPE);

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
                  CNodeTranslator* shaderTr = ExportNode(connections[0]);
                  AtNode* shaderNode = (shaderTr) ? shaderTr->m_impl->m_atRoot : nullptr;
                  // When exporting to .ass, if shaders are exported but not shapes,
                  // we want to keep track of the shading engine assignments (surface, volume, displacement)
                  // so that we can eventually restore them later on when importing them (#4033)
                  if (exportShadingGroupData && shaderNode)
                  {                     
                     // Set the user data material_surface / material_volume on the root shader, 
                     // so that it returns the name of the shading engine
                     std::string user_data;
                     if (shaderAttrs[a] == MString("aiVolumeShader") || shaderAttrs[a] == MString("volumeShader"))
                        user_data = "material_volume";
                     else if (shaderAttrs[a] == MString("aiSurfaceShader") || shaderAttrs[a] == MString("surfaceShader"))
                        user_data = "material_surface";
                     else if (shaderAttrs[a] == MString("displacementShader"))
                        user_data = "material_displacement";
                     
                     if (AiNodeLookUpUserParameter(shaderNode, user_data.c_str()) == NULL)
                        AiNodeDeclare(shaderNode, user_data.c_str(), "constant STRING");   
                     AiNodeSetStr(shaderNode, user_data.c_str(), AtString(shEngineNode.name().asChar()));
                  }

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
   translatorsToExport.reserve(m_translators.size());
   ObjectToTranslatorMap::iterator it = m_translators.begin();
   ObjectToTranslatorMap::iterator itEnd = m_translators.end();
   for (; it != itEnd; ++it)
      if (it->second) translatorsToExport.push_back(it->second);
   
   // First export the current frame 
   m_isExportingMotion = false;
   // set motion step
   m_motionStep = 0;
   int currentFrameIndex = -1;
   if (mb)
   {
      for (size_t i = 0; i < m_sessionOptions.m_motion_frames.size(); ++i)
      {
         if (std::abs(m_sessionOptions.m_motion_frames[i] - m_sessionOptions.m_frame) < 
            (m_sessionOptions.m_motion_frames[1] - m_sessionOptions.m_motion_frames[0]) /10.)
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
      translatorsToExport.reserve(m_translators.size());
      it = m_translators.begin();
      itEnd = m_translators.end();
      for (; it != itEnd; ++it)
         if (it->second) translatorsToExport.push_back(it->second);

      // now loop through motion steps
      unsigned int numSteps = m_sessionOptions.GetNumMotionSteps();
      m_isExportingMotion = true;
      
      SetStatus(MString("Exporting Motion Data..."));

      for (unsigned int step = 0; step < numSteps; ++step)
      {
         if (step == (unsigned int)currentFrameIndex)
            continue; // current frame, has already been exported above
         ChangeCurrentFrame(MTime(m_sessionOptions.m_motion_frames[step], MTime::uiUnit()));
         
         if (MtoaTranslationInfo())
         {
            MString log = "[mtoa.session]     Exporting step ";
            log += step + 1;
            log += " of ";
            log += numSteps;
            log += " at frame ";
            log +=  m_sessionOptions.m_motion_frames[step];
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


      ChangeCurrentFrame(MTime(m_sessionOptions.GetExportFrame(), MTime::uiUnit()));
      m_isExportingMotion = false;
   }

   m_motionStep = 0;

   if (m_updateCallbacks)
   {
      ObjectToTranslatorMap::iterator it = m_translators.begin();
      ObjectToTranslatorMap::iterator itEnd = m_translators.end();

      for ( ; it != itEnd; ++it)
      {
         CNodeTranslator *nodeTr = it->second;
         if (nodeTr == NULL) continue;
         CNodeTranslatorImpl *trImpl = nodeTr->m_impl;
         if (trImpl->m_mayaCallbackIDs.length() == 0)
            nodeTr->AddUpdateCallbacks();
         trImpl->m_updateMode = CNodeTranslator::AI_UPDATE_ONLY;
         trImpl->m_inUpdateQueue = false; // allow to trigger new updates
         trImpl->m_isExported = true; // next export will be an update
         // for motion blur, check which nodes are static and which aren't (#2316)
         if (mb ) // && GetNumMotionSteps() > 1
         {
            trImpl->m_animArrays = trImpl->HasAnimatedArrays();
         } else trImpl->m_animArrays = false;
      }
      m_objectsToUpdate.clear(); // I finished exporting, I don't have any other object to Update now
   }
    
   // it would seem correct to only call ExportTxFiles if m_updateTx = true
   ExportTxFiles();

   // Execute post export callback
   // Do we also want to do it in Update() ? 
   
   MFnDependencyNode fnArnoldRenderOptions(m_sessionOptions.GetArnoldRenderOptions());
   MString postTranslationCallbackScript = fnArnoldRenderOptions.findPlug("post_translation", true).asString();
   MGlobal::executeCommand(postTranslationCallbackScript);
}

void CArnoldSession::Update()
{
   bool mtoa_translation_info = MtoaTranslationInfo();

   if (mtoa_translation_info)
      MtoaDebugLog("[mtoa.session]    Updating Arnold Scene....");
   
   SetStatus(MString("Updating Arnold Scene..."));
   MStatus status;

   std::vector< CNodeTranslator * > translatorsToUpdate;
   std::vector<ObjectToTranslatorPair>::iterator itObj;
   size_t objectsToUpdateCount;
   bool newDag, exportMotion, motionBlur, mbRequiresFrameChange;
   int currentFrameIndex;
   MString hashCode;
   std::string hashStr;
   
   double frame = MAnimControl::currentTime().as(MTime::uiUnit());
   bool frameChanged = (frame != m_sessionOptions.GetExportFrame());

   // only change the frame for interactive renders
   // It appears that Export() doesn't restore the current frame
   // in maya otherwise ( to avoid useless maya evaluations )
   if (frameChanged && m_updateCallbacks) m_sessionOptions.SetExportFrame(frame);


   std::vector<bool> prevRequiresMotion;

   size_t previousUpdatedTranslators = 0;

   int updateRecursions = 0;
   static const int maxUpdateRecursions = 5;

   // If we need to update the motion data, we first have to check 
   // which translators used to require motion blur, BEFORE we update the session options
   if (m_updateMotion)
   {  
      if (mtoa_translation_info)
         MtoaDebugLog("[mtoa.session]    Updating Motion Blur data");

      
      prevRequiresMotion.reserve(m_translators.size());

      // stores requiresMotionData from all translators
      auto it = m_translators.begin();
      auto itEnd = m_translators.end();
      for ( ; it != itEnd; ++it)
         prevRequiresMotion.push_back(it->second->RequiresMotionData());

   }

   if (m_updateOptions)
   {
      if (mtoa_translation_info)
         MtoaDebugLog("[mtoa.session]     Updating Session Options");

      UpdateSessionOptions();
   }

   // Now check again all translators
   if (m_updateMotion)
   {
      int trIdx = 0;
      auto it = m_translators.begin();
      auto itEnd = m_translators.end();
      for (; it != itEnd; ++it, ++trIdx)
      {         
         if (trIdx < prevRequiresMotion.size() && prevRequiresMotion[trIdx])
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

         // If motion blur changes, we need to ensure that the render camera is updated.
         // In particular, we need the shutter_start / shutter_end to be updated properly (see #4126)
         CNodeTranslator *cameraTranslator = GetActiveTranslator(CNodeAttrHandle(m_sessionOptions.GetExportCamera()));
         if (cameraTranslator)
            cameraTranslator->RequestUpdate();
      }
      
      m_sessionOptions.UpdateMotionFrames();
      m_updateMotion = false;
   }


   exportMotion = false;
   motionBlur = m_sessionOptions.IsMotionBlurEnabled();
   mbRequiresFrameChange = false;
   m_isExportingMotion = false;

UPDATE_BEGIN:

   newDag = false;
   
   m_motionStep = 0;
   const std::vector<double> &motionFrames = m_sessionOptions.GetMotionFrames();
     
   currentFrameIndex = -1;
   if (motionBlur)
   {
      for (size_t i = 0; i < motionFrames.size(); ++i)
      {
         if (std::abs(motionFrames[i] - m_sessionOptions.m_frame) < 
            (motionFrames[1] - motionFrames[0]) /10.)
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
         
         m_translators.erase(hashStr); 

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
            //m_translators.erase(handle);

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

                  // If this isn't the primary instance
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
   bool isLightLinksDirty = m_updateLightLinks;
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
      for(it = m_translators.begin(); it != m_translators.end(); ++it)
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
         m_lightLinks.ExportLightLinking(node, tr->GetMayaDagPath());
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
      SetStatus(MString("Exporting Motion Data..."));

      // Scene is motion blured, get the data for the steps.
      unsigned int numSteps = m_sessionOptions.GetNumMotionSteps();

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
            log += motionFrames[step];
            log += " for ";
            log += (int)translatorsToUpdate.size();
            log += " nodes";
            MtoaDebugLog(log);
         }

         // if none of the objects arrays have proven to be animated, 
         // we don't need to change the current frame. However we still need to process
         // the export for every step. Otherwise some AtArray keys could be missing
         if (mbRequiresFrameChange)
            ChangeCurrentFrame(MTime(motionFrames[step], MTime::uiUnit()));

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
         ChangeCurrentFrame(MTime(m_sessionOptions.GetExportFrame(), MTime::uiUnit()));

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
      if (m_updateCallbacks && (trImpl->m_mayaCallbackIDs.length() == 0))
         translator->AddUpdateCallbacks();
      
      trImpl->m_inUpdateQueue = false; // I'm allowed to receive updates once again
      trImpl->m_isExported = true;
      // restore the update mode to "update Only"
      trImpl->m_updateMode = CNodeTranslator::AI_UPDATE_ONLY;
   }
      
   m_objectsToUpdate.clear();
   m_requestUpdate = false;
}



/// Determine if the DAG node should be skipped.
/// The MDGContext defines the frame at which to test visibility
CArnoldSession::DagFiltered CArnoldSession::IsExportable(const MDagPath &path) const
{
   if (IsTemplatedPath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;
   if (m_checkVisibility && !IsVisiblePath(path))
      return MTOA_EXPORT_REJECTED_BRANCH;
   // if we're exporting a selection, we don't want to check for render layers
   if (m_checkRenderLayer && !IsInRenderLayer(path))
      return MTOA_EXPORT_REJECTED_NODE;

   return MTOA_EXPORT_ACCEPTED;
}

void CArnoldSession::SetStatus(MString status)
{
   MGlobal::displayInfo(status);
}

void CArnoldSession::RequestUpdateTx(const std::string &filename, const std::string &colorSpace)
{
   if (filename.empty())
      return;
   size_t filenameLength = filename.length();
   std::string extension = (filenameLength > 4) ? filename.substr(filenameLength - 3) : "";

   if (extension == ".tx" || extension == ".TX")
      return;

   m_updateTx = true; 
   auto iter = m_updateTxFiles.find(filename);
   if (iter != m_updateTxFiles.end())
   {
      // We already have that filename is our list
      if (colorSpace == iter->second)
         return;

      // At this point we have the same filename being used multiple times, but with different color spaces.
      // We can't handle this currently, so we need to raise an error
      AiMsgWarning("[mtoa.autotx]  %s is referenced multiple times with different color spaces. Forcing to %s for all instances of this texture", filename.c_str(), colorSpace.c_str());
   }
   m_updateTxFiles[filename] = colorSpace;      
}

void CArnoldSession::ExportTxFiles()
{
   if (!m_sessionOptions.GetAutoTx())
      return;

   if (m_updateTxFiles.empty())
      return;
   
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.session]    Updating TX files");

   const MStringArray &searchPaths = m_sessionOptions.GetTextureSearchPaths();

   bool progressBar = (MGlobal::mayaState() == MGlobal::kInteractive);

   // First, let's get Color management configuration files

   // check if color management prefs is enabled
   // colorManagementPrefs(q=True, inputSpaceNames=True)

   int cmEnabled = 0;
   MGlobal::executeCommand("colorManagementPrefs -q -cmEnabled", cmEnabled);

   MString renderingSpace = "";
   //MString colorConfig = "";

   if(cmEnabled)
   {
      MGlobal::executeCommand("colorManagementPrefs -q -renderingSpaceName", renderingSpace);      
   }

   unordered_map<AtString, AtString, AtStringHash> textureColorSpaces;

   MStringArray expandedFilenames;
   std::string txArguments;
   std::vector<std::string> listTextures;
   std::vector<bool> listConvertTx;
   std::vector<std::string> listArguments;
   
   MStringArray  listFullPaths;

   MString txFilename;


   listTextures.reserve(m_updateTxFiles.size());
   listArguments.reserve(m_updateTxFiles.size());

//============= Part 1 : get the list of textures that  need to be converted to TX

   // Loop over the image nodes that requested a Tx update
   for (auto iter : m_updateTxFiles)
   {
      // We're assuming that only image shaders are in this list, OR
      // any other custom shader that has "filename" and "color_space" attributes
      const std::string &filename = iter.first;
      const std::string &colorSpace = iter.second;
      MString filenameStr(filename.c_str());
      MString colorSpaceStr(colorSpace.c_str());

      MString searchPath = "";
      MString searchFilename = filenameStr;
      
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
            searchFilename = searchPath + filenameStr;
            expandedFilenames = expandFilename(searchFilename);

            // found some files, no need to continue
            if (expandedFilenames.length() > 0)
               break;
         }
      }

      txArguments = "-v -u --unpremult --oiio";
      if (cmEnabled && colorSpaceStr != renderingSpace && colorSpace.length() > 0)
      {
         txArguments += " --colorconvert \"";
         txArguments += colorSpace;
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

         unsigned int bitdepth = 32;
         AiTextureGetBitDepth(expandedFilenames[t].asChar(), &bitdepth);

         std::string bitdepth_args = "";

         if (cmEnabled && colorSpaceStr != renderingSpace &&
            colorSpaceStr.toLowerCase() != MString("raw") && bitdepth <= 8)
         {
            bitdepth_args += " --format exr -d half --compression dwaa";
         }

         listArguments.push_back(txArguments + bitdepth_args);
      }
   }

//================= Part 2 : run MakeTX on the necessary textures   

   if (!listTextures.empty())
   {
      MString arv_msg("Converting ");
      arv_msg += (int) listTextures.size();
      arv_msg += " textures to .TX....";
      SetStatus(arv_msg);
   }

   // We now have the full list of textures, let's loop over them
   for (unsigned int i = 0; i < listTextures.size(); ++i)
   {
      // now call AiMakeTx with the corresponding arguments (including color space)
      AiMakeTx(listTextures[i].c_str(), listArguments[i].c_str(), m_universe);
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
         progressStatus += (unsigned int)m_updateTxFiles.size();
         progressStatus += ")";

         while (progressStatus.length() < 50)
         {
            progressStatus += "    ";
         }

         MProgressWindow::setProgressStatus(progressStatus);
         MProgressWindow::setProgress(index * 100 / m_updateTxFiles.size());
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
   m_updateTxFiles.clear();
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
   
   m_translators.erase(hashStr);
   // we're not deleting the translator yet
}

CNodeTranslator *CArnoldSession::GetActiveTranslator(const CNodeAttrHandle &handle)
{
   MString hashCode;
   handle.GetHashString(hashCode);
   std::string hashStr(hashCode.asChar());
   ObjectToTranslatorMap::iterator it = m_translators.find(hashStr);

   if (it != m_translators.end())
      return it->second;


   // FIXME, should we try now with the attribute name for multi-output translators ?
   /*
   handle.GetHashString(hashCode, true);
   hashStr= hashCode.asChar();
   ObjectToTranslatorMap::iterator it = m_translators.find(hashStr);

   if (it != m_translators.end())
      return it->second;   
   */

   return NULL;
}

/// Export the Arnold Render Options node
AtNode* CArnoldSession::ExportOptions()
{
   MObject options = m_sessionOptions.GetArnoldRenderOptions();

   if (options.isNull())
   {
      // Calling CSessionOptions::Update will initialize the options node to the 
      // default one (and create it if needed)
      m_sessionOptions.Update();
   }
   
   options = m_sessionOptions.GetArnoldRenderOptions();
   if (options.isNull())
   {
      AiMsgWarning("[mtoa] Failed to find Arnold options node");
      return NULL;
   }
   MStatus status;
   MFnDependencyNode fnNode(options, &status);
   if (status != MS::kSuccess)
   {
      AiMsgWarning("[mtoa] Invalid options node");
      return NULL;
   }
   if(MtoaTranslationInfo())
      MtoaDebugLog("[mtoa] Exporting Arnold options "+ fnNode.name());

   MPlug optPlug = fnNode.findPlug("message", true);
   m_optionsTranslator = (COptionsTranslator*)ExportNode(optPlug, false);
   if (m_optionsTranslator == nullptr)
   {
      AiMsgError("[mtoa] No translator found for the options node");
      return NULL;
   }
   ExportColorManager();
   return m_optionsTranslator->GetArnoldNode();
}

AtNode *CArnoldSession::ExportColorManager()
{
   // Export the color manager node
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
   return nullptr;
}

static inline bool IsArnoldLight(const MObject &object)
{
   MFnDependencyNode depFn(object);
   std::string classification(MFnDependencyNode::classification(depFn.typeName()).asChar());
   if (classification.find(CLASSIFY_ARNOLD_LIGHT.asChar()) != std::string::npos)
      return true;
   else
      return false;
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
            if (node.hasFn(MFn::kLight) || IsArnoldLight(node))
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
      for (; (!dagIterLights.isDone()); dagIterLights.next())
      {
         if (dagIterLights.getPath(path))
         {
            // Only check for lights being visible, not templated and in render layer
            // FIXME: does a light need to be in layer to render actually in Maya?
            MFnDagNode node(path.node());
            MString name = node.name();
            if (m_checkRenderLayer && !IsInRenderLayer(path))
               continue;
            if (IsTemplatedPath(path))
               continue;
            if (m_checkVisibility && !IsVisiblePath(path))
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
         if (IsArnoldLight(dagIterPlugin.currentItem()))
         {
            if (dagIterPlugin.getPath(path))
            {
               // Only check for lights being visible, not templated and in render layer
               // FIXME: does a light need to be in layer to render actually in Maya?
               MFnDagNode node(path.node());
               MString name = node.name();
               if (m_checkRenderLayer && !IsInRenderLayer(path))
                  continue;
               if (IsTemplatedPath(path))
                  continue;
               if (m_checkVisibility && !IsVisiblePath(path))
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
            filtered = IsExportable(path);
            if (filtered != MTOA_EXPORT_ACCEPTED)
            {
               if (m_updateCallbacks)
               {
                  HiddenObjectCallbackPair hiddenObj;
                  hiddenObj.first = CNodeAttrHandle(obj, "");
                  hiddenObj.second = MNodeMessage::addNodeDirtyPlugCallback(obj,
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



// Public Methods

// Export a single dag path (a dag node or an instance of a dag node)
// Considered to be already filtered and checked
CDagTranslator* CArnoldSession::ExportDagPath(const MDagPath &dagPath, bool initOnly, MStatus* stat)
{

   //m_motionStep = 0;
   MStatus status = MStatus::kSuccess;
   AtNode* arnoldNode = NULL;

   MString name = m_sessionOptions.GetArnoldNaming(dagPath);
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
   ObjectToTranslatorMap::iterator it = m_translators.find(hashStr);

   if (it != m_translators.end())
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
      if (it != m_translators.end())
      {
         it->second = translator;
      }
      else
      {
         m_translators[hashStr] = translator;
         // This node handle might have already been added to the list of objects to update
         // but since no translator was found in m_translators, it might have been discarded
         // if we don't QueueForUpdate now, addUpdateCallbacks could not be called and we'd loose all callbacks
         // for this shader
         if (m_updateCallbacks) QueueForUpdate(translator);
      }
      if (!initOnly)
         arnoldNode = translator->m_impl->DoExport();
   }

   if (NULL != stat) *stat = status;
   AiMsgTab(-1);
   return translator;
}

CNodeTranslator* CArnoldSession::ExportNode(const MObject &object, 
                                   bool initOnly, int instanceNumber, MStatus *stat)
{
   if (object.isNull())
   {
      if (stat)
         *stat = MS::kFailure;
      return nullptr;
   }
   MPlug nodePlug = MFnDependencyNode(object).findPlug("message", true);
   if (!nodePlug.isNull())
      return ExportNode(nodePlug, initOnly, instanceNumber, stat);            

   return nullptr;
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
   ObjectToTranslatorMap::iterator it = m_translators.find(hashStr);

   if (it != m_translators.end())
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
      if (it != m_translators.end())
      {
         it->second = translator;
      }
      else
      {
         m_translators[hashStr] = translator;
         
         // This node handle might have already been added to the list of objects to update
         // but since no translator was found in m_translators, it might have been discarded
         // if we don't QueueForUpdate now, addUpdateCallbacks could not be called and we'd loose all callbacks
         // for this shader
         if (m_updateCallbacks) QueueForUpdate(translator);
      }
      if (!initOnly)
         arnoldNode = translator->m_impl->DoExport();
   }
   if (NULL != stat) *stat = status;
   AiMsgTab(-1);
   return translator;
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
      MDagPathArray cameras = GetRenderCameras(false); // don't include the active view
      MStatus camStatus;
      for (unsigned int i = 0; i < cameras.length(); ++i)
      {
         ExportDagPath(cameras[i], true, &camStatus);
         if (camStatus != MStatus::kSuccess)
            status = MStatus::kFailure;
      }
   }

   return status;
}

void CArnoldSession::AddUpdateCallbacks()
{
   m_updateCallbacks = true;

   MCallbackId id;
   MStatus status;
   // Add the node added callback
   if (m_newNodeCallbackId == 0)
   {
      // We used to call the callback for every "dependNode", but now we're just doing it
      // for dag nodes. This is introduced for #2540 when hypershade is opened, but more generically
      // it shouldn't be necessary to restart when every node is created. Depend nodes should
      // only appear once they're connected to other exported nodes
      id = MDGMessage::addNodeAddedCallback(NewNodeCallback, "dagNode", (void*) this, &status);
      if (status == MS::kSuccess)
         m_newNodeCallbackId = id;
      else
         AiMsgError("[mtoa] Unable to setup IPR node added callback");
   }

   if (m_addParentCallbackId == 0)
   {
      id = MDagMessage::addParentAddedCallback(ParentingChangedCallback,  (void*) this, &status);
      if (status == MS::kSuccess)
         m_addParentCallbackId = id;
      else
         AiMsgError("[mtoa] Unable to setup IPR parent added callback");
      
   }
   if (m_removeParentCallbackId == 0)
   {
      id = MDagMessage::addParentRemovedCallback(ParentingChangedCallback,  (void*) this, &status);
      if (status == MS::kSuccess)
         m_removeParentCallbackId = id;
      else
         AiMsgError("[mtoa] Unable to setup IPR parent removed callback");
   }
   
   // TODO : might add a forceUpdateCallback to re-export when frame changes
   // static MCallbackId   addForceUpdateCallback (MMessage::MTimeFunction func, void *clientData=NULL, MStatus *ReturnStatus=NULL)
   // This method registers a callback that is called after the time changes and after all nodes have been evaluated in the dependency graph. 
   
   // If some translators were already processed, 
   ObjectToTranslatorMap::iterator it = m_translators.begin();
   ObjectToTranslatorMap::iterator itEnd = m_translators.end();
   const bool mb = m_sessionOptions.IsMotionBlurEnabled();
   for ( ; it != itEnd; ++it)
   {
      CNodeTranslator *nodeTr = it->second;
      if (nodeTr == NULL) continue;
      nodeTr->AddUpdateCallbacks();
         nodeTr->m_impl->m_updateMode = CNodeTranslator::AI_UPDATE_ONLY;
      nodeTr->m_impl->m_inUpdateQueue = false; // allow to trigger new updates
      // for motion blur, check which nodes are static and which aren't (#2316)
      if (mb && m_sessionOptions.GetNumMotionSteps() > 1)
      {
         nodeTr->m_impl->m_animArrays = nodeTr->m_impl->HasAnimatedArrays();
      } else nodeTr->m_impl->m_animArrays = false;
   }   
}

void CArnoldSession::ClearUpdateCallbacks()
{
   m_updateCallbacks = false;
   if (m_newNodeCallbackId != 0)
   {
      MMessage::removeCallback(m_newNodeCallbackId);
      m_newNodeCallbackId = 0;
   }
   if (m_addParentCallbackId != 0)
   {
      MMessage::removeCallback(m_addParentCallbackId);
      m_addParentCallbackId = 0;
   }
   if (m_removeParentCallbackId != 0)
   {
      MMessage::removeCallback(m_removeParentCallbackId);
      m_removeParentCallbackId = 0;
   }
   ObjectToTranslatorMap::iterator it = m_translators.begin();
   ObjectToTranslatorMap::iterator itEnd = m_translators.end();
   for ( ; it != itEnd; ++it)
   {
      CNodeTranslator *nodeTr = it->second;
      if (nodeTr == NULL) continue;
      nodeTr->m_impl->RemoveUpdateCallbacks();
   }
}

// We used to call the callback for every "dependNode", but now we're just doing it
// for dag nodes. This is introduced for #2540 when hypershade is opened, but more generically
// it shouldn't be necessary to restart when every node is created. Depend nodes should
// only appear once they're connected to other exported nodes
void CArnoldSession::NewNodeCallback(MObject & node, void *clientData)
{
   CArnoldSession* session = (CArnoldSession*)clientData;
   if (session)
      session->NewNode(node);
}
void CArnoldSession::NewNode(MObject &node)
{   
   MFnDependencyNode depNodeFn(node);
   MString type = depNodeFn.typeName();
   
   MString name = depNodeFn.name();
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.ipr] IPRNewNodeCallback on "+ MString(name.asChar())+" ("+MString(type.asChar())+")");
      
   
   MFnDagNode dagNodeFn(node);
   MDagPath path;
   const MStatus status = dagNodeFn.getPath(path);
   if (status == MS::kSuccess)
      QueueForUpdate(path);
   else
      QueueForUpdate(node);
   
   // FIXME : instead of testing specific types, we could 
   // simply get a translator for this type (as ArnoldSession::ExportDagPath does).
   // if no translator is provided then we skip it
   if(type == "transform" || type == "locator") return; // no need to do anything with a simple transform node

   // new cameras shouldn't restart IPR
   if (node.hasFn(MFn::kCamera)) 
      return;
   
   RequestUpdate();
}
void CArnoldSession::ParentingChangedCallback(MDagPath &child, MDagPath &parent, void *clientData)
{
   CArnoldSession* session = (CArnoldSession*)clientData;
   session->RecursiveUpdateDagChildren(child); 
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

   ObjectToTranslatorMap::iterator it = m_translators.find(hashStr);
   if (it != m_translators.end())
   {  

      CNodeTranslator *tr = it->second;
      if (tr)
      {
         tr->m_impl->RemoveUpdateCallbacks();

         // This node's name might have changed, we need to update it here #4238
         // Option 1 : re-generate the node
         tr->SetUpdateMode(CNodeTranslator::AI_RECREATE_NODE);

         // Option 2 : just rename the node, is this enough ? or would there be any reason
         // for things to be done differently in CNodeTranslator::CreateArnoldNode depending 
         // on the hierarchy ?

         /*   already commented !!!!
         AtNode *arnoldNode = tr->GetArnoldNode();
         if (arnoldNode)
         {
            MString oldName = AiNodeGetName(arnoldNode);
            MString newName = tr->m_impl->MakeArnoldName(AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)));
            if (newName != oldName)
            {
               AiNodeSetStr(arnoldNode, "name", newName.asChar());
               CMayaScene::GetRenderSession()->ObjectNameChanged(path.node(), oldName);
            }
         }*/
         tr->RequestUpdate();
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

void CArnoldSession::QueueForUpdate(const CNodeAttrHandle & handle)
{
   if (m_isExportingMotion && m_updateCallbacks) return;
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
   if (m_isExportingMotion && m_updateCallbacks) return;

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
}


// When a hidden node gets "unhidden", we need to go through an "idle" callback
// so that Maya has time to process its visibility status properly

void CArnoldSession::DoHiddenCallback(void* clientData)
{
   CArnoldSession *session = (CArnoldSession*)clientData;
   MMessage::removeCallback(session->m_hiddenNodeCb);
   session->m_hiddenNodeCb = 0;
   MStatus status;
   MDagPathArray &hiddenNodesArray = session->m_hiddenNodesArray;
   
   if (session)
   {
      for (unsigned int i = 0; i < hiddenNodesArray.length(); i++)
      {
         DagFiltered filtered = session->IsExportable(hiddenNodesArray[i]);
         if (filtered == MTOA_EXPORT_ACCEPTED)
            session->SetDagVisible(hiddenNodesArray[i]);
      }
   }
   hiddenNodesArray.clear();
}
// This callback is invoked when one of the skipped (hidden) objects is modified 
// during an IPR session. We have to check if it became visible in order to export it
void CArnoldSession::HiddenNodeCallback(MObject& node, MPlug& plug, void* clientData)
{
   // just check if this node is visible now 
   MFnDagNode dagNode(node);
   MDagPath path;
   if (dagNode.getPath(path) != MS::kSuccess) return;

   if(!path.isValid() || clientData == nullptr) return;
   CArnoldSession *session = (CArnoldSession*)clientData;
   
   MString plugName = plug.partialName(false, false, false, false, false, true);

   if (plugName != "visibility")
    return; 

   // We need to go through an "idle" callback, otherwise Maya won't have time to process the 
   // visibility status of this node 
   if(session->m_hiddenNodeCb == 0)
   {
      session->m_hiddenNodeCb = MEventMessage::addEventCallback("idle",
                                                  CArnoldSession::DoHiddenCallback,
                                                  clientData
                                                  );
   } 
   session->m_hiddenNodesArray.append(path);
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
         DagFiltered filtered = IsExportable(path);
         if (filtered != MTOA_EXPORT_ACCEPTED)
         {
            if (m_updateCallbacks)
            {
               HiddenObjectCallbackPair hiddenObj;
               hiddenObj.first = CNodeAttrHandle(obj, "");
               hiddenObj.second = MNodeMessage::addNodeDirtyPlugCallback(obj,
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


MString CArnoldSession::GetMayaObjectName(const AtNode *node) const
{   
   // first check if an object exists with the same name ?
   MString arnoldName = (AiNodeLookUpUserParameter(node, "dcc_name") != nullptr) ? 
      MString(AiNodeGetStr(node, "dcc_name")) : MString(AiNodeGetName(node));
   // slashes should be replaced by pipes
   static MString slashStr("/");
   static MString pipeStr("|");
   arnoldName.substitute(slashStr, pipeStr);

   MSelectionList camList;

   camList.add(arnoldName);
   MObject mayaObject;
   if (camList.getDependNode(0, mayaObject) == MS::kSuccess && !mayaObject.isNull())
   {
      // There is an object with the same name in Maya.
      // We're assuming it's this one....
      return MString(arnoldName);
   }
   // Check the dcc_name


   // There is no object with this name in the scene.
   // Let's search it amongst the list of processed translators
   ObjectToTranslatorMap::const_iterator it = m_translators.begin();
   ObjectToTranslatorMap::const_iterator itEnd = m_translators.end();
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

/// Set the camera to export.

/// If called prior to export, only the specified camera will be exported. If not set, all cameras
/// will be exported, but some translators may not be able to fully export without an export camera specified.
/// To address this potential issue, this method should be called after a multi-cam export, as it will cause
/// the options translator to be updated
///
/// FIXME !!! we're changing the explanation above !! we don't want a special behaviour depending on whether this
// function is called before or after Export(). We want to eventually control this in the session 
/// (i.e. export all cameras or only the session one)
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
   m_sessionOptions.m_camera = camera;

   if (updateRender == false  && m_optionsTranslator == NULL) return;
   // just queue the options translator now 
   // instead of relying on the DependsOnExportCamera.
   // In the future we should have a generic way to make translators dependent from others,
   // so that whatever change in one translator propagates an update on the others
   QueueForUpdate(m_optionsTranslator);
   // FIXME we should also update any translator that depends on the render camera, for example the image plane
   Update();
}

void CArnoldSession::UpdateLightLinks()
{
   // FIXME: we are not sure all these lights will actually have been exported to 
   // the Arnold universe
   // Possible solution, make sure ExportLights is done first, before dag objects
   // are exported and light linking queried, and count the number of lights
   // actually in the Arnold universe
   
   /* FIXME : do we need the iterations below ? we just seem to be accumulating m_numLights
   which was never used anywhere

   int numLights = 0;
   MItDag dagIterLights(MItDag::kDepthFirst, MFn::kLight);
   for (; (!dagIterLights.isDone()); dagIterLights.next())
   {
      numLights += 1;
   }
   // For plugin lights, 
   MItDag dagIterPlugin(MItDag::kDepthFirst, MFn::kPluginLocatorNode);
   for (; (!dagIterPlugin.isDone()); dagIterPlugin.next())
   {
      if (IsArnoldLight(dagIterPlugin.currentItem()))
      {
         numLights += 1;
      }
   }
   // TODO : turn off light linking option if we detect here that all lights
   // "illuminate by default" ?
*/
   m_lightLinks.ClearLightLinks();

//   if (numLights > 0)
   {      
      m_lightLinks.SetLinkingMode(m_sessionOptions.GetLightLinkMode(), 
           m_sessionOptions.GetShadowLinkMode());
      if (m_sessionOptions.GetLightLinkMode() == MTOA_LIGHTLINK_MAYA
            || m_sessionOptions.GetShadowLinkMode() == MTOA_SHADOWLINK_MAYA)
      {
         m_lightLinks.ParseLights();
      }
   }
/*   else if (!IsFileExport())
   {
      AiMsgWarning("[mtoa] No light in scene");
   }*/

   m_updateLightLinks = false;

   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.sesion]    Updating Light-linking information");


}
void CArnoldSession::ExportLightLinking(AtNode *shape, const MDagPath &path)
{
    m_lightLinks.ExportLightLinking(shape, path);
}

void CArnoldSession::RequestUpdateImagers(bool listChanged)
{
   // Some imager nodes might have been created without going through the usual Update function, 
   // we need to ensure the update callbacks were properly created for each of them.
   if (!IsInteractiveSession())
      return;

   // the code below only applies if the list of imagers has changed
   if (!listChanged)
      return;

   for(size_t i = 0; i < m_objectsToUpdate.size(); ++i)
   {
      CNodeTranslator *tr = m_objectsToUpdate[i].second;
      if (tr == nullptr)
         continue;

      if (tr->m_impl->m_mayaCallbackIDs.length() == 0) 
         tr->AddUpdateCallbacks();
      
      tr->m_impl->m_inUpdateQueue = false; // I'm allowed to receive updates once again
      tr->m_impl->m_isExported = true;
      // restore the update mode to "update Only"
      tr->m_impl->m_updateMode = CNodeTranslator::AI_UPDATE_ONLY;
   }
}
MObject CArnoldSession::GetDefaultArnoldRenderOptions()
{
   MObject options;
   MSelectionList list;
   list.add("defaultArnoldRenderOptions");
  if (list.length() > 0)
      list.getDependNode(0, options);
   else
   {
       // defaultArnoldRenderOptions doesn't exist, we need to initialize it
      MGlobal::executePythonCommand("import mtoa.core;mtoa.core.createOptions()"); 
      list.clear();
      list.add("defaultArnoldRenderOptions");
      if (list.length() > 0)
         list.getDependNode(0, options);
   }
   return options;
}

void CArnoldSession::InterruptRender()
{
   if (m_renderSession)
      AiRenderInterrupt(m_renderSession, AI_BLOCKING);
}
bool CArnoldSession::IsRendering()
{
   if (m_renderSession == nullptr)
      return false;

   return AiRenderGetStatus(m_renderSession) == AI_RENDER_STATUS_RENDERING;
}
