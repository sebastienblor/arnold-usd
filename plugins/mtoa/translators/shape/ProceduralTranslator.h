#pragma once

#include "ShapeTranslator.h"

class CArnoldProceduralTranslator : public CShapeTranslator
{
public:
   static void* creator()
   {
      return new CArnoldProceduralTranslator();
   }
   virtual AtNode* CreateArnoldNodes();

   static void NodeInitializer(CAbTranslator context);
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode);

protected:
   CArnoldProceduralTranslator() :
      CShapeTranslator()
   {}
   
   void ExportBoundingBox(AtNode* procedural);

   void ExportStandinsShaders(AtNode* procedural);
   virtual void ExportShaders();
   virtual void NodeChanged(MObject& node, MPlug& plug);

   AtNode* ExportInstance(AtNode *instance, const MDagPath& masterInstance);
   AtNode* ExportProcedural(AtNode* procedural, bool update);
   AtByte ComputeOverrideVisibility();

   static void IdleCallback(void *data);

protected:
   MFnDagNode m_DagNode;



};
