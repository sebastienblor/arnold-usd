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

   static void NodeInitializer(MString nodeClassName, CNodeInitContext context);
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode, AtUInt step);

protected:
   void ExportBoundingBox(AtNode* procedural);

   void ExportStandinsShaders(AtNode* procedural);
   virtual void ExportShaders();

   AtNode* ExportInstance(AtNode *instance, const MDagPath& masterInstance);
   AtNode* ExportProcedural(AtNode* procedural, bool update);

protected:
   MFnDagNode m_DagNode;
};

#endif // STANDINSTRANSLATOR_H
