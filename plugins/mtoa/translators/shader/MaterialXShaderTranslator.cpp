#include "MaterialXShaderTranslator.h"

AtNode* CArnoldMaterialXShaderTranslator::CreateArnoldNodes()
{
   MString mtlx_path = FindMayaPlug("materialXFilePath").asString();
   MString material_name = FindMayaPlug("materialName").asString();

   MFnDependencyNode fnNode(GetMayaObject());
   MString maya_node_name = fnNode.name();




   if (mtlx_path.length()==0 )
   {
      AiMsgError("[mtoa] [translator %s] No materialX file  Provided to node : %s", GetTranslatorName().asChar(), maya_node_name.asChar());
      return NULL;
   }
   AtParamValueMap* hints = AiParamValueMap();

   AiParamValueMapSetStr(hints, AtString("material"), AtString(material_name.asChar()));
   AiParamValueMapSetStr(hints, AtString("shader_prefix"), AtString(maya_node_name.asChar()));
   AtArray* nodes = AiArrayAllocate(0, 0, AI_TYPE_NODE);

   AtNode* root_node;
   if ( AiMaterialxReadMaterials(NULL, mtlx_path.asChar(), hints, nodes) == AI_MATX_SUCCESS )
   {
      unsigned int arrElems = AiArrayGetNumElements(nodes);
      for(unsigned int i =0 ; i < arrElems ; i ++)
      {
         AtNode* node = (AtNode*)AiArrayGetPtr(nodes, i );
         AddExistingArnoldNode(node);
         if (AiNodeLookUpUserParameter(node, "material_surface"))
         {
            root_node = node;
         }
      }
   }

   // unsigned int arrElems = AiArrayGetNumElements(nodes);
   // std::cout << "Array Type is " << AiArrayGetType(nodes) << std::endl;
   // std::cout << " Num elemenets " << arrElems << std::endl;
   

   
   

    
    
   

   //  AtNodeIterator* iter = AiUniverseGetNodeIterator(NULL, mask);
   //  while (!AiNodeIteratorFinished(iter))
   //  {
   //      AtNode* node = AiNodeIteratorGetNext(iter);
   //      if (node == NULL)
   //          continue;

   //      std::string nodeName = AiNodeGetName(node);
   //      AtString result;
   //      std::cout << " NodeName " << nodeName << std::endl;
   //      // AtShaderGlobals *sg;
   //      // std::cout << " User Data" << AiUDataGetStr(AtString("material_surface"),result) << std::endl;
   //  }

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
   // return AddArnoldNode("lambert");
}

// void CArnoldMaterialXShaderTranslator::NodeChanged(MObject& node, MPlug& plug)
// {
//    MString plugName = plug.partialName(false, false, false, false, false, true);
//    if (plugName == "materialXFilePath")
//    {   
//       SetUpdateMode(AI_RECREATE_NODE);
//    }
//    CShaderTranslator::NodeChanged(node, plug);
// }