#include "translators.h"

#include <ai_msg.h>
#include <ai_nodes.h>

void MtoaTestProcTranslator::Export(AtNode* shader)
{	
	CShapeTranslator::Export(shader);
	ExportShaders();

}

AtNode* MtoaTestProcTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("mtoa_test_proc");
}

void MtoaTestProcTranslator::ExportShaders()
{
   AtNode *node = GetArnoldNode();
   if (node == NULL)
      return;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), 0);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *shader = ExportConnectedNode(shadingGroupPlug);
      if (shader != NULL)
      {
         AiNodeSetPtr(node, "shader", shader);
      }
      else
      {         
         AiNodeSetPtr(node, "shader", NULL);
      }
   }
}


AtNode* MtoaTestShaderTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("mtoa_test_shader");
}

void MtoaTestShaderTranslator::Export(AtNode* shader)
{
	std::cerr<<"export shader"<<std::endl;
	CShaderTranslator::Export(shader);

}
