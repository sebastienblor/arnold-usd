#pragma once

#include "translators/shader/ShaderTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>
#include <maya/MRenderLineArray.h>

class CUsdPreviewSurfaceTranslator
   :   public CShaderTranslator
{
public:
   CUsdPreviewSurfaceTranslator() : 
      CShaderTranslator() 
   {}

   virtual void Export(AtNode* node);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CUsdPreviewSurfaceTranslator();
   }
private:
};
