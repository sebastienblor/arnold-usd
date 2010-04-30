
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
#include <maya/MFnNumericAttribute.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnInstancer.h>
#include <maya/MItInstancer.h>

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

   // Since arnold does not support a ginstance pointing to another ginstance
   // we need to keep track of which nodes , in arnold, are the "master" ones
   MDagPathArray masterInstances;

   // First we export all cameras
   // We do not reset the iterator to avoid getting kWorld
   for (; (!dagIterCameras.isDone()); dagIterCameras.next())
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

         if (!instancedDag)
         {
            MFnMeshData meshData;
            MObject     meshFromNURBS;
            MObject     meshDataObject = meshData.create();

            meshFromNURBS = surface.tesselate(MTesselationParams::fsDefaultTesselationParams, meshDataObject);
            // in order to get displacement, we need a couple of attributes
            MFnNumericAttribute  nAttr;   

            MObject subdiv_type = nAttr.create("subdiv_type", "sdbt", MFnNumericData::kInt, 1);
            surface.addAttribute(subdiv_type);
            MObject subdiv_iterations = nAttr.create("subdiv_iterations", "sdbit", MFnNumericData::kInt, 1);
            surface.addAttribute(subdiv_iterations);
            MObject subdiv_adaptive_metric = nAttr.create("subdiv_adaptive_metric", "sdbam", MFnNumericData::kInt, 1);
            surface.addAttribute(subdiv_adaptive_metric);
            MObject subdiv_pixel_error = nAttr.create("subdiv_pixel_error", "sdbpe", MFnNumericData::kInt, 0);
            surface.addAttribute(subdiv_pixel_error);

            ExportMesh(meshFromNURBS, dagPath, step);
         }
         else
         {
            MDagPathArray allInstances;
            dagIterator.getAllPaths(allInstances);

            // check if there is a created master node
            bool thereIsMaster = false;
            MDagPath masterDag;
            for (int i=0; ((i<masterInstances.length())&&(!thereIsMaster)); i++)
            {
               for (int j=0; ((j<allInstances.length())&&(!thereIsMaster)); j++)
               {
                  // if there is a master already created, break
                  if (allInstances[j]==masterInstances[i])
                  {
                     masterDag = masterInstances[j];
                     thereIsMaster = true; 
                  }
               }
            }

            // there is no masternode yet created, we create the first one
            if(!thereIsMaster)
            {
               // make this one the master in the masters array
               masterInstances.append(dagPath);

               MFnMeshData meshData;
               MObject     meshFromNURBS;
               MObject     meshDataObject = meshData.create();

               MFnNurbsSurface surface(dagPath, &status);

               meshFromNURBS = surface.tesselate(MTesselationParams::fsDefaultTesselationParams, meshDataObject);
               // in order to get displacement, we need a couple of attributes
               MFnNumericAttribute  nAttr;   

               MObject subdiv_type = nAttr.create("subdiv_type", "sdbt", MFnNumericData::kInt, 1);
               surface.addAttribute(subdiv_type);
               MObject subdiv_iterations = nAttr.create("subdiv_iterations", "sdbit", MFnNumericData::kInt, 1);
               surface.addAttribute(subdiv_iterations);
               MObject subdiv_adaptive_metric = nAttr.create("subdiv_adaptive_metric", "sdbam", MFnNumericData::kInt, 1);
               surface.addAttribute(subdiv_adaptive_metric);
               MObject subdiv_pixel_error = nAttr.create("subdiv_pixel_error", "sdbpe", MFnNumericData::kInt, 0);
               surface.addAttribute(subdiv_pixel_error);

               // export the first one normally
               // check if there is a master instance created
               ExportMesh(meshFromNURBS, dagPath, step);
            }
            // there is already a master, so we create an instance
            else
            {
               ExportMeshInstance(dagPath, masterDag, step);
            } 
         }
      }

      // Polygons
      else if (dagIterator.item().hasFn(MFn::kMesh))
      {
         unsigned int      numMeshGroups;
         MFnDependencyNode fnDGNode(dagIterator.item());
         int instanceNum = dagPath.isInstanced() ? dagPath.instanceNumber() : 0;

         MPlug plug(dagIterator.item(), fnDGNode.attribute("instObjGroups"));

         if (plug.elementByLogicalIndex(instanceNum).isConnected())
         {
            numMeshGroups = 1;
         }
         else
         {
            MFnMesh      mesh(dagIterator.item());
            MObjectArray shaders;
            MIntArray    indices;

            mesh.getConnectedShaders(instanceNum, shaders, indices);

            numMeshGroups = shaders.length();
         }

         if (numMeshGroups == 0)
         {
            if (step == 0)
               AiMsgError("[mtoa] ERROR: Mesh not exported. It has 0 groups.");
         }
         else
         {
            // if its an instance
            if (instancedDag)
            {
               MDagPathArray allInstances;
               dagIterator.getAllPaths(allInstances);

               // check if there is a created master node
               bool thereIsMaster = false;
               MDagPath masterDag;
               for (int i=0; ((i<masterInstances.length())&&(!thereIsMaster)); i++)
               {
                  for (int j=0; ((j<allInstances.length())&&(!thereIsMaster)); j++)
                  {
                     // if there is a master already created, break
                     if (allInstances[j]==masterInstances[i])
                     {
                        masterDag = masterInstances[i];
                        thereIsMaster = true; 
                     }
                  }
               }

               // there is no masternode yet created, we create the first one
               if(!thereIsMaster)
               {
                  // make this one the master in the masters array
                  masterInstances.append(dagPath);

                  ExportMesh(dagIterator.item(), dagPath, step);
               } 
               // instanced meshes
               else
               {
                  ExportMeshInstance(dagPath, masterDag, step);
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

   // Once all regular objects are created, check for FX
   
   // Particle Instancer
   MItInstancer   instIterator;
   for (instIterator.reset(); (!instIterator.isDone()); instIterator.nextInstancer())
   {
      dagPath = instIterator.instancerPath();
      MFnDagNode node(dagPath.node());

      if (IsVisible(node))
         ExportInstancerReplacement(dagPath, step);
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
