#ifndef FLUIDTRANSLATOR_H
#define	FLUIDTRANSLATOR_H

#include "ShapeTranslator.h"

class CFluidTranslator : public CShapeTranslator{
public:
   static void NodeInitializer(CAbTranslator context);
   static void* creator() {return new CFluidTranslator();}
   virtual AtNode* CreateArnoldNodes();   
   bool RequiresMotionData() {return false;}
protected:
   virtual void Export(AtNode* fluid);
};


#endif	/* FLUIDTRANSLATOR_H */

