#pragma once

#include "ShapeTranslator.h"

class CArnoldProceduralTranslator : public CShapeTranslator
{
public:

   static void* creator()
   {
      return new CArnoldProceduralTranslator();
   }
   static void NodeInitializer(CAbTranslator context);

//------- Derived from CNodeTranslator   
   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* anode);
   virtual void ExportMotion(AtNode* anode);

protected:
   CArnoldProceduralTranslator() :
      CShapeTranslator()
   {}
   
   // Method used to set the min/max parameters for this procedural
   //void ExportBoundingBox(AtNode* procedural);
   // Export this node as an instance (if IsMasterInstance() is false)
   void ExportInstance(AtNode *instance);
   
//-------- Derived from CDagTranslator
   // Export the shaders for this Geometry
   virtual void ExportShaders();

   virtual void NodeChanged(MObject& node, MPlug& plug);


};
