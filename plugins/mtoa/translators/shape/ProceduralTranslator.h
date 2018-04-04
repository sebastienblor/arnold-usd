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
   virtual void NodeChanged(MObject& node, MPlug& plug); 

   MPlug FindProceduralPlug(const char *attrName);

protected:
   MFnDagNode m_DagNode;
   
   bool m_attrChanged;

};
