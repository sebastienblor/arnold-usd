#ifndef STANDINSTRANSLATOR_H
#define STANDINSTRANSLATOR_H

#include "GeometryTranslator.h"

class CArnoldStandInsTranslator : public CGeometryTranslator
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
   virtual void UpdateMotion(AtNode* anode, AtUInt step);

protected:
   CArnoldStandInsTranslator()  :
      CGeometryTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "procedural";
   }
   void ExportBoundingBox(AtNode* procedural);

   void ExportStandinsShaders(AtNode* procedural);
   virtual void ExportShaders();

   AtNode* ExportInstance(AtNode *instance, const MDagPath& masterInstance);
   AtNode* ExportProcedural(AtNode* procedural, bool update);

protected:
   MFnDagNode m_DagNode;
};

#endif // STANDINSTRANSLATOR_H
