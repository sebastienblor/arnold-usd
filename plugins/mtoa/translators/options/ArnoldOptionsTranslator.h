#ifndef ARNOLDOPTIONSTRANSLATOR_H
#define ARNOLDOPTIONSTRANSLATOR_H

#include "translators/NodeTranslator.h"

class DLLEXPORT CArnoldOptionsTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CArnoldOptionsTranslator();}
   virtual void Export(AtNode* options);
   AtNode* CreateArnoldNodes();
   void SetupImageOptions(AtNode* options);
};

#endif // ARNOLDOPTIONSTRANSLATOR_H
