#pragma once

#include "translators/shape/ShapeTranslator.h"
#include "translators/NodeTranslator.h"

namespace XgArnoldInternal
{
   class XgSplineProcedural;
}

// Translate xgmSplineDescription shape to xgenSpline_procedural
//
class CXgSplineDescriptionTranslator : public CShapeTranslator
{
public:
   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* procedural);
   virtual void ExportMotion(AtNode* procedurald);

   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CXgSplineDescriptionTranslator();
   }
   virtual void Delete();
   virtual void RequestUpdate();

protected:
   virtual void PostExport(AtNode *node);
private:
   void ExportSplineData(AtNode* procedural, unsigned int step);
   void ExpandProcedural();
   std::vector<XgArnoldInternal::XgSplineProcedural*> m_expandedProcedurals;
};
