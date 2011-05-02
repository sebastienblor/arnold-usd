#ifndef OPTIONS_H
#define OPTIONS_H

#include "NodeTranslator.h"

class DLLEXPORT CRenderOptionsTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new CRenderOptionsTranslator();}
   void Export(AtNode* options);
   const char* GetArnoldNodeType();
   AtNode* CreateArnoldNode();
   void SetupImageOptions(AtNode* options);
};

#endif // OPTIONS_H
