#pragma once

#include "ShapeTranslator.h"

/** 
 *   Base Translator class for nodes exporting a procedural
 *
 **/ 
class DLLEXPORT CProceduralTranslator : public CShapeTranslator
{
public:
   static void* creator()
   {
      return new CProceduralTranslator();
   }
   virtual AtNode* CreateArnoldNodes();
   virtual void ProcessRenderFlags(AtNode* node);

   static void NodeInitializer(CAbTranslator context);
   void Export(AtNode* anode);
   void ExportMotion(AtNode* anode);
   
protected:
   CProceduralTranslator() :
      CShapeTranslator()
   {}
   
   virtual void ExportShaders();

   AtNode* ExportInstance(AtNode *instance, const MDagPath& masterInstance);
   AtNode* ExportProcedural(AtNode* procedural);
   AtByte ComputeOverrideVisibility();
   virtual void RequestUpdate();

protected:
   MFnDagNode m_DagNode;
};
