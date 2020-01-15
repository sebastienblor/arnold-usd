#include "MaterialXShaderTranslator.h"

AtNode* CArnoldMaterialXShaderTranslator::CreateArnoldNodes()
{
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
            // std::cout << " Displacement found "  << AiNodeGetStr(node, "material_displacement") <<std::endl;
            std::cout << " Displacment Found Node Name " << AiNodeGetName(node) << std::endl;
            AtString shadingGroup = AiNodeGetStr(node, "material_displacement");
            MStatus stat;
            MPlug plug = fnNode.findPlug("outColor", true, &stat);
            if (stat == MStatus::kSuccess && plug.isConnected())
            {
               MPlugArray filtermapPlug;
               plug.connectedTo(filtermapPlug, false, true);
               if (filtermapPlug.length() > 0)
               {
                  std::cout << "Connected Plug is " <<filtermapPlug[0].name().asChar() << std::endl;
                  MFnDependencyNode shadingEngine(filtermapPlug[0].node());
                  // MString connectCmd = MString("connectAttr -f ") + maya_node_name + MString(".displacement ") + MString(shadingEngine.name()) + MString(".displacementShader");
                  // std::cout << " Connect command is " <<connectCmd.asChar() << std::endl;
               }
            // MGlobal::executeCommand(connectCmd);
            }
         }
      }
   }
   if (root_node == NULL)
   {
      AiMsgError("[mtoa] [translator %s] Unable to perform translation", GetTranslatorName().asChar());
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

void CArnoldMaterialXShaderTranslator::Export(AtNode* shader)
{
   std::cout << " Export is being called" << std::endl;
}