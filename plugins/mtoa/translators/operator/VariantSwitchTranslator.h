#pragma once

#include "translators/NodeTranslator.h"

//--------------- VarientSwitchTranslator ------------------------------------------

class DLLEXPORT CVariantSwitchTranslator
   :  public CNodeTranslator
{
public:
    static void* creator()
    {
        return new CVariantSwitchTranslator();
    }

    static void NodeInitializer(CAbTranslator context);

    AtNode* CreateArnoldNodes()
    {
        return AddArnoldNode("switch_operator");
    }

    void Export(AtNode* atNode);
};
