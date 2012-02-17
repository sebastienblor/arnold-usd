#include "ShadingEngineTranslator.h"

AtNode*  CShadingEngineTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaShadingEngine");
}


void CShadingEngineTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya);

   std::vector<CAttrData> children(2);

   children[0].name = "aovName";
   children[0].shortName = "aov_name";
   children[0].type = AI_TYPE_STRING;

   children[1].name = "aovInput";
   children[1].shortName = "aov_input";
   children[1].type = AI_TYPE_NODE;

   CAttrData data;
   data.name = "aiCustomAOVs";
   data.shortName = "aovs";
   data.isArray = true;

   helper.MakeInputCompound(data, children);
}

/// Find and export the surfaceShader for the passed shadingGroup and add the AOV defaults.
void CShadingEngineTranslator::Export(AtNode *shadingEngine)
{
   std::vector<AtNode*> aovShaders;
   AtNode* rootShader = NULL;
   MPlugArray        connections;
   MPlug shaderPlug = FindMayaObjectPlug("surfaceShader");
   shaderPlug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      // export the root shading network, this fills m_shaders
      rootShader = ExportNode(connections[0]);
      AiNodeLink(rootShader, "beauty", shadingEngine);

      // loop through and export custom AOV networks
      MPlug arrayPlug = FindMayaObjectPlug("aiCustomAOVs");
      for (unsigned int i = 0; i < arrayPlug.numElements (); i++)
      {
         MPlug msgPlug = arrayPlug[i].child(1);
         msgPlug.connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            // by using m_session->ExportNode we avoid tracking aovs.
            AtNode* writeNode = m_session->ExportNode(connections[0]);
            // since we know this maya node is connected to aiCustomAOVs it will have a write node
            // inserted after it by CShaderTranslator::ProcessAOVOutput (assuming the node is translated by
            // CShaderTranslator)
            // TODO: check shader type: rootShader should always be an aov write node, unless it is a conversion node

            // if the node is not yet in the shading network for this shape, then branch it in.
            // m_shaders contains all the arnold nodes in a shape's shading network, as tracked by ExportRootShader.
            if (!m_shaders->count(writeNode))
            {
               aovShaders.push_back(writeNode);
            }
         }
      }
   }
   else
      AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
            GetTranslatorName().asChar(), GetMayaNodeName().asChar());

   AddAOVDefaults(aovShaders); // modifies aovShaders list
   AiNodeSetArray(shadingEngine, "aov_inputs", AiArrayConvert(aovShaders.size(), 1, AI_TYPE_NODE, &aovShaders[0]));
}

