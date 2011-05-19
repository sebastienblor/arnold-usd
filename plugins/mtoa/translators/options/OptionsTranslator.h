#ifndef OPTIONSTRANSLATOR_H
#define OPTIONSTRANSLATOR_H

#include "translators/NodeTranslator.h"

class DLLEXPORT COptionsTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new COptionsTranslator();}
   virtual void Export(AtNode* options);
   AtNode* CreateArnoldNodes();
   void SetupImageOptions(AtNode* options);
};

#endif // OPTIONSTRANSLATOR_H
