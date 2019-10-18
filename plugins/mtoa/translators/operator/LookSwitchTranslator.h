#pragma once

#include "translators/NodeTranslator.h"

//--------------- LookSwitchTranslator ------------------------------------------

class DLLEXPORT CLookSwitchTranslator
   :  public CNodeTranslator
{
public:
    static void* creator()
    {
        return new CLookSwitchTranslator();
    }

    static void NodeInitializer(CAbTranslator context);

    AtNode* CreateArnoldNodes()
    {
        return AddArnoldNode("switch_operator");
    }

    void Export(AtNode* atNode);
  protected:
    virtual void NodeChanged(MObject& node, MPlug& plug);
};
