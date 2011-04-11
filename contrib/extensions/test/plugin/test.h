#include "scene/NodeTranslator.h"

// this is an example translator that overrides the translation of maya's lambert
// node to export a red arnold standard shader

class CTestTranslatorCmd
   :  public CNodeTranslator
{
public:
   const char * GetArnoldNodeType();
   void Export(AtNode*);
   void Update(AtNode*);
   static void* creator();
};
