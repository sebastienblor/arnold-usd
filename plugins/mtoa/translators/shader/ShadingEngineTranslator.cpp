#include "ShadingEngineTranslator.h"
#include "ShadingEngineTranslatorImpl.h"
#include "../DagTranslator.h"
#include "scene/MayaScene.h"

CShadingEngineTranslator::~CShadingEngineTranslator()
{
   if (m_impl)
      delete m_impl->m_shaders;
}

void CShadingEngineTranslator::Init()
{
   m_impl->m_shaders = new std::set<AtNode*>;//AtNodeSet;
   CNodeTranslator::Init();
}

AtNode*  CShadingEngineTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaShadingEngine");
}

void CShadingEngineTranslator::CreateImplementation()
{
   m_impl = new CShadingEngineTranslatorImpl(*this);
}

void CShadingEngineTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, NULL, "ai_", false);

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
   
   helper.MakeInputRGB(data);
   
   data.name = "aiVolumeShader";
   data.shortName = "ai_volume_shader";
   
   helper.MakeInputRGB(data);
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
   if ((CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER) == 0)
      return;
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
      CNodeTranslator* shaderNodeTranslator = 0;
      // here we call the private implementation function as we need the output translator
      rootShader = m_impl->ExportConnectedNode(connections[0], true, &shaderNodeTranslator);
      if (rootShader)
      {
         AiNodeLink(rootShader, "beauty", shadingEngine);
      
         if (shaderNodeTranslator)
         {
            MStatus status;
            MPlug mattePlug = shaderNodeTranslator->FindMayaPlug("aiEnableMatte", &status);
            if (status)
               ProcessParameter(shadingEngine, "enable_matte", AI_TYPE_BOOLEAN, mattePlug);
            MPlug matteColorPlug = shaderNodeTranslator->FindMayaPlug("aiMatteColor", &status);
            if (status)
               ProcessParameter(shadingEngine, "matte_color", AI_TYPE_RGBA, matteColorPlug);
         }
      }

      // loop through and export custom AOV networks
      CShadingEngineTranslatorImpl *trImpl = static_cast<CShadingEngineTranslatorImpl*>(m_impl);
      for (unsigned int i = 0; i < trImpl->m_customAOVPlugs.length(); i++)
      {
         // by passing false we avoid tracking shaders and aovs.
         // we need to call the private implementation function to prevent shaders tracking
         AtNode* writeNode = m_impl->ExportConnectedNode(trImpl->m_customAOVPlugs[i], false);
         
         // since we know this maya node is connected to aiCustomAOVs it will have a write node
         // inserted after it by CShaderTranslator::ProcessAOVOutput (assuming the node is translated by
         // CShaderTranslator)
         // TODO: check shader type: rootShader should always be an aov write node, unless it is a conversion node

         // if the node is not yet in the shading network for this shape, then branch it in.
         // m_shaders contains all the arnold nodes in a shape's shading network.
         if (!m_impl->m_shaders->count(writeNode))
         {
            aovShaders.push_back(writeNode);
         }
      }
   }
   else
      AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
            GetTranslatorName().asChar(), GetMayaNodeName().asChar());
   
   connections.clear();
   MPlug volumeShaderPlug = FindMayaPlug("aiVolumeShader");
   volumeShaderPlug.connectedTo(connections, true, false);
   if (connections.length() == 0)
   {
      volumeShaderPlug = FindMayaPlug("volumeShader");
      AiMsgDebug("[mtoa] CShadingEngineTranslator::Export found volumeShader plug %s", volumeShaderPlug.name().asChar());
      volumeShaderPlug.connectedTo(connections, true, false);
   }
   if (connections.length() > 0)
   {
      // export the root shading network, this fills m_shaders
      MFnDependencyNode shaderNode(connections[0].node());
      MStatus status;
      rootShader = ExportConnectedNode(connections[0]);
      AiNodeLink(rootShader, "volume", shadingEngine);
   }

   m_impl->AddAOVDefaults(shadingEngine, aovShaders); // modifies aovShaders list
}

void CShadingEngineTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.name().substring(plug.name().rindex('.'), plug.name().length()-1);
   if(plugName == ".displacementShader")
   {
      MFnDependencyNode dnode(node);
      std::vector< CNodeTranslator * > translatorsToUpdate;
      bool reexport = true;
      MPlug dagSetMembersPlug = dnode.findPlug("dagSetMembers");
      const unsigned int numElements = dagSetMembersPlug.numElements();
      for(unsigned int i = 0; i < numElements; i++)
      {
         MPlug a = dagSetMembersPlug[i];
         MPlugArray connectedPlugs;
         a.connectedTo(connectedPlugs,true,false);

         const unsigned int connectedPlugsLength = connectedPlugs.length();
         for(unsigned int j = 0; j < connectedPlugsLength; j++)
         {
            MPlug connection = connectedPlugs[j];
            MObject parent = connection.node();
            MFnDependencyNode parentDag(parent);
            MString nameParent = parentDag.name();

            MDagPath dagPath;
            MStatus status = MDagPath::getAPathTo(parent, dagPath);
            if (!status)
               continue;

            CNodeTranslator* translator2 = GetSession()->ExportDagPath(dagPath, true);

            if (translator2 == 0)
               continue;

            // TODO: By now we have to check the connected nodes and if something that is not a mesh
            //  is connected, we do not reexport, as some crashes may happen.
            if(translator2->GetMayaNodeTypeName() != "mesh")
            {
               reexport = false;
               break;
            }

            translatorsToUpdate.push_back(translator2);
         }

         if(reexport == false)
            break;
      }

      // We only reexport if all nodes connected to the displacement are mesh nodes
      if (reexport)
      {
         for (std::vector<CNodeTranslator*>::iterator iter = translatorsToUpdate.begin();
            iter != translatorsToUpdate.end(); ++iter)
         {
            CNodeTranslator* translator3 = (*iter);
            if (translator3 != NULL)
            {
               translator3->SetUpdateMode(AI_RECREATE_NODE);
               translator3->RequestUpdate();
            }
         }
      }
   }
   CNodeTranslator::NodeChanged(node, plug);
}

