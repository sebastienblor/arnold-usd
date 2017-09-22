#include "ShadingEngineTranslator.h"
#include "ShadingEngineTranslatorImpl.h"
#include "../DagTranslator.h"
#include "scene/MayaScene.h"
#include "utils/MtoaLog.h"

CShadingEngineTranslator::~CShadingEngineTranslator()
{
}

// FIXME to be removed in next ABI-breaking version
void CShadingEngineTranslator::Init()
{
   CNodeTranslator::Init();
}

static MString GetAOVWriteNodeType(int type)
{
   MString nodeType = "";
   switch (type)
   {
   case AI_TYPE_FLOAT:
      nodeType = "aov_write_float";
      break;
   case AI_TYPE_INT:
   case AI_TYPE_UINT:
   case AI_TYPE_BYTE:
      nodeType = "aov_write_int";
      break;
   default:
   case AI_TYPE_RGB:
   case AI_TYPE_RGBA:
   case AI_TYPE_VECTOR:
   case AI_TYPE_VECTOR2:
      nodeType = "aov_write_rgb";
      break;
   }
   return nodeType;
}

AtNode*  CShadingEngineTranslator::CreateArnoldNodes()
{
   MPlugArray connections;
   MPlug arrayPlug = FindMayaPlug("aiCustomAOVs");

   for (unsigned int i = 0; i < arrayPlug.numElements (); i++)
   {
      MPlug msgPlug = arrayPlug[i].child(1);
      msgPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode *aovNode = ExportConnectedNode(connections[0]);
         if (aovNode == NULL) continue;

         MString aovNodeName = GetAOVWriteNodeType(AiNodeEntryGetOutputType(AiNodeGetNodeEntry(aovNode)));

         return AddArnoldNode(aovNodeName.asChar()); // do not set the connections yet
      }
   }
   return NULL;
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
   data.defaultValue.RGB() = AI_RGB_BLACK;

   helper.MakeInputRGB(data);
   
   data.name = "aiVolumeShader";
   data.shortName = "ai_volume_shader";
   
   helper.MakeInputRGB(data);
}

/// Shading Engine translator no longer exports MayaShadingEngine shader.
/// Now, it only needs to create AtNodes if it has custom AOVs plugged.
/// In that case, it will return a chained list of aovWrite nodes.
void CShadingEngineTranslator::Export(AtNode *node)
{
   // general case : no custom AOVs
   if (node == NULL)
      return;

   MPlugArray connections;
   MPlug arrayPlug = FindMayaPlug("aiCustomAOVs");

   std::vector<AtNode*> aovWriteNodes;

   // first, get the list of custom AOVs, export each of them,
   // and if there's more than a single one, connect them in a chained list
   // though attribute "passthrough"
   for (unsigned int i = 0; i < arrayPlug.numElements (); i++)
   {
      MPlug msgPlug = arrayPlug[i].child(1);
      msgPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode *aovNode = ExportConnectedNode(connections[0]);
         if (aovNode == NULL) continue; // no shader connected 

         MString aovNodeName = GetAOVWriteNodeType(AiNodeEntryGetOutputType(AiNodeGetNodeEntry(aovNode)));
         
         MString aovValue = arrayPlug[i].child(0).asString();
         if (aovValue.length() == 0) continue; // no AOV name

         MString tag("");
         if (!aovWriteNodes.empty())
         {
            tag += "aov";
            tag += i;
         }

         AtNode *aovWriteNode = GetArnoldNode(tag.asChar());
         if (aovWriteNode == NULL)
            aovWriteNode = AddArnoldNode(aovNodeName.asChar(), tag.asChar()); 

         AiNodeSetStr(aovWriteNode, "aov_name", aovValue.asChar());
         AiNodeLink(aovNode, "aov_input", aovWriteNode);
         
         if (!aovWriteNodes.empty())
            AiNodeLink(aovWriteNode, "passthrough", aovWriteNodes.back());
         
         aovWriteNodes.push_back(aovWriteNode);
      }
   }

   if (aovWriteNodes.empty()) return; // if there's no custom AOV, there's nothing more to do 

   // If I've exported aov write nodes, then I need to export the connected surface shader,
   // and connect it to my aovWrite list
   connections.clear();

   MStringArray shaderNames;
   shaderNames.append("aiSurfaceShader");
   shaderNames.append("surfaceShader");
   shaderNames.append("aiVolumeShader");
   shaderNames.append("volumeShader");
   
   MPlug shaderPlug;
   for (unsigned int i = 0; i < 4; ++i)
   {
      MPlug plug = FindMayaPlug(shaderNames[i]);
      if(plug.isNull()) continue;

      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         shaderPlug = connections[0];
         break;
      }
   }
   
   if (shaderPlug.isNull()) return; // no shader assigned

   // Now export the assigned surface shader
   AtNode *assignedShader = ExportConnectedNode(shaderPlug);
   
   if(assignedShader == NULL) return; // no shader exported

   // connect the assigned shader to the latest aovWrite node
   AiNodeLink(assignedShader, "passthrough", aovWriteNodes.back());
}

void CShadingEngineTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);

   // we happen to receive this signal quite often, but it doesn't seem to affect the render.
   // For example, when we select a shader in the hypershade (#2540), it used to trigger a re-render.
   // We're returning without calling CNodeTranslator::NodeChanged so that it doesn't request an update
   if(plugName == "dagSetMembers") return;

   if(plugName == "displacementShader")
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

            CNodeTranslator* translator2 = m_impl->m_session->ExportDagPath(dagPath, true);

            if (translator2 == 0)
               continue;

            // TODO: By now we have to check the connected nodes and if something that is not a mesh
            //  is connected, we do not reexport, as some crashes may happen.
            if(MFnDependencyNode(translator2->GetMayaObject()).typeName() != "mesh")
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

   if (plugName == "aiCustomAOVs")
      SetUpdateMode(AI_RECREATE_NODE);

   CNodeTranslator::NodeChanged(node, plug);
}

