
#include "MayaScene.h"
#include "NodeTranslator.h"

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

std::map<int, CreatorFunction>  CMayaScene::s_dagTranslators;
std::map<int, CreatorFunction>  CMayaScene::s_dependTranslators;

CMayaScene::~CMayaScene()
{
   delete m_fnCommonRenderOptions;
   delete m_fnArnoldRenderOptions;
   // delete translators
   ObjectToTranslatorMap::iterator it;
   for(it = m_processedTranslators.begin(); it != m_processedTranslators.end(); it++)
   {
      delete it->second;
   }
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
         MGlobal::viewFrame(MTime(m_currentFrame, MTime::uiUnit()));
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
         MGlobal::viewFrame(MTime(m_currentFrame, MTime::uiUnit()));
      }
   }
   else
   {
      status = ExportSelected();
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
      if (IsVisible(path.node()))
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

      if (!IsVisible(node))
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
   // Export scene installing message callbacks (the 'true')
   // so we can pickup changes in the scene.
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
         float frame = m_currentFrame -
                       m_motionBlurData.motion_frames * 0.5f +
                       m_motionBlurData.shutter_offset +
                       m_motionBlurData.motion_frames / (m_motionBlurData.motion_steps - 1) * J;

         m_motionBlurData.frames.push_back(frame);
      }
   }
}
