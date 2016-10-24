#include "translators/NodeTranslator.h"

// this is an example translator that overrides the translation of maya's lambert
// node to export a red arnold standard shader

class CTestTranslatorCmd
   :  public CNodeTranslator
{
public:
   AtNode* CreateArnoldNodes();
   void Export(AtNode*);
   static void* creator();
};
