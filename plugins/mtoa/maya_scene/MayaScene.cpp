
#include "MayaScene.h"
#include "Utils.h"

#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/M3DView.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>

void ProcessCamera(const MDagPath& dagPath);
void ProcessLight(const MDagPath& dagPath);
void ProcessMesh(MObject mayaMesh, MObject dagNode, MMatrix tm);

MStatus ProcessMayaScene(MItDag::TraversalType traversalType)
{

   MDagPath dagPath;
   MStatus  status;
   MItDag   dagIterator(traversalType, MFn::kInvalid);

   // Export current camera
   MDagPath cameraPath;

   M3dView::active3dView().getCamera(cameraPath);

   AiMsgDebug("[mtoa] Processing camera");
   ProcessCamera(cameraPath);

   for (dagIterator.reset(); (!dagIterator.isDone()); dagIterator.next())
   {
      // MItDag::getPath() gets the reference to the object that the iterator is currently on.
      // This DAG path can then be used in a function set to operate on the object.
      // In general it is not a good idea to rearrange the DAG from with an iterator.
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

      //AiMsgDebug("Node: %s", node.name().asChar());

      MMatrix tm = dagPath.inclusiveMatrix();

      if (dagIterator.item().hasFn(MFn::kLight))
      {
         AiMsgDebug("[mtoa] Processing light");
         ProcessLight(dagPath);
      }
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

         AiMsgDebug("[mtoa] Processing NURBS surface");
         meshFromNURBS = surface.tesselate(MTesselationParams::fsDefaultTesselationParams, meshDataObject);

         ProcessMesh(meshFromNURBS, dagIterator.item(), tm);
      }
      else if (dagIterator.item().hasFn(MFn::kMesh))
      {
         unsigned int      numMeshGroups;
         MFnDependencyNode fnDGNode(dagIterator.item());

         // Buscamos el atributo "instObjGroups" del nodo para ver si esta conectado
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

            unsigned int numMeshGroups = shaders.length();
         }

         if (numMeshGroups == 0)
         {
            AiMsgError("[mtoa] ERROR: Mesh not exported. It has 0 groups.");
         }
         else
         {
            AiMsgDebug("[mtoa] Processing mesh");
            ProcessMesh(dagIterator.item(), dagIterator.item(), tm);
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

}  // ProcessMayaScene()
