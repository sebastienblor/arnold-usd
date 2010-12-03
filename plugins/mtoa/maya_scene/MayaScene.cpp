#include "MayaScene.h"

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
#include <maya/MFnNumericAttribute.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnInstancer.h>
#include <maya/MItInstancer.h>
#include <maya/MPlugArray.h>

MStatus CMayaScene::ExportToArnold(ExportMode exportMode)
{
   MStatus status;

   if (exportMode == MTOA_EXPORT_ALL)
   {
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
   }
   else
   {
      PrepareExport();
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
               // Export poly mesh
               if (path.apiType() == 295)
               {
                  ExportMesh(path.node(), path, 0);
               }
               // Exports maya hair
               else if (path.apiType() == 916)
               {
                  ExportHair(path, 0);
               }
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

// Get shading engine associated with a custom shape
//
void CMayaScene::GetCustomShapeInstanceShader(const MDagPath &path, MFnDependencyNode &shadingEngineNode)
{
   // Get instance shadingEngine
   shadingEngineNode.setObject(MObject::kNullObj);

   char buffer[64];

   MStringArray connections;
   MGlobal::executeCommand("listConnections -s 1 -d 0 -c 1 -type shadingEngine "+path.fullPathName(), connections);

   MSelectionList sl;

   if (connections.length() == 2)
   {
      sl.add(connections[1]);
   }
   else if (connections.length() > 2)
   {
      sprintf(buffer, "[%d]", path.instanceNumber());
      MString iidx = buffer;

      for (unsigned int cidx = 0; cidx < connections.length(); cidx += 2)
      {
         MString conn = connections[cidx];

         if (conn.length() < iidx.length())
         {
            continue;
         }

         if (conn.substring(conn.length() - iidx.length(), conn.length() - 1) != iidx)
         {
            continue;
         }

         sl.add(connections[cidx+1]);
         break;
      }
   }

   if (sl.length() == 1)
   {
      MObject shadingEngineObj;
      sl.getDependNode(0, shadingEngineObj);

      shadingEngineNode.setObject(shadingEngineObj);
   }
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

      ExportCamera(dagPath, step);
   }

   std::map<std::string, CCustomData>::iterator customShapeIt;

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

      // Custom shapes [do them first so we can overrides built-in ones]
      customShapeIt = m_customShapes.find(node.typeName().asChar());

      if (customShapeIt != m_customShapes.end())
      {
         ExportCustomShape(dagPath, step, customShapeIt->second.exportCmd, customShapeIt->second.cleanupCmd);
      }

      // Lights
      else if (dagIterator.item().hasFn(MFn::kLight))
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
         MDagPath masterDag;
         if (IsMasterInstance(dagPath, masterDag))
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
            ExportMeshInstance(dagPath, masterDag, step);
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
            MDagPath masterDag;
            if (IsMasterInstance(dagPath, masterDag))
            {
               ExportMesh(dagIterator.item(), dagPath, step);
            }
            else
            {
               ExportMeshInstance(dagPath, masterDag, step);
            }
         }
      }
      else if (dagIterator.item().hasFn(MFn::kHairSystem))
      {
         ExportHair(dagPath, step);
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

   m_currentFrame = static_cast<float>(MAnimControl::currentTime().as(MTime::uiUnit()));

   GetCustomShapes();

   GetMotionBlurData();
}

bool CMayaScene::RegisterCustomShape(std::string &shapeType)
{
   static const char whitespaces[] = " \t\n\v";

   // strip leading and trailing spaces
   size_t w0 = shapeType.find_first_not_of(whitespaces);

   if (w0 != std::string::npos)
   {
      size_t w1 = shapeType.find_last_not_of(whitespaces);

      shapeType = shapeType.substr(w0, w1-w0+1);

      if (shapeType.length() > 0)
      {
         // check if shapeType already registered
         std::map<std::string, CCustomData>::iterator it = m_customShapes.find(shapeType);

         if (it == m_customShapes.end())
         {
            // check if export command can be found
            std::string scriptName = "mtoa_export_" + shapeType;

            MString rv = MGlobal::executeCommandStringResult("whatIs " + MString(scriptName.c_str()));

            if (rv != "Unknown")
            {
               m_customShapes[shapeType].exportCmd = scriptName.c_str();
               
               std::string scriptName = "mtoa_cleanup_" + shapeType;
               
               rv = MGlobal::executeCommandStringResult("whatIs " + MString(scriptName.c_str()));
               
               if (rv == "Unknown")
               {
                  scriptName = "";
               }
               
               m_customShapes[shapeType].cleanupCmd = scriptName.c_str();
               
               return true;
            }
         }
      }
   }

   return false;
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
