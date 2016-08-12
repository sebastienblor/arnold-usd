
#include "MeshTranslator.h"

#include <maya/MFnMeshData.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMeshSmoothOptions.h>
#include <maya/MItMeshVertex.h>
#include <maya/MDagPathArray.h>

unsigned int CMeshTranslator::GetNumMeshGroups(const MDagPath& dagPath)
{
   // FIXME: what happens when the master instance has no shading groups, but another instance does?
   // Will we skip exporting the master and thereby break the others?
   MObject node = dagPath.node();
   MFnDependencyNode fnDGNode(node);
   int instanceNum = dagPath.isInstanced() ? dagPath.instanceNumber() : 0;

   MPlug plug = fnDGNode.findPlug("instObjGroups");

   if (plug.elementByLogicalIndex(instanceNum).isConnected())
   {
      return 1;
   }
   else
   {
      MFnMesh mesh(node);
      MPlug plug = mesh.findPlug("instObjGroups").elementByLogicalIndex(instanceNum);
      MPlugArray conns;
      plug.connectedTo(conns, false, true);
      if (conns.length() != 0) // no per face assigment
         return 1;
      else // check for per face assigment
         return plug.child(0).numElements();
   }
}

// Return whether the dag object in dagPath is the master instance. The master
// is the first instance that is completely visible (including parent transforms)
// for which full geometry should be exported
//
// always returns true if dagPath is not instanced.
// if dagPath is instanced, this searches the preceding instances
// for the first that is visible. if none are found, dagPath is considered the master.
//
// note: dagPath is assumed to be visible.
//
// @param[out] masterDag    the master MDagPath result, only filled if result is false
// @return                  whether or not dagPath is a master
//
bool CMeshTranslator::DoIsMasterInstance(const MDagPath& dagPath, MDagPath &masterDag)
{
   if (dagPath.isInstanced())
   {
      MObjectHandle handle = MObjectHandle(dagPath.node());
      unsigned int instNum = dagPath.instanceNumber();
      // first instance
      if (instNum == 0)
      {
         // first visible instance is always the master (passed dagPath is assumed to be visible)
         m_session->AddMasterInstanceHandle(handle, dagPath);
         return true;
      }
      else
      {
         // if handle is not in the map, a new entry will be made with a default value
         MDagPath currDag = m_session->GetMasterInstanceDagPath(handle);
         if (currDag.isValid())
         {
            // previously found the master
            masterDag.set(currDag);
            return false;
         }
         // find the master by searching preceding instances
         MDagPathArray allInstances;
         MDagPath::getAllPathsTo(dagPath.node(), allInstances);
         for (unsigned int master_index = 0; master_index < instNum; master_index++)
         {
            currDag = allInstances[master_index];
            // The following line was overridden to add the GetNumMeshGroups check
            if (m_session->IsRenderablePath(currDag) && GetNumMeshGroups(currDag) > 0)
            {
               // found it
               m_session->AddMasterInstanceHandle(handle, currDag);
               masterDag.set(currDag);
               return false;
            }
         }
         // didn't find a master: dagPath is the master
         m_session->AddMasterInstanceHandle(handle, dagPath);
         return true;
      }
   }
   // not instanced: dagPath is the master
   return true;
}

AtNode* CMeshTranslator::CreateArnoldNodes()
{
   if (GetNumMeshGroups(m_dagPath) == 0)
   {
      AiMsgError("[mtoa.translator]  %-30s | Mesh not exported, it has 0 shading groups.", GetMayaNodeName().asChar());
      return NULL;
   }

   if (IsMasterInstance())
   {
      const float stepSize = FindMayaPlug("aiStepSize").asFloat();
      if ((stepSize > AI_EPSILON))
         return AddArnoldNode("box");
      else
         return AddArnoldNode("polymesh");
   }
   else
   {
      return AddArnoldNode("ginstance");
   }
}

void CMeshTranslator::Export(AtNode* anode)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, GetMasterInstance());
   }
   else if (strcmp(nodeType, "polymesh") == 0)
   {
      // Early return if we can't tessalate.
      if (!Tessellate(m_dagPath, true))
         return;
      ExportMesh(anode, false);
   }
   else if (strcmp(nodeType, "box") == 0)
   {
      ExportBBox(anode);  
   }
}

void CMeshTranslator::ExportMotion(AtNode* anode, unsigned int step)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportMatrix(anode, step);
   }
   else if (strcmp(nodeType, "polymesh") == 0)
   {
      if (m_motion)
         ExportMatrix(anode, step);
      if (m_motionDeform)
      {
         // Early return if we can't tessalate.
         if (!Tessellate(m_dagPath, false))
            return;
         ExportMeshGeoData(anode, step);
      }
   }
   else if (strcmp(nodeType, "box") == 0)
   {
      ExportMatrix(anode, step);
   }
}

bool CMeshTranslator::IsGeoDeforming()
{
   MFnMesh fnMesh(m_dagPath);
   bool history = false;
   bool pnts = false;

   MPlug inMeshPlug = fnMesh.findPlug("inMesh");
   MPlugArray conn;
   inMeshPlug.connectedTo(conn, true, false);
   if (conn.length())
   {
     history = true;
   }

   inMeshPlug = fnMesh.findPlug("pnts");
   unsigned int numElements = inMeshPlug.numElements();
   if (numElements > 0)
   {
     pnts = true;
   }

   if (!history && !pnts && !m_displaced)
   {
      return false;
   }
   else
   {
      return true;
   }
}

MStatus CMeshTranslator::Tessellate(const MDagPath &path, bool doRef)
{
   MStatus status = MStatus::kSuccess;
   MFnMesh fnMesh(path);
   m_isRefSmooth = false;

   m_geometry = path.node();

   // Check if the object is smoothed with maya method
   if (fnMesh.findPlug("displaySmoothMesh").asBool())
   {
      MMeshSmoothOptions options;
      status = fnMesh.getSmoothMeshDisplayOptions(options);

      CHECK_MSTATUS_AND_RETURN_IT(status);

      if(!fnMesh.findPlug("useSmoothPreviewForRender", false, &status).asBool())
         options.setDivisions(fnMesh.findPlug("renderSmoothLevel", false, &status).asInt());

      if (options.divisions() > 0)
      {
         if (doRef)
         {
            // Check if mesh got a reference object. If so, we must also smooth it and reconnect it
            //FIXME : This has to be done in a better way, but how ?
            MDagPath dagPathRef = GetMeshRefObj();

            if (dagPathRef.isValid())
            {
               MDagPath transform(dagPathRef);
               transform.pop();
               MFnMesh fnMeshRef(dagPathRef);
               MObject mesh_mobj_ref = fnMeshRef.generateSmoothMesh(transform.node(), &options, &status);
               CHECK_MSTATUS_AND_RETURN_IT(status);
               MFnDagNode dag_node(mesh_mobj_ref);
               status = dag_node.getPath(m_dagPathRef);
               CHECK_MSTATUS_AND_RETURN_IT(status);
               m_isRefSmooth = true;
            }
         }

         MFnMeshData meshData;
         // Set m_geometry to new smoothed object
         // This is a member variable. We have to keep hold of it or Maya will release it.
         m_dataMesh = meshData.create();
         MObject m_smooth = fnMesh.generateSmoothMesh(m_dataMesh, &options, &status);
         if (status == MStatus::kSuccess)
         {
            m_geometry = m_smooth;
         }
      }
   }

   return status;
}

void CMeshTranslator::NodeChanged(MObject& node, MPlug& plug)
{  
   MString plugName = plug.name().substring(plug.name().rindex('.'), plug.name().length()-1);
   
   if(plugName == ".pnts" || plugName == ".inMesh" || plugName == ".dispResolution" || plugName == ".useMeshSculptCache" ||
      (plugName.length() > 9 && plugName.substring(0,8) == ".aiSubdiv"))/*|| node.apiType() == MFn::kPluginShape*/
   {
      SetUpdateMode(AI_RECREATE_NODE);
   }
   CPolygonGeometryTranslator::NodeChanged(node, plug);

}

