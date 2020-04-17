#pragma once

#include "translators/shape/ProceduralTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>
#include <maya/MRenderLineArray.h>

class CGpuCacheTranslator
   :   public CProceduralTranslator
{
public:
   CGpuCacheTranslator() : 
      CProceduralTranslator() 
   {}

   void Export(AtNode* anode);
   virtual void ExportMotion(AtNode* shape);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   virtual void AddUpdateCallbacks();
   static void timeChangedCallback(void* clientData);
   static void* creator()
   {
      return new CGpuCacheTranslator();
   }
   virtual void NodeChanged(MObject& node, MPlug& plug);

private:
   virtual void ExportProcedural(AtNode* shape);
   virtual void ExportShaders();
};
