#include "ObjectSetTranslator.h"
#include "NodeTranslatorImpl.h"

#include <maya/MFnSet.h>
#include <maya/MDagPathArray.h>
#include <maya/MObjectSetMessage.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MSelectionList.h>
#include "utils/MtoaLog.h"

void CObjectSetTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, NULL, "ai_", false);

   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "aiOverride";
   data.shortName = "ai_override";
   data.channelBox = false;
   data.keyable = false;

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

   // do we want to fill the member translators here ?
   // FillMembersTranslators();

   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.translator]  "+GetTranslatorName()+": Maya node "+GetMayaNodeName()+"("+MFnDependencyNode(GetMayaObject()).typeName()+").");

}

/// Sets have extra specific callback addAttributeChangedCallback
void CObjectSetTranslator::AddUpdateCallbacks()
{
   CNodeTranslator::AddUpdateCallbacks();

   MStatus status;
   MCallbackId id;

   MObject object = GetMayaObject();
      
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

void CObjectSetTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   m_membersListDirty = true;

   MString plugName = plug.name();
   if ((plug.partialName()=="dsm") || (plug.partialName()=="dnsm"))
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.translator.ipr] "+GetMayaNodeName()+" | NodeChanged: ignoring plug "+ plugName);
      return;
   }
   if ((plug.partialName()=="ai_override") || FindMayaPlug("aiOverride").asBool())
   {
      // Only update if THIS set is active (not containing sets)
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.translator.ipr] "+GetMayaNodeName()+" | NodeChanged: client data is translator "+GetTranslatorName());
      
      // Get list of nodes on it

      FillMembersTranslators();
      if (m_membersTranslators.empty())
         return;

      // we want to keep NodeChanged protected. Calling the static NodeDirty callback instead
      // (it will end up calling NodeChanged)
      for (size_t i = 0; i < m_membersTranslators.size(); ++i)
         NodeDirtyCallback(node, plug, m_membersTranslators[i]);

      RequestUpdate();
   }

}

void CObjectSetTranslator::DirtyElement(CNodeTranslator *elemTr)
{
   if (elemTr == NULL)
      return;

   CNodeTranslatorImpl::DirtyOverrideSets(elemTr);
   
}
void CObjectSetTranslator::AttributeChangedCallback(MNodeMessage::AttributeMessage msg,
                                                    MPlug& plug, MPlug& otherPlug,
                                                    void* clientData)
{
   CObjectSetTranslator * translator = static_cast< CObjectSetTranslator* >(clientData);
   if (translator != NULL)
   {
      translator->m_membersListDirty = true;

      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.translator.ipr] "+translator->GetMayaNodeName()+" | "+
            translator->GetTranslatorName()+": AttributeChangedCallback "+plug.name()+" to or from "+otherPlug.name());
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
                  translator->DirtyElement(GetTranslator(path));
                 
               
               // Check also for shapes
               if (MStatus::kSuccess == path.extendToShape())
                  translator->DirtyElement(GetTranslator(path)); 
               
            }
            else if (pname == "dnsm")
               translator->DirtyElement(GetTranslator(otherPlug.node())); 
         }
      }
      else if (msg & (MNodeMessage::kAttributeAdded | MNodeMessage::kAttributeRemoved))
      {
         // Some override attribute was added or removed
         if (MtoaTranslationInfo())
            MtoaDebugLog("[mtoa.translator.ipr] "+translator->GetMayaNodeName()+" | "+ translator->GetTranslatorName()+": Attribute added or removed on "+ plug.name());
         // Only need to update if THIS set is active
         if (translator->FindMayaPlug("aiOverride").asBool())
         {
            translator->RequestUpdate();
         }
      }
      else
      {
         if (MtoaTranslationInfo())
            MtoaDebugLog("[mtoa.translator.ipr] "+translator->GetMayaNodeName()+" | "+translator->GetTranslatorName()+": Ignored on " + plug.name());
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
   
   CObjectSetTranslator * translator = static_cast< CObjectSetTranslator* >(clientData);
   if (translator != NULL)
   {
      translator->m_membersListDirty = true;

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
         
         MStringArray plugNameParts;
         MString plugShortName = plug.partialName();
         plugShortName.split('.', plugNameParts);
         MString leafAttrName = plugNameParts[plugNameParts.length()-1];
         if ((leafAttrName == "olnk") || (leafAttrName == "solk"))
         {
            // If we got a connected light linker downstream, we need to update the set
            // FIXME: we could probably only update the added / removed node if we knew them
            translator->RequestUpdate();
         }
         else if ((leafAttrName == "llnk") || (leafAttrName == "sllk"))
         {
            translator->DirtyElement(GetTranslator(linker)); 
         }
         
      }
   }
   else
   {
      AiMsgWarning("[mtoa.translator.ipr] %-30s | SetMembersChangedCallback: no translator in client data: %p.",
                     nodeName.asChar(), clientData);
   }
}
static void FillMembersDagTranslators(MDagPath path, std::vector<CNodeTranslator*> &translators)
{   
   std::vector<CNodeTranslator*>::iterator it;
   
   CNodeTranslator *elemTr = CNodeTranslator::GetTranslator(path); 
   if (elemTr) 
      translators.push_back(elemTr);

   // Check also for shape
   if (MStatus::kSuccess == path.extendToShape())
   {
      CNodeTranslator *elemTr = CNodeTranslator::GetTranslator(path); 
      if (elemTr)
         translators.push_back(elemTr);
   }

   // Check for child in the hierarchy
   for (uint child = 0; (child < path.childCount()); child++)
   {
      MObject childObject = path.child(child);
      path.push(childObject);
      FillMembersDagTranslators(path, translators);
      path.pop(1);
   }
}

void CObjectSetTranslator::FillMembersTranslators()
{
   if (!m_membersListDirty)
      return;

   m_membersTranslators.clear();

   MFnSet fnSet(GetMayaObject());
   MSelectionList list;
   fnSet.getMembers(list, false);
   MObject element;
   MDagPath path;

   std::vector<CNodeTranslator*>::iterator it;
   unsigned int l = list.length();
   if (l > 0)
   {
      
      // loop ove all elements in the list
      for (unsigned int i=0; i<l; i++)
      {
         if (MStatus::kSuccess == list.getDagPath(i, path))
         {
            FillMembersDagTranslators(path, m_membersTranslators);
         }
         else if (MStatus::kSuccess == list.getDependNode(i, element))
         {
            CNodeTranslator *elemTr = GetTranslator(element); 
            if (elemTr)
               m_membersTranslators.push_back(elemTr);
         }
         else
         {
            AiMsgError("[mtoa.translator.ipr] %-30s | %s: Cannot get member %i of set.",
                   GetMayaNodeName().asChar(), "objectSet", i);
         }
      }
   }

   m_membersListDirty = false;
}

/// Update a set means update all members
// Note that this function RequestUpdate is local to this class.
// It will never be called from CNodeTranslator as we're overriding the callbacks here
void CObjectSetTranslator::RequestUpdate()
{
   FillMembersTranslators();
   if (m_membersTranslators.empty())
      return;

   // The code from CNodeTranslator::RequestUpdate was duplicated here
   // we're now just calling the base class. Only difference is that
   // CArnoldSession::Request is being called now, while before it
   // was called explicitely a few lines below. But does it make a difference,
   // since we're about to call RequestUpdate on several other translators ?
   CNodeTranslator::RequestUpdate();

   for (size_t i = 0; i < m_membersTranslators.size(); ++i)
      m_membersTranslators[i]->RequestUpdate();


   // Update means all members should be updated
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.translator.ipr] "+GetMayaNodeName()+" | "+GetTranslatorName()+": RequestUpdate for set updates all set members.");
              
   
}

void CObjectSetTranslator::Delete()
{
   for (size_t i = 0; i < m_membersTranslators.size(); ++i)
      CNodeTranslatorImpl::DirtyOverrideSets(m_membersTranslators[i]);

   CNodeTranslator::Delete();

}
