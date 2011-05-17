#ifndef OPTIONS_H
#define OPTIONS_H

#include "NodeTranslator.h"

class DLLEXPORT CRenderOptionsTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CRenderOptionsTranslator();}
   virtual void Export(AtNode* options);
   AtNode* CreateArnoldNodes();
   void SetupImageOptions(AtNode* options);
};

#endif // OPTIONS_H
