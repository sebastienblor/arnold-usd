#pragma once

#include "translators/shape/ShapeTranslator.h"

namespace XGenArnold
{
   class ProceduralWrapper;
}

class CXgDescriptionTranslator : public CShapeTranslator
{
public:

   AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* shape);
   void ExportMotion(AtNode*);
   virtual bool DependsOnExportCamera() {return true;}

   static void* creator()
   {
      return new CXgDescriptionTranslator();
   }
   static void NodeInitializer(CAbTranslator context);
   virtual void ExportShaders();

   virtual void RequestUpdate();
   virtual void Delete();
private:

   AtNode* ExportRootShader(AtNode *);
   void ExpandProcedural();

   std::vector<bool> m_exportedSteps;
   std::vector<XGenArnold::ProceduralWrapper*> m_expandedProcedurals;
};
