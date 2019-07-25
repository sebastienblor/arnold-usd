#pragma once

#include "translators/shape/ProceduralTranslator.h"

#include <maya/MFnMesh.h>
#include <maya/MMeshIntersector.h>
#include <maya/MRenderLineArray.h>

class CBifShapeTranslator
   :   public CProceduralTranslator
{
public:
   CBifShapeTranslator() : 
      CProceduralTranslator() 
   {}

   virtual void Export(AtNode* curve);
   virtual void ExportMotion(AtNode* curve);
   virtual bool RequiresMotionData(); 
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CBifShapeTranslator();
   }
   virtual void ExportShaders(AtNode* shape);

private:

   void GetSerializedData(MDoubleArray& array);
};
