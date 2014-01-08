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

private:
    void ExportFloatGradient(MPlug plug, AtNode* node, const char* paramName);
    void ExportRGBGradient(MPlug plug, AtNode* node, const char* paramName);
};
