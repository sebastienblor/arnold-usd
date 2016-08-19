#pragma once

#include "ShapeTranslator.h"

class CArnoldStandInsTranslator : public CShapeTranslator
{
public:
   static void* creator()
   {
      return new CArnoldStandInsTranslator();
   }
   virtual AtNode* CreateArnoldNodes();
   virtual void ProcessRenderFlags(AtNode* node);

   static void NodeInitializer(CAbTranslator context);
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode, unsigned int step);
   virtual void Update(AtNode* anode);
   virtual void UpdateMotion(AtNode* anode, unsigned int step);

protected:
   CArnoldStandInsTranslator();
   
   void ExportBoundingBox(AtNode* procedural);

   void ExportStandinsShaders(AtNode* procedural);
   virtual void ExportShaders();

   AtNode* ExportInstance(AtNode *instance, const MDagPath& masterInstance);
   AtNode* ExportProcedural(AtNode* procedural, bool update);
   AtByte ComputeOverrideVisibility();

protected:
   MFnDagNode m_DagNode;
};
