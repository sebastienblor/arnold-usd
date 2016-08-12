#pragma once

#include "GeometryTranslator.h"

class CArnoldProceduralTranslator : public CGeometryTranslator
{
public:
   static void* creator()
   {
      return new CArnoldProceduralTranslator();
   }
   virtual AtNode* CreateArnoldNodes();

   static void NodeInitializer(CAbTranslator context);
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode, unsigned int step);
   virtual void Update(AtNode* anode);
   virtual void UpdateMotion(AtNode* anode, unsigned int step);

protected:
   CArnoldProceduralTranslator()  :
      CGeometryTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "procedural";
   }
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
