#include "MaterialXShaderTranslator.h"

AtNode* CArnoldMaterialXShaderTranslator::CreateArnoldNodes()
{
  
   MString outputAttr = GetMayaOutputAttributeName();

   MString mtlx_path = FindMayaPlug("materialXFilePath").asString();
   MString material_name = FindMayaPlug("materialName").asString();

   MFnDependencyNode fnNode(GetMayaObject());
   MString maya_node_name = fnNode.name();

   if (mtlx_path.length()==0 )
   {
      AiMsgWarning("[mtoa] [translator %s] No materialX file provided to node : %s", GetTranslatorName().asChar(), maya_node_name.asChar());
      return AddArnoldNode("lambert");
   }

   if (material_name.length()==0 )
   {
      AiMsgWarning("[mtoa] [translator %s] No material name provided to node : %s", GetTranslatorName().asChar(), maya_node_name.asChar());
      return AddArnoldNode("lambert");
   }

   AtParamValueMap* hints = AiParamValueMap();

   AiParamValueMapSetStr(hints, AtString("material"), AtString(material_name.asChar()));
   AiParamValueMapSetStr(hints, AtString("shader_prefix"), AtString(maya_node_name.asChar()));
   AtArray* nodes = AiArrayAllocate(0, 0, AI_TYPE_NODE);

   AtNode* root_node = NULL;
   AtNode* displ_node = NULL;
   AtNode* volume_node = NULL;

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
         else if (AiNodeLookUpUserParameter(node, "material_displacement"))
         {
            displ_node = node;
         }
         else if(AiNodeLookUpUserParameter(node, "material_volume"))
         {
            volume_node = node;
         }
         
      }
   }
   if (root_node == NULL )
   {
      AiMsgError("[mtoa] [translator %s] Unable to perform translation", GetTranslatorName().asChar());
   }

   if (outputAttr == "outDisplacement")
   {
      return displ_node;
   }
   else if (outputAttr == "outVolume" || outputAttr == "aiVolumeShader")
   {
      return volume_node;
   }


   return root_node;
}

void CArnoldMaterialXShaderTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "materialXFilePath" || plugName == "materialName")
   {   
      SetUpdateMode(AI_RECREATE_NODE);
   }
   CShaderTranslator::NodeChanged(node, plug);
}
