#include "MeshTranslator.h"
#include <maya/MFnMeshData.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMeshSmoothOptions.h>
#include <maya/MItMeshVertex.h>
#include <maya/MDagPathArray.h>

static unsigned int GetNumMeshGroups(const MDagPath& dagPath)
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

bool CMeshTranslator::IsRenderable() const
{
   return CPolygonGeometryTranslator::IsRenderable() && GetNumMeshGroups(m_dagPath) > 0;
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
      return AddArnoldNode("polymesh");
   }
   else
   {
      return AddArnoldNode("ginstance");
   }
}

void CMeshTranslator::ExportMotion(AtNode* anode)
{
   ExportMatrix(anode);

   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "polymesh") == 0)
   {
      // if this is an export update, we haven't called ExportMeshGeoData
      if (m_motionDeform && !IsExported())
      {
         // Early return if we can't tessalate.
         if (!Tessellate(m_dagPath))
            return;
         ExportMeshGeoData(anode);
      }
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

bool CMeshTranslator::Tessellate(const MDagPath &path)
{
   bool doRef = (!IsExportingMotion());

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

   return (status == MS::kSuccess);
}

void CMeshTranslator::NodeChanged(MObject& node, MPlug& plug)
{  
   MString plugName = plug.name().substring(plug.name().rindex('.'), plug.name().length()-1);
   
   bool recreate_geom = (plugName == ".pnts" || plugName == ".inMesh" || plugName == ".dispResolution" || plugName == ".useMeshSculptCache");
   recreate_geom = recreate_geom || (plugName.length() > 9 && plugName.substring(0,8) == ".aiSubdiv")/*|| node.apiType() == MFn::kPluginShape*/;
   recreate_geom = recreate_geom || (plugName.indexW("mooth") >= 1); // parameters relative to smooth
   recreate_geom = recreate_geom || (plugName.length() > 7 && plugName.substring(0,6) == ".aiDisp");
   
   // UVs being changed. Most of the time they trigger a change in .inMesh
   // but not always
   recreate_geom = recreate_geom || (plugName.indexW("uvSetPoints") >= 1); 
   
   if (recreate_geom)
      SetUpdateMode(AI_RECREATE_NODE);
   
   CPolygonGeometryTranslator::NodeChanged(node, plug);

}

