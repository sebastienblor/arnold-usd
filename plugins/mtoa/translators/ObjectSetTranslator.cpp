#include "ObjectSetTranslator.h"
#include "NodeTranslatorImpl.h"
#include <maya/MFnSet.h>
#include <maya/MDagPathArray.h>
#include <maya/MObjectSetMessage.h>
#include <maya/MItDependencyGraph.h>

void CObjectSetTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, NULL, "ai_", false);

   CAttrData data;
   data.defaultValue.BOOL = true;
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
   // We don't call CNodeTranslator::Export
   // because we don't want to export anything

   AiMsgDebug("[mtoa.translator]  %s: Maya node %s(%s).",
               GetTranslatorName().asChar(), GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar());

}

/// Sets have extra specific callback addAttributeChangedCallback
void CObjectSetTranslator::AddUpdateCallbacks()
{
   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Add update callbacks on translator %p",
         GetMayaNodeName().asChar(), GetTranslatorName().asChar(), this);
   MStatus status;
   MCallbackId id;

   MObject object = GetMayaObject();
   // So we update on attribute/input changes.
   id = MNodeMessage::addNodeDirtyCallback(object,
                                           NodeDirtyCallback,
                                           this,
                                           &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   // In case we're deleted!
   id = MNodeMessage::addNodeAboutToDeleteCallback(object,
                                                   NodeAboutToBeDeletedCallback,
                                                   this,
                                                   &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   // Set members change (with precise info of what was added or removed)
   id = MNodeMessage::addAttributeChangedCallback(object,
                                                  AttributeChangedCallback,
                                                  this,
                                                  &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   // Set members have changed
   id = MObjectSetMessage::addSetMembersModifiedCallback(object,
                                                         SetMembersChangedCallback,
                                                         this,
                                                         &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

}

void CObjectSetTranslator::NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData)
{
   MString nodeName = MFnDependencyNode(node).name();
   MString plugName = plug.name();
   AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: plug that fired: %s, client data: %p.",
               nodeName.asChar(), plugName.asChar(), clientData);

   CObjectSetTranslator * translator = static_cast< CObjectSetTranslator* >(clientData);
   if (translator != NULL)
   {
      if ((plug.partialName()=="dsm") || (plug.partialName()=="dnsm"))
      {
         AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: ignoring plug %s.",
                     nodeName.asChar(), plugName.asChar());
      }
      else if ((plug.partialName()=="ai_override") || translator->FindMayaPlug("aiOverride").asBool())
      {
         // Only update if THIS set is active (not containing sets)
         AiMsgDebug("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: client data is translator %s, providing Arnold %s(%s): %p",
                          translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                          translator->GetArnoldNodeName(), translator->GetArnoldTypeName(), translator->GetArnoldNode());
         translator->RequestUpdate();
      }
   }
   else
   {
      AiMsgWarning("[mtoa.translator.ipr] %-30s | NodeDirtyCallback: no translator in client data: %p.",
                     nodeName.asChar(), clientData);
   }
}

void CObjectSetTranslator::AttributeChangedCallback(MNodeMessage::AttributeMessage msg,
                                                    MPlug& plug, MPlug& otherPlug,
                                                    void* clientData)
{
   CObjectSetTranslator * translator = static_cast< CObjectSetTranslator* >(clientData);
   if (translator != NULL)
   {
      AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: AttributeChangedCallback %s to or from %s, attributeMessage %i, clientData %p.",
                 translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(),
                 plug.name().asChar(), otherPlug.name().asChar(), msg, clientData);
      // NOTE: no need for full refresh of set
      // translator->RequestUpdate(clientData);
      // Only refresh new member or removed member
      if (msg & (MNodeMessage::kConnectionMade | MNodeMessage::kConnectionBroken))
      {
         // If this set is active (the set aiOverride is true) but also if any set containing this set
         // is active means we need an update on membership change
         if ((msg & MNodeMessage::kIncomingDirection)
               && (msg & MNodeMessage::kOtherPlugSet)
               && translator->FindMayaPlug("aiOverride").asBool())
         {
            MString pname = plug.partialName();
            CNodeTranslator* tr;
            std::vector<CNodeTranslator*> translators;
            std::vector<CNodeTranslator*>::iterator it;
            if (pname == "dsm")
            {
               // dag node
               MObject object(otherPlug.node());
               MFnDagNode fnDag(object);
               int instanceNumber = -1;
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
                  if (translator->GetSession()->GetActiveTranslators(handle, translators) > 0)
                  {
                     for (it=translators.begin(); it!=translators.end(); it++)
                     {
                        tr = static_cast< CNodeTranslator* >(*it);
                        tr->RequestUpdate();
                     }
                  }
               }
               // Check also for shapes
               if (MStatus::kSuccess == path.extendToShape())
               {
                  CNodeAttrHandle handle(path);
                  AiMsgDebug("[mtoa.translator.ipr] %-30s | Looking for processed translators for %s.",
                      translator->GetMayaNodeName().asChar(), path.partialPathName().asChar());
                  if (translator->GetSession()->GetActiveTranslators(handle, translators) > 0)
                  {
                     for (it=translators.begin(); it!=translators.end(); it++)
                     {
                        tr = static_cast< CNodeTranslator* >(*it);
                        tr->RequestUpdate();
                     }
                  }
               }
            }
            else if (pname == "dnsm")
            {
               // dependency node
               CNodeAttrHandle handle(otherPlug.node());

               AiMsgDebug("[mtoa.translator.ipr] %-30s | Looking for processed translators for %s.%s",
                   translator->GetMayaNodeName().asChar(), MFnDependencyNode(handle.object()).name().asChar(), handle.attribute().asChar());

               if (translator->GetSession()->GetActiveTranslators(handle, translators) > 0)
               {
                  for (it=translators.begin(); it!=translators.end(); it++)
                  {
                     tr = static_cast< CNodeTranslator* >(*it);
                     tr->RequestUpdate();
                  }
               }
            }
         }
      }
      else if (msg & (MNodeMessage::kAttributeAdded | MNodeMessage::kAttributeRemoved))
      {
         // Some override attribute was added or removed
         AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Attribute added or removed on %s.",
                    translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(), plug.name().asChar());
         // Only need to update if THIS set is active
         if (translator->FindMayaPlug("aiOverride").asBool())
         {
            translator->RequestUpdate();
         }
      }
      else
      {
         AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Ignored on %s.",
                    translator->GetMayaNodeName().asChar(), translator->GetTranslatorName().asChar(), plug.name().asChar());
         // Should not be necessary, should have triggered nodeDirty callback
         // NodeDirtyCallback(plug.node(), plug, clientData);
      }
   }
   else
   {
      // No translator in client data
      AiMsgError("[mtoa.translator.ipr] AttributeChangedCallback: no translator in client data: %p.", clientData);
   }
}

/// For sets that are used by light linkers we must know it to trigger an update on the light linker
void CObjectSetTranslator::SetMembersChangedCallback(MObject &node, void *clientData)
{
   MStatus stat;
   MString nodeName = MFnDependencyNode(node).name();
   AiMsgDebug("[mtoa.translator.ipr] %-30s | SetMembersChangedCallback: client data: %p.",
               nodeName.asChar(), clientData);

   CObjectSetTranslator * translator = static_cast< CObjectSetTranslator* >(clientData);
   if (translator != NULL)
   {
      // Should be a translator for that node
      if (node != translator->GetMayaObject())
      {
         MString translatedNodeName = translator->GetMayaNodeName();
         AiMsgError("[mtoa.translator.ipr] %-30s | SetMembersChangedCallback: translator in client data is not translating the node %s that triggered the callback.",
                     nodeName.asChar(), translatedNodeName.asChar());
         return;
      }
      MFnSet fnSet(node);
      MPlug msg = fnSet.findPlug("message");
      MItDependencyGraph iter(msg,
                              MFn::kLightLink,
                              MItDependencyGraph::kDownstream,
                              MItDependencyGraph::kDepthFirst,
                                 MItDependencyGraph::kPlugLevel,
                                 &stat);
      for ( ; !iter.isDone(); iter.next() ) {
         MPlug plug = iter.thisPlug(&stat);
         MObject linker = plug.node();
         MString linkerName = MFnDependencyNode(linker).name();
         MString plugName = plug.name();
         AiMsgDebug("[mtoa.translator.ipr] %-30s | SetMembersChangedCallback: found connected light linker node %s downstream.",
                     nodeName.asChar(), plugName.asChar());
         MStringArray plugNameParts;
         MString plugShortName = plug.partialName();
         plugShortName.split('.', plugNameParts);
         MString leafAttrName = plugNameParts[plugNameParts.length()-1];
         if ((leafAttrName == "olnk") || (leafAttrName == "solk"))
         {
            AiMsgDebug("[mtoa.translator.ipr] %-30s | SetMembersChangedCallback: set of objects for the light linker %s has changed, updating the objects.",
                        nodeName.asChar(), linkerName.asChar());
            // If we got a connected light linker downstream, we need to update the set
            // FIXME: we could probably only update the added / removed node if we knew them
            translator->RequestUpdate();
         }
         else if ((leafAttrName == "llnk") || (leafAttrName == "sllk"))
         {
            CNodeAttrHandle handle(linker);
            // If we changed lights the whole list of objects in the light linker need an update
            AiMsgDebug("[mtoa.translator.ipr] %-30s | SetMembersChangedCallback: set of lights for the light linker %s has changed, requesting full update on linker.",
                        nodeName.asChar(), linkerName.asChar());
            CNodeTranslator* tr;
            std::vector<CNodeTranslator*> translators;
            std::vector<CNodeTranslator*>::iterator it;
            if (translator->GetSession()->GetActiveTranslators(handle, translators) > 0)
            {
               for (it=translators.begin(); it!=translators.end(); it++)
               {
                  tr = static_cast< CNodeTranslator* >(*it);
                  tr->RequestUpdate();
               }
            }
         }
         else
         {
            AiMsgDebug("[mtoa.translator.ipr] %-30s | SetMembersChangedCallback: connection %s is irrelevant and ignored.",
                        nodeName.asChar(), plugName.asChar());
         }
      }
   }
   else
   {
      AiMsgWarning("[mtoa.translator.ipr] %-30s | SetMembersChangedCallback: no translator in client data: %p.",
                     nodeName.asChar(), clientData);
   }
}

static void RecursiveRequestUpdate(MDagPath path, CArnoldSession *session, CNodeTranslator *translator, std::vector<CNodeTranslator*>& translators)
{   

   std::vector<CNodeTranslator*>::iterator it;
   CNodeTranslator* tr;
   // this is a Dag Path, we need to search for all translators below it in the hierarchy
   CNodeAttrHandle handle(path);
   MString pathName = path.partialPathName();
   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Looking for processed translators for %s.",
               translator->GetMayaNodeName().asChar(), "objectSet", pathName.asChar());
   if (session->GetActiveTranslators(handle, translators) > 0)
   {
      for (it=translators.begin(); it!=translators.end(); it++)
      {
         tr = static_cast< CDagTranslator* >(*it);
         tr->RequestUpdate();
      }
   }
   // Check also for shape
   if (MStatus::kSuccess == path.extendToShape())
   {
      CNodeAttrHandle handle(path);
      MString pathName = path.partialPathName();
      AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Looking for processed translators for %s.",
                  translator->GetMayaNodeName().asChar(), "objectSet", pathName.asChar());
      if (session->GetActiveTranslators(handle, translators) > 0)
      {
         for (it=translators.begin(); it!=translators.end(); it++)
         {
            tr = static_cast< CDagTranslator* >(*it);
            tr->RequestUpdate();
         }
      }
   }

   // Check for child in the hierarchy
   for (uint child = 0; (child < path.childCount()); child++)
   {
      MObject childObject = path.child(child);
      path.push(childObject);
      RecursiveRequestUpdate(path, session, translator, translators);
      path.pop(1);
   }
}

/// Update a set means update all members
// Note that this function RequestUpdate is local to this class.
// It will never be called from CNodeTranslator as we're overriding the callbacks here
void CObjectSetTranslator::RequestUpdate()
{
   // Update means all members should be updated
   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: RequestUpdate for set updates all set members.",
              GetMayaNodeName().asChar(), GetTranslatorName().asChar());
   MFnSet fnSet(GetMayaObject());
   MSelectionList list;
   fnSet.getMembers(list, false);
   MObject element;
   MDagPath path;

   CNodeTranslator* tr;
   std::vector<CNodeTranslator*> translators;
   std::vector<CNodeTranslator*>::iterator it;
   unsigned int l = list.length();
   if (l > 0)
   {
      // The code from CNodeTranslator::RequestUpdate was duplicated here
      // we're now just calling the base class. Only difference is that
      // CArnoldSession::Request is being called now, while before it
      // was called explicitely a few lines below. But does it make a difference,
      // since we're about to call RequestUpdate on several other translators ?
      CNodeTranslator::RequestUpdate();

      // loop ove all elements in the list
      for (unsigned int i=0; i<l; i++)
      {
         if (MStatus::kSuccess == list.getDagPath(i, path))
         {
            RecursiveRequestUpdate(path, GetSession(), this, translators);

         }
         else if (MStatus::kSuccess == list.getDependNode(i, element))
         {
            CNodeAttrHandle handle(element);
            MString nodeName = MFnDependencyNode(handle.object()).name().asChar();
            AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Looking for processed translators for %s.%s",
                   GetMayaNodeName().asChar(), GetTranslatorName().asChar(),
                   nodeName.asChar(), handle.attribute().asChar());
            if (GetSession()->GetActiveTranslators(handle, translators) > 0)
            {
               for (it=translators.begin(); it!=translators.end(); it++)
               {
                  tr = static_cast< CNodeTranslator* >(*it);
                  tr->RequestUpdate();
               }
            }
         }
         else
         {
            AiMsgError("[mtoa.translator.ipr] %-30s | %s: Cannot get member %i of set.",
                   GetMayaNodeName().asChar(), "objectSet", i);
         }
      }
      // removed the explicit call to CArnoldSession::RequestUpdate 
      // as this is being done by each of the calls to CNodeTranslator::RequestUpdate Above
   }
}

