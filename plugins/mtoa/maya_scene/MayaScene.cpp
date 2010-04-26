
#include "MayaScene.h"

#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/M3DView.h>
#include <maya/MAnimControl.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPathArray.h>

MStatus CMayaScene::ExportToArnold()
{
   MStatus status;

   PrepareExport();

   bool mb = m_motionBlurData.enabled && (m_fnArnoldRenderOptions->findPlug("mb_camera_enable").asBool() ||
                                          m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool() ||
                                          m_fnArnoldRenderOptions->findPlug("mb_lights_enable").asBool());

   if (!mb)
   {
      status = ExportScene(0);
   }
   else
   {
      for (int J = 0; (J < m_motionBlurData.motion_steps); ++J)
      {
         MGlobal::viewFrame(MTime(m_motionBlurData.frames[J], MTime::uiUnit()));
         status = ExportScene(J);
      }
      MGlobal::viewFrame(MTime(m_currentFrame, MTime::uiUnit()));
   }

   return status;
}

MStatus CMayaScene::ExportScene(AtUInt step)
{
   MStatus  status;
   MDagPath dagPath;
   MItDag   dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);

   // First we export all cameras
   for (dagIterCameras.reset(); (!dagIterCameras.isDone()); dagIterCameras.next())
   {
      if (!dagIterCameras.getPath(dagPath))
      {
         AiMsgError("[mtoa] ERROR: Could not get path for DAG iterator.");
         return status;
      }

      ExportCamera(dagPath, step);
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

      MFnDagNode node(dagPath.node());

      if (!IsVisible(node))
      {
         dagIterator.prune();
         continue;
      }
      
      // Find if the node is instanced
      bool instancedDag = dagIterator.isInstanced(true);
      if (instancedDag)
      {
         // once we find an instanced object, we export all instances
         // so if step is 0, we do not create again the instance if it's already created
         if( (AiNodeLookUpByName(dagPath.fullPathName().asChar())) && ( step == 0 ))
         {
            dagIterator.prune();
            continue;
         }
      }

      // Lights
      if (dagIterator.item().hasFn(MFn::kLight))
      {
         ExportLight(dagPath, step);
      }
    
      // Nurbs 
      else if (dagIterator.item().hasFn(MFn::kNurbsSurface))
      {
         MFnNurbsSurface surface(dagPath, &status);

         if (!status)
         {
            AiMsgError("[mtoa] ERROR: Could not create NURBS surface.");

            return status;
         }

         MFnMeshData meshData;
         MObject     meshFromNURBS;
         MObject     meshDataObject = meshData.create();

         meshFromNURBS = surface.tesselate(MTesselationParams::fsDefaultTesselationParams, meshDataObject);
         ExportMesh(meshFromNURBS, dagPath, step);
      }

      // Polygons
      else if (dagIterator.item().hasFn(MFn::kMesh))
      {
         unsigned int      numMeshGroups;
         MFnDependencyNode fnDGNode(dagIterator.item());

         MPlug plug(dagIterator.item(), fnDGNode.attribute("instObjGroups"));

         if (plug.elementByLogicalIndex(0).isConnected())
         {
            numMeshGroups = 1;
         }
         else
         {
            MFnMesh      mesh(dagIterator.item());
            MObjectArray shaders;
            MIntArray    indices;

            mesh.getConnectedShaders(0, shaders, indices);

            numMeshGroups = shaders.length();
         }

         if (numMeshGroups == 0)
         {
            if (step == 0)
               AiMsgError("[mtoa] ERROR: Mesh not exported. It has 0 groups.");
         }
         else
         {
            // if its a direct instance, get all the instances and export them
            if (instancedDag)
            {
               MDagPathArray allInstances;
               dagIterator.getAllPaths(allInstances);
               // export the first one normally

               ExportMesh(allInstances[0].node(), allInstances[0], step);
               for (int i=1; i<allInstances.length(); i++)
               {
                  // then instanced meshes
                  ExportMeshInstance(allInstances[i], allInstances[0], step);
               } 
            }
            else
            {
               ExportMesh(dagIterator.item(), dagPath, step);
            }
         }
      }
      else
      {
         if (!status)
         {
            AiMsgError("[mtoa] ERROR: Unrecognized node found while iterating DAG.");

            return status;
         }
      }
   }

   return MS::kSuccess;
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

   m_currentFrame = MAnimControl::currentTime().as(MTime::uiUnit());

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
