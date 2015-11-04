#pragma once

#include "translators/shape/ShapeTranslator.h"
#include "translators/NodeTranslator.h"

class CXgDescriptionTranslator : public CShapeTranslator
{
public:

   AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* shape);
   virtual void Update(AtNode* shape);
   void ExportMotion(AtNode*, unsigned int);
   virtual bool DependsOnExportCamera() {return true;}

   static void* creator()
   {
      return new CXgDescriptionTranslator();
   }
   static void NodeInitializer(CAbTranslator context);

private:

   AtNode* ExportShaders(AtNode* instance);
};
