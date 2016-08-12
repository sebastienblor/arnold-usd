#pragma once

#include "ShapeTranslator.h"

class CArnoldVolumeTranslator : public CShapeTranslator
{
public:
   static void* creator()
   {
      return new CArnoldVolumeTranslator();
   }
   virtual AtNode* CreateArnoldNodes();

   static void NodeInitializer(CAbTranslator context);
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode, unsigned int step);
   virtual void Update(AtNode* anode);
   virtual void UpdateMotion(AtNode* anode, unsigned int step);

protected:
   CArnoldVolumeTranslator()  :
      CShapeTranslator()
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "volume";
   }
   void ExportBoundingBox(AtNode* procedural);

   void ExportVolumeShaders(AtNode* procedural);
   virtual void ExportShaders();

   AtNode* ExportInstance(AtNode *instance, const MDagPath& masterInstance);
   AtNode* ExportVolume(AtNode* volume, bool update);

protected:
   MFnDagNode m_DagNode;
};
