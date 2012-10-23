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
   
   data.name = "aiSurfaceShader";
   data.shortName = "ai_surface_shader";
   data.isArray = false;
   
   helper.MakeInputNode(data);
}

/// Compute the shading engine's AOVs. these are connected to aiCustomAOVs compound array.
/// note that the final list of AOVs as tracked on "mtoa_aovs" user parameter does not include AOV global defaults
void CShadingEngineTranslator::ComputeAOVs()
{
   MPlugArray connections;
   // loop through and export custom AOV networks
   MPlug arrayPlug = FindMayaPlug("aiCustomAOVs");
   for (unsigned int i = 0; i < arrayPlug.numElements (); i++)
   {
      MPlug msgPlug = arrayPlug[i].child(1);
      msgPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         CAOV aov;
         MString value = arrayPlug[i].child(0).asString();
         aov.SetName(value);
         if (m_session->IsActiveAOV(aov))
         {
            m_localAOVs.insert(aov);
            m_customAOVPlugs.append(connections[0]);
            AiMsgDebug("[mtoa.translator.aov] %-30s | \"%s\" is active on attr %s",
                       GetMayaNodeName().asChar(), value.asChar(), msgPlug.partialName(false, false, false, false, true, true).asChar());
         }
      }
   }
}

/// Find and export the surfaceShader and custom AOVs for the passed shadingGroup, and add the global AOV defaults.
///
/// Nodes to be written to AOVs are connected to a special attribute on the shading group called aiCustomAOVs.
/// The simplest solution to exporting these custom AOVs would be to branch them in at the root of the network.
/// However, because Arnold lacks output caching, and considering that the nodes connected to aiCustomAOVs may
/// appear elsewhere in the shape's shading network, we must take great pains to build linear node networks in
/// order to avoid entire sub-networks from being evaluated multiple times during render (i.e., we must avoid a
/// node's output being connected to more than one node). So instead of a simple branching design, we must insert the
/// AOV write nodes within the body of the network, immediately following the node whose output needs to be written.
/// AOVs that are exported within the shading network are handled by CShaderTranslator::ProcessAOVOutput, while
/// the remaining custom AOVs are processed by CShadingEngineTranslator::Export.
void CShadingEngineTranslator::Export(AtNode *shadingEngine)
{
   std::vector<AtNode*> aovShaders;
   AtNode* rootShader = NULL;
   MPlugArray        connections;
   MPlug shaderPlug = FindMayaPlug("aiSurfaceShader");
   shaderPlug.connectedTo(connections, true, false);
   if (connections.length() == 0)
   {
      shaderPlug = FindMayaPlug("surfaceShader");
      AiMsgDebug("[mtoa] CShadingEngineTranslator::Export found surfaceShader plug %s", shaderPlug.name().asChar());
      shaderPlug.connectedTo(connections, true, false);
   }
   if (connections.length() > 0)
   {
      // export the root shading network, this fills m_shaders
      rootShader = ExportNode(connections[0]);
      AiNodeLink(rootShader, "beauty", shadingEngine);

      // loop through and export custom AOV networks
      for (unsigned int i = 0; i < m_customAOVPlugs.length(); i++)
      {
         // by passing false we avoid tracking shaders and aovs.
         AtNode* writeNode = ExportNode(m_customAOVPlugs[i], false);
         // since we know this maya node is connected to aiCustomAOVs it will have a write node
         // inserted after it by CShaderTranslator::ProcessAOVOutput (assuming the node is translated by
         // CShaderTranslator)
         // TODO: check shader type: rootShader should always be an aov write node, unless it is a conversion node

         // if the node is not yet in the shading network for this shape, then branch it in.
         // m_shaders contains all the arnold nodes in a shape's shading network.
         if (!m_shaders->count(writeNode))
         {
            aovShaders.push_back(writeNode);
         }
      }
   }
   else
      AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
            GetTranslatorName().asChar(), GetMayaNodeName().asChar());

   AddAOVDefaults(shadingEngine, aovShaders); // modifies aovShaders list
}

