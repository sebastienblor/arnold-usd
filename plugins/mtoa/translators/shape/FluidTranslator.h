#pragma once

#include "ShapeTranslator.h"

class CFluidTranslator : public CShapeTranslator{
public:
   static void NodeInitializer(CAbTranslator context);
   static void* creator() {return new CFluidTranslator();}
   virtual AtNode* CreateArnoldNodes();   
   bool RequiresMotionData() {return true;}
protected:
   virtual void Export(AtNode* fluid);
   virtual void ExportMotion(AtNode* fluid, unsigned int step);
};
