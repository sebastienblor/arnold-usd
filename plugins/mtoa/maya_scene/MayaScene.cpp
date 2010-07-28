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
#include <maya/MBoundingBox.h>
#include <maya/MPlugArray.h>

static bool StringInList(const MString &str, const MStringArray &ary)
{
   for (unsigned int i=0; i<ary.length(); ++i)
   {
      if (ary[i] == str)
      {
         return true;
      }
   }
   return false;
}

MStatus CMayaScene::ExportToArnold(ExportMode exportMode)
{
   MStatus status;

   if (exportMode == exportMode)
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

// Loop and export the selection and all its hirarchy down stream
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
         for (int child=0; child<path.childCount(); child++)
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


MStatus CMayaScene::ExportSelected()
{
   MStatus status;

   MSelectionList selected;
   MGlobal::getActiveSelectionList(selected);

   IterSelection(selected);

   selected.clear();

   return status;
}




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

   std::map<std::string, std::string>::iterator customShapeIt;

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


      // Custom shapes [do them first so we can overrides built-in ones]
      customShapeIt = m_customShapes.find(node.typeName().asChar());

      if (customShapeIt != m_customShapes.end())
      {
         bool thereIsMaster = false;

         bool transformBlur = m_motionBlurData.enabled &&
                              m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool() &&
                              node.findPlug("motionBlur").asBool();

         bool deformBlur = m_motionBlurData.enabled &&
                           m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
                           node.findPlug("motionBlur").asBool();

         AtMatrix matrix;
         MMatrix mmatrix = dagPath.inclusiveMatrix();
         ConvertMatrix(matrix, mmatrix);

         char buffer[64];

         MString command = MString(customShapeIt->second.c_str());
         command += "(";

         sprintf(buffer, "%f", m_currentFrame);
         command += buffer;
         command += ", ";

         sprintf(buffer, "%d", step);
         command += buffer;
         command += ", ";

         sprintf(buffer, "%f", (step < m_motionBlurData.frames.size() ? m_motionBlurData.frames[step] : m_currentFrame));
         command += buffer;
         command += ", ";

         if (!instancedDag)
         {
            command += "\"" + dagIterator.fullPathName() + "\", 0, \"\"";
         }
         else
         // if its an instance
         {
            // only if it is the 0 instance , export all
            if (dagPath.instanceNumber()==0)
            {
               // get all paths and make the first visible the master
               MDagPathArray allInstances;
               dagIterator.getAllPaths(allInstances);

               MDagPath masterDag;
               bool master_found = false;
               int master_index = 0;
               for (; ((master_index<allInstances.length())); master_index++)
               {
                  MFnDagNode node(allInstances[master_index].node());        
                  // master set
                  if (IsVisible(node))
                  {
                     if (!master_found)
                     {
                        masterDag = allInstances[master_index];
                        master_found = true;
                        command += "\"" + dagPath.fullPathName() + "\", 0, \"\"";
                     }
                     else
                     {
                        command += "\"" + dagPath.fullPathName() + "\", 1, \"" + masterDag.fullPathName() + "\"";
                     }
                  }
               } 
            }
         }

         command += ")";

         MStringArray attrs;
         status = MGlobal::executeCommand(command, attrs);

         if (!status)
         {
            AiMsgError("[mtoa] ERROR: Failed to export custom shape \"%s\" (%s).", node.name().asChar(), customShapeIt->first.c_str());
            return status;
         }

         AtNode *proc = AiNodeLookUpByName(dagPath.fullPathName().asChar());

         if (proc)
         {
            // Set transformation matrices
            if (transformBlur && !StringInList("matrix", attrs))
            {

               if (step == 0)
               {
                  AtArray* matrices = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_MATRIX);
                  AiArraySetMtx(matrices, step, matrix);
                  AiNodeSetArray(proc, "matrix", matrices);
               }
               else
               {
                  AtArray* matrices = AiNodeGetArray(proc, "matrix");
                  AiArraySetMtx(matrices, step, matrix);
               }
            }
            else
            {
               AiNodeSetMatrix(proc, "matrix", matrix);
            }

            if (!instancedDag || !thereIsMaster)
            {
               // Set bounding box
               if (!StringInList("min", attrs) && !StringInList("max", attrs))
               {
                  if (step == 0)
                  {
                     // set for the first time
                     MBoundingBox bbox = node.boundingBox();

                     MPoint bmin = bbox.min() * mmatrix;
                     MPoint bmax = bbox.max() * mmatrix;

                     AiNodeSetPnt(proc, "min", bmin.x, bmin.y, bmin.z);
                     AiNodeSetPnt(proc, "max", bmax.x, bmax.y, bmax.z);
                  }
                  else
                  {
                     if (transformBlur || deformBlur)
                     {
                        AtPoint cmin = AiNodeGetPnt(proc, "min");
                        AtPoint cmax = AiNodeGetPnt(proc, "max");

                        MBoundingBox bbox = node.boundingBox();

                        MPoint bmin = bbox.min() * mmatrix;
                        MPoint bmax = bbox.max() * mmatrix;

                        if (bmin.x < cmin.x)
                           cmin.x = bmin.x;
                        if (bmin.y < cmin.y)
                           cmin.y = bmin.y;
                        if (bmin.z < cmin.z)
                           cmin.z = bmin.z;
                        if (bmax.x > cmax.x)
                           cmax.x = bmax.x;
                        if (bmax.y > cmax.y)
                           cmax.y = bmax.y;
                        if (bmax.z > cmax.z)
                           cmax.z = bmax.z;

                        AiNodeSetPnt(proc, "min", cmin.x, cmin.y, cmin.z);
                        AiNodeSetPnt(proc, "max", cmax.x, cmax.y, cmax.z);
                     }
                  }
               }

               // Set common attributes
               MPlug plug;

               if (!StringInList("sss_max_samples", attrs))
               {
                  plug = node.findPlug("sss_max_samples");
                  if (!plug.isNull())
                  {
                     AiNodeSetInt(proc, "sss_max_samples", plug.asInt());
                  }
               }

               if (!StringInList("sss_sample_spacing", attrs))
               {
                  plug = node.findPlug("sss_sample_spacing");
                  if (!plug.isNull())
                  {
                     AiNodeSetFlt(proc, "sss_sample_spacing", plug.asFloat());
                  }
               }

               if (!StringInList("sss_use_gi", attrs))
               {
                  plug = node.findPlug("sss_use_gi");
                  if (!plug.isNull())
                  {
                     AiNodeSetBool(proc, "sss_use_gi", plug.asBool());
                  }
               }

               if (!StringInList("receive_shadows", attrs))
               {
                  plug = node.findPlug("receiveShadows");
                  if (!plug.isNull())
                  {
                     AiNodeSetBool(proc, "receive_shadows", plug.asBool());
                  }
               }

               if (!StringInList("self_shadows", attrs))
               {
                  plug = node.findPlug("self_shadows");
                  if (!plug.isNull())
                  {
                     AiNodeSetBool(proc, "self_shadows", plug.asBool());
                  }
               }

               if (!StringInList("opaque", attrs))
               {
                  plug = node.findPlug("opaque");
                  if (!plug.isNull())
                  {
                     AiNodeSetBool(proc, "opaque", plug.asBool());
                  }
               }

               if (!StringInList("visibility", attrs))
               {
                  AtInt visibility = AI_RAY_ALL;

                  plug = node.findPlug("castsShadows");
                  if (!plug.isNull() && !plug.asBool())
                  {
                     visibility &= ~AI_RAY_SHADOW;
                  }

                  plug = node.findPlug("primaryVisibility");
                  if (!plug.isNull() && !plug.asBool())
                  {
                     visibility &= ~AI_RAY_CAMERA;
                  }

                  plug = node.findPlug("visibleInReflections");
                  if (!plug.isNull() && !plug.asBool())
                  {
                     visibility &= ~AI_RAY_REFLECTED;
                  }

                  plug = node.findPlug("visibleInRefractions");
                  if (!plug.isNull() && !plug.asBool())
                  {
                     visibility &= ~AI_RAY_REFRACTED;
                  }

                  plug = node.findPlug("diffuse_visibility");
                  if (!plug.isNull() && !plug.asBool())
                  {
                     visibility &= ~AI_RAY_DIFFUSE;
                  }

                  plug = node.findPlug("glossy_visibility");
                  if (!plug.isNull() && !plug.asBool())
                  {
                     visibility &= ~AI_RAY_GLOSSY;
                  }

                  AiNodeSetInt(proc, "visibility", visibility);
               }
            }

            // Surface shader
            if (!StringInList("shader", attrs))
            {
               MStringArray connections;
               MGlobal::executeCommand("listConnections -s 1 -d 0 -type shadingEngine "+dagPath.fullPathName(), connections);

               if (connections.length() == 1)
               {
                  MSelectionList sl;
                  sl.add(connections[0]);

                  MObject shadingEngineObj;
                  sl.getDependNode(0, shadingEngineObj);

                  MFnDependencyNode shadingEngine(shadingEngineObj);

                  MPlugArray shaderConns;

                  MPlug shaderPlug = shadingEngine.findPlug("surfaceShader");

                  shaderPlug.connectedTo(shaderConns, true, false);

                  if (shaderConns.length() > 0)
                  {
                     AtNode *shader = ExportShader(shaderConns[0].node());

                     AiNodeSetPtr(proc, "shader", shader);
                  }
               }
            }

            // NOTE: no displacement attributes on procedural nodes

            //shaderPlug = shadingEngine.findPlug("displacementShader");
            //
            //shaderPlug.connectedTo(shaderConns, true, false);
            //
            //if (shaderConns.length() > 0)
            //{
            //}
         }
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
            // only if it is the 0 instance , export all
            if (dagPath.instanceNumber()==0)
            {
               // get all paths and make the first visible the master
               MDagPathArray allInstances;
               dagIterator.getAllPaths(allInstances);

               MDagPath masterDag;
               bool master_found = false;
               int master_index = 0;
               for (; ((master_index<allInstances.length())); master_index++)
               {
                  MFnDagNode node(allInstances[master_index].node());        
                  // master set
                  if (IsVisible(node))
                  {
                     if (!master_found)
                     {
                        masterDag = allInstances[master_index];
                        master_found = true;
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
                        ExportMesh(meshFromNURBS, masterDag, step);
                     }
                     else
                     {
                        ExportMeshInstance(allInstances[master_index], masterDag, step);
                     }
                  }
               } 
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
               // only if it is the 0 instance , export all
               if (dagPath.instanceNumber()==0)
               {
                  // get all paths and make the first visible the master
                  MDagPathArray allInstances;
                  dagIterator.getAllPaths(allInstances);

                  MDagPath masterDag;
                  bool master_found = false;
                  int master_index = 0;
                  for (; ((master_index<allInstances.length())); master_index++)
                  {
                     MFnDagNode node(allInstances[master_index].node());        
                     // master set
                     if (IsVisible(node))
                     {
                        if (!master_found)
                        {
                           masterDag = allInstances[master_index];
                           master_found = true;
                           ExportMesh(masterDag.node(), masterDag, step);
                        }
                        else
                        {
                           ExportMeshInstance(allInstances[master_index], masterDag, step);
                        }
                     }
                  } 
               }
            }

            else
            {
               ExportMesh(dagIterator.item(), dagPath, step);
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

   m_currentFrame = MAnimControl::currentTime().as(MTime::uiUnit());

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
         std::map<std::string, std::string>::iterator it = m_customShapes.find(shapeType);

         if (it == m_customShapes.end())
         {
            // check if export command can be found
            std::string scriptName = "mtoa_export_" + shapeType;

            MString rv = MGlobal::executeCommandStringResult("whatIs " + MString(scriptName.c_str()));

            if (rv != "Unknown")
            {
               m_customShapes[shapeType] = scriptName;
               return true;
            }
         }
      }
   }

   return false;
}

void CMayaScene::GetCustomShapes()
{
   static const char separators[] = ",:;";

   MString csVar = "$MTOA_CUSTOM_SHAPES";

   MString csExpVar = csVar.expandEnvironmentVariablesAndTilde();

   if (csExpVar != csVar)
   {
      std::string shapeType;
      std::string shapeList = csExpVar.asChar();

      size_t p0 = 0;
      size_t p1 = shapeList.find_first_of(separators, p0);

      while (p1 != std::string::npos)
      {
         shapeType = shapeList.substr(p0, p1-p0);

         if (RegisterCustomShape(shapeType))
         {
            MGlobal::displayInfo("[mtoa] Registered custom shape \"" + MString(shapeType.c_str()) + "\"");
         }

         p0 = p1 + 1;
         p1 = shapeList.find_first_of(separators, p0);
      }

      shapeType = shapeList.substr(p0);

      if (RegisterCustomShape(shapeType))
      {
         MGlobal::displayInfo("[mtoa] Registered custom shape \"" + MString(shapeType.c_str()) + "\"");
      }
   }
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
