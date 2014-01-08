#pragma once

#include "ShapeTranslator.h"

class CXgDescriptionTranslator : public CShapeTranslator
{
public:

   AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* shape);
   virtual void Update(AtNode* shape);
   void ExportMotion(AtNode*, unsigned int);

   static void* creator()
   {
      return new CXgDescriptionTranslator();
   }
   static void NodeInitializer(CAbTranslator context);

private:

   AtNode* ExportShaders(AtNode* instance);
};
