#pragma once

#include "ShapeTranslator.h"
#include "maya/MDagPathArray.h"

class CCurveCollectorTranslator
   :   public CShapeTranslator
{
public:
   CCurveCollectorTranslator() : CShapeTranslator(), m_sampleRate(5)
   {}
   virtual ~CCurveCollectorTranslator(){}

   virtual void Export(AtNode* curve);
   virtual void ExportMotion(AtNode* curve);
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   static void* creator()
   {
      return new CCurveCollectorTranslator();
   }
   virtual bool ExportDagChildren() const {return false;}
   virtual void Init();
   virtual void AddUpdateCallbacks();

protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
   
private:
   void ComputeCurvesList(MDagPathArray &pathArray);

   MDagPathArray m_curveDagPaths;
   int m_sampleRate;
};