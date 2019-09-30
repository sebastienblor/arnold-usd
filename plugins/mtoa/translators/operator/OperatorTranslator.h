#pragma once

#include "translators/NodeTranslator.h"

//--------------- OperatorTranslator ------------------------------------------

class DLLEXPORT COperatorTranslator
   :  public CNodeTranslator
{
public:
   static void* creator()
   {
      return new COperatorTranslator();
   }

   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   void Export(AtNode* atNode);

  protected:
    void ExportAssignedShaders(AtNode *shader);
    MStringArray WalkOutputs(MObject obj);
    virtual void NodeChanged(MObject& node, MPlug& plug);
};
