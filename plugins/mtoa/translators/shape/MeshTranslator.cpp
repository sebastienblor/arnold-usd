
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
      m_fnMesh.setObject(m_dagPath.node());
      return AddArnoldNode("polymesh");
   }
   else
      return AddArnoldNode("ginstance");
}

void CMeshTranslator::Export(AtNode* anode)
{
   if (GetNumMeshGroups() == 0)
   {
      AiMsgError("[mtoa] [translator %s] Mesh not exported, it has 0 groups.", GetTranslatorName().asChar());
      return;
   }
   const char* nodeType = AiNodeEntryGetName(anode->base_node);
   if (strcmp(nodeType, "ginstance") == 0)
      ExportInstance(anode, m_masterDag);
   else if (strcmp(nodeType, "polymesh") == 0)
      ExportMesh(anode, false);
}



