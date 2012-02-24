
#include "MeshTranslator.h"

#include <maya/MFnMeshData.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMeshSmoothOptions.h>
#include <maya/MItMeshVertex.h>


unsigned int CMeshTranslator::GetNumMeshGroups()
{
   MObject node = m_dagPath.node();
   MFnDependencyNode fnDGNode(node);
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   MPlug plug = fnDGNode.findPlug("instObjGroups");

   if (plug.elementByLogicalIndex(instanceNum).isConnected())
   {
      return 1;
   }
   else
   {
      MFnMesh      mesh(node);
      MObjectArray shaders;
      MIntArray    indices;

      mesh.getConnectedShaders(instanceNum, shaders, indices);

      return shaders.length();
   }
}

AtNode* CMeshTranslator::CreateArnoldNodes()
{
   m_isMasterDag = IsMasterInstance(m_masterDag);
   if (m_isMasterDag)
   {
      return AddArnoldNode("polymesh");
   }
   else
   {
      return AddArnoldNode("ginstance");
   }
}

void CMeshTranslator::Export(AtNode* anode)
{
   if (GetNumMeshGroups() == 0)
   {
      AiMsgError("[mtoa.translator]  %-30s | Mesh not exported, it has 0 shading groups.", GetMayaNodeName().asChar());
      return;
   }
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, m_masterDag);
   }
   else if (strcmp(nodeType, "polymesh") == 0)
   {
      // Early return if we can't tessalate.
      if (!Tessellate(m_dagPath))
         return;
      ExportMesh(anode, false);
   }
}

bool CMeshTranslator::IsGeoDeforming()
{
   // MFnMesh fnMesh(m_dagPath);
   MFnMesh fnMesh(m_geometry);
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
   inMeshPlug.connectedTo(conn, true, false);
   if (conn.length())
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

MStatus CMeshTranslator::Tessellate(const MDagPath &path)
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

