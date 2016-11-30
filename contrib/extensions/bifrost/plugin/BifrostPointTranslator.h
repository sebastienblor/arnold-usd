#pragma once

#include "translators/shader/ShaderTranslator.h"
#include "translators/NodeTranslator.h"

class BifrostPointTranslator : public CShaderTranslator
{
public:

   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* shader);

   static void* creator() { return new BifrostPointTranslator(); }
   static void NodeInitializer(CAbTranslator context);

};
