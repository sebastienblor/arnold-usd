#pragma once

#include "DagTranslator.h"
#include <maya/MNodeMessage.h>

class DLLEXPORT CObjectSetTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CObjectSetTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   static void NodeInitializer(CAbTranslator context);
   virtual void RequestUpdate();

protected:
   CObjectSetTranslator() :
      CNodeTranslator(),
        m_membersListDirty(true)
   {}
   virtual ~CObjectSetTranslator()
   {}
   virtual void NodeChanged(MObject& node, MPlug& plug);
   virtual void AddUpdateCallbacks();
   virtual void Delete();
   void DirtyElement(CNodeTranslator *tr);
   static void AttributeChangedCallback(MNodeMessage::AttributeMessage msg,
                                        MPlug& plug, MPlug& otherPlug,
                                        void* clientData);
   static void SetMembersChangedCallback(MObject &node, void *clientData);
private:
   void FillMembersTranslators();

   std::vector<CNodeTranslator *> m_membersTranslators;
   bool m_membersListDirty;



   
};
