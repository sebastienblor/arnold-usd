#ifndef OBJECT_SET_TRANSLATOR_H
#define OBJECT_SET_TRANSLATOR_H

#include "NodeTranslator.h"
#include <maya/MNodeMessage.h>

class DLLEXPORT CObjectSetTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CObjectSetTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   static void NodeInitializer(CAbTranslator context);

protected:
   CObjectSetTranslator() :
      CNodeTranslator()
   {}
   virtual ~CObjectSetTranslator()
   {}
   virtual void AddUpdateCallbacks();
   static void NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData);
   static void AttributeChangedCallback(MNodeMessage::AttributeMessage msg,
                                        MPlug& plug, MPlug& otherPlug,
                                        void* clientData);
   virtual void RequestUpdate(void *clientData);
};


#endif // OBJECT_SET_TRANSLATOR_H
