
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

   if (exportMode == MTOA_EXPORT_ALL || exportMode == MTOA_EXPORT_IPR)
   {
      if (mb)
      {
         // first step is the real export
         AiMsgDebug("[mtoa] exporting step 0 at frame %f", m_motionBlurData.frames[0]);
         MGlobal::viewFrame(MTime(m_motionBlurData.frames[0], MTime::uiUnit()));
         status = ExportScene(0);
         // next, loop through motion steps
         for (int step = 1; step < m_motionBlurData.motion_steps; ++step)
         {
            MGlobal::viewFrame(MTime(m_motionBlurData.frames[step], MTime::uiUnit()));
            AiMsgDebug("[mtoa] exporting step %d at frame %f", step, m_motionBlurData.frames[step]);
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
      else
         status = ExportScene(0);

   }
   else if ( exportMode == MTOA_EXPORT_SELECTED )
   {
      if (!mb)
      {
         status = ExportSelected(0);
      }
      else
      {
         // Scene is motion blured, get the data for the steps.
         for (int J = 0; (J < m_motionBlurData.motion_steps); ++J)
         {
            MGlobal::viewFrame(MTime(m_motionBlurData.frames[J], MTime::uiUnit()));
            status = ExportSelected(J);
         }
         MGlobal::viewFrame(MTime(GetCurrentFrame(), MTime::uiUnit()));
      }
   }
   else
   {
      AiMsgDebug( "[mtoa] unsupported export mode: %d", exportMode );
   }

   return status;
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

// Export a shader (dependency node)
//
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

   if (attrName != "")
      AiMsgDebug("[mtoa] Exporting shader: %s.%s", node.name().asChar(), attrName.asChar());
   else
      AiMsgDebug("[mtoa] Exporting shader: %s", node.name().asChar());

   AtInt nodeId = node.typeId().id();
   std::map<int, CreatorFunction>::iterator dependTransIt = s_dependTranslators.find(nodeId);
   if (dependTransIt != s_dependTranslators.end())
   {
      if (mayaShader.hasFn(MFn::kDagNode))
      {
         CDagTranslator* translator;
         MDagPath dagPath;
         MDagPath::getAPathTo(mayaShader, dagPath);
         translator = (CDagTranslator*)dependTransIt->second();
         translator->Init(dagPath, this, attrName);
         // FIXME: currently shaders are only exported for step = 0
         shader = translator->DoExport(0);
         m_processedTranslators[MObjectHandle(mayaShader)] = translator;
      }
      else
      {
         CNodeTranslator* translator;
         translator = (CNodeTranslator*)dependTransIt->second();
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

// Export the maya scene
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::ExportScene(AtUInt step)
{
   MStatus  status;
   MDagPath dagPath;
   MItDag   dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);

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
   
   // Add callbacks if we're in IPR mode.
   if ( GetExportMode() == MTOA_EXPORT_IPR && s_NewNodeCallbackId == 0x0 )
   {
      s_NewNodeCallbackId = MDGMessage::addNodeAddedCallback( CMayaScene::IPRNewNodeCallback );
   }
   
   return MS::kSuccess;
}

// Get the selection from maya and export it with the IterSelection methode
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::ExportSelected(AtUInt step)
{
   MStatus status;

   MSelectionList selected;
   MGlobal::getActiveSelectionList(selected);

   status = IterSelection(selected, step);

   selected.clear();

   return status;
}

// Loop and export the selection, and all its hirarchy down stream
//
// @return              MS::kSuccess / MS::kFailure is returned in case of failure.
//
MStatus CMayaScene::IterSelection(MSelectionList selected, AtUInt step)
{
   MStatus status = MStatus::kSuccess;
   MItSelectionList it(selected, MFn::kInvalid, &status);

   MObject node;
   MObjectArray nodeArray;
   MDagPath path;
   MFnDagNode dgNode;
   MFnSet set;
   MFnRenderLayer layer;
   MSelectionList children;
   // loop users selection
   for (it.reset(); !it.isDone(); it.next())
   {
      if (it.getDagPath(path) == MStatus::kSuccess)
      {
         // Got a dag node, iterate Hierarchy
         if (IsVisible(path.node()) || !IsTemplated(path.node()))
         {
            for (AtUInt child = 0; (child < path.childCount()); child++)
            {
               MObject ChildObject = path.child(child);
               path.push(ChildObject);
               children.clear();
               children.add(path.fullPathName());
               dgNode.setObject(path.node());
               if (!dgNode.isIntermediateObject())
                  ExportDagPath(path, step);
               path.pop(1);
               status = (status && IterSelection(children, step)) ? MStatus::kSuccess : MStatus::kFailure;
            }
         }
      }
      else if (it.getDependNode(node) == MStatus::kSuccess)
      {
         // Got a dependency (not dag) node
         // What kind of node is it
         if (node.hasFn(MFn::kSet))
         {
            // if it's a set we actually iterate on its content
            set.setObject(node);
            children.clear();
            // get set members, we don't set flatten to true in case we'd want a
            // test on each set recursively
            set.getMembers(children, false);
            status = (status && IterSelection(children, step)) ? MStatus::kSuccess : MStatus::kFailure;
         }
         else if (node.hasFn(MFn::kRenderLayer))
         {
            // if it's a render layer we need to do this both for
            // sub layers and objects in render layer
            layer.setObject(node);
            // Get sub layers
            // not using recurse in case we want a test on render layers first
            nodeArray.clear();
            layer.layerChildren(nodeArray);
            children.clear();
            unsigned int nc = nodeArray.length();
            for (unsigned int c=0; c<nc; c++)
               children.add(nodeArray[c]);
            status = (status && IterSelection(children, step)) ? MStatus::kSuccess : MStatus::kFailure;
            // Get layer members (objects)
            nodeArray.clear();
            layer.listMembers(nodeArray);
            // Why the heck doesn't it fill a MSelectionList like a set really?
            children.clear();
            unsigned int nm = nodeArray.length();
            for (unsigned int m=0; m<nm; m++)
               children.add(nodeArray[m]);
            status = (status && IterSelection(children, step)) ? MStatus::kSuccess : MStatus::kFailure;
         }
         else
         {
            // TODO: if we got a shape selected export all dag paths (instances) to that shape?
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

bool CMayaScene::ExportDagPath(MDagPath &dagPath, AtUInt step)
{
   MFnDagNode node(dagPath.node());
   MObjectHandle handle = MObjectHandle(dagPath.node());
   int instanceNum = dagPath.instanceNumber();
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
         // save it for later
         m_processedDagTranslators[handle][instanceNum] = translator;
         return true;
      }
   }
   else
   {
      // this will eventually go away when we do our motion export by looping through processed translators
      CDagTranslator* translator = (CDagTranslator*)m_processedDagTranslators[handle][instanceNum];
      if (translator != NULL)
      {
         translator->DoExport(step);
         return true;
      }
   }
   return false;
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
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); ++it)
   {
      if ( it->second != 0x0 ) it->second->RemoveCallbacks();
   }

   ObjectToDagTranslatorMap::iterator dagIt;
   for(dagIt = m_processedDagTranslators.begin(); dagIt != m_processedDagTranslators.end(); ++dagIt)
   {
      std::map<int, CNodeTranslator*>::iterator instIt;
      for(instIt = dagIt->second.begin(); instIt != dagIt->second.end(); ++instIt)
      {
         instIt->second->RemoveCallbacks();
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

