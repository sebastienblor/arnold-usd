#pragma once

#include "ProceduralTranslator.h"

class CArnoldStandInsTranslator : public CProceduralTranslator
{
public:
   static void* creator()
   {
      return new CArnoldStandInsTranslator();
   }
   virtual AtNode* CreateArnoldNodes();
   
   static void NodeInitializer(CAbTranslator context)
      {CProceduralTranslator::NodeInitializer(context);}

   void Export(AtNode* anode);
   virtual bool ExportDagChildren() const;

protected:
   CArnoldStandInsTranslator() :
      CProceduralTranslator()
   {}

   void ExportStandInFilename(AtNode *node);
   void ExportAlembicParameters(AtNode *node);
   virtual void NodeChanged(MObject& node, MPlug& plug); 
   bool m_isAlembic;
   bool m_isAss;
   bool m_isUsd;
};
