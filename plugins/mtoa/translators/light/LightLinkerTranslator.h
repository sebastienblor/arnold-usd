#pragma once

#include "translators/DagTranslator.h"
#include <maya/MNodeMessage.h>

class DLLEXPORT CLightLinkerTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CLightLinkerTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();

protected:
   CLightLinkerTranslator() :
      CNodeTranslator()
   {}
   virtual ~CLightLinkerTranslator()
   {}
   virtual void AddUpdateCallbacks();
   static void NodeDirtyCallback(MObject &node, MPlug &plug, void *clientData);
   static void AttributeChangedCallback(MNodeMessage::AttributeMessage msg,
                                        MPlug& plug, MPlug& otherPlug,
                                        void* clientData);
   virtual void RequestUpdate();

private:
   unsigned int GetMembers(MSelectionList &list, const MPlug &plug, bool doLights, bool doObjects);
};
