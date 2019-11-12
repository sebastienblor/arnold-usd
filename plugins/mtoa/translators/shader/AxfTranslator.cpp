#include "AxfTranslator.h"
#include <AxFtoA.h>

AtNode* CArnoldAxfShaderTranslator::CreateArnoldNodes()
{
   MString axf_path = FindMayaPlug("axfFilePath").asString();
   MString tex_path = FindMayaPlug("texturePath").asString();
   float uvScale = FindMayaPlug("uvScale").asFloat();
   MFnDependencyNode fnNode(GetMayaObject());
   MString maya_node_name = fnNode.name();


   if (axf_path.length()==0 )
   {
      AiMsgWarning("[mtoa] [translator %s] No Axf File Path Provided to node : %s", GetTranslatorName().asChar(), maya_node_name.asChar());
      return AddArnoldNode("lambert");
   }
   
   if (tex_path.length()==0)
   {
      AiMsgError("[mtoa] [translator %s] No texture path provided to Node : %s", GetTranslatorName().asChar(), maya_node_name.asChar());
      return AddArnoldNode("lambert");
   }

   AxFtoASessionStart();
   AxFtoASessionClearErrors();
   int verbosity = 0;
   MObject node = GetArnoldRenderOptions();
   if (node != MObject::kNullObj)
   {
      MFnDependencyNode fnArnoldRenderOptions(node);
      MPlug plug;
      plug = fnArnoldRenderOptions.findPlug("log_verbosity", true);
      if (!plug.isNull())
         verbosity = plug.asInt();
   }
   AxFtoASessionSetVerbosity(verbosity);
   
   AxFtoAFile* axf_file = AxFtoAFileOpen(axf_path.asChar());
   
   // TODO : Right now we're assuming that we have only 1 material in the axf file 
   // We need to set the material index based off of user selection
   AxFtoAMaterial *material = AxFtoAFileGetMaterial(axf_file, 0);
   if (AxFtoASessionHasErrors())
   {
      AiMsgError("[mtoa] Unable to perform operation.");
      return NULL; 
   }
   
   
   AxFtoAMaterialSetTextureFolder(material, tex_path.asChar());
   AxFtoAMaterialSetUVUnitSize(material, uvScale);
   
   // TODO : We're also setting the namespace to be a default namespace 
   // This also need to be exposed to the user and set based on user action
   AxFtoAMaterialSetColorSpace(material, "Rec709,E");
   
   AxFtoAMaterialSetUniverse(material, NULL);
   AxFtoAMaterialSetTextureNamePrefix(material, maya_node_name.asChar());
   AxFtoAMaterialSetNodeNamePrefix(material, maya_node_name.asChar());
   AtNode* root_node = AxFtoAMaterialGetRootNode(material);
   AxFtoAMaterialWriteTextures(material);
   int num_arnold_nodes = AxFtoAMaterialGetNumNodes(material);
   for (int i = 0 ; i < num_arnold_nodes ; i++)
   {
      AtNode* node = AxFtoAMaterialGetNode(material, i);
      AddExistingArnoldNode(node);
   }

   if (root_node == NULL)
   {
      AiMsgError("[mtoa] [translator %s] Unable to perform translation. Axf File is still not supported", GetTranslatorName().asChar());
   }

   AxFtoAFileClose(axf_file);
   AxFtoASessionEnd();

   return root_node;
}

void CArnoldAxfShaderTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if ((plugName == "axfFilePath") || (plugName == "uvScale") || (plugName == "texturePath"))
   {   
      SetUpdateMode(AI_RECREATE_NODE);
   }
   CShaderTranslator::NodeChanged(node, plug);
}