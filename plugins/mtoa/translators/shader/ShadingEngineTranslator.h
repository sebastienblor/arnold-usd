#pragma once

#include "ShaderTranslator.h"

#include "maya/MPlugArray.h"

class DLLEXPORT CShadingEngineTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CShadingEngineTranslator();}
   virtual void Export(AtNode* shader);
   AtNode* CreateArnoldNodes();
   static void NodeInitializer(CAbTranslator context);


protected:

   CShadingEngineTranslator() :
      CNodeTranslator()
   {}
   virtual ~CShadingEngineTranslator();
   virtual void NodeChanged(MObject& node, MPlug& plug);
   virtual void Init();

   virtual void AddUpdateCallbacks();

   static void AttributeChangedCallback(MNodeMessage::AttributeMessage msg,
                                        MPlug& plug, MPlug& otherPlug,
                                        void* clientData);

private:
   // internal use only : do not override it
   virtual void CreateImplementation();

};
