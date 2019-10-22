#include "MaterialXShaderTranslator.h"

AtNode* CArnoldMaterialXShaderTranslator::CreateArnoldNodes()
{
   MString mtlx_path = FindMayaPlug("materialXFilePath").asString();

   MFnDependencyNode fnNode(GetMayaObject());
   MString maya_node_name = fnNode.name();


   if (mtlx_path.length()==0 )
   {
      AiMsgError("[mtoa] [translator %s] No materialX file  Provided to node : %s", GetTranslatorName().asChar(), maya_node_name.asChar());
      return NULL;
   }

   //  AiMaterialxReadMaterials(NULL, mtlx_path.asChar());

    
    AtNode* root_node;
   /*
    unsigned int mask = AI_NODE_ALL;
    AtNodeIterator* iter = AiUniverseGetNodeIterator(NULL, mask);
    while (!AiNodeIteratorFinished(iter))
    {
        AtNode* node = AiNodeIteratorGetNext(iter);
        if (node == NULL)
            continue;

        std::string nodeName = AiNodeGetName(node);
        AtString result;
        std::cout << " NodeName " << nodeName << std::endl;
        // AtShaderGlobals *sg;
        // std::cout << " User Data" << AiUDataGetStr(AtString("material_surface"),result) << std::endl;
    }*/

//    for (int i = 0 ; i < num_arnold_nodes ; i++)
//    {
//       AtNode* node = AxFtoAMaterialGetNode(material, i);
//       AddExistingArnoldNode(node);
//    }

   if (root_node == NULL)
   {
      AiMsgError("[mtoa] [translator %s] Unable to perform translation. Axf File is still not supported", GetTranslatorName().asChar());
   }

   return root_node;
}

void CArnoldMaterialXShaderTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "materialXFilePath")
   {   
      SetUpdateMode(AI_RECREATE_NODE);
   }
   CShaderTranslator::NodeChanged(node, plug);
}