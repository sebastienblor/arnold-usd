#pragma once

#include "translators/shader/ShaderTranslator.h"
#include "translators/NodeTranslator.h"


class CBfAeroMaterialTranslator : public CShaderTranslator
{
public:

   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* shader);

   static void* creator()
   {
      return new CBfAeroMaterialTranslator();
   }
   static void NodeInitializer(CAbTranslator context);

private:

   void ExportBifrostShader();
   
  void ExportFloatGradient(MPlug plug, AtNode* node, const char* paramName);
    void ExportRGBGradient(MPlug plug, AtNode* node, const char* paramName);

};
