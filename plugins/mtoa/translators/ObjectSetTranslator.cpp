#include "ObjectSetTranslator.h"
#include <maya/MFnSet.h>
#include <maya/MDagPathArray.h>

void CObjectSetTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya);

   CAttrData data;
   data.name = "aiOverride";
   data.shortName = "ai_override";
   data.type = AI_TYPE_BOOLEAN;

   helper.MakeInput(data);
}

// This translator doesn't create Arnold nodes,
// it's only purpose is to create additionnal attributes on Sets
AtNode*  CObjectSetTranslator::CreateArnoldNodes()
{
   return NULL;
}

void CObjectSetTranslator::Export(AtNode *set)
{
   AiMsgDebug("[mtoa.translator]  %s: Maya node %s(%s).",
               GetTranslatorName().asChar(), GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar());
}

/// Sets have extra specific callback addAttributeChangedCallback
void CObjectSetTranslator::AddUpdateCallbacks()
{
   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Add update callbacks on translator %p", GetMayaNodeName().asChar(), GetTranslatorName().asChar(), this);
   MStatus status;
   MCallbackId id;

   MObject object = GetMayaObject();
   // So we update on attribute/input changes.
   id = MNodeMessage::addNodeDirtyCallback(object,
                                           NodeDirtyCallback,
                                           this,
                                           &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);

   // In case we're deleted!
   id = MNodeMessage::addNodeAboutToDeleteCallback(object,
                                                   NodeDeletedCallback,
                                                   this,
                                                   &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);

   // Set members change
   id = MNodeMessage::addAttributeChangedCallback(object,
                                                  AttributeChangedCallback,
                                                  this,
                                                  &status);
   if (MS::kSuccess == status) ManageUpdateCallback(id);
}

void CObjectSetTranslator::NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData)
{
   AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: plug that fired: %s, client data: %p.",
         MFnDependencyNode(node).name().asChar(), plug.name().asChar(), clientData);

   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      if ((plug.partialName()=="dsm") && (plug.partialName()=="dnsm"))
      {
         AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: ignoring plug %s.",
                  MFnDependencyNode(node).name().asChar(), plug.name().asChar());
      }
      else if ((plug.partialName()=="ai_override") || translator->FindMayaPlug("aiOverride").asBool())
      {
         AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: client data is translator %s, providing Arnold %s(%s): %p",
                          translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                          translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
         translator->RequestUpdate(clientData);
      }
   }
   else
   {
      AiMsgWarning("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: no translator in client data: %p.",
            translator->GetMayaNodeName().asChar(), clientData);
   }
}

void CObjectSetTranslator::AttributeChangedCallback(MNodeMessage::AttributeMessage msg,
                                                    MPlug& plug, MPlug& otherPlug,
                                                    void* clientData)
{
   CObjectSetTranslator * translator = static_cast< CObjectSetTranslator* >(clientData);
   if ((translator != NULL) && translator->FindMayaPlug("aiOverride").asBool())
   {
      AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: AttributeChangedCallback %s to or from %s: %p.",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                 plug.name().asChar(), otherPlug.name().asChar(), clientData);
      // NOTE: no need for full refresh of set
      // translator->RequestUpdate(clientData);
      // Only refresh new member or removed member
      if (msg & MNodeMessage::kOtherPlugSet & MNodeMessage::kIncomingDirection
            & (MNodeMessage::kConnectionMade | MNodeMessage::kConnectionBroken))
      {
         CNodeTranslator* tr;
         std::vector<CNodeTranslator*> translators;
         std::vector<CNodeTranslator*>::iterator it;
         if (plug.partialName() == "dsm")
         {
            // dag node
            MObject object(otherPlug.node());
            MFnDagNode fnDag(object);
            unsigned int instanceNumber = -1;
            instanceNumber = otherPlug.logicalIndex();
            MDagPath path;
            if (instanceNumber == -1)
            {
               fnDag.getPath(path);
            }
            else
            {
               MDagPathArray allPaths;
               fnDag.getAllPaths(allPaths);
               path = allPaths[instanceNumber];
            }
            if (path.isValid())
            {
               CNodeAttrHandle handle(path);
               AiMsgDebug("[mtoa.translator.ipr] %-30s | Looking for processed translators for %s.",
                   translator->GetMayaNodeName().asChar(), path.partialPathName().asChar());
               if (translator->m_session->GetActiveTranslators(handle, translators) > 0)
               {
                  for (it=translators.begin(); it!=translators.end(); it++)
                  {
                     tr = static_cast< CNodeTranslator* >(*it);
                     tr->RequestUpdate((void *)tr);
                  }
               }
            }
            // Check also for shapes
            if (MStatus::kSuccess == path.extendToShape())
            {
               CNodeAttrHandle handle(path);
               AiMsgDebug("[mtoa.translator.ipr] %-30s | Looking for processed translators for %s.",
                   translator->GetMayaNodeName().asChar(), path.partialPathName().asChar());
               if (translator->m_session->GetActiveTranslators(handle, translators) > 0)
               {
                  for (it=translators.begin(); it!=translators.end(); it++)
                  {
                     tr = static_cast< CNodeTranslator* >(*it);
                     tr->RequestUpdate((void *)tr);
                  }
               }
            }
         }
         else if (plug.partialName() == "dnsm")
         {
            // dependency node
            CNodeAttrHandle handle(otherPlug.node());

            AiMsgDebug("[mtoa.translator.ipr] %-30s | Looking for processed translators for %s.%s",
                translator->GetMayaNodeName().asChar(), MFnDependencyNode(handle.object()).name().asChar(), handle.attribute().asChar());

            if (translator->m_session->GetActiveTranslators(handle, translators) > 0)
            {
               for (it=translators.begin(); it!=translators.end(); it++)
               {
                  tr = static_cast< CNodeTranslator* >(*it);
                  tr->RequestUpdate((void *)tr);
               }
            }
         }
      }
      else if (msg & (MNodeMessage::kAttributeAdded | MNodeMessage::kAttributeRemoved))
      {
         // Some override attribute added or removed
         translator->RequestUpdate((void *)translator);
      }
   }
   else
   {
      // No translator in client data
      AiMsgError("[mtoa.translator.ipr] AttributeChangedCallback: no translator in client data: %p.", clientData);
   }
}

/// Update a set means update all members
void CObjectSetTranslator::RequestUpdate(void *clientData)
{
   // Remove this node from the callback list.
   CNodeTranslator * translator = static_cast< CNodeTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: RequestUpdate: Arnold node %s(%s): %p.",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                 translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
      translator->RemoveUpdateCallbacks();
      // Add translator to the list of translators to update
      m_session->QueueForUpdate(translator);
   }
   else
   {
      // Deletion doesn't pass a translator
      AiMsgDebug("[mtoa.translator.ipr] RequestUpdate: no translator in client data: %p.", clientData);
   }

   // FIXME : if it's dagSetMembers or dnSetMembers that triggered update, should not do a full update

   // Update means all members should be updated
   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Update all set members.",
              translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(), clientData);
   MFnSet fnSet(GetMayaObject());
   MSelectionList list;
   fnSet.getMembers(list, false);
   MObject element;
   MDagPath path;

   CNodeTranslator* tr;
   std::vector<CNodeTranslator*> translators;
   std::vector<CNodeTranslator*>::iterator it;
   unsigned int l = list.length();
   for (unsigned int i=0; i<l; i++)
   {
      if (MStatus::kSuccess == list.getDagPath(i, path))
      {
         CNodeAttrHandle handle(path);
         AiMsgDebug("[mtoa.translator.ipr] %-30s | Looking for processed translators for %s.",
                translator->GetMayaNodeName().asChar(), MFnDagNode(handle.object()).partialPathName().asChar());
         if (m_session->GetActiveTranslators(handle, translators) > 0)
         {
            for (it=translators.begin(); it!=translators.end(); it++)
            {
               tr = static_cast< CDagTranslator* >(*it);
               tr->RequestUpdate((void *)tr);
            }
         }
         // Check also for shape
         // FIXME: check for whole hierarchy?
         if (MStatus::kSuccess == path.extendToShape())
         {
            CNodeAttrHandle handle(path);
            AiMsgDebug("[mtoa.translator.ipr] %-30s | Looking for processed translators for %s.",
                translator->GetMayaNodeName().asChar(), MFnDagNode(handle.object()).partialPathName().asChar());
            if (m_session->GetActiveTranslators(handle, translators) > 0)
            {
               for (it=translators.begin(); it!=translators.end(); it++)
               {
                  tr = static_cast< CDagTranslator* >(*it);
                  tr->RequestUpdate((void *)tr);
               }
            }
         }
      }
      else if (MStatus::kSuccess == list.getDependNode(i, element))
      {
         CNodeAttrHandle handle(element);
         AiMsgDebug("[mtoa.translator.ipr] %-30s | Looking for processed translators for %s.%s",
                translator->GetMayaNodeName().asChar(), MFnDependencyNode(handle.object()).name().asChar(), handle.attribute().asChar());
         if (m_session->GetActiveTranslators(handle, translators) > 0)
         {
            for (it=translators.begin(); it!=translators.end(); it++)
            {
               tr = static_cast< CNodeTranslator* >(*it);
               tr->RequestUpdate((void *)tr);
            }
         }
      }
      else
      {
         AiMsgError("[mtoa.translator.ipr] %-30s | Cannot get member %i of set.",
                translator->GetMayaNodeName().asChar(), i);
      }
   }

   // Pass the update request to the export session
   m_session->RequestUpdate();
}
