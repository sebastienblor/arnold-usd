#pragma once

#include "ShapeTranslator.h"

class CFluidTranslator : public CShapeTranslator{
public:
   static void NodeInitializer(CAbTranslator context);
   static void* creator() {return new CFluidTranslator();}
   virtual AtNode* CreateArnoldNodes();   
   bool RequiresMotionData() {return IsMotionBlurEnabled(MTOA_MBLUR_OBJECT) && (!m_isStatic);}
protected:
   virtual void Export(AtNode* fluid);
   virtual void ExportMotion(AtNode* fluid, unsigned int step);
   void ExportFloatGradient(MPlug plug, AtNode* node, const char* paramName);
   void ExportRGBGradient(MPlug plug, AtNode* node, const char* paramName);
};
