#pragma once

#include "translators/NodeTranslator.h"

class DLLEXPORT CImagerTranslator
   :  public CNodeTranslator
{
public:
   static void* creator()
   {
      return new CImagerTranslator();
   }

   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   void Export(AtNode* atNode);
   virtual void NodeChanged(MObject& node, MPlug& plug);
   void ProcessImagerChanges();

private:
   MCallbackId m_idleCb;
};