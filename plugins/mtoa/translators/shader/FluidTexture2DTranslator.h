#pragma once

#include "ShaderTranslator.h"

class CFluidTexture2DTranslator : public CShaderTranslator
{
public:
   static void* creator()
   {
      return new CFluidTexture2DTranslator();
   }
   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* fluid2d);
};
