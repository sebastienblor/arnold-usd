#pragma once

#include "translators/shape/ShapeTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>
#include <maya/MRenderLineArray.h>

class CGpuCacheTranslator
   :   public CShapeTranslator
{
public:
   CGpuCacheTranslator() : 
      CShapeTranslator() 
   {}

   virtual void Export(AtNode* curve);
   virtual void ExportMotion(AtNode* curve);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CGpuCacheTranslator();
   }
   
private:                         

};
