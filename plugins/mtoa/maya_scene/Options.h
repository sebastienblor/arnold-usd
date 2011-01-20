#ifndef OPTIONS_H
#define OPTIONS_H

#include "NodeTranslator.h"

class CRenderOptionsTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CRenderOptionsTranslator();}
   AtNode* Export();
   void Update(AtNode* options);
   void SetupImageOptions(AtNode* options);
};

#endif
